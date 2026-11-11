
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    off_t size;

    fd = open("sparse.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (write(fd, "START", 5) != 5) {
        perror("write");
        close(fd);
        return 1;
    }

    if (lseek(fd, 1024 * 1024, SEEK_CUR) == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    // The skipped space is a hole, so disk blocks are not
    // allocated for all those zeros. du shows smaller usage.
    if (write(fd, "END", 3) != 3) {
        perror("write");
        close(fd);
        return 1;
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    fd = open("sparse.bin", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    printf("File size: %lld bytes\n", (long long)size);

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}
