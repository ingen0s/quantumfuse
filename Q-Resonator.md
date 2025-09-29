### Project Q-Resonator: A Theoretical Quantum Computer

#### Overview

This project is a theoretical exploration into building a two-qubit quantum computer using a green diode laser, an ESP32 microcontroller, an AD9850 DDS module, and two separate synthetic quartz crystals. We hypothesize that the quantized mechanical vibrations of the quartz crystals can serve as our qubits, which are controlled and measured using our available components.

---

#### Core Components ( First run setup )

- **ICE40UP5K-B-EVN** Serves as the classical control unit. It sends precise timing signals and commands to the AD9850 to execute quantum gates.
     - Initially we were using an ESP32 as it advertises a fine-grained timing resolution of 1μs (theoretical limit of the core timer). The practical limit we learned is 20μs and 50μs.
     - Switching over to FPGA board for faster control
    
- **AD9850 DDS Module:** Generates the radio frequency (RF) signals that will drive the piezoelectric vibrations of the quartz.
    
- **Green Diode Laser:** Used for the theoretical **interferometric measurement** of the qubit's vibrational state.
    
- **Piezoelectric Pickup Discs:** Two discs, each acting as both an actuator (to initiate vibrations) and a sensor (to read back signals) for a separate quartz plate.
    
- **Synthetic Quartz Crystals:** Two separate plates, each **50mm x 50mm x 1mm**, serving as the material for our qubits.


---

#### Theoretical Qubit Design

Each quartz plate has a single, isolated **mechanical vibrational mode** that acts as our qubit. The two quantum states, ∣0⟩ and ∣1⟩, would correspond to the ground state of vibration and the first excited state (a single phonon). The 1mm thickness of the plates gives them a fundamental resonant frequency in the tested +200mHz-300kHz and possibly up to the MHz range, which is perfectly suited for the AD9850.

---

#### Quantum Operations

- **Single-Qubit Gates:** The ICE40UP5K-B-EVN would command the AD9850 to produce a precise RF pulse with a specific duration and frequency, which is sent to one of the piezoelectric discs. This pulse would cause that quartz plate to vibrate, theoretically rotating the qubit's state.
- **Polality-Controlled Gates**: We would use the ICE40UP5K-B-EVN to control the polarity of the signal sent to the piezo disc. By switching the ground and AC connections, we could reverse the force applied to the quartz plate, causing it to compress instead of stretch (or vice-versa). This would allow us to create a new type of quantum gate that manipulates the qubit's state in a new way.
- **Two-Qubit Gates:** We would make the two separate quartz plates interact. For example, by pulsing one qubit to create a specific vibrational state, we could theoretically couple it to the other plate, performing a controlled gate like a CNOT. The ESP32 would also need to independently switch between the two plates to address them individually.
    

---

#### Quantum Algorithm

Our theoretical quantum computer would run the **Deutsch-Jozsa algorithm**. The ICE40UP5K-B-EVN would coordinate the sequence of RF pulses and laser measurements to determine whether a hypothetical black-box function is "constant" or "balanced" in a single run.

---

#### Experiment Update Log

- **September 17, 2025:** I hooked up one piezo pickup to one plate and set the **ICL8038** to cycle at 10 Hz. I shined a green laser diode onto the plate at an angle to reflect to a piece of paper. Each time the cycle turned to 10 Hz, the laser projection shifted partially, and each time it returned to 0 Hz, it shifted back. The shifts are almost perfect with slight variations. We also discovered that using a square wave from the ICL8038 had a more noticeable effect than a sine wave because it excited multiple harmonics.
    
- **September 18, 2025:** We've introduced a new theoretical concept: using a second, smaller quartz crystal with its own piezo pickup as a **quantum beam splitter**. By carefully pulsing this crystal with the **AD9850**, its vibrations would alter the phase of the laser light reflecting off it. This allows us to perform a **quantum gate directly on the photon**, which adds a new dimension to our quantum operations.

- **September 20, 2025:** Realized that we can switch the polarity of the crystal, allowing for 4 states on one crystal 'bed'.
    - Idle (Positive Polarity): The crystal is in its ground vibrational state (∣0⟩) and the piezo is configured for a positive-going force (stretch).
    - Excited (Positive Polarity): The crystal is in its excited vibrational state (∣1⟩) and the piezo is still configured for a positive-going force.
    - Idle (Negative Polarity): The crystal is in its ground vibrational state (∣0⟩) and the piezo is configured for a negative-going force (compression).
    - Excited (Negative Polarity): The crystal is in its excited vibrational state (∣1⟩) and the piezo is configured for a negative-going force.
- **September 21, 2025:** Successfully created a Gaussian splat with nerfstudio.
    - The idea is to be able to map the entire surface are of the crystal in all 3 states (+, -, idle).
    - Each crystal has a unique set of quantum wells that we can plot coordinates for.
- **September 24-26, 2025:** Starting creating piezo arrays for larger surface area stimulation.
    - 1 piezo on crystal in center.
    - 2 piezos arranged with grounds pointed towards center.
    - 4 modified piezos arranged with grounds pointed towards center.
        - Clipped on both x and y outer edges, so they fit snug in the middle of the plate.

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
        
- **CCD Measurement Setup:** We would use a camera, like a **OV9281** with a global shutter and monochromatic, to capture the interference pattern from the interferometer. The lens would be removed to get the pattern directly on the **CCD sensor** and a fixed neutral density filter to make sure we dont damage the sensor . The camera would then stream the live video data to a computer, where a theoretical **AI** could be used to analyze the pattern to determine the quantum state.
- **Gaussian Splatting Measurement:** We would use a camera, like a Sony a7III, to capture live video of the laser beam passing through the crystal. A theoretical AI would then use Gaussian Splatting to create a 3D map of the crystal's vibrating lattice, providing a real-time view of the quantum state. We would also perform a separate Gaussian Splatting for the "on" and "off" states of the qubit and layer them to visually represent the state difference.

### Characterizing the System and Timing Quantum Gates

- **Rabi Oscillation:** We would send a series of progressively longer RF pulses from the **AD9850** to one of our quartz plates. After each pulse, the **ESP32** would use the interferometer and camera setup to measure the state of the qubit. We would plot the probability of the qubit being in the **|1>** state against the duration of the pulse. As the pulse duration increases, the qubit would cycle between **|0>** and **|1>**, creating a sine wave pattern known as a **Rabi oscillation**.
    
- **Determining Timing:** A **Hadamard gate** requires a "pi-over-2" (**π/2**) pulse, which is exactly a quarter of a full oscillation. A **NOT gate** requires a "pi" (**π**) pulse, which is half of a full oscillation. By analyzing the Rabi oscillation plot, we would find the exact pulse durations needed for all of our gates.

---
### Grover's Algorithm

**Grover's algorithm** is a powerful quantum algorithm for searching an unsorted database. It finds a specific item in far fewer steps than a classical computer. The algorithm has three key parts that we'll implement with our components:

1. **Initialization:** We start by applying **Hadamard gate** pulses to both quartz plates. This puts both qubits into a superposition state, making all four possible states—∣00⟩, ∣01⟩, ∣10⟩, and ∣11⟩—equally possible.
    
2. **The Quantum Oracle:** This is a special gate that "marks" the solution state. It inverts the amplitude of the solution state without changing the others, which is a very complex sequence involving our **CNOT gates** and single-qubit rotations.
    
3. **The Amplification Step:** We then perform a series of pulses known as a **Grover diffusion operator**. This operator amplifies the probability of measuring the marked solution state. After a few rounds of the oracle and amplification steps, the probability of measuring the correct answer is significantly increased.
    

After these steps, we take a single measurement on both qubits to get our answer.

### Grover's Algorithm Code Sketch (Initialization)

C++

```
// Theoretical Code for Grover's Algorithm Initialization
// This sketch sets up our qubits for the search.

#include <Arduino.h>

// --- Theoretical Hardware Definitions ---
#define QUBIT_1_ID 1
#define QUBIT_2_ID 2

// Assumes the hadamard_gate function is defined elsewhere
void hadamard_gate(int qubit_id);

void initialize_grover_qubits() {
  // Step 1: Initialize the qubits.
  hadamard_gate(QUBIT_1_ID);
  hadamard_gate(QUBIT_2_ID);
  Serial.println("Grover qubits initialized into superposition.");
}

void setup() {
  Serial.begin(115200);
  initialize_grover_qubits();
}

void loop() {
  // Main loop can be empty after initialization
}
```

### The Quantum Oracle

**The Quantum Oracle** is a special gate that "marks" the solution to our search problem. In our two-qubit system, it inverts the amplitude of the solution state without changing the others, which is a crucial step that the rest of the algorithm builds on. It is a specially designed quantum circuit made of our **CNOT** and other single-qubit gates.

- **Solution Identification:** The **ESP32** would run a series of pulses that, if the qubits are in the solution state, will apply a **NOT gate** to an auxiliary qubit (a third vibrational mode on one of our plates).
    
- **Phase Flip:** This conditional flip would then be used to apply a phase flip to the solution state. The phase flip is a subtle change in the qubit's quantum state that we can't observe directly, but it is what makes the oracle work.
    
- **Uncomputing:** Finally, the **ESP32** would run the entire circuit in reverse to put the auxiliary qubit back into its original state, leaving only the solution state with its phase flipped.
    

### Theoretical Quantum Oracle Code Sketch

Here is a conceptual code sketch that shows how the **ESP32** would run the quantum oracle. It assumes we have functions for our **CNOT** gate and a way to apply a phase flip.

C++

```
// Theoretical Code for a Quantum Oracle
// This function "marks" the solution state for Grover's algorithm.

// Assumes a NOT gate is defined elsewhere.
void not_gate(int qubit_id);

// The quantum oracle function.
void theoretical_oracle(int control_qubit_id, int target_qubit_id, int solution_state_to_mark) {
    // Step 1: Prepare the qubits to perform the phase flip.
    // This is a complex series of pulses. For our sketch, we can assume a
    // series of Hadamard and CNOT gates.

    // Step 2: Conditionally apply a NOT gate to "mark" the solution.
    if (solution_state_to_mark == 1) {
        // If the solution is state |1>, we apply a conditional NOT gate.
        cnot_gate(control_qubit_id, target_qubit_id);
    }

    // Step 3: "Uncompute" the preparation.
    // The pulses are run in reverse to put the qubits back into their original state,
    // leaving only the solution state with its phase flipped.
}
```
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
