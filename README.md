Solución IoT para Configuración Dinámica WiFi con ESP32
Integrantes
Juan Camilo Gómez Bayona
Daniel Sanchez Sotelo
Jeronimo Infante Vega
1. Introducción

En el desarrollo de soluciones IoT modernas, uno de los principales retos consiste en permitir la configuración inalámbrica de dispositivos sin necesidad de recompilar o reprogramar el firmware cada vez que cambia la red WiFi.

Para solucionar esta problemática, se diseñó e implementó una solución IoT basada en el microcontrolador ESP32 utilizando el framework Arduino. La solución desarrollada permite configurar dinámicamente las credenciales WiFi mediante una interfaz web moderna y amigable accesible desde dispositivos móviles o computadores.

El sistema implementa:

Modo Access Point (AP)
Portal cautivo local
Reconexión automática
Persistencia de credenciales
Endpoints HTTP
Interfaz web responsive
Mecanismo de restablecimiento

La implementación fue validada funcionalmente mediante pruebas reales utilizando hotspot móvil, navegador web y monitoreo serial.

2. Objetivos
2.1 Objetivo General

Diseñar e implementar una solución IoT basada en ESP32 que permita la configuración dinámica de redes WiFi sin necesidad de reprogramación del dispositivo.

2.2 Objetivos Específicos
Implementar un portal web de configuración WiFi.
Permitir almacenamiento persistente de credenciales.
Implementar reconexión automática.
Desarrollar una interfaz amigable y responsive.
Implementar endpoints REST para administración.
Diseñar un mecanismo de restauración de configuración.
Validar el sistema mediante pruebas funcionales reales.
Documentar completamente la solución.
3. Descripción General del Sistema

La solución desarrollada permite que el ESP32 opere bajo dos modos:

3.1 Modo AP (Access Point)

Cuando el dispositivo no posee credenciales almacenadas:

El ESP32 crea una red WiFi propia.
El usuario se conecta a ella.
Se accede al portal cautivo mediante:
http://192.168.4.1

La red creada por el dispositivo es:

ESP32_Setup
3.2 Modo STA (Station)

Cuando existen credenciales guardadas:

El ESP32 se conecta automáticamente a la red WiFi.
Obtiene una IP dinámica.
Mantiene disponible el servidor web.
Permite acceder nuevamente mediante la nueva IP asignada.
4. Arquitectura del Sistema
4.1 Componentes Utilizados
Componente	Función
ESP32	Microcontrolador principal
WiFi.h	Gestión WiFi
WebServer.h	Servidor HTTP
DNSServer.h	Portal cautivo
Preferences.h	Memoria persistente
HTML/CSS/JS	Interfaz web
5. Funcionamiento del Sistema
5.1 Inicialización

Al encender el ESP32:

Se verifica la existencia de credenciales almacenadas.
Si no existen:
inicia en modo AP.
Si existen:
intenta reconectarse automáticamente.
5.2 Configuración WiFi

El usuario:

Accede a la red del ESP32.
Abre el portal web.
Escanea redes disponibles.
Selecciona SSID.
Ingresa contraseña.
Presiona conectar.

El ESP32:

Guarda las credenciales.
Reinicia automáticamente.
Se conecta a la red configurada.
6. Evidencia Experimental
6.1 Portal Web Inicial

El ESP32 inicia en modo AP y expone una interfaz web moderna accesible desde:

192.168.4.1
Evidencia
Imagen: PaginaWebHttp.jpg

Características observadas:

Interfaz responsive
Diseño glassmorphism
Escaneo de redes WiFi
Configuración dinámica
Botón de restablecimiento
6.2 Conexión del ESP32 al Hotspot

Luego de configurar el SSID y contraseña, el ESP32 se conectó exitosamente al hotspot móvil.

Evidencia
Imagen: Hotspot.jpg

La imagen demuestra:

El ESP32 conectado al hotspot.
Asignación exitosa de red.
Comunicación activa.
6.3 Validación por Monitor Serial

Desde el monitor serial se verificó:

Reconexión automática.
Obtención de IP dinámica.
Inicio del servidor HTTP.
Evidencia
Imagen: ConexionWifi.jpg

Información obtenida:

WiFi conectado
IP: 10.254.186.116
Modo STA activo
Servidor HTTP iniciado
6.4 Acceso mediante Nueva IP

Después de conectarse al hotspot:

El ESP32 dejó de utilizar 192.168.4.1.
El servidor quedó disponible en la nueva IP asignada.
Evidencia
Imagen: IpNueva.jpeg

Esto demuestra:

Funcionamiento correcto del modo STA.
Persistencia de endpoints.
Operación dentro de red local.
7. Memoria No Volátil

La persistencia de credenciales se implementó mediante:

Preferences.h

Se almacenan:

SSID
Contraseña

La información permanece incluso después de:

Reinicios
Desconexión de energía
Reconexión USB
8. Mecanismo de Restablecimiento
8.1 Reset desde Interfaz Web

El sistema implementa:

/reset

Permitiendo:

Borrar credenciales
Reiniciar el ESP32
Volver a modo AP
8.2 Reset Físico

Se implementó un botón conectado al:

GPIO 4

Si el botón permanece presionado por 3 segundos:

Se elimina la configuración WiFi.
El dispositivo reinicia.
El ESP32 vuelve a modo AP.
9. Documentación de Endpoints
9.1 GET /
Campo	Valor
URL	/
Método	GET
Descripción	Retorna la interfaz web
Headers	Ninguno
Query Params	Ninguno
Payload	Ninguno
Respuesta	HTML
9.2 GET /scan
Campo	Valor
URL	/scan
Método	GET
Descripción	Escanea redes WiFi
Headers	Ninguno
Payload	Ninguno
Respuesta	JSON
Ejemplo
[
  {
    "ssid":"MiWiFi",
    "rssi":-45
  }
]
9.3 POST /connect
Campo	Valor
URL	/connect
Método	POST
Descripción	Conecta el ESP32
Headers	Content-Type: application/json
Payload	JSON
Payload
{
  "ssid":"MiWiFi",
  "password":"12345678"
}
Respuesta Exitosa
✅ Conectado! Reiniciando...
Respuesta Error
❌ Error de conexión
9.4 GET /reset
Campo	Valor
URL	/reset
Método	GET
Descripción	Restablece configuración
Headers	Ninguno
Payload	Ninguno
Respuesta
Reset OK
10. Validación Funcional
Escenario	Resultado
Inicio sin credenciales	Correcto
Portal cautivo	Correcto
Escaneo WiFi	Correcto
Guardado persistente	Correcto
Reconexión automática	Correcto
Acceso desde IP dinámica	Correcto
Restablecimiento	Correcto
11. Diagrama UML de Secuencia
Usuario -> ESP32: Conectarse a AP
Usuario -> Portal Web: Abrir 192.168.4.1
Portal Web -> ESP32: Solicitar redes
ESP32 -> Portal Web: Lista de redes
Usuario -> Portal Web: Enviar SSID y contraseña
Portal Web -> ESP32: POST /connect
ESP32 -> Router/Hotspot: Intentar conexión
Router -> ESP32: Conexión exitosa
ESP32 -> NVS: Guardar credenciales
ESP32 -> ESP32: Reiniciar
ESP32 -> Router: Reconexión automática
ESP32 -> Usuario: Nueva IP disponible
12. Comparación de Uso de Memoria Flash
12.1 Implementación Propia

Uso de memoria Flash:

73%

Características:

Interfaz personalizada
Código optimizado
Dependencias mínimas
Diseño modular
12.2 WiFiManager Basic Example

Uso de memoria Flash:

77%
12.3 Conclusión

La implementación desarrollada presenta un menor consumo de memoria Flash respecto al ejemplo básico de WiFiManager.

Esto se logró gracias a:

Eliminación de funcionalidades innecesarias.
Control manual del flujo.
Interfaz desarrollada directamente en HTML/CSS/JS.
Uso optimizado de librerías.
13. Preguntas Teóricas
13.1 ¿Es posible conectarse a redes PEAP Enterprise?

Sí.

El ESP32 soporta WPA2 Enterprise utilizando:

esp_wpa2.h

Se requiere:

Usuario
Identidad
Contraseña
Configuración WPA2 Enterprise

Ejemplo:

esp_wifi_sta_wpa2_ent_set_identity()
esp_wifi_sta_wpa2_ent_set_username()
esp_wifi_sta_wpa2_ent_set_password()
13.2 ¿Cuántos clientes soporta WebServer?

La librería WebServer es síncrona y bloqueante.

Soporta aproximadamente:

3 a 5 clientes simultáneos

Dependiendo de:

RAM disponible
Tamaño de respuesta
Complejidad del procesamiento
13.3 Alternativas
ESPAsyncWebServer

Ventajas:

Arquitectura asíncrona
Mayor rendimiento
Mejor concurrencia
Menor bloqueo
14. Buenas Prácticas Implementadas
Persistencia de datos
Modularización
Interfaz responsive
Diseño desacoplado
Manejo de errores
Uso de endpoints REST
Reinicio controlado
Validación de conexión
15. Resultados

La solución implementada logró:

Configuración dinámica WiFi
Portal cautivo funcional
Reconexión automática
Persistencia de credenciales
Interfaz web moderna
Funcionamiento estable
Administración remota
16. Conclusiones

La implementación realizada permitió desarrollar una solución IoT robusta basada en ESP32 capaz de gestionar dinámicamente redes WiFi sin necesidad de reprogramación.

El sistema demostró estabilidad tanto en modo AP como STA, validando exitosamente:

Persistencia de credenciales
Reconexión automática
Configuración dinámica
Restablecimiento funcional

Finalmente, la solución desarrollada puede aplicarse en proyectos IoT reales relacionados con automatización, monitoreo remoto y dispositivos inteligentes.

17. Referencias
Arduino ESP32 Core
Espressif Documentation
WiFiManager Library
Random Nerd Tutorials
HTTP/1.1 RFC
ESP32 Preferences API
