# ESP32 IR Air Conditioner Controller

A WiFi-enabled ESP32 device that controls Fujitsu air conditioners via infrared commands. Features a web dashboard for manual control, REST API for automation, and automatic WiFi fallback with hotspot configuration.

## What it does

This device replaces your physical AC remote with wireless control:
- **Manual Control**: Web dashboard with temperature, mode, fan speed, and swing controls
- **API Integration**: REST endpoints for home automation systems
- **Status Monitoring**: Real-time temperature readings and AC status
- **WiFi Management**: Automatic hotspot fallback when WiFi connection fails
- **API Key Management**: Secure API access with key generation and management

## Supported AC Models

Tested and confirmed working with:
- **ASHG24KMTA** (24,000 BTU unit) with AR-REW2E remote (0.5°C steps)
- **ASHG09KMCC** (9,000 BTU unit) with AR-REB1E remote (1°C steps)

Uses ARREB1E remote protocol for both units. The project can be modified to support other Fujitsu models or different AC brands by updating the IR command codes.

## Hardware Requirements

📋 **[Complete Shopping List](docs/shopping-list.md)** - All components with supplier links

🔌 **[Pin Layout Guide](docs/module-pins.md)** - Wiring connections and schematic

📦 **[3D Printable Case](models/AircoControllerCase.stl)** - Custom enclosure design

## Software Setup

### Prerequisites
- Visual Studio Code with PlatformIO extension
- ESP32 connected via USB

### Installation
1. **Open Project**: Use PlatformIO Home → Open Project (important: don't just open the folder in VSCode)
2. **Flash Device**: Press `Ctrl+Shift+P` → "PlatformIO: Upload and Monitor"
3. **Monitor Output**: Watch terminal for hotspot credentials and IP information

### Optional Pre-Configuration
Create `airco-controller/data/config.json` based on `airco-controller/data/config_example.json` to flash the device with WiFi credentials and settings pre-configured.

## WiFi & Network Setup

### Automatic Hotspot Fallback
- **Triggers**: No WiFi config OR WiFi connection lost for >30 seconds
- **Hotspot Name & PIN**: Printed on device bottom + shown in terminal during flashing
- **Access Point**: Connect to hotspot and navigate to `192.168.255.1`
- **Dashboard**: Configure WiFi and access all device features

### Normal Operation
Once WiFi is configured, access the dashboard at the device's local IP address (shown in terminal/hotspot setup).

## Web Dashboard Features

- **Manual Controls**: Set temperature, mode (Cool/Heat/Dry/Fan), fan speed, swing, and powerful mode
- **Status Display**: Current temperature, AC state, WiFi connection info
- **API Key Management**: Generate, view, and delete API keys for automation access
- **Quick Actions**: One-click summer/winter mode presets

## REST API

Complete API documentation available in the repository. Key endpoints:
- `POST /api/ir/off` - Turn off AC
- `POST /api/ir/on` - Turn on with settings (mode, temp, fan, swing)
- `POST /api/ir/toggle-powerful` - Toggle powerful mode
- `GET /api/sensors` - Get temperature and mode status (no auth required)

All control endpoints require API key authentication:
```
Authorization: ApiKey YOUR_API_KEY
```

Generate and manage API keys through the web dashboard.

## Project Structure

- **`airco-controller/`** - Main ESP32 firmware (flash this to your device)
- **`ir-receiver/`** - Test module for capturing IR signals from remotes
- **`docs/`** - Hardware guides and component lists  
- **`models/`** - 3D printable case files

## IR Receiver Test Module

The `ir-receiver/` folder contains a simple test program to capture and decode IR signals from your existing AC remote. Useful for:
- Verifying hardware connections
- Adding support for new AC models
- Debugging IR transmission issues

Flash this first if you want to test your hardware setup before installing the main controller.
