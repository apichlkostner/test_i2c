CXX = arm-linux-gnueabihf-g++
INCLUDES= -I./
CXXFLAGS = -g $(INCLUDES) -std=c++11 -O3 -Wall

OBJ = i2c_com.o
LINKFLAGS=

all: $(OBJ)
	$(CXX) -o i2c_com $(OBJ)

%.o: %.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $<

clean:
	rm *.o i2c_com

upload:
	scp i2c_com raspi:~/usr/