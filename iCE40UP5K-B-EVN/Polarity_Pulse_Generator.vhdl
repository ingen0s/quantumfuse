module polarity_pulse_generator (
    input wire clk,
    input wire reset_n,
    input wire start_pulse,
    input wire pulse_polarity, // 0 = positive, 1 = negative
    output reg ad9850_pulse_pos,
    output reg ad9850_pulse_neg
);
    // Internal counter for pulse duration
    reg [19:0] pulse_counter;
    
    // State machine to manage the pulse
    // ... (logic from previous examples) ...
    
    always @(posedge clk) begin
        if (!reset_n) begin
            pulse_counter <= 20'b0;
            ad9850_pulse_pos <= 1'b0;
            ad9850_pulse_neg <= 1'b0;
        end else if (start_pulse) begin
            if (pulse_counter < H_PULSE_CYCLES) begin
                if (pulse_polarity == 1'b0) begin
                    ad9850_pulse_pos <= 1'b1;
                    ad9850_pulse_neg <= 1'b0;
                end else begin
                    ad9850_pulse_pos <= 1'b0;
                    ad9850_pulse_neg <= 1'b1;
                end
                pulse_counter <= pulse_counter + 1;
            end else begin
                ad9850_pulse_pos <= 1'b0;
                ad9850_pulse_neg <= 1'b0;
                pulse_counter <= 20'b0;
            end
        end
    end
endmodule
