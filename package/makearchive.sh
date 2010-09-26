version=0.99.71

cd ..
git archive --prefix=heytrack-${version}/ -o package/heytrack-${version}.tar v${version} \
    || return 1
cd package

gzip heytrack-${version}.tar
