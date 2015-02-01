#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#define BUFSIZE 8192

#ifdef __GNUC__
// Because why the hell not
void err(char *errstr, char *progname) __attribute__((cold, noreturn));
#endif

void err(char *errstr, char *progname)
{
    fprintf(stderr, "%s: %s\n", progname, errstr);
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc > 1) err("This program takes no arguments", argv[0]);

    uint8_t buf[BUFSIZE];
    bool in_escape = false;
    ssize_t avail;
    while ((avail = read(0, buf, BUFSIZE)) > 0) {
        for (ssize_t i = 0; i < avail; i++) {
            uint8_t chr = buf[i];
            if (in_escape) {
                if (chr == 'n') buf[i] = '\n';
                else if (chr == '0') buf[i] = '\0';
                else if (chr == 't') buf[i] = '\t';
                else if (chr != '\\' && chr != '/') err("Invalid escape sequence", argv[0]);
                in_escape = false;
            } else if (chr == '\\') {
                in_escape = true;
                memmove(buf + i, buf + i + 1, avail - i - 1);
                i--;
                avail--;
            }
        }
        write(1, buf, avail);
    }

    if (in_escape) err("Incomplete escape sequence", argv[0]);
    exit(0);
}
