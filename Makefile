CC = gcc
CFLAGS = -Wall -O2
LIB  = -lstdc++ -lm
LIB += -lgpg3300 -ldac

all: libdac.so libadc.so libqei.so libqei.so libdiff.so libpid.so libplot.so dac_demo adc_demo qei_demo diff_demo speed_control noload_runnnig_torque

libdac.so: lib/DAC/DAC.cpp
	gcc -shared -o libdac.so lib/DAC/DAC.cpp -lgpg3300
	sudo cp libdac.so /usr/local/bin/

libadc.so: lib/ADC/adc.cpp
	gcc -shared -o libadc.so lib/ADC/adc.cpp -lgpg3100
	sudo cp libadc.so /usr/local/bin/

libqei.so: lib/QEI/QEI.cpp
	gcc -shared -o libqei.so lib/QEI/QEI.cpp -lgpg6201
	sudo cp libqei.so /usr/local/bin/

libdiff.so: lib/Diff/differentiator.cpp
	gcc -shared -o libdiff.so lib/Diff/differentiator.cpp -lrt
	sudo cp libdiff.so /usr/local/bin/

libpid.so: lib/control/pid.cpp
	gcc -shared -o libpid.so lib/control/pid.cpp -lrt
	sudo cp libpid.so /usr/local/bin/

libplot.so: lib/eggx_plot/plot.cpp
	egg -shared -o libplot.so lib/eggx_plot/plot.cpp
	sudo cp libplot.so /usr/local/bin/

liblog.so: lib/log/log.cpp
	gcc -shared -o liblog.so lib/log/log.cpp
	sudo cp liblog.so /usr/local/bin/

#export LD_LIBRARY_PATH=/usr/local/bin:$LD_LIBRARY_PATH
        
dac_demo: examples/dac_demo.cpp
	gcc -I./ -L./ examples/dac_demo.cpp -o examples/dac_demo -ldac -lstdc++

adc_demo: examples/adc_demo.cpp
	gcc -I./ -L./ examples/adc_demo.cpp -o examples/adc_demo -ladc -lstdc++

qei_demo: examples/qei_demo.cpp
	gcc -I./ -L./ examples/qei_demo.cpp -o examples/qei_demo -lqei -lstdc++ -lrt

diff_demo: examples/diff_demo.cpp
	gcc -I./ -L./ examples/diff_demo.cpp -o examples/diff_demo -ldiff -lstdc++

plot_demo: examples/plot_demo.cpp
	gcc -std=gnu++0x -I./ -L./ examples/plot_demo.cpp -o examples/plot_demo -lplot -lstdc++

speed_control: examples/speed_control.cpp
	g++ -I./ -L./ examples/speed_control.cpp -o examples/speed_control -ldac -lqei -ldiff -lpid -lplot -lstdc++

log_demo: examples/log_demo.cpp
	g++ -I./ -L./ examples/log_demo.cpp -o examples/log_demo -llog -lstdc++

noload_runnnig_torque: noload_runnnig_torque.cpp
	g++ -I./ -L./ noload_runnnig_torque.cpp -o noload_runnnig_torque -ldac -lqei -ldiff -lpid -lplot -lpthread -lstdc++ /usr/lib/art_syscalls.o

motor_torque: motor_torque.cpp
	g++ -I./ -L./ motor_torque.cpp -o motor_torque -ldac -ladc -lqei -ldiff -lpid -lplot -llog -lpthread -lstdc++ /usr/lib/art_syscalls.o

clean:
	$(RM) *~ ./*.o ./*.so examples/dac_demo examples/adc_demo examples/qei_demo examples/diff_demo examples/speed_control
