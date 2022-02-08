#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "coolcc.h"

int main(int argc, char** argv) {
    if (argc < 4) {
        error("Arg count");
        return 1;
    }
    int fin = open(argv[1], O_RDONLY | O_CLOEXEC);
    if (fin < 0) {
        error("Can't find %s", argv[1]);
        return 1;
    }
    int fout = open(argv[2], O_WRONLY | O_CREAT | O_CLOEXEC);
    if (fout < 0) {
        error("Can't create %s", argv[2]);
        return 1;
    }
    const char* temp_file_name = argv[3];
    // xdflg++; // ???

    init("int", 0);
    init("char", 1);
    init("float", 2);
    return 0;
}
