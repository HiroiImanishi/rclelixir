CC		:= gcc
TARGET_INIT	:= rclinit.so
TARGET_NODE	:= nodeinit.so
TARGET_PUB	:= publisher.so
TARGET_MSG	:= int16.so
TARGET_ALL  := all.so
DEST	:= ./lib
ROOTDIR := /opt/ros/dashing
RCLDIR	:= ..

SRCDIR	:= ./nif_src
CFLAGS  := -O3 -fpic -shared
LINK 	:= -lstdc++
INCLUDE := -I /usr/lib/erlang/erts-10.5.2/include -I $(ROOTDIR)/include
LDFLAGS	:= -L $(ROOTDIR)/lib
LIBS	:= -lrcl -lrmw -lrcutils -lfastrtps -lrosidl_generator_c -lrosidl_typesupport_c -lstd_msgs__rosidl_generator_c -lstd_msgs__rosidl_typesupport_c

#init_nif.cの内容はnode_nif.cにマージしてる
#all:$(TARGET_ALL)
#$(TARGET_ALL):
#	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_ALL) $(SRCDIR)/all_nif.c $(SRCDIR)/msg_type.c $(LIBS)

all:$(TARGET_ALL)
$(TARGET_ALL):node_nif.o publisher_nif.o msg_int16_nif.o 
	$(CC) $(CFLAGS) -o $(DEST)/$(TARGET_ALL) node_nif.o publisher_nif.o msg_int16_nif.o 

test.o:node_nif.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) node_nif.o -c $(SRCDIR)/total_nif.c  $(LIBS_NODE)
rclinit_nif.o:$(SRCDIR)/init_nif.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c $(SRCDIR)/total_nif.c $(SRCDIR)/init_nif.c  $(LIBS_NODE)

node_nif.o:$(SRCDIR)/node_nif.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c $(SRCDIR)/node_nif.c  $(LIBS_NODE)

publisher_nif.o:$(SRCDIR)/publisher_nif.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c  $(SRCDIR)/publisher_nif.c $(SRCDIR)/total_nif.c $(LIBS_PUB)

msg_int16_nif.o:$(SRCDIR)/msg_int16_nif.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c $(SRCDIR)/total_nif.c $(SRCDIR)/msg_int16_nif.c $(SRCDIR)/msg_type.c $(LIBS_PUB)


#
#init:$(TARGET_INIT)
#$(TARGET_INIT):
#	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_INIT) $(SRCDIR)/init_nif.c $(LIBS)
#
#node:$(TARGET_NODE)
#$(TARGET_NODE):
#	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_NODE) $(SRCDIR)/node_nif.c $(LIBS_NODE)
#
#publisher:$(TARGET_PUB)
#$(TARGET_PUB):
#	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_PUB) $(SRCDIR)/publisher_nif.c $(LIBS_PUB)
#
#msg:$(TARGET_MSG)
#$(TARGET_MSG):
#	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_MSG) $(SRCDIR)/msg_int16_nif.c $(SRCDIR)/msg_type.c $(LIBS_PUB)

clean:
	rm $(DEST)/$(TARGET_ALL) init_nif.o node_nif.o publisher_nif.o msg_int16_nif.o