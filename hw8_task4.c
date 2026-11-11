
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char buffer[256], prefix[50];
    ssize_t n;
    off_t pos;

    fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    printf("Enter log message: ");
    fflush(stdout);

    n = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (n == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    int len = snprintf(prefix, sizeof(prefix), "PID=%ld: ",
                       (long)getpid());

    if (write(fd, prefix, len) != len) {
        perror("write");
        close(fd);
        return 1;
    }

    ssize_t line_len = 0;
    while (line_len < n && buffer[line_len] != '\n')
        line_len++;

    if (line_len > 0 && write(fd, buffer, line_len) != line_len) {
        perror("write");
        close(fd);
        return 1;
    }

    if (write(fd, "\n", 1) != 1) {
        perror("write");
        close(fd);
        return 1;
    }

    // O_APPEND moves each write to the end, so SEEK_CUR
    // returns the updated offset after writing.
    pos = lseek(fd, 0, SEEK_CUR);
    if (pos == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    printf("Final offset: %lld\n", (long long)pos);

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}

