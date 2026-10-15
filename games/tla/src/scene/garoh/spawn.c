#include "garoh.h"
#include "field_sprite.h"

struct SpawnObject
{
    u8                  unk00[0x50];
    struct FieldSprite *sprite;
    u8                  unk54;
    u8                  motionFlags;
    u8                  unk56[8];
    u16                 unk5e;
};

extern u8 gData_02006330[];

void                Func_02005164(struct SpawnObject *object, s32 mode);
void                Func_0200516c(struct SpawnObject *object, const void *script);
struct SpawnObject *Func_02005174(s32 type, s32 x, s32 y, s32 z);

void Func_0200132c(void)
{
    s32 b = 10;
    s32 c = 11;

    /* FAKEMATCH: the native builds the first argument last, after the second and third; without the barrier it is built first (one reordered instruction, differ 2). */
    __asm__ volatile("" : "+r"(b), "+r"(c));
    Func_02005354(1, b, c);
    Flag_Set(0x203);
}

s32 Func_02001344(void)
{
    if (!Flag_IsSet(0x91b))
        return Flag_IsSet(0x91e);
}

// DriftScene_SpawnObject の双子: 五つ目の引数を 0x5e に置く
void Func_02001360(s32 x, s32 y, s32 z, s16 value, s32 timer)
{
    struct SpawnObject *object;
    struct FieldSprite *sprite;

    object = Func_02005174(0x1e8, x, y, z);
    if (object != 0)
    {
        sprite = object->sprite;
        Func_02005164(object, 1);
        Func_0200516c(object, gData_02006330);
        object->motionFlags = 0;
        /* FAKEMATCH: the native loads the stack argument before the pooled zero for nextFrame; without the barrier the zero is loaded first (one reordered pair, differ 2). */
        __asm__ volatile("");
        object->unk5e = timer;
        sprite->nextFrame = 0;
        sprite->rotation = value;
    }
}
