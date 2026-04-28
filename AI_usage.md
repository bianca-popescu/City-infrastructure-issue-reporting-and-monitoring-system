AI tool used: Gemini 3 Flash

Purpose: assistance for binary file manipulation and command-line argument parsing

Functions generated with AI assistance:
    parse_condition: used to split the field:operator:value string
    match_condition: used to match records to given conditions with different opperators (<, >, ==, etc.).
    remove_report: used to remove a report from reports.dat binary file, used AI-generated logic as a starting point for the shifting logic using lseek and fruncate.
    get_permissions_string: used to convert mode_t bits to a human-readable string, used AI to better undestand permission bits, groups and their importance.

Modifications/ debugging:
    struct fields: I mapped the AI's generic field names to my specific ones
    type correction: I corrected an AI suggestion in snprintf function for the integer threshold from %s to %d

Problems encountered:
    interpretation of relational operators: when passing filter conditions like "severity:>=>2", the terminal shell interprets the < and > characters
incorrectly, as input/output redirection operators rather than as a string for the program. To be able to pass the string as intented, I wrapped the 
condition in quotes.

intent: ./city_manager --filter district1 severity:>=:2
action: Executes ./city_manager --filter district1 severity: and redirects the result into a file named =:2
correct usage: ./city_manager --filter district1 "severity:>=:2"

Learning outcomes (from using AI):
    better undestanding of binary file navigation with lseek and physically resizing files with ftruncate, permission bits usage and conditions