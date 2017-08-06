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

int main(int argc, char *argv[])
{
    long numbersOfSteps = 550, stepsToSave = 110, whenToPrint;
    long double x = 6644.52, y = 5889, velocityX = 0.0, velocityY = 0,
            accelerationX = 0.0, accelerationY = 0.0, totalTime = 16.544468478;
    long double dt;
    char *destinationFileName;
    FILE *destinationFile;

    if (argc == 2)
    {
        destinationFileName = argv[1];
        //startingDataFromUserInput();
    } else if (argc == 3)
    {
        destinationFileName = argv[2];
        //stratingDataFromFile(argv[1]);
    } else
    {
        fprintf(stderr,"Invalid number of arguments");
        return -1;
    }

    destinationFile = fopen(destinationFileName,"w");
    if (destinationFile == NULL)
    {
        perror("Problem with destination file");
        return errno;
    }

    dt = totalTime / numbersOfSteps;
    whenToPrint = numbersOfSteps / stepsToSave;
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
            fprintf(destinationFile,"%e,%e,", x,y);

        }
    }
    fclose(destinationFile);
    return 0;

}