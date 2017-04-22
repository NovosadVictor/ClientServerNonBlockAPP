#include "conn.hpp"

Conn::Conn(int fd, int file_desc):
        fd_(fd), actv_(true), request_(Parser(file_desc)),
        icur_(0), iall_(4), already_written_(true),
        ocur_(0), oall_(1, 0),
        ibuf_(4, 0), obuf_(1, std::vector<char>(4, 0)),
        file_desc_(file_desc), curr_(0) {}

Conn::~Conn() {
    close_();
}

bool Conn::canRcv() const {
    return actv_ && iall_ > icur_ && already_written_;
}

bool Conn::canSnd() const {
    return actv_ && oall_[curr_] > ocur_;
}

bool Conn::isActv() const {
    return actv_;
}

void Conn::rcv() {
    uint32_t h;
    ssize_t s;

    if (!canRcv()) return;

    s = read(fd_, &ibuf_[icur_], iall_ - icur_);

    if (s > 0)
        icur_ += s;
    else if (!s) {
        close_();
        return;
    } else if (s == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
        fprintf(stderr, "%4d - Read error.\n", fd_);
        close_();
        return;
    }

    if (iall_ == icur_) {
        if (iall_ == sizeof h) {
            memcpy(&h, &ibuf_[0], sizeof h);

            ibuf_.resize(h + sizeof h, 0);
            iall_ += h;
        } else {
            printf("Input request from %d:\n    ", fd_);
            puts(&ibuf_[4]);
            obuf_.clear();
            oall_.clear();
            already_written_ = false;
            try {
                request_.ParseRequest(&ibuf_[sizeof(h)], file_desc_);
            }
            catch (int err) {
                obuf_.resize(1, std::vector<char>(4, 0));
                oall_.resize(1, 0);

                h = 63;
                oall_[0] = h + sizeof(h);
                obuf_[0].resize(oall_[0], 0);
                memcpy(&obuf_[0][0], &h, sizeof(h));
                sprintf(&obuf_[0][0] + sizeof(h),
                        "Server:\n   status = -1\n    It was wrong request, try again...\n");
                obuf_[0][oall_[0] - 1] = '\0';
                return;
            }
            int type = request_.GetType();
            if (type > 1) {
                oall_.resize(2, 0);
                obuf_.resize(2, std::vector<char>(4, 0));
                h = 22;
                oall_[0] = h + sizeof(h);
                obuf_[0].resize(oall_[0], 0);
                memcpy(&obuf_[0][0], &h, sizeof(h));
                sprintf(&obuf_[0][0] + sizeof(h), "Server:\n   status = %d", type);
                obuf_[0][21] = '\0';
                switch (type) {
                    case 2: {
                        h = 30;
                        oall_[1] = h + sizeof(h);
                        obuf_[1].resize(oall_[1], 0);
                        memcpy(&obuf_[1][0], &h, sizeof(h));
                        sprintf(&obuf_[1][0] + sizeof(h), "\n    User successfully added\n");
                        obuf_[1][oall_[1] - 1] = '\0';
                        break;
                    }
                    case 3: {
                        h = 42;
                        oall_[1] = h + sizeof(h);
                        obuf_[1].resize(oall_[1], 0);
                        memcpy(&obuf_[1][0], &h, sizeof(h));
                        size_t total = request_.GetTotal();
                        sprintf(&obuf_[1][0] + sizeof(h), "%07lu\n    User(s) successfully deleted\n",
                                total);
                        obuf_[1][oall_[1] - 1] = '\0';
                        break;
                    }
                    case 4: {
                        h = 42;
                        oall_[1] = h + sizeof(h);
                        obuf_[1].resize(oall_[1], 0);
                        memcpy(&obuf_[1][0], &h, sizeof(h));
                        size_t total = request_.GetTotal();
                        sprintf(&obuf_[1][0] + sizeof(h), "%07lu\n    User(s) successfully updated\n",
                                total);
                        obuf_[1][oall_[1] - 1] = '\0';
                        break;
                    }
                    case 7: {
                        h = 24;
                        oall_[1] = h + sizeof(h);
                        obuf_[1].resize(oall_[1], 0);
                        memcpy(&obuf_[1][0], &h, sizeof(h));
                        sprintf(&obuf_[1][0] + sizeof(h), "\n    All changes saved\n");
                        obuf_[1][oall_[1] - 1] = '\0';
                        break;
                    }
                }
                return;
            }
            if (type == 1) {
                obuf_.resize(request_.GetResponse().size() + 2, std::vector<char>(4, 0));
                oall_.resize(request_.GetResponse().size() + 2, 0);

                h = 22;
                oall_[0] = h + sizeof(h);
                obuf_[0].resize(oall_[0], 0);
                memcpy(&obuf_[0][0], &h, sizeof(h));
                sprintf(&obuf_[0][0] + sizeof(h), "Server:\n   status = %d", type);
                obuf_[0][oall_[0] - 1] = '\0';

                h = 24;
                oall_[1] = h + sizeof(h);
                obuf_[1].resize(oall_[1], 0);
                memcpy(&obuf_[1][0], &h, sizeof(h));
                size_t request_response_size = (request_.GetResponse()).size();
                sprintf(&obuf_[1][0] + sizeof(h), "    quantity = %07lu\n", request_response_size);
                obuf_[1][oall_[1] - 1] = '\0';

                for (size_t i = 2; i < oall_.size(); ++i) {
                    h = 52;
                    oall_[i] = h + sizeof(h);
                    obuf_[i].resize(oall_[i], 0);
                    memcpy(&obuf_[i][0], &h, sizeof(h));
                    int service = (request_.GetResponse()[i - 2]).GetService();
                    double sum = (request_.GetResponse()[i - 2]).GetSum();
                    sprintf(&obuf_[i][0] + sizeof(h), "    %s %d %s %05lf",
                            (request_.GetResponse()[i - 2]).GetPhone().GetNumber(),
                            service,
                            (request_.GetResponse()[i - 2]).GetDate().GetDate(),
                            sum
                    );
                    obuf_[i][oall_[i] - 1] = '\0';
                }
                return;
            }
        }
    }
}

void Conn::snd() {
    if(!canSnd()) return;

    ssize_t s = write(fd_, &obuf_[curr_][0] + ocur_, oall_[curr_] - ocur_);

    if(s > 0)
        ocur_ += s;
    else
        if(!s) {
            close_();
            return;
        }
        else
            if(s == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
                fprintf(stderr, "%4d - Write error.\n", fd_);
                close_();
                return;
            }

    if(oall_[curr_] == ocur_) {
        oall_[curr_] = 0;
        ocur_ = 0;
        ++curr_;
    }
    if (curr_ == obuf_.size()) {
        already_written_ = true;
        icur_ = 0;
        iall_ = 4;
        curr_ = 0;
    }
}

void Conn::close_() {
    if(actv_) {
        printf("%4d - Close.\n", fd_);

        if(shutdown(fd_, 2) == -1)
            fprintf(stderr, "%4d - Shutdown error.\n", fd_);

        if(close(fd_))
            fprintf(stderr, "%4d - Close error.\n", fd_);

        actv_ = false;
    }
}


ConnDb::ConnDb(int ld, size_t nc, int file_desc): file_desc_(file_desc), ld_(ld), nc_(nc), cs_(nc, 0), ps_(nc + 1) {
    ps_[0].fd = ld;
    ps_[0].events = POLLIN;
}

ConnDb::~ConnDb() {
    for(size_t d = 0; d < nc_; d ++)
        delete cs_[d];
}

void ConnDb::perform() {
    nfds_t np;    
    size_t d;

    for(np = 1, d = 0; d < nc_; d ++)
        if(cs_[d] && cs_[d]->isActv()) {
            short e = 0;

            if(cs_[d]->canRcv())
                e = POLLIN;


            if(cs_[d]->canSnd())
                e = e ? e | POLLOUT : POLLOUT;

            if(e) { 
                ps_[np].fd     = d;
                ps_[np].events = e;
                np ++;
            }
        }

    switch(poll(&ps_[0], np, 1 * 1000)) {
        case -1:
            if(errno != EINTR)
                fprintf(stderr, "     - Poll error.\n"); 
            break;

        default:
            if(ps_[0].revents & POLLIN) 
                accept_();

            for(d = 1; d < np; d ++) {
                if(ps_[d].revents & POLLIN)
                    cs_[ps_[d].fd]->rcv();

                if(ps_[d].revents & POLLOUT)
                    cs_[ps_[d].fd]->snd();

            }
    }
}

static void printConn(int fd, struct sockaddr_in *addr);

void ConnDb::accept_() {
    struct sockaddr_in addr;
    socklen_t          addrlen;
    int                fd;

    memset(&addr, 0, sizeof addr);
    addrlen = sizeof addr;

    fd = accept(ld_, (struct sockaddr *)&addr, &addrlen);
    if(fd == -1) {
        if(fd == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
            fprintf(stderr, "     - Accept error.\n");
        return;
    }

    printConn(fd, &addr);

    if((size_t)fd >= nc_) {
        fprintf(stderr, "%4d - Storage limit error.\n", fd);
        Conn c(fd, file_desc_);
    }
    else
        if(fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
            fprintf(stderr, "%4d - Nonblock error.\n", fd);
            Conn c(fd, file_desc_);
        }
        else {
            delete cs_[fd];
            cs_[fd] = new Conn(fd, file_desc_);
        }
}

static void printConn(int fd, struct sockaddr_in *addr) {
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof ip);
    printf("%4d -New (%s %d).\n", fd, ip, addr->sin_port);
}

int toUInt16(const char *str, uint16_t *p) {
    long  l;
    char *e;

    errno = 0;
    l = strtol(str, &e, 10);

    if(!errno && *e == '\0' && 0 <= l && l <= 65535) {
        *p = (uint16_t)l;
        return 0;
    }
    else
        return -1;
}

