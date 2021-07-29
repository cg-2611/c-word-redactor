#include <stdio.h>
#include <stdlib.h>

char **get_words_to_redact(const char*redact_filename, int *word_count)
{
    FILE *word_file;

    if ((word_file = fopen(redact_filename, "r")) == NULL)
    {
        return NULL;
    }

    char **word_array;

    char c;
    int line_count = 0;

    int i, j = 0;
    int character_count = 0;

    // count number of lines in the file
    while ((c = fgetc(word_file)) != EOF)
    {
        if (c == '\n')
        {
            line_count++;
        }
    }

    // allocate memory to store a string for each line of the file
    word_array = malloc(line_count * sizeof(*word_array));

    if (word_array == NULL)
    {
        fclose(word_file);
        return NULL;
    }


    rewind(word_file); // go back to the beginning of the file

    // loop through the file and count the number of characters in each line
    for (i = 0; i < line_count; i++)
    {
        character_count = 0;
        while ((c = fgetc(word_file)) != '\n')
        {
            character_count++;
        }

        // for each line, allocate enough memory to store the number of characters in the line
        word_array[i] = malloc((character_count + 1) * sizeof(*word_array[i]));

        if (word_array[i] == NULL)
        {
            fclose(word_file);
            return NULL;
        }

    }

    rewind(word_file);

    // loop through the file a final time and store each line of the file as a string
    for (i = 0; i < line_count; i++)
    {
        j = 0;
        while ((c = fgetc(word_file)) != '\n')
        {
            word_array[i][j++] = c;
        }

        word_array[i][j] = '\0'; // add the null terminator to the end
    }

    fclose(word_file);

    *word_count = line_count;
    return word_array;
}

int redact_words(const char *text_filename, const char *redact_filename, const char *output_filename)
{
    FILE *text;
    FILE *result;

    if ((text = fopen(text_filename, "r")) == NULL)
    {
        perror("Error opening text file");
        exit(2);
    }

    if ((result = fopen(output_filename, "w")) == NULL)
    {
        fclose(text);
        perror("Error opening output file");
        exit(3);
    }

    int word_count;

    // store the words from the redact file in an array of strings and get the length of the array
    char **redact_array = get_words_to_redact(redact_filename, &word_count);

    if (redact_array == NULL)
    {
        fclose(text);
        fclose(result);
        perror("Error reading from redact file");
        exit(3);
        return 4;
    }

    char *line_buffer = NULL;
    size_t line_buffer_size = 0;
    ssize_t line_length = 0;

    int i, j = 0, k;

    int match_found = 0;

    // loop through every line of the text file
    while ((line_length = getline(&line_buffer, &line_buffer_size, text)) > 0)
    {
        // loop through every character in the line
        for (i = 0; i < line_length; i++)
        {
            // to optimise the search, ignore any '*' characters
            if (line_buffer[i] != '*')
            {
                j = 0;
                // loop through each word from the redact file
                while (j < word_count)
                {
                    // loop through each character of word j
                    for (k = 0; redact_array[j][k] != '\0'; k++)
                    {
                        // if the character from the text file doesn't match,
                        // try the next word from the redact file
                        if (line_buffer[i + k] != redact_array[j][k])
                        {
                            j++;
                            break;
                        }

                        // if all the characters match, and the end of the word is reached,
                        // then a match has been found
                        if (redact_array[j][k + 1] == '\0')
                        {
                            match_found = 1;
                        }
                    }

                    // if a match has been found, replace all the characters of the match
                    // in the line_buffer with an '*'
                    if (match_found == 1)
                    {
                        match_found = 0;
                        for (k = 0; redact_array[j][k] != '\0'; k++)
                        {
                            if (line_buffer[i + k] != ' ')
                            {
                                line_buffer[i + k] = '*';
                            }
                        }
                    }
                }
            }
        }

        fprintf(result, "%s", line_buffer); // print the line_buffer to the result file
    }

    free(line_buffer);
    line_buffer = NULL;

    for (i = 0; i < word_count; i++)
    {
        free(redact_array[i]);
        redact_array[i] = NULL;
    }

    free(redact_array);
    redact_array = NULL;

    fclose(text);
    fclose(result);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "\nInvalid arguments passed\n");
        fprintf(stderr, "\nUsage:\n");
        fprintf(stderr, "\tword_redactor <text_file_name> <redact_words_file_name> <output_file_name>\n\n");
        exit(EXIT_FAILURE);
    }

    const char *input_file = argv[1];
    const char *redact_file = argv[2];
    const char *output_file;

    if (argc == 4)
    {
        output_file = argv[3];
    }
    else
    {
        output_file = "result.txt";
    }

    int exit_status = redact_words(input_file, redact_file, output_file);

    return exit_status;
}
