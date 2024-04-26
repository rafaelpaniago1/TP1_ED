#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define ALGBUBBLE      1
#define ALGINSERTION   2
#define ALGSELECTION   3
#define ALGMERGE       4
#define ALGQSORT       5
#define ALGSHELLSORT   6
#define ALGCOUTINGSORT 7
#define ALGBUCKETSORT  8
#define ALGRADIXSORT   9

typedef struct alg {
    int num;
    char* name;
} alg_t;

alg_t algvet[] = {
    {ALGBUBBLE,"b"},
    {ALGINSERTION,"i"},
    {ALGSELECTION,"s"},
    {ALGMERGE,"m"},
    {ALGQSORT,"q"},
    {ALGSHELLSORT ,"h"},
    {ALGCOUTINGSORT,"c"},
    {ALGBUCKETSORT,"bs"},
    {ALGRADIXSORT, "r"},
    {0,0}
};

int name2num(char* name) {
    int i = 0;
    while (algvet[i].num) {
        if (!strcmp(algvet[i].name, name)) return algvet[i].num;
        i++;
    }
    return 0;
}

char* num2name(int num) {
    int i = 0;
    while (algvet[i].num) {
        if (algvet[i].num == num) return algvet[i].name;
        i++;
    }
    return 0;
}

typedef struct opt {
    int size;
    int seed;
    int alg;
} opt_t;

typedef struct sortperf {
    int cmp;
    int move;
    int calls;
} sortperf_t;

void resetcounter(sortperf_t* s) {
    s->cmp = 0;
    s->move = 0;
    s->calls = 0;
}

void inccmp(sortperf_t* s, int num) {
    s->cmp += num;
}

void incmove(sortperf_t* s, int num) {
    s->move += num;
}

void inccalls(sortperf_t* s, int num) {
    s->calls += num;
}

char* printsortperf(sortperf_t* s, char* str, char* pref) {
    sprintf(str, "%s cmp %d move %d calls %d", pref, s->cmp, s->move, s->calls);
    return str;
}

void initVector(long int* vet, int size) {
    int i;
    for (i = 0; i < size; i++) {
        vet[i] = (long int)(size-i);
    }
}

void printVector(long int* vet, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%ld ", vet[i]);
    }
    printf("\n");
}

void swap(long int* xp, long int* yp, sortperf_t* s) {
    long int temp = *xp;
    *xp = *yp;
    *yp = temp;
    incmove(s, 3);
}

void bubbleSort(long int arr[], int l, int r) {
    int i, j;
    for (i = l; i < r; i++) {
        for (j = l; j < r - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                long int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void shellSort(long int* A, int n, sortperf_t* s) {
    int h = n >> 1;
    while (h >= 1) {
        for (int k = 0; k < h; k++) {
            for (int j = k + h; j < n; j += h) {
                int i = j;
                int cur = A[i];
                while (i > k && A[i - h] > cur) {
                    A[i] = A[i - h];
                    i -= h;
                }
                A[i] = cur;
            }
        }
        h >>= 1;
    }
}

void selectionSort(long int arr[], int l, int r, sortperf_t* s) {
    for (int i = l; i <= r; i++) {
        int min = i;
        for (int j = i + 1; j <= r; j++) {
            min = (arr[j] < arr[min] ? j : min);
        }
        if (min != i) {
            swap(&arr[i], &arr[min], s);
        }
    }
}

void insertionSort(long int v[], int l, int r, sortperf_t* s) {
    for (int i = l + 1; i <= r; i++) {
        int j = i;
        long int cur = v[j];
        while (j >= l && v[j - 1] > cur) {
            v[j] = v[j - 1];
            --j;
        }
        v[j] = cur;
    }
}

void partition(long int* A, int l, int r, int* i, int* j, sortperf_t* s) {
    int pivot = A[(r + l) / 2];
    while (*i <= *j) {
        while (A[*i] < pivot) {
            (*i)++;
        }
        while (A[*j] > pivot) {
            (*j)--;
        }
        if (*i <= *j) {
            swap(&A[*i], &A[*j], s);
            (*i)++, (*j)--;
        }
    }
}

void quickSort(long int* A, int l, int r, sortperf_t* s) {
    int i = l, j = r;
    partition(A, l, r, &i, &j, s);
    if (j > l) {
        quickSort(A, l, j, s);
    }
    if (i < r) {
        quickSort(A, i, r, s);
    }
}

void merge(long int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    long int* L = (long int*)malloc(n1 * sizeof(long int));
    long int* R = (long int*)malloc(n2 * sizeof(long int));

    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(long int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void countingSort(long int arr[], int n, int exp) {
    long int output[n];
    int i, count[10] = {0};

    for (i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    for (i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void radixSort(long int arr[], int l, int r) {
    int n = r - l + 1;
    long int max = arr[l];
    for (int i = l + 1; i <= r; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSort(arr, n, exp);
    }
}

void bucketSort(long int arr[], int l, int r) {
    int n = r - l + 1;
    
    // Find the minimum and maximum values in the array
    long int min = arr[l];
    long int max = arr[l];
    for (int i = l + 1; i <= r; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    // Determine the number of buckets and their size
    int numBuckets = (int)(max - min) + 1;
    int bucketSize = (n + numBuckets - 1) / numBuckets; // Ceiling division

    // Allocate memory for buckets
    long int* buckets[numBuckets];
    for (int i = 0; i < numBuckets; i++) {
        buckets[i] = (long int*)malloc(bucketSize * sizeof(long int));
    }

    // Distribute elements into buckets
    for (int i = l; i <= r; i++) {
        int bucketIndex = (arr[i] - min) / bucketSize;
        int bucketPos = bucketIndex < numBuckets ? bucketIndex : numBuckets - 1;
        buckets[bucketPos][i - l] = arr[i];
    }

    // Sort each bucket individually
    for (int i = 0; i < numBuckets; i++) {
        int bucketLen = (i == numBuckets - 1) ? (r - l + 1) % bucketSize : bucketSize;
        if (bucketLen > 0) {
            insertionSort(buckets[i], 0, bucketLen - 1, NULL); // Sorting without performance tracking
        }
    }

    // Concatenate the sorted buckets
    int index = l;
    for (int i = 0; i < numBuckets; i++) {
        int bucketLen = (i == numBuckets - 1) ? (r - l + 1) % bucketSize : bucketSize;
        for (int j = 0; j < bucketLen; j++) {
            arr[index++] = buckets[i][j];
        }
        free(buckets[i]); // Free memory allocated for buckets
    }
}

void uso() {
    fprintf(stderr, "sortperf\n");
    fprintf(stderr, "\t-z <int>\t(vector size)\n");
    fprintf(stderr, "\t-s <int>\t(initialization seed)\n");
    fprintf(stderr, "\t-a <|b|i|s|m|q|h|c|bs|r|>\t(algorithm)\n");
    fprintf(stderr, "\t    b\tbubble\n");
    fprintf(stderr, "\t    i\tinsertion\n");
    fprintf(stderr, "\t    s\tselection\n");
    fprintf(stderr, "\t    m\tmerge\n");
    fprintf(stderr, "\t    q\tquicksort\n");
    fprintf(stderr, "\t    h\tshellsort\n");
    fprintf(stderr, "\t    c\tcouting\n");
    fprintf(stderr, "\t    bs\tbucket\n");
    fprintf(stderr, "\t    r\tradix\n");
}

void parse_args(int argc, char** argv, opt_t* opt) {
    extern char* optarg;
    extern int optind;
    int c;

    opt->seed = 1;
    opt->size = 10;
    opt->alg = 1;

    while ((c = getopt(argc, argv, "z:s:a:h")) != EOF) {
        switch (c) {
        case 'z':
            opt->size = atoi(optarg);
            break;
        case 's':
            opt->seed = atoi(optarg);
            break;
        case 'a':
            opt->alg = name2num(optarg);
            break;
        case 'h':
        default:
            uso();
            exit(1);
        }
    }
    if (!opt->alg) {
        uso();
        exit(1);
    }
}

void clkDiff(struct timespec t1, struct timespec t2, struct timespec* res) {
    if (t2.tv_nsec < t1.tv_nsec) {
        res->tv_nsec = 1000000000 + t2.tv_nsec - t1.tv_nsec;
        res->tv_sec = t2.tv_sec - t1.tv_sec - 1;
    } else {
        res->tv_nsec = t2.tv_nsec - t1.tv_nsec;
        res->tv_sec = t2.tv_sec - t1.tv_sec;
    }
}

int main(int argc, char** argv) {
    sortperf_t s;
    long int* vet;
    char buf[200];
    char pref[100];
    opt_t opt;
    struct timespec inittp, endtp, restp;
    int retp;

    parse_args(argc, argv, &opt);

    vet = (long int*)malloc((opt.size + 1) * sizeof(long int));

    resetcounter(&s);
    srand48(opt.seed);
    initVector(vet, opt.size);
    vet[opt.size] = vet[0];

    retp = clock_gettime(CLOCK_MONOTONIC, &inittp);

    switch (opt.alg) {
    case ALGBUBBLE:
        bubbleSort(vet, 0, opt.size - 1);
        break;
    case ALGINSERTION:
        insertionSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGSELECTION:
        selectionSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGMERGE:
        mergeSort(vet, 0, opt.size - 1);
        break;
    case ALGQSORT:
        quickSort(vet, 0, opt.size - 1, &s);
        break;
    case ALGSHELLSORT:
        shellSort(vet, opt.size, &s);
        break;
    case ALGCOUTINGSORT:
        countingSort(vet, 0, opt.size - 1);
        break;
    case ALGBUCKETSORT:
        bucketSort(vet, 0, opt.size - 1);
        break;
    case ALGRADIXSORT:
        radixSort(vet, 0, opt.size - 1);
        break;
    }
    retp = clock_gettime(CLOCK_MONOTONIC, &endtp);
    clkDiff(inittp, endtp, &restp);

    sprintf(pref, "alg %s seed %d size %d time %ld.%.9ld", num2name(opt.alg), opt.seed, opt.size, restp.tv_sec,
            restp.tv_nsec);

    printsortperf(&s, buf, pref);
    printf("%s\n", buf);

    exit(0);
}
