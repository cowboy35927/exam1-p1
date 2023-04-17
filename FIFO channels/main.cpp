#include "stim.h"
#define CLOCK_PERIOD 1.0
using namespace std;
int sc_main(int argc, char *argv[]) {
  
  //Create modules and signals
  stim testbench("testbench");
  adder dut("dut", 1);
  sc_clock clk("clk", CLOCK_PERIOD, SC_NS);
  sc_signal<bool> rst("rst");

  //Create FIFO channels
  sc_fifo<sc_ufixed_fast<4,1>> fifo_i_x;
  sc_fifo<sc_ufixed_fast<4,1>> fifo_i_h;
  sc_fifo<sc_ufixed_fast<4,1>> fifo_o_y;

  //Connect FIFO channels with modules
  testbench.i_clk(clk);
  testbench.o_rst(rst);
  dut.i_clk(clk);
  dut.i_rst(rst);
  testbench.o_x(fifo_i_x);
  testbench.o_h(fifo_i_h);
  testbench.i_y(fifo_o_y);
  dut.i_x_port(fifo_i_x);
  dut.i_h_port(fifo_i_h);
  dut.o_y_port(fifo_o_y);

  sc_start();
  return 0;
}
