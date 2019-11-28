CC		:= gcc
TARGET_INIT	:= rclinit.so
TARGET_NODE	:= nodeinit.so
TARGET_PUB	:= publisher.so
TARGET_MSG	:= int16.so
DEST	:= ./lib
ROOTDIR := /opt/ros/dashing
RCLDIR	:= ..

SRCDIR	:= ./nif_src
CFLAGS  := -O3 -fpic -shared
LINK 	:= -lstdc++
INCLUDE := -I /usr/lib/erlang/erts-10.5.2/include -I $(ROOTDIR)/include
LDFLAGS	:= -L $(ROOTDIR)/lib
LIBS_NODE := -lrcl -lrmw -lrcutils -lfastrtps 
LIBS_PUB := $(LIBS_NODE) -lrosidl_generator_c -lrosidl_typesupport_c -lstd_msgs__rosidl_generator_c -lstd_msgs__rosidl_typesupport_c

init:$(TARGET_INIT)
$(TARGET_INIT):
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_INIT) $(SRCDIR)/init_nif.c $(LIBS)

node:$(TARGET_NODE)
$(TARGET_NODE):
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_NODE) $(SRCDIR)/node_nif.c $(LIBS_NODE)

publisher:$(TARGET_PUB)
$(TARGET_PUB):
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_PUB) $(SRCDIR)/publisher_nif.c $(LIBS_PUB)

msg:$(TARGET_MSG)
$(TARGET_MSG):
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $(DEST)/$(TARGET_MSG) $(SRCDIR)/msg_int16_nif.c $(SRCDIR)/msg_type.c $(LIBS_PUB)

clean:
	rm $(DEST)/$(TARGET_INIT) $(DEST)/$(TARGET_NODE)