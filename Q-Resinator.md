### Project Q-Resonator: A Theoretical Quantum Computer

#### Overview

This project is a theoretical exploration into building a two-qubit quantum computer using a green diode laser, an ESP32 microcontroller, an AD9850 DDS module, and two separate synthetic quartz crystals. We hypothesize that the quantized mechanical vibrations of the quartz crystals can serve as our qubits, which are controlled and measured using our available components.

---

#### Core Components

- **ESP32 Microcontroller:** Serves as the classical control unit. It sends precise timing signals and commands to the AD9850 to execute quantum gates.
    
- **AD9850 DDS Module:** Generates the radio frequency (RF) signals that will drive the piezoelectric vibrations of the quartz.
    
- **Green Diode Laser:** Used for the theoretical **interferometric measurement** of the qubit's vibrational state.
    
- **Piezoelectric Pickup Discs:** Two discs, each acting as both an actuator (to initiate vibrations) and a sensor (to read back signals) for a separate quartz plate.
    
- **Synthetic Quartz Crystals:** Two separate plates, each **50mm x 50mm x 1mm**, serving as the material for our qubits.
    

---

#### Theoretical Qubit Design

Each quartz plate has a single, isolated **mechanical vibrational mode** that acts as our qubit. The two quantum states, ∣0⟩ and ∣1⟩, would correspond to the ground state of vibration and the first excited state (a single phonon). The 1mm thickness of the plates gives them a fundamental resonant frequency in the MHz range, which is perfectly suited for the AD9850.

---

#### Quantum Operations

- **Single-Qubit Gates:** The ESP32 would command the AD9850 to produce a precise RF pulse with a specific duration and frequency, which is sent to one of the piezoelectric discs. This pulse would cause that quartz plate to vibrate, theoretically rotating the qubit's state.
    
- **Two-Qubit Gates:** We would make the two separate quartz plates interact. For example, by pulsing one qubit to create a specific vibrational state, we could theoretically couple it to the other plate, performing a controlled gate like a CNOT. The ESP32 would also need to independently switch between the two plates to address them individually.
    

---

#### Quantum Algorithm

Our theoretical quantum computer would run the **Deutsch-Jozsa algorithm**. The ESP32 would coordinate the sequence of RF pulses and laser measurements to determine whether a hypothetical black-box function is "constant" or "balanced" in a single run.

---

#### Experiment Update Log

- **September 17, 2025:** I hooked up one piezo pickup to one plate and set the **ICL8038** to cycle at 10 Hz. I shined a green laser diode onto the plate at an angle to reflect to a piece of paper. Each time the cycle turned to 10 Hz, the laser projection shifted partially, and each time it returned to 0 Hz, it shifted back. The shifts are almost perfect with slight variations. We also discovered that using a square wave from the ICL8038 had a more noticeable effect than a sine wave because it excited multiple harmonics.
    
- **September 18, 2025:** We've introduced a new theoretical concept: using a second, smaller quartz crystal with its own piezo pickup as a **quantum beam splitter**. By carefully pulsing this crystal with the **AD9850**, its vibrations would alter the phase of the laser light reflecting off it. This allows us to perform a **quantum gate directly on the photon**, which adds a new dimension to our quantum operations.
    

---

#### Physical Implementation Sketches

- **Hadamard Gate Sketch:** This conceptual code outlines how the ESP32 would command the AD9850 to create a quantum gate pulse, specifically a **Hadamard gate** which puts a qubit into a superposition.
    
    C++
    
    ```
    // Theoretical Code for a Hadamard Gate Pulse
    // This code is conceptual and would require specific libraries
    // and hardware configuration to be functional.
    #define H_PULSE_DURATION_US 25 // Example: 25 microseconds
    #define QUBIT_FREQUENCY_MHZ 2.5 // Example: Matches the quartz plate's resonance
    void setupAD9850() {
      // Commands to reset the AD9850 and set the output frequency.
      // For a 125MHz clock, 2.5MHz would be (2.5 * 2^32) / 125 = 85899345.
      send_frequency_word(85899345);
    }
    void loop() {
      ad9850_power_up();
      delayMicroseconds(H_PULSE_DURATION_US);
      ad9850_power_down();
      delay(100); // Wait for measurement system to stabilize.
    }
    ```
    
- **Two-Qubit CNOT Gate Sketch:** A **Controlled-NOT (CNOT) gate** flips the state of a "target" qubit if and only if the "control" qubit is in the ∣1⟩ state. In our theoretical setup, this means we would only pulse the second quartz plate if the first plate is vibrating. The ESP32 would need to perform a sequence of precise operations:
    
    1. **Read Control Qubit:** The ESP32 would briefly activate the laser interferometer for the first quartz plate. It would analyze the interference pattern to theoretically determine if the plate is in the ∣0⟩ or ∣1⟩ state.
        
    2. **Conditional Pulse:** Based on the measurement, the ESP32 would decide whether to send a pulse to the second quartz plate.
        
    3. **Perform X-Gate (if necessary):** If the measurement shows the control qubit is in the ∣1⟩ state, the ESP32 would command the AD9850 to send a precise RF pulse (a "pi-pulse") to the second plate, effectively flipping its state.
        
- **CCD Measurement Setup:** We would use a camera, like a **Sony a7III**, to capture the interference pattern from the interferometer. The lens would be removed to get the pattern directly on the **CCD sensor**. The camera would then stream the live video data to a computer, where a theoretical **AI** could be used to analyze the pattern to determine the quantum state.
    

---

### Final Implementation: Deutsch-Jozsa Algorithm

This code sketch combines all our gate and measurement ideas to perform the full algorithm.

C++

```
// Theoretical Code for the Deutsch-Jozsa Algorithm
// This is a high-level conceptual sketch.
// Assumes functions for our gates and measurement system exist.

#include <Arduino.h>

// --- Theoretical Hardware Definitions ---
// Define our two qubits
#define QUBIT_1_ID 1
#define QUBIT_2_ID 2

// Define the type of oracle function we're testing
// 0 = constant, 1 = balanced
#define ORACLE_TYPE 1

// Define the precise pulse durations for our gates
#define H_PULSE_DURATION_US 25 // For Hadamard gate (Pi/2 pulse)
#define NOT_PULSE_DURATION_US 50 // For NOT gate (Pi pulse)

// --- Theoretical Function Definitions ---
// These functions are placeholders for the real-world hardware control.

// Function to control the AD9850 to send a pulse to a specific qubit.
void ad9850_pulse_on_qubit(int qubit_id, int duration_us) {
  // Logic to select the correct piezo disc and send the pulse.
  // This would involve controlling digital pins to route the signal.
}

// Function to perform a theoretical quantum measurement.
int theoretical_measure_qubit(int qubit_id) {
  // This would involve triggering our laser interferometer and
  // analyzing the output from the sensor.
  // It collapses the qubit's superposition to a classical 0 or 1.
  return 0; // The theoretical result from the sensor.
}

// --- Quantum Gates ---
void hadamard_gate(int qubit_id) {
  ad9850_pulse_on_qubit(qubit_id, H_PULSE_DURATION_US);
}

void cnot_gate(int control_qubit_id, int target_qubit_id) {
  int control_state = theoretical_measure_qubit(control_qubit_id);

  if (control_state == 1) {
    ad9850_pulse_on_qubit(target_qubit_id, NOT_PULSE_DURATION_US);
  }
}

// --- The Quantum Oracle ---
void theoretical_oracle(int control_qubit_id, int target_qubit_id, int oracle_type) {
  if (oracle_type == 0) { // Constant function
    // Do nothing. The qubit states are preserved.
  } else if (oracle_type == 1) { // Balanced function
    cnot_gate(control_qubit_id, target_qubit_id);
  }
}

// --- Main Program ---
void setup() {
  Serial.begin(115200);
  Serial.println("Starting Deutsch-Jozsa Algorithm...");
  // setupAD9850();
}

void loop() {
  // Step 1: Initialize the qubits.
  hadamard_gate(QUBIT_1_ID);
  hadamard_gate(QUBIT_2_ID);
  Serial.println("Qubits initialized into superposition.");

  // Step 2: Apply the quantum oracle.
  theoretical_oracle(QUBIT_1_ID, QUBIT_2_ID, ORACLE_TYPE);
  Serial.println("Quantum oracle applied.");

  // Step 3: Apply Hadamard gates again.
  hadamard_gate(QUBIT_1_ID);
  hadamard_gate(QUBIT_2_ID);
  Serial.println("Final Hadamard gates applied.");

  // Step 4: Measure the final state.
  int result = theoretical_measure_qubit(QUBIT_1_ID);

  // Step 5: Interpret the result.
  if (result == 0) {
    Serial.println("Measurement result is 0. The function is CONSTANT.");
  } else {
    Serial.println("Measurement result is 1. The function is BALANCED.");
  }
  
  while(true);
}
```
### Challenges and Creative Solutions

We have addressed several major theoretical challenges with creative, physics-based solutions:

- **Qubit Isolation:** We would use precise frequency tuning to address a single vibrational mode within each crystal, avoiding unwanted excitations.
    
- **Decoherence:** We would use theoretical solutions like **cryogenics** (cooling the system to near absolute zero) and a vacuum chamber to protect the qubits from environmental noise.
    
- **Physical Implementation:** We would map the steps of the Deutsch-Jozsa algorithm to a sequence of classical commands sent from the ESP32 to the AD9850.
