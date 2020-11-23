clear_dmesg(){
    echo "Clearing Kernel Log Buffer"
    dmesg -c
    echo "Kernel Log Buffer Cleared"
}

clear_dmesg
make
echo "Initialized"


# Exercises on Page P-4
echo "Running Codes for Project 2-2 on Page P-15"

# install module
echo "\tInstalling Module"
insmod pid_module.ko
dmesg
echo "\tModule Installed"

echo "Testing PID = 1395"
echo "1395" > /proc/pid
sleep 5
cat /proc/pid
echo

echo "Testing PID = 1671"
echo "7346" > /proc/pid
cat /proc/pid
echo

# remove module
echo "\tRemoving Module"
rmmod pid_module
dmesg
echo "\tModule Removed"