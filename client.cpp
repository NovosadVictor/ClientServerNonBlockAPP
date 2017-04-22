//#include "conn.hpp"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <cinttypes>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

#include <vector>

static void repl(int fd);

int main(int argc, char *argv[]) {
    const char        *host = "127.0.0.1";
    uint16_t           port = 8000;
    struct sockaddr_in addr;
    int                fd;

    if(argc != 1 && argc != 3) {
        fprintf(stderr, "Usage: %s [<ip> <port>]\n", argv[0]);
        return -1;
    }

    if(argc == 3) {
        host = argv[1];
	port = atoi(argv[2]);
   //     if(toUInt16(argv[2], &port) == -1) {
     //       fprintf(stderr, "Wrong parametr <port>\n");
      //      return -1;
       // }
    }

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    if(inet_pton(AF_INET, host, &addr.sin_addr) < 1) {
        fprintf(stderr, "     - Host error.\n");
        return -1;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        fprintf(stderr, "     - Socket error.\n");
        return -1;
    }

    if(connect(fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
        fprintf(stderr, "%4d - Connect error.\n", fd);
        if(close(fd))
            fprintf(stderr, "%4d - Close error.\n", fd);
        return -1;
    }

    repl(fd);

    if(shutdown(fd, 2) == -1) {
        fprintf(stderr, "%4d - Shutdown error.\n", fd);
        if(close(fd))
            fprintf(stderr, "%4d - Close error.\n", fd);
        return -1;
    }

    if(close(fd)) {
        fprintf(stderr, "%4d - Close error.\n", fd);
        return -1;
    }

    return 0;
}

#define BUFLEN 50

static bool readLine(std::vector<char> &l) {
    char   buf[BUFLEN];    
    size_t len;

    l.clear();    

    for(;;) {
        if(!fgets(buf, sizeof buf, stdin))
            return false;

        len = strlen(buf);
       
        l.insert(l.end(), buf, buf + len); 

        if(buf[len - 1] == '\n') {
            l.push_back('\0');
            break;
        } 
    }

    return true;
}

static bool writeAll(int fd, const char *buf, size_t len);
static bool readAll(int fd, char *buf, size_t len);
static bool readLine(std::vector<char> &l);

static void repl(int fd) {
    for(;;) {
        printf("Write your request:\n>>> ");
   
        std::vector<char> l;
        if(!readLine(l))
            return;

        try {
            uint32_t len = strlen(&l[0]) + 1;

            if (!writeAll(fd, (const char *) &len, sizeof len))
                throw "     - Write(length) error.";

            if (!writeAll(fd, &l[0], len))
                throw "     - Write(text) error.";

            for (;;) {

                if (!readAll(fd, (char *) &len, sizeof len))
                    throw "     - Read(length) error.";

                std::vector<char> buf(len, 0);

                if (!readAll(fd, &buf[0], len))
                    throw "     - Read(text) error.";

                puts(&buf[0]);

                int status = atoi(&buf[20]);

                if (status == -1)
                    break;

                int quantity;

                if (status == 3 || status == 4) {
                    buf.clear();

                    if (!readAll(fd, (char *) &len, sizeof len))
                        throw "     - Read(length) error.";

                    std::vector<char> buf(len, 0);

                    if (!readAll(fd, &buf[0], len))
                        throw "     - Read(text) error.";
                    quantity = atoi(&buf[0]);

                    puts(&buf[7]);
                    printf("    Total: %d\n", quantity);

                    break;
                }

                if (status == 1) {
                    buf.clear();

                    if (!readAll(fd, (char *) &len, sizeof len))
                        throw "     - Read(length) error.";

                    std::vector<char> buf(len, 0);

                    if (!readAll(fd, &buf[0], len))
                        throw "     - Read(text) error.";

                    quantity = atoi(&buf[15]);

                    for (size_t i = 0; i < quantity; ++i) {
                        buf.clear();

                        if (!readAll(fd, (char *) &len, sizeof len))
                            throw "     - Read(length) error.";

                        std::vector<char> buf(len, 0);

                        if (!readAll(fd, &buf[0], len))
                            throw "     - Read(text) error.";

                        puts(&buf[0]);
                    }
                    printf("    Total: %d\n", quantity);

                    break;
                }

                if (status > 1) {
                    buf.clear();

                    if (!readAll(fd, (char *) &len, sizeof len))
                        throw "     - Read(length) error.";

                    std::vector<char> buf(len, 0);

                    if (!readAll(fd, &buf[0], len))
                        throw "     - Read(text) error.";

                    puts(&buf[0]);

                    break;
                }
            }
        }
        catch(const char *err) {
            fprintf(stderr, "%s\n", err);
        }
        catch(std::bad_alloc) {
            fprintf(stderr, "Memory allocation error\n");
        }
    }
}

#define LINESIZE 70

static bool writeAll(int fd, const char *buf, size_t len) {
    for(size_t cur = 0; len - cur;) {
        ssize_t s = write(fd, buf + cur, len - cur);
        if(s > 0) {
      //      fprintf(stderr, "     - Write %d bytes\n", (int)s);
            cur += s;
        }
        else
            return false;
    }
    return true;
}

static bool readAll(int fd, char *buf, size_t len) {
    for(size_t cur = 0; len - cur;) {
        ssize_t s = read(fd, buf + cur, len - cur);
        if(s > 0) {
       //     fprintf(stderr, "     - Read %d bytes\n", (int)s);
            cur += s;
        } 
        else
            return false;
    }
    return true;
}

