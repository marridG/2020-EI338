echo "Make-File ..."
make
echo "Make-File Done"

echo ""
echo "Entering simple_shell"
./simple_shell <<'EOF'

# we have now entered the simple_shell, input commands
ls
EOF