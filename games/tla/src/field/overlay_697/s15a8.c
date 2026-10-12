#include "ov697.h"

void Func_02002dec(void);
void Func_0200322c(void);

void Func_020015a8(void)
{
    GameFlag_SetBit(0x289);
}

void Func_020015b8(void)
{
    Func_02005a50_(68, 6, 1, 21, 13, 64, 64);
}

void Func_020015d8(void)
{
    Func_02005a50_(68, 6, 1, 21, 13, 64, 64);
    if (!GameFlag_Test(0x9b9)) {
        Func_02005a60(249, 72, 152);
        ((struct PartyState254 *)&gPartyState)->value = 0x19b9;
        gPartyState.battle_rule_24b = 2;
        Func_02005998(106, 4);
    }
}

s32 Func_02001638(void)
{
    return 1;
}

void Func_0200163c(void)
{
    Func_02005810_(33, 30, 1, 1, 10, 11);
    Func_02005810_(33, 30, 1, 1, 18, 7);
    Func_02005810_(33, 30, 1, 1, 18, 15);
    Func_02005810_(35, 30, 1, 1, 74, 11);
    Func_02005810_(35, 30, 1, 1, 82, 7);
    Func_02005810_(35, 30, 1, 1, 82, 15);
}

void Func_020016c8(void)
{
    Func_02005810_(34, 30, 1, 1, 10, 11);
    Func_02005810_(34, 30, 1, 1, 18, 7);
    Func_02005810_(34, 30, 1, 1, 18, 15);
    Func_02005810_(36, 30, 1, 1, 74, 11);
    Func_02005810_(36, 30, 1, 1, 82, 7);
    Func_02005810_(36, 30, 1, 1, 82, 15);
}

void Func_02001754(s32 sound)
{
    if (GameFlag_Test(0x9ff)) {
        if (gOv697State->mirrored == 0) {
            Map_Stamp(26, 79, 1, 2, 13, 79);
            Map_CopyCells(78, 18, 1, 1, 77, 18);
        } else {
            Map_Stamp(26, 79, 1, 2, 18, 79);
            Map_CopyCells(81, 18, 1, 1, 82, 18);
        }
        if (sound)
            Func_02005a68(138);
    }
    GameFlag_SetBit(0x283);
}

void Func_020017e0(void)
{
    struct PartyState *party;
    s32 count = GameFlag_Test(0x381);

    count += GameFlag_Test(0x382);
    count += GameFlag_Test(0x383);
    count += GameFlag_Test(0x384);
    if (GameFlag_Test(0x385))
        Func_02002dec();
    else
        Func_0200322c();
    /* FAKEMATCH: the one-pass block loads the party state address before it builds the field offset, and the empty one after the store keeps the scene id load behind it (differ 6 without both). */
    do {
        party = &gPartyState;
    } while (0);
    party->battle_rule_24b = 2;
    do { } while (0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov697Room2, 4);
    Party_SetFields1f2And1f4((s32)&SceneId_Ov697Room2, 5);
    Func_02005998(13, 4 - count);
}
