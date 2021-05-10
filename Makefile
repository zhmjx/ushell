all:
	gcc ushell.c utel.c fork_pty.c -o ushell -static -lutil -lpthread -g
