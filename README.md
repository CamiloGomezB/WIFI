# 📡 Sistema IoT de Configuración Dinámica WiFi con ESP32

## 🧾 Descripción del Proyecto

Este proyecto consiste en el diseño e implementación de una solución IoT basada en el microcontrolador ESP32 que permite la configuración dinámica de redes WiFi sin necesidad de reprogramación del dispositivo.

El sistema implementa un mecanismo de autoconfiguración mediante un portal web accesible en modo punto de acceso (Access Point), permitiendo al usuario ingresar las credenciales de red de manera sencilla e intuitiva.

---

## 🎯 Objetivos

### 🔹 Objetivo General

Desarrollar un sistema IoT capaz de gestionar dinámicamente la conexión a redes WiFi mediante una interfaz web embebida en el ESP32.

### 🔹 Objetivos Específicos

* Implementar modo Access Point (AP) para configuración inicial
* Diseñar una interfaz web para ingreso de credenciales
* Almacenar credenciales en memoria no volátil
* Garantizar reconexión automática
* Proveer mecanismo de reinicio de configuración
* Documentar el sistema siguiendo buenas prácticas de ingeniería

---

## 👥 Integrantes del Proyecto

* Juan Camilo Gómez Bayona
* Daniel Sanchez Sotelo
* Jeronimo Infante Vega

---

## 🧠 Arquitectura del Sistema

El sistema se compone de los siguientes módulos:

* **Gestión de red (WiFi Manager)**
* **Servidor web embebido**
* **Módulo de almacenamiento (NVS - Preferences)**
* **Interfaz de usuario (portal web)**
* **Mecanismo de reset**

### 🔄 Flujo de funcionamiento

1. El ESP32 inicia
2. Verifica credenciales almacenadas
3. Si existen:

   * Intenta conectarse a la red WiFi
4. Si no existen o falla:

   * Inicia modo AP
   * Levanta servidor web
5. Usuario accede al portal
6. Ingresa credenciales
7. Se guardan en memoria
8. El sistema se reinicia y conecta

---

## ⚙️ Tecnologías Utilizadas

* Microcontrolador: ESP32
* Framework: Arduino
* Librerías principales:

  * WiFi.h
  * WebServer.h
  * Preferences.h
  * DNSServer.h

---

## 🌐 Endpoints Implementados

### 🔹 GET /

**Descripción:**
Muestra la interfaz web para configuración de red.

**Respuesta:**

* Código: 200 OK
* Tipo: text/html

---

### 🔹 POST /save

**Descripción:**
Guarda las credenciales WiFi ingresadas por el usuario.

**Headers:**

```id="a1b2c3"
Content-Type: application/x-www-form-urlencoded
```

**Body:**

```id="d4e5f6"
ssid=NombreRed&password=ClaveWiFi
```

**Respuesta:**

```id="g7h8i9"
200 OK  
Guardado. Reiniciando...
```

---

## 💾 Almacenamiento de Datos

Se utiliza memoria no volátil (NVS) mediante la librería Preferences para almacenar:

* SSID
* Contraseña

Esto permite conservar la configuración incluso después de reinicios o pérdida de energía.

---

## 🔄 Mecanismo de Reconexión

El sistema intenta conectarse automáticamente a la red configurada al iniciar.
Se implementa un límite de intentos para evitar bloqueos.

---

## 🔁 Mecanismo de Reset

Se incluye un método para borrar la configuración almacenada:

### 🔹 Botón físico

Al presionar el botón conectado al pin definido:

* Se eliminan las credenciales
* El dispositivo reinicia
* Se activa el modo AP nuevamente

---

## 🧪 Validación del Sistema

El sistema fue validado mediante pruebas de prototipado:

### ✔ Casos probados:

* Configuración inicial sin credenciales
* Conexión exitosa a red WiFi
* Reconexión automática tras reinicio
* Fallo de conexión → retorno a modo AP
* Reset de configuración

---

## 📬 Pruebas con Postman

Se incluye colección Postman en:

```id="j1k2l3"
/postman/collection.json
```

### Ejemplo de prueba:

* Método: POST
* URL: http://192.168.4.1/save
* Body: x-www-form-urlencoded

| key      | value    |
| -------- | -------- |
| ssid     | MiRed    |
| password | 12345678 |

---

## 📊 Diagramas

Incluidos en la carpeta `/docs`:

* Diagrama de secuencia
* Diagrama de clases
* Diagrama de componentes
* Diagrama de despliegue

---

## 📁 Estructura del Proyecto

```id="m4n5o6"
ESP32-WiFi-Config/
│
├── src/
│   └── main.ino
│
├── docs/
│   ├── diagramas/
│   └── arquitectura.md
│
├── postman/
│   └── collection.json
│
├── README.md
```

---

## 🛠️ Instalación y Uso

### 🔹 Requisitos

* Arduino IDE
* ESP32 Board instalado
* Cable USB

### 🔹 Pasos

1. Clonar el repositorio
2. Abrir el archivo `.ino`
3. Seleccionar placa ESP32
4. Subir el código

### 🔹 Configuración inicial

1. Conectarse a la red WiFi:

```id="p7q8r9"
ESP32_Config
```

2. Abrir navegador:

```id="s1t2u3"
http://192.168.4.1
```

3. Ingresar credenciales

---

## 📌 Buenas Prácticas Aplicadas

* Separación de responsabilidades
* Manejo de errores
* Uso de memoria no volátil segura
* Código modular y documentado
* Interfaz simple y usable

---

## 🚀 Posibles Mejoras

* Portal cautivo automático
* Interfaz gráfica avanzada (Bootstrap)
* Escaneo de redes WiFi
* Seguridad (cifrado de credenciales)
* Integración con servicios en la nube

---

## 🏫 Información Académica

* Asignatura: IoT / Sistemas Embebidos
* Institución: [Nombre de la universidad]
* Año: 2026

---

## 📄 Licencia

Este proyecto es de uso académico.
