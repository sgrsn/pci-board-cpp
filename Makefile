CC = gcc
CFLAGS = -Wall -O2
LIB  = -lstdc++ -lm
LIB += -lgpg3300 -ldac

all: libdac.so dac_demo

libdac.so: lib/DAC/DAC.cpp
	gcc -shared -o libdac.so lib/DAC/DAC.cpp -lgpg3300
	sudo cp libdac.so /usr/local/bin/

#export LD_LIBRARY_PATH=/usr/local/bin:$LD_LIBRARY_PATH
        
dac_demo: dac_demo.cpp
	gcc -I./ -L./ dac_demo.cpp -o dac_demo -ldac -lstdc++

clean:
	$(RM) *~ ./*.o ./*.so dac_demo 

