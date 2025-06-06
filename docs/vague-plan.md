interop with european-smart-meter or seperate application within either repo
use MQTT for communication between devices

rasp handles logic, two esp32s handle ir command transmission and temperature sensing

ir receiver to reverse engineer (or confirm what we suspect) the protocol to be.

# Integrations?
- home assistant (as per mathis usually good advice when it comes to these things)
- custom svelte dashboard with backend (more control, but restricts european-smart-meter's ability to be universal if directly integrated)
- grafana dashboard (can we integrate with this at this point? if we're doing severely more than just power consumption measurements)


home assistant with seperate plugins may be the best option if we can trigger set custom conditions and have them trigger properly in real time.