#include "types.h"
#include "scene.h"

#include "staged_actor.h"

/*
 * resource_3b1 owner at 0x020002f4, 76 bytes.
 * Puts an actor into its ready state: stage byte at +89, two mode fields on the
 * linked record, and the flag byte at +35 rewritten. Returns 1.
 *
 * The two mode writes are bitfields (the -13 mask stays 32-bit and is shared
 * between them); the +35 write is ordinary byte arithmetic, and its 0xfe mask
 * is narrowed to a byte, which is why the two are spelled differently.
 */
struct Rec_3b1 {
    u8 pad00[9];
    u8 lo9 : 2;
    u8 mode9 : 2;               /* +9,  bits 2..3 */
    u8 hi9 : 4;
    u8 pad0a[11];
    u8 lo15 : 2;
    u8 mode15 : 2;              /* +21, bits 2..3 */
    u8 hi15 : 4;
};

struct Work_3b1 {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_3b1 *f80;        /* +80 */
    u8 pad51[5];
    u8 f89;                     /* +89 */
};

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};

struct SceneActor {
    u8 pad00[10];
    s16 x;
    u8 pad0c[6];
    s16 y;
};

struct SceneWork {
    u8 pad00[52];
    struct SceneActor *actors[58];
};

extern s16 gCell[];
extern u8 Value_0000006f;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3;
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 Value_00001f47;
extern u8 Value_00001ea0;
extern u8 Value_0000092b;
extern u8 Value_00001e7e;
extern u8 Value_00000993;
extern u8 Value_0000092a;
extern u8 Value_0000091a;
extern u8 Value_00000929;
extern u8 Value_00000938;
extern u8 Value_0000092f;
extern u8 Value_00001ea4;
extern u8 Value_00000996;
extern u8 Value_00001e8b;
extern u8 Value_0000091d;
extern u8 Value_0000093b;
extern u8 Value_00000932;
extern u8 Value_00001ea5;
extern u8 Value_00000997;
extern u8 Value_00001e8e;
extern u8 Value_0000091e;
extern u8 Value_0000093c;
extern u8 Value_00000933;
extern u8 gVal[];
extern u8 gOv15[];
extern u8 gVal2[];
extern u8 gOv16[];
extern u8 Value_00001dd1;
extern u8 Value_00000925;
extern u8 Value_00001e19;
extern u8 Value_00001d50;
extern u8 Value_00001e9e;
extern u8 Value_00001e78;
extern u8 Value_00000917;
extern u8 Value_00000935;
extern u8 Value_0000092c;
extern u8 Value_00000928;
extern u8 Value_00001e9f;
extern u8 Value_00000992;
extern u8 Value_00001e7b;
extern u8 Value_00000919;
extern u8 Value_00000937;
extern u8 Value_0000092e;
extern u8 Value_00001ea3;
extern u8 Value_00000995;
extern u8 Value_00001e88;
extern u8 Value_0000091c;
extern u8 Value_0000093a;
extern u8 Value_00000931;
extern u32 gOv17[];
extern u8 gOv18[];
extern u8 gOv19[];
extern u8 Value_0000093e;
extern u8 Value_00000911;
extern u8 Value_00000922;
extern u8 gOv20[];
extern u8 gOv21[];
extern const s32 gOv22[];
extern const s32 gOv23[];
extern const s32 gOv24[];
extern const s32 gOv25[];

struct SceneActor *Field_unk38_4(s32 actor_id);

u8 *Field_unk39_4(void);

u8 *Field_unk40_4(s32 n);

u8 *Field_unk41_4(void);

u8 *Field_unk42_4(s32 n);

u8 *Field_unk43_4(void);

u8 *Field_unk44_4(s32 n);

u8 *Field_unk45_4(void);

u8 *Field_unk46_4(s32 n);

u8 *Field_unk47_4();

u8 *Field_unk48_4();

u8 *Field_unk49_4();

u8 *Field_unk50_4();

u8 *Field_unk51_4();

u8 *Field_unk52_4();

u8 *Field_unk53_4(void);

u8 *Field_unk54_4(s32 n);

u8 *Field_unk55_4();

u8 *Field_unk56_4(void);

u8 *Field_unk57_4(s32 n);

u8 *Field_unk58_4();

u8 *Field_unk59_4();
u8 *Field_unk60_4();

u8 *Field_unk61_4();

u8 *Field_unk62_4();

u8 *Field_unk63_4();

u8 *Field_unk64_4();

u8 *Field_unk65_4();

u8 *Field_unk66_4();

u8 *Field_unk67_4();

u8 *Field_unk68_4();

u8 *Field_unk69_4();

u8 *Field_unk70_4();

u8 *Field_unk71_4();

void *Field_unk72_4();

u8 *Field_unk73_4();

u8 *Field_unk74_4();

void Value();
u8 *Value4();

u8 *Field_unk75_4();
u8 *Field_unk76_4();
u8 *Field_unk77_4();

u8 *Field_unk78_4();
u8 *Field_unk79_4();
u8 *Field_unk80_4();

u8 *Field_unk81_4();
u8 *Field_unk82_4();

void Value5();
void Value6();

u8 *Field_unk83_4();

void Value_0200e5d0_b();
void Value7();

u8 *Field_unk84_4();

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/* The scene work record pointer; +0x1c0 holds the scene request word. */

/*
 * Actor slot search for resource_3b1.  The 48-byte owner at 0x02005038 has no
 * pool; the halfword at 0x02005066 is alignment before the next owner.
 */

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

/*
 * resource_3b1 helper: set bit 3 of the flags byte of actors 28 through 35.
 */

/*
 * The owner at 0x020037b4 is 36 bytes: 34 bytes of code and one alignment
 * halfword; it needs no literal pool.  The index, bit and zero are held in
 * locals initialised in that order.  The zero is an OR identity that folds
 * away unless it has its own local, and initialising the index in a for-init
 * emits it last.  Field_unk55_4 names the in-overlay entry point, not its
 * veneer.
 */
static __inline__ void SetDirection(s32 actor, s32 direction, s32 duration)
{
    Field_unk255(actor, direction, duration);
}

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 id)
{
    return f(id);
}

/*
 * Flag-gated scene setup for overlay resource_3b1. Each callee name refers
 * to that call site's own call word rather than to a shared runtime
 * address.
 */

/* Scene selection chain of the resource_3b1 overlay: tests scene ids in turn and runs the matching call sequence. */

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a)
{
    return f(a);
}

/* The halfword field at +6 of a scene record is written from an int-sized
 * value; storing a plain constant through the cast would make the compiler
 * fetch a halfword literal from the pool instead. */
static __inline__ void SetPose(u8 *rec, s32 pose)
{
    extern const s32 gOv26[];
    extern u8 gWork[];

    *(u16 *)(rec + 6) = pose;
}

s32 Field_unk85_4(void);      /* site 0x1086 */

void Field_unk86_4();         /* site 0x1092 */

void Field_unk87_4();         /* site 0x109e */

s32 Field_unk88_4();          /* site 0x107e; story flag test */

void Field_unk89_4();         /* site 0x108c */

void Field_unk90_4();         /* site 0x1098 */

void Field_unk91_4();         /* site 0x10a6 */

u8 *Field_unk92_4();          /* site 0x10ac; returns the actor record */

void Field_unk93_4();         /* site 0x10be */

void Field_unk94_4();         /* site 0x10c4 */

void Field_unk95_4();         /* site 0x10ce */

void Field_unk96_4();         /* site 0x10d2 */

s32 Field_unk97_4();          /* site 0x10da; story flag test */

s32 Field_unk98_4();          /* site 0x10ea; story flag test */

s32 Field_unk99_4();          /* site 0x10fa; story flag test */

void Field_unk100_4();         /* site 0x1108; the three selected variants share it */

void Field_unk101_4();         /* site 0x1114; the default variant has its own site */

/* Contiguous unnamed state-owner run for resource_3b1. */
s32 ResetStagedActorState(struct StagedActor *actor)
{
    InitializeStagedActorState(actor, 1);
    actor->x.value = 0;
    actor->y = 0;
    actor->z.value = 0;
    actor->unknown_24 = 0;
    actor->unknown_28 = 0;
    actor->unknown_2c = 0;
    actor->unknown_3c = 0x80000000;
    actor->unknown_38 = 0x80000000;
    return 0;
}

s32 UpdateStagedActorVerticalPosition(struct StagedActor *actor)
{
    s16 vertical_motion_direction;
    s32 descending_y;
    s32 ascending_y;

    if (actor->vertical_motion_direction != 0) {
        descending_y = (actor->y
            - ((u32)(GetDescendingMotionStep() << 0xF) >> 0x10)) + 0xFFFF8000;
        actor->y = descending_y;
        if (descending_y < 0) {
            vertical_motion_direction = 0;
            goto block_5;
        }
    } else {
        ascending_y = actor->y
            + ((u32)(GetAscendingMotionStep() << 0xF) >> 0x10) + 0x8000;
        actor->y = ascending_y;
        if (ascending_y > 0x80000) {
            vertical_motion_direction = 1;
block_5:
            actor->vertical_motion_direction = vertical_motion_direction;
        }
    }
    return 1;
}

s32 StagedActor_SetAngleByKind(u8 *o)
{
    u32 v = (u32)(Field_unk66(o) << 6) >> 16;

    if (v == 6) {
        s32 k = 0x3000;
        *(u16 *)(o + 6) = k;
    } else if (v == 9) {
        s32 k = 0x5000;
        *(u16 *)(o + 6) = k;
    }
    return 1;
}

void StagedActor_AdvanceCounter98(u8 *o)
{
    u8 *p = o + 98;
    s32 n = *p + 1;

    *p = n;
    if ((u8)n > 80) {
        *(u16 *)(o + 102) += 1;
    }
}

s32 StagedActor_CountdownUntilPositionUnset(u8 *o)
{
    s32 n = *(s32 *)(o + 76);

    if (n != 0) {
        *(s32 *)(o + 76) = n - 1;
    } else {
        return 1;
    }
    if (*(s32 *)(o + 56) == 0x80000000
        && *(s32 *)(o + 60) == *(s32 *)(o + 56)
        && *(s32 *)(o + 64) == *(s32 *)(o + 60)) {
        return 1;
    }
    return 0;
}

void UpdateActorNineEffectMode(struct EffectRecord *record)
{
    struct SceneActor *actor;

    actor = Field_unk38_4(9);
    if (record->state != 0)
        return;
    if (actor->mode == 1) {
        record->angle = 0xd000;
        record->active = 1;
        actor->mode = 0;
    } else if (actor->mode == 2) {
        if (record->active != 0)
            Field_Apply(record, 3);
        record->active = 0;
        actor->mode = 0;
    } else if (actor->mode == 3) {
        record->angle = 0;
        actor->mode = 0;
    }
}

s32 StagedActor_SetReadyState(struct Work_3b1 *work)
{
    struct Rec_3b1 *rec = work->f80;

    work->f89 = 8;
    Field_unk102_4(work, 0);
    rec->mode9 = 1;
    rec->mode15 = 1;
    work->f35 = (work->f35 & ~1) | 2;
    Field_unk103_4(work, 15);
    return 1;
}

/*
 * Overlay resource_3b1. Picks the scene data table for the current scene
 * index, with two arms further narrowed by story flags.
 */
s32 SceneData_SelectTableByWord224(void)
{
    if (gCell[224] == (s32)&Value_0000006f) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTableEB94(void)
{
    return (s32)&gOv3;
}

/*
 * The 292-byte owner at 0x0200054c covers the dispatcher, a 23-entry jump
 * table, the case bodies, an alignment halfword and the literal pool. Case
 * order and the shared arms reproduce the reference: 23 shares an arm with
 * 4 while 22 does not, and the 15/17/19 arm skips 16, 18 and 20. 2208 is
 * synthesised in the reference and stays decimal; 0x928 and 0x93e are pool
 * loads.
 */
u8 *SceneData_SelectTableBySceneIndexAndFlags(void)
{
    s16 *tbl = gCell;
    s32 scene = tbl[225];

    switch (scene) {
    case 1:
    case 2:
        if (Field_unk67(2208) != 0) {
            return gOv7;
        }
        if (Field_unk68(0x928) != 0 && Field_unk69(0x93e) == 0) {
            return gOv6;
        }
        return gOv5;
    case 4:
    case 23:
        if (Field_unk70(0x93e) != 0) {
            return gOv14;
        }
        return gOv11;
    case 5:
        if (Field_unk71(2208) != 0) {
            return gOv9;
        }
        if (Field_unk72(0x93e) != 0) {
            return gOv10;
        }
        return gOv8;
    case 15:
    case 17:
    case 19:
        return gOv12;
    case 21:
        return gOv13;
    default:
        break;
    }

    return gOv4;
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk104_4();
    Field_unk105_4();
    if (Field_Check(0x921) != 0) {
        Field_Do(0x1dd4);
        Field_unk106_4(10, 0);
    } else {
        if (Field_unk2(0x922) != 0) {
            Field_unk2_2(0x1d91);
            Field_unk3(10, 0);
            if (Field_unk4(0, 0) == 0) {
                Field_unk107_4();
                goto L_020006ea;
            }
            Field_unk108_4(10, 2);
            Field_unk109_4(10, 0);
            Field_Place(10, 0xd000, 0);
        } else {
            Field_unk3_2(0x1d31);
            Field_unk110_4(10, 0);
        }
    }
    L_020006ea:;
    Field_unk111_3();
}

void Dialogue_RunActor12Line(void)
{
    Field_unk112_3();
    Field_unk69_2((s32)&Value_00001dd1);
    Field_Apply2(12, 0);
    Field_unk113_3();
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk114_3();
    if (Field_unk5(0x928) != 0) {
        Field_unk4_2(0x1eb2);
        Field_unk115_3(8);
        Field_unk2_3(8, 0xd000, 60);
        Field_unk116_3(8, 4);
        Field_unk117_3(8);
        Field_unk118_3(8, 3);
    } else {
        if (Field_unk6(0x925) != 0) {
            Field_unk5_2(0x1e06);
            Field_unk119_3(8, 0);
        } else {
            if (Field_unk7(0x921) != 0) {
                Field_unk6_2(0x1dcd);
                Field_unk120_3(8, 0);
                record = Field_unk8(0x925);
                if (record != 0) {
                    goto L_020007cc;
                }
                record = Field_unk9(0x924);
                if (record == 0) {
                    goto L_020007cc;
                }
                {
                    u16 *target = (u16 *)((*(u8 *volatile *)gWork + 0x172));
                    s32 shown = 1;

                    *target = shown;
                }
            } else {
                Field_unk7_2(0x1d30);
                Field_unk121_3(8, 0);
            }
        }
    }
    L_020007cc:;
    Field_unk122_3();
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk123_3();
    if (Field_unk10(0x925) != 0) {
        Field_unk124_3(8, 2);
        Field_unk8_2(0x1e13);
        Field_unk125_3(8);
        Field_unk126_3(8, 0, 10);
        Field_unk11(8, 0);
        if (Field_unk12(0, 0) == 0) {
            Field_unk127_3(40);
            Field_unk128_3(8);
            Field_unk13(8, 0x3000);
            Field_unk129_3(8, 0);
            goto L_0200088e;
        }
        bump_step(2);
        Field_unk130_3(8, 0);
        Field_unk3_3(8, 0x3000, 0);
    } else {
        Field_unk9_2(0x1d4e);
        Field_unk131_3(8, 0);
    }
    L_0200088e:;
    Field_unk132_3();
}

void Dialogue_ShowLine1E19Or1D50(void)
{
    Field_unk133_3();
    if (Field_unk73((s32)&Value_00000925) != 0) {
        Field_unk70_2((s32)&Value_00001e19);
        Field_Apply3(10, 0);
    } else {
        Field_unk71_2((s32)&Value_00001d50);
        Field_Apply4(10, 0);
    }
    Field_unk134_3();
}

/*
 * Flag-branched scene setup for overlay resource_3b1. Each callee name
 * refers to that call site's own call word rather than to a shared runtime
 * address.
 */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee name refers
 * to its own call word rather than to a shared runtime address.
 */

/* Scene setup for resource_3b1: installs actors 10 through 17. */

/*
 * Set up actors 24 and 25 -- resource_3b1. A flat setter sequence with no
 * branches; the owner includes its one literal pool word.
 */

/* The pool word, referenced by address so that it is emitted. */

/*
 * The aliases name the call words encoded in the overlay image, and the
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee slot uses
 * its own local veneer, so the names are per call site and not the shared
 * main-image symbol.
 */
void State_RunFlagBranchedActor8Setup(void)
{
    extern u8 *gWork;

    if (Field_unk74(0x300) != 0) {
        u8 *obj = Field_unk45_4();
        u8 *p;

        Field_unk135_3();
        Field_unk72_2(obj);
        Field_unk73_2((s32)&Value_00001e9e);
        Field_unk74_2(8);
        Field_Apply5(obj, 2);
        p = Field_unk46_4(0);
        if (p != 0) {
            Field_unk256(obj, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk75_2(obj);
        Field_unk257(obj, 0, 0);
        Field_unk136_3();
    } else if (Field_unk75((s32)&Value_0000092b) != 0) {
        Field_unk258(8, (s32)&Value_00001e78, 0x990);
    } else if (Field_unk76((s32)&Value_0000092a) != 0) {
        Field_unk258(8, (s32)&Value_00001e78, (s32)&Value_00000917);
    } else if (Field_unk77((s32)&Value_00000929) != 0) {
        Field_unk258(8, (s32)&Value_00001e78, (s32)&Value_00000935);
    } else {
        Field_unk259(8, (s32)&Value_00001e78, (s32)&Value_0000092c);
    }
}

void Dialogue_RunActorTenFlaggedDialogue(void)
{
    if (Field_unk78(0x300) != 0) {
        u8 *o = Field_unk53_4();
        u8 *p;

        Field_unk137_3();
        Field_unk76_2(o);
        Field_unk77_2((s32)&Value_00001e9f);
        Field_unk78_2(10);
        Field_Apply6(o, 2);
        p = Field_unk54_4(0);
        if (p != 0) {
            Field_unk260(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk79_2(o);
        Field_unk261(o, 0, 0);
        Field_unk138_3();
    } else if (Field_unk79((s32)&Value_0000092b) != 0) {
        Field_unk262(10, (s32)&Value_00001e7b, (s32)&Value_00000992);
    } else if (Field_unk80((s32)&Value_0000092a) != 0) {
        Field_unk262(10, (s32)&Value_00001e7b, (s32)&Value_00000919);
    } else if (Field_unk81((s32)&Value_00000929) != 0) {
        Field_unk262(10, (s32)&Value_00001e7b, (s32)&Value_00000937);
    } else {
        Field_unk263(10, (s32)&Value_00001e7b, (s32)&Value_0000092e);
    }
}

void Scene_RunActor11FlagDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_unk82(0x8A0) != 0) {
        Field_unk139_3();
        Field_Apply7(11, 0x102);
        Field_unk80_2(40);
        Field_unk81_2((s32)&Value_00001f47);
        Field_Apply8(11, 0);
        Field_unk140_3();
    } else if (Field_unk83(0x300) != 0) {
        u8 *o = Field_unk39_4();
        u8 *p;

        Field_unk141_2();
        Field_unk82_2(o);
        Field_unk83_2((s32)&Value_00001ea0);
        Field_unk84_2(11);
        Field_Apply9(o, 2);
        p = Field_unk40_4(0);
        if (p != 0) {
            Field_unk264(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk85_2(o);
        Field_unk265(o, 0, 0);
        Field_unk142_2();
    } else if (Field_unk84((s32)&Value_0000092b) != 0) {
        Field_unk266(11, (s32)&Value_00001e7e, (s32)&Value_00000993);
    } else if (Field_unk85((s32)&Value_0000092a) != 0) {
        Field_unk266(11, (s32)&Value_00001e7e, (s32)&Value_0000091a);
    } else if (Field_unk86((s32)&Value_00000929) != 0) {
        Field_unk266(11, (s32)&Value_00001e7e, (s32)&Value_00000938);
    } else {
        Field_unk267(11, (s32)&Value_00001e7e, (s32)&Value_0000092f);
    }
}

void Scene_RunScene3b1SequenceA(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_unk143_2();
    if (Field_unk14(0x300) != 0) {
        rec7 = Field_Run();
        Field_unk144_2();
        Field_unk10_2(0x1ea1);
        Field_unk145_2(12);
        Field_unk146_2(rec7, 2);
        record = Field_unk15(0);
        if (record != 0) {
            Field_unk147_2(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Field_unk148_2(rec7);
        Field_unk149_2(rec7, 0, 0);
    } else {
        Field_unk150_2(12, 2);
        Field_unk151_2(20);
        Field_unk11_2(0x1e81);
        Field_unk16(12, 0);
        if (Field_unk17(0, 0) == 0) {
            Field_unk152_2(12);
            Field_unk153_2(12, 2);
            record = Field_unk18(0);
            if (record != 0) {
                Field_unk154_2(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Field_unk155_2(12);
            Field_unk156_2(12, 0, 0);
            Field_unk12_2(0x300);
            if (Field_unk19(0x92b) != 0) {
                Field_unk13_2(0x994);
                goto L_02000c9a;
            }
            if (Field_unk20(0x92a) != 0) {
                Field_unk14_2(0x91b);
                goto L_02000c9a;
            }
            if (Field_unk21(0x929) != 0) {
                Field_unk15_2(0x939);
                goto L_02000c9a;
            }
            Field_unk16_2(0x930);
        } else {
            bump_step(1);
            Field_unk157_2(12);
        }
    }
    L_02000c9a:;
    Field_unk158_2();
}

void Scene_RunScene3b1SequenceB(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_unk159_2();
    if (Field_unk22(0x300) != 0) {
        rec7 = Field_unk2_4();
        Field_unk160_2();
        Field_unk17_2(0x1ea2);
        Field_unk161_2(9);
        Field_unk162_2(rec7, 2);
        record = Field_unk23(0);
        if (record != 0) {
            Field_unk163_2(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ((void (*)())Field_unk87)(rec7);
        Field_unk164_2(rec7, 0, 0);
    } else {
        Field_unk18_2(0x1e84);
        ((void (*)())Field_unk88)(9, 0, 60);
        Field_unk165_2(9, 1);
        Field_unk24(9, 0);
        if (Field_unk25(0, 0) == 0) {
            Field_unk166_2(9);
            Field_unk167_2(9, 2);
            record = Field_unk26(0);
            if (record != 0) {
                Field_unk168_2(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Field_unk169_2(9);
            Field_unk170_2(9, 0, 0);
            Field_unk19_2(0x300);
            if (Field_unk27(0x92b) != 0) {
                Field_unk20_2(0x991);
                goto L_02000de0;
            }
            if (Field_unk28(0x92a) != 0) {
                Field_unk21_2(0x918);
                goto L_02000de0;
            }
            if (Field_unk29(0x929) != 0) {
                Field_unk22_2(0x936);
                goto L_02000de0;
            }
            Field_unk23_2(0x92d);
        } else {
            bump_step(1);
            Field_unk171_2(9);
        }
    }
    L_02000de0:;
    Field_unk172_2();
}

void Dialogue_RunActorThirteenFlag300Branch(void)
{
    if (Field_unk89(0x300) != 0) {
        u8 *o = Field_unk56_4();
        u8 *p;

        Field_unk173_2();
        Field_unk86_2(o);
        Field_unk87_2((s32)&Value_00001ea3);
        Field_unk88_2(13);
        Field_Apply10(o, 2);
        p = Field_unk57_4(0);
        if (p != 0) {
            Field_unk268(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk89_2(o);
        Field_unk269(o, 0, 0);
        Field_unk174_2();
    } else if (Field_unk90((s32)&Value_0000092b) != 0) {
        Field_unk270(13, (s32)&Value_00001e88, (s32)&Value_00000995);
    } else if (Field_unk91((s32)&Value_0000092a) != 0) {
        Field_unk270(13, (s32)&Value_00001e88, (s32)&Value_0000091c);
    } else if (Field_unk92((s32)&Value_00000929) != 0) {
        Field_unk270(13, (s32)&Value_00001e88, (s32)&Value_0000093a);
    } else {
        Field_unk271(13, (s32)&Value_00001e88, (s32)&Value_00000931);
    }
}

void Scene_RunFlag300BranchDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_unk93(0x300) != 0) {
        u8 *o = Field_unk41_4();
        u8 *p;

        Field_unk175_2();
        Field_unk90_2(o);
        Field_unk91_2((s32)&Value_00001ea4);
        Field_unk92_2(14);
        Field_Apply11(o, 2);
        p = Field_unk42_4(0);
        if (p != 0) {
            Field_unk272(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk93_2(o);
        Field_unk273(o, 0, 0);
        Field_unk176_2();
    } else if (Field_unk94((s32)&Value_0000092b) != 0) {
        Field_unk274(14, (s32)&Value_00001e8b, (s32)&Value_00000996);
    } else if (Field_unk95((s32)&Value_0000092a) != 0) {
        Field_unk274(14, (s32)&Value_00001e8b, (s32)&Value_0000091d);
    } else if (Field_unk96((s32)&Value_00000929) != 0) {
        Field_unk274(14, (s32)&Value_00001e8b, (s32)&Value_0000093b);
    } else {
        Field_unk275(14, (s32)&Value_00001e8b, (s32)&Value_00000932);
    }
}

void Scene_RunActor15FlagDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_unk97(0x300) != 0) {
        u8 *o = Field_unk43_4();
        u8 *p;

        Field_unk177_2();
        Field_unk94_2(o);
        Field_unk95_2((s32)&Value_00001ea5);
        Field_unk96_2(15);
        Field_Apply12(o, 2);
        p = Field_unk44_4(0);
        if (p != 0) {
            Field_unk276(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk97_2(o);
        Field_unk277(o, 0, 0);
        Field_unk178_2();
    } else if (Field_unk98((s32)&Value_0000092b) != 0) {
        Field_unk278(15, (s32)&Value_00001e8e, (s32)&Value_00000997);
    } else if (Field_unk99((s32)&Value_0000092a) != 0) {
        Field_unk278(15, (s32)&Value_00001e8e, (s32)&Value_0000091e);
    } else if (Field_unk100((s32)&Value_00000929) != 0) {
        Field_unk278(15, (s32)&Value_00001e8e, (s32)&Value_0000093c);
    } else {
        Field_unk279(15, (s32)&Value_00001e8e, (s32)&Value_00000933);
    }
}

/*
 * With story flag 0x300 set, opens a scripted sequence, creates a local
 * object, shows message 0x1ea6, configures it as slot 2, moves it onto actor
 * 0's signed halfword coordinates when actor 0 exists, then releases and
 * closes the sequence.  Otherwise flags 0x92b, 0x92a and 0x929 select the
 * setup call's third argument.  The 204-byte owner includes an alignment
 * halfword and its nine pool words.
 */
void Scene_RunActor16FlagDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    s32 obj;
    u8 *actor;

    if (Field_unk88_4(0x300) != 0) {
        obj = Field_unk85_4();
        Field_unk89_4();
        Field_unk86_4(obj);
        Field_unk90_4(0x1ea6);
        Field_unk87_4(16);
        Field_unk91_4(obj, 2);

        actor = Field_unk92_4(0);
        if (actor != 0) {
            Field_unk93_4(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Field_unk94_4(obj);
        Field_unk95_4(obj, 0, 0);
        Field_unk96_4();
    } else {
        if (Field_unk97_4(0x92b) != 0) {
            Field_unk100_4(16, 0x1e91, 0x998);
        } else if (Field_unk98_4(0x92a) != 0) {
            Field_unk100_4(16, 0x1e91, 0x91f);
        } else if (Field_unk99_4(0x929) != 0) {
            Field_unk100_4(16, 0x1e91, 0x93d);
        } else {
            Field_unk101_4(16, 0x1e91, 0x934);
        }
    }
}

struct SceneActor *FindActorNearPosition(s32 x, s32 y)
{
    struct SceneWork *work;
    struct SceneActor **actor;
    struct SceneActor *current;
    u32 i;
    s32 actor_x;
    s32 actor_y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    work = *(struct SceneWork **)0x03001ebc;
    i = 8;
    left = x - 12;
    right = x + 12;
    top = y - 12;
    bottom = y + 12;
    actor = work->actors;
    while (i <= 65) {
        current = *actor++;
        actor_x = current->x;
        actor_y = current->y;
        if (left < actor_x && right > actor_x &&
            top < actor_y && bottom > actor_y)
            return current;
        i++;
    }
    return 0;
}

/* Actor placement check for resource_3b1. */

/* Bucket offsets, packed as {s16 hi; s16 lo} per entry. */

/*
 * Offset obj+10 and obj+18 by the bucket's packed hi/lo pair, test the
 * candidate point, and on success pack {x << 16, obj+12, z << 16} into a
 * stack struct for a second check.  Returns 1 only if both checks pass.  The
 * owner includes its one pool word, the bucket table base.  Callees are named
 * by the address their call site computes, not by a runtime address.
 */
s32 Actor_CheckBucketOffsetPoint(s32 bucket)
{
    u8 *obj = Field_unk58_4(0);
    u32 ofs = gOv17[bucket];
    s32 x = *(s16 *)(obj + 10) + ((s32)ofs >> 16);
    s32 z = *(s16 *)(obj + 18) + (s32)(s16)ofs;

    if (Field_unk101(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(obj + 12);
        point[2] = z << 16;

        if (Field_unk102(obj, point) != 0) {
            return 0;
        }
    }

    return 1;
}

/*
 * Level selection from scene flags, overlay resource_3b1. Each callee name
 * refers to its own call word rather than to a shared runtime address.
 */

/*
 * Actor 8 flag setup for overlay resource_3b1. Each callee name refers to
 * its own call word rather than to a shared runtime address.
 */

/* Scene state helper for overlay resource_3b1. */

/*
 * Picks a level from the highest flag that is set and applies it. The
 * 72-byte owner at 0x020012dc includes its three pool words, which are the
 * addresses taken as Value_0000092b, Value_0000092a and Value_00000929.
 */
s32 State_ApplyLevelFromFlags(void)
{
    s32 ret = 0;

    if (Field_unk103((s32)&Value_0000092b) != 0) {
        ret = 3;
    } else if (Field_unk104((s32)&Value_0000092a) != 0) {
        ret = 2;
    } else if (Field_unk105((s32)&Value_00000929) != 0) {
        ret = 1;
    }

    return Field_unk106(ret, 1);
}

/* Story selector owner at 0x02001324, 84 bytes; eight calls. Per-site call
 * veneers (raw asm confirms each callee slot uses a distinct local stub
 * even across the three near-identical "twin" owners at 0x1324/1378/13cc). */
void Dialogue_ShowLine1ECETo1ED0(void)
{
    Field_unk179_2();
    if (Field_unk180_2(0x92c)) Field_unk181_2(0x1ece);
    else if (Field_unk182_2(0x935)) Field_unk183_2(0x1ecf);
    else Field_unk184_2(0x1ed0);
    Field_unk185_2(0x12, 0); Field_unk186_2();
}

/* Story selector owner at 0x02001378, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x13cc with distinct local stub addresses). */
void Dialogue_RunActor19TwoFlagLineA(void)
{
    Field_unk187_2();
    if (Field_unk188_2(0x92d)) Field_unk189_2(0x1ece);
    else if (Field_unk190_2(0x936)) Field_unk191_2(0x1ecf);
    else Field_unk192_2(0x1ed0);
    Field_unk193_2(0x13, 0); Field_unk194_2();
}

/* Story selector owner at 0x020013cc, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x1378 with distinct local stub addresses). */
void Dialogue_RunActor20TwoFlagLine(void)
{
    Field_unk195_2();
    if (Field_unk196_2(0x92e)) Field_unk197_2(0x1ece);
    else if (Field_unk198_2(0x937)) Field_unk199_2(0x1ecf);
    else Field_unk200_2(0x1ed0);
    Field_unk201_2(0x14, 0); Field_unk202_2();
}

/* Story selector owner at 0x02001420, 60 bytes; six calls. */
void Dialogue_ShowLine1ED1Or1ED2(void)
{
    Field_unk203_2();
    if (Field_unk204_2(0x92f)) Field_unk205_2(0x1ed1);
    else Field_unk206_2(0x1ed2);
    Field_unk207_2(21, 0); Field_unk208_2();
}

/* Story selector owner at 0x0200145c, 84 bytes; eight calls. */
void Dialogue_RunActor22TwoFlagLine(void)
{
    Field_unk209_2();
    if (Field_unk210_2(0x930)) Field_unk211_2(0x1ece);
    else if (Field_unk212_2(0x939)) Field_unk213_2(0x1ecf);
    else Field_unk214_2(0x1ed0);
    Field_unk215_2(22, 0); Field_unk216_2();
}

/* Story selector owner at 0x020014b0, 84 bytes; eight calls. */
void Dialogue_RunActor23BranchedDialogue(void)
{
    Field_unk217_2();
    if (Field_unk218_2(0x931)) Field_unk219_2(0x1ece);
    else if (Field_unk220_2(0x93a)) Field_unk221_2(0x1ecf);
    else Field_unk222_2(0x1ed0);
    Field_unk223_2(23, 0); Field_unk224_2();
}

/* Story selector owner at 0x02001504, 84 bytes; eight calls. */
void Dialogue_RunActor24BranchedDialogue(void)
{
    Field_unk225_2();
    if (Field_unk226_2(0x932)) Field_unk227_2(0x1ece);
    else if (Field_unk228_2(0x93b)) Field_unk229_2(0x1ecf);
    else Field_unk230_2(0x1ed0);
    Field_unk231_2(24, 0); Field_unk232_2();
}

/* Story selector owner at 0x02001558, 60 bytes; six calls. */
void Dialogue_RunActor25FlaggedLine(void)
{
    Field_unk233_2();
    if (Field_unk234_2(0x933)) Field_unk235_2(0x1ed1);
    else Field_unk236_2(0x1ed2);
    Field_unk237_2(25, 0); Field_unk238_2();
}

/* Second-phase story selector at 0x02001594, 84 bytes; eight calls. */
void Dialogue_RunActor18TwoFlagLine(void)
{
    Field_unk239_2();
    if (Field_unk240_2(0x92c)) Field_unk241_2(0x1edb);
    else if (Field_unk242_2(0x935)) Field_unk243_2(0x1edc);
    else Field_unk244_2(0x1edd);
    Field_unk245_2(18, 0); Field_unk246_2();
}

/* Second-phase story selector at 0x020015e8, 84 bytes; eight calls. */
void Dialogue_RunActor19TwoFlagLineB(void)
{
    Field_unk247_2();
    if (Field_unk248_2(0x92d)) Field_unk249_2(0x1edb);
    else if (Field_unk250_2(0x936)) Field_unk251_2(0x1edc);
    else Field_unk252_2(0x1edd);
    Field_unk253_2(19, 0); Field_unk254_2();
}

/* Second-phase story selector at 0x0200163c, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Field_unk255_2();
    if (Field_unk256_2(0x92e)) Field_unk257_2(0x1edb);
    else if (Field_unk258_2(0x937)) Field_unk259_2(0x1edc);
    else Field_unk260_2(0x1edd);
    Field_unk261_2(20, 0); Field_unk262_2();
}

/* Second-phase story selector at 0x02001690, 60 bytes; six calls. */
void Dialogue_RunActor21FlaggedLine(void)
{
    Field_unk263_2();
    if (Field_unk264_2(0x92f)) Field_unk265_2(0x1ede);
    else Field_unk266_2(0x1edf);
    Field_unk267_2(21, 0); Field_unk268_2();
}

/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void Dialogue_RunActor22BranchedDialogue(void)
{
    Field_unk269_2();
    if (Field_unk270_2(0x930)) Field_unk271_2(0x1edb);
    else if (Field_unk272_2(0x939)) Field_unk273_2(0x1edc);
    else Field_unk274_2(0x1edd);
    Field_unk275_2(22, 0); Field_unk276_2();
}

/* Second-phase story selector at 0x02001720, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Field_unk277_2();
    if (Field_unk278_2(0x931)) Field_unk279_2(0x1edb);
    else if (Field_unk280_2(0x93a)) Field_unk281(0x1edc);
    else Field_unk282(0x1edd);
    Field_unk283(23, 0); Field_unk284();
}

/* Second-phase story selector at 0x02001774, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Field_unk285();
    if (Field_unk286(0x932)) Field_unk287(0x1edb);
    else if (Field_unk288(0x93b)) Field_unk289(0x1edc);
    else Field_unk290(0x1edd);
    Field_unk291(24, 0); Field_unk292();
}

/* Second-phase story selector at 0x020017c8, 60 bytes; six calls. */
void Dialogue_ShowLine1EDEOr1EDF(void)
{
    Field_unk293();
    if (Field_unk294(0x933)) Field_unk295(0x1ede);
    else Field_unk296(0x1edf);
    Field_unk297(25, 0); Field_unk298();
}

void Scene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk299();
    Field_unk300(a1);
    Field_unk30(a0, 0);
    if (Field_unk31(0, 0) == 0) {
        Field_unk301(a0);
        Field_unk302(a0, 2);
        record = Field_unk32(0);
        if (record != 0) {
            Field_unk303(a0, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Field_unk304(a0);
        Field_unk305(a0, 0, 0);
        Field_unk24_2(0x300);
        Field_unk306(a2);
    } else {
        bump_step(1);
        Field_unk307(a0);
    }
    Field_unk308();
}

void Scene_RunScene3b1SequenceC(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    record = Field_unk107(0);
    if ((u32)((*(volatile u16 *)(record + 6) + -0x2000) << 16) > -0x40000000) {
        if (Field_unk33(0x928) != 0) {
            if (Field_unk34(0x93e) == 0) {
                Field_unk309(17);
                goto L_0200194e;
            }
        }
        Field_unk310(15);
    } else {
        Field_unk311();
        if (Field_unk35(0x93e) != 0) {
            Field_unk25_2(0x1f81);
        } else {
            if (Field_unk36(0x8a0) != 0) {
                Field_unk26_2(0x1f48);
            } else {
                if (Field_unk37(0x928) != 0) {
                    Field_unk27_2(0x1f7f);
                } else {
                    if (Field_unk38(0x925) != 0) {
                        Field_unk28_2(0x1f7d);
                    } else {
                        Field_unk29_2(0x1f7b);
                    }
                }
            }
        }
        if (Field_unk39(0x928) != 0) {
            record = Field_unk40(0x93e);
            if (record != 0) {
                goto L_02001942;
            }
            Field_unk312(17, 0);
        } else {
            L_02001942:;
            Field_unk313(15, 0);
        }
        Field_unk314();
    }
    L_0200194e:;
}

void Scene_RunScene3b1SequenceD(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Field_unk41(0x301) != 0) {
        Field_unk315();
        Field_unk316(8);
        Field_unk4_3(0x1e48, 1, 8);
        Field_unk5_3(0, 0x19999, 0xcccc);
        Field_unk6_3(0, 0x198, 134);
        Field_unk42(0, 0x4000);
        Field_unk317();
    }
}

/*
 * Gated on flag 0x922.  Every callee slot has its own local call stub;
 * Field_unk318 and Field_unk319 are the same stub declared twice without
 * a prototype, because the two call sites pass different argument counts.
 */

/*
 * Scene setup for resource_3b1.  The 212-byte owner at 0x02001a60 includes
 * the alignment halfword at 0x02001b1a and the six pool words that follow it,
 * ending before the next owner's prologue at 0x02001b34.
 */
void Scene_RunFlagGatedThreeActorSetup(void)
{
    extern u8 *gWork;

    if (Field_unk108(0x922) == 0)
        return;

    Field_unk320();
    Field_unk321();
    Field_Apply13(0x19999, 0x3333);
    Field_unk318(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Field_unk98_2(0x1d26);

    Field_unk99_2(8);
    Field_unk100_2(10);
    Field_Apply14(8, 0x3000);
    Field_unk101_2(8);
    Field_unk319(10, 0xd000);
    Field_unk102_2(10);
    Field_Apply15(9, 0x5000);
    Field_unk103_2(9);

    Field_unk280(8, 0, 20);
    Field_unk104_2(8);
    Field_Apply16(9, 0x8000);
    Field_unk105_2(9);
    Field_unk106_2(10);
    Field_unk107_2(8);
    Field_Apply17(10, 0xb000);
    Field_unk108_2(8);

    Field_unk109_2(0x920);
    Field_unk322();
}

void Scene_RunThreeActorPresentation(void)
{
    s32 request_a;
    s32 request_b;
    s32 action;

    if (Field_unk43(0x911) == 0) {
    } else {
        Field_unk323();
        Field_unk324();
        Field_unk3_4(0x26666, 0x4ccc);
        Field_unk325( 0x5b70000, -1, 0x1d00000, 0x10000014);
        Field_unk326(13, 1);
        Field_unk30_2(0x1d56);
        Field_unk327(0x200d);
        Field_unk328(12, 0xd000);
        request_a = 0x800c;
        Field_unk7_3(12, 0x102, 20);
        Field_unk329(12, 2);
        Field_unk330(request_a);
        Field_unk331(14, 1);
        Field_unk8_3(0xa00e, 0, 20);
        Field_unk332(12, 0);
        request_b = 0xa00e;
        Field_unk9_3(12, 0x101, 40);
        Field_unk10_3(14, 0x103, 40);
        Field_unk333(14, 3);
        Field_unk334(request_b);
        Field_unk4_4(12, 0x102);
        Field_unk335(40);
        Field_unk336(12, 3);
        Field_unk337(request_a);
        Field_unk338(14, 1);
        Field_unk339(request_b);
        Field_unk340(14, 0xb000);
        Field_unk341(request_b);
        Field_unk342(12, 0xd000);
        Field_unk11_3(12, 0x100, 30);
        Field_unk343(12, 1);
        Field_unk344(request_a);
        Field_unk345(13, 4);
        Field_unk346(0x200d);
        Field_unk347(13, 2);
        Field_unk348(0x200d);
        Field_unk349(12, 4);
        Field_unk350(request_a);
        Field_unk351(14, 4);
        Field_unk352(request_b);
        Field_unk353(14, 0x8000);
        Field_unk354(14, 2);
        Field_unk355(request_b, 0, 20);
        Field_unk356(12, 0, 0);
        Field_unk12_3(12, 0x102, 80);
        Field_unk357(request_a, 0, 20);
        Field_unk13_3(14, 0x103, 0);
        Field_unk14_3(13, 0x103, 60);
        Field_unk358(14, 2);
        Field_unk359(request_b);
        Field_unk360(14, 0xb000);
        Field_unk361(14, 1);
        Field_unk362(request_b);
        Field_unk363(13, 0x3000);
        Field_unk15_3(13, 0x101, 0);
        Field_unk16_3(12, 0x101, 60);
        Field_unk364(13, 1);
        Field_unk365(13);
        Field_unk17_3(14, 0x103, 40);
        Field_unk366(14, 1);
        Field_unk367(request_b);
        Field_unk368(12, 0xd000, 0);
        Field_unk18_3(13, 0x5000, 40);
        Field_unk19_3(12, 0, 0);
        Field_unk369(13, 0x3000);
        Field_unk370(12, 2);
        Field_unk371(request_a, 0, 20);
        Field_unk20_3(14, 0x4000, 40);
        Field_unk372(request_b);
        Field_unk373(12, 2);
        Field_unk374(13, 2);
        Field_unk375(60);
        Field_unk376(13, 1);
        Field_unk377(13);
        Field_unk378(14, 3);
        Field_unk379(request_b);
        Field_unk21_3(12, 0x102, 40);
        Field_unk5_4(12, 2);
        Field_unk380(request_a);
        Field_unk381(13, 3);
        Field_unk382(13);
        Field_unk383(14, 0xb000, 40);
        Field_unk384(14, 3);
        Field_unk385(13, 3);
        Field_unk22_3(14, 0x19999, 0xcccc);
        Field_unk23_3(13, 0x19999, 0xcccc);
        action = (s32)gOv18;
        Field_unk386(14, action);
        Field_unk387(13, action);
        Field_unk388(20);
        Field_unk24_3(12, 0x4000, 0);
        Field_unk25_3(0, 0x26666, 0x13333);
        *(u8 *)(Field_unk59_4(0) + 90) &= 254;
        Field_unk26_3(0, 184, 0x208);
        Field_unk389(1);
        {
            u8 *record = Field_unk60_4(0);
            u32 flag = 1;

            flag = flag | record[90];
            record[90] = (u8)flag;
        }
        Field_unk390(0, 0x8000, 20);
        Field_unk27_3(0, 0x4000, 0);
        Field_unk391(12, 4, 20);
        Field_unk28_3(0, 0xa000, 20);
        Field_unk392(12, 2);
        Field_unk393(12);
        Field_unk29_3(12, 0x19999, 0xcccc);
        Field_unk394(12, action);
        Field_unk395(40);
        Field_unk30_3(0, 0x4000, 0);
        Field_unk396(12);
        Field_unk31_2(0x922);
        Field_unk397();
    }
}

void Scene_RunExtendedActorChoreography(void)
{
    s32 request_a;
    s32 request_b;
    s32 request_c;

    Field_unk398(28);
    Field_unk6_4(0x26666, 0x4ccc);
    Field_unk399(0x1c80000, -1, 0x2880000, 0x10000014);
    Field_unk400(9, 1);
    Field_unk32_2(0x1d93);
    Field_unk401(9);
    Field_unk31_3(0, 0xd000, 0);
    Field_unk32_3(10, 0xd000, 0);
    Field_unk402(11, 0, 0);
    Field_unk33_3(12, 0x3000, 0);
    Field_unk34_3(13, 0x8000, 40);
    Field_unk35_3(9, 0x103, 40);
    Field_unk403(9, 2);
    Field_unk404(9);
    Field_unk405(12, 0, 0);
    Field_unk36_3(11, 0xd000, 0);
    Field_unk37_3(13, 0xd000, 20);
    Field_unk406(11, 1);
    Field_unk407(0x100b);
    Field_unk38_3(13, 0x102, 20);
    Field_unk408(13, 2);
    Field_unk409(13);
    Field_unk39_3(9, 0x105, 60);
    Field_unk410(9);
    Field_unk40_3(12, 0x104, 20);
    Field_unk33_2(0x900c);
    Field_unk411(8, 1);
    Field_unk412(8, 3);
    Field_unk413(8);
    Field_unk414(12, 0x3000);
    Field_unk34_2(0x900c);
    Field_unk415(11, 0xb000);
    Field_unk416(11, 3);
    Field_unk417(10);
    Field_unk418(13, 1);
    Field_unk419(13, 3);
    Field_unk420(13);
    Field_unk41_3(13, 0x8000, 0);
    Field_unk42_3(12, 0x5000, 0);
    Field_unk421(11, 0x5000);
    Field_unk43_3(13, 0x6666, 0x3333);
    Field_unk44_3(12, 0xcccc, 0x6666);
    Field_unk45_3(12, 0x1bc, 0x29c);
    Field_unk46_3(13, 0x1d8, 0x29c);
    Field_unk422(12);
    Field_unk423(12, 1);
    Field_unk424(80);
    Field_unk425(12, 0xd000);
    Field_unk47_3(12, 0x101, 60);
    Field_unk426(11, 1);
    Field_unk427(20);
    Field_unk48_3(0x400b, 0, 40);
    Field_unk428(11, 2);
    Field_unk429(11, 0xd000, 0);
    Field_unk430(0x100b);
    Field_unk431(12, 0xd000, 0);
    Field_unk49_3(9, 0x101, 60);
    Field_unk432(11, 4);
    Field_unk433(20);
    Field_unk434(0x100b);
    Field_unk435(9, 3);
    Field_unk436(9);
    Field_unk437(13, 0xd000, 0);
    Field_unk7_4(13, 0x102);
    Field_unk438(13, 2, 20);
    Field_unk439(13);
    Field_unk440(9, 3);
    request_a = 0x100c;
    Field_unk441(9);
    Field_unk442(11, 0xd000);
    Field_unk443(12, 1);
    Field_unk444(request_a);
    Field_unk50_3(8, 0x105, 40);
    Field_unk445(8, 3);
    Field_unk446(8);
    Field_unk51_3(13, 0x102, 40);
    Field_unk447(13, 4, 0);
    Field_unk448(13);
    Field_unk449(9, 3);
    Field_unk450(9);
    Field_unk451(11, 1);
    Field_unk452(0x100b);
    Field_unk52_3(8, 0x102, 40);
    Field_unk453(8, 0);
    Field_unk454(11, 2);
    Field_unk455(0x100b, 0, 40);
    Field_unk53_3(9, 0x100, 0);
    Field_unk456(9, 0x5000, 20);
    Field_unk457(9, 2);
    Field_unk458(9, 0, 20);
    Field_unk459(11, 3);
    Field_unk460(20);
    Field_unk54_3(12, 0x100, 40);
    Field_unk461(12, 2);
    request_b = 0x400b;
    Field_unk462(request_a);
    Field_unk463(11, 0x5000, 20);
    Field_unk464(request_b);
    Field_unk465(12, 2);
    Field_unk466(request_a);
    Field_unk467(11, 3);
    Field_unk468(11, 1);
    Field_unk469(request_b);
    Field_unk55_3(12, 0x102, 60);
    Field_unk470(9, 1);
    Field_unk471(9);
    Field_unk56_3(11, 0x101, 40);
    Field_unk472(11, 0xd000, 20);
    Field_unk473(9, 3);
    Field_unk474(9);
    Field_unk57_3(11, 0x103, 20);
    Field_unk475(11, 2);
    Field_unk476(0x100b);
    Field_unk58_3(9, 0x108, 40);
    Field_unk477(9);
    Field_unk478(8, 1);
    Field_unk479(8, 3);
    Field_unk480(8);
    Field_unk481(9, 0xd000, 40);
    Field_unk482(9);
    Field_unk483(12, 1);
    Field_unk484(20);
    Field_unk485(request_a);
    Field_unk486(11, 0x5000, 0);
    Field_unk487(9, 0x5000, 0);
    Field_unk59_3(13, 0x8000, 0);
    Field_unk60_3(0, 0xc000, 0);
    Field_unk488(10, 0xb000, 40);
    Field_unk489(11, 1);
    Field_unk490(request_b);
    Field_unk491(12, 3);
    Field_unk492(request_a, 0, 20);
    Field_unk493(9, 2);
    Field_unk494(9);
    Field_unk61_3(12, 0x108, 40);
    Field_unk495(12, 3);
    Field_unk496(request_a);
    Field_unk497(8, 3);
    Field_unk498(8);
    Field_unk62_3(8, 0x8000, 20);
    Field_unk499(19);
    Field_unk500(8, 2);
    request_c = 0x8008;
    Field_unk63_3(8, 0x100, 80);
    Field_unk501(request_c);
    Field_unk64_3(12, 0x101, 0);
    Field_unk65_3(11, 0x101, 0);
    Field_unk66_3(13, 0x101, 0);
    Field_unk67_3(10, 0x101, 0);
    Field_unk68_3(0, 0x101, 40);
    Field_unk502(12, 0xd000, 0);
    Field_unk503(11, 0xd000, 0);
    Field_unk504(13, 0xb000, 0);
    Field_unk505(10, 0xb000, 0);
    Field_unk69_3(0, 0xc000, 40);
    Field_unk8_4(8, 0x102);
    Field_unk506(8, 4, 40);
    Field_unk507(8, 2);
    Field_unk508(request_c, 0);
    Field_unk70_3(8, 0x19999, 0xcccc);
    Field_unk71_3(8, 0x1db, 0x256);
    Field_unk72_3(8, 0x8000, 0);
    Field_unk73_3(9, 0x10000, 0x8000);
    Field_unk74_3(9, 0x1ce, 0x26a);
    Field_unk509(9, 0xb000);
    Field_unk75_3(9, 0x100, 40);
    Field_unk510(9, 2);
    Field_unk35_2(0x8009);
    Field_unk76_3(11, 0x101, 60);
    Field_unk511(11);
    Field_unk77_3(12, 0x102, 20);
    Field_unk512(request_a);
    Field_unk78_3(8, 0x103, 20);
    Field_unk513(8, 4, 0);
    Field_unk514(8, 0x5000, 20);
    Field_unk515(8);
    Field_unk516(28);
    Field_unk517(8, 3);
    Field_unk518(8);
    Field_unk79_3(13, 0x101, 60);
    Field_unk519(13);
    Field_unk520(8, 0x3000);
    Field_unk521(8, 4);
    Field_unk522(8);
    Field_unk80_3(12, 0x1bc, 0x274);
    Field_unk523(12, 0xd000);
    Field_unk36_2(0x900c);
    Field_unk524(8, 0x5000, 20);
    Field_unk525(8, 3);
    Field_unk526(8);
    Field_unk81_3(11, 0x102, 60);
    Field_unk527(0x100b);
    Field_unk82_3(13, 0x107, 40);
    Field_unk528(13, 2);
    Field_unk529(13);
    Field_unk530(9, 0x3000);
    Field_unk531(9, 4);
    Field_unk37_2(0x1009);
    Field_unk532(12, 0);
    Field_unk533(8, 1);
    Field_unk534(8);
    Field_unk535(11, 0, 0);
    Field_unk83_3(12, 0x105, 0);
    Field_unk84_3(9, 0x105, 60);
    Field_unk9_4(0x13333, 0x2666);
    Field_unk536(0x1d00000, -1, 0x2a80000, 0x10000000);
    Field_unk537(10, 1);
    Field_unk538(10, 0);
    Field_unk539(10);
    Field_unk540(0, 0, 0);
    Field_unk85_3(10, 0x102, 40);
    Field_unk541(10);
    Field_unk86_3(10, 0x8000, 20);
    Field_unk87_3(10, 0x100, 0);
    Field_unk542(10, 4, 40);
    Field_unk543(10);
    Field_unk544(10, 1);
    Field_unk545(10);
    Field_unk88_3(13, 0x10000, 0x8000);
    Field_unk89_3(13, 0x1b6, 0x293);
    Field_unk90_3(8, 0x8000, 0);
    Field_unk546(9, 0xb000, 0);
    Field_unk547(12, 0x3000, 0);
    Field_unk548(11, 0xb000, 0);
    Field_unk549(17);
    Field_unk91_3(10, 0x10000, 0x8000);
    Field_unk92_3(10, 0x1e8, 0x2ae);
    Field_unk550(10, 0xb000, 0);
    Field_unk551(13);
    Field_unk552(13, 1);
    Field_unk553(13, 0xd000, 0);
    Field_unk554();
    Field_unk38_2(0x921);
}

void Scene_RunBranchingActorPresentation(void)
{
    u8 *record;
    s32 request_a;
    s32 request_b;
    s32 value;
    s32 request_c;
    s32 action;

    Field_unk555();
    Field_unk556();
    Field_unk557(8, 0, 0);
    Field_unk93_3(8, 0x100, 40);
    request_a = 0x1008;
    Field_unk558(8, 3);
    Field_unk39_2(0x1ddb);
    Field_unk559(request_a);
    Field_unk560(9, 1);
    Field_unk561(12, 1);
    Field_unk562(11, 1);
    Field_unk563(13, 1);
    Field_unk564(10, 1);
    Field_unk94_3(9, 0xd000, 0);
    Field_unk95_3(12, 0xd000, 0);
    Field_unk96_3(11, 0xd000, 0);
    Field_unk97_3(13, 0xd000, 0);
    Field_unk98_3(10, 0xb000, 20);
    Field_unk565(8, 1);
    Field_unk44(request_a, 0);
    if (Field_unk45(0, 0) == 0) {
        Field_unk566(9, 2);
        Field_unk40_2(0x9009);
        Field_unk99_3(8, 0x108, 40);
        Field_unk567(request_a);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
        Field_unk568(9, 1);
        Field_unk41_2(0x9009);
        Field_unk569(8, 2);
        Field_unk42_2(0x9008);
    }
    Field_unk100_3(13, 0x105, 40);
    Field_unk10_4(0xcccc, 0x1999);
    Field_unk11_4(0x1d80000, -1, 0x27c0000, 1);
    Field_unk101_3(13, 0x10000, 0x8000);
    Field_unk102_3(13, 0x1d8, 0x296);
    Field_unk570(13, 0xb000);
    Field_unk571(13);
    Field_unk572(8, 0x5000);
    Field_unk573(8, 3);
    Field_unk574(9, 3);
    Field_unk575(11, 0, 0);
    Field_unk103_3(13, 0x8000, 20);
    Field_unk576(11, 3);
    Field_unk577(13, 3);
    Field_unk578(20);
    Field_unk579(12, 1);
    Field_unk580(12, 0x3000);
    Field_unk104_3(0x100c, 0, 20);
    Field_unk581(11, 0xb000, 20);
    Field_unk105_3(11, 0x101, 40);
    Field_unk582(11);
    request_b = 0x900c;
    Field_unk583(12, 0xd000);
    Field_unk584(12, 4);
    Field_unk585(request_b);
    Field_unk586(13, 0xb000);
    Field_unk587(13, 1);
    Field_unk588(13);
    Field_unk106_3(9, 0x100, 20);
    Field_unk589(9, 0x3000);
    Field_unk590(9, 1);
    Field_unk591(9);
    Field_unk592(12, 3);
    Field_unk593(request_b);
    Field_unk594(8, 2);
    Field_unk595(8);
    Field_unk596(12, 0xd000);
    Field_unk597(12, 3);
    Field_unk598(request_b);
    Field_unk599(11, 2);
    Field_unk600(11, 0xb000);
    Field_unk601(11);
    Field_unk602(12, 0);
    Field_unk603(request_b);
    Field_unk604(8, 0x3000, 0);
    Field_unk605(9, 0, 0);
    Field_unk606(11, 0xd000, 0);
    Field_unk607(13, 0xd000, 0);
    Field_unk107_3(0, 0x10000, 0x8000);
    Field_unk108_3(0, 0x1e6, 0x260);
    Field_unk109_3(0, 0x8000, 0);
    record = Pointer1(Field_unk61_4, 0);
    if (record != 0) {
        Field_unk608(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk110_3(1, 0x10000, 0x8000);
    Field_unk111_2(1, 0x1e6, 0x270);
    Field_unk112_2(1, 0x8000, 0);
    record = Pointer1(Field_unk62_4, 1);
    if (record != 0) {
        Field_unk609(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk113_2(2, 0x10000, 0x8000);
    Field_unk114_2(2, 0x1e6, 0x280);
    Field_unk115_2(2, 0x8000, 0);
    record = Pointer1(Field_unk63_4, 2);
    if (record != 0) {
        Field_unk610(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk116_2(3, 0x10000, 0x8000);
    Field_unk117_2(3, 0x1e6, 0x290);
    Field_unk118_2(3, 0x8000, 20);
    Field_unk119_2(12, 0x108, 40);
    Field_unk611(request_b);
    Field_unk612(9, 1);
    Field_unk43_2(0x1009);
    Field_unk613(8, 3);
    Field_unk614(8, 0x5000);
    Field_unk615(8);
    Field_unk616(8, 0x3000);
    Field_unk46(8, 0);
    if (Field_unk47(0, 0) == 1) {
        Field_unk617(8, 2);
        Field_unk618(8);
        Field_unk619(12, 3);
        Field_unk620(request_b);
        Field_unk621(9, 1);
        Field_unk120_2(0x9009, 0, 40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 3;
        Field_unk622(8, 3);
        Field_unk623(8, 0, 40);
    }
    Field_unk624(13, 1);
    Field_unk625(13);
    Field_unk626(8, 1);
    value = 176;
    Field_unk627(8, 0x5000);
    Field_unk628(8);
    Field_unk629(13, 1);
    Field_unk630(13, (value << 8));
    Field_unk631(13, 0, 20);
    Field_unk632(8, 3);
    Field_unk121_2(8, 0xcccc, 0x6666);
    request_c = 0x4008;
    Field_unk122_2(8, 0x1d8, 0x278);
    Field_unk633(request_c);
    Field_unk123_2(13, 0x103, 40);
    Field_unk634(13, 2);
    Field_unk635(13);
    Field_unk636(8, 4);
    Field_unk637(request_c, 0, 40);
    Field_unk638(11, 1);
    Field_unk639(11, (value << 8));
    Field_unk44_2(0x100b);
    Field_unk124_2(10, 0x102, 20);
    Field_unk125_2(10, 0x26666, 0x13333);
    Field_unk640(10, 2, 0);
    Field_unk126_2(10, 0x1ce, 0x2a2);
    Field_unk641(10, (value << 8));
    Field_unk642(10, 2);
    Field_unk643(10);
    Field_unk644(9, 0x5000);
    Field_unk645(9, 4);
    Field_unk646(9);
    Field_unk647(8, 3);
    Field_unk648(request_c);
    Field_unk127_2(13, 0x102, 40);
    Field_unk649(13, 0, 40);
    Field_unk650(9, 0x3000);
    Field_unk651(9, 2);
    Field_unk45_2(0x1009);
    Field_unk652(12, 0);
    Field_unk128_2(8, 0x8000, 0);
    Field_unk653(9, 0x5000, 0);
    Field_unk654(11, (value << 8), 0);
    Field_unk655(13, (value << 8), 0);
    Field_unk656(10, (value << 8), 20);
    Field_unk657(12, 1);
    Field_unk129_2(0x100c, 0, 20);
    Field_unk130_2(8, 0x101, 40);
    Field_unk658(8, 0xd000);
    Field_unk48(0x1008, 0);
    if (Field_unk49(0, 0) == 0) {
        Field_unk659(8, 3);
        Field_unk46_2(0x1008);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Field_unk47_2(0x1008);
    }
    Field_unk660(0, 3);
    Field_unk661(8, 3);
    Field_unk48_2(0x1008);
    Field_unk12_4(8, 0x8000);
    Field_unk49_2(0x4008);
    Field_unk662(2, 0, 0);
    Field_unk663(12, 3);
    Field_unk664(11, 3);
    Field_unk665(9, 3);
    Field_unk666(10, 2);
    Field_unk667(13, 2);
    Field_unk668(20);
    action = (s32)gOv19;
    Field_unk669(10, action);
    Field_unk670(4);
    Field_unk671(11, action);
    Field_unk672(4);
    Field_unk673(12, action);
    Field_unk674(4);
    Field_unk675(9, action);
    Field_unk676(3, 2);
    record = Pointer1(Field_unk66_4, 2);
    if (record != 0) {
        Field_unk677(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_unk678(3);
    Field_unk679(3, 0, 0);
    Field_unk680(2, 2);
    record = Pointer1(Field_unk64_4, 1);
    if (record != 0) {
        Field_unk681(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_unk682(2);
    Field_unk683(2, 0, 0);
    Field_unk684(1, 2);
    record = Pointer1(Field_unk65_4, 0);
    if (record != 0) {
        Field_unk685(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_unk686(1);
    Field_unk687(1, 0, 0);
    Field_unk688(13, action);
    Field_unk131_2(8, 0x1c8, 0x288);
    Field_unk689(8, 0);
    Field_unk690(232);
    Field_unk50_2(0x925);
    Field_unk691();
}

/*
 * A four-way gated cascade. Each gate either runs its own setter sequence
 * and returns or falls through to the next, and all four share one exit.
 * The 336-byte owner at 0x0200351c includes its eight trailing pool words.
 */
void State_RunFlagGatedSetupCascade(void)
{
    if (Field_unk109((s32)&Value_0000093e) != 0) {
        Field_unk692(8, 0, 0);
        Field_unk693(9, 0, 0);
        Field_unk694(10, 0, 0);
        Field_unk695(11, 0, 0);
        Field_unk696(12, 0, 0);
        Field_unk697(14, 0, 0);
        return;
    }

    if (Field_unk110(0x8a0) != 0) {
        Field_unk698(8, 0x98, 0x1bc, 0x3000);
        Field_unk699(8, 0x0200e958);
        Field_unk700(10, 0xb8, 0x1e0, 0xb000);
        Field_unk701(12, 0xaa, 0x1e8, 0xb000);
        Field_unk702(13, 0x88, 0x1e8, 0xd000);
        Field_unk703(15, 0x78, 0x1e0, 0xd000);
        Field_unk704(14, 0xb8, 0x20e, 0xb000);
        Field_unk705(11, 0x88, 0x248, 0x8000);
        Field_unk706(11, 0x0200e840);
        return;
    }

    {
        s32 t = Field_unk111((s32)&Value_00000928);
        if (t != 0) {
            Field_unk707(t);
            return;
        }
    }

    if (Field_unk112((s32)&Value_00000925) != 0) {
        Field_unk697(18, 0, 0);
        return;
    }

    if (Field_unk113((s32)&Value_00000911) != 0 &&
        Field_unk114((s32)&Value_00000922) != 0) {
        Field_unk708(14, 0, 0);
        Field_unk709(12, 0, 0);
    }
}

void RunSceneSelectionChain(void)
{
    Field_unk710(1);
    Field_unk711();
    if (Field_unk115(2366) != 0) {
        Field_unk712(4, 4, 0);
        Field_unk713(8, 412, 222, 12288);
        Field_unk714(9, 458, 161, 32768);
    } else {
        if (Field_unk116(2208) != 0) {
            Field_unk132_2(8, 30932992, 9961472);
            Field_unk715(9, 5);
            Field_unk716(4, 4, 0);
        } else {
            if (Field_unk117(2347) != 0) {
                Field_unk717(16, 0, 0);
                Field_unk718(4, 4, 0);
                Field_unk719(3);
            } else {
                if (Field_unk118(2346) != 0) {
                    Field_unk720(16, 0, 0);
                    Field_unk721(4, 3, 0);
                    Field_unk722(2);
                } else {
                    if (Field_unk119(2345) != 0) {
                        Field_unk723(16, 0, 0);
                        Field_unk724(4, 2, 0);
                        Field_unk725(1);
                    } else {
                        if (Field_unk120(2344) != 0) {
                            Field_unk726(16, 0, 0);
                            Field_unk727(10, 0, 0);
                            Field_unk728(0);
                        } else {
                            Field_unk729(9, 5);
                            if (Field_unk121(2341) != 0 && Field_unk122(2342) == 0) {
                                Field_unk730();
                            }
                        }
                    }
                }
            }
        }
    }
}

void Actor_SetFlagBit3ForActors28To35(void)
{
    u32 i;
    u32 bit;
    u32 zero;

    i = 28;
    bit = 8;
    zero = 0;
    for (; i <= 35; i++) {
        u8 *obj = Field_unk55_4(i);
        u32 v = obj[0x59];
        obj[0x59] = (u8)((v | bit) | zero);
    }
}

void Scene_RunFourActorCoordinatePresentation(void)
{
    u8 *record;
    s32 mode;

    Field_unk731();
    Field_unk732(25, 0, 0);
    Field_unk733(24, 1, 0);
    Field_unk734(0x1b80000, -1, 0xa80000, 0x1000001);
    Field_unk735(27, 0x1b8, 164, 0x5000);
    Field_unk736(8, 0x1ac, 190, 0xd000);
    Field_unk737(9, 0x1c4, 190, 0xb000);
    Field_unk738(9, 1);
    mode = 128;
    Field_unk739(0, 0x1b8, 134, 0x8000);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = (mode << 1);
    Field_unk740();
    Field_unk133_2(0, 0xcccc, 0x6666);
    Field_unk134_2(0, 0x198, 134);
    Field_unk135_2(0, 0x198, 148);
    Field_unk136_2(0, 0x1a8, 148);
    Field_unk137_2(0, 0x4000, 20);
    Field_unk741(27, 1);
    Field_unk51_2(0x1e27);
    Field_unk742(27);
    Field_unk743(8, 1);
    Field_unk744(8);
    Field_unk745(27, 3);
    Field_unk746(27);
    Field_unk747(27, 0xd000);
    record = Pointer1(Field_unk67_4, 0);
    if (record != 0) {
        Field_unk748(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk138_2(1, 0xcccc, 0x6666);
    Field_unk749(1, 0x1b8, 148);
    Field_unk139_2(1, 0x4000, 0);
    record = Pointer1(Field_unk68_4, 1);
    if (record != 0) {
        Field_unk750(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk140_2(2, 0xcccc, 0x6666);
    Field_unk141(2, 0x1c8, 148);
    Field_unk142(2, 0x4000, 0);
    record = Pointer1(Field_unk69_4, 2);
    if (record != 0) {
        Field_unk751(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk143(3, 0xcccc, 0x6666);
    Field_unk144(3, 0x1d8, 148);
    Field_unk145(3, 0x4000, 20);
    Field_unk752(0, 0, 60);
    Field_unk753(1, 0x4000, 20);
    Field_unk754(2, 1, 20);
    Field_unk755(27, 0x5000, 20);
    Field_unk756(27);
    Field_unk757(9, 1);
    Field_unk758(9, (mode << 1), 40);
    Field_unk759(9);
    Field_unk760(1, 3);
    Field_unk146(1, 0x103, 60);
    Field_unk761(27, 3);
    Field_unk762(27);
    Field_unk763(10, 1);
    Field_unk764(10, 3);
    Field_unk765(10);
    Field_unk766(8, 3);
    Field_unk767(9, 3);
    Field_unk768(11, 3);
    Field_unk769(12, 3);
    Field_unk770(13, 3);
    Field_unk771(0, 0, 40);
    Field_unk772(2, 1, 0);
    Field_unk773(1, 0x4000, 20);
    Field_unk774(27, 4);
    Field_unk775(27);
    Field_unk147(8, 0x102, 60);
    Field_unk776(8, 1);
    Field_unk777(8);
    Field_unk778(27, 3);
    Field_unk779(27);
    Field_unk780(8, 0, 0);
    Field_unk781(9, 0x8000, 40);
    Field_unk148(8, 0x102, 0);
    Field_unk149(8, 0x102, 40);
    Field_unk782(27, 1);
    Field_unk783(27, 3);
    Field_unk784(27, 0, 20);
    Field_unk785(8, 3);
    Field_unk786(9, 3);
    Field_unk787(40);
    Field_unk788(9, (mode << 1), 20);
    Field_unk789(9, 0xb000);
    Field_unk790(9);
    Field_unk791(27, 0x3000);
    Field_unk150(27, 0x101, 60);
    Field_unk792(27, 0, 60);
    Field_unk151(27, 0x106, 20);
    Field_unk793(27, 0xb000);
    Field_unk794(27, 3);
    Field_unk795(27);
    Field_unk796(3, 2, 80);
    Field_unk797(8, 0xd000);
    Field_unk798(8, 2);
    Field_unk799(8);
    Field_unk800(9, 3);
    Field_unk801(9, 2);
    Field_unk802(9);
    Field_unk803(27, 0x5000);
    Field_unk804(27, 3);
    Field_unk805(27, 1);
    Field_unk806(27);
    Field_unk152(27, 0xcccc, 0x6666);
    Field_unk153(27, 0x198, 158);
    Field_unk154(27, 0x198, 148);
    Field_unk807(27, 0, 20);
    Field_unk808(27, 1);
    Field_unk809(27);
    Field_unk810(1, 0x8000, 20);
    Field_unk811(2, 1, 0);
    Field_unk155(27, 0x198, 134);
    Field_unk812(27, 0x1b8, 134);
    Field_unk813(40);
    Field_unk814(9, 10, 0);
    Field_unk52_2(0x926);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_unk815();
    Field_unk816(15, 0, 1);
    Field_unk817(8, 1);
    Field_unk818(20);
    Field_unk156(8, 0xcccc, 0x6666);
    Field_unk157(8, 0x1d4, 0x266);
    Field_unk158(8, 0x1d8, 0x254);
    Field_unk159(8, 0x8000, 20);
    Field_unk819(8, 4, 20);
    rec7 = Field_unk13_4();
    Field_unk820(20);
    Field_unk821(214);
    Field_unk822(rec7, 0x200e738);
    Field_unk823(40);
    Field_unk824(8, 3);
    Field_unk825(20);
    Field_unk160(8, 0x1d2, 0x270);
    Field_unk50(8, 0x5000);
    Field_unk826(8, 2);
    Field_unk53_2(0x1e3b);
    Field_unk827(8, 0, 20);
    Field_unk828(9, 11, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk829();
    Field_unk830(15, 1, 1);
    Field_unk161(8, 0x5000, 40);
    Field_unk831(8, 2);
    Field_unk54_2(0x1e3d);
    Field_unk832(8, 0, 20);
    Field_unk833(9, 11, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk834();
    Field_unk835(24, 0, 0);
    Field_unk836(18, 0, 0);
    Field_unk837(0, 0, 0);
    Field_unk162(16, 0x960000, 0x24a0000);
    Field_unk14_4(0x9c0000, -1, 0x2180000, 0x1000001);
    Field_unk838(8, 0, 0);
    Field_unk163(16, 0xcccc, 0x6666);
    Field_unk164(16, 168, 0x242);
    Field_unk165(16, 168, 0x22a);
    Field_unk166(16, 0x8000, 20);
    Field_unk839(16, 2);
    Field_unk55_2(0x1e3c);
    Field_unk840(16, 0, 20);
    Field_unk841(9, 12, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk842();
    Field_unk843(0, 0, 0);
    Field_unk844();
    Field_unk167(18, 0x960000, 0x24a0000);
    Field_unk15_4(0x9c0000, -1, 0x2180000, 0x1000001);
    Field_unk845(8, 0, 0);
    Field_unk168(18, 0xcccc, 0x6666);
    Field_unk169(18, 168, 0x242);
    Field_unk170(18, 168, 0x22a);
    Field_unk171(18, 0x8000, 20);
    Field_unk846(18, 2);
    Field_unk56_2(0x1e3c);
    Field_unk847(18, 0, 20);
    Field_unk848(9, 12, 0);
}

/*
 * A flat setter cascade, one workspace-slot store, then a four-way gated
 * chain ending in an unconditional default arm. The store spells both its
 * offset and its stored value as 224 << 1 rather than folded constants.
 * The 340-byte owner at 0x02003f94 includes its trailing pool words.
 */
void Scene_RunFlagBranchedSetupCascade(void)
{
    extern u8 *gWork;

    Field_unk849();
    Field_unk850(9, 5);
    Field_unk851(24, 1, 0);
    Field_unk852(0, 0, 0);
    Field_unk853(17, 0, 0);
    Field_unk854(0);
    Field_unk855(8, 1, 20);
    Field_unk856(0x6666, 0xccc);
    Field_unk857(0x1b80000, -1, 0xb00000, 1);
    Field_unk858(20);
    Field_unk859(9, 7);
    Field_unk860(30);
    Field_unk861(0xbc);
    Field_unk862(30);
    Field_unk863(16);
    Field_unk864(0x50);
    Field_unk865(0);
    Field_unk866(0x3c);
    Field_unk867(9, 7);
    Field_unk868(30);
    Field_unk869(0xbc);
    Field_unk870(30);
    Field_unk871(16);
    Field_unk872(0x50);
    Field_unk873(0);
    Field_unk874(0x5a);
    Field_unk875(0xbc);
    Field_unk876(30);

    *(u32 *)(gWork + (224 << 1)) = (224 << 1) + 67;

    Field_unk877(9, 0, 0);

    if (Field_unk123((s32)&Value_0000092b) != 0) {
        Field_unk878(20);
    } else if (Field_unk124((s32)&Value_0000092a) != 0) {
        Field_unk879(18);
    } else if (Field_unk125((s32)&Value_00000929) != 0) {
        Field_unk880(17);
    } else if (Field_unk126((s32)&Value_00000928) != 0) {
        Field_unk881(16);
    } else {
        Field_unk882(13);
    }
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk883();
    Field_unk884(15, 1, 1);
    Field_unk885(8, 1);
    Field_unk886(10);
    Field_unk172(8, 0x3000, 20);
    Field_unk887(8, 2);
    Field_unk888((s32)gVal);
    Field_unk889(8, 0, 20);
    Field_unk890(9, 14, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk891();
    Field_unk16_4(-1, -1, -1, 0);
    Field_unk892(1);
    Field_unk893(15, 1, 1);
    Field_unk894(8, 1);
    Field_unk57_2(0x1e43);
    Field_unk895(8);
    Field_unk173(8, 0xd000, 40);
    Field_unk896(9, 15, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_200e8e4;

    Field_unk897();
    Field_unk898(24, 1, 0);
    Field_unk899(25, 1, 0);
    Field_unk900();
    Field_unk901(19, 11, 12);
    Field_unk902(10, 6);
    Field_unk51(12, 0x200e840);
    base5_200e8e4 = (s32)gOv15;
    Field_unk903(36, base5_200e8e4);
    Field_unk52(37, base5_200e8e4);
    Field_unk53(38, base5_200e8e4);
    Field_unk904(36, 3);
    Field_unk905(37, 3);
    Field_Apply18(38, 3);
    Field_unk906();
    Field_unk907();
}

/*
 * A flat setter sequence, no branches; the final call takes no arguments.
 * The 60-byte owner at 0x02004218 includes its one pool word, the address
 * taken as Value_00000928.
 */
void Scene_RunActors24And25Setup(void)
{
    extern u8 *gWork;

    Field_unk908();
    Field_unk909(24, 1, 0);
    Field_unk910(25, 0, 0);
    Field_unk911(19, 11, 12);
    Field_unk912();
    Field_unk913((s32)&Value_00000928);
    Field_unk914();
}

/*
 * Four "install or fallback" blocks, each gated by a flag check, then four
 * unconditional installs.  src is threaded through every block unmodified,
 * and blocks two and four store the 0xffff0000 sentinel into the installed
 * object's +24 field.  The owner includes the trailing pool words that hold
 * the flag addresses and the sentinel.  Callees are named by the address
 * their call site computes, not by a runtime address.
 */
void Scene_InstallFlaggedActors10To17(u8 *src)
{
    extern u8 *gWork;

    if (Field_unk127((s32)&Value_00000928) != 0) {
        u8 *obj = Field_unk47_4(0, 0);
        Field_unk915(obj, 0xcd << 17, 0xac << 16);
        Field_unk916(7, obj, src);
        Field_unk917(10, 0, 0);
    } else {
        Field_unk918(5, 10, src);
    }

    if (Field_unk128((s32)&Value_00000929) != 0) {
        u8 *obj = Field_unk48_4(1, 0);
        Field_unk919(obj, 0xeb << 17, 0xac << 16);
        *(u32 *)(Field_unk49_4(obj) + 24) = 0xffff0000;
        Field_unk920(7, obj, src);
        Field_unk921(11, 0, 0);
    } else {
        Field_unk922(6, 11, src);
    }

    if (Field_unk129((s32)&Value_0000092a) != 0) {
        u8 *obj = Field_unk50_4(2, 0);
        Field_unk923(obj, 0xcd << 17, 0xcc << 16);
        Field_unk924(7, obj, src);
        Field_unk925(12, 0, 0);
    } else {
        Field_unk926(5, 12, src);
    }

    if (Field_unk130((s32)&Value_0000092b) != 0) {
        u8 *obj = Field_unk51_4(3, 0);
        Field_unk927(obj, 0xeb << 17, 0xcc << 16);
        *(u32 *)(Field_unk52_4(obj) + 24) = 0xffff0000;
        Field_unk928(7, obj, src);
        Field_unk929(13, 0, 0);
    } else {
        Field_unk930(6, 13, src);
    }

    Field_unk931(5, 14, src);
    Field_unk932(6, 15, src);
    Field_unk933(5, 16, src);
    Field_unk934(6, 17, src);
}

/*
 * Both callees live inside this overlay and are declared without a prototype,
 * so each call site fixes its own arity.
 */

/*
 * Resource 3b1 unindexed helper at 0x02004880 (16 bytes, 69 calls).
 *
 * Derived span: no inventory row (item 28). `push {lr}` at 0x02004880,
 * epilogue `pop {r0} / bx r0` at 0x0200488c-0x0200488e returns void.
 * Adjoins 0x0200486c below and 0x02004890 above with no pad or pool on
 * either side.
 *
 * The second argument is masked to 16 bits (`lsls`/`lsrs` #16) before the
 * call, so it is genuinely `u16`, not a truncated `s32`.
 */

/*
 * Shared helper for resource_3b1.  The owner at 0x0200486c is 18 bytes; the
 * halfword before the next owner at 0x02004880 is alignment, not part of it.
 */
void Scene_RunStepThen10(s32 a)
{
    Field_unk935(a, 0);
    Field_unk936(10);
}

void Scene_CallPairWith10(s32 a, u16 b)
{
    Field_unk937(a, b, 10);
}

/*
 * Resource 3b1 unindexed helper at 0x02004890 (28 bytes, 55 calls).
 *
 * Derived span: no inventory row (item 28). `push {r5,r6,lr}` at
 * 0x02004890, epilogue `pop {r5,r6} / pop {r0} / bx r0` at
 * 0x020048a6-0x020048aa returns void. Adjoins 0x02004880 below and
 * 0x020048ac above with no pad or pool.
 *
 * `b` and `c` are shifted left 16 (16.16 fixed point) before the first
 * call but not masked, so they stay full `s32`. `Field_unk72_4` returns a
 * pointer whose halfword at offset 6 is then set to `d`.
 */
void OvObj_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d)
{
    Field_unk938(a, b << 16, c << 16, d);
    *(s16 *)((u8 *)Field_unk72_4(a) + 6) = d;
}

void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags)
{
    u32 selected;

    Field_SetMode(x, y, z, ~flags & 1);
    selected = flags & 0x1111;
    if ((flags & 0x10000000) != 0)
        Field_unk939();
    if ((flags & 0x01000000) != 0)
        Field_unk940();
    Field_unk110_2(selected);
}

void Scene_RunSceneStep(s32 step, u32 arg, u32 opt)
{
    extern const s32 gOv26[];
    extern u8 gWork[];

    u32 slot;

    switch (step) {
    case 0:
        Field_unk941(0, 0, 0);
        Field_unk174(1, 0x8000, 0);
        Value(2, 0, 0);
        Field_unk175(3, 0x8000, opt);
        break;
    case 1:
        Field_unk942(0, arg, 0);
        Field_unk943(1, arg, 0);
        Field_unk944(2, arg, 0);
        Field_unk175(3, arg, opt);
        break;
    case 2:
        Field_unk945(0, 3);
        Field_unk946(1, 3);
        Field_unk947(2, 3);
        Field_unk948(3, 3);
        if (arg != 0) {
            Field_unk949(3);
        }
        if (opt == 0) {
            break;
        }
        Field_unk950(opt);
        break;
    case 3:
        Field_unk17_4(0, 0x102);
        Field_unk18_4(1, 0x102);
        Field_unk19_4(2, 0x102);
        Field_unk20_4(3, 0x102);
        Field_unk950(opt);
        break;
    case 4:
        for (slot = 0; slot < arg; slot++) {
            Field_unk951(slot + 10, 0, 0);
        }
        break;
    case 5:
        {
            u8 *rec;

            rec = Field_unk73_4(arg);
            SetPose(rec, 0x5000);
        }
        Field_unk952(arg, 5);
        Field_unk953(arg, opt);
        break;
    case 6:
        {
            u8 *rec;

            rec = Field_unk74_4(arg);
            SetPose(rec, 0x5000);
            *(s32 *)(rec + 24) = -0x10000;
        }
        Field_unk952(arg, 5);
        Field_unk953(arg, opt);
        break;
    case 7:
        {
            u8 *rec;

            rec = Value4(arg);
            SetPose(rec, 0x5000);
        }
        Field_unk954(arg);
        if (opt == 0) {
            Field_unk955(arg, 0);
        }
        break;
    case 8:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x202;
        Field_unk956();
        if (arg != 0) {
            Value5();
        }
        Field_unk957(0);
        break;
    case 9:
        Field_unk958();
        Field_unk959();
        if (arg == 0) {
            break;
        }
        Field_unk960(arg);
        break;
    case 10:
        Field_unk961(24, 1, 0);
        Field_unk962(25, 0, 0);
        Field_unk963(0);
        Field_unk964(0, 0x1b0, 168, 0x4000);
        Field_unk965(1, 0x1c0, 168, 0x4000);
        Field_unk966(2, 0x1a8, 152, 0x4000);
        Field_unk967(3, 0x1ca, 152, 0x4000);
        break;
    case 11:
        if (arg != 0) {
            u8 *rec;

            Field_unk968(13, 1);
            rec = Field_unk75_4(13);
            SetPose(rec, 0x3000);
            rec = Field_unk76_4(13);
            *(s32 *)(rec + 24) = 0x10000;
        }
        {
            u8 *rec;

            Field_unk969(14, 1);
            rec = Field_unk77_4(14);
            SetPose(rec, 0x5000);
            Field_unk970(15, 1);
            rec = Field_unk78_4(15);
            SetPose(rec, 0x3000);
            rec = Field_unk79_4(15);
            *(s32 *)(rec + 24) = 0x10000;
            Field_unk971(16, 1);
            rec = Field_unk80_4(16);
            SetPose(rec, 0x5000);
            Value6(17, 1);
            rec = Field_unk81_4(17);
            SetPose(rec, 0x3000);
            rec = Field_unk82_4(17);
            *(s32 *)(rec + 24) = 0x10000;
        }
        Field_unk176(28, 0x19a0000, 0xae0000);
        Field_unk177(29, 0x1d60000, 0xae0000);
        Field_unk178(30, 0x19a0000, 0xce0000);
        Field_unk179(31, 0x1d60000, 0xce0000);
        Field_unk180(32, 0x19a0000, 0x11e0000);
        Field_unk181(33, 0x1d60000, 0x11e0000);
        Field_unk182(34, 0x19a0000, 0x13c0000);
        Field_unk183(35, 0x1d60000, 0x13c0000);
        Field_unk972(1);
        if (arg != 0) {
            Field_unk184(13, 0xb000, 0);
        }
        Field_unk185(14, 0xd000, 0);
        Field_unk186(15, 0xb000, 0);
        Field_unk187(16, 0xd000, 0);
        Field_unk973(17, 0xb000);
        break;
    case 12:
        {
            u8 *rec;

            rec = Field_unk83_4(arg);
            Field_unk974(arg, 1);
            if (opt != 0) {
                SetPose(rec, 0x3000);
            } else {
                SetPose(rec, 0x5000);
            }
            *(s32 *)(rec + 24) = 0x10000;
        }
        break;
    case 13:
        Field_unk975(9, 0, 0);
        Field_unk976(12, 0, 0);
        Field_unk977(11, 0, 0);
        Field_unk978(13, 0, 0);
        Field_unk979(10, 0, 0);
        break;
    case 14:
        Field_unk980(14, 0, 0);
        Field_unk979(13, 0, 0);
        break;
    case 15:
        Field_unk981(24, 1, 0);
        Field_unk982(9, 0, 0);
        Field_unk983(10, 0, 0);
        Field_unk984(8, 0x1bc, 0x266, 0xd000);
        Field_unk985(0, 0, 0);
        if (arg != 0) {
            Field_unk986();
        }
        Field_unk987(0x1c00000, 0x200000, 0x2700000, 0x1000001);
        if (opt == 0) {
            break;
        }
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x202;
        Field_unk988();
        Field_unk989();
        Value7(20);
        break;
    case 16:
        Field_unk990(8, 0, 0);
        Field_unk991(9, 0, 0);
        Field_unk188(27, 0x1b60000, 0x980000);
        break;
    case 17:
        slot = 0;
        do {
            Field_unk992(slot + 28, 0, 0);
            slot++;
        } while (slot <= 7);
        break;
    case 18:
        Field_unk993(12, 152, 0x214, 0xb000);
        Field_unk994(8, 134, 0x1ea, 0x3000);
        Field_unk995(9, 166, 0x1ea, 0x5000);
        Field_unk996(10, 182, 0x1f8, 0x5000);
        Field_unk997(11, 118, 0x1f8, 0x3000);
        Scene_RunSceneStep(14, 0, 0);
        break;
    case 19:
        Field_unk998(8, 0x1a0, 0x148, 0);
        Field_unk999(9, 0x1c0, 0x160, 0xd000);
        Field_unk1000(10, 0x1c6, 248, 0x3000);
        Field_unk1001(arg, 0x198, 0x122, 0);
        Field_unk1002(opt, 0x198, 0x156, 0);
        Field_unk1003(13, 0x1a4, 0x164, 0xd000);
        Field_unk1004(14, 0x198, 0x130, 0);
        Field_unk1005(15, 0x1a2, 0x17a, 0xd000);
        Field_unk1006(16, 0x1b8, 0x106, 0x3000);
        Field_unk1007(17, 0x1c0, 0x17a, 0xd000);
        break;
    case 20:
        for (slot = arg; slot <= opt; slot++) {
            Field_unk1008(slot);
        }
        break;
    case 21:
        Field_unk1009(20, 0x92c, 0x93d);
        Field_unk1010(20, 0x917, 0x91f);
        Field_unk1011(20, 0x990, 0x998);
        Field_unk1012(0x300);
        Field_unk1013(0x301);
        Field_unk1014(0x302);
        break;
    case 22:
        Field_unk1015(1);
        Field_unk1016(23, 0, 0);
        Field_unk1017(12, gOv26);
        break;
    case 23:
        Field_unk1018(1);
        Field_unk1019(2);
        Field_unk1020(3);
        break;
    case 24:
        Field_unk21_4(-1, -1, -1, 0);
        Field_unk1021(1);
        if (arg != 0) {
            *(u8 *)(Field_unk84_4() + 0x55) = 0;
        }
        break;
    case 25:
        Field_unk1022(gOv22);
        Field_unk1023(1);
        if (arg == 1) {
            Field_unk1024(gOv23);
            Field_unk1025(1);
        } else if (arg == 2) {
            Field_unk1024(gOv24);
            Field_unk1025(1);
        } else if (arg == 3) {
            Field_unk1026(gOv25);
            Field_unk1027(1);
        }
        break;
    }
}

/*
 * A flat setter sequence, no branches. The 52-byte owner at 0x02005004
 * includes its three pool words, which are plain numeric arguments and not
 * addresses of Value_ globals.
 */
void State_ApplyActor8FourFlags(void)
{
    Field_unk1028(8, 0x92c);
    Field_unk1029(8, 0x935);
    Field_unk1030(8, 0x917);
    Field_unk1031(8, 0x990);
}

/*
 * Scan slots 0 through 8 inclusive.  On the first a1 that Field_unk131
 * accepts, call Field_unk1032 and stop.  a0 and a1 advance together.
 */
void Actor_RunFirstMatchingSlot(s32 a0, s32 a1)
{
    unsigned int i = 0;

    do {
        if (Field_unk131(a1)!= 0) {
            Field_unk1032(a0, 0, 0);
            break;
        }
        i++;
        a0++;
        a1++;
    } while (i <= 8);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Field_unk54(0, 0);
    Field_unk1033();
    Field_unk1034(24, 1, 0);
    Field_unk1035(25, 2, 0);
    Field_unk1036();
    Field_unk189(19, rec8, 12);
    Field_unk1037(10, 6);
    base5_200e840 = (s32)gOv26;
    Field_unk1038(rec8, base5_200e840);
    Field_unk1039(11);
    Field_unk55(12, base5_200e840);
    base5_200e8e4 = (s32)gOv15;
    Field_unk1040(36, base5_200e8e4);
    Field_unk1041(37, base5_200e8e4);
    Field_unk1042();
    Field_unk1043();
}

/* Sets up three actor slots (2, 1, 3) with position/pose data pulled from a
 * per-slot lookup record (fields at +8 and +16), then drives a chain of
 * actor animation, camera, and text/dialog calls for the scene. */
void Scene_RunPositionTransferPresentation(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    /* Per-slot lookup record; fields at +8 and +16 feed the setup call. */
    s32 record;
    s32 base5_1e46;
    s32 base5_200e7c8;

    Field_unk1044(0x1b80000, -1, 0xb00000, 0x1000001);
    Motion_SetHPosTerrain_1(0, 0x1b80000, 0x860000);
    Battle_WaitMode0_1(); /* main:0808a360 */
    Motion_SetSpeed_1(0, 0x19999, 0xcccc);
    Object_SetModeById_1(0, 5);
    Motion_SetPositionAndCommit_1(0, 0x198, 134);
    Motion_SetPositionAndCommit_2(0, 0x198, 152);
    Motion_SetPositionAndCommit_3(0, 0x1b0, 166);
    Object_SetModeById_2(0, 1);
    Motion_ArmCb_1(0, 0x4000, 0);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Motion_SetHPosTerrain_2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_SetHPosTerrain_3(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(1);
    if (record != 0) {
        Motion_SetHPosTerrain_4(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk1045(1); /* main:080000c0 */
    Motion_SetSpeed_2(2, 0x19999, 0xcccc);
    Motion_ResetPosMode2_1(2, 0x1a8, 152);
    Motion_SetSpeed_3(1, 0x19999, 0xcccc);
    Motion_ResetPosMode2_2(1, 0x1c0, 168);
    Motion_SetSpeed_4(3, 0x20000, 0x10000);
    Motion_SetPosReset_1(3, 0x1ca, 152);
    Object_SetModeById_3(1, 1);
    Object_SetModeById_4(2, 1);
    Motion_ArmCb_2(2, 0x4000, 0);
    Motion_ArmCb_3(1, 0x4000, 0);
    Motion_ArmCb_4(3, 0x4000, 40);
    Field_unk1046(10); /* main:0808a1d8 */
    /* Text/dialog resource pointer, passed by base address and by base+1. */
    base5_1e46 = (s32)gVal2;
    Field_unk1047(base5_1e46, 1, 10); /* main:08015210 */
    Battle_WaitMode0_2(10);
    Field_unk1048(0, 0, 40);
    Field_unk190(1, 0x4000, 20);
    Motion_SetSpeedLim_1(0x39999, 0x7333); /* main:0808a208 */
    Field_unk22_4(0x1b80000, -1, 0x1400000, 0x10000014);
    Motion_SetVarCbObj_1(8, 2); /* main:0808a138 */
    Field_unk23_4(8, 0xd000);
    SceneWork_SetStepValue_1((base5_1e46 + 1)); /* main:0808a170 */
    Field_unk1049(8);
    Motion_ArmCb_5(8, 0, 20);
    Field_unk24_4(0x1b80000, -1, 0x860000, 0x10000000);
    /* Second text/dialog resource pointer, shared across three calls. */
    base5_200e7c8 = (s32)gOv16;
    Motion_EnableActCb_1(1, base5_200e7c8);
    Motion_EnableActCb_2(2, base5_200e7c8);
    Motion_MarkActiveAndSetActionCallback_1(3, base5_200e7c8); /* main:0808a0b0 */
    Battle_WaitMode0_3(40);
    GameFlag_Set_1(0x301);
    Field_unk1050(23, 0, 0);
    GameFlag_Clear_1(0x12f);
}

void Scene_RunFormationAndEffectPresentation(void)
{
    s32 action;

    Field_unk1051(0, 0x1bc, 0x12c, 0);
    Field_unk1052(1, 0x1ca, 0x136, 0);
    Field_unk1053(2, 0x1bc, 0x14a, 0);
    Field_unk1054(3, 0x1b0, 0x136, 0);
    Field_unk1055(27, 0x1b8, 134, 0x8000);
    Field_unk1056(10, 0x1c6, 248, 0x3000);
    Field_unk1057(10, 6);
    Field_unk1058(0x1b80000, -1, 0x1340000, 0x1000001);
    Field_unk1059();
    Field_unk1060();
    Field_unk1061(20);
    Field_unk191(0, 0x2000, 0);
    Field_unk192(1, 0xa000, 0);
    Field_unk193(2, 0xa000, 0);
    Field_unk194(3, 0x2000, 40);
    Field_unk1062(2, 1, 20);
    Field_unk58_2(0x1e6e);
    Field_unk1063(27);
    Field_unk1064(1, 0xc000, 0);
    Field_unk25_4(0x26666, 0x4ccc);
    Field_unk26_4(0x1b80000, -1, 0xb00000, 1);
    Field_unk195(27, 0x19999, 0xcccc);
    Field_unk196(27, 0x198, 134);
    Field_unk197(27, 0x198, 152);
    Field_unk198(27, 0x1a8, 164);
    Field_unk27_4(0x19999, 0x3333);
    Field_unk28_4(0x1b80000, -1, 0x12c0000, 1);
    Field_unk199(27, 0x1a8, 222);
    Field_unk200(27, 0x1a8, 0x106);
    Field_unk1065(27, 0x3000, 20);
    Field_unk1066(27, 1);
    Field_unk1067(27);
    Field_unk1068(2, 1, 20);
    Field_unk1069(27, 3);
    Field_unk1070(27, 1);
    Field_unk1071(27);
    Field_unk1072(3, 2, 60);
    Field_unk1073(1, 0xe000, 60);
    Field_unk1074(27, 0, 40);
    Field_unk1075(27, 1);
    Field_unk1076(27, 2);
    Field_unk201(27, 0x1b0, 0x10c);
    Field_unk202(27, 0x1c4, 0x10c);
    Field_unk1077(27, 1);
    Field_unk1078(27, 0xd000);
    Field_unk1079(27, 2);
    Field_unk1080(27, 0, 20);
    Field_unk1081(1, 0xc000, 20);
    Field_unk1082(27, 4);
    Field_unk1083(40);
    Field_unk1084(27, 0, 80);
    Field_unk1085(27, 1);
    Field_unk1086(20);
    Field_unk1087(27, 3);
    Field_unk1088(10);
    Field_unk1089(27, 0x5000);
    Field_unk56(27, 0);
    if (Field_unk57(0, 0) == 0) {
        Field_unk1090(27, 3);
        Field_unk1091(27);
    } else {
        Field_unk1092(27, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Field_unk1093(27);
        Field_unk1094(3, 2, 40);
        Field_unk1095(27, 1);
        Field_unk1096(27, 3);
        Field_unk1097(27);
    }
    Field_unk1098(2, 1, 20);
    action = (s32)gOv20;
    Field_unk1099(1, action);
    Field_unk1100(2, action);
    Field_unk1101(3, action);
    Field_unk29_4(0x9999, 0x1333);
    Field_unk30_4(0x1b80000, -1, 0xb00000, 1);
    Field_unk203(0, 0x10000, 0x8000);
    Field_unk204(0, 0x1a8, 0x110);
    Field_unk205(0, 0x1a8, 164);
    Field_unk1102(60);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    Field_unk1103(9, 0, 0);
    Field_unk59_2(0x301);
    Field_unk60_2(0x927);
    Field_unk1104(4);
}

void Scene_RunActors24And25SetupWithValue929(void)
{
    extern u8 *gWork;

    s32 handle = Field_unk132(0, 0);

    Field_unk1105();
    Field_unk1106(24, 1, 0);
    Field_unk1107(25, 0, 0);
    Field_unk1108(0);
    Field_unk1109(19, handle, 12);
    Field_unk1110(11, 0, 0);
    Field_unk1111();
    Field_unk1112((s32)&Value_00000929);
    Field_unk1113();
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec2;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Field_unk58(0, 0);
    rec2 = Field_unk59(1, 0);
    Field_unk1114();
    Field_unk1115(24, 1, 0);
    Field_unk1116(25, 3, 0);
    Field_unk1117();
    Field_unk206(19, rec8, rec2);
    Field_unk1118(10, 6);
    base5_200e840 = (s32)gOv26;
    Field_unk1119(rec8, base5_200e840);
    Field_unk1120(11);
    Field_unk1121(rec2, base5_200e840);
    Field_unk1122(12);
    base5_200e8e4 = (s32)gOv15;
    Field_unk1123(36, base5_200e8e4);
    Field_unk60(37, base5_200e8e4);
    Field_unk1124(36, 3);
    Field_Apply19(37, 3);
    Field_unk1125();
    Field_unk1126();
}

/*
 * A flat setter sequence, no branches. The 108-byte owner at 0x02005780
 * includes its one pool word, the address taken as Value_0000092a.
 */
void Scene_RunActors24And25SetupWithValue92a(void)
{
    extern u8 *gWork;

    s32 handle = Field_unk133(0, 0);
    s32 other = Field_unk134(1, 0);

    Field_unk1127();
    Field_unk1128(24, 1, 0);
    Field_unk1129(25, 0, 0);
    Field_unk1130(0);
    Field_unk1131(19, handle, other);
    Field_unk1132(11, 0, 0);
    Field_unk1133(12, 0, 0);
    Field_unk1134();
    Field_unk1135((s32)&Value_0000092a);
    Field_unk1136();
}

void Scene_RunExtendedFormationPresentation(void)
{
    s32 slot_b;
    s32 slot_c;
    s32 slot_a;
    u8 *record;
    s32 action;

    slot_a = Field_unk61(0, 0);
    slot_b = Field_unk135(1, 0);
    slot_c = Field_unk62(2, 0);
    Field_unk1137();
    Field_unk1138(10, 0, 0);
    Field_unk1139(17, 0, 0);
    Field_unk207(8, 0x1d80000, 0x980000);
    Field_unk1140(9, 5);
    Field_unk208(27, 0x1b80000, 0x860000);
    Field_unk1141(27, 15);
    record = Field_unk70_4(27);
    Field_unk1142(record, 0);
    Field_unk1143(16);
    Field_unk1144(0x1b60000, -1, 0xae0000, 0x1000001);
    Field_unk1145(8, 1, 20);
    Field_unk61_2(19);
    Field_unk1146(181);
    Field_unk209(0x20000, 0x20000, 0x10000);
    Field_unk1147(10);
    Field_unk210(-1, -1, 0xe666);
    Field_unk1148(80);
    Field_unk1149(181);
    Field_unk211(0x20000, 0x20000, 0x10000);
    Field_unk1150(10);
    Field_unk212(-1, -1, 0xe666);
    Field_unk1151(63);
    Field_unk62_2(0x11a);
    Field_unk31_4(3, 0x102);
    Field_unk1152(40);
    Field_unk1153(3, 0x6000);
    Field_unk63_2(0x1ec1);
    Field_unk1154(3, 0, 40);
    Field_unk1155(27);
    Field_unk213(0, 0x2000, 0);
    Field_unk214(1, 0xa000, 0);
    Field_unk1156(2, 0, 0);
    Field_unk215(3, 0xe000, 40);
    Field_unk1157(0, 0x6000, 0);
    Field_unk216(1, 0xe000, 0);
    Field_unk1158(2, 0x6000, 0);
    Field_unk217(3, 0x8000, 40);
    Field_unk218(2, 0xe000, 0);
    Field_unk219(2, 0x100, 60);
    Field_unk1159(2, 0x2000);
    Field_unk1160(2, 1);
    Field_unk1161(2);
    Field_unk1162(0, 1);
    Field_unk1163(1, 1);
    Field_unk1164(3, 1);
    Field_unk1165(10);
    Field_unk220(0, 0xc000, 0);
    Field_unk221(1, 0xc000, 0);
    Field_unk222(2, 0xe000, 0);
    Field_unk223(3, 0xa000, 20);
    Field_unk1166(27, 0);
    record = Field_unk71_4(27);
    Field_unk1167(record, 1);
    Field_unk224(27, 0x10000, 0x8000);
    Field_unk225(27, 0x1ae, 134);
    Field_unk1168(27, 0x3000);
    Field_unk1169(27, 2);
    Field_unk1170(27);
    Field_unk1171(slot_a, 1);
    Field_unk1172(slot_b, 1);
    Field_unk1173(slot_c, 1);
    Field_unk1174(13, 1);
    Field_unk32_4(slot_a, 0x102);
    Field_unk33_4(slot_b, 0x102);
    Field_unk34_4(slot_c, 0x102);
    Field_unk35_4(13, 0x102);
    Field_unk1175(40);
    Field_unk1176(12, slot_a, 0);
    Field_unk1177(12, slot_b, 1);
    Field_unk1178(12, slot_c, 0);
    Field_unk1179(11, 1, 0);
    Field_unk226(slot_a, 0xd000, 0);
    Field_unk227(slot_b, 0xb000, 0);
    Field_unk228(slot_c, 0xd000, 0);
    Field_unk1180(0, 0, 0);
    Field_unk229(1, 0x8000, 0);
    Field_unk1181(2, 0, 0);
    Field_unk230(3, 0x8000, 40);
    Field_unk1182(27, 2);
    Field_unk1183(27, 0);
    Field_unk231(0, 0xc000, 0);
    Field_unk232(1, 0xc000, 0);
    Field_unk233(2, 0xc000, 0);
    Field_unk234(3, 0xc000, 20);
    Field_unk235(27, 0x1b8, 134);
    Field_unk1184(27, 0, 0);
    Field_unk1185(1, 0x8000);
    Field_unk1186(1, 1);
    Field_unk1187(1);
    Field_unk1188(2, 0, 0);
    Field_unk1189(3, 0x8000);
    Field_unk1190(0, 3);
    Field_unk1191(1, 3);
    Field_unk1192(2, 3);
    Field_unk1193(3, 3);
    Field_unk236(1, 0x10000, 0x8000);
    Field_unk237(2, 0x10000, 0x8000);
    Field_unk238(3, 0x10000, 0x8000);
    action = (s32)gOv21;
    Field_unk1194(1, action);
    Field_unk1195(2, action);
    Field_unk1196(3, action);
    Field_unk64_2(0x302);
    *(s32 *)0x0200ff84 = 0;
    Field_unk63((s32)Field_unk1197, 0xc80);
    Field_unk1198(23, 0, 0);
    Field_unk1199(27);
    Field_unk65_2(0x12f);
    Field_unk66_2(0x927);
    Field_unk1200();
}

void RunActorsEightAndNineMapEvent(void)
{
    Field_unk1201();
    Field_unk1202(15, 1, 0);
    Field_unk1203(9, 468, 616, 32768);
    Field_unk1204(8, 1, 20);
    Field_unk1205(9, 2);
    Field_unk1206(20);
    SetDirection(8, 53248, 80);
    Field_unk1207(8, 0, 20);
    Field_unk1208(8, 3);
    Field_unk1209(20);
    Field_unk1210(9, 21, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec2;
    s32 rec4;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec2 = Field_unk64(0, 0);
    rec8 = Field_unk136(1, 0);
    rec7 = Field_unk137(2, 0);
    rec4 = Field_unk65(3, 0);
    Field_unk1211();
    Field_unk1212(10, 0, 0);
    Field_unk1213(8, 0x1d8, 144, 0x5000);
    Field_unk1214(27, 0x198, 142, 0x3000);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x201;
    Field_unk1215();
    Field_unk1216();
    Field_unk1217(40);
    Field_unk1218(27, 1);
    Field_unk67_2(0x1f78);
    Field_unk1219(27);
    Field_unk1220(rec2, 2);
    Field_unk1221(rec8, 2);
    Field_unk1222(rec7, 2);
    Field_unk1223(rec4, 2);
    Field_unk1224(20);
    Field_unk1225(rec2, 0, 0);
    Field_unk239(rec8, 0x8000, 0);
    Field_unk1226(rec7, 0, 0);
    Field_unk240(rec4, 0x8000, 40);
    Field_unk241(rec2, 0x10000, 0x8000);
    Field_unk242(rec8, 0x10000, 0x8000);
    Field_unk243(rec7, 0x10000, 0x8000);
    Field_unk244(rec4, 0x10000, 0x8000);
    Field_unk245(rec2, 0x1d6, 172);
    Field_unk246(rec8, 0x19a, 172);
    Field_unk247(rec7, 0x1d6, 204);
    Field_unk248(rec4, 0x19a, 204);
    Field_unk1227(rec2, 1);
    Field_unk1228(rec8, 1);
    Field_unk1229(rec7, 1);
    Field_unk249(rec8, 0xd000, 0);
    Field_unk250(rec2, 0xb000, 0);
    Field_unk251(rec4, 0xd000, 0);
    Field_unk252(rec7, 0xb000, 20);
    Field_unk1230(27, 1);
    Field_unk1231(27);
    Field_unk1232(rec2, 3);
    Field_unk1233(rec8, 3);
    Field_unk1234(rec7, 3);
    Field_unk1235(rec4, 3);
    Field_unk1236(27);
    Field_unk1237(rec2, 3);
    Field_unk1238(rec8, 3);
    Field_unk1239(rec7, 3);
    Field_unk1240(rec4, 3);
    Field_unk1241(27, 0, 0);
    Field_unk138(0, 0x8000);
    Field_unk1242(0, 3);
    Field_unk1243(27, 3);
    Field_unk253(27, 0x10000, 0x8000);
    Field_unk1244(27, 0x198, 132);
    Field_unk254(27, 0x1bc, 132);
    Field_unk1245(27, 0, 0);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x202;
    Field_unk1246();
    Field_unk1247();
    Field_unk36_4(0x92c, 0x935);
    Field_unk37_4(0x917, 0x990);
    Field_unk68_2(0x8a0);
    Field_unk1248(10);
}

/*
 * Runs two first-match linear scans over indices 0 to 8, each breaking on its
 * first hit and calling a per-element handler, then cross-pairs the miss
 * counts: the count from scanning `a` indexes into `b`, and the count from
 * scanning `b` indexes into `a`.  Each callee is named for its own call site,
 * because every call reaches its target through its own local veneer and two
 * of the sites share one veneer.
 */
void State_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b)
{
    s32 cnt_a = 0;
    s32 cnt_b = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        u8 *p = a + i;
        if (Field_unk139(p)!= 0) {
            Field_unk1249(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (Field_unk140(p)!= 0) {
            Field_unk1250(p);
            break;
        }
        cnt_b++;
    }

    Field_unk1251(b + cnt_a);
    Field_unk1252(a + cnt_b);
}
