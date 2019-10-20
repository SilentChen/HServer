## Base setting
CC					=	g++
OBJ_PATH			=	build
BIN_PATH			=	bin

## Target's common setting
FLAGS_COMMON		=	-Wall -g -std=c++11 -O3
INCLUDE_COMMON		=	-I include -I src/lib/public -I src/flutter/public -I/usr/local/mysql/include -I/usr/include/boost
LIBNAME_COMMON		=	-lpthread -lboost_system -lmysqlclient
LIBPATH_COMMON		=  -L /usr/local/mysql/lib -L /usr/local/boost/stage/lib -L /usr/lib
SRCDIR_COMMON		=	src/flutter src/json src/lib  src/mysql
C_SOURCE_COMMON		=	$(foreach d, $(SRCDIR_COMMON), $(wildcard $(d)/*.c))
C_OBJECT_COMMON		=	$(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCE_COMMON))
CPP_SOURCE_COMMON	=	$(foreach d, $(SRCDIR_COMMON), $(wildcard $(d)/*.cpp))
CPP_OBJECT_COMMON	=	$(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCE_COMMON))

## Target HTTP setting
TARGET_HTTP			=	server_http
FLAGS_HTTP			=	
INCLUDE_HTTP		=
LIBNAME_HTTP		=
LIBPATH_HTTP		=
SRCDIR_HTTP			=	src/http
C_SOURCE_HTTP		=	$(foreach d, $(SRCDIR_HTTP), $(wildcard $(d)/*.c))
C_OBJECT_HTTP		=	$(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCE_HTTP))
CPP_SOURCE_HTTP		=	$(foreach d, $(SRCDIR_HTTP), $(wildcard $(d)/*.cpp))
CPP_OBJECT_HTTP		=	$(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCE_HTTP))

## Target HTTPS setting
TARGET_HTTPS		=	server_https
FLAGS_HTTPS			=	
INCLUDE_HTTPS		=	-I/usr/include/openssl
LIBNAME_HTTPS		=	-lssl -lcrypto
LIBPATH_HTTPS		=
SRCDIR_HTTPS		=	src/https
C_SOURCE_HTTPS		=	$(foreach d, $(SRCDIR_HTTPS), $(wildcard $(d)/*.c))
C_OBJECT_HTTPS		=	$(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCE_HTTPS))
CPP_SOURCE_HTTPS	=	$(foreach d, $(SRCDIR_HTTPS), $(wildcard $(d)/*.cpp))
CPP_OBJECT_HTTPS	=	$(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCE_HTTPS))

## Final Settint Summary
FLAG				=	$(FLAGS_COMMON) $(FLAGS_HTTP) $(FLAGS_HTTPS)
INCLUDE				=	$(INCLUDE_COMMON) $(INCLUDE_HTTP) $(INCLUDE_HTTPS)
TARGET				=	$(TARGET_HTTP) $(TARGET_HTTPS)
SRCDIR				=	$(SRCDIR_COMMON) $(SRCDIR_HTTP) $(SRCDIR_HTTPS)

all: init $(TARGET)
$(TARGET_HTTP) : $(C_OBJECT_COMMON) $(C_OBJECT_HTTP) $(CPP_OBJECT_COMMON) $(CPP_OBJECT_HTTP)
	$(CC) $^ -o $(BIN_PATH)/$(TARGET_HTTP) $(LIBNAME_COMMON) $(LIBNAME_HTTP) $(LIBPATH_COMMON) $(LIBPATH_HTTP)

$(TARGET_HTTPS) : $(C_OBJECT_COMMON) $(C_OBJECT_HTTPS) $(CPP_OBJECT_COMMON) $(CPP_OBJECT_HTTPS)
	$(CC) $^ -o $(BIN_PATH)/$(TARGET_HTTPS) $(LIBNAME_COMMON) $(LIBNAME_HTTPS) $(LIBPATH_COMMON) $(LIBPATH_HTTPS)

$(C_OBJECT_COMMON) : $(OBJ_PATH)/%.o : %.c
	$(CC) -c $(FLAG) $(INCLUDE) $< -o $@
$(C_OBJECT_HTTP) : $(OBJ_PATH)/%.o : %.c
	$(CC) -c $(FLAG) $(INCLUDE) $< -o $@
$(C_OBJECT_HTTPS) : $(OBJ_PATH)/%.o : %.c
	$(CC) -c $(FLAG) $(INCLUDE) $< -o $@
$(CPP_OBJECT_COMMON) : $(OBJ_PATH)/%.o : %.cpp
	$(CC) -c $(FLAG) $(INCLUDE) $< -o $@
$(CPP_OBJECT_HTTP) : $(OBJ_PATH)/%.o : %.cpp
	$(CC) -c $(FLAG) $(INCLUDE) $< -o $@
$(CPP_OBJECT_HTTPS) : $(OBJ_PATH)/%.o : %.cpp
	$(CC) -c $(FLAG) $(INCLUDE) $< -o $@

init:
	$(foreach d, $(SRCDIR), mkdir -p $(OBJ_PATH)/$(d);)

info:
	@echo "CC: $(CC)"
	@echo "OBJECT PATH: $(OBJ_PATH)"
	@echo "BIN PATH: $(BIN_PATH)"
	@echo "FLAG: $(FLAG)"
	@echo "INCLUDE: $(INCLUDE)"
	@echo "C_SRCDIR_COMMON: $(C_SRCDIR_COMMON)"
	@echo "C_SRCDIR_HTTP: $(C_SRCDIR_HTTP)"
	@echo "C_SRCDIR_HTTPS: $(C_SRCDIR_HTTPS)"
	@echo "C_SOURCE_COMMON: $(C_SOURCE_COMMON)"
	@echo "C_SOURCE_HTTP: $(C_SOURCE_HTTP)"
	@echo "C_SOURCE_HTTPS: $(C_SOURCE_HTTPS)"
	@echo "C_OBJECT_COMMON: $(C_OBJECT_COMMON)"
	@echo "C_OBJECT_HTTP: $(C_OBJECT_HTTP)"
	@echo "C_OBJECT_HTTPS: $(C_OBJECT_HTTPS)"
	@echo "CPP_SRCDIR_COMMON: $(CPP_SRCDIR_COMMON)"
	@echo "CPP_SRCDIR_HTTP: $(CPP_SRCDIR_HTTP)"
	@echo "CPP_SRCDIR_HTTPS: $(CPP_SRCDIR_HTTPS)"
	@echo "CPP_SOURCE_COMMON: $(CPP_SOURCE_COMMON)"
	@echo "CPP_SOURCE_HTTP: $(CPP_SOURCE_HTTP)"
	@echo "CPP_SOURCE_HTTPS: $(CPP_SOURCE_HTTPS)"
	@echo "CPP_OBJECT_COMMON: $(CPP_OBJECT_COMMON)"
	@echo "CPP_OBJECT_HTTP: $(CPP_OBJECT_HTTP)"
	@echo "CPP_OBJECT_HTTPS: $(CPP_OBJECT_HTTPS)"
	@echo "INCLUDE: $(INCLUDE)"

help:
	@echo "useage: "
	@echo " |- init: make build object dir"
	@echo " |- info: show some make setting detail"
	@echo " |- http: make http target"
	@echo " |- https:make https target"
	@echo " |- all:  make all target [defaule]"

http: init server_http

https: init server_https

.PHONE: clean

clean:
	rm -rf $(OBJ_PATH)
	rm -rf $(BIN_PATH)/*