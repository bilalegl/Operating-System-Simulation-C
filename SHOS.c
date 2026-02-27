#include <stdio.h>
#include <string.h>

#define MAX 10
#define TOTAL_MEMORY 100
#define PARTITION_SIZE 20
#define TOTAL_PARTITIONS (TOTAL_MEMORY / PARTITION_SIZE)
#define MAX_GANTT 50
#define FRAMES 3
#define PAGES 10
#define MAX_FILES 10
#define ENC_KEY 0xAA

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"
#define BOLD    "\033[1m"

// Background Colors
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

int totalWaiting = 0;
int totalTurnaround = 0;
int pageFaults = 0;
int pageHits = 0;
int securityViolations = 0;

struct SecureFile {
    char filename[30];
    char data[200];
    int ownerPID;
};

struct SecureFile vault[MAX_FILES];
int fileCount = 0;

void encryptDecrypt(char *data) {
    for (int i = 0; data[i] != '\0'; i++)
        data[i] ^= ENC_KEY;
}

void logAccess(char *event, char *file) {
    if (strcmp(event, "Unauthorized Access Attempt") == 0) {
        printf(RED "[SECURITY LOG] %s on %s\n" RESET, event, file);
        securityViolations++;
    } else {
        printf(GREEN "[SECURITY LOG] %s on %s\n" RESET, event, file);
    }
}

void createFile(char *name, char *content, int pid) {
    if (fileCount >= MAX_FILES) {
        printf(RED "Vault Full!\n" RESET);
        return;
    }

    strcpy(vault[fileCount].filename, name);
    strcpy(vault[fileCount].data, content);
    encryptDecrypt(vault[fileCount].data);
    vault[fileCount].ownerPID = pid;

    logAccess("File Created", name);
    fileCount++;
}

void readFile(char *name, int pid) {
    for (int i = 0; i < fileCount; i++) {
        if (strcmp(vault[i].filename, name) == 0) {
            if (vault[i].ownerPID != pid) {
                logAccess("Unauthorized Access Attempt", name);
                return;
            }

            encryptDecrypt(vault[i].data);
            printf(CYAN "Reading File %s: " YELLOW "%s\n" RESET, name, vault[i].data);
            encryptDecrypt(vault[i].data);

            logAccess("File Read", name);
            return;
        }
    }
    printf(RED "File Not Found\n" RESET);
}

int frame[FRAMES];
int fifoIndex = 0;

void initPaging() {
    for (int i = 0; i < FRAMES; i++)
        frame[i] = -1;
}

int isPageHit(int page) {
    for (int i = 0; i < FRAMES; i++)
        if (frame[i] == page)
            return 1;
    return 0;
}

void replacePageFIFO(int page) {
    frame[fifoIndex] = page;
    fifoIndex = (fifoIndex + 1) % FRAMES;
}

void simulatePaging() {
    int referenceString[] = {1, 3, 0, 3, 5, 6, 3};
    int size = sizeof(referenceString) / sizeof(int);

    printf(BOLD CYAN "\n[ PAGING & VIRTUAL MEMORY ]\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);

    for (int i = 0; i < size; i++) {
        int page = referenceString[i];
        printf("Request Page %d -> ", page);

        if (isPageHit(page)) {
            pageHits++;
            printf(GREEN "HIT\n" RESET);
        } else {
            pageFaults++;
            replacePageFIFO(page);
            printf(RED "FAULT" RESET " | Frames: ");
            for (int j = 0; j < FRAMES; j++) {
                if (frame[j] == -1)
                    printf(MAGENTA "%d " RESET, frame[j]);
                else
                    printf(GREEN "%d " RESET, frame[j]);
            }
            printf("\n");
        }
    }
}

void displayPagingReport() {
    printf(BOLD CYAN "\n[ PAGING STATISTICS ]\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);
    printf("Total Page Hits   : " GREEN "%d\n" RESET, pageHits);
    printf("Total Page Faults : " RED "%d\n" RESET, pageFaults);
}

int ganttPID[MAX_GANTT];
int ganttTime[MAX_GANTT];
int ganttCount = 0;

void addGantt(int pid, int time) {
    ganttPID[ganttCount] = pid;
    ganttTime[ganttCount] = time;
    ganttCount++;
}

int memory[TOTAL_PARTITIONS];   // 0 = free, PID = occupied

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int priority;
    int waiting;
    int turnaround;
    int type;   // 1 = High Priority, 2 = Normal
};

struct Process processes[MAX] = {
    {1, 0, 5, 5, 1, 0, 0, 1}, // pid, arrival, burst, remaining, priority, waiting, tat, type
    {2, 1, 3, 3, 1, 0, 0, 1},
    {3, 2, 8, 8, 2, 0, 0, 2},
    {4, 3, 6, 6, 2, 0, 0, 2}
};

int n = 4;

void initMemory() {
    for (int i = 0; i < TOTAL_PARTITIONS; i++)
        memory[i] = 0;
}

void allocateMemory() {
    printf(BOLD CYAN "\n[ MEMORY ALLOCATION STATUS ]\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);

    for (int i = 0; i < n; i++) {
        int allocated = 0;

        for (int j = 0; j < TOTAL_PARTITIONS; j++) {
            if (memory[j] == 0) {
                memory[j] = processes[i].pid;
                printf(GREEN "Process %d allocated to Partition %d\n" RESET,
                       processes[i].pid, j);
                allocated = 1;
                break;
            }
        }

        if (!allocated) {
            printf(YELLOW "Process %d waiting (No free partition)\n" RESET,
                   processes[i].pid);
        }
    }
}

void displayMemoryStatus() {
    int used = 0;

    printf(BOLD CYAN "\n[ MEMORY STATUS REPORT ]\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);

    for (int i = 0; i < TOTAL_PARTITIONS; i++) {
        if (memory[i] != 0) {
            printf(GREEN "Partition %d → Process %d\n" RESET, i, memory[i]);
            used++;
        } else {
            printf(MAGENTA "Partition %d → Free\n" RESET, i);
        }
    }

    printf("\nTotal Partitions : " CYAN "%d\n" RESET, TOTAL_PARTITIONS);
    printf("Used Partitions  : " GREEN "%d\n" RESET, used);
    printf("Free Partitions  : " MAGENTA "%d\n" RESET,
           TOTAL_PARTITIONS - used);
}

void priorityScheduling() {
    int i, j;
    struct Process temp;

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (processes[i].type == 1 && processes[j].type == 1 &&
                processes[i].priority < processes[j].priority) {

                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void applyAging() {
    for (int i = 0; i < n; i++) {
        if (processes[i].type == 1) {
            processes[i].priority++;
            printf(YELLOW "Aging: Process %d priority increased to %d\n" RESET,
                   processes[i].pid, processes[i].priority);
        }
    }
}

void roundRobin(int timeQuantum, int startTime) {
    int remaining[MAX];
    for (int i = 0; i < n; i++)
        remaining[i] = processes[i].burst;

    int time = startTime;
    int done;

    do {
        done = 1;
        for (int i = 0; i < n; i++) {
            if (processes[i].type == 2 && remaining[i] > 0) {
                done = 0;
                addGantt(processes[i].pid, time);

                if (remaining[i] > timeQuantum) {
                    time += timeQuantum;
                    remaining[i] -= timeQuantum;
                    printf(CYAN "Process %d executed for %d units (remaining: %d)\n" RESET,
                           processes[i].pid, timeQuantum, remaining[i]);
                } else {
                    time += remaining[i];
                    processes[i].waiting = time - processes[i].burst;
                    processes[i].turnaround = time;
                    remaining[i] = 0;
                    printf(GREEN "Process %d completed at time %d\n" RESET,
                           processes[i].pid, time);
                }
            }
        }
    } while (!done);
}

void calculateHighPriorityTimes() {
    int time = 0;

    for (int i = 0; i < n; i++) {
        if (processes[i].type == 1) {
            processes[i].waiting = time;
            addGantt(processes[i].pid, time);
            printf(GREEN "High Priority Process %d executing from %d to %d\n" RESET,
                   processes[i].pid, time, time + processes[i].burst);
            time += processes[i].burst;
            processes[i].turnaround = time;
        }
    }
}

void displayScheduler() {
    printf(BOLD CYAN "\n[ CPU SCHEDULER STATUS ]\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);
    printf(BOLD "PID\tType\tBurst\tWT\tTAT\n" RESET);

    for (int i = 0; i < n; i++) {
        printf("%d\t", processes[i].pid);
        
        if (processes[i].type == 1)
            printf(RED "High\t" RESET);
        else
            printf(CYAN "Normal\t" RESET);
            
        printf("%d\t", processes[i].burst);
        printf(YELLOW "%d\t" RESET, processes[i].waiting);
        printf(GREEN "%d\n" RESET, processes[i].turnaround);
    }
}

void printExecutionPhase(char *phase) {
    printf("\n" BG_BLUE WHITE "=============================================\n" RESET);
    printf(BG_BLUE WHITE ">> %s\n" RESET, phase);
    printf(BG_BLUE WHITE "=============================================\n" RESET);
}

void displayPriorities() {
    printf(BOLD CYAN "\nPID\tType\tPriority\n" RESET);
    printf(YELLOW "---------------------------\n" RESET);

    for (int i = 0; i < n; i++) {
        printf("%d\t", processes[i].pid);
        
        if (processes[i].type == 1) {
            printf(RED "High\t" RESET);
            printf(RED "%d\n" RESET, processes[i].priority);
        } else {
            printf(CYAN "Normal\t" RESET);
            printf(CYAN "%d\n" RESET, processes[i].priority);
        }
    }
}

void displayGanttChart() {
    printf(BOLD CYAN "\n[ GANTT CHART ]\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);
    printf("|");
    
    for (int i = 0; i < ganttCount; i++) {
        if (ganttPID[i] == 1 || ganttPID[i] == 2)
            printf(BG_RED " P%d |" RESET, ganttPID[i]);
        else
            printf(BG_CYAN " P%d |" RESET, ganttPID[i]);
    }
    
    printf("\n");
    
    for (int i = 0; i < ganttCount; i++) {
        printf("%d   ", ganttTime[i]);
    }
    printf("\n");
}

void calculateAverages() {
    float totalWT = 0, totalTAT = 0;

    for (int i = 0; i < n; i++) {
        totalWT += processes[i].waiting;
        totalTAT += processes[i].turnaround;
    }

    printf(BOLD CYAN "\n[ PERFORMANCE METRICS ]\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);
    printf("Average Waiting Time    : " YELLOW "%.2f\n" RESET, totalWT / n);
    printf("Average Turnaround Time : " GREEN "%.2f\n" RESET, totalTAT / n);
}

void systemReport() {
    printf("\n" BG_MAGENTA WHITE "=============================================\n" RESET);
    printf(BG_MAGENTA WHITE "        SMART HOSPITAL OS — SYSTEM REPORT     \n" RESET);
    printf(BG_MAGENTA WHITE "=============================================\n" RESET);

    printf(BOLD CYAN "\nCPU STATISTICS\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);
    
    for (int i = 0; i < n; i++) {
        totalWaiting += processes[i].waiting;
        totalTurnaround += processes[i].turnaround;
    }

    float avgWT = (float)totalWaiting / n;
    float avgTAT = (float)totalTurnaround / n;

    printf("Average Waiting Time     : " YELLOW "%.2f\n" RESET, avgWT);
    printf("Average Turnaround Time  : " GREEN "%.2f\n" RESET, avgTAT);

    int totalBurst = 0;
    for (int i = 0; i < n; i++)
        totalBurst += processes[i].burst;

    float cpuUtil = ((float)totalBurst / (totalBurst + 5)) * 100;
    if (cpuUtil > 80)
        printf("CPU Utilization (approx) : " GREEN "%.2f%%\n" RESET, cpuUtil);
    else if (cpuUtil > 50)
        printf("CPU Utilization (approx) : " YELLOW "%.2f%%\n" RESET, cpuUtil);
    else
        printf("CPU Utilization (approx) : " RED "%.2f%%\n" RESET, cpuUtil);

    printf(BOLD CYAN "\nMEMORY STATISTICS\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);
    
    int used = 0;
    for (int i = 0; i < TOTAL_PARTITIONS; i++) {
        if (memory[i] != 0)
            used++;
    }
    
    printf("Total Memory: " CYAN "%d MB\n" RESET, TOTAL_MEMORY);
    printf("Used Memory : " RED "%d MB\n" RESET, used * PARTITION_SIZE);
    printf("Free Memory : " GREEN "%d MB\n" RESET, (TOTAL_PARTITIONS - used) * PARTITION_SIZE);

    printf(BOLD CYAN "\nPAGING STATISTICS\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);
    printf("Page Hits   : " GREEN "%d\n" RESET, pageHits);
    printf("Page Faults : " RED "%d\n" RESET, pageFaults);

    printf(BOLD CYAN "\nSECURITY VIOLATIONS\n" RESET);
    printf(YELLOW "---------------------------------------------\n" RESET);
    printf("Unauthorized Access Attempts : " RED "%d\n" RESET, securityViolations);

    printf("\n" BG_GREEN WHITE "=============================================\n" RESET);
    printf(BG_GREEN WHITE "           SIMULATION COMPLETED SUCCESSFULLY     \n" RESET);
    printf(BG_GREEN WHITE "=============================================\n\n" RESET);
}

int main() {
    printf(BOLD BG_BLUE WHITE "\n===== SMART HOSPITAL OS SIMULATION =====\n" RESET);

    printExecutionPhase("SECURE FILE VAULT");
    createFile("EHR_Patient1", "BloodGroup: O+", 1);
    createFile("EHR_Patient2", "Diagnosis: Flu", 2);
    readFile("EHR_Patient1", 1);   // Authorized
    readFile("EHR_Patient1", 3);   // Unauthorized

    printExecutionPhase("INITIAL PROCESS STATE");
    displayPriorities();

    printExecutionPhase("PRIORITY SCHEDULING (HIGH PRIORITY PROCESSES)");
    priorityScheduling();
    displayPriorities();

    printExecutionPhase("AGING APPLIED (PREVENT STARVATION)");
    applyAging();
    displayPriorities();

    printExecutionPhase("HIGH PRIORITY EXECUTION");
    calculateHighPriorityTimes();

    int highPriorityTime = 0;
    for (int i = 0; i < n; i++)
        if (processes[i].type == 1)
            highPriorityTime += processes[i].burst;

    printExecutionPhase("ROUND ROBIN EXECUTION (NORMAL PROCESSES)");
    roundRobin(3, highPriorityTime);

    printExecutionPhase("FINAL CPU SCHEDULER OUTPUT");
    displayScheduler();
    displayGanttChart();
    calculateAverages();

    printExecutionPhase("MEMORY MANAGEMENT (FIXED PARTITION)");
    initMemory();
    allocateMemory();
    displayMemoryStatus();

    printExecutionPhase("VIRTUAL MEMORY MANAGEMENT");
    initPaging();
    simulatePaging();
    displayPagingReport();

    systemReport();

    return 0;
}