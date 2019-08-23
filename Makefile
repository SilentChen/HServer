TARGET = http_server
OBJ_PATH = objs
BIN_PATH = bin

CC = g++
INCLUDES = -I include -I /usr/local/mysql/include
LIBS = -lpthread -lboost_system -lmysqlclient
CFLAGS = -Wall -g -std=c++11 -O3
LINKFLAGS = -L /usr/local/mysql/lib

SRCDIR = src src/mysql src/json

#C_SOURCES = $(wildcard *.c)
C_SRCDIR = $(SRCDIR)
C_SOURCES = $(foreach d, $(C_SRCDIR), $(wildcard $(d)/*.c))
C_OBJS = $(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCES))

#CPP_SOURCES = $(wildcard *.c)
CPP_SRCDIR  = $(SRCDIR)
CPP_SOURCES = $(foreach d, $(CPP_SRCDIR), $(wildcard $(d)/*.cpp))
CPP_OBJS = $(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCES))

default: init compile install

$(C_OBJS):$(OBJ_PATH)/%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(CPP_OBJS):$(OBJ_PATH)/%.o:%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

init:
	$(foreach d, $(SRCDIR), mkdir -p $(OBJ_PATH)/$(d);)

test:
	@echo "C_SOURCES: $(C_SOURCES)"
	@echo "C_OBJS: $(C_OBJS)"
	@echo "CPP_SROUCES: $(CPP_SOURCES)"
	@echo "CPP_OBJS: $(CPP_OBJS)"

compile:$(C_OBJS) $(CPP_OBJS)
	$(CC) $^ -o $(TARGET) $(LINKFLAGS) $(LIBS)

.PHONY : clean
clean:
	rm -rf $(OBJ_PATH)
	rm -f $(BIN_PATH)/$(TARGET)
	rm -f $(TARGET)

install:
	mv $(TARGET) $(BIN_PATH)/$(TARGET)

uninstall:
	rm -f $(BIN_PATH) / $(TARGET)

rebuild: clean compile
