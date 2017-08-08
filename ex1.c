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
#define PARSING_FIRST "Trouble parsing first line"
#define PARSING_SECOND "Trouble parsing second line"

/**
 * calculates D1
 * @param x x co-ordinate
 * @param y y co-ordinate
 * @return D1 in long double
 */
long double calculateD1(long double x, long double y)
{
    long double adjustedXSquared = powl(x + ALPHA, SQUARED);
    long double ySquared = powl(y, SQUARED);
    return powl(adjustedXSquared + ySquared, QUBED_SQRT);
}

/**
 * calculates D2
 * @param x x co-ordinate
 * @param y y co-ordinate
 * @return D2 in long double
 */
long double calculateD2(long double x, long double y)
{
    long double adjustedXSquared = powl(x - BETA, SQUARED);
    long double ySquared = powl(y, SQUARED);
    return powl(adjustedXSquared + ySquared, QUBED_SQRT);
}

/**
 * calculetes accel on the x axis by euler formula
 * @param x  x co-ordinate
 * @param velocityY  velocity in y axis
 * @param D1 D1 as gotten by calculateD1
 * @param D2 D2 as gotten by calculateD2
 * @return accelartion for curret coordinates in x axis
 */
long double calculatedXAcceleration(long double x, long double velocityY, long double D1, long double D2)
{
    long double linearPart = x + 2.0*velocityY;
    long double dependedD1 = BETA * ((x + ALPHA) / D1);
    long double dependedD2 = ALPHA * ((x - BETA) / D2);
    return linearPart - dependedD1 - dependedD2;
}


/**
 * calculetes accel on the y axis by euler formula
 * @param x  y co-ordinate
 * @param velocityx  velocity in x axis
 * @param D1 D1 as gotten by calculateD1
 * @param D2 D2 as gotten by calculateD2
 * @return accelartion for curret coordinates in y axis
 */
long double calculatedYAcceleration(long double y, long double velocityX, long double D1, long double D2)
{
    long double linearPart = y - 2.0*velocityX;
    long double dependedD1 = BETA * (y / D1);
    long double dependedD2 = ALPHA * (y / D2);
    return linearPart - dependedD1 -dependedD2;
}

/**
 * addes in a linear line
 * @param startingValue starting value on the line
 * @param slope slope of the line
 * @param segmentLength segement on the x axis
 * @return the value y after the segment length
 */
long double linearAdding(long double startingValue, long double slope, long double segmentLength)
{
    return startingValue + slope * segmentLength;
}

/**
 * gets double from user in stdin and puts it in varible. can exit program if cannot accses file.
 * @param outputVariable varible to assign
 * @return 0 if all is good, -1 if didn't reach end of line.
 */
int getLongDoubleFromUser(long double *outputVariable)
{
    errno = 0;
    char *endChar;
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
    {
        perror("Error with file");
        exit(errno);
    }
    *outputVariable = strtold(buffer, &endChar);
    if (errno != 0)
    {
        perror("Problem with parsing");
        exit(errno);

    }
    else if (*endChar != '\n')
    {
        return -1;
    }

    return 0;
}

/**
 * gets long from user in stdin and puts it in varible. can exit program if cannot accses file.
 * @param outputVariable varible to assign
 * @return 0 if all is good, -1 if didn't reach end of line.
 */
int getLongFromUser(long *outputVariable)
{
    errno = 0;
    char *endChar;
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
    {
        perror("Error with file");
        exit(errno);
    }
    *outputVariable = strtol(buffer, &endChar, DECIMAL);
    if (errno != 0)
    {
        perror("Problem with parsing");
        exit(errno);

    }
    else if (*endChar != '\n')
    {
        return -1;
    }
    return 0;
}

/**
 * gets starting values from user input in stdin
 * @param x pointer to x
 * @param y pointer to y
 * @param velocityX pointer to vaelocityX
 * @param velocityY pointer to vaelocityY
 * @param totalTime pointer to totalTime
 * @param numberOfSteps pointer to numberOfSteps
 * @param stepsToSave pointer to stepsToSave
 * @return 0 if all parsingins went well, -1 otherwise.
 */
int startingDataFromUserInput(long double *x, long double *y, long double *velocityX, long double *velocityY,
                              long double *totalTime, long *numberOfSteps, long *stepsToSave)
{
    int parseResult = 0;
    printf("Enter initial position x:\n");
    parseResult += getLongDoubleFromUser(x);
    printf("Enter initial position y:\n");
    parseResult += getLongDoubleFromUser(y);
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
        return -1;
    }
    return 0;
}

/**
 * gets a line from file after getting rid of starting whitespace
 * @param buffer buffer to work with
 * @param bufferSize the size of said buffer
 * @param inputFile file to read from.
 * @return pointer to the first char in a string representing the line. 0 if couldn't read.
 */
char *readLine(char *buffer, int bufferSize, FILE* inputFile)
{
    char *cp = fgets(buffer, bufferSize, inputFile);
    if (cp != NULL)
    {
        while (*cp == ' ' || *cp == '\t')
        {
            cp++;
        }
        return cp;
    }
    return 0;

}

/**
 * takes the first line of the file and put value in the parameters.
 * @param line line to parse from (should be the first one)
 * @param x pointer to x
 * @param y pointer to y
 * @param velocityX pointer to vaelocityX
 * @param velocityY pointer to vaelocityY
 * @return 0 if parsing was ok, -1 if not.
 */
int parseFirstLine(char *line, long double *x, long double *y, long double *velocityX, long double *velocityY)
{
    errno = 0;
    char *endChar;
    *x = strtold(line, &endChar);
    if (*endChar != ',' || errno != 0)
    {
        fprintf(stderr, PARSING_FIRST);
        return -1;
    }
    endChar += 2;

    *y = strtold(endChar, &endChar);
    if (*endChar != ',' || errno != 0)
    {
        fprintf(stderr, PARSING_FIRST);
        return -1;
    }
    endChar += 2;

    *velocityX = strtold(endChar, &endChar);
    if (*endChar != ',' || errno != 0)
    {
        fprintf(stderr, PARSING_FIRST);
        return -1;
    }
    endChar += 2;

    *velocityY = strtold(endChar, &endChar);
    if (*endChar != '\n' || errno != 0)
    {
        fprintf(stderr, PARSING_FIRST);
        return -1;
    }

    return 0;
}
/**
 * takes the second line and put value in the parameters.
 * @param line the line to parse (shold be the second line)
 * @param time pointer to totalTime
 * @param totalSteps pointer to total steps.
 * @param stepsToSave pointer to stepsToSave.
 * @return 0 if parsing was ok, -1 if not.
 */
int parseSecondLine(char *line, long double *time, long *totalSteps, long *stepsToSave)
{
    errno = 0;
    char *endChar;
    *time = strtold(line, &endChar);
    if (*endChar != ',' || errno != 0)
    {
        fprintf(stderr, PARSING_SECOND);
        return -1;
    }
    endChar += 2;

    *totalSteps = strtol(endChar, &endChar, DECIMAL);
    if (*endChar != ',' || errno != 0)
    {
        fprintf(stderr, PARSING_SECOND);
        return -1;
    }
    endChar += 2;

    *stepsToSave = strtol(endChar, &endChar, DECIMAL);
    if (*endChar != '\0' || errno != 0)
    {
        fprintf(stderr, PARSING_SECOND);
        return -1;
    }
    return 0;
}
/**
 * get the starting data from a file
 * @param inputFileName name of the file to read from.
 * @param x pointer to x
 * @param y pointer to y
 * @param velocityX pointer to vaelocityX
 * @param velocityY pointer to vaelocityY
 * @param time pointer to totalTime
 * @param totalSteps pointer to total steps.
 * @param stepsToSave pointer to stepsToSave.
 * @return 0 if succeded, errono if encounterd an error.
 */
int startingDataFromFile(char *inputFileName, long double *x, long double *y, long double *velocityX,
                         long double *velocityY, long double *totalTime, long *numberOfSteps, long *stepsToSave)
{
    char buffer[BUFFER_SIZE];
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL)
    {
        perror("Problem with input file");
        return errno;
    }

    char* firstLine = readLine(buffer, BUFFER_SIZE, inputFile);
    if (parseFirstLine(firstLine, x, y, velocityX, velocityY))
    {
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    char* secondLine = readLine(buffer, BUFFER_SIZE, inputFile);
    if (parseSecondLine(secondLine, totalTime, numberOfSteps, stepsToSave))
    {
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    if (fclose(inputFile))
    {
        perror("Problem closing input file");
        return errno;
    }

    return 0;

}

/**
 * main loop of the program, does the calculation and prints the reasults
 * @param numbersOfSteps number of times the loop will go.
 * @param whenToPrint how many steps to print.
 * @param destinationFile where to print the reasults.
 * @param initialX  inital x axsis vaule
 * @param initialY inital y axsis vaule
 * @param initialVelX inital velocity in x axsis vaule
 * @param initialVelY inital velocity in y axsis vaule
 * @param delta time interval that passed between each iteration.
 * @return 0 if all went well, -1 if couldn't close the file in the end.
 */
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

        velocityX = linearAdding(velocityX, accelerationX, dt);
        velocityY = linearAdding(velocityY, accelerationY, dt);


        if (i % whenToPrint == 0)
        {
            if (fprintf(destinationFile, "%.3Le,%.3Le,", x, y) < 0)
            {
                fprintf(stderr, "couldn't write to output file");
            }
        }

    }
    if (fclose(destinationFile))
    {
        perror("Problem closing destination file");
        return -1;
    }
    return 0;

}

/**
 * check the logic of the input
 * @param time the inputted time
 * @param totalSteps total steps inputed
 * @param stepsToSave total steps to save inputed
 * @return 0 if all parameters are good , -1 if not.
 */
int checkVariableLogic(long double time, long totalSteps, long stepsToSave)
{
    if (time <= 0)
    {
        fprintf(stderr, "Time cannot be negative or zero!");
        return -1;
    }

    if (totalSteps >0 && stepsToSave >0)
    {
        if (totalSteps % stepsToSave != 0)
        {
            fprintf(stderr, "Steps to save does not divide total steps evenly!");
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "Steps must be a non-negative,non-zero number!");
        return -1;
    }
    return 0;
}


int main(int argc, char *argv[])
{
    long numbersOfSteps, stepsToSave, whenToPrint;
    long double x, y , velocityX, velocityY, totalTime, dt;
    char *destinationFileName = NULL;
    FILE *destinationFile;

    // getting the data.
    if (argc == 2)
    {
        destinationFileName = argv[1];
        if (startingDataFromUserInput(&x, &y, &velocityX, &velocityY, &totalTime, &numbersOfSteps, &stepsToSave))
        {
            fprintf(stderr, "Parsing of parameters failed");
            return 1;
        }
    }
    else if (argc == 3)
    {
        destinationFileName = argv[2];
        if (startingDataFromFile(argv[1], &x, &y, &velocityX, &velocityY, &totalTime, &numbersOfSteps, &stepsToSave))
        {
            if (errno == 0)
            {
                fprintf(stderr, "Problem parsing from file");
            }
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Invalid number of arguments");
        return 1;
    }
    destinationFile = fopen(destinationFileName, "w");
    if (destinationFile == NULL)
    {
        perror("Problem with destination file");
        return errno;
    }

    //sanity check and final pre-calculation.
    if (checkVariableLogic(totalTime, numbersOfSteps, stepsToSave))
    {
        return 2;
    }
    whenToPrint = numbersOfSteps / stepsToSave;
    dt = totalTime / numbersOfSteps;

    //the good stuff
    mainLoop(numbersOfSteps, whenToPrint, destinationFile, x, y, velocityX, velocityY, dt);
    return 0;
}
