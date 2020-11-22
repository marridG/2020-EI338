echo "Make-File ..."
make
echo "Make-File Done"

echo ""
echo "Entering simple_shell"

echo "Pre-Designed Commands Set"
./simple_shell <<'EOF'
echo "DEBUG = False"
!!
--config debug false
!!

ls
ls > out.txt
cat < out.txt

ls | sort
cat < out.txt | sort > out_sorted.txt
cat out_sorted.txt


echo "DEBUG = True"
!!
--config debug true
!!

ls
ls > out.txt
cat < out.txt

ls | sort
cat < out.txt | sort > out_sorted.txt
cat out_sorted.txt


echo "Try Unrecognizable Commands, We can also Exit Shell by Calling \"exit\" Once"
123456
123 456 789

exit
EOF

echo ""
echo "Now Try Your Commands"
./simple_shell