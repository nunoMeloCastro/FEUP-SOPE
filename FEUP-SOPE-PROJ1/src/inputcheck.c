#include "inputcheck.h"

void usageNotRight() {
    printf("usage: ./xmod [OPTIONS] MODE FILE/DIR\n");
    printf("   or  ./xmod [OPTIONS] OCTAL-MODE FILE/DIR\n\n");

    // Define the description better
    printf(" OPTIONS: \n");
    printf("       '-v' : verbose mode\n");
    printf("       '-c' : verbose mode only for modifications\n");
    printf("       '-R' : recursivity on the directory\n");
    printf("  If more than one option write them like this for example: -vR\n\n");

    printf(" MODE: <u|g|o|a><-|+|=><rwx> \n");
    printf("       <u|g|o|a> : type of user to affected (only one of the four) \n");
    printf("       <-|+|=> : permissions to be (-) removed, (+) added or (=)\n");
    printf("            replaced (only one of the four)\n");
    printf("       <rwx> : which permissions are going to be modified (one \n");
    printf("            to three - respecting the order)\n");
    printf("         Ex.: 'rw', 'wx', 'w' and 'rx' are accepted but 'xr'\n");
    printf("            and 'wr' are not.\n\n");

    printf(" OCTAL-MODE: \n");
    printf("       A four number sequence, starting with 0 and then 3 digits\n");
    printf("            in octal-base (0 to 7)\n");
    printf("         Ex.: 0056, 0777 and 0175 are accepted but 1000, 075 and 0867 are not.\n\n");

}

int isValidInput(int argc, char *argv[]) {
    // Checks the size of args, the options, the octal_mode number and the order of MODE parameters.
    
    if ((argc < 3) || (argc > 4))
        return 0;

    char *mode = argv[argc - 2];
    char *options = argc == 3 ? NULL : argv[1];

    if (options != NULL) {

        if (options[0] != '-')
            return 0;

        int opt_v = counterChar(options, 'v');
        int opt_c = counterChar(options, 'c');
        int opt_R = counterChar(options, 'R');

        if(opt_v > 1 || opt_c > 1 || opt_R > 1)
            return 0;

        if(opt_v + opt_c + opt_R + 1 != strlen(options))
            return 0;
    }


    // Check if octal mode
    if (isNumber(mode)) {
        // Check if it starts with 0 => no_digits must be 4; otherwise no_digits must be 3
        int no_digits = mode[0] == '0' ? 4 : 3;

        // Checks if there is actually the exepected number of digits
        if(mode[no_digits] != '\0') 
            return 0;

        // Checks if there is really the expected number of digits
        for(int i = 1; i < no_digits; i++) {
            if(mode[i] == '\0') 
                return 0;
        }

        int oct_mode = atoi(mode); 

        while (oct_mode) {
            int digit = oct_mode % 10;
            oct_mode /= 10;

            if (digit > 7) 
                return 0;
        }
    } else {
        const char possible_users[] = {'a', 'u', 'o', 'g'};
        const char possible_operator[] = {'-', '+', '='};
        char user = mode[0];
        char operator = mode[1];
        char *checkPer = getStrPerms(mode);

        // Check user
        if(!findChar(possible_users, 4, user)) 
            return 0;

        // Check separator
        if(!findChar(possible_operator, 3, operator))
            return 0;

        return strstr("rwx", checkPer) || !strcmp(checkPer, "rx");
    }

    return 1;
}