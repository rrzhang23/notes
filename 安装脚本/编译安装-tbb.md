
```
mkdir ~/.local/tbb/
mkdir ~/.local/tbb/lib
ln -s ~/.local/tbb/lib/ ~/.local/tbb/lib64

cd ~/.local/build/
mkdir build-tbb
cd build-tbb
wget https://github.com/oneapi-src/oneTBB/archive/v2020.3.zip
unzip v2020.3.zip > /dev/null
cd oneTBB-2020.3
make -j32
cd build
chmod +x *.sh
sh generate_tbbvars.sh
sh tbbvars.sh
cd linux_intel64_gcc_cc7.4.0_libc2.17_kernel5.0.5_release

cd linux_intel64_gcc_cc7.4.0_libc2.17_kernel5.0.5_release

cp *.so ~/.local/tbb/lib/
cp *.so.2 ~/.local/tbb/lib/
cp -r ~/.local/build/build-tbb/oneTBB-2020.3/include/ ~/.local/tbb/

```

```
# .bashrc
# tbb
export           TBB_ROOT=$HOME/.local/tbb
export    LD_LIBRARY_PATH=$TBB_ROOT/lib:$LD_LIBRARY_PATH
export       LIBRARY_PATH=$LIBRARY_PATH:$LD_LIBRARY_PATH
export        LD_RUN_PATH=$LD_RUN_PATH:$LD_LIBRARY_PATH
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TBB_ROOT/include
export     C_INCLUDE_PATH=$C_INCLUDE_PATH:$CPLUS_INCLUDE_PATH
```