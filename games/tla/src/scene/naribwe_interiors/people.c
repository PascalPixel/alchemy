#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "text/msg_ids.h"

extern const u8 Data_02001370[];
extern const u8 Data_020014c0[];

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020010e0(void);
void Func_02001180(s32 a0);
void Func_02001138(s32 a0);
void Func_02001148(s32 a0, s32 a1);
void Func_020010e8(void);
void Func_02001190(s32 slot, s32 actor);
void Func_02001198(s32 slot, s32 actor);

const u8 *Func_020000a4(void)
{
    if (GameFlag_Test(0x8ff))
        return Data_020014c0;
    return Data_02001370;
}

void Func_020000c4(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: keeps the quadrant offset after the facing load (differ 2 without it; a quadrant temporary, inline helpers and step-by-step arithmetic measured the same). */
    do { } while (0);

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001190(14, actor);
    } else {
        Func_020010e0();
        Func_02001180(0);
        Func_02001138((s32)MsgIfYouAre);
        Func_02001148(actor, 0);
        Func_020010e8();
    }
}

void Func_02000124(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: keeps the quadrant offset after the facing load (differ 2 without it, as in Func_020000c4). */
    do { } while (0);

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001190(14, actor);
    } else {
        Func_020010e0();
        Func_02001180(0);
        Func_02001138((s32)MsgWheneverIGet);
        Func_02001148(actor, 0);
        Func_020010e8();
    }
}

void Func_02000184(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: keeps the quadrant offset after the facing load (differ 2 without it, as in Func_020000c4). */
    do { } while (0);

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001190(15, actor);
    } else {
        Func_020010e0();
        Func_02001180(0);
        Func_02001138((s32)MsgYouShouldVisit);
        Func_02001148(actor, 0);
        Func_020010e8();
    }
}

void Func_020001e4(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: keeps the quadrant offset after the facing load (differ 2 without it, as in Func_020000c4). */
    do { } while (0);

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001190(15, actor);
    } else {
        Func_020010e0();
        Func_02001180(0);
        Func_02001138((s32)MsgIfYouWish);
        Func_02001148(actor, 0);
        Func_020010e8();
    }
}

void Func_02000244(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: keeps the quadrant offset after the facing load (differ 2 without it, as in Func_020000c4). */
    do { } while (0);

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001198(5, actor);
    } else {
        Func_020010e0();
        Func_02001180(0);
        Func_02001138((s32)MsgOurCookingIs);
        Func_02001148(actor, 0);
        Func_020010e8();
    }
}

void Func_020002a4(s32 actor)
{
    u16 facing = Object_GetById(gPartyState.currentOwner)->angle;

    /* FAKEMATCH: keeps the quadrant offset after the facing load (differ 2 without it, as in Func_020000c4). */
    do { } while (0);

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001198(5, actor);
    } else {
        Func_020010e0();
        Func_02001180(0);
        Func_02001138((s32)MsgISeeSo);
        Func_02001148(actor, 0);
        Func_020010e8();
    }
}
