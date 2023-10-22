#include <stdio.h>
#include "utils.h"
#include <string.h>
#include <time.h>

float scale(float value, int scale)
{
	return (value * scale);
}

void write_to_log(const char* file, const char *buffer)
{
    char time[80];
	strcpy_s(time, 80, get_current_time());

	FILE* log_file;
	fopen_s(&log_file, file, "w");
	if (log_file != NULL)
	{
		fputs(time, log_file);
		fputs(buffer, log_file);
		fclose(log_file);
	}
}

char* get_current_time()
{
    time_t current_time;
    struct tm timeinfo;

    char time_string[80]; // Adjust the buffer size as needed

    // Get the current time
    time(&current_time);

    // Convert the current time to a struct tm using localtime_s
    if (localtime_s(&timeinfo, &current_time) != 0) {
        printf("Failed to get the current time.\n");
        return 1;
    }

    // Format the time as a string
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S\n", &timeinfo);
    
    return time_string;
}

