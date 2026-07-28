#ifndef ALCHEMY_BATTLE_EFFECT_RUNTIME_H
#define ALCHEMY_BATTLE_EFFECT_RUNTIME_H

#include "types.h"

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
    s32 unknown_1cc;
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

struct BattleWork {
    u8 unknown_000[0x1f2];
    u8 mode_1f2;
    u8 unknown_1f3;
    s32 object_id;
    s32 unknown_1f8;
    u8 unknown_1fc[0x0e];
    u8 enabled_20a;
};

struct BattleEffectBuffers {
    s16 primary_value;
    u8 unknown_002[0x37e];
    u8 buffer_380[0xa80];
    u8 buffer_e00[0xa80];
    u8 buffer_1880[0x1181];
    u8 mode_2a01;
    u8 unknown_2a02;
};

struct BattleEffectEntry {
    u16 id;
    u8 value;
    u8 flags;
};

extern struct BattleRuntime *Data_03001ebc;
extern struct BattleEffectBuffers *Data_03001ed0;
extern struct BattleWork Data_02000240;

struct BattleRenderObject *Func_0808ba1c();

#endif
