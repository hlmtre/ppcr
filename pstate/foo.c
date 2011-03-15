#include <stdio.h>

main()
{
	int night = 1;
	int today = 0;
	int tomorrow = 1;
	if (night && (today || tomorrow)) printf("true\n");
	return 0;
}
