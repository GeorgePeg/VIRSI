# VIRSI
VIRSI (Voice Interface for Robotic Systems Interaction): Control through ESP-NOW and your voice a robot. The robot we will control is a car. This is a project within the framework of the "Sound and Music Processing" course that combines wireless networks, robotics and signal processing.


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
<br>
<code>esp_now_register_send_cb(dataReceive)</code>:
<p>By calling the above function, we declare the function we have defined as a confirmation reception function, let's say dataReceive, which is called when the sending of a message from the sender to the recipient is complete.</p><br>
Return function to the recipient:
<br>
<code>esp_now_register_recv_cb(dataReceive)</code>:
<p>By calling the above function, we declare the function we have defined as a confirmation reception function, let's say dataReceive, which is called when a message is received by the recipient.</p><br>
Add a paired device:
<br>
<code>esp_now_add_peer(&peerInfo)</code>: Add a new device/peer with MAC address.
<p>This function takes as an argument a pointer to a struct, esp_now_peer_info_t, containing information such as the MAC address, the channel, or whether encryption is required. The channel range of paired devices is from 0 to 14.If the channel is set to 0, data is sent on the current channel, otherwise the channel must be set to the channel the local device is on. The following applies to <code>peerInfo</code>:</p>
<ul>
  <li><code>peerInfo.peer_addr</code>: Recipient MAC address.</li>
  <li><code>peerInfo.channel</code>: Integer number from 1-14.</li>
  <li><code>peerInfo.encrypt</code>: True/false value to enable or disable encryption.</li>
</ul>
<p>The function returns an integer type representing the failure/success of the action (<code>esp_err_t</code>). If the returned value is <code>ESP_OK</code>, then the peer was added successfully. If the returned value is <code>ESP_ERR_ESPNOW_EXIST</code>, it means that the peer already exists. Another possible return value is <code>ESP_ERR_ESPNOW_FULL</code>, which indicates that we have reached the maximum number of peers that we can add.
The value <code>ESP_ERR_ESPNOW_NOT_INIT</code> essentially informs us that we have forgotten to initialize ESP-NOW. Finally, the value <code>ESP_NOW_NO_MEM</code> indicates insufficient memory, which makes adding the peer impossible.
</p><br>
Data sending function:
<br>
<code>esp_now_send(mac_addr, data, len)</code>:
<p>The above function sends data to a device with the specified MAC address. The variable len refers to the length of the packet which depends on the version of the device (v1.0 or v2.0).
</p><br>
Peer existence check function:
<br>
<code>esp_now_is_peer_exist(mac_addr)</code>: <p>The peer existence check function essentially checks whether a MAC address already exists in the peer list.</p><br>
Extra functions related to the <code>WiFi.h</code> library:
<br>
<ul>
  <li><code>WiFi.mode(WIFI_STA)</code>: Sets the device to Station Mode.</li>
  <li><code>WiFi.macAddress()</code>: Returns the MAC address of the device.</li>
  <li><code>WiFi.disconnect()</code>: Disconnects the device from the current WiFi network (current AP- access point), without disabling WiFi functionality (the device can still operate as a STA or AP). However, if it is given a true argument, it disconnects and deletes the stored connection information (e.g. SSID/Password).
</li>
</ul>
In the <a href="Sender.ino">Sender.ino</a> and <a href="Receiver.ino">Receiver.ino</a> files you can see how to easily implement sending a simple message from the sender to the receiver.

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
<p>The board is equipped with a built-in <strong>unidirectional MEMS (Micro-Electro-Mechanical Systems) microphone</strong>. The microphone is small in size, but at the same time low in consumption. It is designed to receive sound mainly from one direction (uni-directional), reducing environmental noise. Such a microphone is suitable for short-distance voice commands (≈ 20-50 centimeters for optimal accuracy – approximately 99% in ideal environmental conditions).In addition, to the VRM, through the audio jack, we can connect an external microphone, also known as a gooseneck, which is equipped with a sponge for better attenuation of the noise received by the microphone (e.g. from breathing or air).</p><br>
<p>The voice recognition board supports user-trained commands. It can be trained locally with <strong>up to 80 words/commands</strong>, with the ability to support <strong>up to 7 words/commands active at a time </strong>, meaning the VRM "listens" and recognizes only these seven.</p><br>
<img width="568" height="391" alt="image" src="https://github.com/user-attachments/assets/354a046f-1300-4327-90f1-e9dbeb5bd54c" />
<p>Each word is stored in the board's internal flash with a specific id. Each time it successfully recognizes one of the active words/commands, it returns the corresponding id number of that word/command, via UART communication (i.e. via Tx/Rx).The choice of language in which the VRM will be "trained" does not play a role, since the operation of the board is based <strong>entirely on sound and not on any specific language </strong>.All voice commands are stored in a common group for all. As shown in the diagram
above. On the board is the recognizer, which acts as a container in which the voice commands that can be recognized are loaded. In essence, it functions as a list in which the elements we want each time are stored.</p><br>

# **Headphone circuit**
For the project, a custom pair of headphones was created in which the transmitter circuit was integrated.This circuit includes the Voice Recognition Module, the speakers from the headphones, and an ESP32-S3 which performs the job of the sender.
![sender_circuit](https://github.com/user-attachments/assets/059904c7-31f2-4766-89aa-b21a01781dc9)
As you can see, in the circuit there is a voltage divider between the transmitter of the Voice Recognition Module and the receiver of the ESP32-S3 used in the sender. This voltage divider is included for one reason:
_In case you want to supply the VRM V3 with 5V, you will have to use the voltage divider because the **pins of the ESP32-S3 are not 5V tolerant**.You will have to reduce the voltage down to 3.3V._ You can make the voltage divider using a **4k7Ω (4.700 Ohms)** and a **10kΩ (10.000 Ohms)** resistors. Of course, you are not required to use the voltage divider, since the VRM V3 **operates normally at 3.3V**.

# **Robot circuit**
Here you can see the circuit diagram of our robot (our receiver):
![receiver_robot_circuit](https://github.com/user-attachments/assets/ad4ef4f6-60db-4aee-8c23-eea44ad8f434)

# **Compression**
One technique we tried during the implementation of the project is that of **compression**. With compression we can adjust **the volume of our signal** to be as stable as possible aurally, without losing words, simply by **reducing large volume fluctuations and amplifying the weakest points**. A compressor is characterized by the following:
<ul>
  <li><strong>Threshold:</strong>the volume threshold above which compression begins. Measured in <strong>dB</strong>.</li>
  <li><strong>Compression ratio:</strong>the rate of decrease in intensity beyond the threshold.</li>
  <li><strong>Attack/release time:</strong>times that show us how quickly the compression starts or stops.</li>
  <li><strong>Final gain (make-up gain):</strong>the value of the amplification that results after compression, in order to reach the desired volume.</li>
</ul>




