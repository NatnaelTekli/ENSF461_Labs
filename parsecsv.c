#include <stdio.h>
#include <string.h>
#include "record_list.h"
#include "util.h"


int main(int argc, char** argv) {

    char usage[] = "Usage: parsecsv.out <input CSV file> <output CSV file>\n\n";
    char foerr[] = "Error: unable to open/create file\n\n";

    if ( argc != 3 ) {
        fprintf(stderr, "Usage: parsecsv.out <input CSV file> <output CSV file>\n\n");
        return -1;
    }

    FILE* fin = fopen(argv[1], "r");
    if ( fin == NULL ) {
        fprintf(stderr, "Error: unable to open file %s\n\n", argv[1]);
        return -2;
    }

    int* newline = read_next_line(fin);
    record_t* head = NULL;
    record_t* curr = NULL;
    
    while ( newline != NULL ) {
        float avg = compute_average(newline);
        float sdv = compute_stdev(newline);
        curr = append(curr, avg, sdv);
        if ( head == NULL )
            head = curr;
        free(newline);
        newline = read_next_line(fin);
    }
    fclose(fin);

    FILE* fout = fopen(argv[2], "w");
    if ( fout == NULL ) {
        fprintf(stderr, "Error: unable to open/create file\n\n");
        while ( head != NULL ) {
            record_t* temp = head;
            head = next(head);
            free(temp);
        }
        return -3;
    }

    record_t* current = head;
    while ( current != NULL ) {
        fprintf(fout, "%.6f,%.6f\n", current->avg, current->sdv);
        current = next(current);
    }
    fclose(fout);

    while ( head != NULL ) {
        record_t* temp = head;
        head = next(head);
        free(temp);
    }

    return 0;
}