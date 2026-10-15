#include "shrine_of_the_sea_god.h"

struct KabeEffectParams {
    u8 unknown_00[0x18];
    u16 kind;
    u8 unknown_1a[0xe];
};

struct KabePartyExtra {
    u8 unknown_000[0x256];
    s16 value;
};

extern u32 gFrameCount;

s32 Random16Far(void);
void Func_02003b18(void);
void Func_02003b20(s32 a, s32 b, s32 c, s32 d);
void Func_02003988(void);
void Func_02000994(void);
void Func_02000ce8(void);
struct KabeObject *Func_02002f88(s32 id);
void Func_020000b8(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase, s32 life,
                   struct KabeEffectParams *params);
void Func_02003854(s32 id);

void Func_02003740(void)
{
    Func_02003854(8);
    Func_02003854(9);
    Func_02003854(10);
    if (gPartyState.entrance == 3 && GameFlag_Test(265) == 0)
        Func_02000994();
}

void Func_0200377c(void)
{
    if (gPartyState.entrance == 1 && GameFlag_Test(265) == 0)
        Func_02000ce8();
}

void Func_020037a4(struct KabeObject *object)
{
    struct KabeEffectParams params;
    u32 phase = gFrameCount & 7;
    s32 dx;
    s32 dy;

    if (phase == 0) {
        {
            /* FAKEMATCH: the native builds the mask before the random draw. */
            s32 mask = 15;

            dx = mask & Random16Far();
        }
        dy = Random16Far() & 7;
        params.kind = 673;
        Func_020000b8(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16), object->z, 0,
                      0x8000, phase, 0x100000, &params);
    }
}

void Func_0200380c(void)
{
    struct KabeObject *object = Object_GetById(64);
    struct KabeMapWork *map = (struct KabeMapWork *)Ram_HeapSlots->map_work;
    struct KabeMapLayer *layer = &map->layers[2];

    layer->x += 0x800000;
    Func_02003988();
    WaitFrames(1);
    if (object != NULL) {
        object->motion_flags = 0;
        object->y = 0x420000;
        object->update = Func_020037a4;
    }
}

void Func_02003854(s32 id)
{
    struct KabeObject *object = Object_GetById(id);

    ObjectMotion_SetActionVariant(id, 3);
    Func_02003aa0(id);
    object->priority_flags |= 2;
    object->motion_flags = 0;
    object->field_18 = 0xcccc;
    object->field_1c = 0xe666;
}

s32 Func_02003894(void)
{
    s32 angle;

    if (GameFlag_Test(0x340)) {
        struct KabePartyExtra *party = (struct KabePartyExtra *)&gPartyState;

        /* FAKEMATCH: loads the party base before it builds the field offset. */
        __asm__("");
        if (party->value == 1) {
            if (GameFlag_Test(0x341))
                Kabe_ObjPlaceAt(14, 360 << 16, 216 << 16, 0);
            else if (GameFlag_Test(0x342)) {
                /* FAKEMATCH: the function-scope angle temporary preserves its early constant construction; a direct argument through the unbound helper differs 4. */
                angle = 0x8000;
                Kabe_ObjPlaceTurn(14, 328 << 16, 216 << 16, angle);
            }
        }
        GameFlag_ClearBit(0x340);
        GameFlag_ClearBit(0x341);
        GameFlag_ClearBit(0x342);
    }
    return 0;
}
