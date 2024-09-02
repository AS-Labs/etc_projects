#!/bin/bash


mkdir -p /mnt/decrypted
mkdir -p /mnt/windows-drive


lsblk |grep bitlocker > /tmp/bitlocker.out

dislocker $partName -u$bitpass -- /mnt/decrypted

mount -o loop /mnt/decrypt/dislocker-file /mnt/windows-mount
# or 
mount -t ntfs-3g -o loop /mnt/decrypt/dislocker-file /mnt/windows-mount



rm -f /mnt/windows-mount/$PATHTOFILE

