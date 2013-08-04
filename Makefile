TARGET = all 

FILES = main config lock log string bruco_session bruco_pipe

OBJECTS = $(addsuffix .o,$(FILES))

CFLAGS   = -Wall -g
CPPFLAGS = -Wall -g
INCLUDES = -I/usr/local/include
LIBS     = -L/usr/local/lib -lre2

all: bruco_pipe test_server

bruco_pipe: $(OBJECTS)
	g++ $(LIBS) $^ -o $@

$(OBJECTS): %.o : %.cpp 
	g++ $(CPPFLAGS) $(INCLUDES) -c $< -o $@

test_server: test_server.c
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS)
	rm -rf bruco_pipe test_server
