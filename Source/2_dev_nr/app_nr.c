#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (void)
{
	int dev = open("/dev/tango_nr", O_RDONLY);
	if (dev == -1)
	{
		printf("open failed \n");
		return -1;
	}

	printf("open success\n");

	close(dev);
}
