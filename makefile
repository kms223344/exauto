CC = g++
CFLAGS = --std=c++2a
OBJS = DFA.o AutomataBase.o
TARGET_LIB = libexauto.a
TARGET = test
 
all: $(TARGET_LIB)

$(TARGET_LIB) : $(OJBS)
	ar rc $(TARGET_LIB) $(OBJS)

test1 : $(OBJS) exauto.o
	ar rc libexauto.a $(OBJS)
	$(CC) -o $@ $@.o $(LIBS)
 
clean:
	rm -f *.o
	rm -f $(TARGET) $(TARGET_LIB)
	
test.o: Automata test.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $@

AutomataBase.o: AutomataBase.hpp AutomataBase.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $@

DFA.o: DFA.hpp DFA.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $@
