#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
//------------------------defines-------------------------
#define N 5
#define FIN_PROB 0.5
#define MIN_INTER_ARRIVAL_IN_NS 8000000
#define MAX_INTER_ARRIVAL_IN_NS 9000000
#define INTER_MOVES_IN_NS 100000
#define SIM_TIME 2
#define MAX_CARS_AMOUNT (int)(((SIM_TIME*10000000000)/(MIN_INTER_ARRIVAL_IN_NS))+50) //Total amount of cars since the program starts till end
#define CAR_PRINT "*"
#define BLANK_PRINT "@"
#define BLANK_CAR " "
//------------------functions signatures------------------
void* printBoard(void* id);
void* tubeGenCar(void* id);
void* createCar(void* carIndexArray);
int prob();
//-----------------------structures-----------------------
typedef struct Car{
    pthread_t carThread;
    int carID;
}Car;
typedef struct Cell{
    pthread_mutex_t cellMutex;
}Cell;
typedef struct TubeStation{
    pthread_t tubeThread;
    int id;
}TubeStation;
typedef struct Board{
    Cell cells[4*N-4];
    TubeStation tubes [4];
    int boardArray[(4*N)-4];
}Board;
//------------------------variables-----------------------
Car cars[MAX_CARS_AMOUNT];
pthread_t print_thread;
Board board;
int carCounter= 0;
pthread_mutex_t createCarMutex;
pthread_mutex_t printCarMutex;
int arg[4] = {0,N-1,2*(N-1),3*(N-1)};
char blanks[N-1];
//------------------------Main code-----------------------
int main() {

    for (int i = 0; i < MAX_CARS_AMOUNT; i ++ )
        cars[i].carID = -1;


    for (int i = 0; i < N-2; i++)
        strcat(blanks, BLANK_PRINT);

    for(int i=0;i<(4*N)-4;i++)
        board.boardArray[i] = -1;

    //Mutex initialize
    for (int i = 0;i < ((N*4)-4); i++)
    {
        if(pthread_mutex_init(&board.cells[i].cellMutex, NULL) != 0)
        {
            perror("MUTEX INITIALIZE IS FAILED\n");
            exit(-1);
        }
    }
    if(pthread_mutex_init(&createCarMutex, NULL) != 0)
    {
        perror("MUTEX INITIALIZE IS FAILED\n");
        exit(-1);
    }
    if(pthread_mutex_init(&printCarMutex, NULL) != 0)
    {
        perror("MUTEX INITIALIZE IS FAILED\n");
        exit(-1);
    }

   //Tubes initialize
    for (int i = 0; i < 4; i++)
    {
        board.tubes[i].id = i;
        if(pthread_create(&board.tubes[i].tubeThread,NULL,tubeGenCar,(void*)&board.tubes[i].id) != 0)
        {
            perror("TUBE INITIALIZE IS FAILED\n");
            exit(-1);
        }

    }

   //Print initialize
    if(pthread_create(&print_thread,NULL,printBoard,NULL) != 0)
    {
         perror("PRINT INITIALIZE IS FAILED\n");
         exit(-1);
    }

    //sleep(50);
    sleep(SIM_TIME);


    for (int i = 0; i < 4; i++)
        pthread_cancel(board.tubes[i].tubeThread);
    pthread_cancel(print_thread);


    if(pthread_mutex_destroy(&createCarMutex) != 0)
    {
        perror("MUTEX INITIALIZE IS FAILED\n");
        exit(-1);
    }
    if(pthread_mutex_destroy(&printCarMutex) != 0)
    {
        perror("MUTEX INITIALIZE IS FAILED\n");
        exit(-1);
    }

        return 0;

}


void* tubeGenCar(void* id){
    double randomTime;
    int tubeID = *(int*) id;
    while(1) {
        randomTime = rand() % (MAX_INTER_ARRIVAL_IN_NS - MIN_INTER_ARRIVAL_IN_NS) + MIN_INTER_ARRIVAL_IN_NS;
        randomTime = randomTime / (double) 1000;
        usleep((int) randomTime);
        switch (tubeID) {
            case 0:
                pthread_mutex_lock(&board.cells[0].cellMutex);

                if (pthread_create(&cars[carCounter].carThread, NULL, createCar, arg) != 0) {
                    perror("TUBE INITIALIZE I FAILED\n");
                    exit(-1);
                }
                break;
            case 1:
                pthread_mutex_lock(&board.cells[N - 1].cellMutex);
                if (pthread_create(&cars[carCounter].carThread, NULL, createCar, arg + 1) != 0) {
                    perror("TUBE INITIALIZE IS FAILED\n");
                    exit(-1);
                }
                break;
            case 2:
                pthread_mutex_lock(&board.cells[2 * (N - 1)].cellMutex);
                if (pthread_create(&cars[carCounter].carThread, NULL, createCar, arg + 2) != 0) {
                    perror("TUBE INITIALIZE IS FAILED\n");
                    exit(-1);
                }
                break;
            case 3:
                pthread_mutex_lock(&board.cells[3 * (N - 1)].cellMutex);
                if (pthread_create(&cars[carCounter].carThread, NULL, createCar, arg + 3) != 0) {
                    perror("TUBE INITIALIZE IS FAILED\n");
                    exit(-1);
                }
                break;
            default:
                break;
        }
    }
}

void* createCar(void* carIndexArray) {
    int nextIndex;
    pthread_mutex_lock(&createCarMutex);
    int carCounterPT = carCounter;
    carCounter++;
    pthread_mutex_unlock(&createCarMutex);

    int indexOfCar = *(int *) carIndexArray;
    cars[carCounterPT].carID = carCounterPT;
    board.boardArray[indexOfCar] = cars[carCounterPT].carID;
    int currentIndex = indexOfCar;
    nextIndex = indexOfCar + 1;
    int probForCar = 0;
    while (1) {
        usleep(INTER_MOVES_IN_NS / (double) 1000);
        if ((nextIndex == (N - 1)) || (nextIndex == (2 * (N - 1))) || (nextIndex == (3 * (N - 1)))) {
            if (board.boardArray[nextIndex] == -1) {
                pthread_mutex_lock(&board.cells[nextIndex].cellMutex);
                board.boardArray[currentIndex] = -1;
                board.boardArray[nextIndex] = cars[carCounterPT].carID;
                nextIndex = nextIndex + 1;
                pthread_mutex_unlock(&board.cells[currentIndex].cellMutex);
                currentIndex = currentIndex + 1;
                probForCar = prob();
                if (probForCar == 1) {
                    usleep(INTER_MOVES_IN_NS / (double) 1000);
                    board.boardArray[currentIndex] = -1;
                    pthread_mutex_unlock(&board.cells[currentIndex].cellMutex);

                    pthread_cancel(pthread_self());
               }
            }
        } else if (nextIndex == (4 * N - 4)) {
            if (board.boardArray[0] == -1) {
                pthread_mutex_lock(&board.cells[0].cellMutex);
                board.boardArray[currentIndex] = -1;
                board.boardArray[0] = cars[carCounterPT].carID;
                nextIndex =  1;
                pthread_mutex_unlock(&board.cells[currentIndex].cellMutex);
                currentIndex = 0;
                probForCar = prob();
                if (probForCar == 1) {
                    usleep(INTER_MOVES_IN_NS / (double) 1000);
                    board.boardArray[currentIndex] = -1;
                    pthread_mutex_unlock(&board.cells[currentIndex].cellMutex);
                    pthread_cancel(pthread_self());
                }
            }
        } else {
            if (board.boardArray[nextIndex] == -1) {
                pthread_mutex_lock(&board.cells[nextIndex].cellMutex);
                board.boardArray[currentIndex] = -1;
                board.boardArray[nextIndex] = cars[carCounterPT].carID;
                nextIndex = nextIndex + 1;
                pthread_mutex_unlock(&board.cells[currentIndex].cellMutex);
                currentIndex = currentIndex + 1;
            }
        }
    }
}

void* printBoard(void* id) {
    while (1) {
        usleep((int) ((SIM_TIME / (double) (11)) * 1000000));
        if(board.boardArray[0] != -1)
            printf(CAR_PRINT);
        else
            printf(BLANK_CAR);

        for(int i = (4*N - 5); i >= 3*(N-1); i--) {
            if(board.boardArray[i] != -1)
                printf(CAR_PRINT);
            else
                printf(BLANK_CAR);
        }
        printf("\n");
        for(int i = 1; i <= N-2; i++)
        {
            if(board.boardArray[i] != -1)
                printf(CAR_PRINT);
            else
                printf(BLANK_CAR);
            printf("%s", blanks);
            if(board.boardArray[3*(N-1) - i] != -1)
                printf(CAR_PRINT);
            else
                printf(BLANK_CAR);
            printf("\n");
        }
        if(board.boardArray[N-1] != -1)
            printf(CAR_PRINT);
        else
            printf(BLANK_CAR);

        for(int i = N-1; i < 2*(N-1); i++) {
            if(board.boardArray[i] != -1)
                printf(CAR_PRINT);
            else
                printf(BLANK_CAR);
        }
        printf("\n");
        printf("\n");
        printf("\n");

    }
}
//This function check in probability of FIN_PROB if the car is going To Die die or not
int prob(){
    int goingToDie = (rand()%100) <= 100 * FIN_PROB;
    return goingToDie;
}
