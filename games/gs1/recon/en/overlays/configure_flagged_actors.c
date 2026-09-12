#include "types.h"

extern s16 Data_02000240_t[][1];
extern u8 Value_00000000;
extern void Scene_PlaceSlots(void);
extern s32 Scene_CheckFlag(s32);
extern void Scene_SetFlag(s32);
extern void Scene_SetMode(s32, s32);
extern void Scene_SetPosition(s32, s32, s32);
extern void Scene_SetFacing(s32, s32, s32);
extern u8 *Scene_GetActor(s32);
extern void Scene_SetTiles(s32, s32, s32, s32, s32, s32);

static __inline__ s32 Check(s32 flag)
{
    return Scene_CheckFlag(flag);
}

static __inline__ void Position(s32 slot, s32 x, s32 z)
{
    Scene_SetPosition(slot, x, z);
}

static __inline__ void Facing(s32 slot, s32 angle, s32 mode)
{
    Scene_SetFacing(slot, angle, mode);
}

/*
 * resource_3ae owner at 0x02000958, 378 bytes: 340 bytes of code around a
 * mid-function five-word literal pool at 0x02000a82, with an unconditional
 * branch over it, then the trailing 0x950 block.
 *
 * TWO COMPILER FACTS ARE LOAD-BEARING HERE.
 *
 * The zero stored into actor 11's record[35] is not an ordinary integer
 * literal. The reference materialises it with a pool load, and the pool sits
 * in the middle of the function rather than after the return. Both follow
 * from a HImode constant: the Thumb halfword move declares pool_range 60, so
 * its pool must be dumped within 60 bytes of the reference, which forces the
 * mid-function dump and the branch that jumps over it. An SImode constant
 * declares pool_range 1020 and puts the pool at the end, six bytes short.
 *
 * The mask 12 and this zero occupy r5 and r6 in that order. The zero is
 * assigned before the surrounding block so that its live range is long
 * enough for the mask to be allocated first; assigning it immediately before
 * the Scene_GetActor call reverses the pair.
 *
 * RESIDUAL, THREE HALFWORDS AT 0x02000a64-0x02000a6a. The reference schedules
 * the pool load after the Scene_GetActor(11) call; this draft emits it
 * before. Size, registers, pool contents, pool position and every opcode
 * match. Source placement of the assignment on either side of the call does
 * not move it, and neither does the cast width, the neighbouring block
 * spelling, nor the declaration order: the post-reload list scheduler ranks
 * the pool load above the call in every spelling tried. Not adopted.
 */
void FieldScene_ConfigureFlaggedActors(void)
{
    u8 *record;
    s32 none;

    Scene_PlaceSlots();
    if (Check(0x950) != 0)
        Scene_SetMode(12, 2);
    if (Data_02000240_t[225][0] == 3)
        Scene_SetFlag(0x12f);
    if (Data_02000240_t[225][0] == 1)
        Scene_SetFlag(0x8aa);
    if (Check(0x8aa) != 0) {
        Position(8, 0x1980000, 0x1280000);
        Facing(8, 0x8000, 0);
    }
    if (Check(0x8ab) != 0) {
        Position(13, 0x1180000, 0x1280000);
        Facing(13, 0xc000, 0);
        Position(16, 0x1200000, 0x1180000);
        Facing(16, 0xe000, 0);
        Position(10, 0xe80000, 0x1300000);
        Facing(10, 0x4000, 0);
        Position(11, 0xf00000, 0x1380000);
        Facing(11, 0xc000, 0);
        record = Scene_GetActor(10);
        record[89] = 0;
        record[35] = 2;
        (*(u8 **)(record + 80))[9] |= 12;
        (*(u8 **)(record + 80))[38] = 0;
        none = (u16)&Value_00000000;
        {
            s32 target = *(s32 *)(record + 80);
            s32 shown = 0xc000;
            *(u16 *)(target + 30) = shown;
        }
        record = Scene_GetActor(11);
        record[35] = none;
        (*(u8 **)(record + 80))[9] |= 12;
        (*(u8 **)(record + 80))[21] |= 12;
    }
    if (Check(0x950) != 0) {
        s32 col = 14;
        s32 row = 18;
        Scene_SetTiles(18, 18, 1, 1, col, row);
        Scene_SetTiles(18, 18, 1, 1, 15, row);
    }
}
