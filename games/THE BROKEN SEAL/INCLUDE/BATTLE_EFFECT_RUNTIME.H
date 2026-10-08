#ifndef ALCHEMY_BATTLE_EFFECT_RUNTIME_H
#define ALCHEMY_BATTLE_EFFECT_RUNTIME_H

#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "PALBUF.H"

struct BattleAnimationState {
    u8 unknown_00[0x28];
    s16 *value_28;
};

struct BattleRenderObject {
    u8 unknown_00[0x24];
    s32 unknown_24;
    u8 unknown_28[4];
    s32 unknown_2c;
    s32 unknown_30;
    s32 unknown_34;
    s32 unknown_38;
    u8 unknown_3c[4];
    s32 unknown_40;
    u8 unknown_44[0x0c];
    struct BattleAnimationState *animation;
    u8 kind;
};

struct BattleRuntime {
    u8 unknown_000[0x1c8];
    s32 unknown_1c8;
    s32 mode_1cc;
    u8 unknown_1d0[0x0a];
    u16 unknown_1da;
    s16 unknown_1dc;
    s16 unknown_1de;
    u8 unknown_1e0[0x14];
    s32 object_id;
    s32 unknown_1f8;
    u8 unknown_1fc[0xaba];
    s16 unknown_cb6;
    u8 unknown_cb8[0x0a];
    s16 unknown_cc2;
    s16 unknown_cc4;
};

struct BattleEffectEntry {
    u16 id;
    u8 value;
    u8 flags;
};

extern struct BattleEffectBuffers *Data_03001ed0;

void BattleFx_SpawnLinked(s32 resource_id, s32 flags, s32 state);

#endif
