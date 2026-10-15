#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "party_state.h"
#include "field_event.h"

s32                GameFlag_Test(s32 flag);
void               GameFlag_SetBit(s32 flag);
struct FieldActor *Object_GetById(s32 id);
void               ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void               ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void               WaitFrames(s32 frames);
void               Func_0200097c(void);
void               Func_02001010(void);
void               Func_02001018(void);
void               Func_02001060(s32 id, s32 x, s32 z);
void               Func_020010b8(s32 id, s32 value);
void               Func_02001108(s32 value);
void Func_02001120(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY, s32 layer);

static inline void MapCopy(
    s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY, s32 layer)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each stack argument in its own register */
    Func_02001120(srcX, srcY, width, height, destX, destY, layer);
}

static inline void Place(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep the native argument order */
    ObjectMotion_ResetAndSetPosition(id, x, z);
}

void Func_0200077c(void)
{
    if (GameFlag_Test(0x820) && GameFlag_Test(0x9b1) && !GameFlag_Test(0x9ad))
        Func_0200097c();
}

void Func_020007ac(void)
{
    struct FieldActor *object;
    s32                moved;

    MapCopy(9, 23, 1, 1, 7, 23, 0);
    if (!GameFlag_Test(63))
    {
        object = Object_GetById(gPartyState.currentOwner);
        Func_02001010();
        Func_02001108(0);
        moved = 0;
        if ((object->x >> 20) == 7 && (object->z >> 20) == 23)
        {
            ObjectMotion_Launch(gPartyState.currentOwner, 6, 0);
            Place(gPartyState.currentOwner, 104, 376);
            moved = 1;
        }
        Func_02001060(14, 120 << 16, 376 << 16);
        WaitFrames(1);
        ObjectMotion_Launch(14, 4, 10);
        GameFlag_SetBit(0x300);
        if (moved)
            Func_020010b8(gPartyState.currentOwner, 0);
        Func_02001018();
    }
}

void Func_0200085c(void)
{
    MapCopy(9, 23, 1, 1, 4, 10, 0);
}

void Func_0200087c(void)
{
    MapCopy(9, 23, 1, 1, 6, 9, 0);
}

void Func_0200089c(void)
{
    MapCopy(9, 23, 1, 1, 6, 6, 0);
}

void Func_020008bc(void)
{
    MapCopy(12, 5, 1, 1, 12, 6, 0);
}

void Func_020008dc(void)
{
    MapCopy(9, 23, 1, 1, 13, 7, 0);
}

void Func_020008fc(void)
{
    MapCopy(9, 23, 1, 1, 14, 6, 0);
}

void Func_0200091c(void)
{
    MapCopy(9, 23, 1, 1, 21, 6, 0);
}

void Func_0200093c(void)
{
    MapCopy(9, 23, 1, 1, 22, 6, 0);
}

void Func_0200095c(void)
{
    MapCopy(9, 23, 1, 1, 23, 7, 0);
}
