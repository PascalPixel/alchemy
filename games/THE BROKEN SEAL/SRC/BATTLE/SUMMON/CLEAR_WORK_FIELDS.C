#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001e74[];

/* battle/summon/clear_work_fields.c */
extern s16 gGameState[];

union Word {
    s32 value;
};

void Summon_ClearWorkFields(void)
{
    u8 *base;
    union Word *words;
    s16 *slots;
    s32 index;

    base = *(u8 **)((u32)&Data_03001e74);
    words = (union Word *)(base + 0x530);
    gGameState[286] = 0;
    words[0].value = 0;
    words[1].value = 0;
    words[2].value = 0;
    slots = (s16 *)(base + 0x53C);
    for (index = 3; index >= 0; index--)
        slots[index] = 0;
}
