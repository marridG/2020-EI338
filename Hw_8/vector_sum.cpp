//
// Created by GuoZL on 2020-11-09.
//

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;


// #define SHOW_ASGN_THD_GE_MAX_THD
// #define DEBUG
#define LATEX

#ifdef DEBUG
const int MAX_NUMBER_CNT = 100;  // sqrt(LONG_LONG_MAX) = 3.037e+09 (INT_MAX=2.147e+09)
#else
const int MAX_NUMBER_CNT = 2000000000;  // sqrt(LONG_LONG_MAX) = 3.037e+09 (INT_MAX=2.147e+09)
#endif
const int NUMBER_STEP_RATIO = 2;
const int MAX_THREAD_CNT = 10;

#ifdef LATEX
const int LATEX_COL_CNT = 2;
#endif

vector<int> numbers = {};
long long correct_sum = 0;


struct calculation_report {
    unsigned long long number_count = -1;
    int thread_count = -1;    // -1=sequential, otherwise (positive) is parallel
    bool correct = false;
    long long sum = 0;
    double time_elapsed = 0.0;

    calculation_report &operator=(calculation_report const &right_item) = default;
    // calculation_report &operator=(calculation_report const &right_item) {
    //     this->correct = right_item.correct;
    //     sum = right_item.sum;
    //     time_elapsed = right_item.time_elapsed;
    //     return *this;
    // }
};

struct execution_report {
    bool all_correct = false;
    int number_count = 0;
    double parallel_time = 0.0;
    double sequential_time = 0.0;
    double acceleration = 0.0;  // in percentage

    execution_report &operator=(execution_report const &right_item) = default;
};


calculation_report parallel_vector_sum() {
    double start_time = 0.0, end_time = 0.0, time_elapsed = 0.0;
    long long sum = 0;
    int thread_count = omp_get_num_threads();

    start_time = omp_get_wtime();
#ifdef SHOW_ASGN_THD_GE_MAX_THD
# pragma omp parallel num_threads(thread_count)
    {
        int thread_id = omp_get_thread_num();
        printf("Hello from thread %d of %d\n", thread_id, thread_count);
    }
#else
# pragma omp parallel for num_threads(thread_count) reduction (+:sum)
    for (int i = 0; i <= numbers.size() - 1; i++) {
        sum += numbers[i];
    }
#endif

    end_time = omp_get_wtime();
    time_elapsed = end_time - start_time;
    calculation_report report = {numbers.size(), thread_count, sum == correct_sum, sum, time_elapsed};

    return report;
}

calculation_report sequential_vector_sum() {
    clock_t start_clock, finish_clock;
    double time_elapsed = 0.0;
    long long sum = 0;

    start_clock = clock();
    for (int i = 0; i <= numbers.size() - 1; i++) {
        sum += numbers[i];
    }
    finish_clock = clock();

    time_elapsed = (double) (finish_clock - start_clock) / CLOCKS_PER_SEC;;
    calculation_report report = {numbers.size(), -1, sum == correct_sum, sum, time_elapsed};

    return report;
}


execution_report execute(int number_cnt) {
    cout << "=======================================================" << endl;

    // Initialization
    numbers.clear();
    correct_sum = 0;
    for (int i = 1; i <= number_cnt; i++) {
        numbers.push_back(i);
        correct_sum += i;
    }
    printf("%d Numbers Initiated (1, 2, 3, ... %d)\n", number_cnt, number_cnt);
    printf("\tSum = %lld\n\n", correct_sum);

    calculation_report temp_prl_report;
    vector<calculation_report> results_parallel_report = {};
    calculation_report results_sequential_report;
    bool all_correct = true;

    // Parallel Approach
    for (int th_cnt = 0; th_cnt <= MAX_THREAD_CNT; th_cnt++) {
        printf("Using %d Thread(s)\n", th_cnt);
        temp_prl_report = parallel_vector_sum();
        results_parallel_report.push_back(temp_prl_report);
        printf("\tCorrect = %d\tSum = %lld\tTime Elapsed = %f\n",
               temp_prl_report.correct, temp_prl_report.sum, temp_prl_report.time_elapsed);
        if (!temp_prl_report.correct) all_correct = false;
    }

    printf("\n");

    //Sequential Approach
    printf("Using Sequential Approach\n");
    results_sequential_report = sequential_vector_sum();
    printf("\tCorrect = %d\tSum = %lld\tTime Elapsed = %f\n",
           results_sequential_report.correct, results_sequential_report.sum,
           results_sequential_report.time_elapsed);
    if (!results_sequential_report.correct) all_correct = false;

    cout << "=======================================================\n\n" << endl;

    execution_report report = {all_correct, number_cnt,
                               results_parallel_report.front().time_elapsed,
                               results_sequential_report.time_elapsed,
                               results_parallel_report.front().time_elapsed /
                               results_sequential_report.time_elapsed * 100.};
    return report;
}


int main() {
    execution_report exec_report;
    vector<execution_report> reports = {};
    // for (int num_cnt = 1; num_cnt <= MAX_NUMBER_CNT; num_cnt *= 10) {
    for (int num_cnt = 1; num_cnt <= MAX_NUMBER_CNT; num_cnt *= NUMBER_STEP_RATIO) {
        exec_report = execute(num_cnt);
        reports.push_back(exec_report);
        if (INT_MAX - num_cnt < num_cnt)
            break;
    }

    printf("\n\n\n===========\nREPORT\n===========\n\n");
    printf("Array Length\tAll Correct\tParallel\tSequential\tAcceleration\n");
    for (int i = 0; i <= reports.size() - 1; i++) {
        printf("%d\t%d\t%f\t%f\t%f\n", reports[i].number_count, reports[i].all_correct,
               reports[i].parallel_time, reports[i].sequential_time, reports[i].acceleration);
    }

#ifdef LATEX
    printf("\n\n\n===========\nLATEX CODES\n===========\n\n");

    printf("\\begin{table}[H]\n"
           "\t\\centering\n"
           "\t\\begin{tabular}{cccc");
    for (int i = 2; i <= LATEX_COL_CNT; i++)
        printf("||cccc");
    printf("}\n");

    printf("\t\\toprule\n\n"
           "\t\\textbf{Length} & \\textbf{Parallel}  & \\textbf{Sequential} & \\textbf{Accl.}\n");
    for (int i = 2; i <= LATEX_COL_CNT; i++)
        printf("\t & "
               "\\textbf{Length} & \\textbf{Parallel}  & \\textbf{Sequential} & \\textbf{Accl.}\n");
    printf("\t\\\\ \n");

    printf("\t\\midrule\n\n");

    int report_idx = 0;
    while (report_idx <= reports.size() - 1) {
        printf("\t%d & %f & %f & %f\n", reports[report_idx].number_count,
               reports[report_idx].parallel_time, reports[report_idx].sequential_time,
               reports[report_idx].acceleration);
        report_idx++;

        for (int i = 1; i <= LATEX_COL_CNT - 1; i++) {
            printf("\t &  %d & %f & %f & %f\n", reports[report_idx].number_count,
                   reports[report_idx].parallel_time, reports[report_idx].sequential_time,
                   reports[report_idx].acceleration);
            report_idx++;
        }
        printf("\t\\\\ \n");
    }
    printf("\t\\bottomrule\n");
    printf("\t\\end{tabular}\n");
    printf("\\end{table}");
    printf("\n\n");
#endif

    return 0;
}
