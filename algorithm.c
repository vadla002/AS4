/*
    Shirisha Vadlamudi
    Peter Kemper
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define SLEEP_TIME 1

int counter = 0;
int i, j, k;
int argCount;

int safety(int process);
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);
void print_resources();
void initialize_resources();
    
int available[NUMBER_OF_RESOURCES];
int max[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int request[NUMBER_OF_RESOURCES];
int release[NUMBER_OF_RESOURCES];

int main(int argumentCount, char* argumentArray[])
{
        
    printf("Available\n");
    
    for(i=1; i<argumentCount; i++) {
        printf("%d\t", atoi(argumentArray[i]));
    }
    argCount = argumentCount;
    
    printf("\n");
    printf("\tAllocation");
    printf("\tMax");
    printf("\tNeed\n");
    
    for(int n=0; n<NUMBER_OF_RESOURCES; n++) {
            available[i] = atoi(argumentArray[i + 1]);
    }
    
    for(i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        printf("P%d\t", i);
        for(j=0; j<NUMBER_OF_RESOURCES; j++) {
            allocation[i][j] = 0;
            printf("%d", allocation[i][j]);
        }
        
        printf("\t\t");
        for(int k=0; k<NUMBER_OF_RESOURCES; k++) {
            max[i][k] = rand() % (argCount+1);
            printf("%d", max[i][k]);
        }

        printf("\t");
        for(int l=0; l<NUMBER_OF_RESOURCES; l++) {
            need[i][l] = max[i][l];
            printf("%d", need[i][l]);
        }
        printf("\n");
    }
    // initialize_resources();
    
    while(1)
    {
        printf("\nEnter your command:\n");
        char* input[100];
        char req[2];
        int cust, res1, res2, res3, res4;

        char c = getchar();

        if (c == '*')
        {
            print_resources();
        }
        else
        {
            for (int x = 0; x<6; x++)
            {
                scanf("%s", &input[x]);
            }
            strcpy(req,&input[0]);
            cust = atoi(&input[1]);
            res1 = atoi(&input[2]);
            res2 = atoi(&input[3]);
            res3 = atoi(&input[4]);
            res4 = atoi(&input[5]);

            int res[4] = {res1, res2, res3, res4};
            if(strcmp(req,"Q") == 0)
            {
                    request_resources(cust, res);
            }
            else if(strcmp(req,"L") == 0)
            {
                    release_resources(cust, res);
            }
        }
        getchar();
    }
}

int request_resources(int customer_num, int request[])
{    
    printf("P%d is requesting: [%d, %d, %d, %d]\n", customer_num, request[0], request[1], request[2], request[3]);
    
    for(i=0; i<NUMBER_OF_RESOURCES; i++) {
        //checks to see if the requested resources does not exceed the amount needed.
        if(request[i] <= need[customer_num][i]) {
            //checks to see if the requested resources does not exceed the amount available.
            if(request[i] <= available[i]) {
                
                available[i] = available[i] - request[i];
                
                //checks to see if each customer process is in a safe state.
                int condition = safety(customer_num);
                
                //change the allocation to represent the amount requested.
                allocation[customer_num][i] = allocation[customer_num][i] + request[i];
                
                //the amount needed by the process is updated with the number of requested resources removed by the array.
                need[customer_num][i] = need[customer_num][i] - request[i];
                
                //continue if there is a safety sequence.
                if(condition==0) {
                    
                    printf("The request has been granted\n");
                    
                    print_resources();
                
                    return 0; //successfully allocated a certain number of resources.
                
                }
                
                //perform the rollback if found that system is in an unsafe state.
                else {
                    available[i] = available[i] + request[i];
                
                    //change the allocation to represent the amount requested.
                    allocation[customer_num][i] = allocation[customer_num][i] - request[i];
                
                    //the amount needed by the process is updated with the number of requested resources removed by the array.
                    need[customer_num][i] = need[customer_num][i] + request[i];
                    
                    printf("The system is in an unsafe state.\n");
                    return -1;
                }
    
            }
            
            //make the procss wait if the number of resources requested for a type exceeds the amount available for the process.
            else if(request[i] > available[i]) {
                printf("The process must wait, since the resources are not available.\n");
                return -1;
            }
        }
        
        //if the amount of a type of resource requested exceeds the amount needed by the customer process, do not allow the resources to be allocated.
        else if(request[i] > need[customer_num][i]) {
            printf("The process has exceeded its maximum claim.\n");
            return -1;
        }
        
    }
}

int safety(int process)
{    
    int work[process];
    int finish[process];
    
    work[process] = available;
    finish[process] = -1;
    
    for(i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        //loop through the number of types of each resources for each customer process.
        for(j=0; j<NUMBER_OF_RESOURCES; j++) {
        
            //checks if the customer process has been completed, and the amount of resources needed does not exceeded the work done.
            if(finish[i]==-1 && need[i][j] <= work[i]) {
                //the system is an unsafe state.
                return -1;
            }
            else {            
                work[i] = work[i] + allocation[i][j];
                finish[i] = 0;
            }
        }
    }
    
    return 0;
}

int release_resources(int customer_num, int release[])
{
    printf("P%d is releasing: [%d, %d, %d, %d]\n", customer_num, release[0], release[1], release[2], release[3]);   
    //return the types of resources back to the available vector.
    for(i=0; i<NUMBER_OF_RESOURCES; i++) {
        available[i] = available[i] + request[i];
        //the amount requested should be removed by what is allocated.
        allocation[customer_num][i] = allocation[customer_num][i] - request[i];
        //update what is needed by the request, and start the process over again.
        need[customer_num][i] = need[customer_num][i] + request[i];   
    }
    
    print_resources();
    return 0;
}

void print_resources()
{
    printf("\n");
    printf("\tAllocation");
    printf("\tMax");
    printf("\tNeed\n");
    
    for(i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        printf("P%d\t", i);
        for(j=0; j<NUMBER_OF_RESOURCES; j++) {
            printf("%d", allocation[i][j]);
        }
        
        printf("\t\t");
        for(int k=0; k<NUMBER_OF_RESOURCES; k++) {
            printf("%d", max[i][k]);
        }

        printf("\t");
        for(int l=0; l<NUMBER_OF_RESOURCES; l++) {
            printf("%d", need[i][l]);
        }
        printf("\n");
    }
}
