/* Draft of overlay 6aa Func_0200196c and the five that follow it
 * (Func_02001988, _19a4, _19c4, _19e0, _1a00), one call each. differ 2 of
 * 11 (8 of 12 for the two with 0x4000): the listing builds the id in r0
 * last, after r1 to r3; a direct call and this inline build it first. An
 * inline with the id as its last parameter and an s16 id gave the same
 * order. Func_02000070 shows the same r0-last order. */
#include "types.h"

extern u8 Data_0200547c[];
extern u8 Data_020054cc[];
extern u8 Data_02005528[];
extern u8 Data_02005578[];
extern u8 Data_020055d4[];
extern u8 Data_02005644[];

void Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *data);

static inline void Path(s32 id, s32 a1, s32 a2, s32 a3, const void *data)
{
    Func_02002f4c(id, a1, a2, a3, data);
}

void Func_0200196c(void)
{
    Path(10, 19, 20, 0, Data_0200547c);
}

void Func_02001988(void)
{
    Path(12, 19, 20, 0, Data_020054cc);
}

void Func_020019a4(void)
{
    Path(9, 18, 20, 0x4000, Data_02005528);
}

void Func_020019c4(void)
{
    Path(9, 13, 14, 0, Data_02005578);
}

void Func_020019e0(void)
{
    Path(11, 13, 14, 0x4000, Data_020055d4);
}

void Func_02001a00(void)
{
    Path(13, 21, 22, 0, Data_02005644);
}
