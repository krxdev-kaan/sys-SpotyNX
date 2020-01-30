if [ -f "./spotynx.nsp" ]
then
	sleep 1
else
	exit 10
fi

if [ -f "./4900000000000FEF/exefs.nsp" ]
then
	rm -dfr ./4900000000000FEF/exefs.nsp
    sleep 1
    mv ./spotynx.nsp ./4900000000000FEF/exefs.nsp
else
	mv ./spotynx.nsp ./4900000000000FEF/exefs.nsp
fi