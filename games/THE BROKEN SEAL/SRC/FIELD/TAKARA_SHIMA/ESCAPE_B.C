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

void RetreatActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Object_GetByIdFar(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Object_GetByIdFar(14) + 16) >> 20;
    companion18Row = *(s32 *)(Object_GetByIdFar(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 12) <= 2) {
            PositionSceneActorPair(14, -16, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            PositionSceneActorPair(14, -64, 0);
        } else {
            PositionSceneActorPair(14, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 12) <= 2) {
            PositionSceneActorPair(14, -48, 0);
        } else {
            PositionSceneActorPair(14, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        PositionSceneActorPair(14, -48, 0);
    } else if (column == 8) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        PositionSceneActorPair(14, -32, 0);
    } else if (column == 6) {
        return;
    }

    Task_Wait(2);

    rowM1 = row - 1;
    Map_CopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Object_GetByIdFar(14) + 8) >> 20, rowM1);
    Map_CopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Object_GetByIdFar(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Object_GetByIdFar(14) + 16) >> 20;
    companion18Row = *(s32 *)(Object_GetByIdFar(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;

    if (column == 6) {
        if ((u32)(companion9Row - 12) <= 2) {
            PositionSceneActorPair(14, 32, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            PositionSceneActorPair(14, 64, 0);
        } else {
            PositionSceneActorPair(14, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        PositionSceneActorPair(14, 80, 0);
    } else if (column == 9) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        PositionSceneActorPair(14, 64, 0);
    } else if (column == 12) {
        PositionSceneActorPair(14, 16, 0);
    } else if (column == 13) {
        return;
    }

    Task_Wait(2);

    rowM1 = row - 1;
    Map_CopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Object_GetByIdFar(14) + 8) >> 20, rowM1);
    Map_CopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void RetreatActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Object_GetByIdFar(16) + 8) >> 20;
    permuted_7 = *(s32 *)(Object_GetByIdFar(16) + 16) >> 20;
    companion18Row = *(s32 *)(Object_GetByIdFar(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 9) <= 2) {
            PositionSceneActorPair(16, -16, 0);
        } else if ((u32)(companion18Row - 9) <= 2) {
            PositionSceneActorPair(16, -64, 0);
        } else {
            PositionSceneActorPair(16, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 9) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 9) <= 2) {
            PositionSceneActorPair(16, -48, 0);
        } else {
            PositionSceneActorPair(16, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 9) <= 2) {
            return;
        }
        PositionSceneActorPair(16, -48, 0);
    } else if (column == 8) {
        PositionSceneActorPair(16, -32, 0);
    } else if (column == 6) {
        return;
    }

    Task_Wait(2);

    rowM1 = row - 1;
    Map_CopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Object_GetByIdFar(16) + 8) >> 20, rowM1);
    Map_CopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companionRow;

    s32 permuted_6;
    column = *(s32 *)(Object_GetByIdFar(16) + 8) >> 20;
    permuted_6 = *(s32 *)(Object_GetByIdFar(16) + 16) >> 20;
    companionRow = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;
    row  = permuted_6;

    if (column == 6) {
        if ((u32)(companionRow - 9) <= 2) {
            PositionSceneActorPair(16, 32, 0);
        } else {
            PositionSceneActorPair(16, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companionRow - 9) <= 2) {
            return;
        }
        PositionSceneActorPair(16, 80, 0);
    } else if (column == 9) {
        PositionSceneActorPair(16, 64, 0);
    } else if (column == 12) {
        PositionSceneActorPair(16, 16, 0);
    } else if (column == 13) {
        return;
    }

    Task_Wait(2);

    row -= 1;
    Map_CopyCellAttributes(column, row, 1, 3,
                  *(s32 *)(Object_GetByIdFar(16) + 8) >> 20, row);
    Map_CopyCellAttributes(0, 0, 1, 3, column, row);
}

s32 UpdateSwayingSceneObject(struct S *object)
{

    struct T *sprite = object->f50;
    s32 vertical_offset = Math_Sin(object->f30) * 2;
    s32 random_b;
    s32 random_a;

    if (vertical_offset > 0) {
        vertical_offset = -vertical_offset;
    }
    object->f08 = object->f38 + Math_Cos(object->f30) * 2;
    object->f0c = object->f3c + vertical_offset;
    sprite->f1e = (u16)(Math_Cos(object->f30 + 0x8000) / 8);
    random_a = Random_Next();
    random_b = Random_Next();
    object->f30 += (((u32)(random_a << 9)) >> 16) + (((u32)(random_b << 9)) >> 16) + 0x400;
    return 0;
}

void InitializeSwayingSceneObject(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)UpdateSwayingSceneObject;
    actor->state = zero;
}
