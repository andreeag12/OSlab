gcc -Wall "$1" 2>&1 | grep "error:" | wc -l 
gcc -Wall "$1" 2>&1 | grep "warning:" | wc -l
