echo "Make-File ..."
make
echo "Make-File Done"

echo ""
echo "Entering simple_shell"

echo "Pre-Designed Commands Set"
./simple_shell <<'EOF'
echo !!
!!
echo DEBUG=False
echo --config debug false
--config debug false
echo !!
!!

echo ls
ls
echo ls > out.txt
ls > out.txt
echo cat < out.txt
cat < out.txt

echo ls | sort
ls | sort
echo cat < out.txt | sort > out_sorted.txt
cat < out.txt | sort > out_sorted.txt
echo cat out_sorted.txt
cat out_sorted.txt


echo "DEBUG=True"
--config debug true

ls
ls > out.txt
cat < out.txt

ls | sort
cat < out.txt | sort > out_sorted.txt

--config debug false
echo "Try Unrecognizable Commands, We can also Exit Shell by Calling \"exit\" Once"
echo 123456
123456
echo 123 456 789
123 456 789

exit
EOF

echo ""
echo "Now Try Your Commands"
./simple_shell