pkgname=kpie-git
_gitname=kpie
pkgver=
pkgrel=1
pkgdesc='Window rules daemon similar to devilspie with active development'
arch=('x86_64')
url="https://github.com/skx/kpie"
license=('GPL3')
depends=('glib2' 'gtk2' 'lua52' 'libwnck' 'xorgproto' 'libx11')
makedepends=('git')
provides=('kpie')
conflicts=('kpie')
source=('git+${url}.git')
sha256sums=('SKIP')

pkgver() {
  cd "$srcdir/$_gitname"
  git describe --long --tags | sed -E 's/([^-]*-g)/r\1/;s/-/./g'
}
    
prepare() {
  cd "$srcdir/$_gitname"
  # tweak Makefile
  sed -i 's/^LUA_VERSION=5.1/LUA_VERSION=52/' Makefile
  sed -i 's/^LDFLAGS.*dpkg-buildflags.*//' Makefile
}

build() {
  cd "$srcdir/$_gitname"
  make
}
 
package() {
  cd "$srcdir/$_gitname"

  mkdir -p "$pkgdir/usr/bin/"
  mkdir -p "$pkgdir/usr/share/$_gitname/samples"

  install -m755 "$srcdir/$_gitname"/kpie "$pkgdir/usr/bin"
  install -m644 "$srcdir/$_gitname"/{README,FAQ}.md "$pkgdir/usr/share/$_gitname"
  install -m644 "$srcdir/$_gitname/samples"/* "$pkgdir/usr/share/$_gitname/samples"
}