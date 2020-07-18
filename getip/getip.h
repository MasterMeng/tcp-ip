#pragma once

#include <iostream>
#include <map>
#include <string>

using namespace std;

#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

map<string, string> GetIP(map<string, string> &macs);