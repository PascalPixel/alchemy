#include "local.h"

struct MapCell
{
    u8 unk0[2];
    u8 unk2;
    u8 unk3;
};

struct MapWork170
{
    u8              unk000[0x170];
    struct MapCell *cells;
};

void Func_02001c8c(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    s32                   priority = ((struct FieldSprite *)leader->animation)->priority;

    ObjectMotion_SetActionVariant(13, priority);
    ObjectMotion_SetActionVariant(14, priority);
    ObjectMotion_SetActionVariant(15, priority);
}

void Func_02001cc4(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    struct MapWork170    *map = (struct MapWork170 *)Ram_HeapSlots->mapWork;

    struct MapCell *cell = &map->cells[Func_020053b0()];

    if (cell->unk2 != 0)
    {
        leader->y = 0;
        leader->terrainHeight = 0;
    }
}

void Func_02001d00(void)
{
    Func_020051c0();
    Func_02005370(0);
    UiText_ShowPositionedMessageAndWait((s32)&MsgKandoreanTemple10, 1);
    Func_020051c8();
}

void Func_02001d20(void)
{
    Func_020051c0();
    Func_02005370(0);
    UiText_ShowPositionedMessageAndWait((s32)&MsgKandoreanTemple01, 1);
    Func_020051c8();
}

void Func_02001d40(void)
{
    Func_020051c0();
    Func_02005370(0);
    ObjectMotion_EnableActionAndResetMotion(9);
    WaitFrames(1);
    ObjectMotion_SetAngleToward(9, gPartyState.currentOwner, 0);
    Func_02005280((s32)&MsgKandoreanTemple02);
    Func_020052a0(9, 0);
    Object_GetById(9)->angle = 0x3000;
    WaitFrames(1);
    ObjectMotion_EnableActionAndSetCallback(9, gData_02005514);
    WaitFrames(1);
    Func_020051c8();
    GameFlag_SetBit(0x845);
}

void Func_02001db4(void)
{
    Func_020051c0();
    Func_02005370(0);
    ObjectMotion_SetAngleToward(10, gPartyState.currentOwner, 0);
    Func_02005280((s32)&MsgKandoreanTemple03);
    Func_02005298(10, 0);
    Object_GetById(10)->angle = 0x5000;
    WaitFrames(1);
    Func_020051c8();
    GameFlag_SetBit(0x846);
}

void Func_02001e0c(void)
{
    Func_020051c0();
    Func_02005370(0);
    Func_02005280((s32)&MsgKandoreanTemple09);
    Func_020052a0(13, 0);
    Func_020051c8();
}

void Func_02001e30(void)
{
    Func_020052c8(9, 0x100, 0);
    Func_02001d40();
}

void Func_02001e44(void)
{
    Func_020052c8(10, 0x100, 0);
    Func_02001db4();
}

void Func_02001e58(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    s32                   x = leader->x >> 20;
    s32                   z = leader->z >> 20;
    struct PartyState    *party;
    u8                   *flags;
    s32                   zero;

    if (z == 5)
    {
        if (x == 9 && (gInput.held & 16) == 0)
            return;
        if (x == 11 && (gInput.held & 32) == 0)
            return;
    }
    else if (x == 10 && z == 6 && (gInput.held & 64) == 0)
    {
        return;
    }
    Func_020051c0();
    Func_02005370(0);
    /* FAKEMATCH: the flags address, the zero and gPartyState's address are taken before an empty asm, which holds the byte store below the address load as the native has it; the plain store goes first (differ 2). */
    flags = &leader->motionFlags;
    zero = 0;
    party = &gPartyState;
    asm("");
    *flags = zero;
    ObjectMotion_SetSpeedParameters(party->currentOwner, 0x10000, 0x8000);
    ObjectMotion_SetPositionAndReset(party->currentOwner, 168, 80);
    Func_020052b0(party->currentOwner, 0xc000);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(party->currentOwner), 0);
    Object_SetModeById(party->currentOwner, 13);
    Func_02005160(leader, 0xa80000, -0xc0000, 0x500000);
    Func_02005168(leader);
    Object_SetModeById(party->currentOwner, 10);
    Func_020053d8(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020052f8(8);
}
