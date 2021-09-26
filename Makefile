CFLAGS = -Wall -Wunused -fexceptions -Winvalid-pch -std=c++2a -g -O0 -DDEBUG
TARGET = TargetBinary
LDFLAGS = \
	$(shell pkg-config --libs libpng) \
	$(shell pkg-config --libs zlib) \
	-lgtest -lpthread

include Makefile.inc