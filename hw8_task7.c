
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char file1[256], file2[256];
    char buf1[1024], buf2[1024];
    int fd1, fd2;
    ssize_t n1, n2;
    long long index = 0;

    printf("First file: ");
    if (scanf("%255s", file1) != 1) return 1;

    printf("Second file: ");
    if (scanf("%255s", file2) != 1) return 1;

    fd1 = open(file1, O_RDONLY);
    if (fd1 == -1) {
        perror("open first file");
        return 1;
    }

    fd2 = open(file2, O_RDONLY);
    if (fd2 == -1) {
        perror("open second file");
        close(fd1);
        return 1;
    }

    while (1) {
        n1 = read(fd1, buf1, sizeof(buf1));
        n2 = read(fd2, buf2, sizeof(buf2));

        if (n1 == -1 || n2 == -1) {
            perror("read");
            close(fd1);
            close(fd2);
            return 1;
        }

        ssize_t min = n1 < n2 ? n1 : n2;

        for (ssize_t i = 0; i < min; i++) {
            if (buf1[i] != buf2[i]) {
                printf("Files differ at byte %lld\n", index + i);
                close(fd1);
                close(fd2);
                return 1;
            }
        }

        index += min;

        if (n1 != n2) {
            printf("Files differ at byte %lld\n", index);
            close(fd1);
            close(fd2);
            return 1;
        }

        if (n1 == 0)
            break;
    }

    printf("Files are identical\n");

    if (close(fd1) == -1) {
        perror("close");
        close(fd2);
        return 1;
    }
    if (close(fd2) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
