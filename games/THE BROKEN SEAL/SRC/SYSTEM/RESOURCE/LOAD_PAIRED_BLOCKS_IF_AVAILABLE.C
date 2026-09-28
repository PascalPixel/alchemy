#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001f2c[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s32 Resource_FindFreeEntry(void);
extern s32 VramBlock_LoadCached(s32, s32, u8 *);
extern u8 Data_080aed4c[];
extern u8 Data_080aedcc[];

void Resource_LoadPairedBlocksIfAvailable(void)
{
    s32 first_slot;
    s32 second_slot;
    void *active_state;

    active_state = *(void **)((u32)&Data_03001f2c);
    first_slot = Resource_FindFreeEntry();
    FIELD_AT_OFFSET(active_state, s16, 0x392) = (s16)first_slot;
    if (first_slot != -1) {
        VramBlock_LoadCached(first_slot, 0x80, Data_080aed4c);
    }
    second_slot = Resource_FindFreeEntry();
    FIELD_AT_OFFSET(active_state, s16, 0x394) = (s16)second_slot;
    if (second_slot != -1) {
        VramBlock_LoadCached(second_slot, 0x80, Data_080aedcc);
    }
}
