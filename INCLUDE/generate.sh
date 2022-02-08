# Build the include files
echo "Building the include files..."
./deggl -i ../Hazelnupp/*.cpp -o Hazelnupp

# Verify that they compile cleanly
echo "Verifying that they compile"
g++ Hazelnupp.cpp -c -S -std=c++17 -o - -Wall -Wextra -Wpedantic > /dev/null
