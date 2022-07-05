#include "xmod.h"

unsigned int nftot = 0;
unsigned int nfmod = 0;
extern mode_t new_perm, old_perm;

int xmod(const char *options, const char *mode, const char *path_name){
	mode_t new_perms = getNewPerms(mode, path_name);
	new_perm = new_perms & GET_MODE;

	if (handleOptions(options, path_name, new_perms)) exit(1);

	nftot++; // new file/dir found

	if (chmod(path_name, new_perms) == -1) {
		perror("chmod()");
		exit(1);
	}

	return 0;
}

int handleOptions(const char *options, const char *path_name, const mode_t new_perms){
	if (options == NULL) return 0;

	struct stat st;
	if (stat(path_name, &st) == -1) {
		perror("stat()");
		exit(1);
	}

	mode_t perms_initial = st.st_mode & GET_MODE;
	mode_t perms_final = new_perms & GET_MODE;

	int is_change = perms_initial == perms_final? 0 : 1;
	int opt_v = strchr(options, 'v') == NULL ? 0 : 1;
	int opt_c = strchr(options, 'c') == NULL ? 0 : 1;

	char *str_perms_initial = convertPermsToString(perms_initial);

	// increment variable only if a permission is modified and writes to logger
	if (is_change) {
		nfmod++;
		writeToLogger(getpid(), FILE_MODF, getInfoFModf(path_name));
	}
	
	if (is_change && (opt_v || opt_c)) {
		char *str_perms_final = convertPermsToString(perms_final);
		printf("mode of '%s' changed from 0%o (%s) to 0%o (%s)\n", path_name, perms_initial, str_perms_initial, perms_final, str_perms_final);
	}
	else if (!is_change && opt_v && !opt_c)
		printf("mode of '%s' retained as 0%o (%s)\n", path_name, perms_initial, str_perms_initial);

	return 0;
}

char *getStrPerms(const char* mode){
	char *perms = (char*)malloc(MAX_STR_LEN);
	strcpy(perms, &mode[2]);
	return perms;
}

mode_t getNewPerms(const char *mode, const char *path_name){
	struct stat st;
	if (stat(path_name, &st) == -1) {
		perror("stat()");
		exit(1);
	}

	// Save old permissions to logger
	old_perm = st.st_mode & GET_MODE;

	// if octal-mode
	if (isNumber(mode)) {
		char *ptr;
		return strtol(mode, &ptr, 8) | (st.st_mode & RESET_MODE);
	}
	
	// if normal mode
	char user = mode[0]; // user: <u|g|o|a>
	int all = user == 'a' ? 1 : 0;

	char operator = mode[1]; // operator: <+|-|=>
	int remove = operator == '-' ? 1 : 0;

	char *str_perms = getStrPerms(mode); // permissions: [rwx]
	int read = strchr(str_perms, 'r') == NULL ? 0 : 1;
	int write = strchr(str_perms, 'w') == NULL? 0 : 1;
	int execute = strchr(str_perms, 'x') == NULL? 0 : 1;

	mode_t perms = operator == '=' ? resetModeUser(st.st_mode, user) : st.st_mode;

	if (read) {
		if (user == 'o' || all) perms = remove ? perms & ~S_IROTH : perms | S_IROTH;
		if (user == 'g' || all) perms = remove ? perms & ~S_IRGRP : perms | S_IRGRP;
		if (user == 'u' || all) perms = remove ? perms & ~S_IRUSR : perms | S_IRUSR;
	}

	if (write) {
		if (user == 'o' || all) perms = remove ? perms & ~S_IWOTH : perms | S_IWOTH;
		if (user == 'g' || all) perms = remove ? perms & ~S_IWGRP : perms | S_IWGRP;
		if (user == 'u' || all) perms = remove ? perms & ~S_IWUSR : perms | S_IWUSR;
	}

	if (execute) {
		if (user == 'o' || all) perms = remove ? perms & ~S_IXOTH : perms | S_IXOTH;
		if (user == 'g' || all) perms = remove ? perms & ~S_IXGRP : perms | S_IXGRP;
		if (user == 'u' || all) perms = remove ? perms & ~S_IXUSR : perms | S_IXUSR;
	}
	return perms;
}

mode_t resetModeUser(const mode_t current_mode, const char user){
    if(user == 'u') return current_mode & RESET_MODE_USR;
    else if(user == 'g') return current_mode & RESET_MODE_GRP;
    else if(user == 'o') return current_mode & RESET_MODE_OTH;
    return current_mode & RESET_MODE_ALL;
}
