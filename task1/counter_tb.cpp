// Mandatory header files
// Vcounter.h for the module counter

#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    // i counts the number of clock cycles to simulate.
    // clk is the module clock signal.
    int i;
    int clk;
    int stop_cycles = 0;

    Verilated::commandArgs(argc, argv);

    // init top verilaog instance

    /*
    instantiate the counter module as Vcounter, 
    which  is the name of all generated fiels.
    This is the DUT! 
    */
    Vcounter*top = new Vcounter;

    // init trace dump

    /*
    Turn on signal tracing,
    and tell Verilator to dump the waveform data to counter.vcd
    */
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // initialize simulation inputs
    
    /*
    set initial signal levels
    top is the name of the top-level entity
    only the top-level signals are visible
    */
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles 
    for (i = 0; i < 300; i++) {  // this is the for loop where simulations happens

        // dump variables into VCD file and toggle clock

        /*
        This is the loop that toggles the clock.
        It also output the trace for each half of the clock cycle, 
        and force the model to evaluate on both edges of the clock
        */
        for (clk = 0; clk < 2; clk++) {
            tfp->dump (2*i+clk);  // unit is in ps!!!
            top->clk = !top->clk;
            top->eval ();
        }

        top->rst = (i % 3 == 0);
        top->en = (i>4);
        // if (stop_cycles > 0) {
        //     top->en = 0;
        //     stop_cycles--;
        // } else {
        //     top->en = 1;
        //     if (top->count == 0x9) {
        //         stop_cycles = 3;
        //     }
        // }

        if (Verilated::gotFinish())  exit(0);
    }
    tfp->close();
    exit(0);
}
