#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

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
    const int buffer_size = 100;

    char* time_string = (char*)malloc(sizeof(char) * buffer_size);

    // Get the current time
    time(&current_time);

    // Convert the current time to a struct tm using localtime_s
    if (localtime_s(&timeinfo, &current_time) != 0) 
    {
        strcpy_s(time_string, buffer_size, "Failed to get the current time\n");
        return time_string;
    }

    // Format the time as a string
    strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S\n", &timeinfo);
    
    return time_string;
}

