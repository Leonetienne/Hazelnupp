all:
	# Copy all but the header of the readme here
	tail ../../readme.md -n +7 > index.md
	#
	# Run doxygen
	doxygen doxyfig
	#
	# Cleanup index.md
	rm index.md

