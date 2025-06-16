# ESP32 AC Controller - Module Connections

## ⚠️ CRITICAL WARNING ⚠️

**ALWAYS VERIFY PIN LAYOUTS WITH THE COMPONENT DATASHEET BEFORE CONNECTING!**

Different manufacturers and model revisions can have completely different pin arrangements, even for components with the same part number. Double-check your specific component's datasheet to avoid damaging your ESP32 or components. When in doubt, use a multimeter to verify connections.

---

## Temperature Sensor

**DS18B20 Waterproof Digital Temperature Sensor**  
[Gotron DS18B20](https://www.gotron.be/waterproof-ds18b20-digital-temperature-sensor-extras.html)

**Connections:**
- Blue wire → GND
- Red wire → VIN (3.3V)
- Yellow wire → D13
- **4.7kΩ pull-up resistor between Red and Yellow wires**

**Soldering Configuration:**
![Temperature Sensor Wiring](https://raw.githubusercontent.com/NotCoffee418/esp32-ir-airco-controller/refs/heads/main/docs/tempsensor.png)

*The pull-up resistor must be soldered between the red (VIN) and yellow (data) wires as shown above.*

## IR LED Transmitter

**ELIR204 IR LED (940nm, 3mm)**  
[Gotron IR LED](https://www.gotron.be/componenten/actief/opto/zender-ontvanger/ir-led-blauw-35-3mm-940nm-20ma-1-2v.html)

**Connections:**
- Long leg (Anode) → D4 (via 100Ω resistor)
- Short leg (Cathode) → GND

## Mode Switch

**MS100A SPST Toggle Switch (6A, 250V)**  
[Gotron Toggle Switch](https://www.gotron.be/componenten/schakelmat/schakelaars/toestel/toestelschakelaar-enkelpolig-on-off-spst-6a-250v-zwart.html)

**Connections:**
- Pin 1 → GND
- Pin 2 → D12

---

## Ground Connection Notes

**All GND connections from the modules can be combined using:**
- WAGO 5-pole connectors for clean, reusable connections
- Direct soldering for permanent installations

This allows sharing a single GND connection from the ESP32 to all modules requiring ground.