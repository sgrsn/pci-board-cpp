CC = gcc
CFLAGS = -Wall -O2
LIB  = -lstdc++ -lm
LIB += -lgpg3300 -ldac

all: libdac.so libqei.so dac_demo qei_demo

libdac.so: lib/DAC/DAC.cpp
	gcc -shared -o libdac.so lib/DAC/DAC.cpp -lgpg3300
	sudo cp libdac.so /usr/local/bin/

libqei.so: lib/QEI/QEI.cpp
	gcc -shared -o libqei.so lib/QEI/QEI.cpp -lgpg6201
	sudo cp libqei.so /usr/local/bin/

#export LD_LIBRARY_PATH=/usr/local/bin:$LD_LIBRARY_PATH
        
dac_demo: dac_demo.cpp
	gcc -I./ -L./ dac_demo.cpp -o dac_demo -ldac -lstdc++

qei_demo: qei_demo.cpp
	gcc -I./ -L./ qei_demo.cpp -o qei_demo -lqei -lstdc++

clean:
	$(RM) *~ ./*.o ./*.so dac_demo qei_demo
