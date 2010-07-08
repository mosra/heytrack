# Author: mosra <mosra@centrum.cz>
pkgname=heytrack
pkgver=dev
pkgrel=1
pkgdesc="Displays current track in internet radio stream"
arch=('i686' 'x86_64')
url="http://archerfx.cz/blog/"
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
