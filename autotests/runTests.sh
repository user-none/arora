#!/bin/sh

for directory in `ls -d */`
do
    name=`basename $directory`

    if [ $name == "modeltest" ]
    then
        continue
    fi

    if [ $name == "downloadmanager" ]
    then
        continue
    fi

    cd $name

    if [ ! -f $name ]
    then
        printf "$name/$name is not compiled.\n"
    else
        ./$name -silent
    fi

    printf "\n"
    cd ../
done