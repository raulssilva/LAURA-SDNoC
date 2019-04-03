#include <systemc.h>
#include <iostream>
#include "constants.h"
#include "mux.h"

using namespace std;

int sc_main(int argc, char* argv[]){
	sc_signal< sc_uint<CHANNEL_WIDITH> > v1;
	sc_signal< sc_uint<CHANNEL_WIDITH> > v2;
	sc_signal< sc_uint<CHANNEL_WIDITH> > v3;
	sc_signal< sc_uint<CHANNEL_WIDITH> > v4;

	sc_signal< sc_uint<2> > s;
	sc_signal<bool> e;
	sc_signal< sc_uint<CHANNEL_WIDITH> > o;

	Mux *mux = new Mux("Mux");
	mux->in_0(v1);
	mux->in_1(v2);
	mux->in_2(v3);
	mux->in_3(v4);
	mux->selector(s);
	mux->enable(e);
	mux->output(o);

	sc_start();

	v1 = 10;
	v2 = 20;
	v3 = 30;
	v4 = 40;

	s = 0;
	e = 1;

	sc_start();

	cout << o.read() << endl;
	// cout << mux.output.read() << endl;

	return 0;
}