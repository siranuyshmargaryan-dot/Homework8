
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char path[256], ch;
    int fd;
    off_t size;

    printf("Enter file path: ");
    if (scanf("%255s", path) != 1) return 1;

    fd = open(path, O_RDONLY);
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

    for (off_t i = size; i > 0; i--) {
        if (lseek(fd, i - 1, SEEK_SET) == -1) {
            perror("lseek");
            close(fd);
            return 1;
        }

        if (read(fd, &ch, 1) != 1) {
            perror("read");
            close(fd);
            return 1;
        }

        if (write(STDOUT_FILENO, &ch, 1) != 1) {
            perror("write");
            close(fd);
            return 1;
        }
    }

    if (write(STDOUT_FILENO, "\n", 1) != 1) {
        perror("write");
        close(fd);
        return 1;
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}
