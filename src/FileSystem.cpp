#include <mbed.h>
#include "FileSystem.h"

BlockDevice *bd = BlockDevice::get_default_instance();
LittleFileSystem fs("fs");
FILE *f; // file handle
DigitalOut redled(LED_RED);

// erase the block device if corrupted
void erase()
{
    printf("\r\nInitializing the block device... ");
    redled = !redled;
    fflush(stdout);
    int err = bd->init();
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err)
    {
        error("error: %s (%d)\n", strerror(-err), err);
    }

    printf("Erasing the block device... ");
    fflush(stdout);
    err = bd->erase(0, bd->size());
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err)
    {
        error("error: %s (%d)\n", strerror(-err), err);
    }

    printf("Deinitializing the block device... ");
    fflush(stdout);
    err = bd->deinit();
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err)
    {
        error("error: %s (%d)\n", strerror(-err), err);
    }
    redled = !redled;
}

void MountFileSystem()
{
    printf("Mounting the filesystem... ");
    fflush(stdout);
    int err = fs.mount(bd);
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err)
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
    // Unmount the file system
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
    // Open the data file
    printf("Opening \"/fs/data.txt\"... ");
    fflush(stdout);
    f = fopen("/fs/data.txt", "r+");
    printf("%s\n", (!f ? "Fail :(" : "OK"));
    if (!f)
    {
        // Create the data file if it doesn't exist
        printf("No file found, creating a new file... ");
        fflush(stdout);
        f = fopen("/fs/data.txt", "w+");
        printf("%s\n", (!f ? "Fail :(" : "OK"));
        if (!f)
        {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }
        printf("Seeking file... ");
        fflush(stdout);
        int err = fseek(f, 0, SEEK_SET);
        printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
        if (err < 0)
        {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }
    }

    // Store number
    fprintf(f, "%f\n", data);

    // Flush between write and read on same file
    fflush(f);

    // Close the file which also flushes any cached writes
    printf("Closing \"/fs/data.txt\"... ");
    fflush(stdout);
    int err = fclose(f);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    if (err < 0)
    {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
}

// read data from file
void ReadFile(float *record)
{
    // Display the data file
    printf("Opening \"/fs/data.txt\"... ");
    fflush(stdout);
    f = fopen("/fs/data.txt", "r+");
    printf("%s\n", (!f ? "Fail :(" : "OK"));
    if (!f)
    {
        // error("error: %s (%d)\n", strerror(errno), -errno);
        printf("No file found, creating a new file... ");
        fflush(stdout);
        f = fopen("/fs/data.txt", "w+");
        printf("%s\n", (!f ? "Fail :(" : "OK"));
        if (!f)
        {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }
        printf("Seeking file... ");
        fflush(stdout);
        int err = fseek(f, 0, SEEK_SET);
        printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
        if (err < 0)
        {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }
    }

    // float record;
    while (!feof(f))
    {
        fscanf(f, "%f\n", record);
    }

    printf("\rClosing \"/fs/data.txt\"... ");
    fflush(stdout);
    int err = fclose(f);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    if (err < 0)
    {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }
}