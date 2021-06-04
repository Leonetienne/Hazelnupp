# Build the include files
echo "Building the include files..."
echo ../Hazelnupp/*.cpp | sed 's/\.\.\/Hazelnupp\/main\.cpp//g' | xargs deggl -o Hazelnupp -i

# Verify that they compile cleanly
echo "Verifying that they compile"
g++ Hazelnupp.cpp -c -S -o - -Wall -Wextra -Wpedantic > /dev/null
