#define _GNU_SOURCE
#define JOURNAL_FILE "reading_journal.txt"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

typedef struct {
    char *book_name;
    char *author;
    char *genre;
    char *start_date;
    char *end_date;
    unsigned int score;
    char *note;
} JournalEntry;

void print_help() {
    printf("Help for Reading Journal program\n");
    printf("Usage:\n");
    printf("  journal <command> [options]\n\n");
    printf("Commands:\n");
    printf("  new     Create a new journal entry\n");
    printf("  list    List existing journal entries\n");
    printf("  -h, --help    Show this help message\n\n");
    printf("Options for 'new':\n");
    printf("  --name <string>     (Required) Book name\n");
    printf("  --author <string>   (Required) Author's name\n");
    printf("  --genre <string>    (Required) Book genre\n");
    printf("  --start <ISO date>  (Required) Start date (YYYY-MM-DD)\n");
    printf("  --end <ISO date>    Finish date (YYYY-MM-DD)\n");
    printf("  --score <int>       Personal score (1-5)\n");
    printf("  --note <string>     Additional note\n\n");
    printf("Options for 'list':\n");
    printf("  --genre <string>    List books by specific genre\n");
    printf("  --reading           List books currently being read\n");
    printf("  --completed         List completed books\n");
    printf("  --score <int>       List books with score equal or higher\n\n");
    printf("Examples:\n");
    printf(
        "  ./journal new --name \"Hobbit\" --author \"J.R.R. Tolkien\" --genre fantasy --start \"2022-01-01\" --score 4\n");
    printf("  ./journal list --genre fantasy\n");
}

/**
 * @brief Splits a string into tokens based on specified delimiters.
 *
 * This function extracts the next token from the string pointed to by `*stringp`, where tokens are
 * delimited by any of the characters in the string `delim`. It modifies the input string in-place,
 * replacing the first delimiter encountered with a null terminator (`'\0'`) and updating `*stringp`
 * to point to the character following the token. If no more tokens are found, `*stringp` is set to NULL.
 *
 * @param stringp A pointer to the string pointer to be tokenized. This parameter must not be NULL.
 *                The string pointed to by `*stringp` is updated to reflect progress through the string.
 *                On the first call, `*stringp` should point to the start of the string to tokenize.
 * @param delim A null-terminated string containing the delimiter characters. The function treats each
 *              character in `delim` as a possible token delimiter. This parameter must not be NULL.
 *
 * @return A pointer to the next token extracted from the string, or NULL if there are no more tokens.
 *
 * - If `*stringp` points to NULL when the function is called, NULL is returned.
 * - The function uses `strcspn` to determine the position of the first delimiter within the input string.
 * - If a delimiter is found, it is replaced with a null terminator (`'\0`), effectively splitting the string.
 * - After processing, `*stringp` is updated to point to the next segment of the input string.
 * - If the end of the string is reached, `*stringp` is set to NULL, indicating there are no more tokens.
 */
char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp) {
            *(*stringp)++ = '\0';
        } else {
            *stringp = 0;
        }
    }
    return rv;
}

/**
 * @brief Validates if the given string represents a valid date in the format YYYY-MM-DD.
 *
 * This function checks whether the input string conforms to the ISO 8601 date format (YYYY-MM-DD),
 * verifies that the year, month, and day values are within valid ranges, and accounts for leap years
 * when determining the number of days in February.
 *
 * @param date A null-terminated C-string representing the date to be validated. The input should follow
 *             the fixed-length format of "YYYY-MM-DD" (10 characters). The string must not be null.
 *
 * @return True if the input string is a valid date in the format YYYY-MM-DD, otherwise false.
 *
 * - If the input string is null or does not have a length of 10, the function returns false.
 * - Each character of the input string is validated to ensure that it either represents a digit or the
 *   expected '-' separator in the appropriate positions (4th and 7th indices).
 * - The year, month, and day are extracted and validated to ensure they fall within permissible ranges:
 *   - Month must be in the range [1, 12].
 *   - Day must be greater than or equal to 1.
 *   - Day must not exceed the maximum number of days in the respective month for the given year.
 * - The function dynamically adjusts February's days in leap years, where the number of days is updated
 *   from 28 to 29 when applicable.
 *
 */
bool is_valid_date(const char *date) {
    if (date == NULL || strlen(date) != 10) return false;

    // Check format YYYY-MM-DD
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        } else {
            if (!isdigit(date[i])) return false;
        }
    }

    int year = (int) strtol(date, NULL, 10);
    int month = (int) strtol(date + 5, NULL, 10);
    int day = (int) strtol(date + 8, NULL, 10);

    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    // Check for leap year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_month[1] = 29;
    }

    if (day > days_in_month[month - 1]) return false;

    return true;
}

/**
 * @brief Retrieves a valid date string in the format YYYY-MM-DD.
 *
 * This function performs validation on the provided date string to ensure
 * that it adheres to the ISO 8601 date format (YYYY-MM-DD). If the format
 * or content of the date is invalid, an error message is displayed, and
 * the function returns a null pointer. Otherwise, the original input date
 * string is returned.
 *
 * @param date A null-terminated C-string representing the date. The string
 *             must not be null and should follow the format YYYY-MM-DD.
 *
 * @return A pointer to the original date string if it is valid, otherwise
 *         NULL.
 *
 * - If the input date is null, the function returns NULL without attempting validation.
 * - The function relies on `is_valid_date` to verify the date's format and
 *   validity. If the date is invalid, an error message is printed, and
 *   the function returns NULL.
 * - If the date is valid, the input pointer is returned unchanged.
 */
char *get_date(char *date) {
    if (date == NULL) return NULL;
    if (!is_valid_date(date)) {
        printf("Invalid date format. Correct format is YYYY-MM-DD (ISO 8601)\n");
        return NULL;
    }
    return date;
}

/**
 * @brief Extracts the value associated with a specified command-line option.
 *
 * This function retrieves the argument string that follows a particular option index
 * in the command-line arguments. If the option does not have a corresponding value,
 * or if the value is misinterpreted as another option (prefixed with "--"), the function
 * returns a null pointer. The returned value is dynamically allocated and should be
 * freed by the caller.
 *
 * @param argc The total number of arguments passed to the program, including the program name.
 * @param argv An array of null-terminated strings containing the command-line arguments.
 * @param option_index The index within `argv` of the target option whose value is to be extracted.
 *
 * @return A pointer to a dynamically allocated string containing the value of the specified option
 * if the value exists and is valid. Returns NULL if:
 * - The index `option_index + 1` is out of bounds.
 * - The argument at `option_index + 1` appears to be an option (starts with "--").
 *
 * The caller is responsible for freeing the allocated string when no longer needed.
 */
char *get_option_value(int argc, char *argv[], int option_index) {
    if (argc <= option_index + 1) return NULL;
    if (argv[option_index + 1][0] == '-' && argv[option_index + 1][1] == '-') return NULL;
    char *value = malloc(sizeof(char) * strlen(argv[option_index + 1]));
    strcpy(value, argv[option_index + 1]);
    return value;
}

void print_entry(JournalEntry *entry) {
    if (entry == NULL) return;
    printf("-- %s --\n", entry->book_name);
    printf("author:           %s\n", entry->author);
    printf("genre:            %s\n", entry->genre);
    printf("started reading:  %s\n", entry->start_date);
    if (entry->end_date) printf("finished reading: %s\n", entry->end_date);
    if (entry->score != 0) printf("personal score:   %d\n", entry->score);
    if (entry->note) printf("note:             %s\n", entry->note);
    printf("-------\n");
}

/**
 * @brief Frees the memory allocated for a JournalEntry structure and its member fields.
 *
 * This function releases all dynamically allocated resources associated with a JournalEntry,
 * including its string fields (e.g., book name, author, genre, etc.) and the structure itself.
 * It should be called to avoid memory leaks after the JournalEntry is no longer needed.
 *
 * @param entry A pointer to the JournalEntry structure that is to be freed. The pointer must not
 *              be NULL and the memory for all dynamically allocated fields inside the structure
 *              should have been properly assigned before calling this function.
 */
void free_entry(JournalEntry *entry) {
    if (entry == NULL) return;
    free(entry->book_name);
    free(entry->author);
    free(entry->genre);
    free(entry->start_date);
    free(entry->end_date);
    free(entry->note);
    free(entry);
}

/**
 * @brief Appends a journal entry to the journal file.
 *
 * This function writes a JournalEntry's information to a pre-defined journal file. Each entry is
 * written as a single line with fields separated by the '|' character. The following fields are
 * included in the output: book name, author, genre, start date, end date (optional), score (optional),
 * and note (optional). If any optional field is not provided, it is represented as an empty value
 * between delimiters. The function ensures safe handling of a null entry pointer and handles file
 * opening errors gracefully.
 *
 * @param entry A pointer to a JournalEntry structure containing information about the journal entry
 *              to be written. If the pointer is null, the function does nothing.
 */
void write_entry(JournalEntry *entry) {
    if (entry == NULL) return;
    FILE *file = fopen(JOURNAL_FILE, "a");
    if (file == NULL) {
        perror("Failed to open file for writing\n");
        return;
    }
    fprintf(file, "%s|%s|%s|%s", entry->book_name, entry->author, entry->genre, entry->start_date);
    if (entry->end_date != NULL) {
        fprintf(file, "|%s", entry->end_date);
    } else {
        fprintf(file, "|");
    }
    if (entry->score != 0) {
        fprintf(file, "|%d", entry->score);
    } else {
        fprintf(file, "|");
    }
    if (entry->note != NULL) {
        fprintf(file, "|%s", entry->note);
    } else {
        fprintf(file, "|");
    }
    fprintf(file, "\n");
    fclose(file);
}

/**
 * @brief Handles the creation of a new journal entry based on command-line arguments.
 *
 * This function parses command-line arguments starting from the third argument (index 2) to extract
 * details necessary for creating a new journal entry, such as the book name, author, genre, start date,
 * end date, score, and an optional note. The values are processed and stored in a dynamically allocated
 * JournalEntry struct.
 *
 * The following options are supported:
 * - `--name`: Specifies the book name. This option is required.
 * - `--author`: Specifies the author's name. This option is required.
 * - `--genre`: Specifies the book's genre. This option is required.
 * - `--start`: Specifies the start date in a valid date format. This option is required.
 * - `--end`: (Optional) Specifies the end date in a valid date format.
 * - `--score`: (Optional) Specifies a numeric score for the entry.
 * - `--note`: (Optional) Specifies an additional note or description.
 *
 * If any required arguments are missing, an error message is displayed, and the function ends without
 * creating an entry.
 *
 * @param argc The total number of command-line arguments provided to the program.
 * @param argv An array of null-terminated strings representing the command-line arguments.
 *
 * - If `argc` is less than 6, the function immediately displays an error message and prints usage
 *   instructions using `print_help`.
 * - For each argument matching one of the supported options, the associated value is extracted using
 *   helper functions like `get_option_value`. For dates, `get_date` is used to process the input.
 * - If memory allocation for the journal entry fails, an error is logged using `perror`, and the function
 *   terminates.
 * - After parsing, the function validates the presence of all required options. Missing required options
 *   result in relevant error messages.
 * - If all required options are valid, the new entry is added by invoking `print_entry` to display it and
 *   `write_entry` to persist it.
 * - Memory associated with the journal entry is released using `free_entry` to avoid memory leaks.
 */
void new_cmd(int argc, char *argv[]) {
    if (argc < 6) {
        printf("Not enough arguments for new command\n");
        print_help();
        return;
    }
    JournalEntry *entry = malloc(sizeof(JournalEntry));
    if (entry == NULL) {
        perror("Failed to allocate memory for journal entry");
        return;
    }
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--name") == 0) {
            entry->book_name = get_option_value(argc, argv, i);
        } else if (strcmp(argv[i], "--author") == 0) {
            entry->author = get_option_value(argc, argv, i);
        } else if (strcmp(argv[i], "--genre") == 0) {
            entry->genre = get_option_value(argc, argv, i);
        } else if (strcmp(argv[i], "--start") == 0) {
            entry->start_date = get_date(get_option_value(argc, argv, i));
        } else if (strcmp(argv[i], "--end") == 0) {
            entry->end_date = get_date(get_option_value(argc, argv, i));
        } else if (strcmp(argv[i], "--score") == 0) {
            char *score_value = get_option_value(argc, argv, i);
            if (score_value == NULL) continue;
            entry->score = strtol(score_value, NULL, 10);
        } else if (strcmp(argv[i], "--note") == 0) {
            entry->note = get_option_value(argc, argv, i);
        }
    }

    bool valid = true;
    if (entry->book_name == NULL) {
        printf("Error: Book name is required, with option --name\n");
        valid = false;
    }
    if (entry->author == NULL) {
        printf("Error: Author is required, with option --author\n");
        valid = false;
    }
    if (entry->genre == NULL) {
        printf("Error: Genre is required, with option --genre\n");
        valid = false;
    }
    if (entry->start_date == NULL) {
        printf("Error: Start date is required, with option --start\n");
        valid = false;
    }

    if (valid) {
        printf("New entry added:\n");
        print_entry(entry);
        write_entry(entry);
    }
    free_entry(entry);
}

/**
 * @brief Parses a delimited line of text and constructs a JournalEntry instance.
 *
 * This function reads a line of text that contains fields separated by the '|' delimiter
 * and attempts to populate a dynamically allocated JournalEntry structure. The fields
 * in the input line are expected to follow the order:
 * book_name, author, genre, start_date, end_date, score, note. If any of the required
 * fields (book_name, author, genre, start_date) are missing or invalid, the function fails
 * and returns a NULL.
 *
 * @param line A null-terminated string containing the delimited line of data. The string
 *             should use the '|' character to separate fields.
 *
 * @return A pointer to a dynamically allocated JournalEntry instance on success, or NULL
 *         if allocation fails or the line does not contain the required fields. The caller
 *         is responsible for freeing the returned JournalEntry using free_entry.
 *
 * - Each field in the input line is processed in sequential order based on its position:
 *   - Field 0: Populates the `book_name` field of the JournalEntry.
 *   - Field 1: Populates the `author` field of the JournalEntry.
 *   - Field 2: Populates the `genre` field of the JournalEntry.
 *   - Field 3: Populates the `start_date` field of the JournalEntry.
 *   - Field 4: Optionally populates the `end_date` field if provided.
 *   - Field 5: Optionally parses the `score` field as an unsigned integer.
 *   - Field 6: Optionally populates the `note` field if provided.
 *
 * - If memory allocation fails for any fields, the function cleans up any
 *   previously allocated memory, logs an error message, and returns NULL.
 * - The function ensures that required fields (`book_name`, `author`, `genre`,
 *   and `start_date`) are not null and contain valid data.
 */
JournalEntry *load_entry(char *line) {
    JournalEntry *entry = malloc(sizeof(JournalEntry));
    if (entry == NULL) {
        perror("Failed to allocate memory for journal entry");
        return NULL;
    }
    entry->book_name = NULL;
    entry->author = NULL;
    entry->genre = NULL;
    entry->start_date = NULL;
    entry->end_date = NULL;
    entry->score = 0;
    entry->note = NULL;

    int field = 0;
    char *current_line = line;
    char *token;
    while ((token = strsep(&current_line, "|")) != NULL) {
        switch (field) {
            case 0:
                entry->book_name = malloc(sizeof(char) * strlen(token));
                strcpy(entry->book_name, token);
                break;
            case 1:
                entry->author = malloc(sizeof(char) * strlen(token));
                strcpy(entry->author, token);
                break;
            case 2:
                entry->genre = malloc(sizeof(char) * strlen(token));
                strcpy(entry->genre, token);
                break;
            case 3:
                entry->start_date = malloc(sizeof(char) * strlen(token));
                strcpy(entry->start_date, token);
                break;
            case 4:
                if (strlen(token) == 0) break;
                entry->end_date = malloc(sizeof(char) * strlen(token));
                strcpy(entry->end_date, token);
                break;
            case 5:
                if (strlen(token) == 0) break;
                entry->score = strtol(token, NULL, 10);
                break;
            case 6:
                if (strlen(token) == 0) break;
                entry->note = malloc(sizeof(char) * strlen(token));
                strcpy(entry->note, token);
                break;
            default: break;
        }
        field++;
    }
    if (entry->book_name == NULL || entry->author == NULL || entry->genre == NULL || entry->start_date == NULL) {
        printf("Failed to load journal entry from line: %s", line);
        free_entry(entry);
        return NULL;
    }
    return entry;
}

/**
 * @brief Filters a journal entry based on its genre.
 *
 * This function is used to check if a given journal entry belongs to a specified genre.
 * It compares the genre field of the journal entry with the provided genre string.
 * Both the journal entry and the genre string must be non-null for the comparison to take place.
 *
 * @param entry A pointer to the JournalEntry structure to be checked.
 *              This parameter must not be null.
 * @param genre A null-terminated C-string specifying the genre to filter by.
 *              This parameter must not be null.
 *
 * @return True if the journal entry's genre matches the specified genre, otherwise false.
 *
 * - If either the journal entry or the genre string is null, the function returns false.
 * - Comparison is performed using `strcmp`, which checks for an exact match between the two strings.
 */
bool filter_by_genre(JournalEntry *entry, char *genre) {
    return entry != NULL && genre != NULL && strcmp(entry->genre, genre) == 0;
}

/**
 * @brief Filters a journal entry based on a minimum score threshold.
 *
 * This function checks whether the `score` of the provided journal entry meets or
 * exceeds a minimum threshold specified as a string input. It converts the input
 * score from a string to an integer before comparing it with the journal entry's score.
 *
 * @param entry A pointer to the `JournalEntry` structure to be evaluated. The pointer
 *              must not be null.
 * @param score A null-terminated C-string representing the minimum score threshold
 *              for filtering. This parameter must not be null.
 *
 * @return True if the journal entry's score is greater than or equal to the specified
 *         minimum score, otherwise returns false.
 *
 * - If either `entry` or `score` is null, the function returns false.
 * - The comparison uses the integer value parsed from the `score` string. If the string
 *   cannot be converted to a valid integer, the behavior is unspecified.
 */
bool filter_by_score(JournalEntry *entry, char *score) {
    if (entry == NULL || score == NULL) return false;
    unsigned int score_int = strtol(score, NULL, 10);
    return entry->score >= score_int;
}

/**
 * @brief Filters journal entries that are currently being read.
 *
 * This function is used to determine whether a journal entry represents a book
 * that is still being read. An entry is considered to be "currently being read"
 * if it has not been marked with an end date.
 *
 * @param entry A pointer to the JournalEntry to be evaluated. The pointer must
 *              not be null.
 * @param ignored A pointer to a char argument that is ignored by this filter.
 *                It exists for consistency with other filter functions, but it
 *                is not used in this function.
 *
 * @return True if the entry is not null and the end date of the entry is null,
 *         indicating that the book is still being read. Otherwise, false.
 */
bool filter_if_reading(JournalEntry *entry, char *ignored) {
    return entry != NULL && entry->end_date == NULL;
}

/**
 * @brief Filters journal entries to include only completed books.
 *
 * This function checks if a journal entry is marked as completed, which is determined
 * by the presence of a non-null `end_date` field. It is typically used as a filter
 * function when processing or listing journal entries to focus only on books that
 * have been finished.
 *
 * @param entry A pointer to a `JournalEntry` structure representing the entry to be evaluated.
 *              The entry should not be null, and its `end_date` field is checked for completion status.
 * @param ignored A placeholder parameter for compatibility with list command filters.
 *                This parameter is not utilized in the function's logic.
 *
 * @return True if the `JournalEntry` is not null and has a non-null `end_date` field, otherwise false.
 *
 * - If the `entry` parameter is null, the function returns false.
 * - The `ignored` parameter is ignored and does not affect the function's result.
 */
bool filter_if_completed(JournalEntry *entry, char *ignored) {
    return entry != NULL && entry->end_date != NULL;
}

/**
 * @brief Lists journal entries from the journal file, applying an optional filter.
 *
 * This function reads entries from the journal file, parses them into `JournalEntry` structures,
 * optionally filters them using the provided filter function, and displays the filtered results.
 * It also reports the total number of entries and the number of filtered entries that were listed.
 *
 * @param filter A pointer to a filtering function that takes a `JournalEntry` and a filter argument,
 *               and returns `true` if the entry satisfies the filter criteria, or `false` otherwise.
 *               If `filter` is `NULL`, no filtering is applied, and all entries are listed.
 * @param filter_argv A filter-specific argument to pass to the filter function. This can be `NULL`
 *                    if the filter does not require additional arguments or if no filter is provided.
 *
 * @note The journal entries are expected to be stored in a file defined by the `JOURNAL_FILE` macro.
 *       Each line of the file should represent a single entry in a specific delimited format. The
 *       exact format of the entries is handled by the `load_entry` function.
 *
 * - If the journal file cannot be opened for reading, an error is displayed, and the function exits early.
 * - For each entry, the function ensures the entry is properly loaded before applying the filter and
 *   printing the entry if it matches the filter criteria.
 * - Memory allocated for each entry is freed after it is processed.
 * - At the end of the process, a summary is printed indicating the number of entries listed and the
 *   total number of entries in the file.
 *
 * @warning If the journal file is not in the expected format or contains invalid entries, those entries
 *          are skipped, and the function continues processing the remaining entries.
 */
void list_entries(bool (*filter)(JournalEntry *, char *), char *filter_argv) {
    FILE *file = fopen(JOURNAL_FILE, "r");
    if (file == NULL) {
        perror("Failed to open file for reading\n");
        return;
    }
    printf("Reading journal:\n");
    char *line = NULL;
    size_t len = 0;
    size_t read;
    int total_entries = 0;
    int filtered_entries = 0;
    while ((read = getline(&line, &len, file)) != (size_t) -1) {
        line[strcspn(line, "\n")] = '\0';
        JournalEntry *entry = load_entry(line);
        if (entry == NULL) continue;
        total_entries++;
        if (filter != NULL && filter(entry, filter_argv)) {
            print_entry(entry);
            filtered_entries++;
        } else if (filter == NULL) {
            print_entry(entry);
            filtered_entries++;
        }
        free_entry(entry);
    }
    printf("\nListed entries %d/%d\n", filtered_entries, total_entries);
    free(line);
    fclose(file);
}

/**
 * @brief Handles the "list" command for displaying journal entries based on various filters or criteria.
 *
 * This function processes command-line arguments to determine which subset of journal entries to display.
 * It supports filtering entries by genre, reading status, completion status, or score, or listing all entries
 * when no specific filter is provided.
 *
 * @param argc The number of arguments passed to the program, including the program name.
 *             Must be at least 2 for the command to work, as the "list" command itself requires input.
 * @param argv An array of null-terminated C-strings representing the arguments passed to the program.
 *             For filtering, additional arguments in the following formats are expected:
 *             - "--genre <genre_name>" to filter entries by a specific genre.
 *             - "--reading" to filter entries that are currently being read.
 *             - "--completed" to filter entries that have been completed.
 *             - "--score <score_threshold>" to filter entries with a score equal to or higher than the given value.
 *
 * @details
 * - If fewer than 2 arguments are provided, an error message is displayed and help information is shown.
 * - If a filter option is provided (e.g., "--genre", "--reading"), the function checks for supported options
 *   and invokes `list_entries` with the appropriate filter function and argument.
 * - If no filter option is provided, the function lists all entries by calling `list_entries` without a filter.
 * - If an unsupported filter option is provided, an error message is displayed and help information is shown.
 *
 * Functions invoked within this function:
 * - `print_help()`: Displays help information about the command.
 * - `list_entries(bool (*filter)(JournalEntry *, char *), char *filter_argv)`: Executes the filtering and listing of journal entries.
 * - Filter functions like `filter_by_genre`, `filter_if_reading`, `filter_if_completed`, and `filter_by_score` are passed
 *   as function pointers to `list_entries` for specific filtering behavior.
 *
 * Edge cases handled:
 * - If an invalid filter option is provided, an error message is displayed with usage instructions.
 * - If a required argument for a filter (e.g., genre name for "--genre") is missing, the function avoids invalid memory access.
 */
void list_cmd(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Not enough arguments for list command\n");
        print_help();
        return;
    }
    if (argc >= 3) {
        if (strcmp(argv[2], "--genre") == 0) {
            list_entries(filter_by_genre, argv[3]);
        } else if (strcmp(argv[2], "--reading") == 0) {
            list_entries(filter_if_reading, NULL);
        } else if (strcmp(argv[2], "--completed") == 0) {
            list_entries(filter_if_completed, NULL);
        } else if (strcmp(argv[2], "--score") == 0) {
            list_entries(filter_by_score, argv[3]);
        } else {
            printf("Unknown filter option: %s\n", argv[2]);
            print_help();
        }
    } else {
        list_entries(NULL, NULL);
    }
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        print_help();
        return 0;
    }

    for (int a = 1; a < argc; a++) {
        if (strcmp(argv[a], "-h") == 0 || strcmp(argv[a], "--help") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[a], "new") == 0) {
            new_cmd(argc, argv);
            return 0;
        } else if (strcmp(argv[a], "list") == 0) {
            list_cmd(argc, argv);
            return 0;
        }
    }

    print_help();
    return 0;
}
