# RE-Receiver

Experimental tool to test and verify IR signals.
Interpret IR signals and display them on a web page. Designed for Fujitsu A/C units.

## Usage

1. PlatformIO vscode extension installed.
2. Configure `data/config.ini` with your WiFi credentials and network settings.
3. Create file system and upload code

    ```bash
    # Only do this once unless full reset
    pio run --target uploadfs
    ```

4. Ctrl+Shift+P -> PlatformIO: Upload and Monitor

## Modules

### TSOP2238 (front view) → ESP32

- Pin 1 (OUT) → GPIO 4
- Pin 2 (VS)  → 3.3V
- Pin 3 (GND) → GND