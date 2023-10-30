#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

ssize_t read_textfile(const char *filename, size_t letters) {
    if (filename == NULL)
        return 0;

    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        return 0;

    char *buffer = (char *)malloc(letters);
    if (buffer == NULL) {
        close(fd);
        return 0;
    }

    ssize_t bytes_read = read(fd, buffer, letters);
    if (bytes_read == -1) {
        close(fd);
        free(buffer);
        return 0;
    }

    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
    if (bytes_written == -1 || bytes_written != bytes_read) {
        close(fd);
        free(buffer);
        return 0;
    }

    close(fd);
    free(buffer);

    return bytes_read;
}

int main() {
    const char *filename = "your_text_file.txt"; 
    size_t letters_to_read = 1024;  

    ssize_t letters_read = read_textfile(filename, letters_to_read);
    if (letters_read == 0) {
        printf("Error: Unable to read the file.\n");
    } else {
        printf("\nTotal letters read and printed: %zd\n", letters_read);
    }

    return 0;
}
