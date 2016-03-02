#!/bin/bash

echo "Setup shared memory 33554432Bytes"
sudo echo 33554432 > /proc/sys/kernel/shmmax
echo "Setup number of huge pages supported : 16x2MB : 33554432Bytes (shared memory)"
sudo echo 16 > /proc/sys/vm/nr_hugepages
