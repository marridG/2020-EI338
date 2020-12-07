

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

/*!
 * Init: Parse & Store Input AVAILABLE Values, Read MAX from File
 * @param argc              number of arguments
 * @param argv              arguments
 * @param resources_file    file in which MAX is stored
 * @return                  0 if success; egative otherwise
 */
int init(int argc, char *argv[], const char *resources_file) {
    if (1 + NUM_RESOURCES != argc) {
        printf("Incorrect number of parameters.\n");
        return -1;
    }

    // store the input values of AVAILABLE
    for (int i = 0; i <= NUM_RESOURCES - 1; i++) {
        available[i] = atoi(argv[i + 1]);
    }

    // get MAX values from the init file
    FILE *f = fopen(resources_file, "r");
    if (nullptr == f) {
        printf("Unable to open file: %s\n", resources_file);
        return -2;
    }
    for (int c = 0; c < NUM_CUSTOMERS; c++) {
        for (int r = 0; r <= NUM_RESOURCES - 1; r++) {
            fscanf(f, "%d", &maximum[c][r]);
            need[c][r] = maximum[c][r];
        }
    }
    fclose(f);

    printf("Initialized, ready to run.\n");
    return 0;
}


/*!
 * Judge whether list A <= B Element-wise
 * @param opr_1             list A
 * @param opr_2             list B
 * @param len               length of the list (len(A)=len(B))
 * @return                  1 if (A<=B).all(); 0 otherwise
 */
int is_leq(const int *opr_1, const int *opr_2, int len) {
    for (int i = 0; i <= len - 1; i++) {
        if (opr_1[i] > opr_2[i]) {
            return 0;
        }
    }
    return 1;
}

/*!
 * Judge by Safety Algorithm whether a State is Safe
 * @param in_avail          input AVAILABLE values
 * @param in_alloc          input ALLOCATION values
 * @param in_need           input NEED values
 * @return                  1 if safe; 0 if unsafe
 */
int state_is_safe(int *in_avail, int *in_alloc, int *in_need) {
    int work[NUM_RESOURCES], finish[NUM_CUSTOMERS];
    memcpy(work, available, NUM_RESOURCES * sizeof(int));
    memset(finish, 0, NUM_CUSTOMERS * sizeof(int));

    for (int finished_cnt = 0; finished_cnt <= NUM_CUSTOMERS - 1; finished_cnt++) {
        int flag_found = 0;
        for (int i = 0; i <= NUM_CUSTOMERS - 1; i++) {
            if (!finish[i] && is_leq(need[i], work, NUM_RESOURCES)) {
                flag_found = 1;
                for (int j = 0; j <= NUM_RESOURCES - 1; j++) {
                    work[j] += allocation[i][j];
                }
                finish[i] = 1;
                break;
            }
        }
        if (!flag_found) {
            return 0;
        }
    }
    return 1;
}

/*!
 * Output Values: AVAILABLE, MAX, ALLOCATION, NEED
 */
void output_values() {
    string indent = "    ";
    string end_of_field = "";
    printf("==============================\n");
    printf("Values Report\n");
    printf("==============================\n%s", end_of_field.c_str());

    // AVAILABLE: Available Resources
    printf("AVAILABLE - Shape (%d,)\n", NUM_RESOURCES);
    printf("%s", indent.c_str());
    for (int i = 0; i <= NUM_RESOURCES - 1; i++) {
        printf("%d ", available[i]);
    }
    printf("\n%s", end_of_field.c_str());

    // MAX: Maximum Resources for Each Customer
    printf("MAX - Shape (%d, %d)\n", NUM_CUSTOMERS, NUM_RESOURCES);
    for (int c = 0; c <= NUM_CUSTOMERS - 1; c++) {
        printf("%s", indent.c_str(), c);
        for (int r = 0; r <= NUM_RESOURCES - 1; r++) {
            printf("%d ", maximum[c][r]);
        }
        printf("\n");
    }
    printf("%s", end_of_field.c_str());

    // ALLOCATION: Allocated Resources for Each Customer
    printf("ALLOCATION - Shape (%d, %d)\n", NUM_CUSTOMERS, NUM_RESOURCES);
    for (int c = 0; c <= NUM_CUSTOMERS - 1; c++) {
        printf("%s", indent.c_str(), c);
        for (int r = 0; r <= NUM_RESOURCES - 1; r++) {
            printf("%d ", allocation[c][r]);
        }
        printf("\n");
    }
    printf("%s", end_of_field.c_str());

    // NEED: Needed Resources for Each Customer
    printf("NEED - Shape (%d, %d)\n", NUM_CUSTOMERS, NUM_RESOURCES);
    for (int c = 0; c <= NUM_CUSTOMERS - 1; c++) {
        printf("%s", indent.c_str(), c);
        for (int r = 0; r <= NUM_RESOURCES - 1; r++) {
            printf("%d ", need[c][r]);
        }
        printf("\n");
    }
    printf("%s", end_of_field.c_str());

    printf("==============================\n");
    printf("==============================\n");
}
