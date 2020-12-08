#include <bits/stdc++.h>

using namespace std;


#define DEBUG


struct MEMORY_BLOCK {
    size_t start = -1, end = -1, size = -1;
    int used = -1;
    char *label;
    struct MEMORY_BLOCK *prev = NULL, *next = NULL;
};
size_t MEMORY_SIZE = 0;
MEMORY_BLOCK *MEMORY_ALL;
const int MAX_LABEL_LEN = 10;

MEMORY_BLOCK *create_new_block(size_t start, size_t end, const char *label, int used,
                               MEMORY_BLOCK *prev, MEMORY_BLOCK *next);
int init(int argc, char *argv[]);
void output_values();
MEMORY_BLOCK *find_best_hole(size_t size, char mode);
int request_memory(const char *label, size_t size, char mode);
int release_memory(const char *label);
int compact();


int main(int argc, char *argv[]) {

#ifdef DEBUG
    argc = 2;
    char arg1[18] = "./mem_alloc";
    // char arg2[8] = "1048576";
    char arg2[4] = "100";
    argv[0] = arg1;
    argv[1] = arg2;
#endif

    if (0 != init(argc, argv)) return 0;
    printf("allocator>");

    char op[5] = "";                            // commands: RQ, RL, C, STAT, X
    int op_success = -1;                        // operation succeeds: 0=×,1=√,-1=?
    while (1 == scanf("%s", op)) {
        op_success = -1;
        char label[MAX_LABEL_LEN];              // operand label
        size_t size = -1;                       // operand target memory size
        char mode;                              // operand resources allocation request mode

        if (strcmp(op, "RQ") == 0) {            // request memory
            scanf("%s %zu %c", label, &size, &mode);
            op_success = (0 == request_memory(label, size, mode));
        }

        else if (strcmp(op, "RL") == 0) {       // release memory
            scanf("%s", label);
            op_success = (0 == release_memory(label));
        }

        else if (strcmp(op, "STAT") == 0) {     // output memory usage status
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

    sscanf(argv[1], "%zu", &MEMORY_SIZE);
    MEMORY_ALL = create_new_block(0, MEMORY_SIZE - 1,
                                  "", 0, NULL, NULL);

    printf("[INFO] Initialized %zu Bytes.\n", MEMORY_SIZE);
    return 0;
}


/*!
 * React when Processes Request to Allocate Memory, in a Certain Mode
 * @param label             label of the requesting process
 * @param size              size of the requested memory allocation
 * @param mode              allocation mode, supports:
 *                              F: first fit
 *                              B: best fit, allocate in the minimum (&first if multiple) hole
 *                              W: worst fit, allocate in the maximum (&first if multiple) hole
 * @return                  0 if success; negative otherwise
 */
int request_memory(const char *label, size_t size, char mode) {
    if (size > MEMORY_SIZE) {
        printf("[Error] Request Memory Exceeds Maximum Memory. ");
        return -1;
    }
    MEMORY_BLOCK *hole = find_best_hole(size, mode);
    if (NULL == hole) {
        printf("[Error] No Available Memory Holes. ");
        return -2;
    }

    hole->label = new char[sizeof(char) * (strlen(label) + 1)];
    strcpy(hole->label, label);
    hole->used = 1;

    // split the remaining memory if exists
    if (size != hole->size) {
        hole->next = create_new_block(hole->start + size, hole->end,
                                      "", 0, hole, hole->next);
        hole->end = hole->start + size - 1;
        hole->size = size;
    }
#ifdef DEBUG
    output_values();
#endif
    return 0;
}

/*!
 * React when Processes Release Memory Allocation
 * @param label             label of the to-release process
 * @return                  0 if success; negative otherwise
 */
int release_memory(const char *label) {
    MEMORY_BLOCK *crt_block = MEMORY_ALL;
    int target_found = 0;
    while (NULL != crt_block) {
        // target NOT found
        if (0 == target_found) {
            // the current is NOT the target (unused or !label)
            if (0 == crt_block->used || 0 != strcmp(crt_block->label, label)) {
                crt_block = crt_block->next;
                continue;
            }
                // the current IS the target
            else if (1 == crt_block->used && 0 == strcmp(crt_block->label, label)) {
                target_found = 1;
                delete[] crt_block->label;
                crt_block->label = NULL;
                crt_block->used = 0;
            }
                // internel error
            else {
                printf("[Error] Internal Error. "
                       "Possibly Unused Blocks have Labels.\n");
                exit(-3);
            }

        }
        // target IS found (and thus updated)
        //      possibly merge the current unused block with the previous unused block
        if (NULL != crt_block->prev && 0 == crt_block->prev->used && 0 == crt_block->used) {
            MEMORY_BLOCK *crt_block_prev = crt_block->prev;
            crt_block->prev = crt_block_prev->prev;
            if (NULL != crt_block_prev->prev) {
                crt_block_prev->prev->next = crt_block;
            }
            crt_block->start = crt_block_prev->start;
            delete crt_block_prev;
        }
        // handle the case that the head is deleted
        //      happens when the head is released & head->next is unused
        if (NULL == crt_block->prev) {
            MEMORY_ALL = crt_block;
        }

        crt_block = crt_block->next;
    }

    if (0 == target_found) {                    // the target process label is not found
        printf("[Error] Invalid Process Label. ");
        return -1;
    }
#ifdef DEBUG
    output_values();
#endif
    return 0;
}

int compact() {
    return 1;
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
MEMORY_BLOCK *create_new_block(size_t start, size_t end, const char *label, int used,
                               MEMORY_BLOCK *prev, MEMORY_BLOCK *next) {
    MEMORY_BLOCK *new_block = new MEMORY_BLOCK;
    if (NULL == new_block) {
        printf("[Error] Failed to Create New Memory Block Representation.\n");
        exit(-1);
    }

    // assign values of the features of the new memory block
    new_block->start = start;
    new_block->end = end;
    new_block->size = end - start + 1;
    if (0 == strlen(label) && 0 == used) {      // unused block
        new_block->label = NULL;
        new_block->used = 0;
    }
    else if (0 != strlen(label) && 1 == used) { // used blocks
        new_block->label = new char[sizeof(char) * (strlen(label) + 1)];
        strcpy(new_block->label, label);
        new_block->used = 1;
    }
    else {                                      // invalid conditions
        delete new_block;
        printf("[Error] Used & StrLen Mismatch.\n");
        exit(-2);
    }
    // insert and link the new block
    new_block->prev = prev;
    new_block->next = next;
    if (NULL != prev) { prev->next = new_block; }
    if (NULL != next) { next->prev = new_block; }

    return new_block;
}

/*!
 * Find the Best Hole to Allocate, under various modes
 * @param size              size of the requested memory allocation
 * @param mode              allocation mode, supports:
 *                              F: first fit
 *                              B: best fit, allocate in the minimum (&first if multiple) hole
 *                              W: worst fit, allocate in the maximum (&first if multiple) hole
 * @return                  pointer to the the best hole if found; NULL otherwise
 */
MEMORY_BLOCK *find_best_hole(size_t size, char mode) {
    MEMORY_BLOCK *crt_block = MEMORY_ALL;
    MEMORY_BLOCK *best_block = NULL;            // pointer to the best block under various modes
    size_t min_size = MEMORY_SIZE;              // size of the best block under BEST mode
    size_t max_size = 0;                        // size of the best block under WORST mode
    while (NULL != crt_block) {
        if (0 != crt_block->used || crt_block->size < size) {
            crt_block = crt_block->next;
            continue;
        }

        switch (mode) {
            case 'F':                           // first fit
                return crt_block;
            case 'B': {                         // best fit
                if (crt_block->size < min_size) {
                    best_block = crt_block;
                    min_size = crt_block->size;
                }
                break;
            }
            case 'W': {                         // worst fit
                if (crt_block->size > max_size) {
                    best_block = crt_block;
                    max_size = crt_block->size;
                }
                break;
            }
            default: {
                printf("[Error] Unknown Mode %c.\n", mode);
                return NULL;
            }
        }
        crt_block = crt_block->next;
    }

    return best_block;
}


/*!
 * Output Values: Memory Holes Usage Status
 */
void output_values() {
    string indent = "    ";
    printf("==============================\n");
    printf("Memory Report\n");
    printf("==============================\n");

    MEMORY_BLOCK *crt_block = MEMORY_ALL;
    while (NULL != crt_block) {
        printf("%sAddress(es) [%07zu - %07zu] (%07zu)",
               indent.c_str(), crt_block->start, crt_block->end, crt_block->size);
        if (0 == crt_block->used) {             // unused blocks
            printf("  =>  Unused\n");
        }
        else {                                  // used blocks
            printf("  =>  Process %s\n", crt_block->label);
        }
        crt_block = crt_block->next;
    }

    printf("==============================\n");
    printf("==============================\n");
}
