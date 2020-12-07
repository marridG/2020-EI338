

#include <bits/stdc++.h>

using namespace std;


#define DEBUG


const int NUM_CUSTOMERS = 5;
const int NUM_RESOURCES = 4;
char INIT_FILENAME[100] = "./init.txt";         // initialization input filename, as MAX matrix

int available[NUM_RESOURCES];                   // available amount of each resource
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];      // maximum demand of each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];   // amount currently allocated to each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES];         // remaining need of each customer


int init(int argc, char *argv[], const char *resources_file);
void output_values();
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);


int main(int argc, char *argv[]) {

#ifdef DEBUG
    strcpy(INIT_FILENAME, "../Project_3/init.txt");
    argc = 5;
    char arg1[18] = "bankers_algorithm";
    char arg2[3] = "10", arg3[2] = "5", arg4[2] = "7", arg5[2] = "8";
    argv[0] = arg1;
    argv[1] = arg2;
    argv[2] = arg3;
    argv[3] = arg4;
    argv[4] = arg5;
#endif

    if (0 != init(argc, argv, INIT_FILENAME))
        return 0;
    printf(">");

    char op[5] = "";// commands: RQ, RL, * (STAT in proj 4)
    while (1 == scanf("%s", op)) {
        if (strcmp(op, "RQ") == 0) {
            // request_wrapper();
        }
        else if (strcmp(op, "RL") == 0) {
            // release_wrapper();
        }
        else if (strcmp(op, "*") == 0) {
            output_values();
        }
        else {
            // display_usage();
        }
        printf("> ");
    }

    return 0;
}

int init(int argc, char *argv[], const char *resources_file) {
    if (1 + NUM_RESOURCES != argc) {
        printf("Incorrect number of parameters.\n");
        return -1;
    }

    // store the input values of AVAILABLE
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] = atoi(argv[i + 1]);
    }

    // get MAX values from the init file
    FILE *f = fopen(resources_file, "r");
    if (nullptr == f) {
        printf("Unable to open file: %s\n", resources_file);
        return -2;
    }
    for (int c = 0; c < NUM_CUSTOMERS; c++) {
        for (int r = 0; r < NUM_RESOURCES; r++) {
            fscanf(f, "%d", &maximum[c][r]);
            need[c][r] = maximum[c][r];
        }
    }
    fclose(f);

    printf("Initialized, ready to run.\n");
    return 0;
}


void output_values() {
    string indent = "    ";
    string end_of_field = "";
    printf("==============================\n");
    printf("Values Report\n");
    printf("==============================\n%s", end_of_field.c_str());

    // AVAILABLE: Available Resources
    printf("AVAILABLE - Shape (%d,)\n", NUM_RESOURCES);
    printf("%s", indent.c_str());
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    printf("\n%s", end_of_field.c_str());

    // MAX: Maximum Resources for Each Customer
    printf("MAX - Shape (%d, %d)\n", NUM_CUSTOMERS, NUM_RESOURCES);
    for (int c = 0; c < NUM_CUSTOMERS; c++) {
        printf("%s", indent.c_str(), c);
        for (int r = 0; r < NUM_RESOURCES; r++) {
            printf("%d ", maximum[c][r]);
        }
        printf("\n");
    }
    printf("%s", end_of_field.c_str());

    // ALLOCATION: Allocated Resources for Each Customer
    printf("ALLOCATION - Shape (%d, %d)\n", NUM_CUSTOMERS, NUM_RESOURCES);
    for (int c = 0; c != NUM_CUSTOMERS; c++) {
        printf("%s", indent.c_str(), c);
        for (int r = 0; r < NUM_RESOURCES; r++) {
            printf("%d ", allocation[c][r]);
        }
        printf("\n");
    }
    printf("%s", end_of_field.c_str());

    // NEED: Needed Resources for Each Customer
    printf("NEED - Shape (%d, %d)\n", NUM_CUSTOMERS, NUM_RESOURCES);
    for (int c = 0; c < NUM_CUSTOMERS; c++) {
        printf("%s", indent.c_str(), c);
        for (int r = 0; r < NUM_RESOURCES; r++) {
            printf("%d ", need[c][r]);
        }
        printf("\n");
    }
    printf("%s", end_of_field.c_str());

    printf("==============================\n");
    printf("==============================\n");
}
