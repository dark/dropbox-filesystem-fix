#
#  Fix the filesystem detection in the Linux Dropbox client
#  Copyright (C) 2018  Marco Leogrande
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CC = gcc
CFLAGS = -shared -fPIC
LDFLAGS = -ldl

.PHONY: all
all: libdropbox_fs_fix.so

debug: CFLAGS += -DDEBUG -g
debug: all

test: debug detect-ext4.o
	@echo 'Test WITH preload'
	LD_PRELOAD=./libdropbox_fs_fix.so ./detect-ext4.o
	@echo 'Test WITHOUT preload'
	@if ./detect-ext4.o; then echo "  *** Test is unreliable: we're actually on ext4"; fi

%.so: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ $<

clean:
	rm -vf libdropbox_fs_fix.so detect-ext4
