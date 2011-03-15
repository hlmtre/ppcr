#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <perror.h>
using namespace std;

string set() 
{
	FILE *fp;
	string line;
	string conf;
	
	// check if file exists and is read- and write-able
	if ((fp = open("~/.pstate/conf", "r+")) != NULL)
	{
		if (getline(fp, line) != 0)
		{
			cout << "Config file empty or could not read. Exiting." << endl;
			exit (1);
		}
		if (line == "# autogen = 1" || line == "# autogen = 0")
		{
			  conf = "~/.pstate/conf";
		}
		else
		{
			// probing for battery info; checks for info in /proc/acpi/battery/BAT0/state, /proc/acpi/battery/BAT1/state and /proc/pmu/battery_0
			FILE *bat;
			if ((fp = open("/proc/acpi/battery/BAT0/state", "r")) != NULL)
			{
				fp << "# autogen = 1" << endl;
				fp << "# loc = acpi" << endl;
				fp << "/proc/acpi/battery/BAT0/state" << endl;
				fp << "/proc/acpi/battery/BAT0/info" << endl;
				conf = "/etc/pstate/conf" << endl;
			}
			else if ((fp = open("/proc/acpi/battery/BAT1/state", "r")) != NULL)
			{
				fp << "# autogen = 1" << endl;
				fp << "# loc = acpi" << endl;
				fp << "/proc/acpi/battery/BAT1/state" << endl;
				fp << "/proc/acpi/battery/BAT1/info" << endl;
				conf = "/etc/pstate/conf" << endl;
			}
			else if ((fp = open("/proc/pmu/battery_0", "r")) != NULL)
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
