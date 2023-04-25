if test-d "$1"
then
    for files in $1/*
    do
        if test -f "$files"
        then
            echo -n "$files: " >> leaderbord
            gcc -Wall "$files" 2>&1 | grep "warning:\|error:" | wc -l >> leaderbord
        else
            echo "Not a reg file"
        fi
    done
else
    echo "Argument is not a directory"
fi