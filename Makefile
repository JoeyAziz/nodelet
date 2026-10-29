build:
	gcc nodelet_main.c \
	nodelet_bindings.c \
	nodelet_eval.c \
	nodelet_file.c \
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
