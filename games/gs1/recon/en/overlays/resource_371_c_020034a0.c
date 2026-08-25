#include "types.h"

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
    u8 reserved_0a[28];
    u8 state;
};

struct SceneActor {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[15];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
    u8 reserved_54;
    u8 variant_55;
};

struct SceneEffect {
    u8 reserved_00[8];
    s32 x;
    u8 reserved_0c[4];
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[3];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
    u8 reserved_54;
    u8 variant_55;
};

extern u32 Data_03001e40;

u16 Func_02007678(void);
u16 Func_02007684(void);
u16 Func_020076a0(void);
struct SceneEffect *Func_020076d4(s32 kind, s32 x, s32 y, s32 z);
void Func_02007758(struct SceneEffect *effect, s32 mode);
void Func_02007768(struct SceneEffect *effect, const void *callback);
struct SceneActor *Func_020077ae(s32 slot);
void Func_020078f0(struct SceneEffect *effect, s32 presentation);

void Func_020034a0(void)
{
    struct Presentation *presentation;
    struct SceneEffect *effect;
    struct SceneActor *source;
    u32 random;

    if ((Data_03001e40 & 15) != 0) {
        return;
    }

    source = Func_020077ae(8);
    effect = Func_020076d4(
        222,
        source->x - 0x200000,
        source->y,
        source->z - 0x100000);
    if (effect == 0) {
        return;
    }

    effect->scale_x = 0x8000;
    effect->scale_y = 0x8000;
    presentation = effect->presentation;

    random = Func_02007678();
    random <<= 1;
    random = (u16)random;
    if (random != 0) {
        s32 offset = Func_02007684();

        offset *= 3;
        offset <<= 4;
        offset = (u16)offset;
        offset <<= 16;

        effect->x -= offset >> 1;
        effect->z -= offset;
    } else {
        s32 offset = Func_020076a0();

        offset <<= 5;
        offset = (u16)offset;
        offset <<= 16;

        effect->x += offset;
        effect->z += offset >> 1;
    }

    presentation->state = 0;
    presentation->flags =
        (presentation->flags & ~0x0c) |
        (source->presentation->flags & 0x0c);
    effect->state_23 |= 2;
    effect->variant_55 = source->variant_55;
    Func_020078f0(effect, 9);
    Func_02007758(effect, 2);
    Func_02007768(effect, (const void *)0x0200e73c);
}
