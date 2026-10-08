#ifndef TLA_OVERLAY_687_OV687_H
#define TLA_OVERLAY_687_OV687_H

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"

struct Ov687Sprite {
    u8 unknown_00[0x1a];
    u8 visible;
};

struct Ov687Object {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 field_18;
    s32 scale;
    u8 unknown_20[2];
    u8 layer;
    u8 priority_flags;
    u8 unknown_24[0x2c];
    struct Ov687Sprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
};

struct Ov687MapLayer {
    u8 unknown_00[0x30];
    u8 *cells;
    u8 unknown_34[4];
};

struct Ov687MapWork {
    u8 unknown_000[0x108];
    struct Ov687MapLayer layers[3];
};

struct Ov687EventWork {
    u8 unknown_000[0x16a];
    s16 alarm;
    u8 unknown_16c[0x40];
    s32 screen_effect;
};

struct Ov687Spark {
    s32 mode;
    s32 kind;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_10[8];
    s16 life;
    u8 unknown_1a[0xe];
};

extern u32 gFrameCount;
extern volatile u32 Data_020022c8;
extern u32 Data_020022cc;
extern u32 gSceneState[];
extern u8 gMapCellBuffer[];

struct Ov687Object *Object_GetById(s32 id);
struct Ov687Object *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct Ov687Object *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct Ov687Object *object, s32 value);
void Object_SetPartAttribute(struct Ov687Object *object, s32 value);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
u32 Random16Far(void);
s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Battle_WaitMode0(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001f8c(s32 a, s32 b, s32 c);
void Func_02001fac(void);
void Func_02001fb4(void);
void Func_02001fdc(s32 actor, s32 x, s32 z);
void Func_02001fec(s32 speed, s32 acceleration);
void Func_02001ffc(void);
void Func_02002004(s32 countdown);
void Func_02002024(s32 a, s32 b);
void Func_02002044(s32 skip);
void Func_02002054(s32 cue);

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d, struct Ov687Spark *spark);
s32 Func_0200034c(u32 layer, s32 x, s32 y);
s32 Func_02000388(u32 layer, s32 x, s32 y, s32 value);
s32 Func_020003d0(s32 *a, s32 *b);
void Func_0200141c(s32 x, s32 z, s32 actor);
void Func_020018a8(s32 x, s32 z, s32 actor);

static inline s32 Ov687_FlagChk(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void Ov687_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void Ov687_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void Ov687_Tint(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001f8c(a, b, c);
}

static inline s32 Ov687_Cos(s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return Math_Cosine(angle);
}

static inline s32 Ov687_Sin(s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return Math_Sine(angle);
}

#endif
