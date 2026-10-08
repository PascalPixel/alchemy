#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "field_event.h"
#include "field_sprite.h"

extern u8 Data_02002fb8[];
extern u8 Data_02003214[];
extern s32 Data_02003274;
extern s32 Data_02003278;
extern u8 Data_02002f70[];
extern u8 Data_02002f94[];
extern u32 gFrameCount;

void Func_020029d0();
void Func_020029d8();
struct FieldActor *Object_GetById();
s32 GameFlag_Test();
void GameFlag_SetBit();
void WaitFrames();
void Func_02000e04();
u32 __umodsi3();
void Animation_ApplyChildValues();
s32 Math_Cosine();
s32 Math_Sine();
u32 Random16Far();
struct FieldActor *Func_02002828();
void Func_02002820();
void Object_SetPartAttribute();
void Func_02002a40();
void Func_02001650();

struct SpriteFrames {
    u8 unknown_00[5];
    u8 frame;
};

struct AnimatedSprite {
    u8 unknown_00[0x19];
    u8 frame;
    u8 next_frame;
    u8 unknown_1b[0xd];
    struct SpriteFrames *frames;
};

void Func_02000054(void)
{
    Func_020029d0(Data_02002fb8);
}

void Func_02000064(s32 unused, s32 id)
{
    struct FieldActor *object;
    s32 i;

    object = Object_GetById(id);
    if (id == 11) {
        if (!GameFlag_Test(0xa00) || object->x >> 20 != 13 || object->z >> 20 != 34)
            return;
        object->motion_flags = 0;
        for (i = 15; i >= 0; i--) {
            object->y += 0x20000;
            WaitFrames(1);
        }
        Func_02000e04(id);
        GameFlag_SetBit(0xa09);
    } else {
        Func_020029d8();
    }
}

void Func_020000c8(struct FieldActor *object)
{
    struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;
    struct SpriteFrames *frames = sprite->frames;

    switch (object->timer & 3) {
    case 0:
        frames->frame = 7;
        sprite->frame = 1;
        sprite->next_frame = 2;
        break;
    case 1:
        sprite->frame = 1;
        sprite->next_frame = 0;
        break;
    case 2:
        frames->frame = 7;
        sprite->frame = 1;
        sprite->next_frame = 0;
        break;
    case 3:
        frames->frame = 0;
        sprite->frame = 1;
        sprite->next_frame = 0;
        break;
    }
    object->timer++;
}

void Func_02000120(struct FieldActor *object)
{
    u32 tick = gFrameCount;

    if (tick & 1)
        Animation_ApplyChildValues(object, (tick >> 1) % 6);
}

void Func_02000148(struct FieldActor *object)
{
    struct FieldActor *center;
    u16 angle;

    center = Object_GetById(23);
    angle = object->timer;
    object->x = center->x + Math_Cosine(angle) * (object->speed + 3);
    {
        s32 sine = Math_Sine(angle);
        s32 z = center->z;
        s32 x = object->x;
        s32 step;

        object->z = z + sine * 2;
        object->target_x = x;
        object->target_z = object->z;
        step = -0x800;
        asm volatile("" : "+l"(step)); /* FAKEMATCH: ⚓️ loads the step before the angle */
        object->timer += step;
    }
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

static inline s32 FlagChk(s32 flag)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameter keeps each flag's constant */
    return GameFlag_Test(flag);
}

#define Rand(range) ((Random16Far() * (range)) >> 16)

void Func_020001a4(void)
{
    struct FieldActor *center;
    struct FieldActor *spark;
    struct FieldSprite *sprite;
    s32 y;

    if (!FlagChk(0x201) && gFrameCount % 3 != 0)
        return;
    center = Object_GetById(23);
    if (FlagChk(0x201))
        y = (Rand(256) << 16) + center->y;
    else
        y = (Rand(64) << 16) + center->y;
    spark = Func_02002828(672, center->x, y - 0x1c0000, center->z);
    if (spark == NULL)
        return;
    sprite = spark->sprite;
    Func_02002820(spark, Data_02003214);
    Object_SetPartAttribute(spark, 1);
    spark->motion_flags = 0;
    spark->timer = ((Random16Far() << 4) >> 16) << 12;
    HALF_AT(spark, 102) = 0;
    WORD_AT(spark, 108) = (s32)Func_02000148;
    spark->speed = (Math_Sine((Random16Far() * 0xffff) >> 20) * 24) >> 16;
    BYTE_AT(sprite, 26) = 0;
    sprite->priority = 1;
}

void Func_02000294(void)
{
    if (Data_02003278 != 0)
        Func_02002a40(201);
    Func_02001650(Data_02003274);
}
