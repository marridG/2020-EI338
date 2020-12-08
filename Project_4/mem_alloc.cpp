#include <bits/stdc++.h>

using namespace std;


#define DEBUG


struct memory_block {
    size_t start = -1, end = -1;
    char *label;
    struct memory_block *prev, *next;
};
size_t memory_size = 0;
memory_block *memory_all;


memory_block *create_new_block(size_t start, size_t end, const char *label,
                               memory_block *prev, memory_block *next);
int init(int argc, char *argv[]);
void output_values();
int is_leq(const int *opr_1, const int *opr_2, int len);
int state_is_safe();
int request_memory(const char *label, size_t size, char mode);
int release_resources(int customer_idx, const int *release);


int main(int argc, char *argv[]) {

#ifdef DEBUG
    argc = 2;
    char arg1[18] = "./mem_alloc";
    char arg2[8] = "1048576";
    argv[0] = arg1;
    argv[1] = arg2;
#endif

    if (0 != init(argc, argv)) return 0;
    printf("allocator>");

    char op[5] = "";                            // commands: RQ, RL, C, STAT, X
    int op_success = -1;                        // operation succeeds: 0=×,1=√,-1=?
    while (1 == scanf("%s", op)) {
        op_success = -1;
        char label[10];                         // operand label
        size_t size = -1;                       // operand target memory size
        char mode[2];                           // operand resources allocation request mode

        if (strcmp(op, "RQ") == 0) {            // request resources
            scanf("%s %zu %c", label, &size, &mode);
            // op_success = (0 == request_resources(customer_idx, request));
        }

        else if (strcmp(op, "RL") == 0) {       // release resources
            // scanf("%d", &customer_idx);
            // for (int i = 0; i <= NUM_RESOURCES - 1; ++i) {
            //     scanf("%d", &request[i]);
            // }
            // op_success = (0 == release_resources(customer_idx, request));
        }

        else if (strcmp(op, "STAT") == 0) {        // output memory usage status
            output_values();
            op_success = -1;
        }
        else if (strcmp(op, "X") == 0) {        // exit
            break;
        }
        else {                                  // invalid operation identifier
            printf("[Error] Invalid Operation Identifier.\n");
            op_success = -1;
        }

        if (1 == op_success) printf("SUCCESS.\n");
        else if (0 == op_success) printf("DENIED.\n");
        printf("allocator>");
    }

    return 0;
}

/*!
 * Create a New Memory Block Representation Object
 * @param start             start index of the new memory block
 * @param end               end index of the new memory block
 * @param label             label of the new memory block
 * @param prev              pointer of the previous memory block
 * @param next              pointer of the next memory block
 * @return                  newly created memory block
 */
memory_block *create_new_block(size_t start, size_t end, const char *label,
                               memory_block *prev, memory_block *next) {
    memory_block *new_block = new memory_block;
    if (NULL == new_block) {
        printf("[Error] Failed to Create New Memory Block Representation.\n");
        exit(-1);
    }

    // assign values of the features of the new memory block
    // start & end indices in the memory block
    new_block->start = start;
    new_block->end = end;
    // (un)used block' label
    if (0 == strlen(label)) {         // unused block
        new_block->label = NULL;
    }
    else {                          // used block
        new_block->label = new char[sizeof(char) * (strlen(label) + 1)];
        strcpy(new_block->label, label);
    }
    // insert and link the new block
    new_block->prev = prev;
    new_block->next = next;
    if (NULL != prev) { prev->next = new_block; }
    if (NULL != next) { next->prev = new_block; }

    return new_block;
}


/*!
 * Init: Parse & Store Input AVAILABLE Values, Read MAX from File
 * @param argc              number of arguments
 * @param argv              arguments
 * @param resources_file    file in which MAX is stored
 * @return                  0 if success; negative otherwise
 */
int init(int argc, char *argv[]) {
    if (2 != argc) {
        printf("[Error] Incorrect Number of Initialization Parameters.\n");
        return -1;
    }

    sscanf(argv[1], "%zu", &memory_size);
    memory_all = create_new_block(0, memory_size - 1, "", NULL, NULL);

    if (NULL == memory_all) {
        printf("[Error] Memory Block Initialization Failed.\n");
        return -2;
    }
    printf("[INFO] Initialized %zu Bytes.\n", memory_size);
    return 0;
}

int request_memory(const char *label, size_t size, char mode) {
    //     if (customer_idx < 0 || customer_idx > NUM_CUSTOMERS - 1) {
    //         printf("[Error] Invalid Customer Index. ");
    //         return -1;
    //     }
    //     if (0 == is_leq(request, need[customer_idx], NUM_RESOURCES)) {
    //         printf("[Error] Maximum Claim Exceeded. ");
    //         return -2;
    //     }
    //     if (0 == is_leq(request, available, NUM_RESOURCES)) {
    //         printf("[Error] Insufficient Available Resources. ");
    //         return -3;
    //     }
    //
    // #ifdef DEBUG
    //     output_values();
    // #endif
    //     // try to allocate
    //     for (int r = 0; r <= NUM_RESOURCES - 1; r++) {
    //         available[r] -= request[r];
    //         allocation[customer_idx][r] += request[r];
    //         need[customer_idx][r] -= request[r];
    //     }
    //     int success = (1 == state_is_safe());
    //     if (1 == success) { return 0; }
    //     // if unsafe, restore the unsafe allocation
    // #ifdef DEBUG
    //     output_values();
    // #endif
    //     for (int r = 0; r <= NUM_RESOURCES - 1; r++) {
    //         available[r] += request[r];
    //         allocation[customer_idx][r] -= request[r];
    //         need[customer_idx][r] += request[r];
    //     }
    //     printf("[Error] Allocation Unsafe. ");
    //     return -4;
}

/*!
 * React when Processes Release Resources
 * @param customer_idx      release request customer index
 * @param release           requested released resources amount
 * @return                  0 if success; negative otherwise
 */
int release_resources(int customer_idx, const int *release) {
    //     if (customer_idx < 0 || customer_idx > NUM_CUSTOMERS - 1) {
    //         printf("[Error] Invalid Customer Index. ");
    //         return -1;
    //     }
    //     if (0 == is_leq(release, allocation[customer_idx], NUM_RESOURCES)) {
    //         printf("[Error] Alloaction Exceeded. ");
    //         return -2;
    //     }
    //
    // #ifdef DEBUG
    //     output_values();
    // #endif
    //     // release
    //     for (int r = 0; r <= NUM_RESOURCES - 1; r++) {
    //         available[r] += release[r];
    //         allocation[customer_idx][r] -= release[r];
    //     }
    // #ifdef DEBUG
    //     output_values();
    // #endif
    //
    //     return 0;
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
 * @return                  1 if safe; 0 if unsafe
 */
int state_is_safe() {
    // int work[NUM_RESOURCES], finish[NUM_CUSTOMERS];
    // memcpy(work, available, NUM_RESOURCES * sizeof(int));
    // memset(finish, 0, NUM_CUSTOMERS * sizeof(int));
    //
    // int flag_found = 0;
    // while (1) {
    //     flag_found = 0;
    //     for (int i = 0; i <= NUM_CUSTOMERS - 1; i++) {
    //         if (0 == finish[i] && 1 == is_leq(need[i], work, NUM_RESOURCES)) {
    //             flag_found = 1;
    //             for (int j = 0; j <= NUM_RESOURCES - 1; j++) {
    //                 work[j] += allocation[i][j];
    //             }
    //             finish[i] = 1;
    //             break;
    //         }
    //     }
    //     if (0 == flag_found) { break; }
    // }
    // if (0 == flag_found) {
    //     int is_all_true = 1;
    //     for (int i = 0; i <= NUM_CUSTOMERS - 1; i++) {
    //         if (1 != finish[i]) {
    //             is_all_true = 0;
    //             break;
    //         }
    //     }
    //     if (1 == is_all_true) { return 1; }
    // }
    //
    // return 0;
}


/*!
 * Output Values: Memory Holes Usage Status
 */
void output_values() {
    string indent = "    ";
    printf("==============================\n");
    printf("Memory Report\n");
    printf("==============================\n");

    memory_block *crt_block = memory_all;
    while (NULL != crt_block) {
        printf("%sAddress(es) [%07zu - %07zu]  =>  ",
               indent.c_str(), crt_block->start, crt_block->end);
        if (NULL == crt_block->label) {          // unused block
            printf("Unused\n");
        }
        else {                          // used block
            printf("Process %s\n", crt_block->label);
        }
        crt_block = crt_block->next;
    }

    printf("==============================\n");
    printf("==============================\n");
}
