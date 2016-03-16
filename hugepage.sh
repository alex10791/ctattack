#!/bin/bash


hpcount=16

if [ -n "$1" ] 
then
		hpcount=$1
fi

hpsize=`echo "2*1024*1024" | bc`
shmem=`echo "$hpcount*$hpsize" | bc`
#shmem=33554432
hpsize_M=`echo "$hpsize/1048576" | bc`
shmem_M=`echo "$shmem/1048576" | bc`

echo "Setup shared memory $shmem Bytes"
sudo echo $shmem > /proc/sys/kernel/shmmax
echo "Setup number of huge pages supported : $hpcount x $hpsize_M MB : $shmem Bytes (shared memory)"
sudo echo $hpcount > /proc/sys/vm/nr_hugepages
cat /proc/meminfo | grep Huge
#echo "Create directory /mnt/huge"
#mkdir /mnt/huge
echo "mounting..."
#sudo mount -t hugetlbfs -o uid=0,gid=0,mode=755,pagesize=2k,size=16k,minsize=4k,nr_inodes=0 none /mnt/huge
#sudo mount -t hugetlbfs -o uid=0,gid=0,mode=755,pagesize=2m,size=32m,min_size=0m,nr_inodes=0 none /mnt/hgfs
sudo mount -t hugetlbfs -o uid=0,gid=0,mode=755,pagesize="$hpsize_M"m,size="$shmem_M"m,min_size=0m,nr_inodes=0 none /mnt/hgfs
echo "done"

