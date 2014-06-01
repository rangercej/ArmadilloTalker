
CC = gcc
LD = gcc -s -lcrypt

CFLAGS = -DDEBUG -g -Wall -O2

PROGRAM = armadillo

HDRS  =	armadillo.h

OBJS = armadillo.o cmd_area.o cmd_mail.o cmd_user.o commands.o debug.o editor.o globals.o login.o main.o map.o network.o signals.o system.o users.o
SRCS = armadillo.c cmd_area.c cmd_mail.c cmd_user.c commands.c debug.c editor.c globals.c login.c main.c map.c network.c signals.c system.c users.c

all:	depend $(PROGRAM)

$(PROGRAM):	$(OBJS)
	$(LD) -o $(PROGRAM) $(OBJS)

depend:
	makedepend -v -- $(CFLAGS) -- $(SRCS)

clean:
	rm -f *.o
	rm -f armadillo

asm:	$(SRCS) $(HDRS)
	$(CC) -S $(SRCS)

# DO NOT DELETE

armadillo.o: /usr/include/stdio.h
# /usr/include/stdio.h includes:
#	features.h
#	stddef.h
#	bits/types.h
#	libio.h
#	bits/stdio_lim.h
armadillo.o: /usr/include/features.h
# /usr/include/features.h includes:
#	sys/cdefs.h
#	gnu/stubs.h
armadillo.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
armadillo.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
armadillo.o: /usr/include/bits/types.h
# /usr/include/bits/types.h includes:
#	features.h
#	stddef.h
armadillo.o: /usr/include/libio.h
# /usr/include/libio.h includes:
#	_G_config.h
#	stdarg.h
armadillo.o: /usr/include/_G_config.h
# /usr/include/_G_config.h includes:
#	bits/types.h
#	stddef.h
#	wchar.h
#	gconv.h
armadillo.o: /usr/include/wchar.h
# /usr/include/wchar.h includes:
#	stddef.h
#	bits/wchar.h
armadillo.o: /usr/include/bits/wchar.h /usr/include/gconv.h
# /usr/include/gconv.h includes:
#	features.h
#	wchar.h
#	stddef.h
armadillo.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
armadillo.o: /usr/include/bits/stdio_lim.h armadillo.h
# armadillo.h includes:
#	stdio.h
#	stdarg.h
#	netinet/in.h
armadillo.o: /usr/include/netinet/in.h
# /usr/include/netinet/in.h includes:
#	features.h
#	stdint.h
#	bits/types.h
#	bits/socket.h
#	bits/in.h
#	endian.h
#	bits/byteswap.h
armadillo.o: /usr/include/stdint.h
# /usr/include/stdint.h includes:
#	features.h
#	stddef.h
#	bits/wchar.h
#	bits/wordsize.h
armadillo.o: /usr/include/bits/wordsize.h /usr/include/bits/socket.h
# /usr/include/bits/socket.h includes:
#	stddef.h
#	limits.h
#	sys/types.h
#	bits/sockaddr.h
#	asm/socket.h
armadillo.o: /usr/include/limits.h
# /usr/include/limits.h includes:
#	features.h
#	limits.h
armadillo.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
armadillo.o: /usr/include/sys/types.h
# /usr/include/sys/types.h includes:
#	features.h
#	bits/types.h
#	time.h
#	stddef.h
armadillo.o: /usr/include/time.h
# /usr/include/time.h includes:
#	bits/types.h
armadillo.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
# /usr/include/asm/socket.h includes:
#	asm/sockios.h
armadillo.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
armadillo.o: /usr/include/endian.h
# /usr/include/endian.h includes:
#	features.h
#	bits/endian.h
armadillo.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
cmd_area.o: /usr/include/stdio.h /usr/include/features.h
cmd_area.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
cmd_area.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
cmd_area.o: /usr/include/bits/types.h /usr/include/libio.h
cmd_area.o: /usr/include/_G_config.h /usr/include/wchar.h
cmd_area.o: /usr/include/bits/wchar.h /usr/include/gconv.h
cmd_area.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
cmd_area.o: /usr/include/bits/stdio_lim.h /usr/include/string.h
# /usr/include/string.h includes:
#	features.h
#	stddef.h
cmd_area.o: armadillo.h /usr/include/netinet/in.h /usr/include/stdint.h
cmd_area.o: /usr/include/bits/wordsize.h /usr/include/bits/socket.h
cmd_area.o: /usr/include/limits.h
cmd_area.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
cmd_area.o: /usr/include/sys/types.h /usr/include/time.h
cmd_area.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
cmd_area.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
cmd_area.o: /usr/include/endian.h /usr/include/bits/endian.h
cmd_area.o: /usr/include/bits/byteswap.h
cmd_mail.o: /usr/include/stdio.h /usr/include/features.h
cmd_mail.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
cmd_mail.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
cmd_mail.o: /usr/include/bits/types.h /usr/include/libio.h
cmd_mail.o: /usr/include/_G_config.h /usr/include/wchar.h
cmd_mail.o: /usr/include/bits/wchar.h /usr/include/gconv.h
cmd_mail.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
cmd_mail.o: /usr/include/bits/stdio_lim.h /usr/include/string.h
cmd_mail.o: /usr/include/time.h /usr/include/unistd.h
# /usr/include/unistd.h includes:
#	features.h
#	bits/posix_opt.h
#	bits/types.h
#	stddef.h
#	bits/confname.h
cmd_mail.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
cmd_mail.o: /usr/include/ctype.h
# /usr/include/ctype.h includes:
#	features.h
#	bits/types.h
#	endian.h
cmd_mail.o: /usr/include/endian.h /usr/include/bits/endian.h
cmd_mail.o: /usr/include/stdlib.h
# /usr/include/stdlib.h includes:
#	features.h
#	stddef.h
cmd_mail.o: armadillo.h /usr/include/netinet/in.h /usr/include/stdint.h
cmd_mail.o: /usr/include/bits/wordsize.h /usr/include/bits/socket.h
cmd_mail.o: /usr/include/limits.h
cmd_mail.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
cmd_mail.o: /usr/include/sys/types.h /usr/include/bits/sockaddr.h
cmd_mail.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
cmd_mail.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
cmd_user.o: /usr/include/stdio.h /usr/include/features.h
cmd_user.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
cmd_user.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
cmd_user.o: /usr/include/bits/types.h /usr/include/libio.h
cmd_user.o: /usr/include/_G_config.h /usr/include/wchar.h
cmd_user.o: /usr/include/bits/wchar.h /usr/include/gconv.h
cmd_user.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
cmd_user.o: /usr/include/bits/stdio_lim.h /usr/include/string.h armadillo.h
cmd_user.o: /usr/include/netinet/in.h /usr/include/stdint.h
cmd_user.o: /usr/include/bits/wordsize.h /usr/include/bits/socket.h
cmd_user.o: /usr/include/limits.h
cmd_user.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
cmd_user.o: /usr/include/sys/types.h /usr/include/time.h
cmd_user.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
cmd_user.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
cmd_user.o: /usr/include/endian.h /usr/include/bits/endian.h
cmd_user.o: /usr/include/bits/byteswap.h
commands.o: /usr/include/stdio.h /usr/include/features.h
commands.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
commands.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
commands.o: /usr/include/bits/types.h /usr/include/libio.h
commands.o: /usr/include/_G_config.h /usr/include/wchar.h
commands.o: /usr/include/bits/wchar.h /usr/include/gconv.h
commands.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
commands.o: /usr/include/bits/stdio_lim.h /usr/include/string.h armadillo.h
commands.o: /usr/include/netinet/in.h /usr/include/stdint.h
commands.o: /usr/include/bits/wordsize.h /usr/include/bits/socket.h
commands.o: /usr/include/limits.h
commands.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
commands.o: /usr/include/sys/types.h /usr/include/time.h
commands.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
commands.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
commands.o: /usr/include/endian.h /usr/include/bits/endian.h
commands.o: /usr/include/bits/byteswap.h
debug.o: /usr/include/stdio.h /usr/include/features.h
debug.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
debug.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
debug.o: /usr/include/bits/types.h /usr/include/libio.h
debug.o: /usr/include/_G_config.h /usr/include/wchar.h
debug.o: /usr/include/bits/wchar.h /usr/include/gconv.h
debug.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
debug.o: /usr/include/bits/stdio_lim.h /usr/include/unistd.h
debug.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
editor.o: /usr/include/stdio.h /usr/include/features.h
editor.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
editor.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
editor.o: /usr/include/bits/types.h /usr/include/libio.h
editor.o: /usr/include/_G_config.h /usr/include/wchar.h
editor.o: /usr/include/bits/wchar.h /usr/include/gconv.h
editor.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
editor.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
editor.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
editor.o: /usr/include/bits/endian.h armadillo.h /usr/include/netinet/in.h
editor.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
editor.o: /usr/include/bits/socket.h /usr/include/limits.h
editor.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
editor.o: /usr/include/sys/types.h /usr/include/time.h
editor.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
editor.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
editor.o: /usr/include/bits/byteswap.h
globals.o: /usr/include/time.h /usr/include/bits/types.h
globals.o: /usr/include/features.h /usr/include/sys/cdefs.h
globals.o: /usr/include/gnu/stubs.h
globals.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
globals.o: armadillo.h /usr/include/stdio.h /usr/include/libio.h
globals.o: /usr/include/_G_config.h /usr/include/wchar.h
globals.o: /usr/include/bits/wchar.h /usr/include/gconv.h
globals.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
globals.o: /usr/include/bits/stdio_lim.h /usr/include/netinet/in.h
globals.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
globals.o: /usr/include/bits/socket.h /usr/include/limits.h
globals.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
globals.o: /usr/include/sys/types.h /usr/include/bits/sockaddr.h
globals.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
globals.o: /usr/include/bits/in.h /usr/include/endian.h
globals.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
login.o: /usr/include/stdio.h /usr/include/features.h
login.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
login.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
login.o: /usr/include/bits/types.h /usr/include/libio.h
login.o: /usr/include/_G_config.h /usr/include/wchar.h
login.o: /usr/include/bits/wchar.h /usr/include/gconv.h
login.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
login.o: /usr/include/bits/stdio_lim.h /usr/include/string.h
login.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
login.o: /usr/include/bits/confname.h /usr/include/ctype.h
login.o: /usr/include/endian.h /usr/include/bits/endian.h
login.o: /usr/include/arpa/telnet.h armadillo.h /usr/include/netinet/in.h
login.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
login.o: /usr/include/bits/socket.h /usr/include/limits.h
login.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
login.o: /usr/include/sys/types.h /usr/include/time.h
login.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
login.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
login.o: /usr/include/bits/byteswap.h
main.o: /usr/include/sys/time.h
# /usr/include/sys/time.h includes:
#	features.h
#	bits/types.h
#	time.h
#	bits/time.h
#	sys/select.h
main.o: /usr/include/features.h /usr/include/sys/cdefs.h
main.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
main.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
main.o: /usr/include/time.h /usr/include/bits/time.h
# /usr/include/bits/time.h includes:
#	bits/types.h
main.o: /usr/include/sys/select.h
# /usr/include/sys/select.h includes:
#	features.h
#	bits/types.h
#	bits/select.h
#	bits/sigset.h
#	time.h
#	bits/time.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/fcntl.h
# /usr/include/fcntl.h includes:
#	features.h
#	bits/fcntl.h
main.o: /usr/include/bits/fcntl.h
# /usr/include/bits/fcntl.h includes:
#	sys/types.h
main.o: /usr/include/sys/types.h /usr/include/netinet/in.h
main.o: /usr/include/stdint.h /usr/include/bits/wchar.h
main.o: /usr/include/bits/wordsize.h /usr/include/bits/socket.h
main.o: /usr/include/limits.h
main.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
main.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
main.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
main.o: /usr/include/endian.h /usr/include/bits/endian.h
main.o: /usr/include/bits/byteswap.h /usr/include/stdio.h
main.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/gconv.h
main.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/sys/stat.h
# /usr/include/sys/stat.h includes:
#	features.h
#	bits/types.h
#	bits/stat.h
main.o: /usr/include/bits/stat.h /usr/include/unistd.h
main.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
main.o: /usr/include/string.h armadillo.h
map.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
map.o: /usr/include/gnu/stubs.h
map.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
map.o: /usr/include/bits/types.h /usr/include/libio.h
map.o: /usr/include/_G_config.h /usr/include/wchar.h
map.o: /usr/include/bits/wchar.h /usr/include/gconv.h
map.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
map.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
map.o: /usr/include/string.h armadillo.h /usr/include/netinet/in.h
map.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
map.o: /usr/include/bits/socket.h /usr/include/limits.h
map.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
map.o: /usr/include/sys/types.h /usr/include/time.h
map.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
map.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
map.o: /usr/include/endian.h /usr/include/bits/endian.h
map.o: /usr/include/bits/byteswap.h
network.o: /usr/include/stdio.h /usr/include/features.h
network.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
network.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
network.o: /usr/include/bits/types.h /usr/include/libio.h
network.o: /usr/include/_G_config.h /usr/include/wchar.h
network.o: /usr/include/bits/wchar.h /usr/include/gconv.h
network.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
network.o: /usr/include/bits/stdio_lim.h /usr/include/sys/socket.h
# /usr/include/sys/socket.h includes:
#	features.h
#	sys/uio.h
#	stddef.h
#	bits/socket.h
network.o: /usr/include/sys/uio.h
# /usr/include/sys/uio.h includes:
#	features.h
#	sys/types.h
#	bits/uio.h
network.o: /usr/include/sys/types.h /usr/include/time.h
network.o: /usr/include/bits/uio.h
# /usr/include/bits/uio.h includes:
#	sys/types.h
network.o: /usr/include/bits/socket.h /usr/include/limits.h
network.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
network.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
network.o: /usr/include/asm/sockios.h /usr/include/sys/time.h
network.o: /usr/include/bits/time.h /usr/include/sys/select.h
network.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
network.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
network.o: /usr/include/netdb.h
# /usr/include/netdb.h includes:
#	features.h
#	netinet/in.h
#	stdint.h
#	bits/netdb.h
network.o: /usr/include/netinet/in.h /usr/include/stdint.h
network.o: /usr/include/bits/wordsize.h /usr/include/bits/in.h
network.o: /usr/include/endian.h /usr/include/bits/endian.h
network.o: /usr/include/bits/byteswap.h /usr/include/bits/netdb.h
network.o: /usr/include/arpa/inet.h
# /usr/include/arpa/inet.h includes:
#	features.h
#	netinet/in.h
network.o: /usr/include/string.h /usr/include/unistd.h
network.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
network.o: armadillo.h
signals.o: /usr/include/stdio.h /usr/include/features.h
signals.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
signals.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
signals.o: /usr/include/bits/types.h /usr/include/libio.h
signals.o: /usr/include/_G_config.h /usr/include/wchar.h
signals.o: /usr/include/bits/wchar.h /usr/include/gconv.h
signals.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
signals.o: /usr/include/bits/stdio_lim.h /usr/include/signal.h
# /usr/include/signal.h includes:
#	features.h
#	bits/sigset.h
#	bits/types.h
#	bits/signum.h
signals.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h armadillo.h
signals.o: /usr/include/netinet/in.h /usr/include/stdint.h
signals.o: /usr/include/bits/wordsize.h /usr/include/bits/socket.h
signals.o: /usr/include/limits.h
signals.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
signals.o: /usr/include/sys/types.h /usr/include/time.h
signals.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
signals.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
signals.o: /usr/include/endian.h /usr/include/bits/endian.h
signals.o: /usr/include/bits/byteswap.h
system.o: /usr/include/stdio.h /usr/include/features.h
system.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
system.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
system.o: /usr/include/bits/types.h /usr/include/libio.h
system.o: /usr/include/_G_config.h /usr/include/wchar.h
system.o: /usr/include/bits/wchar.h /usr/include/gconv.h
system.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
system.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
system.o: /usr/include/string.h /usr/include/time.h /usr/include/syslog.h
# /usr/include/syslog.h includes:
#	sys/syslog.h
system.o: /usr/include/sys/syslog.h
# /usr/include/sys/syslog.h includes:
#	features.h
#	stdarg.h
system.o: /usr/include/ctype.h /usr/include/endian.h
system.o: /usr/include/bits/endian.h armadillo.h /usr/include/netinet/in.h
system.o: /usr/include/stdint.h /usr/include/bits/wordsize.h
system.o: /usr/include/bits/socket.h /usr/include/limits.h
system.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
system.o: /usr/include/sys/types.h /usr/include/bits/sockaddr.h
system.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
system.o: /usr/include/bits/in.h /usr/include/bits/byteswap.h
users.o: /usr/include/string.h /usr/include/features.h
users.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
users.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stddef.h
users.o: /usr/include/netinet/in.h /usr/include/stdint.h
users.o: /usr/include/bits/wchar.h /usr/include/bits/wordsize.h
users.o: /usr/include/bits/types.h /usr/include/bits/socket.h
users.o: /usr/include/limits.h
users.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/limits.h
users.o: /usr/include/sys/types.h /usr/include/time.h
users.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
users.o: /usr/include/asm/sockios.h /usr/include/bits/in.h
users.o: /usr/include/endian.h /usr/include/bits/endian.h
users.o: /usr/include/bits/byteswap.h /usr/include/stdio.h
users.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
users.o: /usr/include/gconv.h
users.o: /usr/lib/gcc-lib/i386-slackware-linux/2.95.3/include/stdarg.h
users.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
users.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
users.o: /usr/include/bits/confname.h armadillo.h
