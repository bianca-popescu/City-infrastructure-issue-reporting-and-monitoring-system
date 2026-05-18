// compile
gcc -Wall -o city_manager main.c city_manager.c
gcc -Wall -o scorer scorer.c
gcc -Wall -o monitor_reports monitor_reports.c

// phase 1
./city_manager --role manager --user bianca --add district1
new report:
report ID: 1
GPS coordinates: 1 2
issue category: a
severity level (1-3): 2
description: abc
report 1 added to district1

./city_manager --role manager --user bianca --add district1
new report:
report ID: 2
GPS coordinates: 2 3
issue category: b
severity level (1-3): 3
description: def
report 2 added to district1

./city_manager --role manager --list district1
file permissions: rw-rw-r--
size: 3120 bytes
last modification time: Tue Apr 28 14:26:24 2026

ID: 1
issue category: a
severity level: 2
inspector name: bianca
ID: 2
issue category: b
severity level: 3
inspector name: bianca

// phase 2
./city_manager --role inspector --remove_district district1
acces denied: role manager can't remove districts

./city_manager --role manager --remove_district district1
symlink district1 removed
directory district1 deleteted successfully

// phase 3
