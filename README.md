# Solución IoT para Configuración Dinámica WiFi con ESP32

## Integrantes

* Juan Camilo Gómez Bayona
* Daniel Sanchez Sotelo
* Jeronimo Infante Vega

---

# 1. Introducción

En el desarrollo de soluciones IoT modernas, uno de los principales retos es la configuración dinámica de conectividad inalámbrica sin necesidad de reprogramar el dispositivo. Tradicionalmente, los microcontroladores requieren que las credenciales WiFi sean codificadas directamente dentro del firmware, lo cual limita la flexibilidad del sistema y dificulta su despliegue en entornos reales.

Con el objetivo de solucionar esta problemática, se diseñó e implementó una solución IoT basada en el microcontrolador ESP32 utilizando el framework Arduino. La solución permite que el usuario final configure dinámicamente la red WiFi mediante una interfaz web amigable accesible desde cualquier dispositivo móvil o computador.

El sistema implementa un mecanismo de aprovisionamiento inalámbrico basado en modo Access Point (AP) y un portal cautivo local, permitiendo almacenar las credenciales de red en memoria no volátil utilizando la librería Preferences del ESP32.

Adicionalmente, la solución contempla:

* Reconexión automática.
* Persistencia de credenciales.
* Interfaz gráfica moderna.
* Endpoints REST.
* Mecanismo de restablecimiento de configuración.
* Validación funcional mediante prototipado.
* Documentación técnica completa.

---

# 2. Objetivos

## 2.1 Objetivo General

Diseñar e implementar una solución IoT basada en ESP32 que permita la configuración dinámica de redes WiFi sin necesidad de reprogramación del dispositivo.

## 2.2 Objetivos Específicos

* Implementar un portal web para configuración inalámbrica.
* Permitir almacenamiento persistente de credenciales.
* Implementar reconexión automática.
* Desarrollar una interfaz de usuario amigable.
* Diseñar endpoints HTTP para administración del sistema.
* Implementar un mecanismo de restauración de configuración.
* Validar funcionalmente el sistema mediante pruebas.
* Documentar completamente la solución.

---

# 3. Descripción General del Sistema

La solución desarrollada permite que el ESP32 opere bajo dos modos de funcionamiento:

## 3.1 Modo Access Point (AP)

Cuando el sistema no encuentra credenciales almacenadas en memoria, el ESP32 inicia automáticamente en modo AP generando una red WiFi propia llamada:

```text
ESP32_Setup
```

El usuario puede conectarse a esta red y acceder mediante navegador web a:

```text
http://192.168.4.1
```

Desde allí es posible:

* Escanear redes disponibles.
* Seleccionar SSID.
* Ingresar contraseña.
* Guardar configuración.
* Reiniciar el sistema.

---

## 3.2 Modo Station (STA)

Una vez configuradas las credenciales:

* El ESP32 guarda la información en memoria no volátil.
* Reinicia automáticamente.
* Se conecta a la red configurada.
* Obtiene una dirección IP dinámica.
* Expone nuevamente su interfaz web dentro de la red local.

---

# 4. Arquitectura del Sistema

## 4.1 Componentes Utilizados

| Componente    | Descripción                |
| ------------- | -------------------------- |
| ESP32         | Microcontrolador principal |
| WebServer.h   | Servidor HTTP              |
| DNSServer.h   | Portal cautivo             |
| Preferences.h | Memoria persistente        |
| WiFi.h        | Gestión de conectividad    |
| HTML/CSS/JS   | Interfaz gráfica           |

---

# 5. Flujo de Funcionamiento

## 5.1 Inicialización

1. El ESP32 inicia.
2. Consulta memoria NVS.
3. Verifica si existen credenciales.
4. Si no existen:

   * inicia modo AP.
5. Si existen:

   * intenta conectarse automáticamente.

---

## 5.2 Configuración WiFi

1. Usuario accede al portal.
2. El sistema escanea redes.
3. Usuario selecciona SSID.
4. Usuario ingresa contraseña.
5. El ESP32 intenta conectarse.
6. Si la conexión es exitosa:

   * guarda credenciales.
   * reinicia.

---

## 5.3 Reconexión Automática

Después del reinicio:

* El ESP32 recupera credenciales desde NVS.
* Se conecta automáticamente.
* No requiere reprogramación.

---

# 6. Diseño de Interfaz Web

La interfaz fue desarrollada utilizando:

* HTML5
* CSS3
* JavaScript Vanilla

Características implementadas:

* Diseño responsive.
* Efectos glassmorphism.
* Fondos difuminados.
* Escaneo dinámico de redes.
* Indicadores de intensidad de señal.
* Validaciones visuales.
* Compatibilidad móvil.
* Codificación UTF-8.

---

# 7. Memoria No Volátil

La persistencia de credenciales se implementó mediante:

```cpp
Preferences.h
```

Las credenciales almacenadas incluyen:

* SSID
* Contraseña

La información permanece almacenada incluso después de:

* Reinicios.
* Pérdida de energía.
* Reconexión del dispositivo.

---

# 8. Mecanismo de Restablecimiento

Se implementó un mecanismo físico y lógico para eliminar la configuración WiFi:

## 8.1 Reset Físico

Utilizando un botón conectado al GPIO 4.

Si el botón permanece presionado durante 3 segundos:

* Se eliminan las credenciales.
* El ESP32 reinicia.
* El sistema vuelve a modo AP.

---

## 8.2 Reset Web

La interfaz incluye un botón:

```text
♻ Restablecer configuración
```

El cual ejecuta el endpoint:

```text
/reset
```

---

# 9. Documentación de Endpoints

## 9.1 Endpoint Principal

### GET /

| Campo        | Valor                   |
| ------------ | ----------------------- |
| URL          | /                       |
| Método       | GET                     |
| Descripción  | Retorna la interfaz web |
| Headers      | Ninguno                 |
| Query Params | Ninguno                 |
| Payload      | Ninguno                 |
| Respuesta    | HTML                    |

### Ejemplo de Respuesta

```html
<html>
  <body>
    Portal ESP32
  </body>
</html>
```

---

## 9.2 Endpoint de Escaneo

### GET /scan

| Campo        | Valor                          |
| ------------ | ------------------------------ |
| URL          | /scan                          |
| Método       | GET                            |
| Descripción  | Escanea redes WiFi disponibles |
| Headers      | Ninguno                        |
| Query Params | Ninguno                        |
| Payload      | Ninguno                        |
| Respuesta    | JSON                           |

### Ejemplo de Respuesta

```json
[
  {
    "ssid":"MiWiFi",
    "rssi":-45
  }
]
```

---

## 9.3 Endpoint de Conexión

### POST /connect

| Campo        | Valor                          |
| ------------ | ------------------------------ |
| URL          | /connect                       |
| Método       | POST                           |
| Descripción  | Guarda y conecta el ESP32      |
| Headers      | Content-Type: application/json |
| Query Params | Ninguno                        |
| Payload      | JSON                           |
| Respuesta    | Texto                          |

### Payload

```json
{
  "ssid":"MiWiFi",
  "password":"12345678"
}
```

### Respuesta Exitosa

```text
✅ Conectado! Reiniciando...
```

### Respuesta de Error

```text
❌ Error de conexión
```

---

## 9.4 Endpoint de Reset

### GET /reset

| Campo        | Valor                          |
| ------------ | ------------------------------ |
| URL          | /reset                         |
| Método       | GET                            |
| Descripción  | Elimina configuración guardada |
| Headers      | Ninguno                        |
| Query Params | Ninguno                        |
| Payload      | Ninguno                        |
| Respuesta    | Texto                          |

### Respuesta

```text
Reset OK
```

---

# 10. Validación Funcional

## 10.1 Escenario 1 – Primera Configuración

Resultado esperado:

* El ESP32 inicia en modo AP.
* El portal web es accesible.
* El usuario configura la red.

Resultado obtenido:

* Cumplido exitosamente.

---

## 10.2 Escenario 2 – Persistencia

Resultado esperado:

* El ESP32 recuerda las credenciales después de reiniciar.

Resultado obtenido:

* Cumplido exitosamente.

---

## 10.3 Escenario 3 – Reconexión Automática

Resultado esperado:

* El ESP32 se reconecta automáticamente.

Resultado obtenido:

* Cumplido exitosamente.

---

## 10.4 Escenario 4 – Reset de Configuración

Resultado esperado:

* El sistema elimina credenciales.
* El ESP32 vuelve a modo AP.

Resultado obtenido:

* Cumplido exitosamente.

---

# 11. Diagrama UML de Secuencia

```text
Usuario -> ESP32: Conectarse a AP
Usuario -> Portal Web: Abrir 192.168.4.1
Portal Web -> ESP32: Solicitar redes
ESP32 -> Portal Web: Lista de SSID
Usuario -> Portal Web: Enviar SSID y contraseña
Portal Web -> ESP32: POST /connect
ESP32 -> Router WiFi: Intentar conexión
Router WiFi -> ESP32: Conexión exitosa
ESP32 -> NVS: Guardar credenciales
ESP32 -> ESP32: Reiniciar
ESP32 -> Router WiFi: Reconexión automática
```

---

# 12. Comparación de Memoria Flash

## 12.1 Implementación Propia

Consumo aproximado:

```text
~ 73%
```

---

## 12.2 WiFiManager Basic Example

Consumo aproximado:

```text
~ 77% 
```

---

## 12.3 Conclusión

La implementación desarrollada presenta un menor consumo de memoria debido a:

* Eliminación de dependencias innecesarias.
* Interfaz personalizada.
* Control manual del flujo.
* Uso optimizado de librerías.

---

# 13. Preguntas Teóricas

## 13.1 ¿Es posible conectarse a redes WiFi con seguridad PEAP Enterprise usando ESP32?

Sí.

El ESP32 soporta autenticación WPA2 Enterprise incluyendo métodos PEAP.

### Requisitos:

* Librería:

```cpp
esp_wpa2.h
```

* Usuario.
* Identidad.
* Contraseña.
* Configuración WPA2 Enterprise.

### Ejemplo:

```cpp
esp_wifi_sta_wpa2_ent_set_identity()
esp_wifi_sta_wpa2_ent_set_username()
esp_wifi_sta_wpa2_ent_set_password()
```

---

## 13.2 ¿Cuántos clientes simultáneos soporta WebServer?

La librería WebServer del ESP32 es síncrona y bloqueante.

En aplicaciones simples soporta:

```text
3 - 5 clientes simultáneos aproximadamente
```

Dependiendo de:

* RAM disponible.
* Tamaño de respuestas.
* Complejidad de procesamiento.

---

## 13.3 ¿Qué alternativas existen?

### ESPAsyncWebServer

Ventajas:

* Asíncrono.
* Mejor rendimiento.
* Mayor concurrencia.
* Menor bloqueo.

### AsyncTCP

Permite comunicaciones TCP asíncronas de alto rendimiento.

---

# 14. Buenas Prácticas Implementadas

* Separación lógica del sistema.
* Persistencia en memoria no volátil.
* Diseño modular.
* Manejo de errores.
* Interfaz responsive.
* Uso de endpoints REST.
* Reinicio controlado.
* Validación de conexión.
* Uso de comentarios en código.

---

# 15. Resultados

La solución implementada logró:

* Configuración dinámica WiFi.
* Persistencia de credenciales.
* Reconexión automática.
* Gestión web local.
* Portal cautivo.
* Reset de configuración.
* Interfaz moderna y amigable.
* Funcionamiento estable.

El sistema cumple exitosamente los requisitos propuestos para el desarrollo de una solución IoT basada en ESP32.

---

# 16. Conclusiones

El desarrollo realizado permitió comprender e implementar mecanismos modernos de aprovisionamiento inalámbrico en dispositivos IoT utilizando ESP32.

La solución elimina la necesidad de recompilar firmware cada vez que cambia la red WiFi, permitiendo un sistema más flexible, escalable y fácil de desplegar.

La implementación demostró estabilidad funcional tanto en modo AP como en modo STA, integrando persistencia de datos y reconexión automática.

Finalmente, el proyecto representa una solución aplicable a múltiples escenarios IoT reales, incluyendo automatización, monitoreo remoto y dispositivos inteligentes.

---

# 17. Referencias

* Documentación oficial ESP32.
* Arduino ESP32 Core.
* Random Nerd Tutorials.
* Espressif Documentation.
* WiFiManager Library.
* RFC HTTP/1.1.
