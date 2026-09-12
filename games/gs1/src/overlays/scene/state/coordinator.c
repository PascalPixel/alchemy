#include "types.h"
#include "scene.h"

typedef struct Sub {
    u8 pad00[0x28];
    s16 *f28;
} Sub;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x3c];
    Sub *f50;
    u8 f54;
} Obj;

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

typedef struct Obj2 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj2;

typedef struct Obj3 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
} Obj3;

typedef struct Obj4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[14];
    u8 f22;
} Obj4;

typedef struct Obj5 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} Obj5;

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

typedef struct Rec {
    u8 pad00[216];
    u16 fd8[15];
} Rec;

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct Obj6 {
    u32 f00[9];
    s32 f24;
    s32 f28;
    s32 f2c;
    s32 f30;
    s32 f34;
    u32 f38;
    s32 f3c;
    u32 f40;
} Obj6;

typedef struct Obj7 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 f10[32];
    s32 f30;
    s32 f34;
    u8 f38[35];
    u8 f5b;
} Obj7;

typedef struct Obj8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 f10[32];
    s32 f30;
    s32 f34;
    u8 f38[35];
    u8 f5b;
} Obj8;

typedef struct Obj9 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[20];
    s32 f28;
    u8 f2c[41];
    u8 f55;
} Obj9;

typedef struct SceneRecord {
    u8 pad_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[14];
    u8 state;
    u8 pad_23;
    s32 motion_24;
    u8 pad_28[4];
    s32 motion_2c;
    s32 rate_x;
    s32 rate_z;
    u8 pad_38[33];
    u8 flags;
} SceneRecord;

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u16 gOv7;
extern u16 gOv8;
extern u16 gOv9;
extern u8 *gOv10;
extern u16 gOv11;
extern u16 gOv12;
extern u32 gOv13;
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern s16 gOv14;
extern u8 gOv15[];
extern u16 gOv16;
extern u16 gOv17;
extern u16 gOv18;
extern u16 gOv19;
extern u16 gOv20;
extern u16 gOv21;
extern u16 gOv22;
extern u16 gOv23;
extern u16 gOv24;
extern u16 gOv25;
extern u8 *gIw;
extern u8 Value_0000207c;
extern u8 gOv26[];
extern u8 gOv27[];
extern u8 gOv28[];
extern volatile u32 gIw2;
extern u8 gOv29[];
extern u8 gVal[];
extern u8 gOv30[];
extern u8 gOv31[];
extern u8 gOv32[];
extern u8 gOv33[];
extern u8 HexDigits[];
extern u8 gOv34[];
extern u32 gOv35[];

Obj *State_unk21_4(s32);

Obj *State_unk22_4(s32);

typedef void(*Task02000134)(void);

Obj *State_unk23_4(s32);

Obj *State_unk24_4(s32);

Obj *State_unk25_4(s32);
Obj *State_unk26_4(s32);

Obj *State_unk27_4(s32);

Obj *State_unk28_4(s32);

Obj *State_unk29_4(s32);
Obj *State_unk30_4(s32);

Obj *State_unk31_4(s32);

Obj *State_unk32_4(void);

Obj *State_unk33_4(s16);

PartyInteractionRecord *GetPartyInteractionRecord(void);

Rec *State_unk34_4(s32);

u8 *State_unk35_4();

u8 *State_unk36_4();

Obj *State_unk37_4(void);

Obj *State_unk38_4(s32);

Obj *State_unk39_4(s32);

Obj *State_unk40_4(s32, s32, s32, s32);

SceneRecord *State_unk41_3(Position3 *, SceneRecord *);
SceneRecord *State_unk42_3(Position3 *, SceneRecord *);
SceneRecord *State_unk43_3(Position3 *, SceneRecord *);
u8 *State_unk44_3();

s32 *State_unk45_3();

s32 *State_unk46_3();

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

/* AUDITED GENERATED CALL SCRIPT for Scene_RunSceneFourCoordinator:
 * A phase-two fast path, full and revisit branches, and all 42 calls across
 * the complete scene-four coordinator. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

/* A countdown word this overlay owns at gOv36: each call decrements
 * it by one, and specific values select which sub-sequence runs this call.
 * Reaching 0 restarts the countdown at 120 after running its own branch. */

static inline void InitializeActorZero(void)
{
    State_unk47_2(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    State_unk48_2(actorId, 0x10000, 0x8000);
}

static __inline__ void bump_step(s32 amount)
{
    extern u8 gCell[];
    extern u8 gCell2[][2];
    extern struct ModeRecord gOv37;
    extern struct ModeRecord gOv38;
    extern u8 gWork[];
    void State_unk47_3();
    void State_unk48_3();
    void State_unk49_3();
    void State_unk50_3();

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

u8 *State_unk51_3();            /* allocate a record by (id, size) */

s32 State_unk52_3();            /* reserve a graphics handle */

u8 *State_unk53_3();            /* scene record for an actor selector */

u8 *State_unk54_3();            /* scene record for an actor selector */

s32 State_unk55_3();            /* test a story flag */

void State_unk56_2();           /* upload image data to a handle */

s32 State_unk57_2();            /* next palette slot index */

s32 State_unk58_2();            /* upload a palette ramp */

void State_unk59();           /* install a per-frame task (callback, rate) */

void State_unk60();           /* release a graphics handle */

s32 GameFlag_IsSet();           /* veneer to GameFlag_IsSet */

u8 *Scene_GetRecord();           /* veneer to Scene_GetRecord */

void Object_SetPosition();          /* veneer to Object_SetPosition */

void GameFlag_Set();          /* veneer to GameFlag_Set */

void Object_SetCallback();          /* veneer to Object_SetCallback */

void State_unk49_3();          /* veneer to State_unk66 */

s32 State_unk67();           /* local thunk to Actor_ApplyValueAndMatchingSlots, site A */

s32 State_unk68();           /* local thunk to Actor_ApplyValueAndMatchingSlots, site B */

void State_unk69();          /* veneer to UiText_DrawQuantity, site A */

void State_unk70();          /* veneer to UiText_DrawQuantity, site B */

void State_unk71();          /* shared veneer, selector refresh + 0x96a */

void State_unk50_3();          /* veneer to ObjectDispatch_WaitForValue16 */

SceneRecord *State_unk73();   /* scene record for a subject handle */

s32 State_unk74(SceneRecord *, Position3 *);  /* terrain probe */

void State_unk75(SceneRecord *, s32);         /* select presentation mode */

void State_unk76(s32);                        /* wait n frames */

void State_unk77(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void State_unk78(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void State_unk79(s32);                        /* play a cue */

void State_unk80(SceneRecord *);              /* re-attach the camera */

void State_unk81(s32);                        /* play a cue */

void State_unk82(SceneRecord *, s32);         /* select presentation mode */

/* Contiguous unnamed leaf-owner run for resource_3ba. */
u8 *SceneData_GetTableC194(void)
{
    extern u8 gOv37[];
    extern u8 gOv39[];
    extern u8 gOv38[];

    return gOv;
}

s32 get_default_result(void)
{
    return 0;
}

u8 *SceneData_GetTablec1dc(void)
{
    extern u8 gOv37[];
    extern u8 gOv39[];
    extern u8 gOv38[];

    return gOv2;
}

u8 *SceneData_GetTablec1f4(void)
{
    extern u8 gOv37[];
    extern u8 gOv39[];
    extern u8 gOv38[];

    return gOv3;
}

/* Runs one branch of a scripted auxiliary sequence selected by the current
 * countdown value, then advances (or, from 0, restarts) the countdown. */
void Scene_RunOpeningAuxiliarySequence(void)
{
    extern s16 gCell2[][1];
    extern u8 gOv36[];

    extern u8 gWork[];

    switch ((u32)AUX_COUNTDOWN) {
    case 66:
        State_SetRect(92, 31, 2, 2, 50, 38); /* main:080091c8 */
        State_unk2_5(92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_1(16, 10); /* object 16, action 10 */
        break;
    case 60:
        State_unk3_5(92, 33, 2, 2, 50, 38); /* main:080091c8 */
        State_unk4_5(92, 33, 2, 2, 54, 38); /* main:080091c8 */
        State_unk5_5(50, 25, 6, 1, 50, 12); /* main:080091c0 */
        Object_SetModeById_2(16, 11); /* object 16, action 11 */
        break;
    case 6:
        State_SetRect(92, 31, 2, 2, 50, 38); /* main:080091c8 */
        State_unk2_5(92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_3(16, 10); /* object 16, action 10 */
        break;
    case 0:
        State_unk6_5(92, 29, 2, 2, 50, 38); /* main:080091c8 */
        State_unk7_5(92, 29, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_4(16, 12); /* object 16, action 12 */
        State_unk8_5(50, 24, 6, 1, 50, 12); /* main:080091c0 */
        AUX_COUNTDOWN = 120;
        break;
    }
    AUX_COUNTDOWN = AUX_COUNTDOWN - 1;
}

void State_ResetCounterAndStartTask(void)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    Task02000134 task;

    gOv36 = 0;
    task = (Task02000134) 0x0200804D;
    State_unk13_2(task);
    task();
}

void State_SetMode66AndPassOpeningSequence(void)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    s32 value = 66;
    s32 *mode = (s32 *)0x0200C41C;

    *mode = value;
    State_Apply(0x0200804D, 0xC80);
}

void State_WaitUntilWordC41cIs22(void)
{
    extern s16 gCell[];
    extern u16 gOv40;
    extern u32 gOv36;

    s32 i;

    State_unk14_2(10);
    i = 0;
    if (gOv36 != 22) {
        do {
            State_unk15_2(1);
            i++;
            if (i > 119) {
                break;
            }
        } while (gOv36 != 22);
    }
}

void State_ApplyRectsForActorsNineAndTen(void)
{
    extern Ctl gOv40;

    Obj *o;
    s32 r;

    {
        s32 x = 23;
        s32 y = 12;

        State_unk10_5(27, 13, 3, 1, x, y);
    }
    o = State_unk23_4(9);
    r = State_unk49_2(0, o->f08, o->f10);
    if (o->f0c == 0 && r == 0) {
        o->f23 = 2;
        o->f55 = 0;
        {
            s32 x = o->f08 >> 20;
            s32 y = o->f10 >> 20;

            State_unk11_5(14, 13, 1, 1, x, y);
        }
    }
    o = State_unk24_4(10);
    {
        s32 x = o->f08 >> 20;

        State_Apply2(784, x);
    }
    {
            s32 x = o->f08 >> 20;
            s32 y = o->f10 >> 20;

        State_unk12_5(14, 13, 1, 1, x, y);
    }
}

void Scene_RunTwoCallSequence(void)
{
    extern Ctl gOv40;

    State_unk84();
    State_unk85();
}

void State_ApplyRectAndSend303(void)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    {
        s32 x = 47;
        s32 y = 12;

        State_unk13_5(47, 24, 1, 1, x, y);
    }
    State_unk16_2(0x303);
}

void Scene_RunScene3ba(void)
{
    extern s16 gCell2[][1];
    extern u8 gOv36[];

    extern u8 gWork[];

    u32 i;
    u8 *rec7;
    s32 record;
    s32 none;

    State_Do(0x301);
    rec7 = State_Check(13);
    State_unk86();
    State_Run(0x20000, 0x4000);
    State_unk2_4(0x2580000, -1, 0xc80000, 1);
    State_unk87((s32)rec7, 3);
    State_unk88();
    none = 0;
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    State_unk3_4((s32)rec7, *(s32 *)((s32)rec7 + 8), 0x80000, *(s32 *)((s32)rec7 + 16));
    rec7 = State_unk2(14);
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    State_unk4_4((s32)rec7, *(s32 *)((s32)rec7 + 8), 0x200000, *(s32 *)((s32)rec7 + 16));
    State_unk89((s32)rec7);
    State_unk90(45);
    State_unk9_5(43, 12, 1, 1, 41, 12);
    State_unk91();
}

void StagedActor_PlacePairAtOffsetAndRun(s32 a0, s32 a1, s32 a2)
{
    extern s32 gCell[];

    Obj2 *p;
    Obj2 *q;
    s32 x;
    s32 y;

    p = State_unk25_4(gCell[125]);
    q = State_unk26_4(a0);
    State_unk92();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        State_SetMode(p, x, p->f0c, y);
    }
    State_Apply3(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        State_SetMode2(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        State_Apply4(q, 4);
    } else {
        State_Apply5(q, 3);
    }
    State_unk17_2(226);
    State_unk18_2(p);
    State_Apply6(q, 2);
    State_unk19_2(288);
    State_unk93();
}

void Actor_ShiftActorSeventeenByLeaderRow(void)
{
    extern s32 gCell[];

    Obj3 *o;
    s32 v;
    s32 t;

    o = State_unk27_4(gCell[125]);
    v = o->f10 >> 20;
    t = -48;
    if (v <= 8) {
        t = 48;
    }
    State_unk14_5(67, 8, 3, 1, 64, v);
    State_unk50_2(17, 0, t);
    o = State_unk28_4(17);
    v = o->f10 >> 20;
    State_unk15_5(64, 24, 3, 1, 64, v);
}

void Actor_ShiftActorEighteenByInputAndLeaderColumn(void)
{
    extern s32 gCell[];

    Obj3 *o;
    s32 a;
    s32 b;
    s32 s;

    o = State_unk29_4(gCell[125]);
    a = o->f08 >> 20;
    if ((gIw2 & 32) != 0) {
        s = -1;
    }
    if ((gIw2 & 16) != 0) {
        s = 1;
    }
    o = State_unk30_4(17);
    b = o->f10 >> 20;
    if (a == 63) {
        if (b == 11) {
            return;
        }
        b = 160;
    } else if (a == 67) {
        if (b == 11 && s == -1) {
            return;
        }
        b = 96;
    } else {
        if (b == 11) {
            b = 96;
        } else {
            b = 160;
        }
        b = -b;
    }
    State_unk16_5(72, 9, 1, 3, a, 9);
    State_unk51_2(18, b, 0);
    o = State_unk31_4(18);
    a = o->f08 >> 20;
    State_unk17_5(63, 25, 1, 3, a, 9);
}

void State_ApplyValue768(void)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    State_unk20_2(768);
}

void Scene_RunBranchedStep(void)
{
    extern Ctl gOv40;

    if (State_unk17() == 0) {
        State_unk94();
    } else {
        State_unk95();
    }
}

u8 *SceneData_GetTablec420(void)
{
    extern Ctl gOv40;

    return gOv26;
}

void Scene_RunScene3ba(s32 a0)
{
    extern s16 gCell2[][1];
    extern u8 gOv36[];

    extern u8 gWork[];

    u32 i;
    s32 record;

    State_unk96(24);
    State_unk97(25);
    State_unk98(1);
    State_unk99();
    State_Place(8, 0x5280000, 0xc00000);
    State_unk2_3(0, 0x5080000, 0xc00000);
    State_unk3_3(8, 0x4000, 0);
    State_unk4_3(0, 0x4000, 0);
    if (a0 < 0) {
        State_unk100(8, 10);
        State_unk101(0, 35);
    } else {
        State_unk102(8, 8);
        State_unk103(0, 28);
    }
    State_unk104(1);
    State_unk5_4(0x5180000, 0, 0x800000, 0);
    State_unk105(a0);
    State_unk106();
}

void Actor_MarkObjectAtTiles94To95(void)
{
    extern s32 gCell[];

    Obj4 *o;
    s32 x;
    s32 y;

    o = State_unk32_4();
    if (o != 0) {
        x = o->f08 >> 19;
        y = o->f10 >> 19;
        if (x >= 94 && x <= 95 && y > 23 && y <= 26) {
            o->f22 = 1;
        }
    }
}

void Scene_RunCommandSequence(s32 a0)
{
    extern s16 gCell2[][1];
    extern u8 gOv36[];

    extern u8 gWork[];

    u32 i;
    s32 p10;
    s32 p10b;
    s32 p9;
    s32 p9b;
    s32 record;

    record = State_unk18();
    p9 = *(s16 *)(record + 10);
    p10 = *(s16 *)(record + 18);
    State_unk107();
    State_unk5_3(a0, 0x10000, 0x8000);
    State_unk6_3(0, 0x10000, 0x8000);
    State_unk7_3(1, 0x10000, 0x8000);
    State_unk8_3(2, 0x10000, 0x8000);
    State_unk9_3(3, 0x10000, 0x8000);
    State_unk10_3(0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x300000));
    State_unk108(1, ((s32)((s32)p9 << 16) + -0x100000), (-0x280000 + (s32)((s32)p10 << 16)));
    State_unk109(2, ((s32)((s32)p9 << 16) + 0x100000), (-0x280000 + (s32)((s32)p10 << 16)));
    State_unk110(3, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x200000));
    State_unk111(a0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x500000));
    record = State_unk19(0);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    State_unk112(0, 0);
    State_unk113();
    State_unk114();
    State_unk2_2(0x20cb);
    State_unk115(a0, 0);
    State_unk11_3(3, 0x101, 60);
    State_unk116(3, 0);
    State_unk117(a0, 3);
    State_unk118(a0, 0);
    State_unk12_3(2, 0x101, 60);
    State_unk119(2, 0);
    State_unk120(a0, 2, 0);
    State_unk121(20);
    State_unk122(a0, 3);
    State_unk123(a0, 0);
    State_unk13_3(1, 0x101, 60);
    State_unk124(1, 0);
    State_unk14_3(3, 0x101, 60);
    State_unk125(3, 0);
    State_unk15_3(a0, 0x102, 60);
    if (State_unk3(a0, 0) != 0) {
    } else {
        L_02000f28:;
        State_unk3_2(0x20d5);
        State_unk126(2, 3);
        State_unk127(2);
        State_unk128(1, 3);
        State_unk129(2);
        State_unk130(3, 3);
        State_unk131(1);
        State_unk132(0, 3);
        State_unk133(a0, 3);
        State_unk134(a0, 0);
        State_unk16_3(a0, 0xa000, 0);
        State_unk135(20);
        State_unk136(a0, 0);
        State_unk6_4(0x30000, 0x6000);
        State_unk7_4(0x1380000, -1, 0x680000, 1);
        State_unk137();
        State_unk138(a0, 0);
        State_unk8_4(0x18000, 0x3000);
        State_unk9_4(0x3080000, -1, 0x680000, 1);
        State_unk139(a0, 0);
        State_unk140();
        State_unk141(a0, 0);
        State_unk10_4(0x30000, 0x6000);
        State_unk11_4(0x4d80000, -1, 0xa80000, 1);
        State_unk142();
        State_unk17_3(a0, 0x6000, 0);
        State_unk143(a0, 0);
        State_unk12_4(0x5180000, -1, 0xa80000, 1);
        State_unk144();
        State_unk145(a0, 0, 0);
        State_unk146(a0, 0);
        State_unk147(a0, 0);
        State_unk148(a0, 0);
        State_unk149(0, 0);
        State_unk150(a0, 2);
        if (State_unk4(a0, 0) != 0) {
            goto L_02000f28;
        }
        State_unk151(a0, 2);
        State_unk4_2(0x20d4);
        State_unk152(a0, 0);
    }
    State_unk5_2(0x20e1);
    State_unk153(a0, 2);
    State_unk154(a0, 0);
    State_unk18_3(0, 0x4000, 0);
    State_unk155(1, 0, 0);
    State_unk19_3(2, 0x8000, 0);
    State_unk156(1, 2);
    State_unk157(1, 0);
    State_unk158(2, 2);
    State_unk159(2, 0);
    State_unk160(3, 3);
    State_unk161(3, 0);
    State_unk162(3, 3);
    State_unk163(1);
    State_unk164(1, 3);
    State_unk165(2);
    State_unk166(2, 3);
    State_unk167(1);
    State_unk168(0, 3);
    State_unk169(6);
    State_unk170(1, 2);
    record = State_unk5(0);
    if (record != 0) {
        State_unk171(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    State_unk172(2, 2);
    record = State_unk6(0);
    if (record != 0) {
        State_unk173(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    State_unk174(3, 2);
    record = State_unk7(0);
    if (record != 0) {
        State_unk175(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    State_unk176(a0, (p9 - 16), (p10 - 64));
    State_unk177(1, 0, 0);
    State_unk178(2, 0, 0);
    State_unk179(3, 0, 0);
    State_unk180(a0, (p9 - 16), (p10 - 16));
    State_unk181(a0, p9, p10);
    State_unk20_3(a0, 0xc000, 10);
    State_unk182();
    p9b = (p9 - 16);
    p10b = ((s32)p9 << 16);
}

void Scene_RunScene3ba(s32 a0)
{
    extern s16 gCell2[][1];
    extern u8 gOv36[];

    extern u8 gCell[];
    extern u8 gWork[];

    u32 i;
    s32 rec8;
    s32 record;

    if (gCell2[225][0] == 2) {
        State_unk183();
    } else {
        State_unk184();
        rec8 = State_unk8(a0, 3);
        if (rec8 == 0) {
            State_unk6_2(0x2095);
            State_unk185();
            State_unk13_4(0x30000, 0x6000);
            State_unk14_4(0x3480000, -1, 0xd80000, 1);
            ((void (*)())State_unk20)();
            State_unk186(a0, 0);
            State_unk187();
            State_unk188(60);
            State_unk189(a0, 0);
            State_unk21_3(0, 0x2e0, 200);
            State_unk22_3(0, 0, 0);
            State_unk190();
            State_unk23_3(0, 0x10000, 0x8000);
            State_unk24_3(0, 0x330, 200);
            State_unk191(30);
            State_unk25_3(0, 0x105, 60);
            State_unk192(a0, 0);
            State_unk193(0);
            State_unk194(0, 0);
            State_unk195(a0, 3);
        } else {
            if (rec8 == 1) {
                State_unk7_2(0x2094);
                State_unk196(a0, 0);
            }
        }
        State_unk26_3(rec8, a0, 3);
        State_unk197();
    }
}

void Scene_RunSceneFourCoordinator(s32 scene)
{
    extern s16 gCell[];

    s32 path;

    if (SceneTransition_Phase == 2) {
        State_unk198();
        return;
    }
    State_unk199();
    path = State_unk21(scene, 4);
    if (path == 0) {
        State_unk8_2(8345);
        State_unk15_4(196608, 24576);
        State_unk16_4(71303168, -1, 11010048, 1);
        State_unk200();
        State_unk201(scene, 0);
        State_unk27_3(120, 72, 0);
        State_unk202(scene, 0);
        State_unk203();
        State_unk204(15);
        State_unk28_3(0, 984, 200);
        State_unk205(0, 0, 10);
        State_unk206(scene, 0);
        State_unk29_3(0, 16384, 30);
        State_unk30_3(0, 262, 60);
        State_unk31_3(0, 98304, 49152);
        State_unk22(0, 1000, 192);
        State_unk23(0, 1000, 176);
        State_unk32_3(0, 1016, 168);
        State_unk207(15);
        State_unk33_3(18, 160, 0);
        State_unk17_4(71303168, -1, 11010048, 1);
        State_unk208(0, 1);
        State_unk209(10);
        State_unk34_3(0, 65536, 32768);
        State_unk35_3(0, 1192, 168);
        State_unk210(10);
        State_unk36_3(0, 32768, 30);
        State_unk37_3(0, 258, 60);
        State_unk211(scene, 0);
        State_unk212(0);
        State_unk213(0, 0);
        State_unk38_3(18, 66584576, 11010048);
        State_unk214(scene, 4);
    } else if (path == 1) {
        State_unk9_2(8344);
        State_unk215(scene, 0);
    }
    State_unk39_3(path, scene, 4);
    State_unk216();
}

void Actor_PlaceSlots1To3FromWork(void)
{
    extern s32 gCell[];

    {
        s32 x = State_unk24(896);
        s32 y = State_unk25(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_unk52_2(1, x, y);
    }
    {
        s32 x = State_unk26(912);
        s32 y = State_unk27(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_unk53_2(2, x, y);
    }
    {
        s32 x = State_unk28(928);
        s32 y = State_unk29(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        State_unk54_2(3, x, y);
    }
}

void State_SetStateHalfword386To99WhenMatched(void)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    extern s32 gCell[];

    u8 *state = gWork;
    s32 sel = gCell[125];

    if (sel != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == sel
        && State_unk30(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 val = 99;

        *p = val;
    }
}

void Scene_RunNearestActor165Scene(void)
{
    extern s16 gCell2[][1];
    extern u8 gOv36[];

    extern s32 gCell[];
    extern u8 *gWork;

    u8 *state = gWork;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gCell[125];
    Obj *p = State_unk21_4(n);
    s32 i;
    s32 *q;
    s32 base;

    State_unk217();
    for (i = 8; i <= 66; i++) {
        Obj *o = State_unk22_4(i);

        if (o != 0 && o->f54 == 1 && *o->f50->f28 == 165) {
            s32 dx = (p->f08 - o->f08) / 65536;
            s32 dy = (p->f10 - o->f10) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    State_unk21_2(0x2085);
    State_Apply7(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    State_unk218(20);
    State_unk219();
    State_unk220();
    base = n << 4;
    State_Apply8(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        State_unk221(base + 888, v);
    }
    n++;
    if (n > 3) {
        State_unk22_2(10);
        State_unk23_2(282);
    } else {
        State_unk24_2(n);
        State_unk222();
        State_unk223();
        *q = 0;
    }
    State_unk224();
}

void RunPartyCountInteraction(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = GetPartyInteractionRecord();
    x = record->x;
    y = record->y;
    FinishPartyInteractionRecordRead();

    if (GetPartyMemberCount() <= 1) {
        SetInteractionCue(0x20e5);
        if (CheckActorInteraction(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            PlaceSelectedActor(actorId, x, y + 0x40);
            SetInteractionStep(15);
            PlaceActorZero(0, x, y);
            PlaceSupportActor(0, x, y + 0x20);
            StartInteractionPhase();
            AdvanceInteractionPhase();
            SelectInteractionStep(11);
        }
    } else {
        SetLargePartyInteractionCue(0x20e8);
        RunLargePartyInteraction(actorId, 0);
    }

    FinishPartyCountInteraction();
}

void Scene_RunSixSteps380To3A8(void)
{
    State_Apply9(896, 0);
    State_Apply10(904, 0);
    State_Apply11(912, 0);
    State_Apply12(920, 0);
    State_Apply13(928, 0);
    State_Apply14(936, 0);
}

s32 Dialogue_RunFlagGatedPromptInteraction(s32 a, s32 b)
{
    extern s16 gCell[];
    extern u16 gOv40;
    extern u32 gOv36;

    s32 v;
    s32 id;
    s32 r;

    State_unk225();
    State_Apply15(b, 5);
    v = gCell[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    State_unk25_2(id);
    State_Apply16(a, 0);
    if (State_unk31(b + 512) != 0) {
        return 2;
    }
    if (State_unk32(b + 520) != 0) {
        r = State_unk33(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    State_unk26_2(b + 520);
    State_unk27_2((s32)&Value_0000207c);
    State_Apply17(a, 0);
    return State_Apply18(0, 0);
}

void State_SendIdBySceneId(s32 a, s32 b)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    extern s16 gCell[];

    s32 v;
    s32 id;

    State_Apply19(b, 5);
    v = gCell[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    State_unk28_2(id + 1);
    State_Apply20(a, 0);
}

void Scene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    extern u8 gCell[];

    s32 rec;
    s32 record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = State_unk9(owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = State_unk18_4();
        for (i = 0; i < count; i++) {
            buf[i] = gCell[504 + i];
        }
        if (count <= 1) {
            State_unk226(0x2083);
            State_unk227(owner, 0);
            return;
        }
        if (State_unk10(base + 512) != 0) {
            State_unk226(0x2084);
            State_unk227(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            State_unk228(6);
        } else {
            State_unk10_2(0x207d);
            State_unk11(owner, 0);
            state = State_unk12(0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    State_unk229((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    State_unk230((s32)(s8)buf[i]);
                }
            }
            obj = State_unk19_4();
            for (i = 0; i < count; i++) {
                State_unk231((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                State_unk232((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    State_unk226(0x207e);
    State_unk227(owner, 0);
    return;
L_main:
    ((void (*)())State_unk233)(obj, 1);
    State_unk11_2(0x207f);
    State_unk234(owner, 0);
    State_unk40_3(0, 0x10000, 0x8000);
    State_unk41_2(obj, 0x10000, 0x8000);
    State_unk42_2(owner, 0x10000, 0x8000);
    record = State_unk13(0);
    if (record != 0) {
        State_unk235(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    State_unk236(obj, p9, hi);
    lo = p9 + 16;
    State_unk43_2(0, lo, hi);
    ((void (*)())State_unk34)(obj, 0, 30);
    State_unk237(obj, 3);
    tail = hi - 32;
    State_unk238(0, 3);
    State_unk239(owner, p9, tail);
    State_unk44_2(owner, lo, tail);
    State_unk240(0, obj);
    State_unk241(obj, p9, tail);
    State_unk242(owner, 1);
    State_unk45_2(owner, 0x8000, 0);
    State_unk243(obj, p9, p11 - 48);
    State_unk244(owner, p9, tail);
    State_unk245(owner, p9, p11);
    State_unk246(obj);
    ((void (*)())State_unk35)(base + 512);
    rec = State_unk14(obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    State_unk247((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    State_unk248((obj << 4) + 888, sy);
}

void Actor_ApplyValueAndMatchingSlots(s32 a, s32 b)
{
    Rec *t = State_unk34_4(a);
    s32 i;

    State_Apply21(a, b);
    for (i = 0; i <= 14; i++) {
        if (t->fd8[i] == b) {
            State_Apply22(a, i);
        }
    }
}

void SceneData_SelectBlockAndResetCounters(s32 a, s32 b)
{
    extern u8 gOv37[];
    extern u8 gOv39[];
    extern u8 gOv38[];

    u8 *p;

    gOv7 = a;
    gOv8 = b << 4;
    State_Apply23(gOv4, 3200);
    p = gOv39;
    if (a == 2) {
        p = gOv5;
    }
    if (a == 4) {
        p = gOv6;
    }
    if (a == 3) {
        if (b != 0) {
            p = gOv37;
        } else {
            p = gOv38;
        }
    }
    gOv9 = 0;
    gOv10 = p;
    gOv11 = 0;
    gOv12 = 0;
    gOv13 = 0;
}

void Scene_RunTwoArmSequence(s32 a)
{
    extern u16 gOv39[];

    if (a == 0) {
        State_unk249();
        State_unk250();
        State_unk251();
        State_unk29_2(30);
        State_unk30_2(89);
        State_unk31_2(0);
        State_Apply24(1, 0);
        State_unk32_2(120);
        State_unk252();
    } else {
        State_unk33_2(247);
        State_unk253();
        State_unk254();
        State_unk255();
        gOv39[15] = a * 60;
        State_unk34_2(30);
        State_unk35_2(a + 90);
        State_unk36_2(a);
        State_Apply25(1, 0);
        State_unk37_2(120);
        while (State_unk36() != 0) {
            State_unk38_2(1);
        }
        State_unk39_2(0x121);
        State_unk40_2(5);
        State_Apply26(2, 0);
        State_unk41(236);
        State_unk42(60);
        State_Apply27(2, 1);
        State_unk43(236);
        State_unk44(60);
        State_unk45(6);
        State_Apply28(2, 0);
        State_unk46(236);
        State_unk47(60);
        State_unk48(7);
        State_Apply29(4, 0);
        State_unk49(237);
        State_unk256();
        State_unk257();
        State_unk50(0x123);
    }
}

void Scene_RunLateSequence(s32 a0)
{
    extern u8 gCell[];
    extern u8 gCell2[][2];
    extern struct ModeRecord gOv37;
    extern struct ModeRecord gOv38;
    extern u8 gWork[];
    void State_unk47_3();
    void State_unk48_3();
    void State_unk49_3();
    void State_unk50_3();

    s32 kind;

    State_unk258(247);
    State_unk259();
    State_unk260();
    gOv38.span = a0 * 60;
    gOv37.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        State_unk261(30);
        State_unk262(86);
        State_unk263(8);
        State_unk15(3, 1);
        State_unk264(-a0 * 60 + 60);
        kind = 0;
    } else {
        State_unk265(30);
        State_unk266(a0 + 90);
        State_unk267(4);
        State_unk16(3, 0);
        State_unk268(a0 * 60 + 60);
        kind = 8;
    }
    State_unk46_2(kind, 0x105, 0);
    while (State_unk20_4()!= 0) {
        State_unk269(1);
    }
    State_unk270(19);
    State_unk271(30);
    State_unk12_2(0x121);
    State_unk272();
    State_unk273();
}

void OvObj_ResetMotionFields(void)
{
    Obj6 *o = State_unk37_4();

    State_unk51(o);
    o->f24 = 0;
    o->f2c = 0;
    o->f38 = 0x80000000;
    o->f40 = 0x80000000;
}

void State_InitHalfwordC6a6Once(void)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    if (gOv14 == -1) {
        gOv14 = State_unk274();
    }
}

void State_StoreParamsAndInitTable(s32 a, s32 b, s32 c)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    State_unk275();
    gOv16 = a;
    gOv17 = b;
    gOv18 = c & 3;
    gOv19 = 0;
    gOv20 = 0;
    {
        s32 n = 3200;

        State_Apply30(gOv15, n);
    }
}

void State_InitTableWordsAndLoad3200(s32 a, s32 b, s32 c)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    gOv21 = a;
    gOv22 = b;
    gOv23 = gOv16;
    gOv24 = gOv17;
    gOv20 = c;
    gOv25 = 0;
    {
        s32 n = 3200;

        State_Apply31(gOv15, n);
    }
}

void State_ReleaseTableAndResetC6a6(void)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    State_unk52(gOv15);
    State_unk53(gOv14);
    gOv14 = -1;
}

void Actor_StartMode5MoveToTile(s32 a, s32 b, s32 c)
{
    Obj8 *o = State_unk39_4(a);

    if (o != 0) {
        s32 v = 0x20000;
        s32 z = 0;

        o->f30 = v;
        o->f34 = v >> 1;
        o->f5b = z;
        State_unk54(o);
        State_Apply32(o, 5);
        State_SetMode3(o, b << 16, o->f0c, c << 16);
    }
}

void OvObj_PlaceWithScale14000(s32 a, s32 b, s32 c)
{
    Obj7 *o = State_unk38_4(a);

    if (o != 0) {
        s32 v = 0x14000;
        s32 z = 0;

        o->f30 = v;
        o->f34 = v >> 1;
        o->f5b = z;
        State_unk55(o);
        State_Apply33(o, 5);
        State_SetMode4(o, b << 16, o->f0c, c << 16);
        State_unk56(o);
        State_Apply34(o, 1);
    }
}

void Text_WriteU32AsHex(u8 *buf, u32 value)
{
    s32 i;

    buf += 8;
    *buf = 0;
    buf--;
    for (i = 7; i >= 0; i--) {
        *buf = HexDigits[value & 15];
        value >>= 4;
        buf--;
    }
}

void Resource3ba_NoOpCallback(void)
{
}

void State_SetHalfword1000To9(void)
{
    extern s16 gCell[];
    extern u16 gOv40;
    extern u32 gOv36;

    u16 *p = &gOv40;
    s32 v = 9;

    *p = v;
}

void State_WaitUntilWord1000IsNine(void)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    s16 *p = &gOv40;

    while (*p != 9) {
        State_unk57(1);
    }
}

void Effect_SpawnKind285AtRandomChance(Obj9 *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = State_unk276();
    if (n * 100 >> 16 <= 9) {
        Obj9 *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = State_unk37();
        w = State_unk38();
        State_unk55_2(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = State_unk40_4(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            State_Apply35(o, 0);
            State_Apply36(o, (s32)gOv34);
            State_Apply37(o, 1);
            State_Apply38(o, 0);
        }
    }
}

s32 Actor_PlaceLinkedActorAbove(Obj5 *a)
{
    extern s32 gCell[];

    Obj5 *o = State_unk33_4(a->f64);

    State_SetMode5(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    State_Apply39(o, (s32)gOv29);
    State_unk58(83);
    a->f64 = 0;
    return 0;
}

s32 Scene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b)
{
    extern u8 gCell[];

    u8 *work = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *rec;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = GameFlag_IsSet(0x211);

    shared = gCell;
    rec = Scene_GetRecord(*(s32 *)(shared + 500));

    if (*(s32 *)(work + 232) < *(s32 *)(rec + 8)) {
        x = *(s32 *)(work + 232) + 0xc0000;
    } else {
        x = *(s32 *)(work + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(work + 236) + 0x100000;
        cuep = (u16 *)(work + 228);
    } else {
        z = *(s32 *)(work + 236) - 0x100000;
        cuep = (u16 *)(work + 226);
    }

    waitp = (s16 *)(rec + 100);
    *waitp = *cuep;
    *(s32 *)(rec + 52) = 0x4000;
    *(s32 *)(rec + 48) = 0x10000;

    Object_SetPosition(rec, x, 0, z);
    GameFlag_Set(0x211);
    Object_SetCallback(rec, (void *)0x0200c6fc);

    while (*waitp != 0) {
        State_unk49_3(1);
    }

    if (flag == 0) {
        State_unk67(0, handle_a);
        State_unk69(handle_a, 2);
    } else {
        State_unk68(0, handle_b);
        State_unk70(handle_b, 2);
    }

    shared = gCell;
    State_unk71(*(s32 *)(shared + 500), 1);
    State_unk277(0x96a, 3);
    State_unk50_3(rec);

    return flag;
}

/*
 * Seven arguments: four in registers, three from the caller's stack. The
 * 232-byte owner includes the six-word literal pool the body branches over.
 * The palette index stored at +216 is passed on sign-extended from sixteen
 * bits, so the narrowing is deliberate. Descriptor layout is asserted only
 * for the fields written here, and the actor records are touched at +8 and
 * +16 only on the flag-clear path.
 */
void Scene_BuildSceneDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    extern Ctl gOv40;

    u8 *desc;
    u8 *rec0;
    u8 *rec1;
    s32 handle;
    s32 pal;

    desc = State_unk51_3(59, 0x7170);
    handle = State_unk52_3(512);

    *(u16 *)(desc + 222) = (u16)first;
    *(u16 *)(desc + 224) = (u16)second;
    *(u16 *)(desc + 226) = (u16)third;
    *(u16 *)(desc + 228) = (u16)fourth;
    *(u16 *)(desc + 230) = (u16)mode;
    *(s32 *)(desc + 232) = centre;
    *(s32 *)(desc + 236) = extra;

    rec0 = State_unk53_3(first);
    rec1 = State_unk54_3(second);

    if (State_unk55_3(0x109) == 0) {
        *(s32 *)(rec1 + 8) =
            (centre << 1) - *(s32 *)(rec0 + 8);
        *(s32 *)(rec1 + 16) = *(s32 *)(rec0 + 16);
    }

    *(u16 *)(desc + 218) = 0;
    *(u16 *)(desc + 220) = 0;

    State_unk56_2(gOv27, handle);

    pal = State_unk57_2();
    *(u16 *)(desc + 216) = (u16)pal;
    State_unk58_2((s16)pal, 512, handle);

    State_unk59((s32)State_unk278 + 1, 0xc76);

    State_unk60(handle);
}

void State_InitControlWhenFlag109Clear(void)
{
    extern Ctl gOv40;

    u8 *state = gIw;
    Ctl *m = &gOv40;

    State_Apply40(State_unk39(), (s32)(state + 240));
    if (State_unk40(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        State_Apply41((s32)gOv28, e);
    }
}

void State_SetStateHalfword220(s32 a)
{
    extern s16 gOv40;
    extern s32 gOv36;
    extern u8 *gWork;

    u8 *p = gIw;

    *(s16 *)(p + 220) = a;
}

s32 *Actor_FindSlotAtTilePosition(s32 *arg0)
{
    extern u8 *gWork;

    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] >> 20) == (p[3] >> 20)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/*
 * The push interaction: probe the cell one step ahead of the subject and, if
 * something occupies it, slide it and the subject one step on. The 360-byte
 * owner includes its four-word literal pool, which ends where the next owner
 * begins. Record fields are asserted only where written -- facing at +6,
 * position at +8/+12/+16, state at +0x22, occupancy flag bit 0 at +0x59 -- and
 * the terrain probe is tested signed, so only a positive code refuses.
 */
void StagedActor_PushActorAhead(void)
{
    extern s16 gCell[];

    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)gCell + (idx << 1));
    subject = State_unk73(handle);

    dir = subject->facing >> 12;

    step = gOv35[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = State_unk41_3(&pos, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = gOv35[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = State_unk42_3(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    pos.x = target->x;
    pos.y = target->y + 0x100000;      /* 128 << 13 */
    pos.z = target->z;

    blocker = State_unk43_3(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = gOv35[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    if (State_unk74(target, &pos) > 0) {
        return;
    }

    State_unk75(subject, 8);
    State_unk76(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    State_unk77(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    State_unk78(subject, pos.x, pos.y, pos.z);

    State_unk79(0xee);
    State_unk80(target);
    State_unk81(0x120);                                /* 144 << 1 */

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    State_unk82(subject, 1);
}

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay.
 * The occupancy lookups take the position block; the record pointer the call
 * sites also pass is not asserted as an argument. */

/*
 * Probe the two cells ahead of the active subject and return what occupies the
 * nearer one, else the further one, else zero. The 160-byte owner includes its
 * alignment bytes and two-word literal pool. Facing is the biased quadrant of
 * the halfword at +6, with no sign extension; each probe rounds x and z down to
 * whole units and re-centres them by half a unit, carrying y unrounded. Only
 * the record fields at +6, +8, +12 and +16 are asserted.
 */
s32 *Actor_FindOccupantAheadOfSubject(void)
{
    extern s16 gCell[];

    u8 *rec;
    s32 facing;
    s32 pos[3];
    s32 *hit;

    rec = State_unk44_3(((ActiveSubjectSlot *)gCell)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(rec + 6) + 0x2000) & 0xc000;

    pos[0] = (*(s32 *)(rec + 8) & 0xfff00000) + 0x80000;
    pos[1] = *(s32 *)(rec + 12);
    pos[2] = (*(s32 *)(rec + 16) & 0xfff00000) + 0x80000;
    State_unk279(0x100000, facing, pos);          /* 128 << 13 */

    hit = State_unk45_3(pos, rec);
    if (hit == 0) {
        pos[0] = (*(s32 *)(rec + 8) & 0xfff00000) + 0x80000;
        pos[1] = *(s32 *)(rec + 12);
        pos[2] = (*(s32 *)(rec + 16) & 0xfff00000) + 0x80000;
        State_unk280(0x200000, facing, pos);      /* 128 << 14 */

        hit = State_unk46_3(pos, rec);
    }

    return hit;
}

