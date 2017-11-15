#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int file_i2c;
int length;
unsigned char buffer[60] = {0};
char buf[1];

const int WAIT_TIME_I2C_US = 2000;
const int WAIT_TIME_LOOP_US = 100000;

struct Position {
	float x;
	float y;
};

enum {
	GET_DIST,
	GET_POS
};

int main()
{
	// I2C
	char *filename = (char *)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)	{		
		printf("Failed to open the i2c bus");
		return EXIT_FAILURE;
	}

	int addr = 0x3; // i2c address of arduino
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		return EXIT_FAILURE;
	}


	// position of robot and values of distance sensors
	struct Position pos;
	unsigned char dist_sens[5];

	while (1)
	{		
		buf[0] = GET_DIST;
		write(file_i2c, buf, 1);
		
		usleep(WAIT_TIME_I2C_US);
		
		length = 5;
		if (read(file_i2c, &dist_sens, length) != length) {			
			printf("Failed to read from the i2c bus.\n");
		} else {
			printf("Distances: [%d %d %d %d %d]\n", dist_sens[0], dist_sens[1], dist_sens[2], dist_sens[3], dist_sens[4]);
		}

		buf[0] = GET_POS;
		write(file_i2c, buf, 1);
		
		usleep(WAIT_TIME_I2C_US);
		
		length = sizeof(pos);
		if (read(file_i2c, &pos, length) != length) {
			printf("Failed to read from the i2c bus.\n");
		} else {
			printf("Position: (%f, %f)\n", pos.x, pos.y);
		}

		usleep(WAIT_TIME_LOOP_US);
	}
}
