# ESP32 WiFi Manager

**Solución IoT para configuración dinámica de red WiFi sin reprogramación**

---

## Descripción

Sistema embebido para el microcontrolador ESP32 que permite al usuario final configurar dinámicamente las credenciales de red WiFi (SSID y contraseña) sin necesidad de reprogramar el dispositivo. Implementa un **portal cautivo** y una **API REST** local, con almacenamiento persistente en memoria NVS.

---

## Características principales

- **Modo AP automático**: Inicia como Access Point si no hay credenciales guardadas
- **Portal cautivo**: Redirige automáticamente al portal de configuración
- **Interfaz web responsiva**: Compatible con móvil y escritorio
- **API REST completa**: 6 endpoints documentados con respuestas JSON
- **NVS (Non-Volatile Storage)**: Persistencia de credenciales en memoria flash
- **Reconexión automática**: Intenta conectar con credenciales guardadas al arrancar
- **Reset físico**: Botón hardware para borrar configuración
- **LED de estado**: Indicadores visuales del estado del sistema
- **Escaneo WiFi**: Lista redes disponibles ordenadas por señal

---

## Requisitos de hardware

| Componente | Especificación |
|---|---|
| Microcontrolador | ESP32 (cualquier variante con WiFi) |
| LED de estado | GPIO 2 (built-in en la mayoría de placas) |
| Botón de reset | GPIO 15 → GND (pull-up interno) |
| Alimentación | 3.3V o 5V (según placa) |

### Esquemático de conexiones

```
ESP32                    Componentes externos
─────                    ───────────────────
GPIO  2 ──[330Ω]─── LED ─── GND    (LED de estado)
GPIO 15 ──────────── BTN ─── GND   (Botón de reset)
3V3     ─────────────────────────  (alimentación)
GND     ─────────────────────────  (tierra común)
```

> **Nota**: GPIO 2 y GPIO 0 ya tienen LEDs internos en la mayoría de placas de desarrollo ESP32. El botón en GPIO 15 usa pull-up interno del ESP32.

---

## Dependencias

Instalar desde el Arduino Library Manager (`Herramientas → Administrar bibliotecas`):

| Biblioteca | Versión mínima | Fuente |
|---|---|---|
| ArduinoJson | >= 6.0.0 | Benoit Blanchon |
| ESP32 Arduino Core | >= 2.0.0 | Espressif (gestor de placas) |

Las siguientes están incluidas en el ESP32 Arduino Core:
- `WiFi.h`
- `WebServer.h`
- `DNSServer.h`
- `Preferences.h`

---

## Instalación

### 1. Configurar Arduino IDE

```
1. Abrir Arduino IDE
2. Archivo → Preferencias
3. URLs adicionales de gestor de placas:
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Herramientas → Gestor de placas → buscar "esp32" → Instalar
```

### 2. Instalar dependencias

```
Herramientas → Administrar bibliotecas → buscar "ArduinoJson" → Instalar versión >= 6.0.0
```

### 3. Cargar el código

```
1. Abrir esp32_wifi_manager.ino
2. Seleccionar placa: Herramientas → Placa → ESP32 Arduino → ESP32 Dev Module
3. Seleccionar puerto COM correspondiente
4. Verificar velocidad de subida: 921600 (recomendado)
5. Clic en Subir
```

---

## Flujo de operación

### Primer arranque (sin credenciales)

```
┌─────────┐    ┌──────────────┐    ┌────────────────┐    ┌──────────────┐
│  BOOT   │───▶│ Leer NVS     │───▶│ Sin creds →    │───▶│ Modo AP      │
│         │    │ (credenciales│    │ Iniciar AP     │    │ SSID:        │
│         │    │  guardadas?) │    │                │    │ ESP32-Config │
└─────────┘    └──────────────┘    └────────────────┘    └──────┬───────┘
                                                                  │
                    ┌─────────────────────────────────────────────┘
                    ▼
            ┌──────────────┐    ┌────────────────┐    ┌──────────────┐
            │ Usuario      │    │ Portal cautivo │    │ Guardar en   │
            │ conecta a    │───▶│ 192.168.4.1    │───▶│ NVS →        │
            │ ESP32-Config │    │ Ingresa SSID   │    │ Reiniciar    │
            └──────────────┘    └────────────────┘    └──────────────┘
```

### Arranques subsiguientes (con credenciales)

```
┌─────────┐    ┌──────────────┐    ┌────────────────┐    ┌──────────────┐
│  BOOT   │───▶│ Leer NVS     │───▶│ Credenciales   │───▶│ Conectar a   │
│         │    │ (OK)         │    │ encontradas    │    │ WiFi (15s)   │
└─────────┘    └──────────────┘    └────────────────┘    └──────┬───────┘
                                                                  │
                              ┌──────────────────────────────────▼──────┐
                              │          ¿Conectó exitosamente?          │
                              └───────────┬──────────────────────┬───────┘
                                          │ SÍ                   │ NO
                                          ▼                       ▼
                                   ┌────────────┐         ┌────────────┐
                                   │ Modo STA   │         │ Modo AP    │
                                   │ IP asignada│         │ (fallback) │
                                   └────────────┘         └────────────┘
```

---

## API REST

### Base URL

| Modo | URL base |
|---|---|
| Modo AP | `http://192.168.4.1` |
| Modo STA | `http://{ip-asignada}` |

---

### `GET /`

**Descripción**: Sirve el portal HTML de configuración WiFi.

**Headers de respuesta**:
```
Content-Type: text/html; charset=utf-8
```

**Respuesta**: Página HTML completa del portal cautivo.

---

### `GET /api/status`

**Descripción**: Retorna el estado actual de conectividad del dispositivo.

**Headers de respuesta**:
```
Content-Type: application/json
```

**Respuesta exitosa (200)**:
```json
{
  "wifi_connected": true,
  "mode": "STA",
  "ssid": "MiRedCasa",
  "ip": "192.168.1.105",
  "rssi": -62,
  "channel": 6,
  "mac": "AA:BB:CC:DD:EE:FF",
  "state": "CONNECTED",
  "uptime_ms": 45230
}
```

**Campos de respuesta**:

| Campo | Tipo | Descripción |
|---|---|---|
| `wifi_connected` | boolean | true si está conectado a una red |
| `mode` | string | "STA" (cliente) o "AP" (access point) |
| `ssid` | string | Nombre de la red conectada o AP |
| `ip` | string | IP del dispositivo |
| `rssi` | integer | Potencia de señal en dBm (solo modo STA) |
| `channel` | integer | Canal WiFi (solo modo STA) |
| `mac` | string | Dirección MAC |
| `state` | string | Estado: INIT, AP_MODE, CONNECTING, CONNECTED, CONNECTION_FAILED |
| `uptime_ms` | integer | Milisegundos desde el arranque |

---

### `GET /api/scan`

**Descripción**: Escanea y retorna las redes WiFi disponibles, ordenadas por intensidad de señal.

**Headers de respuesta**:
```
Content-Type: application/json
```

**Respuesta exitosa (200)**:
```json
{
  "count": 3,
  "networks": [
    {
      "ssid": "MiRedCasa",
      "rssi": -45,
      "encrypted": true,
      "channel": 6
    },
    {
      "ssid": "Vecino",
      "rssi": -72,
      "encrypted": true,
      "channel": 11
    },
    {
      "ssid": "RedAbierta",
      "rssi": -80,
      "encrypted": false,
      "channel": 1
    }
  ]
}
```

**Campos de cada red**:

| Campo | Tipo | Descripción |
|---|---|---|
| `ssid` | string | Nombre de la red |
| `rssi` | integer | Potencia de señal (dBm). Valores: -30 excelente, -67 buena, -80 débil |
| `encrypted` | boolean | true si requiere contraseña |
| `channel` | integer | Canal WiFi (1-13) |

> **Nota**: El escaneo puede tardar entre 2 y 5 segundos. Las redes con SSID duplicado se filtran automáticamente.

---

### `POST /api/connect`

**Descripción**: Recibe las credenciales WiFi, las guarda en NVS y tenta la conexión.

**Headers de solicitud**:
```
Content-Type: application/json
```

**Body (JSON)**:
```json
{
  "ssid": "MiRedCasa",
  "password": "micontraseña123"
}
```

**Parámetros del body**:

| Campo | Tipo | Requerido | Descripción |
|---|---|---|---|
| `ssid` | string | ✅ Sí | Nombre de la red (1-32 caracteres) |
| `password` | string | ⬜ No | Contraseña (vacía para redes abiertas) |

**Respuesta exitosa (200)**:
```json
{
  "success": true,
  "message": "Credenciales guardadas. Conectando..."
}
```

**Respuesta de error (400) - SSID faltante**:
```json
{
  "success": false,
  "message": "SSID requerido"
}
```

**Respuesta de error (400) - SSID demasiado largo**:
```json
{
  "success": false,
  "message": "SSID demasiado largo (max 32 chars)"
}
```

**Respuesta de error (400) - JSON inválido**:
```json
{
  "success": false,
  "message": "JSON inválido"
}
```

> **Importante**: El endpoint responde con 200 antes de intentar la conexión (que puede tomar hasta 15 segundos). Para verificar si conectó exitosamente, consultar `/api/status` después de 15-20 segundos.

---

### `POST /api/reset`

**Descripción**: Borra todas las credenciales guardadas en NVS y reinicia el dispositivo en modo Access Point.

**Body**: No requerido (puede estar vacío).

**Respuesta exitosa (200)**:
```json
{
  "success": true,
  "message": "Configuración borrada. Reiniciando en modo AP..."
}
```

> **Nota**: El dispositivo se reiniciará automáticamente 1 segundo después de esta respuesta. La conexión se perderá.

---

### `GET /api/info`

**Descripción**: Retorna información detallada del hardware y software del dispositivo.

**Headers de respuesta**:
```
Content-Type: application/json
```

**Respuesta exitosa (200)**:
```json
{
  "device": "ESP32",
  "firmware": "WiFi Manager v2.0.0",
  "chip_id": "ab12cd34",
  "mac_address": "AA:BB:CC:DD:EE:FF",
  "chip_model": "ESP32-D0WDQ6",
  "chip_cores": 2,
  "cpu_freq_mhz": 240,
  "flash_size_kb": 4096,
  "free_heap_b": 287432,
  "uptime_ms": 120450,
  "uptime_str": "0h 2m 0s",
  "sdk_version": "v4.4.4"
}
```

---

## Indicadores LED

| Patrón | Estado |
|---|---|
| Parpadeo muy rápido (200ms) | Modo AP activo - esperando configuración |
| Parpadeo medio (500ms) | Conectando a red WiFi |
| Encendido fijo | Conectado exitosamente |
| Parpadeo lento (1000ms) | Error de conexión |

---

## Reset de configuración

### Método 1: API REST

```bash
curl -X POST http://192.168.4.1/api/reset
```

### Método 2: Botón físico

Mantener presionado el botón en GPIO 15 por **3 segundos**. El LED parpadeará rápidamente y el dispositivo se reiniciará.

### Método 3: Desde el portal web

En la interfaz HTML, clic en el botón **"Borrar configuración guardada"**.

---

## Configuración de parámetros

Los parámetros principales se pueden modificar en la sección de `#define` del archivo `.ino`:

```cpp
#define AP_SSID          "ESP32-Config"    // Nombre de la red AP
#define AP_PASSWORD      "12345678"        // Contraseña del AP (min 8 chars)
#define WIFI_CONNECT_TIMEOUT_MS  15000    // Timeout de conexión (ms)
#define RESET_BUTTON_HOLD_MS     3000     // Tiempo de hold para reset (ms)
#define PIN_LED_STATUS   2                // GPIO del LED de estado
#define PIN_RESET_BUTTON 15              // GPIO del botón de reset
```

---

## Arquitectura del software

```
esp32_wifi_manager.ino
├── setup()                    ← Inicialización del sistema
│   ├── GPIO setup             ← LED y botón
│   ├── NVS init               ← Preferences library
│   ├── loadCredentials()      ← Lee NVS
│   ├── connectToWiFi() / setupAP()
│   └── setupWebServer()       ← Registra endpoints
│
├── loop()                     ← Bucle principal
│   ├── dnsServer.processNextRequest()   ← Portal cautivo
│   ├── webServer.handleClient()         ← HTTP
│   ├── updateLED()                      ← Indicador visual
│   └── checkResetButton()               ← Reset por hardware
│
├── Handlers HTTP
│   ├── handleRoot()           ← GET /
│   ├── handleApiStatus()      ← GET /api/status
│   ├── handleApiScan()        ← GET /api/scan
│   ├── handleApiConnect()     ← POST /api/connect
│   ├── handleApiReset()       ← POST /api/reset
│   ├── handleApiInfo()        ← GET /api/info
│   └── handleNotFound()       ← Redirección portal cautivo
│
└── Utilidades
    ├── connectToWiFi()        ← Lógica de conexión WiFi
    ├── loadCredentials()      ← Lectura NVS
    ├── saveCredentials()      ← Escritura NVS
    ├── clearCredentials()     ← Borrado NVS
    ├── updateLED()            ← Control LED por estado
    ├── checkResetButton()     ← Debounce y hold detection
    └── getSystemUptime()      ← Formato de uptime
```

---

## Solución de problemas

| Problema | Causa probable | Solución |
|---|---|---|
| No aparece la red "ESP32-Config" | El ESP32 no arrancó en modo AP | Revisar alimentación; verificar si hay credenciales guardadas |
| Portal no abre automáticamente | El SO no redirigió automáticamente | Navegar manualmente a `http://192.168.4.1` |
| Fallo de conexión tras guardar | Contraseña incorrecta o señal débil | Verificar credenciales; acercar el ESP32 al router |
| LED fijo sin estar conectado | Error en el código de estado | Verificar monitor serial a 115200 baud |
| No compila | ArduinoJson no instalado | Instalar ArduinoJson >= 6.0.0 desde Library Manager |

---

## Licencia

MIT License — ver archivo [LICENSE](LICENSE) para detalles.
