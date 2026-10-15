#include "types.h"

#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"

struct PartyState254 {
    u8 unk000[0x254];
    u16 value;
};

extern const u8 gData_020022e8[];
extern const u8 gData_0200236c[];

s32 GameFlag_Test(s32 flag);
s32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void Func_02001bec(struct ObjectRuntime *object, s32 mode);
void Func_02001ce4(s32 a);
void Func_02001cec(s32 a, s32 b);
void Func_02001d1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_02001d44(s32 a, s32 b, s32 c);

static __inline__ void Func_02001d1c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d1c(a, b, c, d, e, f, g);
}

static __inline__ void Func_02001d44_(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001d44(a, b, c);
}

void Func_02000054(void)
{
    Func_02001d1c_(69, 0, 4, 4, 67, 24, 1);
}

void Func_02000074(void)
{
    Func_02001d1c_(64, 0, 4, 3, 89, 28, 1);
}

void Func_02000094(void)
{
    Func_02001d1c_(64, 0, 4, 3, 89, 28, 1);
    if (!GameFlag_Test(0x9b8)) {
        Func_02001d44_(249, 424, 488);
        ((struct PartyState254 *)&gPartyState)->value = 0x19b8;
        gPartyState.battleRule24b = 2;
        Func_02001cec(106, 3);
    }
}

void Func_020000f8(void)
{
    Func_02001d1c_(69, 1, 4, 3, 108, 31, 1);
}

void Func_02000118(void)
{
    Func_02001ce4(4);
}

void Func_02000124(void)
{
    Func_02001ce4(6);
}

const u8 *Func_02000130(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov699Room2)
        return gData_0200236c;
    return gData_020022e8;
}

void Func_0200015c(struct ObjectRuntime *object)
{
    if (Random16Far() & 0xff) {
        s32 position[3];
        s32 radius;
        s32 angle;

        position[0] = 200 << 16;
        position[1] = -32 << 16;
        position[2] = 164 << 16;
        radius = Random16Far();
        angle = Random16Far();
        Vector_AddPolarOffsetFar((radius << 5) + 0x80000, angle, position);
        object->x = position[0];
        object->y = position[1];
        object->z = position[2];
    }
    object->unk66 = object->action;
    if (object->action == 212)
        Func_02001bec(object, 9);
    else
        Func_02001bec(object, 10);
}
