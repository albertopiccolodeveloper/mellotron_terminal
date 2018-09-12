#!/bin/bash
#param $1 absolute path to samples folder

if [[ $1 == "" ]];then
    echo "No path to samples folder given..";
    exit;
fi

cd $1;

#folder operations
for folder in *; do
    nname="${folder##*/}";
    #controllo se il nome della cartella ha uno spazio
    if [[ $folder == *" "* ]];then
        mv -v -n "${folder}"  "${nname//[[:space:]]/_}";
    fi
done

#files operations
EXT="wav";
for folder in *; do
    for file in $folder/*; do
        if [[ $file == *"-"* ]]; then
            newname="${file%-*}";
            mv -v -n "$file"  "$newname.$EXT";
        fi
    done
done