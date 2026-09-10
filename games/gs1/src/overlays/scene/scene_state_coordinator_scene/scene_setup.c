#include "types.h"

#define SceneState_ApplyRectsForActorsNineAndTen Func_020001a8
#define FieldScene_RunTwoCallSequence Func_02000238
#define FieldScene_RunBranchedStep Func_02000954
#define SceneData_GetTablec420 Func_0200096c
#define FieldScene_BuildSceneDescriptorAndInstallTask Func_02003764
#define SceneState_InitControlWhenFlag109Clear Func_0200384c
typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[15];
    u8 f23;
    u8 f24[49];
    u8 f55;
} Obj;

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

extern u8 Data_0200c420[];
extern u8 Data_0200bf14[];
extern Ctl Data_02001000;
extern u8 *Data_03001f3c;
extern u8 Data_0200b1c1[];

void Func_02003db6(s32, s32, s32, s32, s32, s32);
Obj *Func_02003e7c(s32);
s32 Func_02003db8(s32, s32, s32);
void Func_02003df8(s32, s32, s32, s32, s32, s32);
Obj *Func_02003ebe(s32);
void Func_02003e84(s32, s32);
void Func_02003e24(s32, s32, s32, s32, s32, s32);
void Func_02003b34(void);
void Func_020003e8(void);
s32 Func_020043b8(void);
void Func_02004708(void);
void Func_02000b9e(void);
void Func_0200b3a0(void);
s32 Func_020073ee(void);
void Func_020073be(s32, s32);
s32 Func_020074bc(s32);
void Func_02007392(s32, s32);

/* Contiguous unnamed leaf-owner run for resource_3ba. */

/*
 * Scene setup for resource_3ba: allocates a scene descriptor, stamps its
 * parameter block, uploads image and palette, and installs the per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* In-image data at file offset 0x3f14 (0x0200bf14 - 0x8000). */

/* The per-frame task this owner installs: in-image code, published below as
 * its entry address plus the Thumb bit. */

u8 *Func_020072c6();            /* allocate a record by (id, size) */

s32 Func_020072e0();            /* reserve a graphics handle */

u8 *Func_02007478();            /* scene record for an actor selector */

u8 *Func_02007480();            /* scene record for an actor selector */

s32 Func_02007428();            /* test a story flag */

void Func_02007356();           /* upload image data to a handle */

s32 Func_0200737a();            /* next palette slot index */

s32 Func_02007386();            /* upload a palette ramp */

void Func_02007326();           /* install a per-frame task (callback, rate) */

void Func_02007374();           /* release a graphics handle */

void SceneState_ApplyRectsForActorsNineAndTen(void)
{
    Obj *o;
    s32 r;

    {
        s32 x = 23;
        s32 y = 12;

        Func_02003db6(27, 13, 3, 1, x, y);
    }
    o = Func_02003e7c(9);
    r = Func_02003db8(0, o->f08, o->f10);
    if (o->f0c == 0 && r == 0) {
        o->f23 = 2;
        o->f55 = 0;
        {
            s32 x = o->f08 >> 20;
            s32 y = o->f10 >> 20;

            Func_02003df8(14, 13, 1, 1, x, y);
        }
    }
    o = Func_02003ebe(10);
    {
        s32 x = o->f08 >> 20;

        Func_02003e84(784, x);
    }
    {
            s32 x = o->f08 >> 20;
            s32 y = o->f10 >> 20;

        Func_02003e24(14, 13, 1, 1, x, y);
    }
}

void FieldScene_RunTwoCallSequence(void)
{
    Func_02003b34();
    Func_020003e8();
}

void FieldScene_RunBranchedStep(void)
{
    if (Func_020043b8() == 0) {
        Func_02004708();
    } else {
        Func_02000b9e();
    }
}

u8 *SceneData_GetTablec420(void)
{
    return Data_0200c420;
}

/*
 * Seven arguments: four in registers, three from the caller's stack. The
 * 232-byte owner includes the six-word literal pool the body branches over.
 * The palette index stored at +216 is passed on sign-extended from sixteen
 * bits, so the narrowing is deliberate. Descriptor layout is asserted only
 * for the fields written here, and the actor records are touched at +8 and
 * +16 only on the flag-clear path.
 */
void FieldScene_BuildSceneDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *desc;
    u8 *rec0;
    u8 *rec1;
    s32 handle;
    s32 pal;

    desc = Func_020072c6(59, 0x7170);
    handle = Func_020072e0(512);

    *(u16 *)(desc + 222) = (u16)first;
    *(u16 *)(desc + 224) = (u16)second;
    *(u16 *)(desc + 226) = (u16)third;
    *(u16 *)(desc + 228) = (u16)fourth;
    *(u16 *)(desc + 230) = (u16)mode;
    *(s32 *)(desc + 232) = centre;
    *(s32 *)(desc + 236) = extra;

    rec0 = Func_02007478(first);
    rec1 = Func_02007480(second);

    if (Func_02007428(0x109) == 0) {
        *(s32 *)(rec1 + 8) =
            (centre << 1) - *(s32 *)(rec0 + 8);
        *(s32 *)(rec1 + 16) = *(s32 *)(rec0 + 16);
    }

    *(u16 *)(desc + 218) = 0;
    *(u16 *)(desc + 220) = 0;

    Func_02007356(Data_0200bf14, handle);

    pal = Func_0200737a();
    *(u16 *)(desc + 216) = (u16)pal;
    Func_02007386((s16)pal, 512, handle);

    Func_02007326((s32)Func_0200b3a0 + 1, 0xc76);

    Func_02007374(handle);
}

void SceneState_InitControlWhenFlag109Clear(void)
{
    u8 *state = Data_03001f3c;
    Ctl *m = &Data_02001000;

    Func_020073be(Func_020073ee(), (s32)(state + 240));
    if (Func_020074bc(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Func_02007392((s32)Data_0200b1c1, e);
    }
}
