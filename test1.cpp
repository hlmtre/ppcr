#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;

int main()
{
/*	string core0 = "/sys/devices/platform/coretemp.0/temp1.input";
	int temp0;
	fstream input; // open coretemps for reading
	input.open(core0.c_str(), ios::in);
	input >> temp0;
	cout << temp0 << endl;*/
string path = "/sys/devices/platform/";
string core0 = path + "coretemp.0/temp1_input";
string core1 = path + "coretemp.1/temp1_input";
string fan0 = path + "applesmc.768/fan1_min";
string fan1 = path + "applesmc.768/fan2_min";
string test = "/home/hlmtre/repos/code/out";
int speed0, speed1, temp0, temp1, target;

	target = 30000;
	fstream coretemp0; // open coretemps for reading
	coretemp0.open(core0.c_str(), ios::in);
	fstream coretemp1;
	coretemp1.open(core1.c_str(), ios::in);
	coretemp0 >> temp0;
	coretemp1 >> temp1;

	fstream fanspeed0;
	fanspeed0.open(test.c_str(), ios::out);
	while (temp0 > target) 
	{	
		speed0 += 100;
		fanspeed0 << speed0;
		usleep(10000000);
		cout << "Wrote " << speed0 << " to outfile" << endl;
	}

}
