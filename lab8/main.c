#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <time.h>

pthread_t *threads_arr;
int **image_in;
int **image_out;
int max_gray;
int k;
int W, H;

int **allocate2D(int r, int c)
{
    int **result = (int **)malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++)
    {
        result[i] = (int *)malloc(c * sizeof(int));
    }

    return result;
}

void deallocate(int **tab, int r, int c)
{
    for (int i = 0; i < r; i++)
    {
        free(tab[i]);
    }

    free(tab);
}

void *numbers(void *arg)
{
    clock_t real_time[2];
    real_time[0] = clock();
    int idx = *(int *)arg;
    free(arg);
    int start = (int)idx * (max_gray) / k;
    int end;
    if (idx == k - 1)
    {
        end = (idx + 1) * (int)(max_gray) / k + (max_gray) % k;
    }
    else
    {
        end = (idx + 1) * (int)(max_gray) / k - 1;
    }
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (image_in[i][j] >= start && image_in[i][j] <= end)
            {
                image_out[i][j] = 255 - image_in[i][j];
            }
        }
    }
    
    real_time[1] = clock();
    double* result = (double*) malloc(sizeof(double));
    *result = (double)(real_time[1]-real_time[0])/CLOCKS_PER_SEC;
    pthread_exit(result);

}

void *blocks(void *arg)
{
    clock_t real_time[2];
    real_time[0] = clock();

    int idx = *(int *)arg;
    free(arg);
    int start = idx * ceil(W / k);
    int end = (idx + 1) * ceil(W / k) - 1;

    for (int i = 0; i < H; i++)
    {
        for (int j = start; j <= end; j++)
        {
            image_out[i][j] = 255 - image_in[i][j];
        }
    }

    real_time[1] = clock();
    double* result = (double*) malloc(sizeof(double));
    *result = (double)(real_time[1]-real_time[0])/CLOCKS_PER_SEC;
    pthread_exit(result);
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        printf("Niepoprawna liczba argumentow!\n");
        return 1;
    }

    k = atoi(argv[1]);

    printf("Test for %s method with %d threads\n", argv[2], k);

    threads_arr = (pthread_t *)malloc(k * sizeof(pthread_t));

    char *header = (char *)malloc(256 * sizeof(char));
    char *comments = (char *)malloc(256 * sizeof(char));

    FILE *file_in = fopen(argv[3], "r");
    fscanf(file_in, "%s", header);
    while (fscanf(file_in, "%d %d", &W, &H) < 2)
    {
        fscanf(file_in, "%s", comments);
    }
    fscanf(file_in, "%d", &max_gray);

    image_in = allocate2D(H, W);
    image_out = allocate2D(H, W);

    //read image
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            fscanf(file_in, "%d", &image_in[i][j]);
        }
    }

    clock_t real_time[2];
    real_time[0] = clock();

    if (strcmp(argv[2], "numbers") == 0)
    {
        for (int i = 0; i < k; i++)
        {
            int *idx = (int *)malloc(sizeof(int));
            *idx = i;
            if (pthread_create(&threads_arr[i], NULL, &numbers, idx) != 0)
            {
                perror("Failled to create thread");
            }
        }
    } else
    {
        for (int i = 0; i < k; i++)
        {
            int *idx = (int *)malloc(sizeof(int));
            *idx = i;
            if (pthread_create(&threads_arr[i], NULL, &blocks, idx) != 0)
            {
                perror("Failled to create thread");
            }
        }
    }
    double* result;
    for (int i = 0; i < k; i++)
    {
        if (pthread_join(threads_arr[i], (void**)&result) != 0)
        {
            
            perror("Failed to join thread");
        }
        printf("Thread nr %d execution time: %f\n", i + 1, *result);
        free(result);
    }

    real_time[1] = clock();
    printf("Program execution time: %f\n", (double)(real_time[1]-real_time[0])/CLOCKS_PER_SEC);


    FILE *file_out = fopen(argv[4], "w");

    fprintf(file_out, "%s\n%d %d\n%d\n", header, W, H, 255);

    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            fprintf(file_out, "%d ", image_out[i][j]);
        }
        fprintf(file_out, "\n");
    }

    deallocate(image_in, H, W);
    deallocate(image_out, H, W);
    free(threads_arr);
    free(comments);
    free(header);
}