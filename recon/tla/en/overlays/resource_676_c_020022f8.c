/* Near miss (26 instructions differ, one register swap): the native keeps
 * the object in r6 and the step in r7, the draft the other way round.
 * Tried: loops counting up, the step as i * 40, declaring the counters
 * first; the swap stays. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern char SceneId_Ov676Room1;

struct TileSprite {
    u8 unk00[18];
    u16 tile;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02002450(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02002458(struct ObjectRuntime *object);
void Func_02002470(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020025a0(s32 cue);
void Func_020023c8(void);

static __inline__ void Func_02002450_(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002450(object, x, y, z);
}

static __inline__ void Func_02002470_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02002470(a, b, c, d, e, f);
}

void Func_020022f8(void)
{
    s32 step;
    s32 i;
    struct ObjectRuntime *object = Object_GetById(9);
    struct TileSprite *sprite = object->animation;

    GameFlag_SetBit(0x300);
    object->acceleration = 0x18000;
    object->speedLimit = 0x80000;
    Func_020025a0(227);
    Func_02002450_(object, 0xb00000, 0x280000, 0xb80000);
    step = 0;
    for (i = 15; i >= 0; i--) {
        sprite->tile -= step;
        WaitFrames(1);
        step += 40;
    }
    Func_02002450_(object, 0xa80000, 0x100000, 0xb80000);
    step = 400;
    for (i = 25; i >= 0; i--) {
        sprite->tile -= step;
        WaitFrames(1);
        step += 40;
    }
    Func_02002458(object);
    Battle_WaitMode0(2);
    Func_020025a0(240);
    sprite->tile = 0;
    object->x = 0xa80000;
    object->z = 0xc00000;
    object->y = 0;
    object->velocityY = 0;
    object->velocityX = 0;
    Func_02002470_(8, 12, 1, 1, 10, 12);
}
