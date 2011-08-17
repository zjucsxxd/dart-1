#!/bin/bash

sudo /sbin/losetup /dev/loop6 floppy.img
sudo mount /dev/loop6 /mnt2
sudo cp src/kernel /mnt2/kernel
sudo cp initrd.img /mnt2/initrd
sudo umount /dev/loop6
sudo /sbin/losetup -d /dev/loop6
