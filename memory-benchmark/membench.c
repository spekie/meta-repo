#include <stdio.h>
#include <time.h>
#define ARRAY_MIN (1024)
#define ARRAY_MAX (4096*4096)

int x[ARRAY_MAX];

double get_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void label(int i) {
    if (i < 1e3)
        printf("%1dB,", i);
    else if (i < 1e6)
        printf("%1dK,", i / 1024);
    else if (i < 1e9)
        printf("%1dM,", i / 1048576);
    else
        printf("%1dG,", i / 1073741824);
}

int main(int argc, char* argv[]) {
    int nextstep, i, index, stride;
    int csize;
    double steps, tsteps;
    double loadtime, lastsec, sec0, sec1, sec;

    printf(" ,");
    for (stride = 1; stride <= ARRAY_MAX / 2; stride = stride * 2)
        label(stride * sizeof(int));
    printf("\n");

    for (csize = ARRAY_MIN; csize <= ARRAY_MAX; csize = csize * 2) {
        label(csize * sizeof(int));
        for (stride = 1; stride <= csize / 2; stride = stride * 2) {
            for (index = 0; index < csize; index = index + stride)
                x[index] = index + stride;
            x[index - stride] = 0;
            lastsec = get_seconds();
            sec0 = get_seconds();
            while (sec0 == lastsec)
                sec0 = get_seconds();  // wait until time changes
            steps = 0.0;
            nextstep = 0;
            sec0 = get_seconds();
            do {
                for (i = stride; i != 0; i = i - 1) {
                    nextstep = 0;
                    do {
                        nextstep = x[nextstep];
                    } while (nextstep != 0);
                }
                steps = steps + 1.0;
                sec1 = get_seconds();
            } while ((sec1 - sec0) < 20.0);
            sec = sec1 - sec0;
            tsteps = 0.0;
            sec0 = get_seconds();
            do {
                for (i = stride; i != 0; i = i - 1) {
                    index = 0;
                    do {
                        index = index + stride;
                    } while (index < csize);
                }
                tsteps = tsteps + 1.0;
                sec1 = get_seconds();
            } while (tsteps < steps);
            sec = sec - (sec1 - sec0);
            loadtime = (sec * 1e9) / (steps * csize);
            printf("%4.1f,", (loadtime < 0.1) ? 0.1 : loadtime);
        }
        printf("\n");
    }
    return 0;
}
