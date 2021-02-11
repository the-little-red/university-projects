#!/bin/bash

# This script generates a .deb package

apt-get update && apt-get install -y devscripts

CHANGELOG="package/debian/changelog"
LINE=$(head -n 1 $CHANGELOG)
PACKAGE=$(echo "$LINE" | cut -d' ' -f1)
VERSION=$(echo "$LINE" | cut -d' ' -f2 | grep -o -E '[0-9]*\.[0-9]*\.[0-9]*')
tar --exclude=debian -czf "${PACKAGE}"_"${VERSION}".orig.tar.gz package/*

cd package || exit -1
debuild -us -uc
cd ../ || exit -1

mkdir build/
mv "${PACKAGE}"* build/
