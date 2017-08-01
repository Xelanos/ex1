#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ALPHA 0.012299
#define BETA 1-ALPHA
#define SQUARED 2
#define QUBED_SQRT 1.5

double calculateD1(double x, double y)
{
    double adjustedXSquared = pow(x + ALPHA, SQUARED);
    double ySquared = pow(y, SQUARED);
    return pow(adjustedXSquared + ySquared, QUBED_SQRT);
}

double calculateD2(double x, double y)
{
    double adjustedXSquared = pow(x - BETA, SQUARED);
    double ySquared = pow(y, SQUARED);
    return pow(adjustedXSquared + ySquared, QUBED_SQRT);
}

double calculatedXAcellration (double x, double velocityY, double D1, double D2)
{
    double linearPart = x + 2*velocityY;
    double dependedD1 = BETA * ((x + ALPHA)/D1);
    double dependedD2 = ALPHA * ((x - BETA)/D2);
    return linearPart - dependedD1 -dependedD2;
}

double calculatedYAcellration (double y , double velocityx, double D1, double D2)
{
    double linearPart = y + 2*velocityx;
    double dependedD1 = BETA * (y/D1);
    double dependedD2 = ALPHA * (y/D2);
    return linearPart - dependedD1 -dependedD2;
}

double linearAdding(double startingValue, double slope, long double segmentLength)
{
    long double newValue = startingValue + slope * segmentLength;
    return (double) newValue;
}

int main()
{
    int numbersOfSteps = 3, stepsToSave = 1, whenToPrint;
    double x = 1, y = 1, velocityX = 0.5, velocityY = 0.6, accelerationX = 0, accelerationY = 0, totalTime = 0.25;
    long double dt;

    dt = totalTime / numbersOfSteps;
    whenToPrint = numbersOfSteps / stepsToSave;


    for (int i = 1 ; i <= numbersOfSteps; i++)
    {
        double d1 = calculateD1(x, y);
        double d2 = calculateD2(x, y);
        accelerationX = calculatedXAcellration(x, velocityY, d1, d2);
        accelerationY = calculatedYAcellration(y, velocityX, d1, d2);

        x = linearAdding(x, velocityX, dt);
        y = linearAdding(y, velocityY, dt);

        velocityX = linearAdding(velocityX, accelerationX ,dt);
        velocityY = linearAdding(velocityY, accelerationY, dt);

        if (i % stepsToSave == 0){
            printf("<%f,%f>,<%f,%f>\n", x,y,velocityX,velocityY);
        }
    }

}