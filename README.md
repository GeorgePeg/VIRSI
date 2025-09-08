# VIRSI
VIRSI (Voice Interface for Robotic Systems Interaction): Control through ESP-NOW and your voice a robot. The robot we will control is a car. 

# **ESP-NOW - What is it and how to use it**:
ESP-NOW is a data link layer-based wireless communication protocol that reduces five of the seven layers of the **OSI (Open Systems Interconnection)** model to a single layer.
In this way, data does not need to be transmitted through the Network Layer, Transport Layer, Session Layer, Presentation Layer, and Application Layer. Data can therefore be transmitted **only through the Physical Layer and the Data Link Layer**. Thus, there is no need for packet headers or decompression programs at each layer, which leads to fast response, reducing the latency caused by packet loss in congested networks.
<img width="1034" height="344" alt="image" src="https://github.com/user-attachments/assets/01906966-ff28-422c-b21b-16a102c2b39d" />
<br>
ESP-NOW stands out because: 
• It can and does **coexist with Wi-Fi and Bluetooth LE (Low Energy)** and supports various Espressif SoCs with Wi-Fi connectivity.
• It has a **fast and user-friendly pairing method** that is suitable for **connecting “one-to-many” and “many-to-many” devices**, while simultaneously controlling them.
• It occupies **less CPU and Flash resources**.
• It can be used as a standalone protocol that helps with device provisioning, debugging, and firmware upgrades.
• The window synchronization mechanism significantly reduces power consumption.
• It uses the **ECDH and AES-128 algorithms** that make data transmission more secure.
• In normal mode, it has a range of approximately 220-250 meters in open space, while in long range mode it can reach up to 480 meters.

