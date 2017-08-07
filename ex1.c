#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define ALPHA 0.012299
#define BETA (1-ALPHA)
#define SQUARED 2
#define QUBED_SQRT 1.5

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
    long double linearPart = x + 2*velocityY;
    long double dependedD1 = BETA * ((x + ALPHA)/D1);
    long double dependedD2 = ALPHA * ((x - BETA)/D2);
    return linearPart - dependedD1 -dependedD2;
}

long double calculatedYAcceleration(long double y, long double velocityx, long double D1, long double D2)
{
    long double linearPart = y - 2*velocityx;
    long double dependedD1 = BETA * (y/D1);
    long double dependedD2 = ALPHA * (y/D2);
    return linearPart - dependedD1 -dependedD2;
}

long double linearAdding(long double startingValue, long double slope, long double segmentLength)
{
    return startingValue + slope * segmentLength;
}

int parseLongFlaot(long double *outputVariable, char stringToParse[])
{
    *outputVariable = strtold(stringToParse,NULL);
    if (stringToParse[0] != '0' && *outputVariable == 0);

}

int startingDataFromUserInput(long double *x, long double *y, long double *velocityX, long double *velocityY,
                              long double *totalTime, long *numberOfSteps, long *stepsToSave)
{
    char buffer[10];
    printf("Enter initial pos x:\n");
    fgets(buffer,10,stdin);
    int yay = sscanf(buffer,"%Lf",x);
    printf("%d\n",yay);

}


int stratingDataFromFile(char *inputFileName, long double *x, long double *y, long double *velocityX,
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
    long double accelerationX, accelerationY; 
    for (int i = 1 ; i <= numbersOfSteps; i++)
    {
        long double d1 = calculateD1(x, y);
        long double d2 = calculateD2(x, y);
        accelerationX = calculatedXAcceleration(x, velocityY, d1, d2);
        accelerationY = calculatedYAcceleration(y, velocityX, d1, d2);

        x = linearAdding(x, velocityX, dt);
        y = linearAdding(y, velocityY, dt);

        velocityX = linearAdding(velocityX, accelerationX ,dt);
        velocityY = linearAdding(velocityY, accelerationY, dt);

        if (i % whenToPrint == 0){
            fprintf(destinationFile,"%E,%E,", x, y);

        }
    }
    if (fclose(destinationFile) != 0)
    {
        perror("Problem closing destination file");
        return -1;
    }
    
}

int main(int argc, char *argv[])
{
    long numbersOfSteps = 550, stepsToSave = 110, whenToPrint;
    long double x, y = 5889, velocityX = 0.0, velocityY = 0, totalTime = 16.544468478, dt;
    char *destinationFileName;
    FILE *destinationFile;

    if (argc == 2)
    {
        destinationFileName = argv[1];
        startingDataFromUserInput(&x, &y, &velocityX, &velocityY, &totalTime, &numbersOfSteps, &stepsToSave);
        printf("%Lf",x);
    } else if (argc == 3)
    {
        destinationFileName = argv[2];
        stratingDataFromFile(argv[1], &x, &y, &velocityX, &velocityY, &totalTime, &numbersOfSteps, &stepsToSave);
    } else
    {
        fprintf(stderr,"Invalid number of arguments");
        return 1;
    }


    destinationFile = fopen(destinationFileName,"w");
    if (destinationFile == NULL)
    {
        perror("Problem with destination file");
        return errno;
    }
    dt = totalTime / numbersOfSteps;
    whenToPrint = numbersOfSteps / stepsToSave;

    mainLoop(numbersOfSteps, whenToPrint, destinationFile,x,y,velocityX,velocityY,dt);

    return 0;

}