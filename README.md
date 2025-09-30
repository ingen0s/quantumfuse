
https://github.com/user-attachments/assets/e9e66469-74c2-4346-a94c-7afd6688c967



Example of an 823 mHz - 100 Hz pulse

# Quantum Fuse: A Two-Qubit Quantum Computer

Quantum Fuse is a theoretical and experimental project exploring the feasibility of creating a two-qubit quantum computer using accessible and affordable components. Our unique approach utilizes quantized mechanical vibrations within synthetic quartz crystals to serve as our qubits. The initial project testing was built around an ESP32 microcontroller, a green diode laser, and an AD9850 DDS module. **We have since updated to using an FPGA, iCE40UP5K-B-EVN FPGA instead of the ESP32**, while still keeping the ESP32 as a control interface.

- **If you are interested in contributing, would be fantastic - if a bunch of people try to do the build it would happen much faster.**
    

## The Q-Resonator: A FPGA-driven Quantum System

The Q-Resonator is the core of this project's hypothesis. We are investigating whether the quantum mechanical properties of synthetic quartz crystals can be leveraged to create and control qubits. The project aims to use the green diode laser to excite the quartz's vibrations and the DDS module to precisely control the frequencies involved, manipulating the quantum states.

You can find the detailed theoretical framework and ongoing notes here: [Q-Resonator Documentation](https://github.com/ingen0s/quantumfuse/blob/main/Q-Resonator.md) ⚛️

---

## ADR Cryostat: Millikelvin Environment

To achieve the near-absolute-zero temperatures (target **0.1–1 K**) necessary to eliminate thermal noise and observe the quantum mechanical properties of the quartz crystal qubits, we require an Adiabatic Demagnetization Refrigerator (ADR) cryostat.

The full build guide, including components, assembly instructions, and operational procedure, is detailed here: [ADR Cryostat Build Guide](https://github.com/ingen0s/quantumfuse/blob/main/ADR_Cryostat_Build_Guide.md) 🥶

### Cryostat Outline
* **Objective:** Construct a compact, single-stage ADR cryostat to cool the quartz crystal to below 4 K (target $\sim$0.1–1 K).
* **Core Components:** Paramagnetic salt pill (chrome alum), 2–4 T magnet system (Neodymium blocks or solenoid), LHe Dewar ($\sim$4 K pre-cooling), Superconducting NbTi heat switch, and Cernox temperature sensor.
* **Assembly:** Involves fabricating the salt pill, constructing the cold stage with OFHC copper, and integrating the high-vacuum and magnet systems.
* **Operation (Single-Shot Cycle):** Pre-cool to 4 K $\rightarrow$ Magnetize (Isothermal) $\rightarrow$ Isolate (Switch OFF) $\rightarrow$ Demagnetize (Adiabatic) to achieve base temperature.
* **Performance Target:** Base Temperature of $\mathbf{0.1–1\ K}$ with a hold time of 10–60 minutes per run.

---

## The Interferometer: High-Precision Measurement

This project builds a functional interferometer to measure extremely small changes in distance. The FPGA will control the laser and our measurement system, which uses machine learning and advanced mathematical techniques to interpret the interference pattern and quantify changes in the optical path.

[Michelson interferometer](https://github.com/ingen0s/quantumfuse/blob/main/Michelson-Interferometer.md)

---

## Measurement System & ML Denoising

To achieve the precision needed for quantum measurements, our system uses a camera-based interferometer and a powerful software pipeline on a PC workstation. This approach leverages machine learning to overcome environmental noise and capture the subtle changes in the quartz crystal's vibrations.

You can find the detailed plan for the measurement system and ML-based denoising here: [Measurement Outline](https://github.com/ingen0s/quantumfuse/blob/main/measurement_outline.md)

---

## Code & Resources

The code base provides the necessary firmware for the project's electronics. The primary component is a web server running on an ESP32-S3 microcontroller that controls the AD9851 DDS generator. This allows for precise control over the frequencies and timings required for our experiments via a simple web interface.

- Firmware: The web server code for the ESP32-S3 to control the AD985x DDS module can be found here: [AD985x Web Server Code](https://github.com/ingen0s/quantumfuse/blob/main/AD985X/web_server.ino)
    
    - **Note** This firmware code is for our initial testing purposes only, we will be repurposing the WiFi on the ESP32 to control the iCE40UP5K-B-EVN FPGA


![Q-Resonator](https://github.com/user-attachments/assets/596cce6c-0d1a-4744-a5cb-b6bc49421d15)
