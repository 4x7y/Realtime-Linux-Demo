## Realtime-Linux Demo

### Environment

* System: Ubuntu 12.04.5 LTS x86
* Kernel: linux-3.12.74
* Realtime-Linux Patch: patch-3.12.74-rt99.patch
* gcc: 4.6.3 (Ubuntu/Linaro 4.6.3-1ubuntu5)

### Setup

``` bash
xzcat patch-3.12.74-rt99.patch.xz > patch-3.12.74-rt99.patch
tar -xvzf linux-3.12.74.tar.gz 
cd linux-3.12.74-rt/
sudo chmod +x rebuild.sh 
sudo ./rebuild.sh 
cd ..
sudo dpkg -i linux-headers-3.12.74-rt99-custom_3.12.74-rt99-custom-7_i386.deb 
sudo dpkg -i linux-image-3.12.74-rt99-custom_3.12.74-rt99-custom-7_i386.deb 
sudo reboot
```

### RT-Test

``` bash
git clone git://git.kernel.org/pub/scm/utils/rt-tests/rt-tests.git
cd rt-tests/
git branch -r
git checkout origin/stable/v1.0
sudo apt-get install libnuma-dev
make all
make HAVE_PARSE_CPUSTRING_ALL=0
sudo make install
make cyclictest

sudo ./cyclictest
```

### View prink Log

``` bash
dmesg | tail
```

### Current Kernel Info

``` bash
uname -a
```

### Build Kenel Module

``` bash
cd rt-modules/
./mmake kmapsnow.c 
```