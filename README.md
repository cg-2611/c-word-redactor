# C Word Redactor

This was a problem given to me at university. I was not allowed to use the string.h library.

The purpose of the program is to take a list of strings from one file, and redact all occurrences of the words from another file by replacing each character from the word with an '*'. The program will only redact exact matches to the words in the list of strings, i.e. if a word contains an uppercase first letter in the list of words, only the occurrences of the word in the text file that have an uppercase letter are redacted and the ones with a lowercase letter remain untouched. The same goes for strings in the list of strings that contain multiple words, only a substring in the text file will be redacted and not every occurrence of each word in the string from the list of strings, i.e. if the string from the list of strings is "two words" any occurrences of "two" or "words" found in the text file will remain untouched and only occurrences of "two words" exactly will be redacted.

### Usage
---
The program can be compiled any way you wish, however when running the program, there are some command line arguments that need to the passed in the following order:
- `<text_file_name>`: (required) the name of the text file that contains the the text in which the words are to be redacted from.
- `<redact_words_file_name>`: (required) the name of the file that contains the words that are to be redacted from `<text_file_name>`
- `<output_file_name>`: (optional) the name of the file that will contain the text from `<text_file_name>` with the words redacted. The default file if this option is not specified is `result.txt`.
