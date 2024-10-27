#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INPUT_BUFFER_SIZE 100
#define ARGS_ARRAY_SIZE 4
#define OUTPUT_MESSAGE_SIZE 512

const char* help_text = "Available commands:\n"
    "SET PARAM X   - Set PARAM (TEMP, PRESSURE, ALTITUDE) to value X\n"
    "GET PARAM     - Get current value of PARAM (TEMP, PRESSURE, ALTITUDE)\n"
    "HELP          - Display this help message\n"
    "EXIT          - Exit the program\n";
const char* exit_text = "Exit request received. Terminating...\n";

double temperature = 0;
double pressure = 0;
double altitude = 0;

void strupr(char* str) {
    char* c = str;
    while (*c != '\0') {
        *c = toupper((unsigned char) *c);
        c++;
    }
}

int parse_input(char** args, char* str) {
    str[strcspn(str, "\n")] = 0;
    const char* delim = " ";
    char* token = strtok(str, delim);

    if (!token) {
        return -1;
    }

    int i = 0;
    while (token) {
        if (i < ARGS_ARRAY_SIZE - 1) {
            strupr(token);
            args[i] = token;
        } else {
            return -1;
        }
        i++;
        token = strtok(NULL, delim);
    }

    args[i] = NULL; // null terminate the args array for easier error checking

    return 0;
}

int process_command(char* output_message, char** args) {
    char* keyword = args[0];

    if(strcmp(keyword, "SET") == 0) {
        char* param = args[1];
        if (!param) {
            snprintf(output_message, OUTPUT_MESSAGE_SIZE, "No parameter specified for SET.\n"
            "Use HELP to see available parameters.\n");
            return -1;
        }

        char* number = args[2];
        if (!number) {
            snprintf(output_message, OUTPUT_MESSAGE_SIZE, "No value specified for SET.\n"
            "Please provide a correct value.\n");
            return -1;
        }

        char* end;
        double val = strtod(number, &end);
        if (*end != '\0' || end == number) {
            snprintf(output_message, OUTPUT_MESSAGE_SIZE, "Couldn't convert value: %s\n"
            "Please provide a correct value.\n", number);
            return -1;
        }

        if (strcmp(param, "TEMP") == 0) {
            temperature = val;
        } else if (strcmp(param, "PRESSURE") == 0) {
            pressure = val;
        } else if (strcmp(param, "ALTITUDE") == 0) {
            altitude = val;
        } else {
            snprintf(output_message, OUTPUT_MESSAGE_SIZE, "There seems to be no parameter named: %s\n"
            "Use HELP to see available parameters.\n", param);
            return -1;
        }

        snprintf(output_message, OUTPUT_MESSAGE_SIZE, "Parameter %s set to %.4lf\n", param, val);
    } else if(strcmp(keyword, "GET") == 0) {
        double val;
        char* param = args[1];
        if (!param) {
            snprintf(output_message, OUTPUT_MESSAGE_SIZE, "No parameter specified for GET.\n"
            "Use HELP to see available parameters.\n");
            return -1;
        }

        if (strcmp(param, "TEMP") == 0) {
            val = temperature;
        } else if (strcmp(param, "PRESSURE") == 0) {
            val = pressure;
        } else if (strcmp(param, "ALTITUDE") == 0) {
            val = altitude;
        } else {
            snprintf(output_message, OUTPUT_MESSAGE_SIZE, "There seems to be no parameter named: %s\n"
            "Use HELP to see available parameters.\n", param);
            return -1;
        }

        snprintf(output_message, OUTPUT_MESSAGE_SIZE, "Parameter %s is %.4lf\n", param, val);
    } else if(strcmp(keyword, "HELP") == 0) {
        strcpy(output_message, help_text);
        return 0;
    } else if(strcmp(keyword, "EXIT") == 0) {
        strcpy(output_message, exit_text);
        return 1;
    } else {
        snprintf(output_message, OUTPUT_MESSAGE_SIZE, "There seems to be no command named: %s\n"
        "Use HELP to see available commands.\n", keyword);
        return -1;
    }

    return 0;
}

int main(void) {
    char s[INPUT_BUFFER_SIZE];
    char* args[ARGS_ARRAY_SIZE];

    printf("> ");
    while (fgets(s, INPUT_BUFFER_SIZE, stdin)) {
        char message[OUTPUT_MESSAGE_SIZE];

        if (parse_input(args, s) == -1) {
            printf("Malformed command. Use HELP for available commands.\n");
        } else {
            int code = process_command(message, args);

            printf("%s", message);

            if (code == 1) break; // exit received
        }
        printf("> ");
    }

    return 0;
}