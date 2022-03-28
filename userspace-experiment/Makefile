
TARGET = target
CXXFLAGS = -O2 -Wall -Wextra -Werror -std=c++17
LFLAGS =

OBJECTS = \
	main.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $@ $^ $(CXXFLAGS) $(LFLAGS)

%.o: %.c
	g++ -c -o $@ $^ $(CXXFLAGS)

clean:
	$(RM) $(TARGET)
	$(RM) $(OBJECTS)

