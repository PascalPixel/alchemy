#ifndef ALCHEMY_GLOBAL_CELLS_H
#define ALCHEMY_GLOBAL_CELLS_H

#include "types.h"

/* Address-bearing names keep edition-specific IWRAM layout in the linker. */
extern u8 Data_03001ae8_a[];
extern u8 Data_03001e70_a[];
extern u8 Data_03001e74_a[];
extern u8 Data_03001e8c_a[];
extern u8 Data_03001ebc_a[];
extern u8 Data_03001eec_a[];
extern u8 Data_03001f1c_a[];
extern u8 Data_03001f2c_a[];
extern u8 Data_03001f30_a[];

#define ADDR_03001AE8 ((u32)Data_03001ae8_a)
#define ADDR_03001E70 ((u32)Data_03001e70_a)
#define ADDR_03001E74 ((u32)Data_03001e74_a)
#define ADDR_03001E8C ((u32)Data_03001e8c_a)
#define ADDR_03001EBC ((u32)Data_03001ebc_a)
#define ADDR_03001EEC ((u32)Data_03001eec_a)
#define ADDR_03001F1C ((u32)Data_03001f1c_a)
#define ADDR_03001F2C ((u32)Data_03001f2c_a)
#define ADDR_03001F30 ((u32)Data_03001f30_a)

#endif
