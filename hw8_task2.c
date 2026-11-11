
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char buffer[27];
    char text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    off_t size;

    fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (write(fd, text, 26) != 26) {
        perror("write");
        close(fd);
        return 1;
    }
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    fd = open("data.txt", O_RDWR);
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
    printf("Original size: %lld\n", (long long)size);

    if (ftruncate(fd, 10) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }
    printf("New size: %lld\n", (long long)size);

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Content: %s\n", buffer);

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}
