BUILD_DIR=$(CURDIR)/build
TEST_DIR=$(CURDIR)/test
LIB_DIR=$(CURDIR)/lib

CXX=g++
CXX_FLAGS=-std=c++11 -Wall -Werror -g
#LINK_FLAGS=

all : Libxbee Test1

Libxbee : TransmitRequest RecievePacket
	$(CXX) -shared -o $(BUILD_DIR)/libxbee.so $(BUILD_DIR)/TransmitRequest.o $(BUILD_DIR)/RecievePacket.o

TransmitRequest:
	$(CXX) $(CXX_FLAGS) -fPIC -c $(LIB_DIR)/TransmitRequest.cpp -o $(BUILD_DIR)/TransmitRequest.o

RecievePacket:
	$(CXX) $(CXX_FLAGS) -fPIC -c $(LIB_DIR)/RecievePacket.cpp -o $(BUILD_DIR)/RecievePacket.o

Test1:
	$(CXX) $(CXX_FLAGS) $(TEST_DIR)/main.cpp -o $(BUILD_DIR)/Test1 -lboost_system -lboost_thread -L$(BUILD_DIR) -lxbee