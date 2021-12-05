#include <mbed.h>
#include "fs.h"

void MountFileSystem()
{
    // Try to mount the filesystem
    printf("Mounting the filesystem... ");
    fflush(stdout);
    int err = fs.mount(bd);
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err || FORCE_REFORMAT)
    {
        // Reformat if we can't mount the filesystem
        printf("formatting... ");
        fflush(stdout);
        err = fs.reformat(bd);
        printf("%s\n", (err ? "Fail :(" : "OK"));
        if (err)
        {
            error("error: %s (%d)\n", strerror(-err), err);
        }
    }
}

void UnmountFileSystem()
{
    // Tidy up
    printf("Unmounting... ");
    fflush(stdout);
    int err = fs.unmount();
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    if (err < 0)
    {
        error("error: %s (%d)\n", strerror(-err), err);
    }
}

// write data to file
void WriteFile(float data)
{
    // Open the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);
    FILE *f = fopen("/fs/numbers.txt", "r+");
    printf("%s\n", (!f ? "Fail :(" : "OK"));
    if (!f)
    {
        // Create the numbers file if it doesn't exist
        printf("No file found, creating a new file... ");
        fflush(stdout);
        f = fopen("/fs/numbers.txt", "w+");
        printf("%s\n", (!f ? "Fail :(" : "OK"));
        if (!f)
        {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }

        printf("\rWriting data: %d", data);
        fflush(stdout);
        int err = fprintf(f, "%f m\n", data);
        if (err < 0)
        {
            printf("Fail :(\n");
            error("error: %s (%d)\n", strerror(errno), -errno);
        }
    }
}

// read data from file
void ReadFile()
{
    // Display the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);
    FILE *f = fopen("/fs/numbers.txt", "r");
    printf("%s\n", (!f ? "Fail :(" : "OK"));
    if (!f)
    {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    printf("numbers:\n");
    while (!feof(f))
    {
        int c = fgetc(f);
        printf("%c", c);
    }

    printf("\rClosing \"/fs/numbers.txt\"... ");
    fflush(stdout);
    int err = fclose(f);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    if (err < 0)
    {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
}