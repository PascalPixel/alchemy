#ifndef ALCHEMY_GLOBAL_CELLS_H
#define ALCHEMY_GLOBAL_CELLS_H

#include "TYPES.H"

/* Heap cache slot8 and the subsequent cached heap cells. */
extern void *gMapWork[];

/* Byte views of IWRAM work cells whose pointer is read through a cast. */
extern u8 Data_03001e74_a[];
extern u8 Data_03001f2c_a[];

#endif
