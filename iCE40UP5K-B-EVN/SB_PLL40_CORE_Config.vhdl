// The iCE40UP5K internal clock is 48 MHz, which means its smallest time step is roughly 20.8 nanoseconds (1 / 48 MHz)
// We will need something faster but this will do for now.
// Top-level module for the iCE40UP5K-B-EVN board
module top_module (
    // The main 12 MHz clock input from the board's oscillator
    input wire SB_CLK_12MHZ,
    // The reset signal
    input wire SB_RESET_N,

    // The high-speed clock output from the PLL, used to drive all logic
    output wire FAST_CLK_48MHZ,
    // Other project I/O will go here
    output wire AD9850_PULSE
);

    // Wire to connect the PLL's output to the rest of our logic
    wire pll_out;

    // Instantiate the SB_PLL40_CORE primitive
    SB_PLL40_CORE #(
        .DIVR(4'b0000),      // Input Pre-divider (DIVR + 1 = 1, so no division)
        .DIVF(7'b0000011),  // Feedback Divider (DIVF + 1 = 4)
        .DIVQ(3'b001),      // Output Divider (DIVQ + 1 = 2)
        .FILTER_RANGE(3'b001), // Filter range
        .FEEDBACK_PATH("SIMPLE"), // Simple feedback path
        .PLLOUT_SELECT("GENOUT"), // Selects the output clock
        .ENABLE_ICEGATE(1'b1)  // Enables the clock gating for power savings
    ) my_pll (
        .REFERENCECLK(SB_CLK_12MHZ), // Connect the board's 12 MHz clock
        .RESETB(SB_RESET_N),      // Connect the reset signal
        .BYPASS(1'b0),            // Bypass the PLL (set to 0 for normal operation)
        .EXTFEEDBACK(),
        .SCLKIN(),
        .LATCHINPUTVALUE(),
        .SDI(),
        .SDO(),
        .SDA(),
        .SCL(),
        .PKGREFCLK(),
        .LOCK(),                  // Output for lock status
        .PLLOUTCORE(),
        .PLLOUTGLOBAL(pll_out)    // Our new, fast clock
    );

    // Use the new, fast clock to drive our pulse generator logic
    assign FAST_CLK_48MHZ = pll_out;

    // You would instantiate your pulse generator module here, connected to FAST_CLK_48MHZ
    // For example:
    // hadamard_pulse_generator my_hadamard_gen (
    //     .clk(FAST_CLK_48MHZ),
    //     .reset_n(SB_RESET_N),
    //     .ad9850_pulse_out(AD9850_PULSE)
    // );
    
    // Assign the pulse output to a physical pin
    // assign AD9850_PULSE = my_hadamard_gen.ad9850_pulse_out;
endmodule
