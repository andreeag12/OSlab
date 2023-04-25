if test-d "$1"
then
    for files in $1/*
    do
        if test -f "$files"
        then
            echo -n "$files: " >> leaderbord