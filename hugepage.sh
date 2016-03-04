#!/bin/bash

echo "Setup shared memory 33554432Bytes"
sudo echo 33554432 > /proc/sys/kernel/shmmax
echo "Setup number of huge pages supported : 16x2MB : 33554432Bytes (shared memory)"
sudo echo 16 > /proc/sys/vm/nr_hugepages
cat /proc/meminfo | grep Huge
#echo "Create directory /mnt/huge"
#mkdir /mnt/huge
echo "mounting..."
#sudo mount -t hugetlbfs -o uid=0,gid=0,mode=755,pagesize=2k,size=16k,minsize=4k,nr_inodes=0 none /mnt/huge
sudo mount -t hugetlbfs -o uid=0,gid=0,mode=755,pagesize=2m,size=16m,min_size=0m,nr_inodes=0 none /mnt/hgfs
echo "done"

