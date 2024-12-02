CXXFLAGS += -I./sources/common

VPATH += sources/common

OBJ += $(OBJ_DIR)/jsmn.o
OBJ += $(OBJ_DIR)/utils.o
OBJ += $(OBJ_DIR)/base64.o
OBJ += $(OBJ_DIR)/cmd_line.o
