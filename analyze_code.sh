#!/bin/bash

SUBDIR=""
TOP_DIR=`pwd`
SOURCES=""

for d in `find . -maxdepth 1 -mindepth 1 -type d -not -name ".*" -and -not -name "bw-output" -and -not -name "SpectrometerSrv"`; do
	if [ -f "$d/Makefile" ]; then
		if [ "x$SOURCES" = "x" ]; then
			SOURCES="$d"
		else
			SOURCES="$SOURCES,$d"
		fi
		d=$(sed 's/\.\///' <<< $d)
		SUBDIR="$SUBDIR $d"
		sed -i 's/^#RELEASE_TYPE = DEBUG/RELEASE_TYPE = DEBUG/' $d/Makefile
		sed -i 's/^RELEASE_TYPE = OPTIMIZED/#RELEASE_TYPE = OPTIMIZED/' $d/Makefile
	fi
done

echo "" > Makefile
echo "SUBDIRS :=$SUBDIR" >> Makefile
echo '.PHONY : all $(SUBDIRS)' >> Makefile
echo 'all : $(SUBDIRS)' >> Makefile
echo '$(SUBDIRS) :' >> Makefile
echo '	$(MAKE) -C $@ clean all' >> Makefile

build-wrapper-linux-x86-64 --out-dir bw-output make all
sonar-scanner -Dsonar.projectKey=udyni_tango -Dsonar.organization=udyni-github -Dsonar.sources=. -Dsonar.cfamily.build-wrapper-output=bw-output -Dsonar.cs.opencover.reportsPaths -Dsonar.sources=$SOURCES


for d in `find . -maxdepth 1 -mindepth 1 -type d -not -name ".*" -and -not -name "bw-output" -and -not -name "SpectrometerSrv"`; do
	if [ -f "$d/Makefile" ]; then
		sed -i 's/^RELEASE_TYPE = DEBUG/#RELEASE_TYPE = DEBUG/' $d/Makefile
		sed -i 's/^#RELEASE_TYPE = OPTIMIZED/RELEASE_TYPE = OPTIMIZED/' $d/Makefile
	fi
done
