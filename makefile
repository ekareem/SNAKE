OBJS = snake.cpp c_timer.cpp
all :$(OBJS)
	g++ $(OBJS) -IC:\MinGW\include -LC:\MinGW\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -o snake.exe

run:
	snake.exe