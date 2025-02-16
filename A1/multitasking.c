
#define N 100000000
#define NUM_TASKS 2 

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

//Task assignments
struct Task{
    int t_id;
    int pipe_tracker[2];
    long long split_sum;
};

//Calculate Sum for the specific task
void assignedSum(struct Task *current_task){
    long long assigned_sum = 0;
    long long start_point = (N/NUM_TASKS)*current_task->t_id;
    for(long long j = start_point; j < start_point + N/NUM_TASKS; j++){ 
        assigned_sum += j;
    }
    current_task->split_sum = assigned_sum;
}

//Relay the information via pipes to the parent
void relaySum(struct Task *current_task){
    write(current_task->pipe_tracker[1],&current_task->split_sum,sizeof(current_task->split_sum));
    close(current_task->pipe_tracker[1]);
    exit(EXIT_SUCCESS);
}


int main(){
    long long sum = 0;
    struct Task work_tasks[NUM_TASKS];
    pid_t c_pid[NUM_TASKS];

    for(int i = 0; i < NUM_TASKS; i++){
        //Instantiate pipes
        if(pipe(work_tasks[i].pipe_tracker) == -1){
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }

        c_pid[i] = fork();

        //Check created successfully
        if(c_pid[i] == -1){
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        work_tasks[i].t_id = i;

        //Child tasks
        if(c_pid[i] == 0){ 
            close(work_tasks[i].pipe_tracker[0]);
            assignedSum(&work_tasks[i]);
            relaySum(&work_tasks[i]);
        }
    }

    for(int i = 0; i < NUM_TASKS; i++){
        close(work_tasks[i].pipe_tracker[1]);
        long long assigned_sum;
        read(work_tasks[i].pipe_tracker[0],&assigned_sum,sizeof(assigned_sum));
        close(work_tasks[i].pipe_tracker[0]);
        sum += assigned_sum;
        waitpid(c_pid[i],NULL,0);
    }

    printf("Total sum from 0 to %d is: %lld\n",N,sum);
    return 0;

}


