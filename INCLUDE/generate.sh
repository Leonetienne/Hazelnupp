# Build the include files
echo Buildin the include files...
echo ../Hazelnupp/*.cpp | sed 's/\.\.\/Hazelnupp\/main\.cpp//g' | xargs deggl -o Hazelnupp -i

# Verify that they compile cleanly
echo Verifying that they compile
g++ Hazelnupp.cpp -c -Wall -Wextra -Wpedantic
rm -f Hazelnupp.o
