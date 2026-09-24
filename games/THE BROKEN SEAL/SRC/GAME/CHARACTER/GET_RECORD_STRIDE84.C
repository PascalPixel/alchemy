#include "TYPES.H"

extern const u8 Class_DefinitionTable[];

s32 Owner_GetRecordStride84(s32 arg0)
{
    return (s32)(Class_DefinitionTable + arg0 * 0x54);
}
