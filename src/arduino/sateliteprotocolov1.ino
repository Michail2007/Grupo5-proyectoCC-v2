// Sketch integrado: DHT11 + SoftwareSerial (pines 10/11) + Servo + Ultrasonidos + Protocolo 1..5
// Protocolo aplicado:
// 1: datos humedad+temperatura -> 1:<hum_x100>:<temp_x100>
// 2: datos distancia          -> 2:<dist_mm>
// 3: datos error general      -> 3:<subcodigo>
// 4: error temp/hum           -> 4:<subcodigo>  (4:1 = NaN lectura, 4:2 = fuera de rango)
// 5: error distancia          -> 5:<subcodigo>  (5:1 = timeout/sin eco, 5:2 = fuera de rango)
// Nota: cuando el sensor de distancia no responde, este sketch ahora envía la línea literal "e".

#include <DHT.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// ----------------- Configuración sensores / pines -----------------
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// SoftwareSerial: RX, TX (no tocar si dependes del otro Arduino)
SoftwareSerial satSerial(10, 11); // RX=10, TX=11

const uint8_t LEDPIN = 12;   // LED indicador
bool sending = false;        // empieza sin enviar
unsigned long lastSend = 0;  // temporizador para envío cada 2000 ms

// Umbrales para detectar lecturas absurdas
const float HUM_MIN = 0.0;
const float HUM_MAX = 100.0;
const float TEMP_MIN = -40.0;
const float TEMP_MAX = 80.0;

// Servo + pot
const uint8_t servoPin = 8;
const int potPin = A0; // potenciómetro en A0
Servo motor;

// Ultrasonidos (no usar 10/11)
const uint8_t trigPin = 3;
const uint8_t echoPin = 4;
const unsigned long PULSE_TIMEOUT_US = 30000UL; // 30 ms timeout para pulseIn
const int DIST_MAX_MM = 4000; // distancia máxima plausible (mm)

// ----------------- Funciones utilitarias -----------------
/**
 * sendPacket
 * Envía por satSerial y por Serial (debug) un paquete con el formato tipo:payload
 * type  -> número del protocolo (1..5)
 * payload -> texto ya formateado sin '\n'
 */
void sendPacket(uint8_t type, const String &payload) {
  String msg = String(type) + ":" + payload;
  satSerial.println(msg);
  Serial.println("-> Enviado (sat): " + msg);
}

/**
 * pingSensor
 * Realiza una medición con el sensor ultrasónico y devuelve la distancia en mm.
 * Devuelve:
 *  - distancia en mm (>0) si se midió correctamente
 *  - 0 si timeout / sin eco
 */
int pingSensor(uint8_t trig, uint8_t echo, unsigned long timeoutMicros) {
  // Generar pulso
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Espera por el pulso en echo (con timeout)
  unsigned long duration = pulseIn(echo, HIGH, timeoutMicros); // microsegundos
  if (duration == 0) {
    // timeout -> sin eco
    return 0;
  }

  // Velocidad del sonido ≈ 343 m/s => 0.343 mm/µs
  // Distancia (mm) = duration_us * 0.343 / 2
  // Usamos float para precisión y devolvemos int
  float dist_mm_f = (float)duration * 0.343f / 2.0f;
  int dist_mm = (int)round(dist_mm_f);
  return dist_mm;
}

// ----------------- Setup -----------------
void setup() {
  Serial.begin(9600);
  satSerial.begin(9600);
  dht.begin();

  pinMode(LEDPIN, OUTPUT);

  // Ultrasonidos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Servo
  motor.attach(servoPin);

  delay(1000);
  Serial.println("Satelite listo, esperando comandos...");
  satSerial.println("LISTO"); // mensaje inicial a Tierra/PC
}

// ----------------- Loop principal -----------------
void loop() {
  // 1) Actualizar servo con potenciómetro continuamente (sin bloquear)
  int potVal = analogRead(potPin);
  int angle = map(potVal, 0, 1023, 180, 0); // como en tu sketch original
  motor.write(angle);

  // 2) Leer comandos entrantes por satSerial (p, r, i)
  if (satSerial.available()) {
    String cmd = satSerial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();
    if (cmd.length() > 0) {
      Serial.print("Cmd recibido: "); Serial.println(cmd);

      if (cmd == "p") {
        sending = false;
        Serial.println("-> PAUSADO");
        satSerial.println("ACK:P");
      } else if (cmd == "r") {
        sending = true;
        Serial.println("-> REANUDADO");
        satSerial.println("ACK:R");
      } else if (cmd == "i") {
        sending = true;
        Serial.println("-> INICIADO");
        satSerial.println("ACK:I");
      } else {
        satSerial.print("ACK:?"); satSerial.println(cmd);
      }
    }
  }

  // 3) Enviar datos o heartbeat cada 2000 ms
  unsigned long now = millis();
  if (now - lastSend >= 2000UL) {
    if (sending) {
      // --- Leer DHT ---
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      if (isnan(h) || isnan(t)) {
        // Error lectura DHT -> enviar código de error tipo 4:1
        Serial.println("ERROR lectura DHT! -> enviando 4:1");
        sendPacket(4, "e"); // 4:1 = error lectura (NaN)
      } else {
        // Detectar valores fuera de rango
        if (h < HUM_MIN || h > HUM_MAX || t < TEMP_MIN || t > TEMP_MAX) {
          Serial.println("Lectura fuera de rango DHT -> enviando 4:e");
          sendPacket(4, "e"); // 4:e = datos fuera de rango (temp/hum)
        } else {
          // Lectura válida: enviar tipo 1 (hum y temp convertidos *100)
          int hi = (int)round(h * 100.0f);
          int ti = (int)round(t * 100.0f);
          String payload1 = String(hi) + ":" + String(ti);
          sendPacket(1, payload1); // 1:hi:ti
          // Indicador LED corto
          digitalWrite(LEDPIN, HIGH);
          delay(80);
          digitalWrite(LEDPIN, LOW);
        }
      }

      // --- Medir distancia y enviar tipo 2 o 'e' si no responde ---
      int dist_mm = pingSensor(trigPin, echoPin, PULSE_TIMEOUT_US);
      if (dist_mm == 0) {
        // Timeout / sin eco -> enviar la letra 'e' (como pediste)
        satSerial.println("5:e");
        Serial.println("-> Enviado (sat): e");
      } else {
        if (dist_mm > DIST_MAX_MM) {
          // Fuera de rango -> 5:2
          Serial.print("Ultrasonidos: distancia fuera de rango (");
          Serial.print(dist_mm);
          Serial.println(" mm) -> enviando 5:e");
          sendPacket(5, "e");
        } else {
          // Valor válido -> enviar 2:dist_mm
          sendPacket(2, String(dist_mm));
        }
      }

    } else {
      // sending == false -> heartbeat (mantenemos 'g' como antes)
      satSerial.println("g");
      Serial.println("Heartbeat: g");
    }
    lastSend = now;
  }

  // NO delays largos aquí (servo y comprobaciones continuas)
}







