#include "colosso_log_rolling_stage.h"

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_020091bc();
void Func_02009046();
s32 *Func_02008dc6();
void Func_02009078();
s32 *Func_02008df8();

extern s16 Data_02000240[];

typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

/* This overlay's own byte-exact occupancy lookup. */

s32 *ColossoLogRollingStage_FindActorAhead(void)
{
    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Func_020091bc(((ActiveSubjectSlot *)Data_02000240)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Func_02009046(0x100000, facing, position);          /* 128 << 13 */

    occupant = Func_02008dc6(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Func_02009078(0x200000, facing, position);      /* 128 << 14 */

        occupant = Func_02008df8(position, record);
    }

    return occupant;
}
