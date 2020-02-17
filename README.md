# Dropbox filesystem fix for Linux [![Build Status](https://travis-ci.org/dark/dropbox-filesystem-fix.svg?branch=master)](https://travis-ci.org/dark/dropbox-filesystem-fix)

In 2018, Dropbox dropped support for several Linux filesystems; its client refuses to sync if an unsupported filesystem is encountered: [link](https://www.dropboxforum.com/t5/Syncing-and-uploads/Dropbox-client-warns-me-that-it-ll-stop-syncing-in-Nov-why/td-p/290058).

In July 2019, the decision was partially rolled back, allowing syncing from zfs (on 64-bit systems only), eCryptFS, xfs (on 64-bit systems only), and btrfs filesystems: [link](https://www.dropboxforum.com/t5/Desktop-client-builds/Beta-Build-77-3-127/td-p/354660). Other filesystems dropped by the initial change are, however, **still unsupported**.

**This project fixes the filesystem detection in the Linux client to restore the sync capability for all Linux filesystems.**

The fix has been tested against version `x86_64-89.4.278` of the Dropbox Linux Client.


## History

The initial version of this repo was based on code posted by one user of the forums ([link](https://www.dropboxforum.com/t5/Syncing-and-uploads/Dropbox-client-warns-me-that-it-ll-stop-syncing-in-Nov-why/m-p/291470/highlight/true#M42807), [link to gist](https://gist.github.com/dimaryaz/275f2dacc7ac50cebd33fc0217b5c249)), but I extended and added extra detection steps since.


## Installation

**This is an experimental fix, not supported by Dropbox. It might cause data loss.**

You assume your own responsibility to configure, build and use this code. If you do not feel comfortable with researching and running those steps yourself, or dealing with any problem you might encounter, please do not use this fix.



## Pages linking here

* https://www.linuxuprising.com/2018/11/how-to-use-dropbox-on-non-ext4.html (contains a detailed set of instructions to install this code on `.deb`-based Linux distributions)

* https://www.maketecheasier.com/use-dropbox-non-ext4-linux-filesystem/ (contains a few extra tips to install dependencies in `.rpm`-based Linux distributions)

* https://aur.archlinux.org/packages/dropbox-fix2 (a user-contributed package for Arch Linux in the AUR)
