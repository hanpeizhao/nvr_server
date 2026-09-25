/* Minimal smoke test for the platform string utilities.
 * Built with -DNVR_BUILD_TESTS=ON, run via ctest.
 */
#include <string>

#include "platform/utils/util.hpp"

int main() {
    /* mystreq / mystrne */
    if (!mystreq("abc", "abc")) { return 1; }
    if (mystreq("abc", "abd")) { return 2; }
    if (!mystrne("abc", "abd")) { return 3; }

    /* mytrim */
    std::string padded = "  hello world  ";
    mytrim(padded);
    if (padded != "hello world") { return 4; }

    /* myunquote */
    std::string quoted = "\"quoted\"";
    myunquote(quoted);
    if (quoted != "quoted") { return 5; }

    /* mtoi / mtob */
    if (mtoi(std::string("42")) != 42) { return 6; }
    if (!mtob(std::string("on"))) { return 7; }

    return 0;
}
