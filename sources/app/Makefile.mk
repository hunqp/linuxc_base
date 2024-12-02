-include sources/app/interfaces/Makefile.mk
-include sources/app/cloud/Makefile.mk

CXXFLAGS	+= -I./sources/app

VPATH 		+= sources/app

OBJ 		+= $(OBJ_DIR)/app.o
OBJ 		+= $(OBJ_DIR)/app_data.o
OBJ 		+= $(OBJ_DIR)/app_config.o

OBJ 		+= $(OBJ_DIR)/task_list.o
OBJ 		+= $(OBJ_DIR)/task_cloud.o
OBJ 		+= $(OBJ_DIR)/task_system.o
OBJ 		+= $(OBJ_DIR)/task_sensor.o
OBJ 		+= $(OBJ_DIR)/task_device.o


