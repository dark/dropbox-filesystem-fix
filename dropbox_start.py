#!/usr/bin/python
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

# This code is loosely based on:
# dropbox
# Dropbox frontend script
# This file is part of nautilus-dropbox 2.10.0

import os
import subprocess
import sys
import time

# Python 3 compatibility on xrange
# Ref: https://stackoverflow.com/a/31136897/1259696
try:
    xrange
except NameError:
    xrange = range


def is_dropbox_running():
    pidfile = os.path.expanduser("~/.dropbox/dropbox.pid")

    try:
        with open(pidfile, "r") as f:
            pid = int(f.read())
        with open("/proc/%d/cmdline" % pid, "r") as f:
            cmdline = f.read().lower()
    except:
        cmdline = ""

    return "dropbox" in cmdline


def start_dropbox():
    lib_dir = os.path.dirname(os.path.realpath(__file__))
    lib_path = os.path.join(lib_dir, "libdropbox_fs_fix.so")
    if not os.access(lib_path, os.X_OK):
        print(">>> Library '%s' is not available!" % lib_path)
        return False
    os.environ["LD_PRELOAD"] = lib_path

    db_path = os.path.expanduser(u"~/.dropbox-dist/dropboxd").encode(
        sys.getfilesystemencoding()
    )
    if os.access(db_path, os.X_OK):
        f = open("/dev/null", "w")
        # we don't reap the child because we're gonna die anyway, let init do it
        a = subprocess.Popen(
            [db_path],
            preexec_fn=os.setsid,
            cwd=os.path.expanduser("~"),
            stderr=sys.stderr,
            stdout=f,
            close_fds=True,
        )

        # in seconds
        interval = 0.5
        wait_for = 60
        for i in xrange(int(wait_for / interval)):
            if is_dropbox_running():
                return True
            # back off from connect for a while
            time.sleep(interval)

        return False
    else:
        return False


def main():
    if is_dropbox_running():
        print(">>> Dropbox is already running")
        return 0

    if start_dropbox():
        print(">>> Dropbox started successfully")
        return 0

    print(">>> Dropbox failed to start!")
    return 1


if __name__ == "__main__":
    sys.exit(main())
