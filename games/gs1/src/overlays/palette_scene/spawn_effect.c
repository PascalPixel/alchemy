#include "palette_scene.h"

extern s32 Data_03001e40;
extern s32 Data_02009dd0;
extern u8 Data_02009d9c[];

struct EffectSprite { u8 pad00[9]; u8 flags; u8 pad0a[28]; u8 state; };
struct PaletteEffect {
    u8 pad00[0x18];
    s32 progress;
    u8 pad1c[7];
    u8 flags;
    u8 pad24[12];
    s32 rate_x;
    s32 rate_y;
    u8 pad38[24];
    struct EffectSprite *sprite;
    u8 pad54[1];
    u8 mode;
};

void Func_02002ce2(s32);
struct PaletteEffect *Func_02002b84(s32, s32, s32, s32);
void Func_02002bb4(struct PaletteEffect *, s32);
void Func_02002be4(struct PaletteEffect *, s32, s32, s32);
void Func_02002bd4(struct PaletteEffect *, u8 *);

void PaletteScene_SpawnEffect(void) {
    struct PaletteEffect *effect;
    struct EffectSprite *sprite;
    s32 phase;
    s32 effect_flags;
    s32 sprite_flags;
    s32 spawn_x = 0x01460000;
    s32 spawn_y = 0x00200000;
    s32 spawn_z = 0x00c00000;
    s32 target_x = 0x01460000;
    s32 target_z = 0x00f00000;

    phase = Data_03001e40 & 3;
    if (phase != 0) return;
    if (Data_02009dd0 != 0) Func_02002ce2(200);
    effect = Func_02002b84(26, spawn_x, spawn_y, spawn_z);
    if (effect == 0) return;
    sprite = effect->sprite;
    sprite->state = phase;
    effect_flags = 0xfe;
    effect_flags &= effect->flags;
    effect->flags = effect_flags;
    sprite_flags = ~12;
    sprite_flags &= sprite->flags;
    sprite_flags |= 4;
    sprite->flags = sprite_flags;
    effect->progress = 0x1999;
    effect->rate_x = 0x40000;
    effect->rate_y = 0x40000;
    effect->mode = phase;
    Func_02002bb4(effect, 2);
    Func_02002be4(effect, target_x, 0, target_z);
    Func_02002bd4(effect, Data_02009d9c);
}
