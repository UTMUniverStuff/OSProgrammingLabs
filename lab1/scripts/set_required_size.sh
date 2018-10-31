#!/bin/bash

# This script adds the required bytes for an .img file.
# Parameters:
# 	$1 - target file

target_file=$1
file_size=$(wc -c $target_file | awk '{print $1}')
required_floppy_img_size=1474560
let bytes_to_add=$required_floppy_img_size-$file_size

echo "File size is $file_size"

echo "Writing $bytes_to_add bytes to $target_file"
perl -E "print ' ' x $bytes_to_add" >> $target_file

file_size=$(wc -c $target_file | awk '{print $1}')

echo "File size is now $file_size"