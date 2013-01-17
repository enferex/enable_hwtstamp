#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/net_tstamp.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <sys/socket.h>


/* Enable hardware timestamping on all packets received */
static void set_settings(int sd, const char *iface)
{
	struct ifreq dev;
	struct hwtstamp_config cfg;

    memset(&dev, 0, sizeof(struct ifreq));
    memset(&cfg, 0, sizeof(struct hwtstamp_config));

	cfg.rx_filter = HWTSTAMP_FILTER_ALL;
	cfg.tx_type = HWTSTAMP_TX_OFF;

	strncpy(dev.ifr_name, iface, sizeof(dev.ifr_name));
	dev.ifr_data = (void *)&cfg;
	
	if (ioctl(sd, SIOCSHWTSTAMP, &dev) < 0)
	{
		perror("ioctl()");
		exit(EXIT_FAILURE);
	}

	printf("Settings:\n"
		   "    HWTSTAMP_TX: %d\n"
		   "    HWTSTAMP_RX: %d\n"
		   "    Flags:       %d\n",
		   cfg.tx_type,
		   cfg.rx_filter,
		   cfg.flags);
}


static void usage(const char *execname)
{
    printf("Usage: %s <interface>\n", execname);
    exit(EXIT_SUCCESS);
}


int main(int argc, char **argv)
{
	int sd;
    const char *iface = NULL;

    if (argc != 2)
      usage(argv[0]);
    else
      iface = argv[1];
    printf("Using interface: %s\n", iface);
	
	if ((sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
	    perror("socket()");
		exit(EXIT_FAILURE);
	}

	set_settings(sd, iface);
	close(sd);

	return 0;
}
