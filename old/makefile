BUILD_DIR=$(CURDIR)/build
TEST_DIR=$(CURDIR)/../test
SRC_DIR=$(CURDIR)/../src

CXX=g++
CXX_FLAGS=-std=c++11 -Wall -Werror -g
#LINK_FLAGS=

all: Libxbee Test1

Libxbee: TransmitRequest ReceivePacket SerialXbee
	$(CXX) -shared -o $(BUILD_DIR)/libxbee_plus.so $(BUILD_DIR)/TransmitRequest.o $(BUILD_DIR)/ReceivePacket.o $(BUILD_DIR)/SerialXbee.o -lpthread -lboost_system -lboost_thread

TransmitRequest:
	$(CXX) $(CXX_FLAGS) -fPIC -c $(SRC_DIR)/TransmitRequest.cpp -o $(BUILD_DIR)/TransmitRequest.o 

ReceivePacket:
	$(CXX) $(CXX_FLAGS) -fPIC -c $(SRC_DIR)/ReceivePacket.cpp -o $(BUILD_DIR)/ReceivePacket.o

Utility:
	$(CXX) $(CXX_FLAGS) -fPIC -c $(SRC_DIR)/Utility.cpp -o $(BUILD_DIR)/Utility.o

SerialXbee:
	$(CXX) $(CXX_FLAGS) -fPIC -c $(SRC_DIR)/SerialXbee.cpp -o $(BUILD_DIR)/SerialXbee.o

Test1: 
	$(CXX) $(CXX_FLAGS) $(TEST_DIR)/main.cpp $(BUILD_DIR)/SerialXbee.o -o $(BUILD_DIR)/Test1 -L$(BUILD_DIR) -lboost_system -lpthread -lboost_thread -lxbee_plus
	
.PHONY: clean
	
clean:
	rm $(BUILD_DIR)/*
