/*
 *  Fix the filesystem detection in the Linux Dropbox client
 *  Copyright (C) 2018  Marco Leogrande
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <linux/magic.h>
#include <dlfcn.h>

int (*orig_statfs)(const char *path, struct statfs64 *buf) = NULL;

int statfs64(const char *path, struct statfs64 *buf) {
  if (orig_statfs == NULL) {
    orig_statfs = dlsym(RTLD_NEXT, "statfs64");
    if (orig_statfs == NULL) {
      fprintf(stderr, "dlsym failed: %s\n", dlerror());
      return -1;
    }
  }

  int retval = orig_statfs(path, buf);
  if (retval == 0) {
    buf->f_type = EXT4_SUPER_MAGIC;
  }
  return retval;
}
