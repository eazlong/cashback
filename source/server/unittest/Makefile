CXXFLAGS = -g -Wall $(INCLUDE)

INCLUDE = -I $(DRAGONSDK)/include 

LIBS = 	-lpthread -lcppunit

OBJS = unit_test.o process_center_test.o

TARGET = test

$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS) 
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
