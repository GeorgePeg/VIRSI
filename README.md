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
<li>It occupies <strong>less CPU and Flash resources</strong>.</li>
<li>It can be used as a standalone protocol that helps with device provisioning, debugging, and firmware upgrades.</li>
<li>The window synchronization mechanism significantly reduces power consumption.</li>
<li>It uses the <strong>ECDH and AES-128 algorithms</strong> that make data transmission more secure.</li>
<li>In normal mode, it has a range of <strong>approximately 220-250 meters</strong> in open space, while in long range mode it can reach <strong>up to 480 meters</strong>.</li>
</ol>
ESP-NOW uses a vendor-specific action framework for data transmission. The default bit rate is <strong>1Mbps</strong>.

**Basic ESP-NOW Commands**:
Before writing any commands, we must call the ESP-NOW API and start Wi-Fi, since ESP-NOW is a Wi-Fi type wireless network **(IEEE 802.11 b/g/n)**. This is done using the Arduino IDE as follows:
<ul>
  <li><code>#include "WiFi.h"</code></li>
  <li><code>#include "esp_now.h"</code></li>  
</ul>
ESP-NOW initialization/deinitialization functions:
<ul>
  <li><code>esp_now_init()</code>: ESP-NOW initialization</li>
  <li><code>esp_now_deinit()</code>: ESP-NOW deinitialization</li>
  <p class="text-warning">When the deinitialization function is called, all paired device information is deleted.</p>
</ul>
<br>
Return to sender function:
<code>esp_now_register_send_cb(dataReceive)</code>:
<p>By calling the above function, we declare the function we have defined as a confirmation reception function, let's say dataReceive, which is called when the sending of a message from the sender to the recipient is complete.</p><br>
Return function to the recipient:
<code>esp_now_register_recv_cb(dataReceive)</code>:
<p>By calling the above function, we declare the function we have defined as a confirmation reception function, let's say dataReceive, which is called when a message is received by the recipient.</p><br>
Add a paired device:
<code>esp_now_add_peer(&peerInfo)</code>: Add a new device/peer with MAC address.
<p>This function takes as an argument a pointer to a struct, esp_now_peer_info_t, containing information such as the MAC address, the channel, or whether encryption is required. The channel range of paired devices is from 0 to 14.If the channel is set to 0, data is sent on the current channel, otherwise the channel must be set to the channel the local device is on. The following applies to <code>peerInfo</code>:</p>
<ul>
  <li><code>peerInfo.peer_addr</code>: Recipient MAC address.</li>
  <li><code>peerInfo.channel</code>: Integer number from 1-14.</li>
  <li><code>peerInfo.encrypt</code>: True/false value to enable or disable encryption.</li>
</ul>





# Voice Recognition Module V3
<img width="400" height="400" alt="image" src="https://github.com/user-attachments/assets/a2810c22-e465-4ab1-9fae-5b934d4243b6" style="align-items: center; justify-content: center;" />
<br>
<p>Elechouse's VRM (Voice Recognition Module V3) is a standalone voice recognition unit, designed to be easily integrated into systems that operate via voice commands from the user, either with the help of microcontrollers such as Arduino, ESP or STEM32, or microprocessors such as Raspberry Pi, without requiring any kind of connection to an external computer or even a connection to the cloud.</p>
<p>The voice recognition module operates at 3.3V-5V DC (direct current), is small in size (31mm x 50mm), which makes it ideal for use in robotic systems for motion control, for “one-shot” commands in IoT systems, for home automation uses (smart home), etc.</p>
<br>
Finally, the board has two control modes:
<ul>
<li>via serial port and </li>
<li>using general input pins (GIP).</li>
</ul>
<br>
<p>The board is equipped with a built-in <strong>unidirectional MEMS (Micro-Electro-Mechanical Systems) microphone</strong>. The microphone is small in size, but at the same time low in consumption. It is designed to receive sound mainly from one direction (uni-directional), reducing environmental noise. Such a microphone is suitable for short-distance voice commands (≈ 20-50 centimeters for optimal accuracy – approximately 99% in ideal environmental conditions).In addition, to the VRM, through the audio jack, we can connect an external microphone, also known as a gooseneck, which is equipped with a sponge for better attenuation of the noise received by the microphone (e.g. from breathing or air).</p>



