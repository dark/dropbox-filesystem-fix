# Dropbox filesystem fix for Linux

Dropbox confirmed recently ([link](https://www.dropboxforum.com/t5/Syncing-and-uploads/Dropbox-client-warns-me-that-it-ll-stop-syncing-in-Nov-why/td-p/290058)) that it will drop support for several Linux filesystems; its client will refuse to sync if an unsupported filesystem is encountered.

**This project fixes the filesystem detection in the Linux client to restore the sync capability.**

This is based on code posted by one user of the forums ([link](https://www.dropboxforum.com/t5/Syncing-and-uploads/Dropbox-client-warns-me-that-it-ll-stop-syncing-in-Nov-why/m-p/291470/highlight/true#M42807), [link to gist](https://gist.github.com/dimaryaz/275f2dacc7ac50cebd33fc0217b5c249)).

**This is an experimental fix, not supported by Dropbox. It might cause data loss.**
