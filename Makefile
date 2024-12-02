###############################################################################
# AUTHOR: ThanNT
# DATE: 6/1/2017
###############################################################################
NAME_MODULE	= mainProgs
CC			= gcc
CXX			= g++
OBJ_DIR		= build

OPTIMIZE_OPTION	+=	-g -O3
WARNNING_OPTION	+=	-W -Wno-missing-field-initializers

CXXFLAGS += -I/usr/local/include
CXXFLAGS += -I/usr/include

include sources/ak/Makefile.mk
include sources/app/Makefile.mk
include sources/driver/Makefile.mk
include sources/common/Makefile.mk
include sources/sys/Makefile.mk

# CXX compiler option
CXXFLAGS	+=	$(OPTIMIZE_OPTION)	\
				$(WARNNING_OPTION)	\
				-std=c++11			\
				-Wall				\
				-Winline			\
				-Wno-inline			\
				-pipe				\
				-g

# Library paths
LDFLAGS += -L/usr/local/lib
LDFLAGS += -L/usr/include
LDFLAGS += -Wl,-Map=$(OBJ_DIR)/$(NAME_MODULE).map

LDLIBS	+=	-lpthread			\
			-lmosquitto			\
			-lmosquittopp		\
			-lrt				\
			-lm					\
			-lcrypto			\
			-lcurl

all: create $(OBJ_DIR)/$(NAME_MODULE)

create:
	@echo mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@echo CXX $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: %.c
	@echo CXX $<
	@$(CC) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/$(NAME_MODULE): $(OBJ)
	@echo ---------- START LINK PROJECT ----------
	@echo $(CXX) -o $@ $^ $(CXXFLAGS)
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS)

.PHONY: copy
copy: all
	sshpass -p "pi" scp $(OBJ_DIR)/$(NAME_MODULE) pi@192.168.1.6:/home/pi
	@echo COPY $(OBJ_DIR)/$(NAME_MODULE) done.

.PHONY: run
run: all
	@sudo $(OBJ_DIR)/$(NAME_MODULE)
	
.PHONY: debug
debug:
	sudo gdb $(OBJ_DIR)/$(NAME_MODULE)

.PHONY: install
install:
	cp $(OBJ_DIR)/$(NAME_MODULE) /usr/local/bin

.PHONY: clean
clean:
	@echo rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)