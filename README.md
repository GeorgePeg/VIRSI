# VIRSI
VIRSI (Voice Interface for Robotic Systems Interaction): Control through ESP-NOW and your voice a robot. The robot we will control is a car. 

# **ESP-NOW - What is it and how to use it**:
ESP-NOW is a data link layer-based wireless communication protocol that reduces five of the seven layers of the **OSI (Open Systems Interconnection)** model to a single layer.
In this way, data does not need to be transmitted through the Network Layer, Transport Layer, Session Layer, Presentation Layer, and Application Layer. Data can therefore be transmitted **only through the Physical Layer and the Data Link Layer**. Thus, there is no need for packet headers or decompression programs at each layer, which leads to fast response, reducing the latency caused by packet loss in congested networks.
<img width="1034" height="344" alt="image" src="https://github.com/user-attachments/assets/01906966-ff28-422c-b21b-16a102c2b39d" />
<br>
ESP-NOW stands out because: 
<ol>
<li>It can and does coexist with Wi-Fi and Bluetooth LE (Low Energy) and supports various Espressif SoCs with Wi-Fi connectivity.</li>
<li>It has a <strong>fast and user-friendly pairing method that is suitable for connecting “one-to-many” and “many-to-many” devices</strong>, while simultaneously controlling them.</li>
<li>It occupies <strong>less CPU and Flash resources</strong>.<li/>
<li>It can be used as a standalone protocol that helps with device provisioning, debugging, and firmware upgrades.</li>
<li>The window synchronization mechanism significantly reduces power consumption.</li>
<li>It uses the <strong>ECDH and AES-128 algorithms</strong> that make data transmission more secure.</li>
<li>In normal mode, it has a range of <strong>approximately 220-250 meters</strong> in open space, while in long range mode it can reach <strong>up to 480 meters</strong>.</li>
</ol>
