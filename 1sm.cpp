#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

bool log = true;
int get_speed()
{
	int speed, speed1;
	ifstream fan0;
	fan0.open("/sys/devices/platform/applesmc.768/fan1_output");
	fan0 >> speed;

	ifstream fan1;
	fan1.open("/sys/devices/platform/applesmc.768/fan2_output");
	fan1 >> speed1;
	return ((speed + speed1) / 2);
}

void set_fans(int speed)
{
	int m_speed = speed;

	ofstream outfile;
	outfile.open("/sys/devices/platform/applesmc.768/fan1_min");
	outfile << m_speed;
	outfile.close();

	ofstream outfile1;
	outfile1.open("/sys/devices/platform/applesmc.768/fan2_min");
	outfile1 << m_speed;
	outfile1.close();
}

bool line()
{
	string state;
	ifstream adapter;
	adapter.open("/proc/acpi/ac_adapter/ADP1/state");
	getline(adapter, state);
	adapter.close();
	if (state == "state:                   on-line") return true;
	return false;
}

double check()
{
	int temp0, temp1;
	ifstream coretemp0;
	coretemp0.open("/sys/devices/platform/coretemp.0/temp1_input");
	coretemp0 >> temp0;

	ifstream coretemp1;
	coretemp1.open("/sys/devices/platform/coretemp.1/temp1_input");
	coretemp1 >> temp1;

	return ((temp0 + temp1) / 2);
}

void hawg()
{
	int i;
	int minSpeed = 2000;
	int maxSpeed = 6000;
	int offTemp = 30000;
	int aveTemp = 40000;
	int maxTemp = 65000;
	ofstream log;
	log.open("/var/log/smcpp.log");
	if (!line())
	{
		minSpeed = 2000;
		maxTemp = 70000;
	}
	while (1)
	{
		double temp = check();
		double ratio = (temp / aveTemp);
		int curSpeed = get_speed();
		double endspeed = (ratio*minSpeed);
		double distance = (temp / 10000);
		double distance2 = (aveTemp / 1000);
		double total = (distance - distance2);
		int speed = ((endspeed) + minSpeed);
		if (temp > maxTemp) speed = maxSpeed;
		if (temp < offTemp) speed = minSpeed;
		if (speed > maxSpeed) speed = maxSpeed;
		if (log)
		{
			if (i % 5 == 0)
			{
				time_t t = time(0);
				log << ctime(&t) << endl;
				if (line())
				{
					log << "Online" << endl;
				}
				else log << "Offline" << endl;
				log << "ratio: " << ratio << endl;
				log << "endspeed: " << endspeed << endl;
				log << "temp: " << temp << endl;
				log << "speed: " << speed << endl;
			}
		}
		i++;
		set_fans(speed);
		sleep(1);
	}
}
int main()
{
	hawg();
}
