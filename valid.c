#include <stdio.h>
#include "cachestruct.h"

char validReplaceAlg(char *);
char validWritePolicy(char *);

/*checks to see if inputs are valid, and then stores cache parameters into currsim)*/
int validArgs(char *cachesize,char *associativity,char *blocksize,char *replacealg,char *writepolicy,Cache *currsim) {
  int cacheSize = 0;
  int setSize = 0;
  int blockSize = 0;
  int numSets = 0;
  char replaceAlg;
  char writePolicy;

  cacheSize = validCacheSize(cachesize);
  if(!cacheSize)
    return 0;
  currsim->cacheSize = cacheSize;

  setSize = validAssociativity(associativity);
  if(!setSize)
    return 0;
  currsim->setSize = setSize;

  blockSize = validBlockSize(blocksize);
  if(!blockSize)
    return 0;
  currsim->blockSize = blockSize;

  numSets = cacheSize/(setSize*blockSize);
  if(numSets == 0)
    return 0;
  currsim->numSets = numSets;

  replaceAlg = validReplaceAlg(replacealg);
  if(replaceAlg = '\0')
    return 0;
  currsim->replaceAlg = replaceAlg;

  writePolicy = validWritePolicy(writepolicy);
  if(writePolicy == '\0')
    return 0;
  currsim->writePolicy = writePolicy;

  return  1;
}

int validCacheSize(char *cachesize) { 
  int cacheSize;
  int numScanned = sscanf(cachesize,"%d",&cacheSize);
  if(numScanned = 0)
    return 0;
  if(isPow2(cacheSize))
    return cacheSize;
  return 0;
}

int validAssociativity(char *assoc) {
  int setSize;
  if(strcmp(assoc,"-direct") == 0)
    setSize = 1;
  else if(strncmp(assoc,"-assoc:",7) == 0) {
    setSize = strtol(assoc,assoc+7,10);
    if(!isPow2(setSize))
      setSize = 0;
    }
  return setSize;
}

int validBlockSize(char *blocksize) {
  int blockSize;
  int numScanned = sscanf(blocksize,"%d",&blockSize);
  if(numScanned == 0)
    return 0;
  if(isPow2(blockSize))
    return blockSize;
  return 0;
}

char validReplaceAlg(char *replacealg) {
  char replaceAlg;
  if(strcmp(replacealg,"-LRU") == 0)
    replaceAlg = 'l';
  else if(strcmp(replacealg,"-FIFO") == 0)
    replaceAlg = 'f';
  return replaceAlg;
}

char validWritePolicy(char *writepolicy) {
  char writePolicy;
  if(strcmp(writepolicy,"-wt") == 0)
    writePolicy = 't';
  else if(strcmp(writepolicy,"-wb") == 0)
    writePolicy = 'b';
  return writePolicy;
}

/*Utility function that checks if a number is a power of 2
 * Useful for checking cache size, block size, set size, and number of sets values*/
int isPow2(int x) {
  int i;
  for(i = 0; i < 31; i++) {
    if(x == 1<<i)
      return 1;
  }
  return 0;
}
