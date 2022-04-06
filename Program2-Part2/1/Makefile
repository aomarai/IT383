all: serial_mmmavg mt_mmmavg

serial_mmmavg: serial_mmmavg.cpp
	g++ -o serial_mmmavg serial_mmmavg.cpp

mt_mmmavg: mt_mmmavg.cpp
	g++  -lrt -pthread -o mt_mmmavg mt_mmmavg.cpp