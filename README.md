# Quantum Fuse: A Two-Qubit Quantum Computer
Quantum Fuse is a theoretical and experimental project exploring the feasibility of creating a two-qubit quantum computer using accessible and affordable components. Our unique approach utilizes quantized mechanical vibrations within synthetic quartz crystals to serve as our qubits. The project is built around an ESP32 microcontroller, a green diode laser, and an AD9850 DDS module.

## The Q-Resonator
The Q-Resonator is the core of this project's hypothesis. We are investigating whether the quantum mechanical properties of synthetic quartz crystals can be leveraged to create and control qubits. The project aims to use the green diode laser to excite the quartz's vibrations and the DDS module to precisely control the frequencies involved, manipulating the quantum states.

You can find the detailed theoretical framework and ongoing notes here: [Q-Resonator Documentation](https://github.com/ingen0s/quantumfuse/blob/main/Q-Resinator.md) ⚛️

##  Code & Resources
The code base provides the necessary firmware for the project's electronics. The primary component is a web server running on an ESP32-S3 microcontroller that controls the AD9851 DDS generator. This allows for precise control over the frequencies and timings required for our experiments via a simple web interface.

  -  Firmware: The web server code for the ESP32-S3 to control the AD985x DDS module can be found here: [AD985x Web Server Code](https://github.com/ingen0s/quantumfuse/blob/main/AD985X/web_server.ino)


![Q-Resonator](https://github.com/user-attachments/assets/596cce6c-0d1a-4744-a5cb-b6bc49421d15)
