#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"
#include "FIELD_EFFECT.H"

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

/* Integrate position, velocity, rate and sprite angle for one scene effect.
   Signed division preserves decay toward zero for negative Z velocity. */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 pad10[32];
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    u8 pad40[16];
    struct T *f50;
};

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

struct V6 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
};

struct S_02000474 {
    s32 f00;
    u16 f04;
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[20];
    s32 f28;
    u8 pad2c[4];
    s32 f30;
    s32 f34;
    u8 pad38[29];
    u8 f55;
};

struct V {
    s32 a;
    s32 b;
    s32 c;
};

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

struct S_020009dc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[15];
    u8 f23;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    u8 filler00[0x24];
    s32 callback;
};

typedef union {
    s32 w;
    s16 h[2];
} RecWord;

struct S_02001b14 { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };

struct S_02001bbc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001c84 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001d2c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001de0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001e5c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001ef4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001f78 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002004 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002080 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_0200216c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002200 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020022c8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020023c4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002450 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;
u8 *Object_GetByIdFar();


void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset);

void AdvanceActor18AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;

    s32 permuted_5;
    permuted_5 = *(s32 *)(Object_GetByIdFar(18) + 8) >> 20;
    row = *(s32 *)(Object_GetByIdFar(18) + 16) >> 20;
    column  = permuted_5;
    companion19Column = *(s32 *)(Object_GetByIdFar(19) + 8) >> 20;
    companion14Column = *(s32 *)(Object_GetByIdFar(14) + 8) >> 20;

    if (row == 9) {
        if ((u32)(companion14Column - 6) <= 2) {
            goto transition32;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            goto transition80;
        }
        /* This arm runs two transitions back to back. */
        PositionSceneActorPair(18, 0, 64);
        PositionSceneActorPair(18, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            PositionSceneActorPair(18, 0, 48);
        } else {
            PositionSceneActorPair(18, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion19Column - 6) <= 2) {
transition32:
            PositionSceneActorPair(18, 0, 32);
        } else {
            PositionSceneActorPair(18, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
transition80:
        PositionSceneActorPair(18, 0, 80);
    } else if (row == 15) {
        PositionSceneActorPair(18, 0, 64);
    } else if (row == 18) {
        PositionSceneActorPair(18, 0, 16);
    } else if (row == 19) {
        return;
    }

    Task_Wait(2);

    column -= 1;
    Map_CopyCellAttributes(column, row, 3, 1,
                  column, *(s32 *)(Object_GetByIdFar(18) + 16) >> 20);
    Map_CopyCellAttributes(0, 0, 3, 1, column, row);
}
