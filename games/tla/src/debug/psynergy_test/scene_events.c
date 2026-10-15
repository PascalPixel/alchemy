#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "party_state.h"

extern u8 gData_020017b0[];
extern u8 gData_02001860[];

void                  Func_02000f14(s32, s32, s32);
void                  Func_02000ebc(s32, s32);
void                  Func_02000dac(s32, s32, s32, s32, s32, s32);
void                  Func_02000e5c(s32, s32, s32);
void                  Func_0200092c(s32, s32, s32, s32, s32, s32, s32);
void                  Func_02000950(s32, s32, s32, s32);
void                  Func_020009c0(s32, s32, s32, s32);
void                  Func_02000a10(s32, s32, s32, s32, s32);
void                  Func_02000c4c(struct ObjectRuntime *, void *);
void                  Func_02000d4c(struct ObjectRuntime *);
void                  GameFlag_SetBit(s32);
struct ObjectRuntime *Object_GetById(s32);

void Func_02000660(void)
{
    struct PartyState *party;

    Func_02000f14(249, 40, 168);
    party = &gPartyState;
    /* FAKEMATCH: ⚓️ loads the party's address before it builds the field offset. */
    __asm__ volatile("" : "+l"(party));
    party->battleRule24b = 2;
    Func_02000ebc(14, 0);
}

void Func_02000688(s32 actor)
{
    GameFlag_SetBit(0x300);
    Func_0200092c(actor, 36, 28, 1, 1, 10, 10);
}

void Func_020006b0(s32 actor)
{
    GameFlag_SetBit(0x301);
    Func_02000950(actor, 18, 184, 184);
}

void Func_020006cc(s32 actor)
{
    GameFlag_SetBit(0x302);
    Func_020009c0(actor, 19, 200, 200);
}

void Func_020006e8(s32 actor)
{
    GameFlag_SetBit(0x303);
    Func_02000a10(actor, 216, 216, 14, 0);
}

void Func_0200070c(void)
{
    Func_02000e5c(19, ((u8 *)Object_GetById(19))[0x62], 0x310);
}

void Func_02000724(void)
{
    s32 frames;
    s32 flags;

    frames = 16;
    flags = 10;
    Func_02000dac(48, 26, 2, 4, frames, flags);
}

void Func_02000740(void)
{
}

void Func_02000744(void)
{
}

void Func_02000748(s32 actor)
{
    Func_02000c4c(Object_GetById(actor), gData_02001860);
    Func_02000c4c(Object_GetById(gPartyState.currentOwner), gData_020017b0);
    WaitFrames(100);
    Func_02000d4c(Object_GetById(actor));
    Func_02000d4c(Object_GetById(gPartyState.currentOwner));
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSceneEvents;
}
