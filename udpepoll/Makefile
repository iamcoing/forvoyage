CC = gcc
CFLAGS = -Wall -g
TARGET = udpser
SRCS = udpEpollSer.c
OBJS = $(SRCS:.c = .o)
DEPS = $(SRCS:.c = .d)

CLFLAGS += -I=libmycrc

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CC)$(CFLAGS)$^ -o $@
%.d:%.c
	@set -e; rm -rf $@
	$(CC) -MM $(CPPFLAGS) $< >@.$$$$;\
	sed 's, \($*\)\.o [ :]*,\1.o:,g' < $@$$$$ >$@;\
	rm -f $@.$$$$
	-include $(DEPS)
