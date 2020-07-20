#pragma once

#include <iostream>
#include <map>
#include <string>

using namespace std;

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#if __APPLE__
#include <net/if_dl.h>
#include <sys/sysctl.h>
#endif
#include <string.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

map<string, string> GetIP(map<string, string> &macs);