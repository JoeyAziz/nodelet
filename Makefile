build:
	gcc nodelet_main.c \
	vendor/quickjs/quickjs.c \
	vendor/quickjs/libregexp.c \
	vendor/quickjs/libunicode.c \
	vendor/quickjs/dtoa.c \
	-Ivendor/quickjs \
	-lm \
	-Wall -g \
	-o nodelet 
run:
	./nodelet
