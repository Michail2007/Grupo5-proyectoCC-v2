# Configuración del Código Arduino para Satélite

## Requisitos de Hardware
1. Arduino (Uno, Nano, o compatible)
2. Sensor DHT11 (temperatura y humedad)
3. LED (conectado al pin 12)
4. Cables para conexión serial (pines 10 y 11)

## Requisitos de Software
1. Arduino IDE
2. Bibliotecas necesarias:
   - DHT sensor library (por Adafruit)
   - SoftwareSerial (incluida en Arduino IDE)

## Conexiones
1. Sensor DHT11:
   - Pin de datos -> Pin 2 del Arduino
   - VCC -> 5V
   - GND -> GND

2. LED:
   - Ánodo (+) -> Pin 12 del Arduino
   - Cátodo (-) -> GND (con resistencia de 220Ω)

3. Comunicación Serial:
   - RX -> Pin 10 del Arduino
   - TX -> Pin 11 del Arduino

## Instalación de Bibliotecas
1. Abrir Arduino IDE
2. Ir a Herramientas -> Administrar Bibliotecas
3. Buscar e instalar:
   - "DHT sensor library" de Adafruit
   - "Adafruit Unified Sensor" (dependencia de DHT library)

## Verificación del Código
El código incluye:
- Configuración de pines ✓
- Inicialización de sensores ✓
- Manejo de errores ✓
- Detección de valores fuera de rango ✓
- Sistema de heartbeat ✓
- Comunicación serial bidireccional ✓

## Comandos Disponibles
- `i`: Iniciar envío de datos
- `p`: Pausar envío de datos
- `r`: Reanudar envío de datos

## Formato de Mensajes
1. Datos normales: `1:humedad:temperatura`
   - humedad: valor * 100 (entero)
   - temperatura: valor * 100 (entero)
2. Errores:
   - `3:1`: Error de lectura del sensor DHT
   - `3:2`: Datos fuera de rango
3. Heartbeat: `g`

## Verificación Pre-Ejecución
1. Comprobar voltaje de alimentación
2. Verificar conexiones del sensor DHT11
3. Confirmar que el LED está conectado correctamente
4. Revisar las conexiones RX/TX para la comunicación serial

## Solución de Problemas
1. Si no se reciben datos:
   - Verificar alimentación del sensor
   - Comprobar conexiones de pines
   - Revisar puerto serial seleccionado

2. Si los datos son erróneos:
   - Verificar que no haya interferencia electromagnética
   - Comprobar distancia del cableado
   - Verificar calidad de soldaduras si las hay

3. Si el LED no parpadea:
   - Verificar polaridad
   - Comprobar resistencia
   - Verificar conexión al pin 12
