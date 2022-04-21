sudo mount /dev/sdc1 /mnt/pico
sudo cp ./build/jtag.uf2 /mnt/pico
sudo sync
sudo umount /mnt/pico
