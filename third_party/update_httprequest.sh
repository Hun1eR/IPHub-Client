#!/bin/bash

tmpdir=$(mktemp -d)

git clone --depth 1 --recursive --shallow-submodules -b master https://github.com/elnormous/HTTPRequest.git $tmpdir
ret=$?

if test "$ret" -eq 0
then
    echo
    echo 'Commit:' $(git -C $tmpdir log -1 --format='%H')
    echo 'Author:' $(git -C $tmpdir log -1 --format='%an, %ar')
    echo 'Subject:' $(git -C $tmpdir log -1 --format='%s')
    echo

    mkdir -p "../deps/httprequest/include/httprequest"
    ret=$?

    if test "$ret" -eq 0
    then
        echo 'Updating HTTPRequest...'
        cp -f "$tmpdir/include/HTTPRequest.hpp" "../deps/httprequest/include/httprequest/httprequest.h"
        ret=$?

        if test "$ret" -eq 0
        then
            echo 'Successfully completed.'
        fi
    fi
fi

rm -rf $tmpdir

echo
read -p 'Press [Enter] key to exit...'
