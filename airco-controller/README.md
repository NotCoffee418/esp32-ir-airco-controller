# airco-controller    

...

## Usage

1. PlatformIO vscode extension installed.
2. Upload file system and code

    ```bash
    # File system not included in Upload and Monitor
    pio run -t uploadfs

    # Upload code
    pio run -t upload
    ```

3. Ctrl+Shift+P -> PlatformIO: Upload and Monitor

## Modules

...

## User manual notes
### WiFi Credentials
On first boot, spawns hotspot allowing wifi credentials to be set.  
If we can't connect to router for too long, we also spawn hotspot.  
Credentials can also be set connecting to the IP directly.  
When existing WiFi credentials are no longer valid, plug in the device for 60 seconds to spawn hotspot.  

### Device identity
Printed on the label on the bottom of the device:
- MAC address
- Hotspot Config IP (192.168.255.1)
- Hotspot Name (for setting credentials)
- Admin Pin
- IP address (in the hotspot)