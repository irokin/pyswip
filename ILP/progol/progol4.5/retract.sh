rm source/*.o source/progol source/qsample
tar cvf progol4_5.tar examples4.2 examples4.4 examples4.5 source man
gzip progol4_5.tar
rm -r examples4.2 examples4.4 examples4.5 source man
