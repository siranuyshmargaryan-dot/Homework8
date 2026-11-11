
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char buffer[100], ch;
    off_t start = 0, end;
    int lines = 0;

    fd = open("numbers.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    for (int i = 1; i <= 10; i++) {
        char line[10];
        int len = sprintf(line, "%d\n", i);
        if (write(fd, line, len) != len) {
            perror("write");
            close(fd);
            return 1;
        }
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    fd = open("numbers.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    while (lines < 3) {
        ssize_t n = read(fd, &ch, 1);
        if (n != 1) {
            if (n == -1) perror("read");
            else fprintf(stderr, "Line 4 not found\n");
            close(fd);
            return 1;
        }
        start++;
        if (ch == '\n')
            lines++;
    }

    if (read(fd, &ch, 1) != 1) {
        perror("read");
        close(fd);
        return 1;
    }
    if (read(fd, &ch, 1) != 1) {
        perror("read");
        close(fd);
        return 1;
    }

    end = lseek(fd, 0, SEEK_CUR);
    if (end == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    ssize_t n = read(fd, buffer, sizeof(buffer));
    if (n == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    if (lseek(fd, start, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    if (write(fd, "100\n", 4) != 4) {
        perror("write");
        close(fd);
        return 1;
    }

    if (write(fd, buffer, n) != n) {
        perror("write");
        close(fd);
        return 1;
    }

    if (ftruncate(fd, start + 4 + n) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    printf("Final content:\n");
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        ssize_t pos = 0;
        while (pos < n) {
            ssize_t w = write(STDOUT_FILENO, buffer + pos, n - pos);
            if (w <= 0) {
                perror("write");
                close(fd);
                return 1;
            }
            pos += w;
        }
    }

    if (n == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}
