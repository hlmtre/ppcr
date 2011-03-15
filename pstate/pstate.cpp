#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
//#include "conf.h"
using namespace std;

#define RUNNING_DIR	"/tmp"
#define LOCK_FILE	"exampled.lock"
#define LOG_FILE	"exampled.log"


char* set() 
{
	FILE *fp;
	string line;
	string conf;
	
	// check if file exists and is read- and write-able
	if ((fp = open("/etc/pstate/conf", "r+")) != NULL)
	{
		if (getline(fp, line) != 0)
		{
			cout << "Config file empty or could not read. Exiting." << endl;
			exit (1);
		}
		if (line == "# autogen = 1" || line == "# autogen = 0")
		{
			  conf = "/etc/pstate/conf";
		}
		else
		{
			// probing for battery info; checks for info in /proc/acpi/battery/BAT0/state, /proc/acpi/battery/BAT1/state and /proc/pmu/battery_0
			FILE *bat;
			if ((bat = open("/proc/acpi/battery/BAT0/state", "r")) != NULL)
			{
				fp << "# autogen = 1" << endl;
				fp << "# loc = acpi" << endl;
				fp << "/proc/acpi/battery/BAT0/state" << endl;
				fp << "/proc/acpi/battery/BAT0/info" << endl;
				conf = "/etc/pstate/conf" << endl;
			}
			else if ((bat = open("/proc/acpi/battery/BAT1/state", "r")) != NULL)
			{
				fp << "# autogen = 1" << endl;
				fp << "# loc = acpi" << endl;
				fp << "/proc/acpi/battery/BAT1/state" << endl;
				fp << "/proc/acpi/battery/BAT1/info" << endl;
				conf = "/etc/pstate/conf" << endl;
			}
			else if ((bat = open("/proc/pmu/battery_0", "r")) != NULL)
			{
				fp << "# autogen = 1" << endl;
				fp << "# loc = pmu" << endl;
				fp << "/proc/pmu/battery_0" << endl;
				conf = "/etc/pstate/conf" << endl;
			}
			else
			{
				cout << "Could not find battery info. You may manually set battery info in ~/.pstate/conf." << endl;
				cout << "There should be two lines in this file: the first line must be \"# autogen = 0\"" << endl;
				cout << "The second line should be the location of the battery info, e.g." << endl;
				cout << "\"/proc/acpi/battery/BAT0/state\"" << endl;
				exit (1);
			}
	}
	return conf;
}

void setVars(string conf, string &state, string &info)
{
//	set variables to proper values depending on pmu or acpi setting
	string autogen, line;
	ifstream file;
	file.open(conf);
	getline(conf, autogen);
	getline(conf, line);
	if (line == "# loc = acpi")
	{
		getline(conf, line);
		ifstream acpi;
		if (!acpi.open(line))
		{
			exit(EXIT_FAILURE);
		}
		else
		{
			state = line;
			info = getline(conf, line);
		}
	}
	if (line == "# loc = pmu")
	{
		getline(conf, line);
		ifstream pmu;
		if (!pmu.open(line))
		{
			exit(EXIT_FAILURE);
		}
		else
		{
			info = getline(conf, line);
			state = NULL;
		}
	}
}

int query(string target, string file)
{

	string word;
	ifstream procfile;
	procfile.open(file);
	while (!procfile.eof())
	{
		cin << word;
		if (word == target)
		{
			cin << word;
			cin << value;
		}
	}
	procfile.close();
	return value;
}

void signal_handler(sig)
int sig;
{
	switch(sig) {
	case SIGHUP:
		log_message(LOG_FILE,"hangup signal caught");
		break;
	case SIGTERM:
		log_message(LOG_FILE,"terminate signal caught");
		exit(0);
		break;
	}
}

void daemonize()
{
	/* start daemon */
	if (getppid() == 1) return;
	i=fork();
	if (i < 0) exit (1);
	if (i > 0) exit (0);
	/* parent exits if fork() successful */
	setsid();
	for (i=getdtablesize();i>=0;--i) close(i); /* close all descriptors */
	i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */
	umask(027); /* set newly created file permissions */
	chdir(RUNNING_DIR); /* change running directory */
	lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
	if (lfp<0) exit(1); /* can not open */
	if (lockf(lfp,F_TLOCK,0)<0) exit(0); /* can not lock */
	write(lfp,str,strlen(str)); /* record pid to lockfile */
	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP,signal_handler); /* catch hangup signal */
	signal(SIGTERM,signal_handler); /* catch kill signal */
}

int main(int argc, char* argv[])
{
	string state, info;
	setVars(set(), state, info);
	ifstream test;
	test.open("/home/hlmtre/test.log");
	test << "I COMPILED" << endl;
	test.close();
}
