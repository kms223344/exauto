CC = g++
CFLAGS = --std=c++2a
OBJS = exauto.o DFA.o AutomataBase.o
TARGET = exauto
 
all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
 
clean:
	rm -f *.o
	rm -f $(TARGET)
	
exauto.o: Automata exauto.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $@

AutomataBase.o: AutomataBase.hpp AutomataBase.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $@

DFA.o: DFA.hpp DFA.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $@
