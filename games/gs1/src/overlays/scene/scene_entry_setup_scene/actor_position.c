#include "types.h"

#define SceneActor_SwapPositionsByDepth Func_02000910
#define FieldScene_SetFlag987AtActorTwelveTile Func_02000ff0
#define SceneActor_ApplyKind45AtActorsElevenAndTwelve Func_0200199c
#define SceneActor_ApplyPositionsOfActors11And12 Func_020019c0
#define SceneActor_UpdateSlots11And12ByTile Func_02001a9c
#define SceneActor_SetHeightAboveLinkedRecord Func_020022a4
struct Position {
    u32 filler00[2];
    s32 x;
    s32 y;
    s32 z;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_0ff0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_199c;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct_19c0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
} Struct_1a9c;

typedef struct {
    s16 filler0[50];
    s32 unk64;
} Struct_22a4;

typedef struct {
    s32 filler0[3];
    s32 unkC;
} Struct_22a4b;

struct Position *Func_020057c2(s32);
struct Position *Func_020057ca(s32);
void Func_020056e8(s32);
Struct_0ff0 *Func_02005ea2(s32);
void Func_02005e90(void);
void Func_02005e8e(s32);
void Func_02005eb2(void);
Struct_199c *Func_02006852(s32);
void Func_02006818(s32, s32, s32, s32);
Struct_19c0 *Func_02006876(s32);
void Func_02006842(s32, s32, s32, s32);
void Func_02006934(void);
Struct_1a9c *Func_02006952(s32);
void Func_02002400(void);
void Func_020034ce(Struct_1a9c *);
void Func_02003512(Struct_1a9c *);
Struct_1a9c *Func_02006974(s32);
void Func_02002422(void);
void Func_020034f0(Struct_1a9c *);
void Func_02003534(Struct_1a9c *);
void Func_02006984(void);
Struct_22a4b *Func_0200715e(s32);

void SceneActor_SwapPositionsByDepth(s32 group, s32 index)
{
    struct Position *first;
    struct Position *second;
    s32 value;

    first = Func_020057c2(group);
    second = Func_020057ca(index);
    if (first->z <= second->z) {
        value = first->x;
        first->x = second->x;
        second->x = value;

        value = first->y;
        first->y = second->y;
        second->y = value;

        value = first->z;
        first->z = second->z;
        second->z = value;
        Func_020056e8(1);
    }
}

void FieldScene_SetFlag987AtActorTwelveTile(void)
{
    Struct_0ff0 *s;

    s = Func_02005ea2(12);
    Func_02005e90();
    if (s->unk8 >> 20 == 54 || s->unk10 >> 20 == 6) {
        Func_02005e8e(0x987);
    }
    Func_02005eb2();
}

void SceneActor_ApplyKind45AtActorsElevenAndTwelve(void)
{
    u32 i;
    Struct_199c *p;

    i = 0;
    do {
        p = Func_02006852(i + 11);
        i++;
        Func_02006818(0, p->unk8, p->unk10, 45);
    } while (i <= 1);
}

void SceneActor_ApplyPositionsOfActors11And12(void)
{
    u32 i;
    Struct_19c0 *p;

    i = 0;
    do {
        p = Func_02006876(i + 11);
        if (p->unkC > -0x100000) {
            Func_02006842(0, p->unk8, p->unk10, 255);
        }
        i++;
    } while (i <= 1);
}

void SceneActor_UpdateSlots11And12ByTile(void)
{
    Struct_1a9c *o;

    Func_02006934();
    o = Func_02006952(11);
    if (o->unk8 >> 20 == 8) {
        Func_02002400();
        Func_020034ce(o);
    } else {
        Func_02003512(o);
    }
    o = Func_02006974(12);
    if (o->unk8 >> 20 == 7) {
        Func_02002422();
        Func_020034f0(o);
    } else {
        Func_02003534(o);
    }
    Func_02006984();
}

s32 SceneActor_SetHeightAboveLinkedRecord(Struct_22a4 *obj)
{
    Struct_22a4b *rec;

    rec = Func_0200715e(((s16 *)obj)[50]);
    ((s32 *)obj)[3] = rec->unkC + 0x100000;
    return 0;
}
