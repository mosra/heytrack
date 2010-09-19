inkscape --export-dpi=90 --export-area-page --export-png=nsis-header.png \
    nsis-header.svg

convert nsis-header.png -background '#ffffff' -flatten bmp3:nsis-header.bmp
