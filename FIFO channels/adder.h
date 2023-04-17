#ifndef _ADDER_H_
#define _ADDER_H_

#include <iostream>
using namespace std;
#include <sysc/datatypes/fx/sc_ufixed.h>
#include <systemc>
using namespace sc_dt;
using namespace sc_core;

const int WIDTH = 4;
const int int_WIDTH = 1;

class adder : public sc_module {
public:
  sc_in_clk i_clk;
  sc_in<bool> i_rst;
  sc_fifo_in< sc_dt::sc_ufixed_fast<4,1>> i_x_port;
  sc_fifo_in< sc_dt::sc_ufixed_fast<4,1>> i_h_port;

  sc_fifo_out< sc_dt::sc_ufixed_fast<4,1>>  o_y_port;

  void do_add() {
    while (true) {      
       
        _i_x = i_x_port.read();
        _i_h = i_h_port.read();
        wait();
        _o_y =_o_y + _i_x * _i_h;
        if(count == 3){
          count=0;
          o_y_port.write(_o_y);
        }

        wait();
         count++;
    }
  }

  SC_HAS_PROCESS(adder);
  adder(sc_module_name name, int i) : id(i) { 
    SC_THREAD(do_add);  
    sensitive << i_clk.pos();
    dont_initialize();
  }

  sc_ufixed_fast<4,1> i_x() { return _i_x; }
  sc_ufixed_fast<4,1> i_h() { return _i_h; }
  sc_ufixed_fast<4,1> o_y() { return _o_y; }

private:
  const int id;
  int count=0;
  sc_ufixed_fast<4,1> _i_x;
  sc_ufixed_fast<4,1> _i_h;
  sc_ufixed_fast<4,1> _o_y;

 
};

#endif
