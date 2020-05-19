
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "memAlloc.h"
#include "bookshelf_IO.h"



    char **nameCell = NULL;
    
    // from readAuxFile()
    char *nodesFile = NULL;
    char *netsFile = NULL;
    char *wtsFile = NULL;
    char *sclFile = NULL;
    char *plFile = NULL;
    char *benchmarkName = NULL;
    
    // from readNodesFile()
    int movableNodes, numberTerminals;
    float averageCellWidth;
    float *cellWidth = NULL;
    float *cellHeight = NULL;
    float *cellArea = NULL;
    
    // from readNetsFile() 
    int numberNets, numberPins;
    int *netlist = NULL;
    int *netlistIndex = NULL;
    float *xPinOffset = NULL;
    float *yPinOffset = NULL;
    char **netName = NULL;
    
    // from readPlFile()
    float *xCellCoord = NULL;
    float *yCellCoord = NULL;
    char **cellOrientation = NULL;
    
    // from readSclFile()
    ROW *core_row = NULL;
    int numberRows, numRowberBlockages;
    float siteOriginY, siteEndY, coreHeight;
    float siteOriginX, siteEndX, coreWidth;
    float minX, maxX, minY, maxY;
    float coreRowHeight;
    float *xRowBlockage = NULL;
    float *yRowBlockage = NULL;
    float *widthRowBlockage = NULL;
    
    // from readWtsFile()
    float *netWeight = NULL;
    
    
/*-- global variables --*/
    typedef struct nodesHash NODES;
    struct nodesHash  {
        char *name;
        unsigned long index;
    };
    NODES **NodesInfo = NULL;
    NODES **NetsInfo = NULL;
    long hashSize, hashBits;
    unsigned long *RN = NULL;
    unsigned char *hashFlag = NULL;
    
    int numNodes;
    char error_text[BUFFERSIZE];
	
	*-- functions --*/
 
    void readAuxFile(char benchmarkPath[], char auxFile[]);
    void readNodesFile(char benchmarkPath[], char nodesFile[]);
    void readNetsFile(char benchmarkPath[], char netsFile[]);
    void readPlFile(char benchmarkPath[], char plFile[]);
    void readSclFile(char benchmarkPath[], char sclFile[]);
    void readWtsFile(char benchmarkPath[], char wtsFile[], int net_weights);
    void writePlFile(char outputDir[], char fileName[], float xCoord[], float yCoord[], 
                     int totalNodes);
