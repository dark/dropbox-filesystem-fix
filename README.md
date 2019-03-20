# Dropbox filesystem fix for Linux [![Build Status](https://travis-ci.org/dark/dropbox-filesystem-fix.svg?branch=master)](https://travis-ci.org/dark/dropbox-filesystem-fix)

Dropbox confirmed recently ([link](https://www.dropboxforum.com/t5/Syncing-and-uploads/Dropbox-client-warns-me-that-it-ll-stop-syncing-in-Nov-why/td-p/290058)) that it will drop support for several Linux filesystems; its client will refuse to sync if an unsupported filesystem is encountered.

**This project fixes the filesystem detection in the Linux client to restore the sync capability.**

This is based on code posted by one user of the forums ([link](https://www.dropboxforum.com/t5/Syncing-and-uploads/Dropbox-client-warns-me-that-it-ll-stop-syncing-in-Nov-why/m-p/291470/highlight/true#M42807), [link to gist](https://gist.github.com/dimaryaz/275f2dacc7ac50cebd33fc0217b5c249)).

**This is an experimental fix, not supported by Dropbox. It might cause data loss.**

## Installation

### 1. Make sure your system is up to date:
```
sudo apt update; sudo apt upgrade -y
```

### 2. Install Git (to grab the latest dropbox-filesystem-fix code) and build-essential (to compile it):
```
sudo apt install git build-essential
```

### 3. Get the latest dropbox-filesystem-fix source code via Git and compile it:
```
git clone https://github.com/dark/dropbox-filesystem-fix.git
cd dropbox-filesystem-fix
make
```
A file called *libdropbox_fs_fix.so* should be created in the *dropbox-filesystem-fix* folder after running *make*.

You may also download the code by using this [ZIP download](https://github.com/dark/dropbox-filesystem-fix/archive/master.zip).

### 4. Move the *dropbox-filesystem-fix* folder to */opt* and mark *dropbox_start.py* as executable:
```
sudo mv dropbox-filesystem-fix /opt/
sudo chmod +x /opt/dropbox-filesystem-fix/dropbox_start.py
```

You can now try it. This is not how you'll use dropbox-filesystem-fix, but only a one time try to see it actually fixes the Dropbox non-Ext4 filesystem sync issue on your system. To give it a try, make sure Dropbox isn't running - you can stop it using this command:
`dropbox stop`

And now start Dropbox by using this command instead of running it directly:
`/opt/dropbox-filesystem-fix/dropbox_start.py`

If you don't receive any filesystem-related notifications from Dropbox, it means dropbox-filesystem-fix works, and you can proceed to replace the original Dropbox startup entry with the dropbox-filesystem-fix *dropbox_start.py* script.

### 5. Stop Dropbox from autostarting:
The original Dropbox autostart entry needs to be removed / disabled, so we can use the *dropbox_start.py* script instead.
Stop Dropbox from running on startup by unchecking the *Start Dropbox on system startup* checkbox from the *Dropbox Preferences*, on the *General* tab.

### 6. Add a custom Dropbox startup entry:
Now you'll need to open *Startup Applications* or equivalent from your applications menu, and add a new entry called *Dropbox fix* with the following in the command field: */opt/dropbox-filesystem-fix/dropbox_start.py*

### Enjoy
Now logout / login and Dropbox, which starts automatically on login just like before, should be working with your Btrfs, Ext3, ecryptfs on top of Ext4 and so on.

In case you close Dropbox, don't run it from the menu as that entry doesn't make use of dropbox-filesystem-fix. Run it using `/opt/dropbox-filesystem-fix/dropbox_start.py` or make a new desktop file for Dropbox with */opt/dropbox-filesystem-fix/dropbox_start.py* as the command, and place it in *~/.local/share/applications/*. But in most cases, Dropbox is not launched from the application menu, so the autostart changes made above should be enough for most users.

*How-to from [Linux Uprising](https://www.linuxuprising.com/2018/11/how-to-use-dropbox-on-non-ext4.html)*
