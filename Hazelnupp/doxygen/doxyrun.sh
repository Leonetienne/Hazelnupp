#!zsh

# Copy cut readme.md from root
tail ../../readme.md -n +5 > index.md

# Run doxygen
sudo doxygen doxyfig

# Delete index afterwards to prevent people from editing it
rm -f index.md
