#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define ALPHA 0.012299
#define BETA (1-ALPHA)
#define SQUARED 2
#define QUBED_SQRT 1.5
#define DECIMAL 10
#define BUFFER_SIZE 50

long double calculateD1(long double x,long double y)
{
    long double adjustedXSquared = powl(x + ALPHA, SQUARED);
    long double ySquared = powl(y, SQUARED);
    return powl(adjustedXSquared + ySquared, QUBED_SQRT);
}

long double calculateD2(long double x,long double y)
{
    long double adjustedXSquared = powl(x - BETA, SQUARED);
    long double ySquared = powl(y, SQUARED);
    return powl(adjustedXSquared + ySquared, QUBED_SQRT);
}

long double calculatedXAcceleration(long double x, long double velocityY, long double D1, long double D2)
{
    long double linearPart = x + 2.0*velocityY;
    long double dependedD1 = BETA * ((x + ALPHA)/D1);
    long double dependedD2 = ALPHA * ((x - BETA)/D2);
    return linearPart - dependedD1 -dependedD2;
}

long double calculatedYAcceleration(long double y, long double velocityX, long double D1, long double D2)
{
    long double linearPart = y - 2.0*velocityX;
    long double dependedD1 = BETA * (y/D1);
    long double dependedD2 = ALPHA * (y/D2);
    return linearPart - dependedD1 -dependedD2;
}

long double linearAdding(long double startingValue, long double slope, long double segmentLength)
{
    return startingValue + slope * segmentLength;
}

int getLongDoubleFromUser(long double *outputVariable)
{
    errno = 0;
    char *endChar;
    char buffer[BUFFER_SIZE];
    if (fgets(buffer,BUFFER_SIZE,stdin) == NULL)
    {
        perror("Error with file");
        exit(errno);
    }
    *outputVariable = strtold(buffer,&endChar);
    if (errno != 0)
    {
        perror("Problem with parsing");
        exit(errno);

    } else if (*endChar != '\n')
    {
        return -1;
    }

    return 0;
}

int getLongFromUser(long *outputVariable)
{
    errno = 0;
    char *endChar;
    char buffer[BUFFER_SIZE];
    if (fgets(buffer,BUFFER_SIZE,stdin) == NULL)
    {
        perror("Error with file");
        exit(errno);
    }
    *outputVariable = strtol(buffer,&endChar,DECIMAL);
    if (errno != 0)
    {
        perror("Problem with parsing");
        exit(errno);

    } else if (*endChar != '\n')
    {
        return -1;
    }
    return 0;
}

int startingDataFromUserInput(long double *x, long double *y, long double *velocityX, long double *velocityY,
                              long double *totalTime, long *numberOfSteps, long *stepsToSave)
{
    int parseResult = 0;

    printf("Enter initial position x:\n");
    parseResult += getLongDoubleFromUser(x);
    printf("Enter initial position y:\n");
    parseResult += getLongDoubleFromUser(y);;
    printf("Enter initial velocity x:\n");
    parseResult += getLongDoubleFromUser(velocityX);
    printf("Enter initial velocity y:\n");
    parseResult += getLongDoubleFromUser(velocityY);
    printf("Enter total time:\n");
    parseResult += getLongDoubleFromUser(totalTime);
    printf("Enter total number of steps:\n");
    parseResult += getLongFromUser(numberOfSteps);
    printf("Enter number of steps to save:\n");
    parseResult += getLongFromUser(stepsToSave);
    if (parseResult < 0)
    {
        printf("parsing failed");
        return -1;
    }
    return 0;
}


int startingDataFromFile(char *inputFileName, long double *x, long double *y, long double *velocityX,
                         long double *velocityY, long double *totalTime, long *numberOfSteps, long *stepsToSave)
{
    FILE *inputFile = fopen(inputFileName,"r");
    if (inputFile == NULL)
    {
        perror("Problem with input file");
        return errno;
    }


    if (fclose(inputFile) != 0)
    {
        perror("Problem closing input file");
        return errno;
    }

    return 0;

}

int mainLoop(long numbersOfSteps, long whenToPrint, FILE *destinationFile, long double initialX, long double initialY,
             long double initialVelX, long double initialVelY, long double delta)
{
    long double x = initialX, y = initialY, velocityX = initialVelX, velocityY = initialVelY, dt = delta;
    long double accelerationX = 0, accelerationY = 0;
    for (int i = 0 ; i < numbersOfSteps; i++)
    {
        long double d1 = calculateD1(x, y);
        long double d2 = calculateD2(x, y);
        accelerationX = calculatedXAcceleration(x, velocityY, d1, d2);
        accelerationY = calculatedYAcceleration(y, velocityX, d1, d2);

        x = linearAdding(x, velocityX, dt);
        y = linearAdding(y, velocityY, dt);

        velocityX = linearAdding(velocityX, accelerationX ,dt);
        velocityY = linearAdding(velocityY, accelerationY, dt);


        if (i % whenToPrint == 0)
        {
            fprintf(destinationFile,"%.3Le,%.3Le,", x, y);
        }

    }
    if (fclose(destinationFile) != 0)
    {
        perror("Problem closing destination file");
        return -1;
    }
    return 0;
    
}

int main(int argc, char *argv[]) {
    long numbersOfSteps = 550, stepsToSave = 110, whenToPrint;
    long double x, y = 5889, velocityX = 0.0, velocityY = 0, totalTime = 16.544468478, dt;
    char *destinationFileName;
    FILE *destinationFile;

    if (argc == 2) {
        destinationFileName = argv[1];
        startingDataFromUserInput(&x, &y, &velocityX, &velocityY, &totalTime, &numbersOfSteps, &stepsToSave);
    } else if (argc == 3) {
        destinationFileName = argv[2];
        startingDataFromFile(argv[1], &x, &y, &velocityX, &velocityY, &totalTime, &numbersOfSteps, &stepsToSave);
    } else {
        fprintf(stderr, "Invalid number of arguments");
        return 1;
    }
    destinationFile = fopen(destinationFileName, "w");
    if (destinationFile == NULL) {
        perror("Problem with destination file");
        return errno;
    }


    dt = totalTime / numbersOfSteps;
    if (numbersOfSteps % stepsToSave == 0) {
        whenToPrint = numbersOfSteps / stepsToSave;
    } else {
        fprintf(stderr, "Steps to save does not divide total steps evenly!");
        return 2;
    }
    printf("x-%Lf,y-%Lf,%Lf,%Lf\n time= %Lf, step =%ld,save=%ld,print when -%ld ", x, y, velocityX, velocityY,
           totalTime,
           numbersOfSteps, stepsToSave, whenToPrint);

    mainLoop(numbersOfSteps, whenToPrint, destinationFile, x, y, velocityX, velocityY, dt);
    return 0;
}
