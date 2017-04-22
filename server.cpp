#include "conn.hpp"

static int signalIgnoring(void);

#define NC 10
static void loop(int ld, size_t nc, int file_desc);

int main(int argc, char *argv[]) {
    const char        *host = 0;
    uint16_t           port = 8000;
    struct sockaddr_in addr;
    int                ld   = -1;
    int                on;
    int                st   = 0;
    int file_desc = open("database.db", O_RDONLY);
    if (file_desc == -1) {
        printf("ERROR");
        return -1;
    }

    try {
        if(argc != 1 && argc != 3) {
            fprintf(stderr, "Usage: %s [<ip> <port>]\n", argv[0]);
            throw -1;
        }

        if(argc == 3) {
            host = argv[1];
            if(toUInt16(argv[2], &port) == -1) {
                fprintf(stderr, "Wrong parametr <port>\n");
                throw -1;
            }
        }

        if(signalIgnoring() != 0)
            throw -1;

        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port);
        if(host) {
            if(inet_pton(AF_INET, host, &addr.sin_addr) < 1) {
                fprintf(stderr, "     - Host error.\n");
                throw -1;
            }
        }
        else
            addr.sin_addr.s_addr = INADDR_ANY;

        ld = socket(AF_INET, SOCK_STREAM, 0);
        if(ld == -1) {
            fprintf(stderr, "     - Socket error.\n");
            throw -1;
        }

        on = 1;
        if(setsockopt(ld, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on) == -1) {
            fprintf(stderr, "%4d - Setsockopt error.\n", ld);
            throw -1;
        }

        if(fcntl(ld, F_SETFL, O_NONBLOCK) == -1) {
            fprintf(stderr, "%4d - Fcntl error.\n", ld);
            throw -1;
        }

        if(bind(ld, (struct sockaddr *)&addr, sizeof addr) == -1) {
            fprintf(stderr, "%4d - Bind error.\n", ld);
            throw -1;
        }

        if(listen(ld, 5) == -1) {
            fprintf(stderr, "%4d - Listen error.\n", ld);
            throw -1;
        }

        loop(ld, NC, file_desc);
    }
    catch(int e) {
        st = e;
    }

    if(ld != -1 && close(ld)) {
        fprintf(stderr, "%4d - Close error.\n", ld);
        st = -1;
    }

    puts("Done.");

    return st;
}

static void handler(int signo);

static int signalIgnoring(void) {
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags   = 0;
    sigemptyset(&act.sa_mask);

    if(sigaction(SIGINT, &act, 0) == -1) {
        fprintf(stderr, "     - Sigaction(SIGINT) error.\n");
        return -1;
    }

    if(sigaction(SIGPIPE, &act, 0) == -1) {
        fprintf(stderr, "     - Sigaction(SIGPIPE) error.\n");
        return -1;
    }

    return 0;
}

static int quit;

static void handler(int signo) {
    if(signo == SIGINT) 
        quit = 1;
}

static void loop(int ld, size_t nc, int file_desc) {
    ConnDb db(ld, nc, file_desc);

    while(!quit) 
        db.perform();
}

