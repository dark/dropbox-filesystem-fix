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

#include <dlfcn.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/magic.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/vfs.h>

// Necessary on older glibc (?). Borrowed from asm-generic/fcntl.h.
#ifndef O_TMPFILE
#define __O_TMPFILE 020000000
#define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#define O_TMPFILE_MASK (__O_TMPFILE | O_DIRECTORY | O_CREAT)
#endif

#define CANARY_FILE_PREFIX "/tmp"
char *canary_path = NULL;

#define PROC_FILESYSTEMS "/proc/filesystems"


int (*orig_statfs64)(const char *path, struct statfs64 *buf) = NULL;

int statfs64(const char *path, struct statfs64 *buf) {
  if (orig_statfs64 == NULL) {
    orig_statfs64 = dlsym(RTLD_NEXT, "statfs64");
    if (orig_statfs64 == NULL) {
      fprintf(stderr, "dlsym failed: %s\n", dlerror());
      return -1;
    }
  }

  int retval = orig_statfs64(path, buf);
  if (retval == 0) {
    buf->f_type = EXT4_SUPER_MAGIC;

    // Assume this is the Dropbox root, learn the canary path prefix.
    if (canary_path == NULL) {
      char *dup = strdup(path);
      char *dname = dirname(dup);

      canary_path = malloc(strlen(dname) + strlen(CANARY_FILE_PREFIX) + 1);
      if (canary_path) {
        strcpy(canary_path, dname);
        strcat(canary_path, CANARY_FILE_PREFIX);
#ifdef DEBUG
        fprintf(stderr, "  LEARNED canary path: %s\n", canary_path);
#endif
      }

      free(dup);
    }
  }
  return retval;
}

# define OPEN_NEEDS_MODE(flags) \
  (((flags) & O_CREAT) != 0 || ((flags) & O_TMPFILE) == O_TMPFILE)

int (*orig_open64)(const char *pathname, int flags, ...)= NULL;

int open64(const char *pathname, int flags, ...) {
  if (orig_open64 == NULL) {
    orig_open64 = dlsym(RTLD_NEXT, "open64");
    if (orig_open64 == NULL) {
      fprintf(stderr, "dlsym failed: %s\n", dlerror());
      return -1;
    }
  }

  // Reject opens of canary files.
  if (canary_path && strncmp(pathname, canary_path, strlen(canary_path)) == 0) {
#ifdef DEBUG
    fprintf(stderr, "  REJECT canary path: %s\n", pathname);
#endif
    return -1;
  }

  // Reject opens of filesystem pseudofile.
  if (strncmp(pathname, PROC_FILESYSTEMS, strlen(PROC_FILESYSTEMS)) == 0) {
#ifdef DEBUG
    fprintf(stderr, "  REJECT read pseudofile: %s\n", pathname);
#endif
    return -1;
  }

  // Passthru - select target function arity depending on flags. Code
  // borrowed from fcntl.h.
  if (OPEN_NEEDS_MODE(flags)) {
    va_list arg;
    va_start (arg, flags);
    int mode = va_arg (arg, int);
    va_end (arg);

    return orig_open64(pathname, flags, mode);
  }

  return orig_open64(pathname, flags);
}
