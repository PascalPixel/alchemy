#include "TYPES.H"

typedef struct {
    u8 bytes[0xB4];
} Data_080844ec_Record;

extern Data_080844ec_Record Character_DefinitionTable[];

Data_080844ec_Record *Owner_GetRecordStride180(s32 index)
{
    Data_080844ec_Record *base;

    base = Character_DefinitionTable;
    return &base[index];
}
