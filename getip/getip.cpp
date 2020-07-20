#include "getip.h"

map<string, string> GetIP(map<string, string> &macs)
{
    ifreq networks[IF_NAMESIZE];
    ifconf cfg;
    char mac[32] = {0};
    char *ip = NULL;
    int ret;
#if __APPLE__
    ifaddrs *ifap = new (ifaddrs);
    if (ifap == NULL)
    {
        cout << strerror(errno) << endl;
        goto END;
    }
    int mib[6];
    size_t len;
    ifaddrs *tmp;
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    ret = getifaddrs(&ifap);
    if (ret != 0)
    {
        cout << strerror(errno) << endl;
        goto END;
    }
    tmp = ifap;
    while (ifap->ifa_next)
    {
        struct sockaddr_in *addr = (struct sockaddr_in *)(ifap->ifa_addr);
        if (addr->sin_family == AF_INET)
        {
            char *ip = inet_ntoa(addr->sin_addr);
            mib[5] = if_nametoindex(ifap->ifa_name);
            if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
            {
                printf("sysctl 1 error\n");
                continue;
            }
            char *macbuf = new char[len];
            if (sysctl(mib, 6, macbuf, &len, NULL, 0) < 0)
            {
                cout << strerror(errno) << endl;
                freeifaddrs(tmp);
                goto END;
            }
            if_msghdr *ifm = (if_msghdr *)macbuf;
            struct sockaddr_dl *sdl = (struct sockaddr_dl *)(ifm);
            char *ptr = LLADDR(sdl);
            sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                    (unsigned char)ptr[0],
                    (unsigned char)ptr[1],
                    (unsigned char)ptr[2],
                    (unsigned char)ptr[3],
                    (unsigned char)ptr[4],
                    (unsigned char)ptr[5]);
            macs.insert(make_pair<string, string>(mac, ip));
            delete[] macbuf;
        }

        ifap = ifap->ifa_next;
    }
    freeifaddrs(tmp);
#else
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
#endif
END:
    return macs;
}