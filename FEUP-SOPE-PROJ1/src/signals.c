#include "signals.h"

char *process_path;
extern unsigned int nftot;
extern unsigned int nfmod;
extern const char * const sys_siglist[];

void setUpSignals() {
	signal(SIGINT, ctrlcReceived); // handle ctrlc
    signal(SIGUSR1, registerAndIgnore); 
    signal(SIGUSR2, questionPrompt); // confirm exit
    signal(SIGTERM, terminate);

    // ignore other main signals, and be able to regist them
    signal(SIGHUP, registerAndIgnore);
    signal(SIGQUIT, registerAndIgnore);
    signal(SIGSEGV, registerAndIgnore);
    signal(SIGPIPE, registerAndIgnore);
    signal(SIGALRM, registerAndIgnore);
    signal(SIGCHLD, registerAndIgnore);
}

char *getSignalName(int sig) {
    char *str = strdup(sys_siglist[sig]);
    if (!str) return NULL;
    upperCase(str);
    
    char *signalStr = (char*)malloc(MAX_STR_LEN);
    sprintf(signalStr, "SIG%s", str);

    return signalStr;
}

void registerAndIgnore(int sig) {
    signal(sig, registerAndIgnore);

    //register on log the received signal
    writeToLogger(getpid(), SIGNAL_RECV, getSignalName(sig));
}

void ctrlcReceived(int sig) {
	signal(SIGINT, ctrlcReceived); // reset

	// printf("\nParent received signal %u\n", sig); // to test purposes
	writeToLogger(getpid(), SIGNAL_RECV, STR_SIGINT); //received SIGINT

	int status;

	if (0 == waitpid(-1, &status, WNOHANG)) { // unterminated children exist
		// Send SIGUSR1 to every process with the same groupId as the sender (himself + descendents)
		printf("\n");
		if (kill(0, SIGUSR1) != 0) 
			perror(STR_SIGUSR1);
		writeToLogger(getpid(), SIGNAL_SENT, getGroupInfoSig(STR_SIGUSR1, getpgrp()));
	} else {
		// Send SIGUSR2 to himself
		kill(getpid(), SIGUSR2);
		writeToLogger(getpid(), SIGNAL_SENT, getInfoSig(STR_SIGUSR2, getpid()));
	}
}

void displayInfo(int sig) {
	signal(SIGUSR1, displayInfo); // reset

	writeToLogger(getpid(), SIGNAL_RECV, STR_SIGUSR1); //received SIGUSR1

	// Display current process info
	printf("%u ; %s ; %u ; %u\n", getpid(), process_path, nftot, nfmod);

	// Send SIGUSR2 to the parent - display finished
	if (kill(getppid(), SIGUSR2) != 0)
		perror(STR_SIGUSR2);
	writeToLogger(getpid(), SIGNAL_SENT, getInfoSig(STR_SIGUSR2, getppid()));

	writeToLogger(getpid(), SIGNAL_SENT, getInfoSig(STR_SIGSTOP, getpid()));
	// Send SIGSTOP to himself
	if (kill(getpid(), SIGSTOP) != 0)
		perror(STR_SIGSTOP); 

}

void questionPrompt(int sig) {
	signal(SIGUSR2, questionPrompt); // reset

	writeToLogger(getpid(), SIGNAL_RECV, STR_SIGUSR2); //received SIGUSR2

	// Display principal process info
	printf("%u ; %s ; %u ; %u\n", getpid(), process_path, nftot, nfmod);

	// Confirm exit prompt
	printf("\nDo you really want to terminate the program (y/n)? ");
	char answer = readAnswer();

	// Continue all processes including himself
	if (kill(0, SIGCONT) != 0) 
		perror(STR_SIGCONT);
	writeToLogger(getpid(), SIGNAL_SENT, getGroupInfoSig(STR_SIGCONT, getpgrp()));

	if (answer == 'y') { 
		writeToLogger(getpid(), SIGNAL_SENT, getGroupInfoSig(STR_SIGTERM, getpgrp()));
		// Terminate all processes including himself
		if (kill(0, SIGTERM) != 0) 
			perror(STR_SIGTERM);
	} 
}

void terminate(int sig) {
	// to registrate the signal
	//printf("Termitating Process PID %u!!...\n", getpid());  // to test purposes

	writeToLogger(getpid(), SIGNAL_RECV, STR_SIGTERM); //received SIGTERM

	exit(EXIT_CTRLC);
}

