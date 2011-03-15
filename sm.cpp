#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

int get_heat0()
{
	int temp0;
	ifstream coretemp0;
	coretemp0.open("/sys/devices/platform/coretemp.0/temp1_input");
	coretemp0 >> temp0;
	return temp0;
}

int get_heat1()
{
	int temp1;
	ifstream coretemp1;
	coretemp1.open("/sys/devices/platform/coretemp.1/temp1_input");
	coretemp1 >> temp1;
	return temp1;
}

void set_fans(int fanspeed0)
{
	int fanspeed = fanspeed0;
	ofstream outfile;
	outfile.open("/sys/devices/platform/applesmc.768/fan1_min");
	outfile << fanspeed;
	outfile.close();

	ofstream outfile1;
	outfile1.open("/sys/devices/platform/applesmc.768/fan2_min");
	outfile1 << fanspeed;
	outfile1.close();
	return;
}


int main()
{
	int target = 45000;
	int fanspeed0, fanspeed1;
	int speedMax = 6000;
	int speedMin = 2500;
	int temp0, temp1;

	while (true) 
	{ 
		temp0 = get_heat0(); 
		temp1 = get_heat1();
		while (temp0 < target && temp1 < target) 
		{
			cout << "SPEEDMIN" << endl;
			temp0 = get_heat0();
			temp1 = get_heat1();
			fanspeed0 = speedMin;
			fanspeed1 = speedMin;
			set_fans(fanspeed0);
			set_fans(fanspeed1);
			sleep(1);
			continue;
		}
		while (temp0 > target) 
		{ 
			cout << "TEMP0" << endl;
			temp0 = get_heat0(); 
			fanspeed0 += 500; 
			if (fanspeed0 > speedMax) fanspeed0 = speedMax;
	                set_fans(fanspeed0);
			sleep(1);
//			continue;
			main();
		}
		/*while (temp0 < target)
		{
			temp0 = get_heat0(); 
			fanspeed0 -= 500;
			if (fanspeed0 < speedMin) fanspeed0 = speedMin;
			set_fans(fanspeed0);
			sleep(1);
		}*/
		while (temp1 > target)
		{
			cout << "TEMP1" << endl;
			temp1 = get_heat1();
			fanspeed1 += 500;
			if (fanspeed1 > speedMax) fanspeed1 = speedMax;
			set_fans(fanspeed1);
			sleep(1);
//			continue;
			main();
		}
		/*while (temp1 < target) 
		{
			temp1 = get_heat1();
			fanspeed1 -=500;
			if (fanspeed1 < speedMin) fanspeed1 = speedMin;
			set_fans(fanspeed1);
			sleep(1);
		}*/
	}
}

