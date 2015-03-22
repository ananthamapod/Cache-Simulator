#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "cachestruct.h"

void printStats();

typedef struct {
  int tag;
  int minBlock;
  int maxBlock;
  int valid;
  int dirty;
} Line;

typedef struct {
  Line* baseLine;
  int size;
} Set;




FILE *trace;
int reads;
int writes;
int hits;
int misses;




int simulate(Cache *cache,char* tracefile) {
/*iterators for loops*/
  int i = 0;
  int j = 0;

/*string variables for reading from the tracefile*/
  char line[50];
  char eip[20];
  char mode;
  int address;

/*set pointers*/
  Set *currSet;
  Set *baseSet;

/*current Line pointer*/
  Line *currLine;

/*Cache dimensions, stored locally for convenience*/
  int setSize = cache->setSize;
  int numSets = cache->numSets;
  int blockSize = cache->blockSize;
  int writePolicy = cache->writePolicy;
  int replaceAlg = cache->replaceAlg;

/*variables for holding tag, set, and block offset values for each address accessed*/
  int tag;
  int set;
  int blockOffset;

/*number of bits for each part of cache address*/
  int setBits;
  int blockBits;

/*masks for isolating parts of address*/
  int blockMask;
  int setMask;
  int tagMask;

/*e.g. blockSize = 4 = 0b0100.
**4-1 = 3 = 0b0011, and two bits are needed to represent 4 values*/
blockMask = blockSize - 1;
/*finds numbers of bits needed for setMask*/
  blockBits = log2(blockSize);
  setBits = log2(numSets);
/*e.g. blockSize = 4 = 0b100, numSets = 4 = 0b100.
**calculation of numSets takes care of associativity.
**4-1 = 3 - 0b0011. <<blockBits becomes 0b100*/
  setMask = (numSets - 1) << blockBits;
/*e.g. if setMask = 0b00111100 and blockMask = 0b00000011
**-1 = 0b11111111,so -1^setMask = 0b11000011, and that ^blockMask = 0b11000000
**where ^ is the XOR operator*/
  tagMask = -1^setMask^blockMask;

  trace = fopen(tracefile,"r");
  if(!trace) {
    fprintf(stderr,"Could not open tracefile");
    return -1;
  }
/*dynamic allocation for array of Sets*/
  baseSet = (Set *)malloc(numSets*sizeof(Set));
  for(i = 0; i < numSets; i++) {
    currSet = baseSet+i;
    currSet->baseLine = (Line *)malloc(setSize*sizeof(Line));
  }
  currSet = baseSet;

/*main part of cache simulation*/
/*reads in line*/
  while(fgets(line,50,trace) != NULL) {
/*parses into mode, address, etc*/
    sscanf(line,"%s %c %x",eip,&mode,&address);
/*gets the set, tag, and block offset values using the masks*/
    blockOffset = address&blockMask;
    set = (address&setMask) >> blockBits;
    tag = ((address&tagMask) >> blockBits) >> setBits;


    currSet = baseSet+set;
    for(i = 0; i< setSize; i++) {
      currLine = currSet->baseLine + i;
/*skips over blocks with invalid data*/
/*      if(currLine->valid != 1) {
        continue;
      }
*/
/*if tag matches, it's a hit*/
      if(currLine->tag == tag) {
        hits++;
/*writes directly through for write-through cache*/
        if(mode == 'W') {
          if(writePolicy == 't')
            writes++;
          else
/*writes only the cache and sets the dirty bit in the written line*/
            currLine->dirty = 1;
        }
        else
          reads++;
      }
      else {
        misses++;
/*if dirty is set, writes before evicting
 * (dirty is only applicable for the write-back cache in this implementation)*/
        if(currLine->dirty == 1) {
          writes++;
          currLine->dirty = 0;
        }
/*evicts and reads in new block (currently only direct cache is implemented)*/
        currLine->tag = tag;
        currLine->minBlock = blockOffset-blockOffset%blockSize;
        currLine->maxBlock = currLine->minBlock+blockSize;
        currLine->valid = 1;
        if(mode == 'W') {
          if(writePolicy == 't') {
/*reads and writes through to memory immediately, so both read and write*/
            reads++;
            writes++;
          }
          else {
/*reads but holds off on writing until eviction time (or some other time like instruction halting
 * or the computer turning off*/
            reads++;
            currLine->dirty = 1;
          }
        }
      }
    }
  }

/*free all Set and Line instances to avoid memory leak*/
  for(i = 0; i < numSets; i++) {
    currSet = baseSet+i;
    free(currSet->baseLine);
  }
  free(baseSet);
  fclose(trace);
  printStats();
  return 0;
}

void printStats() {
  printf("Memory reads : %d\n",reads);
  printf("Memory writes : %d\n",writes);
  printf("Cache hits : %d\n",hits);
  printf("Cache misses : %d\n",misses);
  return;
}

/*Utility log2 function*/
int log2(int x) {
  int i = 0;
  for(i = 0; i < 31; i++) {
    if((x >> i) == 1)
      break;
  }
  return i;
}
