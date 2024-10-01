#include <sdbus-c++/sdbus-c++.h>
#include <unistd.h>
#include <string>

std::string getObjectPath(pid_t pid) {
    char path[32];
    char buf[512];
    sprintf(path, "/proc/%d/exe", pid);
    int res = readlink(path, buf, 512);
    buf[res] = 0;
    return buf;
}