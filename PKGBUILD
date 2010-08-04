# Author: mosra <mosra@centrum.cz>
pkgname=heytrack
pkgver=dev
pkgrel=1
pkgdesc="Displays current track and plays internet radio streams"
arch=('i686' 'x86_64')
url="http://github.com/mosra/heytrack.git"
license=('LGPLv3')
groups=()
depends=('qt' 'qjson')
makedepends=('cmake')
optdepends=('kdebase-workspace')

build() {
    mkdir -p "$startdir/build"
    cd "$startdir/build"

    cmake -DCMAKE_INSTALL_PREFIX=/usr ..
    make
}

package() {
    cd "$startdir/build"
    make DESTDIR="$pkgdir/" install
}
