// Mandatory header files
// Vcounter.h for the module counter
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    // i counts the number of clock cycles to simulate.
    // clk is the module clock signal.
    int i;
    int clk;
    int preloaded_val = 0x34;
    

    Verilated::commandArgs(argc, argv);

    // init top verilaog instance
    Vcounter* top = new Vcounter();
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // init Vbuddy
    /*
    Open and initialize Vbuddy connection.
    Port path is in vbuddy.cfg
    */
    if (vbdOpen()!=1)  return(-1);
    vbdHeader("Lab 1: Counter");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->ld = 0;

    // run simulation for many clock cycles 
    for (i = 0; i < 800; i++) {  // this is the for loop where simulations happens

        // dump variables into VCD file and toggle clock
        for (clk = 0; clk < 2; clk++) {
            tfp->dump (2*i+clk);  // unit is in ps!!!
            top->clk = !top->clk;
            top->eval ();
        }

        // ++++  Send count value to Vbuddy
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4) & 0xF);
        vbdHex(1, int(top->count) & 0xF);
        vbdCycle(i+1);
        // ---- end of Vbuddy output section 
        vbdSetMode(1);

        top->rst = (i < 2) | (i == 15);
        top->ld = vbdFlag();

        if (vbdFlag())  top->v = preloaded_val;

        if (Verilated::gotFinish())  exit(0);
    }

    // House keeping
    vbdClose();   // ++++
    tfp->close();
    exit(0);
}
