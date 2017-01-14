#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define INPUT 'i'
#define OUTPUT 'o'
#define SEGFAULT 's'
#define CATCH 'c'

// the SIGSEGV handler that is registered
void segfault_handler()
{
    fprintf(stderr, "Segmentation fault caught, now quitting...\n");
    _exit(3);
}

// forces a segmentation fault
void cause_segfault()
{
    char* ptr = NULL;
    *ptr = 'a';
    return;
}


int main(int argc, char *argv[])
{
    // long options
    struct option long_options[] = 
    {
        {"input", required_argument, NULL, INPUT},
        {"output", required_argument, NULL, OUTPUT},
        {"segfault", no_argument, NULL, SEGFAULT},
        {"catch", no_argument, NULL, CATCH},
        {0, 0, 0, 0}
    };

    int ret = 0;
    int do_segfault = 0; // flag for whether to cause segfault
    while (1)
    {
        ret = getopt_long(argc, argv, "", long_options, NULL);
        if (ret == -1)
        {
            break;
        }

        switch (ret)
        {
            case INPUT:
            {
                int ifd = open(optarg, O_RDONLY);
                if (ifd >= 0)
                {
                    dup2(ifd, 0);
                }
                else // the input file could not be opened
                {
                    fprintf(stderr, "Unable to open the specified input file\n");
                    perror("Error");
                    _exit(1);
                }
                break;
            }
            case OUTPUT:
            {
                int ofd = creat(optarg, 0666);
                if (ofd >= 0)
                {
                    dup2(ofd, 1);
                }
                else // the output file could not be created
                {
                    fprintf(stderr, "Unable to create the specified output file\n");
                    perror("Error");
                    _exit(2);
                }
                break;
            }
            case SEGFAULT:
            {
                // set the do_segfault flag to true
                do_segfault = 1;
                break;
            }
            case CATCH:
            {
                // set up the segfault handler
                signal(SIGSEGV, segfault_handler);
                break;
            }
            default: // the default case is for any arguments that are not accepted
            {
                fprintf(stderr, "There is an error in the arguments...\n");
                exit(1);
            }
        }
    }

    // --segfault argument was used
    if (do_segfault == 1)
    {
        cause_segfault();
    }

    // read from fd0 and write to fd1
    char in;
    while (read(0, &in, sizeof(char)) > 0) // read until EOF
    {
        if (write(1, &in, sizeof(char)) < 0)
        {
            // report error in writing to fd1, then exit with return code 2 because 
            // output file could not be created
            fprintf(stderr, "Not able to write char to fd1");
            exit(2);
        }
    }
    
    _exit(0);
}