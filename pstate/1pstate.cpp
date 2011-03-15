#include <string>
#include <fstream>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

#define DAEMON_NAME "cppmd"
#define PID_FILE "/var/run/cppmd.pid"
#define LOCKFILE "/var/lock/cppmd"
#define	SOCK_PATH "/tmp/cppmd_sock"

/*
	TODO
	add functionality to check if on/off battery power and to not be really annoying and hijack the terminal incessantly
	when battery power below 15%
*/
// set empty global variables for battery info file locations
string info, state, conf, arch, test;

bool debug = true;

int linepos()
{
	FILE* lock;
	if ((lock = fopen(LOCKFILE, "r")) != NULL)
	{
		return 0; // we are first in line; return 0
	}
	else return 1;
}

void set()
{
	FILE* m_conf;
	if ((m_conf = fopen("/etc/pstate/conf", "r+")) != NULL)
	{
		conf = "/etc/pstate/conf";
	}

	FILE* m_state;
	if ((m_state = fopen("/proc/acpi/battery/BAT0/state", "r")) != NULL)
	{
		state = "/proc/acpi/battery/BAT0/state";
		arch = "acpi";
	}
	else if ((m_state = fopen("/proc/acpi/battery/BAT1/state", "r")) != NULL)
	{
		state = "/proc/acpi/battery/BAT1/state";
		arch = "acpi";
	}
	else if ((m_state = fopen("/proc/pmu/battery_0", "r")) != NULL)
	{
		state = "/proc/pmu/battery_0";
		arch = "pmu";
	}
	else
	{
		state = "NULL";
	}

	FILE* m_info;
	if ((m_info = fopen("/proc/acpi/battery/BAT1/info", "r")) != NULL)
	{
		info = "/proc/acpi/battery/BAT1/info";
	}
	else if ((m_info = fopen("/proc/acpi/battery/BAT0/info", "r")) != NULL)
	{
		info = "/proc/acpi/battery/BAT0/info";
	}
	else if ((m_info = fopen("/proc/pmu/battery_0", "r")) != NULL)
	{
		info = "/proc/pmu/battery_0";
	}
	else 
	{
		info = "NULL";
	}

}

void signal_handler(int sig)
{
	switch(sig)
	{
		case SIGHUP:
			syslog(LOG_WARNING, "received SIGHUP");
			exit (1);
		case SIGTERM:
			syslog(LOG_WARNING, "received SIGTERM");
			exit (1);
		case SIGKILL:
			syslog(LOG_WARNING, "received SIGKILL");
			exit (1);
		default:
//			syslog(LOG_WARNING, "Unhandled signal (%d) %s", strsignal(sig));
			break;
	}
}

/* 
	'query(string, string)' function returns value if it can read it, -1 if error
*/

int query(string file, string target)
{
	string word;
	int value;
	ifstream qfile;
	qfile.open(file.c_str());
	if (qfile.is_open())
	{
		while (!qfile.eof())
		{
			qfile >> word;
			test = word;
			if (word == target)
			{
				if (target == "remaining" || target == "full") qfile >> word;
				qfile >> value;
				return value;
			}
		}
	}
	else return -1;
}

double calc(int ivar, int svar)
{
	return ((double)svar / ivar);
}

int main(int argc, char** argv)
{
	set();

	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

//	syslog(LOG_INFO, "%s: daemon starting up", DAEMON_NAME);

	setlogmask(LOG_UPTO(LOG_DEBUG));
	openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

	pid_t pid, sid;

	syslog(LOG_INFO, "%s: starting daemon...", DAEMON_NAME);
	pid = fork();
	if (pid < 0) exit (EXIT_FAILURE);
	if (pid > 0) exit (EXIT_SUCCESS);
	umask(0);
	sid = setsid();
	if (sid < 0) exit(EXIT_FAILURE);
	if ((chdir("/")) < 0) exit(EXIT_FAILURE);

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	int ivar, svar;
	while (1)
	{
		if (arch == "acpi")
		{
			ivar = query(info.c_str(), "full"); 
			svar = query(state.c_str(), "remaining");
		}
		if (arch == "pmu")
		{
			ivar = query(info.c_str(), "charge");
			svar = query(info.c_str(), "max_charge");
		
		}
		else ivar = svar = -1;
		if (ivar == -1 || svar == -1)
		{
			syslog(LOG_INFO, "FATAL: could not read state or info; exiting...");
			exit(1);
		}
		double perc = calc(ivar, svar);
		double low_point = .15;
		if (debug)
		{
			syslog(LOG_INFO, "-----------------"); 
			syslog(LOG_INFO, "ivar is: %i", ivar); 
			syslog(LOG_INFO, "svar is: %i", svar);
			syslog(LOG_INFO, "perc is: %f", perc);
			syslog(LOG_INFO, "state is: %s", state.c_str()); 
		}
		if (perc < low_point) syslog(LOG_EMERG, "battery charge left: %f", perc);

//		int first = linepos();
		if (debug) sleep(30);
//		else if (first) // server listen for incoming readers on UNIX socket and reply
//		{
			
	}
}
