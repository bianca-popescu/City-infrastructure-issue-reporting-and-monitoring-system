AI tool used: Gemini 3 Flash

Phase 1:

Purpose: assistance for binary file manipulation and command-line argument parsing.

Functions generated with AI assistance:
    parse_condition: used to split the field:operator:value string.
    match_condition: used to match records to given conditions with different operators (<, >, ==, etc.).
    remove_report: used to remove a report from reports.dat binary file, used AI-generated logic as a starting point 
for the shifting logic using lseek and ftruncate.
    get_permissions_string: used to convert mode_t bits to a human-readable string, used AI to better understand 
permission bits, groups and their importance

Modifications/ debugging:
    struct fields: I mapped the AI's generic field names to my specific ones
    type correction: I corrected an AI suggestion in snprintf function for the integer threshold from %s to %d.
    misinterpretation of relational operators:  when passing filter conditions like "severity:>=>2" 
(without the quotation marks), the terminal shell interprets the 
    < and > characters incorrectly, as input/output redirection operators rather than as a string for the program. 
    To be able to pass the string as intended, I wrapped the condition in quotes.

Phase 2:

Purpose: inter-process communication

Functions generated with AI assistance:
    add: developed the logic within the add function to read a monitor's PID from file and send a SIGUSR1 signal using 
AI assistance.

Modifications/ debugging:
    asynchronous signal safety: I modified AI's initial suggestion of using printf inside the signal handler, using 
write() system call instead. 
    string formatting: I handled the newline character from ctime manually.
    
Learning outcomes (from using AI):
    better understanding of kill and signals.