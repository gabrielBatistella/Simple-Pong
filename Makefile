UTIL = src/vector2.cpp src/pong.cpp
MAIN = app/main.cpp
BINARY = bin/pong
ZIP = pong.zip

all:
	@g++ -Wall  -Wno-unused-result -Werror -g $(UTIL) $(MAIN) -o $(BINARY) -lm -pthread

run:
	@./$(BINARY)

valgrind:
	@valgrind -s --tool=memcheck --leak-check=full  --track-origins=yes --show-leak-kinds=all --show-reachable=yes $(BINARY)

clean:
	@rm $(BINARY) $(ZIP)

zip:
	@zip -r $(ZIP) *