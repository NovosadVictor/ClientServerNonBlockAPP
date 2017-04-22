#ifndef __CONN_HPP__
#define __CONN_HPP__

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

#include "parser.h"

#include <vector>

class Conn {
public:
    Conn(int fd, int file_desc);
   ~Conn();

    bool canRcv() const;
    bool canSnd() const;
    bool isActv() const;

    void rcv();
    void snd();

private:
    Parser request_;
    int               fd_;
    int file_desc_;
    bool              actv_;
    bool already_written_;
    size_t            icur_,
                      iall_,
                      ocur_,
                        curr_;
    std::vector<size_t> oall_;
    std::vector<char> ibuf_;
    std::vector<std::vector<char> > obuf_;

    void close_();
};

class ConnDb {
public:
    ConnDb(int ld, size_t nc, int file_desc);
   ~ConnDb();

    void perform();

private:
    int file_desc_;
    int                 ld_;
    size_t              nc_;
    std::vector<Conn *> cs_;
    std::vector<pollfd> ps_;

    void accept_();
};

int toUInt16(const char *str, uint16_t *p);

#endif
