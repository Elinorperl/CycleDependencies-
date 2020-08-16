	/**
	* @file CheckDependency.c
	* @author Elinor Perl <elinor.perl@mail.huji.ac.il>
	* @version 1.0
	* @date August 24, 2016
	*
	* @brief A system that checks file dependency.
	*
	* @section LICENSE
	* This program is brought to you by Hebrew University - Course C. :)
	*
	* @section DESCRIPTION
	* This program converts a number from one base to another.
    Input : Input is the file to be checked for cyclic dependency
	* Process: The file is parsed according to file and it’s dependency - then using the DFS algorithm checks dependency.
	* Output : The program’s output refers to whether the file input is cyclic or not.
	*/

	// 	includes  	
	#include<stdio.h>
	#include<string.h>
	#include<stdlib.h>
	#include<assert.h>


	// 	const definitions  	
	/**
	* @def LINESIZE 1001
	* @brief the size for one line
	*/
	#define LINESIZE 1001
	/**
	* @def VALID 0
	* @brief valid output
	*/
	#define VALID 0
	/**
	* @def INVALID 1
	* @brief invalid output
	*/
	#define INVALID 1
	/**
	* @def END_OF_LINE ’\0’
	* @brief signifies the end of a line
	*/
	#define END_OF_LINE ’\0’
	/**
	* @def ENTER ", \n"
	* @brief signifies a new line
	*/
	#define ENTER ", \n"
	/**
	* @def COLON ":"
	* @brief a colon which parses the file from its dependencies
	*/
	#define COLON ":"
	/**
	* @def VALID_FILE_AMOUNT 2
	* @brief the amount of arguments expected in order to continue with the program


	*/
	#define VALID_FILE_AMOUNT 2
	/**
	* @def CYCLE_DETECTED "Cyclic Dependency\n"
	* @brief indicates when file cycle has been detected
	*/
    #define CYCLE_DETECTED "Cyclic Dependency\n"
	/**
	* @def NO_CYCLE_DETECTED "No cyclic dependency\n"
	* @brief indicates when no file cycle has been detected
	*/
	#define NO_CYCLE_DETECTED "No cyclic dependency\n"
	/**
	* @def FILE_ERROR "Error trying to open file!"
	* @brief the string to be printed when an invalid amount of arguments have been input.
	*/
	#define FILE_ERROR "Error trying to open file!"
	/**
	* @def LENGTH_OF_FILE_NAME 256
	* @brief The maximum length a file name can be
	*/
	#define LENGTH_OF_FILE_NAME 256
	/**
	* @def AMOUNT_OF_NEIGHBORS 100
	* @brief The maximum amount of neighbors for a dependent file
	*/
	#define AMOUNT_OF_NEIGHBORS 100
	/**
	* @def AMOUNT_OF_DEPENDENCIES 1000
	* @brief The maximum amount of dependencies
	*/
	#define AMOUNT_OF_DEPENDENCIES 1000

	// 	functions  	

	/**
	* An enum called marker - it defines the marker for the DFS algorithm.
	*/
	enumMarker
	{
	WHITE,
	GRAY,
	BLACK
	};

	/**
	* Dependent File struct - defines out the dependent file should look and act with an array of dependent files
	* regarding their neighbors (possible dependencies).
	*/
	typedefstruct DependentFile
	{
	char_fileName[LENGTH_OF_FILE_NAME];
	structDependentFile*_fileNeighbors[AMOUNT_OF_NEIGHBORS];
	intfileIndex;
	enumMarker marker;
	intnumberOfDependencies;

	} DependentFile;
	DependentFile dependencies[AMOUNT_OF_DEPENDENCIES];
	intfileAmount;


	/**
	* @brief gets the desired file.
	* @param name the name of the file for which we’d like to return
	* @return returns the specific file if it should exist - otherwise NULL.
	*/
	DependentFile*getFile(char*name)


	{
	for(intindex=0; index<fileAmount; index++)
	{
    if(strcmp(dependencies[index]._fileName, name)==0)
	{
 	return &dependencies[index];
	}
	}
	return NULL;
	}

	/**
	* @brief removes the ’\n’ from a given line.
	* @param line the line to change.
	*/
	voidremoveEnter(char*line)
	{
	line[strlen(line)-1]=END_OF_LINE;
	}

	voidinitializeFile(DependentFile file)
	{
	file.marker=WHITE;
	file.numberOfDependencies=0;
	file.fileIndex=fileAmount;
	dependencies[fileAmount]=file;
	}

	/**
	* @brief reads and processes the file, initializing it and adding it to the dependencies’ array
	* @param file the file to process
	*/
	voidreadFile(FILE*file)
	{
	charline[LINESIZE];
	fileAmount=0;
	while(fgets(line, LINESIZE, file))
	{
	removeEnter(line);
	char*pointer=strtok(line, COLON);
	if(!pointer)
	{
	continue;
	}
	DependentFile*pointerFile=getFile(pointer);
	if(pointerFile==NULL)
	{
	initializeFile(dependencies[fileAmount]);
	strcpy(dependencies[fileAmount]._fileName, pointer);
	fileAmount++;
	}
	}
	rewind(file);
	}

	/**
	* @brief checks and process the dependencies for each file and adds them to the the relevant neighbor array.
	* @param file the file to be checked
	*/
	voidcheckDependencies(FILE*file)
	{
	charline[LINESIZE];
	char*currentFile;
	char*curDep;
	DependentFile*d;
	DependentFile*neighbor;
	while(fgets(line,sizeof(line), file))
	{


	currentFile=strtok(line, COLON);
	if(!getFile(currentFile))
	{
	continue;
	}
	d=getFile(currentFile);
	curDep=strtok(NULL, ENTER);
	while(curDep)
	{
	neighbor=getFile(curDep);
	if(neighbor)
	{
	(*d)._fileNeighbors[(*d).numberOfDependencies]=neighbor;
	(*d).numberOfDependencies++;
	}
	curDep=strtok(NULL, ENTER);
	}
	}
	}

	/**
	* @brief the DFS algorithm - that determines the cycles by marking the files we’ve already visited.
	* @param filePosition the index of the file we are checking.
	*/
	voiddfsHelper(intfilePosition)
	{
	dependencies[filePosition].marker=GRAY;
	for(inti=0; i<dependencies[filePosition].numberOfDependencies; i++)
	{
	DependentFile dependency=*(dependencies[filePosition]._fileNeighbors[i]);
	if(dependency.marker==GRAY)
	{
	printf (CYCLE_DETECTED);
	exit(VALID);
	}
	if(dependency.marker==WHITE)
	{
	dfsHelper((*dependencies[filePosition]._fileNeighbors[i]).fileIndex);
	}
	}
	dependencies[filePosition].marker=BLACK;


	}
	/**
* @brief The DFS cycle detector - iterating over each file and sending its position to the DFS algorithm to be checked.
	*/
	voiddfsDetectCycles()
	{
	for(inti=0; i<fileAmount; i++)
	{
	if(dependencies[i].marker==WHITE)
	{
	dfsHelper(i);
	}
	}
	}

	/**
	* @brief The main function that runs the program by processing the file and its dependencies and sending it to the DFS
	* algorithm to be checked for Cyclic dependencies
	* @param argc the number of arguments given
	* @param argv the argument - in our case ine argument with the file of files.
	* @return returns 0 for a valid (that is to say no cyclic dependency), and 1 for invalid - (cyclic dependency).
	*/
	int main(intargc,char*argv[])
	{
	if(argc!=VALID_FILE_AMOUNT)


	{
	fprintf(stderr, FILE_ERROR);
	exit(1);
	}
	FILE*file;
	file=fopen(argv[1],"r");
	if(file)
	{
	readFile(file);
	checkDependencies(file);
	dfsDetectCycles();
	printf(NO_CYCLE_DETECTED);
	fclose(file);
	return VALID;
	}
	return INVALID;	
}
