#include "TYPES.H"

extern u8 Data_02009940[];
extern s32 Data_02009928[];
extern s32 Data_02002224[];

void LinkLobby_WriteSlotValue(s32 slot)
{
    s32 *dst = Data_02002224;
    s32 *src = &Data_02009928[slot];
    dst[Data_02009940[slot]] = *src;
}
