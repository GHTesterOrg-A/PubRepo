TOP_DIR = ../..
CC = gcc
CFLAGS = -g -fPIC 
LIB = liblzma.a
RM = rm -f
TEST= test
PROG = libnsxz.a
TARGET_INC = $(TOP_DIR)/target/inc
TARGET_LIB = $(TOP_DIR)/target/lib
TARGET_SO = $(TOP_DIR)/ffffftarget/so

OBJS = nsxz.o
TestObj=testxz.o

all: $(PROG) $(TEST)

release: CFLAGS+= -O2
release: all

debug: CFLAGS+= -g 
debug: all 

%.o:%.c
	$(CC) $(CFLAGS) -c $^

$(PROG):$(OBJS)
	#$(CC) -shared -o $(PROG) $(OBJS) $(LIB)
	ar crs $(PROG) $(OBJS)
	cp $(PROG) $(LIB) $(TARGET_LIB)/
	cp nsxz.h $(TARGET_INC)/

$(TEST):$(TestObj)
	$(CC) -o $(TEST) $(TestObj) $(PROG) $(LIB)

clean:
	@$(RM) $(PROG) $(OBJS) $(TEST) $(TestObj)
	@$(RM) $(TARGET_LIB)/$(PROG)
	@$(RM) $(TARGET_LIB)/$(LIB)
	@$(RM) $(TARGET_INC)/nsxz.h
