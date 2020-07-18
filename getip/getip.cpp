#include "getip.h"

map<string, string> GetIP(map<string, string> &macs)
{
    ifreq networks[IF_NAMESIZE];
    ifconf cfg;
    char mac[32] = {0};
    char *ip = NULL;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        return macs;

    cfg.ifc_len = sizeof(networks);
    cfg.ifc_ifcu.ifcu_buf = (caddr_t)networks;
    if (!ioctl(fd, SIOCGIFCONF, (char *)&cfg))
    {
        int count = cfg.ifc_len / sizeof(ifreq);
        while (count-- > 0)
        {
            if (!ioctl(fd, SIOCGIFADDR, (char *)&networks[count]))
            {
                ip = inet_ntoa(((struct sockaddr_in *)(&networks[count].ifr_ifru.ifru_addr))->sin_addr);
            }
            if (!ioctl(fd, SIOCGIFHWADDR, (char *)&networks[count]))
            {
                sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                        (unsigned char)networks[count].ifr_ifru.ifru_hwaddr.sa_data[0],
                        (unsigned char)networks[count].ifr_ifru.ifru_hwaddr.sa_data[1],
                        (unsigned char)networks[count].ifr_ifru.ifru_hwaddr.sa_data[2],
                        (unsigned char)networks[count].ifr_ifru.ifru_hwaddr.sa_data[3],
                        (unsigned char)networks[count].ifr_ifru.ifru_hwaddr.sa_data[4],
                        (unsigned char)networks[count].ifr_ifru.ifru_hwaddr.sa_data[5]);
            }

            macs.insert(make_pair<string, string>(mac, ip));
        }
    }
    else
    {
        cout << "errno: " << errno << endl;
    }

    close(fd);
    return macs;
}