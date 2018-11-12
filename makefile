SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -std=c99 -o $@ $<

.PHONY: clean
clean: 
	rm -f $(PROGS)
