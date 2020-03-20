(stolen from https://github.com/rose-compiler/rose/wiki/Installing-the-compiler-and-boost-from-source)


# Install Boost

Once you have selected the boost version (we will be using 1_67_0 for this guide) you need to download and extract the source.
```
cd $PREFIX
wget -nv https://sourceforge.net/projects/boost/files/boost/1.67.0/boost_1_67_0.tar.bz2/download -O boost_1_67_0.tar.bz2
tar jxf boost_1_67_0.tar.bz2
rm boost_1_67_0.tar.bz2
cd boost_1_67_0
```
Next you need to run bootstrap to set up the boost installation. The libraries listed for the `--with-libraries` option are those necessary or ROSE. Once bootstrap is done bun b2 to begin boost installation.
```
./bootstrap.sh --prefix="${PREFIX}/boost_1_67_0/install" --with-libraries=chrono,date_time,filesystem,iostreams,program_options,random,regex,serialization,signals,system,thread,wave
./b2 --prefix="${PREFIX}/boost_1_67_0/install" -std=c++11 install
```
Finally update the environment variable to find boost.
```
export LD_LIBRARY_PATH="${PREFIX}/boost_1_67_0/install/lib:${LD_LIBRARY_PATH}"
export BOOST_ROOT="${PREFIX}/boost_1_67_0/install"
