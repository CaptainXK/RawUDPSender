.PHONY:clean check_obj_dir check_lib_dir check_build_dir

CC := g++
SRC_FORMAT := cc
OBJ_DIR := obj
LIB_DIR := lib
SRC_DIR := src
BUILD_DIR := build
SRCS := $(wildcard $(SRC_DIR)/*.$(SRC_FORMAT))
OBJS := $(patsubst %.$(SRC_FORMAT), $(OBJ_DIR)/%.o, $(notdir $(SRCS)) )
INC := -I./src
CFLAGS := -g3 -O3 -std=c++11
#LIBS := -lpqos

test.app : check_obj_dir check_build_dir $(OBJS) main.$(SRC_FORMAT)
	$(CC) $(INC) $(OBJS) main.$(SRC_FORMAT) -o $(BUILD_DIR)/$@ $(CFLAGS) $(LIBS)

slib : check_lib_dir check_build_dir $(OBJS)
	ar crv $(BUILD_DIR)/libcat.a $(OBJ_DIR)/*.o

check_obj_dir:
	@if test ! -d $(OBJ_DIR);\
        then\
        mkdir $(OBJ_DIR);\
    fi

check_lib_dir:
	@if test ! -d $(LIB_DIR);\
        then\
        mkdir $(LIB_DIR);\
    fi

check_build_dir:
	@if test ! -d $(BUILD_DIR);\
        then\
        mkdir $(BUILD_DIR);\
    fi

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(SRC_FORMAT)
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)


test : test.app
	$(EXEC) $(BUILD_DIR)/test.app

clean: check_obj_dir check_build_dir check_lib_dir
	@rm -r $(OBJ_DIR) $(BUILD_DIR) $(LIB_DIR)
