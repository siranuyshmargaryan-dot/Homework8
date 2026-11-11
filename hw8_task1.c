
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char source[256], dest[256], buffer[1024];
    int fd1, fd2;
    ssize_t n, written;
    long long total = 0;

    printf("Source file: ");
    if (scanf("%255s", source) != 1) return 1;

    printf("Destination file: ");
    if (scanf("%255s", dest) != 1) return 1;

    fd1 = open(source, O_RDONLY);
    if (fd1 == -1) {
        perror("open source");
        return 1;
    }

    fd2 = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 == -1) {
        perror("open destination");
        close(fd1);
        return 1;
    }

    while ((n = read(fd1, buffer, sizeof(buffer))) > 0) {
        ssize_t pos = 0;

        while (pos < n) {
            written = write(fd2, buffer + pos, n - pos);
            if (written <= 0) {
                perror("write");
                close(fd1);
                close(fd2);
                return 1;
            }
            pos += written;
        }
        total += n;
    }

    if (n == -1) {
        perror("read");
        close(fd1);
        close(fd2);
        return 1;
    }

    if (close(fd1) == -1) {
        perror("close source");
        close(fd2);
        return 1;
    }
    if (close(fd2) == -1) {
        perror("close destination");
        return 1;
    }

    printf("Total bytes copied: %lld\n", total);
    return 0;
}
