echo "The number of errors is: "
gcc -Wall "$1" 2>&1 | grep "warning:\|error:" | wc -l