#include "util.h"

int* read_next_line(FILE* fin) {
    char buffer[4096];

    if (fgets(buffer, sizeof(buffer), fin) == NULL) {
        return NULL;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    if (buffer[0] == '\0') {
        return NULL;
    }

    char* copy = malloc(strlen(buffer) + 1);
    if (copy == NULL) {
        fprintf(stderr, "Error: unable to allocate memory!\n\n");
        exit(-3);
    }
    strcpy(copy, buffer);

    int count = 1;
    for (char* p = copy; *p != '\0'; ++p) {
        if (*p == ',') {
            ++count;
        }
    }

    int* line = malloc((count + 1) * sizeof(int));
    if (line == NULL) {
        free(copy);
        fprintf(stderr, "Error: unable to allocate memory!\n\n");
        exit(-3);
    }

    line[0] = count;

    char* token = strtok(copy, ",");
    for (int i = 1; token != NULL && i <= count; ++i) {
        line[i] = atoi(token);
        token = strtok(NULL, ",");
    }

    free(copy);
    return line;
}


float compute_average(int* line) {
    int count = line[0];
    if (count <= 0) {
        return 0.0f;
    }

    double sum = 0.0;
    for (int i = 1; i <= count; ++i) {
        sum += line[i];
    }

    return (float)(sum / count);
}

float compute_stdev(int* line) {
    int count = line[0];
    if (count <= 0) {
        return 0.0f;
    }

    float mean = compute_average(line);
    float sum_sq = 0.0f;
    for (int i = 1; i <= count; ++i) {
        float d = line[i] - mean;
        sum_sq += d * d;
    }

    return sqrtf(sum_sq / count);
}
