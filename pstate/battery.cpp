#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
//#include "conf.cpp"
using namespace std;


int query(string target)
{
	string m_targ = target;
	string word;
	string line;
	string junk;
	int value;
	ifstream bat0;
	bat0.open("/proc/pmu/battery_0/state");
	while (bat0 >> word)
	{
		if (word == target)
		{
			bat0 >> junk;
			bat0 >> value;
		}
	}
	bat0.close();
	return value;
}
void help()
{
	cout << "pstate v0.1 -- commandline battery status program for apple powerpc laptops: " << endl;
	cout << "pstate can be called without arguments. In this case it prints all the information it gathers." << endl;
	cout << "It also accepts the following arguments:" << endl;
	cout << "	-t: prints only the time remaining" << endl;
	cout << "	-p: prints only the percentage remaining" << endl;
	cout << "	-h: prints this help message" << endl;
	exit (0);
}

bool line()
{
	int value;
	string junk;
	string word;
	string target = "AC";
	ifstream info;
	info.open("/proc/pmu/info");
	info >> word;
	while (!info.eof())
	{
		info >> word;
		if (word == target)
		{
			info >> junk;
			info >> junk;
			info >> value;
		}
	}
	info.close();
	if (value == 0) return false;
	if (value == 1) return true;
}


int main (int argc, char* argv[])
{
//	string conf;
	int optchr;
	double cur_charge = query("charge");
	double max_charge = query("max_charge");
	int time_sec = query("rem.");
	int time_min = (double)time_sec / 60;
	double perc = (double)cur_charge / max_charge;
	bool ac = line();
	string state, direction;
	if (ac == true) state = "online";
	if (ac == false) state = "offline";
	if (ac == true) direction = " until charged";
	if (ac == false) direction = " until drained";
	if (cur_charge == (double)max_charge) perc = 100;

    	while ((optchr = getopt(argc, argv, "pth")) != EOF) 
	{
		switch (optchr) 
		{
			case 'p':
	    		cout << "pstate: " << perc << "% remaining" << endl;
	    		break;
			case 't':
	    		cout << "pstate: " << time_min << " minutes remaining" << direction << endl;
	    		break;
			case 'h':
			help();
			exit(0);
			default:
			{
				cout << "pstate: " << state << ", " << perc << "% remaining, " << time_min << " minutes remaining" << direction << endl;
			exit (0);
			}
		}
	}
}
