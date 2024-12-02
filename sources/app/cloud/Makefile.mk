CXXFLAGS	+= -I./sources/app/cloud

VPATH 		+= sources/app/cloud

OBJ 		+= $(OBJ_DIR)/cloud_api.o
OBJ 		+= $(OBJ_DIR)/BaseMQTT.o
