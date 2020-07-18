#include "getip/getip.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    map<string, string> macs;
    GetIP(macs);
    for (auto mac : macs)
    {
        cout << mac.first << " : " << mac.second << endl;
    }
    return 0;
}
