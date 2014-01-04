export TARGET=i586-elf
export CC=/usr/gcc-4.7.0/bin/gcc-4.7.0
export CXX=/usr/gcc-4.7.0/bin/g++-4.7.0
export CPP=/usr/gcc-4.7.0/bin/cpp-4.7.0
export LD=/usr/gcc-4.7.0/bin/gcc-4.7.0
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export PREFIX="$DIR/binutils"
function grab () {
    echo "Pulling $1..."
    if [ ! -f "$3" ]; then
        wget "$2/$3"
    else
        echo "Already have $1"
    fi
} #Credit Kevin Lange (klange) for grab function
echo "Building binutils"
grab "binutils" "http://ftp.gnu.org/gnu/binutils" "binutils-2.22.tar.gz"
grab "libiconv" "ftp://ftp.gnu.org/gnu/libiconv/" "libiconv-1.14.tar.gz"
echo "Extracting..."
tar -xf binutils-2.22.tar.gz
tar -xf libiconv-1.14.tar.gz
mkdir binutils
mkdir build-binutils
cd build-binutils
echo "Installing to $PREFIX"
../binutils-2.22/configure --target=$TARGET --prefix=$PREFIX --disable-nls
make -j 4 all
make -j 4 install
