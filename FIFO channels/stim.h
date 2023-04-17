#ifndef _STIM_H_
#define _STIM_H_
using namespace std;
#include <iomanip>
using std::setw;
#include "adder.h"

// WIDTH is defined in adder.h
const int DEPTH = (1 << WIDTH);
const float x_input_signal[8*16]={0.500,   0.525,   0.549,   0.574,   0.598,   0.622,   0.646,   0.670,
                            0.693,   0.715,   0.737,   0.759,   0.780,   0.800,   0.819,   0.838,
                            0.856,   0.873,   0.889,   0.904,   0.918,   0.931,   0.943,   0.954,
                            0.964,   0.972,   0.980,   0.986,   0.991,   0.995,   0.998,   1.000,
                            1.000,   0.999,   0.997,   0.994,   0.989,   0.983,   0.976,   0.968,
                            0.959,   0.949,   0.937,   0.925,   0.911,   0.896,   0.881,   0.864,
                            0.847,   0.829,   0.810,   0.790,   0.769,   0.748,   0.726,   0.704,
                            0.681,   0.658,   0.634,   0.610,   0.586,   0.562,   0.537,   0.512,
                            0.488,   0.463,   0.438,   0.414,   0.390,   0.366,   0.342,   0.319,
                            0.296,   0.274,   0.252,   0.231,   0.210,   0.190,   0.171,   0.153,
                            0.136,   0.119,   0.104,   0.089,   0.075,   0.063,   0.051,   0.041,
                            0.032,   0.024,   0.017,   0.011,   0.006,   0.003,   0.001,   0.000,
                            0.000,   0.002,   0.005,   0.009,   0.014,   0.020,   0.028,   0.036,
                            0.046,   0.057,   0.069,   0.082,   0.096,   0.111,   0.127,   0.144,
                            0.162,   0.181,   0.200,   0.220,   0.241,   0.263,   0.285,   0.307,
                            0.330,   0.354,   0.378,   0.402,   0.426,   0.451,   0.475,   0.500};
float y_downsample_by2[64]={0.429,   0.558,   0.606,   0.654,   0.700,   0.744,   0.786,   0.825,
                               0.861,   0.894,   0.922,   0.946,   0.966,   0.982,   0.993,   0.998,
                               0.999,   0.996,   0.987,   0.973,   0.955,   0.933,   0.906,   0.875,
                               0.841,   0.803,   0.762,   0.719,   0.673,   0.626,   0.578,   0.529,
                               0.479,   0.430,   0.382,   0.334,   0.289,   0.245,   0.204,   0.165,
                               0.130,   0.099,   0.071,   0.048,   0.029,   0.015,   0.006,   0.001,
                               0.001,   0.006,   0.016,   0.031,   0.050,   0.074,   0.101,   0.133,
                               0.168,   0.207,   0.248,   0.292,   0.338,   0.386,   0.434,   0.484};
float h_k[3]={0.5,1/3,1/6};

float M=2;

SC_MODULE(stim) {

  sc_in_clk i_clk;
  sc_out<bool> o_rst;
  sc_fifo_out<sc_ufixed_fast<4,1>> o_x;
  sc_fifo_out<sc_ufixed_fast<4,1>> o_h;
  sc_fifo_in<sc_ufixed_fast<4,1>> i_y;
  //Store the previous inputs to FIFOs
  sc_ufixed_fast<4,1> t_x;
  sc_ufixed_fast<4,1> t_h;
  sc_ufixed_fast<4,1> t_y;

  void stim_gen() {
    cout << setw(12) << "time" << setw(12) << "a" << setw(12) << "b" << endl;
    for (int a = 0; a < 8*16; a++) {
      for (int b = 0; b < 3; b++) {
        o_x.write(x_input_signal[a*2-b+1]);
        o_h.write(h_k[b]);
        t_x=x_input_signal[a*2-b+1];
        t_h=h_k[b];
        //cout << setw(12) << sc_time_stamp();
        //cout << setw(12) << t_x.to_string(SC_BIN);
        //cout << setw(12) << t_h.to_string(SC_BIN) << endl;
        wait();
      }
    }
  }

  void sink() {
    cout << setw(12) << "time" << setw(12) << "sum" << endl;
    int count=0;
    while (true) {
      t_y=i_y.read();
      count++;
      cout <<setw(12) << sc_time_stamp();
      cout << setw(12) << t_y.to_string(SC_BIN) << endl;
      wait();
    }
  }

  void monitor() {
    cout << setw(12) << "time" << setw(12) << "a" << setw(12) << "b" << setw(12)
         << "sum" << endl;
    
    while (true) {
      wait();
     // cout << setw(12) << sc_time_stamp();
     // cout << setw(12) << t_x.to_string(SC_BIN);
     // cout << setw(12) << t_h.to_string(SC_BIN);
     // cout << setw(12) << t_y.to_string(SC_BIN) << endl;
      cout << "======================================================" << endl;
    }
  }

  SC_CTOR(stim) {
    SC_THREAD(stim_gen);
    sensitive << i_clk.pos();
    dont_initialize();

    SC_THREAD(sink);
    sensitive << i_clk.pos();
    
    //SC_THREAD(monitor);
    // sensitive << i_clk.pos();
 
  }
};

#endif
