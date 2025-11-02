# Guía de Instalación y Uso de Live Share

## Requisitos Previos

1. Visual Studio Code instalado en tu computadora
2. Una cuenta de Microsoft o GitHub para autenticación

## Instalación

1. Abre Visual Studio Code
2. Ve a la pestaña de extensiones (Ctrl+Shift+X)
3. Busca "Live Share"
4. Instala el paquete de extensiones "Live Share Extension Pack" que incluye:
   - Live Share (colaboración en tiempo real)
   - Live Share Audio (chat de voz)
   - Chat (chat de texto)

## Configuración Inicial

1. Una vez instalado, haz clic en el botón "Live Share" en la barra inferior azul
2. Inicia sesión con tu cuenta de Microsoft o GitHub cuando se te solicite
3. Permite los permisos necesarios para la extensión

## Cómo Compartir una Sesión

1. Inicia Live Share de una de estas formas:
   - Haz clic en el botón "Live Share" en la barra inferior
   - Presiona Ctrl+Shift+P y busca "Live Share: Start Collaboration Session"
   - Utiliza el comando rápido Ctrl+Alt+Shift+P

2. VS Code generará automáticamente un enlace de invitación
3. Comparte este enlace con tus colaboradores
4. El enlace se copia automáticamente al portapapeles

## Cómo Unirse a una Sesión

1. Recibe el enlace de invitación de un colaborador
2. Abre el enlace en tu navegador
3. Selecciona "Abrir en VS Code"
4. Autoriza la conexión si se te solicita

## Funcionalidades Principales

### Para el Anfitrión
- Puedes compartir terminales (Click derecho en la terminal → Hacer compartible)
- Puedes compartir servidores locales
- Control de permisos de los participantes:
  - Modo solo lectura
  - Permitir/denegar escritura
  - Permitir/denegar depuración

### Para los Participantes
- Ver y editar código en tiempo real
- Seguir al anfitrión (cursor y cambios de archivo)
- Utilizar terminales compartidas
- Participar en sesiones de depuración

## Solución de Problemas Comunes

1. Si no puedes conectarte:
   - Verifica tu conexión a internet
   - Asegúrate de estar autenticado
   - Intenta cerrar y volver a abrir VS Code

2. Si no ves los cambios en tiempo real:
   - Verifica que tienes permisos de escritura
   - Intenta recargar la ventana (F1 → Reload Window)

## Recomendaciones

1. Usa auriculares para evitar el eco en las llamadas
2. Mantén actualizada la extensión
3. Cierra las sesiones cuando termines de trabajar
4. Usa el chat de texto para comunicación que no requiera voz
5. Define roles y permisos antes de comenzar la sesión

## Enlaces Útiles

- [Documentación oficial de VS Live Share](https://docs.microsoft.com/visualstudio/liveshare/)
- [FAQ de Live Share](https://docs.microsoft.com/visualstudio/liveshare/faq)
- [Reporte de problemas](https://github.com/MicrosoftDocs/live-share/issues)
