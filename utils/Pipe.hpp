#ifndef PIPE_HPP
#define PIPE_HPP

#include <cstdio>

struct PipeDeleter {
    void operator()(FILE* fp) const {
        if (fp) pclose(fp);
    }
};

#endif