# Dropbox filesystem fix for Linux [![Build Status](https://travis-ci.org/dark/dropbox-filesystem-fix.svg?branch=master)](https://travis-ci.org/dark/dropbox-filesystem-fix)

Dropbox confirmed recently ([link](https://www.dropboxforum.com/t5/Syncing-and-uploads/Dropbox-client-warns-me-that-it-ll-stop-syncing-in-Nov-why/td-p/290058)) that it will drop support for several Linux filesystems; its client will refuse to sync if an unsupported filesystem is encountered.

**This project fixes the filesystem detection in the Linux client to restore the sync capability.**

The initial version of this repo was based on code posted by one user of the forums ([link](https://www.dropboxforum.com/t5/Syncing-and-uploads/Dropbox-client-warns-me-that-it-ll-stop-syncing-in-Nov-why/m-p/291470/highlight/true#M42807), [link to gist](https://gist.github.com/dimaryaz/275f2dacc7ac50cebd33fc0217b5c249)), but I extended and added extra detection steps since.


## Installation

**This is an experimental fix, not supported by Dropbox. This fix unpredictably changes the behavior of the Dropbox client and might cause irreparable data loss.**

The installation steps are not documented on purpose, given the experimental nature of this code. You assume your own responsibility to configure, build and use this code.

- If you do not feel comfortable with researching and running those steps yourself, or dealing with any problem you might encounter, please do not use this fix. 

- If you are not comfortable with understanding how this code works and is built, this fix is probably too advanced for you. (The author wants to avoid the "implicit warranty" that a copy-and-paste set of instructions would imply.)

## Pages linking here

* For thouse users that accept the risk of data loss, there's [a detailed set of instructions](https://www.linuxuprising.com/2018/11/how-to-use-dropbox-on-non-ext4.html) to install this code on `.deb`-based Linux distributions, that come with no warranties watsoever. Please use on your own risk.
