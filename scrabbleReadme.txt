README

BUILDING

	Simply run buildScrabble.sh and the program will compile and launch.

PLAYING

	Each axis can be between 10 and 20 units long.
	Add words to the board by specifying an index, writing a word, and picking vertical or not.
	If you wish to expand an already existing word (eg. FORM to FORMED) type the entire new word, not just the prefix or suffix (so in the previous example, you would select the indices for the F in FORM and enter FORMED).

NOTES:
	The source can be found at  https://github.com/swordsmanzan/scrabble.git. For convenience, I've included it in an archive as well.
	The word checking method simply returns true, but it does so on each newly created word with each move.
	I did not do extremely aggressive input validation; any input numbers or words will check that they are within a valid range, but putting a '!' into a number field will still cause problems.
	I had a lot of fun with this programming challenge and I hope you can have similar fun testing it!
