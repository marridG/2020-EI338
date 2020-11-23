clear_dmesg(){
    echo "Clearing Kernel Log Buffer"
    dmesg -c
    echo "Kernel Log Buffer Cleared"
}

clear_dmesg
make
echo "Initialized"


# Exercises on Page P-15
echo "Running Codes for Project 2-2 on Page P-15"

# install module
echo "\tInstalling Module"
insmod pid_module.ko
dmesg
echo "\tModule Installed"

for i in 1 11 21 40 1000 666
do
    echo "Testing PID = $i"
    echo $i > /proc/pid
    cat /proc/pid
done

# remove module
echo "\tRemoving Module"
rmmod pid_module
dmesg
echo "\tModule Removed"