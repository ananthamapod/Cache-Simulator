#include <stdio.h>
#include "cachestruct.h"
#include "simulate.h"
#include "valid.h"

int main(int argc, char** argv) {
  int valid;
/*struct that holds the parameters governing current simulation*/
  Cache currsim;
  switch(argc) {
    case 2 :
/*help command*/
      if(strcmp(argv[1],"-h") == 0) {
        help();
        return  0;
      }
      break;
    case 7 :
/*has valid number of arguments, but must check for validity of arguments themselves*/
      valid = validArgs(argv[1],argv[2],argv[3],argv[4],argv[5],&currsim);

      if(valid) {
        if(currsim.setSize > 1) {
          fprintf(stderr,"ERROR: not implemented:");
          return -1;
        }
        simulate(&currsim,argv[6]);
        return 0;
      }
      else {
        break;
      }
    default :
      break;
  }
  fprintf(stderr,"Invalid arguments. Use 'sim -h' for more information\n about using sim cache simulator.\n");
  return -1;
}
int help() {
  printf("Usage:");
  printf("\n\t sim <cache size> <associativity> <block size> <replacement algorithm> <write policy> <tracefile.txt>");
  printf("\nMore information about arguments:");
  printf("\n\n<cache size>:");
  printf("\n\t must be a power of 2 integer represented in decimal(base 10)");
  printf("\n\n<associativity>:");
  printf("\n\t -direct\t simulates of a direct-mapped cache");
  printf("\n\t assoc:n\t simulates an n-way associative cache where n is an integer that is a power of 2");
  printf("\n\n<block size>:");
  printf("\n\t must be a power of 2 integer represented in decimal(base 10)");
  printf("\n\n<replacement algorithm>:");
  printf("\n\t -FIFO\t simulates first-in-first-out replacement");
  printf("\n\t -LRU\t simulates least-recently-used replacement");
  printf("\n\n<write policy>:");
  printf("\n\t -wt\t simulates a write-through cache");
  printf("\n\n<tracefile>:");
  printf("\n\t Must be a text file with the memory access trace of a program.");
  printf("\n\t Each line in the file must be formatted as follows:");
  printf("\n\t 'instruction pointer value'':'<space>'W or R signifying read or write'<space>'memory address accessed'");
  printf("\n");
  return 0;
}
