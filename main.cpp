#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <zconf.h>

int main() {
    // NOTE: O_CREATE is undefined behavior when used with O_DIRECTORY
    // NOTE: mkdir will fail if the directory already exits, so ensure test
    // tear downs happen correctly.
    std::string  basePath = "/tmp/wakeup";
    int ret = TEMP_FAILURE_RETRY(mkdir(basePath.c_str(), S_IRWXU));
    if (ret < 0) {
        std::cout << "Failed to create wakeup dir, errno: " << errno << std::endl;
    }
    // NOTE: O_DIRECTORY fails if O_RDWR is specified, works with O_RDONLY :)
    int wakeupFd = TEMP_FAILURE_RETRY(open(basePath.c_str(), O_DIRECTORY | O_RDONLY | O_CLOEXEC));
    if (wakeupFd < 0) {
        std::cout << "Failed to open wakeup dir, errno: " << errno << std::endl;
    }

    // NOTE: mkdir and mkdir at don't return fd's.
    // They return 0 on success and -1 on error.
    std::string fakeKwlPath = "fakeKwl";
    ret = TEMP_FAILURE_RETRY(mkdirat(wakeupFd, fakeKwlPath.c_str(), S_IRWXU));
    if (ret < 0) {
        std::cout << "Failed to create fakeKwl dir, errno: " << errno << std::endl;
    }

    int fakeKwlFd = TEMP_FAILURE_RETRY(openat(wakeupFd, fakeKwlPath.c_str(), O_DIRECTORY | O_RDONLY | O_CLOEXEC));
    if (fakeKwlFd < 0) {
        std::cout << "Failed to open fakeKwl dir, errno: " << errno << std::endl;
    }
    std::string activeCountPath = "active_count";
    std::string wakeupCountPath = "wakeup_count";
    int activeCountFd = TEMP_FAILURE_RETRY(openat(fakeKwlFd, activeCountPath.c_str(), O_CREAT | O_RDWR | O_CLOEXEC));
    if (activeCountFd < 0) {
        std::cout << "Failed to create active count file, errno: " << errno << std::endl;
    }
    int wakeupCountFd = TEMP_FAILURE_RETRY(openat(fakeKwlFd, activeCountPath.c_str(), O_CREAT | O_RDWR | O_CLOEXEC));
    if (wakeupCountFd < 0) {
        std::cout << "Failed to create wakeup count file, errno: " << errno << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}