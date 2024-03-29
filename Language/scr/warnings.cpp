#include <stdio.h>
#include <stdlib.h>
#include "warnings.h"

//#ifndef WARNINGS_H/
//#define WARNINGS_H

void warning(const char* information, struct call_of_dump arguments_of_call) {
    printf("INFO %s\n", information);
    system("echo \e[31m-----------------!WARNING!----------------\e[0m");
    char warning_info[SIZE_OF_WARNINGS] = "";
    sprintf(warning_info, "echo \"\\e[31mIN FILE %s (FUNCTION %s, LINE %lu): %s\\e[0m\"", arguments_of_call.name_file, arguments_of_call.name_function, arguments_of_call.number_of_line, information);
    system(warning_info);
}

///#endif
