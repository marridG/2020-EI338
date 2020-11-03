clear_dmesg(){
    echo "Clearing Kernel Log Buffer"
    dmesg -c
    echo "Kernel Log Buffer Cleared"
}

clear_dmesg
make
echo "Initialized"


# Exercises on Page P-4
echo "Running Codes for Exercises on Page P-4"

# install module
echo "\tInstalling Module"
insmod ex_P-4.ko
dmesg
echo "\tModule Installed"

# remove module
echo "\tRemoving Module"
rmmod ex_P-4
dmesg
echo "\tModule Removed"



# Assignment 1 on Page P-7
clear_dmesg
echo "Running Codes for Assignment 1 on Page P-7. (Test Time Intervals: 1, 2, 3, 4 Seconds)"

# install module
echo "\tInstalling Module"
insmod asgn_1_jiffies.ko
dmesg
echo "\tModule Installed"

# 5 test rounds
cat /proc/jiffies
sleep 1
cat /proc/jiffies
sleep 2
cat /proc/jiffies
sleep 3
cat /proc/jiffies
sleep 4
cat /proc/jiffies

# remove module
echo "\tRemoving Module"
rmmod asgn_1_jiffies
dmesg
echo "\tModule Removed"



# Assignment 2 on Page P-7
clear_dmesg
echo "Running Codes for Assignment 2 on Page P-7. (Test Time Intervals: 1, 2, 3, 4 Seconds)"

# install module
echo "\tInstalling Module"
insmod asgn_2_seconds.ko
dmesg
echo "\tModule Installed"

# 5 test rounds
cat /proc/seconds
sleep 1
cat /proc/seconds
sleep 2
cat /proc/seconds
sleep 3
cat /proc/seconds
sleep 4
cat /proc/seconds

# remove module
echo "\tRemoving Module"
rmmod asgn_2_seconds
dmesg
echo "\tModule Removed"

