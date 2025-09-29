// Proposed Verilog sketch module to receive commands from the ESP32
module esp32_interface (
    input wire clk,
    input wire reset_n,
    // A data bus from the ESP32 (e.g., 8-bit)
    input wire [7:0] esp32_command,
    // A pulse from the ESP32 to signal a new command is ready
    input wire command_valid,
    // Outputs to trigger the quantum gate modules
    output reg start_hadamard_q1,
    output reg start_hadamard_q2,
    output reg start_cnot
);

    // Register to store the received command
    reg [7:0] current_command;
    
    // Simple state machine to handle the incoming command
    always @(posedge clk) begin
        if (!reset_n) begin
            start_hadamard_q1 <= 1'b0;
            start_hadamard_q2 <= 1'b0;
            start_cnot <= 1'b0;
        end else begin
            // If the ESP32 sends a new command
            if (command_valid) begin
                current_command <= esp32_command;
                // De-assert all start signals to avoid re-triggering
                start_hadamard_q1 <= 1'b0;
                start_hadamard_q2 <= 1'b0;
                start_cnot <= 1'b0;

                // Check the command word and set the appropriate flag
                case (current_command)
                    // Define a command for a Hadamard gate on Qubit 1
                    8'h01: begin
                        start_hadamard_q1 <= 1'b1;
                    end
                    // Define a command for a Hadamard gate on Qubit 2
                    8'h02: begin
                        start_hadamard_q2 <= 1'b1;
                    end
                    // Define a command for a CNOT gate
                    8'h03: begin
                        start_cnot <= 1'b1;
                    end
                    default: begin
                        // Do nothing for unrecognized commands
                    end
                endcase
            end
        end
    end
endmodule
