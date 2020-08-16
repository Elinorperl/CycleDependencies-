/**
 * @file CheckParenthesis.c
 * @version 1.0
 * @date 7.8.16
 *
 * @brief Program to check the structure of the parenthesis of an input file
 *
 * @section LICENSE
 * This program is not a free software;
 *
 * @section DESCRIPTION
 * The system checks the structure of the parenthesis of the file.
 * Input  : A file to check.
 * Process: Intilaizing an array and a pointer ("stack"), reads the file and add the open
 * parenthesis, removes the close parenthesis. In the end of the file (or when a problem is
 * found) prints a message to the user about the state of the file.
 * Output : Notification.
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// -------------------------- const definitions -------------------------

/**
 * @def FILE_PROCESSED_SUCCESSFULLY 0
 * @brief A macro that presents the output of the main function when there weren't any problems
 * with the file.
 */
#define FILE_PROCESSED_SUCCESSFULLY 0

/**
 * @def ERROR -1
 * @brief A macro that presents an error notification.
 */
#define ERROR -1

/**
 * @def ERROR_OPENING_FILE_MESSAGE "Error! trying to open the file %s\n"
 * @brief A macro that presents the output when the program isn't able to read the file.
 */
#define ERROR_OPENING_FILE_MESSAGE "Error! trying to open the file %s\n"

/**
 * @def ERROR_WRONG_NUMBER_OF_INPUTS "Please supply a file! usage: CheckDependency <filename>\n"
 * @brief A macro that presents the output when there are more/less argument in the arguments'
 * array.
 */
#define ERROR_WRONG_NUMBER_OF_INPUTS "Please supply a file! usage: CheckDependency <filename>\n"

/**
 * @def FILE_INPUT 1
 * @brief A macro that presents the index of the file path in the arguments' array.
 */
#define FILE_INPUT 1

/**
 * @def CORRECT_NUM_OF_ARGUMENTS 2
 * @brief A macro that presents the number of the arguments that the user supposes to insert.
 */
#define CORRECT_NUM_OF_ARGUMENTS 2



/**
 * @def MAX_LENGTH_NAME 256
 * @brief A macro that presents the number of the arguments that the user supposes to insert.
 */
#define MAX_LENGTH_NAME 256

/**
 * @def MAX_NUM_OF_NEIGHBORS 100
 * @brief A macro that presents the number of the arguments that the user supposes to insert.
 */
#define MAX_NUM_OF_NEIGHBORS 100

/**
 * @def MAX_NUM_OF_GRAPH_FILES 1000
 * @brief A macro that presents the number of the arguments that the user supposes to insert.
 */
#define MAX_NUM_OF_GRAPH_FILES 1000

/**
 * @def MAX_LENGTH_OF_LINE 1001
 * @brief A macro that presents the number of the arguments that the user supposes to insert.
 */
#define MAX_LENGTH_OF_LINE 1001

/**
 * @def NAMES_PATTERN_IN_THE_FILE ": , \n"
 * @brief A macro that presents the number of the arguments that the user supposes to insert.
 */
#define NAMES_PATTERN_IN_THE_FILE ": ,\n"

/**
 * @def NO_CYCLIC_MESSAGE "No Cyclic dependency\n"
 * @brief A macro that presents the number of the arguments that the user supposes to insert.
 */
#define NO_CYCLIC_MESSAGE "No Cyclic dependency\n"

/**
 * @def CYCLIC_MESSAGE "Cyclic dependency\n"
 * @brief A macro that presents the number of the arguments that the user supposes to insert.
 */
#define CYCLIC_MESSAGE "Cyclic dependency\n"

#define DEBUG

/**
 * @brief
 */
typedef enum
{
    WHITE,
    GREY,
    BLACK
} Group;

/**
 * @brief
 */
typedef struct _DependentFile
{
    int _indexFile;
    char _name[MAX_LENGTH_NAME];
    struct _DependentFile* _fileNeighbors[MAX_NUM_OF_NEIGHBORS];
    char** _neighborsNames;
    int _numOfNeighbors;
    Group _group;
} _DependentFile;

/**
 *
 */
_DependentFile dependencies[MAX_NUM_OF_GRAPH_FILES];

/**
 *
 */
int numOfFiles;

// ------------------------------ functions -----------------------------

/**
 * @brief
 * @param name
 * RETURN VALUE:
 *  @return
 */
static _DependentFile* getFile(const char* name)
{
    int i = 0;
    for (i = 0; i < numOfFiles; i++)
    {
        if (strcmp(dependencies[i]._name, name) == 0)
        {
            return &(dependencies[i]);
        }
    }
    return NULL;
}

/**
 * @brief
 * RETURN VALUE:
 *  @return
 */
static _DependentFile initializeFile()
{
    _DependentFile currentFile;
    currentFile._numOfNeighbors = 0;
    currentFile._group = WHITE;
    currentFile._neighborsNames = (char**)malloc(MAX_NUM_OF_NEIGHBORS*sizeof(char*));
    currentFile._indexFile = numOfFiles;
    return currentFile;
}

/**
 * @brief
 * @param pLine
 * @param indexFile
 */
static void addingNameOfNeighbor(char* pLine, int indexFile)
{
    pLine = strtok(NULL, NAMES_PATTERN_IN_THE_FILE);
    while (pLine != NULL)
    {
        dependencies[indexFile]._neighborsNames[dependencies[indexFile]._numOfNeighbors] =
                (char*)malloc(MAX_LENGTH_NAME * sizeof(char));
        strcpy(dependencies[indexFile]._neighborsNames[dependencies[indexFile]._numOfNeighbors],
               pLine);
        dependencies[indexFile]._numOfNeighbors++;
        pLine = strtok(NULL, NAMES_PATTERN_IN_THE_FILE);
    }
}

/**
 * @brief
 * @param file
 */
static void processFile(FILE * file)
{
    numOfFiles = 0;
    char line[MAX_LENGTH_OF_LINE];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *p = NULL;
        p = strtok(line, NAMES_PATTERN_IN_THE_FILE);
        _DependentFile *pNewFile = getFile(p);
        if (pNewFile == NULL)
        {
            _DependentFile currentFile = initializeFile();
            strcpy(currentFile._name, p);
            dependencies[numOfFiles] = currentFile;
            addingNameOfNeighbor(p, currentFile._indexFile);
            numOfFiles++;
        }
        else
        {
            addingNameOfNeighbor(p, (*pNewFile)._indexFile);
        }
    }
}

/**
 * @brief returns a pointer to the file from the list
 */
static void buildGraph()
{
    int indexFile = 0;
    for (indexFile = 0; indexFile < numOfFiles; indexFile++)
    {
        int indexNeighbor = 0;
        int currentIndexNeighbor = 0;
        int numOfNeighbors = dependencies[indexFile]._numOfNeighbors;
        for(indexNeighbor = 0; indexNeighbor < dependencies[indexFile]._numOfNeighbors;
            indexNeighbor++)
        {
            _DependentFile* neighbor = getFile(dependencies[indexFile]._neighborsNames[indexNeighbor]);
            free(dependencies[indexFile]._neighborsNames[indexNeighbor]);
            dependencies[indexFile]._neighborsNames[indexNeighbor] = NULL;
            if (neighbor == NULL)
            {
                numOfNeighbors--;
            }
            else
            {
                dependencies[indexFile]._fileNeighbors[currentIndexNeighbor] = neighbor;
                currentIndexNeighbor++;
            }
        }
        dependencies[indexFile]._numOfNeighbors = numOfNeighbors;
        free(dependencies[indexFile]._neighborsNames);
        dependencies[indexFile]._neighborsNames = NULL;
    }
}

/**
 * @brief
 * @param fileIndex
 */
static void dfsCheckDependencyHelper(const int fileIndex)
{
    dependencies[fileIndex]._group = GREY;
    int i = 0;
    for (i = 0; i < dependencies[fileIndex]._numOfNeighbors; i++)
    {
        _DependentFile neighbor = *(dependencies[fileIndex]._fileNeighbors)[i];
        if (neighbor._group == GREY)
        {
            printf(CYCLIC_MESSAGE);
            exit(FILE_PROCESSED_SUCCESSFULLY);
        }
        if (neighbor._group == WHITE)
        {
            dfsCheckDependencyHelper((*((dependencies[fileIndex])._fileNeighbors)[i])._indexFile);
        }
    }
    dependencies[fileIndex]._group = BLACK;
}

/**
 * @brief
 */
static void dfsCheckDependency()
{
    int i = 0;
    for (i = 0; i < numOfFiles; i++)
    {
        if (dependencies[i]._group == WHITE)
        {
            dfsCheckDependencyHelper(i);
        }
    }
}

/**
 * @brief This program recieves a file and checks if the parenthesis structure is correct or not.
 * @param argc the number of the cmd arguments (including the name of this project)
 * @param argv the array of the cmd arguments, this case the argument is the file path
 * RETURN VALUE:
 *  @return 0 if the file was ok and the program successes process it, otherwise, it returns 1 -
 *  which means there is an error, and it prints an informative massage about the kind of the
 *  problem.
 */
int main(int argc, char *argv[])
{
    if (argc != CORRECT_NUM_OF_ARGUMENTS)
    {
        fprintf(stderr, ERROR_WRONG_NUMBER_OF_INPUTS);
        return ERROR;
    }
    FILE *file;
    file = fopen(argv[FILE_INPUT], "r");
    if (file)
    {
        processFile(file);
        fclose(file);
        buildGraph();
        dfsCheckDependency();
        printf(NO_CYCLIC_MESSAGE);
        return FILE_PROCESSED_SUCCESSFULLY;
    }
    else
    {
        fprintf(stderr, ERROR_OPENING_FILE_MESSAGE, argv[FILE_INPUT]);
        return ERROR;
    }
}
