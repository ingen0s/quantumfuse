### Project Q-Resonator: A Theoretical Quantum Computer

#### Overview

This project is a theoretical exploration into building a two-qubit quantum computer using a green diode laser, an **iCE40UP5K-B-EVN FPGA**, an AD9850 DDS module, and two separate synthetic quartz crystals. We hypothesize that the quantized mechanical vibrations of the quartz crystals can serve as our qubits, which are controlled and measured using our available components.

Cryogenics and vaccuum chamber strategy and design is in the works and will meet the criterea of accessibility and breakout standard.

---

#### Core Components (First run setup)

- **iCE40UP5K-B-EVN FPGA:** Serves as the classical control unit. It sends precise timing signals and commands to the AD9850 to execute quantum gates.
    
    - Initially, we were using an ESP32 as it advertises a fine-grained timing resolution of 1μs (theoretical limit of the core timer). The practical limit we learned is 20μs and 50μs.
        
    - Switching over to an FPGA board for faster control.
        
- **AD9850 DDS Module:** Generates the radio frequency (RF) signals that will drive the piezoelectric vibrations of the quartz.
    
- **Green Diode Laser:** Used for the theoretical **interferometric measurement** of the qubit's vibrational state.
    
- **Piezoelectric Pickup Discs:** Two discs, each acting as both an actuator (to initiate vibrations) and a sensor (to read back signals) for a separate quartz plate.
    
- **Synthetic Quartz Crystals:** Two separate plates, each **50mm x 50mm x 1mm**, serving as the material for our qubits.
    

---

#### Theoretical Qubit Design

Each quartz plate has a single, isolated **mechanical vibrational mode** that acts as our qubit. The two quantum states, ∣0⟩ and ∣1⟩, would correspond to the ground state of vibration and the first excited state (a single phonon). The 1mm thickness of the plates gives them a fundamental resonant frequency in the tested +200mHz-300kHz and possibly up to the MHz range, which is perfectly suited for the AD9850.

---

#### Quantum Operations

- **Single-Qubit Gates:** The **iCE40UP5K-B-EVN** would command the AD9850 to produce a precise RF pulse with a specific duration and frequency, which is sent to one of the piezoelectric discs. This pulse would cause that quartz plate to vibrate, theoretically rotating the qubit's state.
    
- **Polality-Controlled Gates**: We would use the **iCE40UP5K-B-EVN** to control the polarity of the signal sent to the piezo disc. By switching the ground and AC connections, we could reverse the force applied to the quartz plate, causing it to compress instead of stretch (or vice-versa). This would allow us to create a new type of quantum gate that manipulates the qubit's state in a new way.
    
- **Two-Qubit Gates:** We would make the two separate quartz plates interact. For example, by pulsing one qubit to create a specific vibrational state, we could theoretically couple it to the other plate, performing a controlled gate like a CNOT. The **iCE40UP5K-B-EVN** would also need to independently switch between the two plates to address them individually.
    

---

#### Quantum Algorithm

Our theoretical quantum computer would run the **Deutsch-Jozsa algorithm**. The **iCE40UP5K-B-EVN** would coordinate the sequence of RF pulses and laser measurements to determine whether a hypothetical black-box function is "constant" or "balanced" in a single run.

---

#### Experiment Update Log

- **September 17, 2025:** I hooked up one piezo pickup to one plate and set the **ICL8038** to cycle at 10 Hz. I shined a green laser diode onto the plate at an angle to reflect to a piece of paper. Each time the cycle turned to 10 Hz, the laser projection shifted partially, and each time it returned to 0 Hz, it shifted back. The shifts are almost perfect with slight variations. We also discovered that using a square wave from the ICL8038 had a more noticeable effect than a sine wave because it excited multiple harmonics.
    
- **September 18, 2025:** We've introduced a new theoretical concept: using a second, smaller quartz crystal with its own piezo pickup as a **quantum beam splitter**. By carefully pulsing this crystal with the **AD9850**, its vibrations would alter the phase of the laser light reflecting off it. This allows us to perform a **quantum gate directly on the photon**, which adds a new dimension to our quantum operations.
    
- **September 20, 2025:** We've refined our understanding of how polarity control can be used to manipulate the qubit's state. While we initially theorized that polarity reversal could create a four-state qudit, we now understand that the quantum system remains a two-level qubit, defined by the energy eigenstates of the ground state (∣0⟩) and the first excited state (∣1⟩).

    Instead of creating new quantum states, polarity control offers a powerful new way to manipulate the existing ones. We can now precisely control the direction and phase of the applied mechanical force, allowing us to:
    
    1. Apply a positive pulse to induce a state change through a stretching force.
    
    2. Apply a negative pulse to induce the same state change through a compressive force.
    
    This gives us finer control over the quantum operations and may be instrumental in exploring advanced techniques such as squeezed states, where we manipulate the quantum noise of the system.
        
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

- **Hadamard Gate Sketch:** This conceptual code outlines how the **iCE40UP5K-B-EVN** would command the AD9850 to create a quantum gate pulse, specifically a **Hadamard gate** which puts a qubit into a superposition.
    
    Verilog
    
    Verilog
    
    ```
    // Verilog Code for a Hadamard Gate Pulse on iCE40UP5K
    // This code describes the hardware for a precise pulse.
    
    // Define the clock frequency of the FPGA (e.g., 12 MHz on the iCE40UP5K board)
    `define CLK_FREQ 12_000_000
    // Define the duration of the Hadamard pulse in clock cycles
    // 25 us * 12 MHz = 300 cycles
    `define H_PULSE_CYCLES 300
    
    module hadamard_pulse_generator (
        // Clock input from the iCE40 board
        input wire clk,
        // Reset signal (active low)
        input wire reset_n,
        // Input to trigger the pulse
        input wire start_pulse,
        // Output pin to enable the AD9850 (active low)
        output reg ad9850_pd_n
    );
    
        // A state machine to manage the pulse sequence
        localparam STATE_IDLE = 2'b00;
        localparam STATE_PULSING = 2'b01;
        localparam STATE_WAIT = 2'b10;
    
        reg [1:0] state;
        reg [19:0] pulse_counter;
        reg [19:0] wait_counter;
    
        always @(posedge clk) begin
            if (!reset_n) begin
                state <= STATE_IDLE;
                ad9850_pd_n <= 1'b1; // AD9850 is powered down by default
                pulse_counter <= 20'b0;
                wait_counter <= 20'b0;
            end else begin
                case (state)
                    STATE_IDLE: begin
                        ad9850_pd_n <= 1'b1;
                        if (start_pulse) begin
                            state <= STATE_PULSING;
                            pulse_counter <= 20'b0;
                        end
                    end
    
                    STATE_PULSING: begin
                        ad9850_pd_n <= 1'b0; // Power up AD9850
                        if (pulse_counter < `H_PULSE_CYCLES) begin
                            pulse_counter <= pulse_counter + 1;
                        end else begin
                            state <= STATE_WAIT;
                            ad9850_pd_n <= 1'b1; // Power down
                            wait_counter <= 20'b0;
                        end
                    end
    
                    STATE_WAIT: begin
                        if (wait_counter < `CLK_FREQ / 10) begin // Example: Wait for 100ms
                            wait_counter <= wait_counter + 1;
                        end else begin
                            state <= STATE_IDLE;
                        end
                    end
                endcase
            end
        end
    
    endmodule
    ```
    

---

- **Two-Qubit CNOT Gate Sketch:** A **Controlled-NOT (CNOT) gate** flips the state of a "target" qubit if and only if the "control" qubit is in the ∣1⟩ state. In our theoretical setup, this means we would only pulse the second quartz plate if the first plate is vibrating. The **iCE40UP5K-B-EVN** would need to perform a sequence of precise operations:
    
    1. **Read Control Qubit:** The **iCE40UP5K-B-EVN** would briefly activate the laser interferometer for the first quartz plate. It would analyze the interference pattern to theoretically determine if the plate is in the ∣0⟩ or ∣1⟩ state. This would likely involve a custom hardware module to process the camera's data stream.
        
    2. **Conditional Pulse:** Based on the measurement, the FPGA's logic would decide whether to send a pulse to the second quartz plate.
        
    3. **Perform X-Gate (if necessary):** If the measurement shows the control qubit is in the ∣1⟩ state, the FPGA would command the AD9850 to send a precise RF pulse (a "pi-pulse") to the second plate, effectively flipping its state.
        
### Measurement System

Our project's measurement system is a critical component for observing the qubit's state. It will leverage an interferometer and advanced computational techniques to overcome environmental noise and capture the subtle changes in the quartz crystal's vibrations.

- **The Interferometer:** We will use a laser to create an interference pattern that is highly sensitive to the mechanical vibrations of our quartz crystal. The laser beam will be split, with one part acting as a reference and the other reflecting off the vibrating crystal. When the beams recombine, any change in the crystal's position will cause a shift in the interference pattern.
    
- **The Sensor:** We will use a camera, like an **OV9281** with a global shutter, to capture the interference pattern directly on its monochromatic CCD sensor. To ensure a clear image, the lens will be removed, and a fixed neutral density filter will be used to protect the sensor from the laser. The camera will stream the raw video data at a high frame rate to a PC workstation for real-time analysis.
    
- **Data Processing and Noise Reduction:** On the PC workstation, we will use a dedicated software pipeline to analyze the incoming video data. This is where we will employ **machine learning and advanced mathematical techniques to decouple interference and environmental noise** from the signal. This approach is essential to make the system robust enough to detect the subtle, quantum-relevant vibrations of the quartz crystal.
    
- **Quantum State Measurement:** Once the noise is filtered, we will analyze the clean signal to determine the quantum state of the qubit. The frequency and phase of the signal will be correlated to the vibrational state of the crystal. By analyzing this data, we can collapse the qubit's superposition and measure its state as a classical 0 or 1.
    
- **Gaussian Splatting Measurement:** We would use a camera, like a OV9281I, to capture live video of the laser beam passing through the crystal. A theoretical AI would then use Gaussian Splatting to create a 3D map of the crystal's vibrating lattice, providing a real-time view of the quantum state. We would also perform a separate Gaussian Splatting for the "on" and "off" states of the qubit and layer them to visually represent the state difference.
    

### Characterizing the System and Timing Quantum Gates

- **Rabi Oscillation:** We would send a series of progressively longer RF pulses from the **AD9850** to one of our quartz plates. After each pulse, the **iCE40UP5K-B-EVN** would use the interferometer and camera setup to measure the state of the qubit. We would plot the probability of the qubit being in the **|1>** state against the duration of the pulse. As the pulse duration increases, the qubit would cycle between **|0>** and **|1>**, creating a sine wave pattern known as a **Rabi oscillation**.
    
- **Determining Timing:** A **Hadamard gate** requires a "pi-over-2" (**π/2**) pulse, which is exactly a quarter of a full oscillation. A **NOT gate** requires a "pi" (**π**) pulse, which is half of a full oscillation. By analyzing the Rabi oscillation plot, we would find the exact pulse durations needed for all of our gates.
    

---

### Grover's Algorithm

**Grover's algorithm** is a powerful quantum algorithm for searching an unsorted database. It finds a specific item in far fewer steps than a classical computer. The algorithm has three key parts that we'll implement with our components:

1. **Initialization:** We start by applying **Hadamard gate** pulses to both quartz plates. This puts both qubits into a superposition state, making all four possible states—∣00⟩, ∣01⟩, ∣10⟩, and ∣11⟩—equally possible.
    
2. **The Quantum Oracle:** This is a special gate that "marks" the solution state. It inverts the amplitude of the solution state without changing the others, which is a very complex sequence involving our **CNOT gates** and single-qubit rotations.
    
3. **The Amplification Step:** We then perform a series of pulses known as a **Grover diffusion operator**. This operator amplifies the probability of measuring the marked solution state. After a few rounds of the oracle and amplification steps, the probability of measuring the correct answer is significantly increased.
    

After these steps, we take a single measurement on both qubits to get our answer.

### Grover's Algorithm Code Sketch (Initialization)

Verilog

Verilog

```
// Verilog Code for Grover's Algorithm Initialization
// This is a high-level conceptual sketch.
// It will be a state machine on the FPGA.

module grover_initializer (
    input wire clk,
    input wire reset_n,
    input wire start_init,
    output wire ad9850_pulse_qubit1,
    output wire ad9850_pulse_qubit2,
    output wire init_done
);

    // States for the state machine
    localparam STATE_IDLE = 2'b00;
    localparam STATE_PULSE_Q1 = 2'b01;
    localparam STATE_PULSE_Q2 = 2'b10;
    localparam STATE_DONE = 2'b11;

    reg [1:0] state;
    reg pulse_q1_reg;
    reg pulse_q2_reg;
    reg init_done_reg;

    // Define the module for the Hadamard pulse from our earlier sketch
    // This will be instantiated later.
    // hadamard_pulse_generator H1 (...);
    // hadamard_pulse_generator H2 (...);

    always @(posedge clk) begin
        if (!reset_n) begin
            state <= STATE_IDLE;
            pulse_q1_reg <= 1'b0;
            pulse_q2_reg <= 1'b0;
            init_done_reg <= 1'b0;
        end else begin
            case (state)
                STATE_IDLE: begin
                    if (start_init) begin
                        state <= STATE_PULSE_Q1;
                        pulse_q1_reg <= 1'b1; // Trigger the first Hadamard pulse
                    end
                end

                STATE_PULSE_Q1: begin
                    pulse_q1_reg <= 1'b0; // De-assert the trigger
                    // This is where we'd wait for the Hadamard pulse to finish
                    // using a signal from the hadamard_pulse_generator module.
                    // For this sketch, we move to the next state immediately.
                    state <= STATE_PULSE_Q2;
                end

                STATE_PULSE_Q2: begin
                    pulse_q2_reg <= 1'b1; // Trigger the second Hadamard pulse
                    state <= STATE_DONE;
                end
                
                STATE_DONE: begin
                    pulse_q2_reg <= 1'b0; // De-assert the trigger
                    init_done_reg <= 1'b1;
                end
            endcase
        end
    end

    // Assign the internal registers to the output ports
    assign ad9850_pulse_qubit1 = pulse_q1_reg;
    assign ad9850_pulse_qubit2 = pulse_q2_reg;
    assign init_done = init_done_reg;

endmodule
```

### The Quantum Oracle

**The Quantum Oracle** is a special gate that "marks" the solution to our search problem. In our two-qubit system, it inverts the amplitude of the solution state without changing the others, which is a crucial step that the rest of the algorithm builds on. It is a specially designed quantum circuit made of our **CNOT** and other single-qubit gates.

- **Solution Identification:** The **iCE40UP5K-B-EVN** would run a series of pulses that, if the qubits are in the solution state, will apply a **NOT gate** to an auxiliary qubit (a third vibrational mode on one of our plates).
    
- **Phase Flip:** This conditional flip would then be used to apply a phase flip to the solution state. The phase flip is a subtle change in the qubit's quantum state that we can't observe directly, but it is what makes the oracle work.
    
- **Uncomputing:** Finally, the **iCE40UP5K-B-EVN** would run the entire circuit in reverse to put the auxiliary qubit back into its original state, leaving only the solution state with its phase flipped.
    

### Theoretical Quantum Oracle Code Sketch

Here is a conceptual code sketch that shows how the **iCE40UP5K-B-EVN** would run the quantum oracle. It assumes we have functions for our **CNOT** gate and a way to apply a phase flip.

Verilog

Verilog

```
// Theoretical Verilog Code for a Quantum Oracle
// This module implements the conditional logic for the oracle.

module quantum_oracle (
    input wire clk,
    input wire reset_n,
    input wire start_oracle,
    // Input from the measurement system to determine control qubit state
    input wire control_qubit_state,
    output wire ad9850_pulse_qubit_target
);

    localparam STATE_IDLE = 2'b00;
    localparam STATE_CHECK = 2'b01;
    localparam STATE_CNOT = 2'b10;

    reg [1:0] state;
    reg pulse_target_reg;
    
    // CNOT gate is implemented as a conditional NOT pulse on the target qubit
    // This is a simplified representation.
    // The actual cnot_pulse_generator would need to be instantiated.
    // cnot_pulse_generator CNOT_GEN (...);

    always @(posedge clk) begin
        if (!reset_n) begin
            state <= STATE_IDLE;
            pulse_target_reg <= 1'b0;
        end else begin
            case (state)
                STATE_IDLE: begin
                    if (start_oracle) begin
                        state <= STATE_CHECK;
                    end
                end

                STATE_CHECK: begin
                    if (control_qubit_state == 1) begin
                        state <= STATE_CNOT;
                        pulse_target_reg <= 1'b1; // Trigger a NOT pulse on the target qubit
                    end else begin
                        state <= STATE_IDLE;
                    end
                end

                STATE_CNOT: begin
                    pulse_target_reg <= 1'b0; // De-assert the trigger
                    // Wait for the pulse to finish, then go back to idle.
                    state <= STATE_IDLE;
                end
            endcase
        end
    end

    assign ad9850_pulse_qubit_target = pulse_target_reg;
endmodule
```

---

### Final Implementation: Deutsch-Jozsa Algorithm

This Verilog code sketch combines all our gate and measurement ideas into a top-level module to perform the full algorithm. The entire process is described as a **state machine** that moves from initialization to measurement.

Verilog

Verilog

```
// Theoretical Verilog Code for the Deutsch-Jozsa Algorithm
// This is the top-level module coordinating the entire algorithm.

module deutsch_jozsa_main (
    input wire clk,
    input wire reset_n,
    input wire start_algorithm,
    // Placeholder for measurement system input
    input wire [1:0] qubit_states_in,
    // Outputs for AD9850 control
    output wire ad9850_pulse_q1,
    output wire ad9850_pulse_q2
);

    // Define states for the algorithm
    localparam STATE_IDLE = 3'b000;
    localparam STATE_INIT = 3'b001;
    localparam STATE_ORACLE = 3'b010;
    localparam STATE_HADAMARD_FINAL = 3'b011;
    localparam STATE_MEASURE = 3'b100;
    localparam STATE_DONE = 3'b101;

    reg [2:0] state;
    reg start_init_reg;
    reg start_oracle_reg;
    reg start_hadamard_final_reg;
    reg start_measure_reg;

    // Instantiate our sub-modules (conceptual)
    // We would connect these to the AD9850 pins and other hardware
    //hadamard_pulse_generator H1_GEN (...);
    //hadamard_pulse_generator H2_GEN (...);
    //quantum_oracle ORACLE_GEN (...);

    always @(posedge clk) begin
        if (!reset_n) begin
            state <= STATE_IDLE;
            start_init_reg <= 1'b0;
            start_oracle_reg <= 1'b0;
            start_hadamard_final_reg <= 1'b0;
            start_measure_reg <= 1'b0;
        end else begin
            case (state)
                STATE_IDLE: begin
                    if (start_algorithm) begin
                        state <= STATE_INIT;
                        start_init_reg <= 1'b1; // Trigger initialization
                    end
                end

                STATE_INIT: begin
                    start_init_reg <= 1'b0; // De-assert trigger
                    // We'd wait for a signal from the initializer module here
                    state <= STATE_ORACLE;
                    start_oracle_reg <= 1'b1;
                end

                STATE_ORACLE: begin
                    start_oracle_reg <= 1'b0;
                    // Wait for oracle completion
                    state <= STATE_HADAMARD_FINAL;
                    start_hadamard_final_reg <= 1'b1;
                end
                
                STATE_HADAMARD_FINAL: begin
                    start_hadamard_final_reg <= 1'b0;
                    // Wait for final Hadamards to finish
                    state <= STATE_MEASURE;
                    start_measure_reg <= 1'b1;
                end
                
                STATE_MEASURE: begin
                    // Read the `qubit_states_in` input
                    // Logic to check if the function is constant or balanced
                    if (qubit_states_in[0] == 1) begin
                        // Function is balanced
                    end else begin
                        // Function is constant
                    end
                    state <= STATE_DONE;
                end

                STATE_DONE: begin
                    // The algorithm is complete, hold here until reset.
                end
            endcase
        end
    end

    // Assign outputs based on state
    assign ad9850_pulse_q1 = (state == STATE_INIT || state == STATE_HADAMARD_FINAL);
    assign ad9850_pulse_q2 = (state == STATE_INIT || state == STATE_HADAMARD_FINAL);

endmodule
```

---

### Challenges and Creative Solutions

We have addressed several major theoretical challenges with creative, physics-based solutions:

- **Qubit Isolation:** We would use precise frequency tuning to address a single vibrational mode within each crystal, avoiding unwanted excitations.
    
- **Decoherence:** We would use theoretical solutions like **cryogenics** (cooling the system to near absolute zero) and a vacuum chamber to protect the qubits from environmental noise.
    
- **Physical Implementation:** We would map the steps of the Deutsch-Jozsa algorithm to a sequence of classical commands sent from the **iCE40UP5K-B-EVN** to the AD9850.
