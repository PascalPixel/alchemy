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

struct SceneActor *Field_unk_02006778(s32 actor_id);

u8 *Field_unk_02001da0(void);

u8 *Field_unk_02006fd2(s32 n);

u8 *Field_unk_020021cc(void);

u8 *Field_unk_020073fe(s32 n);

u8 *Field_unk_02002298(void);

u8 *Field_unk_020074ca(s32 n);

u8 *Field_unk_02001bd8(void);

u8 *Field_unk_02006e0a(s32 n);

u8 *Field_unk_0200962c();

u8 *Field_unk_0200966c();

u8 *Field_unk_0200abc0();

u8 *Field_unk_020096b6();

u8 *Field_unk_020096f6();

u8 *Field_unk_0200ac4a();

u8 *Field_unk_02001ca0(void);

u8 *Field_unk_02006ed2(s32 n);

u8 *Field_unk_02009ca8();

u8 *Field_unk_02002100(void);

u8 *Field_unk_02007332(s32 n);

u8 *Field_unk_02007772();

u8 *Field_unk_02008336_a();
u8 *Field_unk_02008358_a();

u8 *Field_unk_02008fbe();

u8 *Field_unk_02008ffa();

u8 *Field_unk_02009036();

u8 *Field_unk_0200940c();

u8 *Field_unk_0200943c();

u8 *Field_unk_020093dc_a();

u8 *Field_unk_02009eaa();

u8 *Field_unk_02009ede();

u8 *Field_unk_02009f14_a();

u8 *Field_unk_0200bd42();

u8 *Field_unk_0200beea_a();

void *Field_unk_0200ad8a();

u8 *Field_unk_0200af28();

u8 *Field_unk_0200af38();

void Value();
u8 *Value4();

u8 *Field_unk_0200b018();
u8 *Field_unk_0200b024();
u8 *Field_unk_0200b038();

u8 *Field_unk_0200b050();
u8 *Field_unk_0200b060();
u8 *Field_unk_0200b074();

u8 *Field_unk_0200b086();
u8 *Field_unk_0200b090();

void Value5();
void Value6();

u8 *Field_unk_0200b14e();

void Value_0200e5d0_b();
void Value7();

u8 *Field_unk_0200b52c();

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
 * emits it last.  Field_unk_02009ca8 names the in-overlay entry point, not its
 * veneer.
 */
static __inline__ void SetDirection(s32 actor, s32 direction, s32 duration)
{
    Field_unk_0200c2b8(actor, direction, duration);
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

s32 Field_unk_02002364(void);      /* site 0x1086 */

void Field_unk_02002224();         /* site 0x1092 */

void Field_unk_0200590c();         /* site 0x109e */

s32 Field_unk_02007520();          /* site 0x107e; story flag test */

void Field_unk_02007556();         /* site 0x108c */

void Field_unk_02007642();         /* site 0x1098 */

void Field_unk_020075f8();         /* site 0x10a6 */

u8 *Field_unk_02007596();          /* site 0x10ac; returns the actor record */

void Field_unk_020075d8();         /* site 0x10be */

void Field_unk_02007606();         /* site 0x10c4 */

void Field_unk_02007618();         /* site 0x10ce */

void Field_unk_020075a4();         /* site 0x10d2 */

s32 Field_unk_0200757c();          /* site 0x10da; story flag test */

s32 Field_unk_0200758c();          /* site 0x10ea; story flag test */

s32 Field_unk_0200759c();          /* site 0x10fa; story flag test */

void Field_unk_0200290e();         /* site 0x1108; the three selected variants share it */

void Field_unk_0200291a();         /* site 0x1114; the default variant has its own site */

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
    u32 v = (u32)(Field_unk_020064e6(o) << 6) >> 16;

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

    actor = Field_unk_02006778(9);
    if (record->state != 0)
        return;
    if (actor->mode == 1) {
        record->angle = 0xd000;
        record->active = 1;
        actor->mode = 0;
    } else if (actor->mode == 2) {
        if (record->active != 0)
            Field_unk_0200671c(record, 3);
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
    Field_unk_02006786(work, 0);
    rec->mode9 = 1;
    rec->mode15 = 1;
    work->f35 = (work->f35 & ~1) | 2;
    Field_unk_020068d6(work, 15);
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
        if (Field_unk_02006a6a(2208) != 0) {
            return gOv7;
        }
        if (Field_unk_02006a78(0x928) != 0 && Field_unk_02006a82(0x93e) == 0) {
            return gOv6;
        }
        return gOv5;
    case 4:
    case 23:
        if (Field_unk_02006a94(0x93e) != 0) {
            return gOv14;
        }
        return gOv11;
    case 5:
        if (Field_unk_02006aa8(2208) != 0) {
            return gOv9;
        }
        if (Field_unk_02006ab6(0x93e) != 0) {
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

    Field_unk_02006b3c();
    Field_unk_02006cc0();
    if (Field_Check(0x921) != 0) {
        Field_unk_02006c30(0x1dd4);
        Field_TestFlag924(10, 0);
    } else {
        if (Field_TestFlag922(0x922) != 0) {
            Field_unk_02006c4a(0x1d91);
            Field_unk_02006c5a(10, 0);
            if (Field_unk_02006b92(0, 0) == 0) {
                Field_unk_020025f6();
                goto L_020006ea;
            }
            Field_unk_02006c3c(10, 2);
            Field_unk_02006c84(10, 0);
            Field_unk_02006ca8(10, 0xd000, 0);
        } else {
            Field_unk_02006c88(0x1d31);
            Field_unk_02006ca0(10, 0);
        }
    }
    L_020006ea:;
    Field_unk_02006bbc();
}

void Dialogue_RunActor12Line(void)
{
    Field_unk_02006bd4();
    Field_unk_02006cba((s32)&Value_00001dd1);
    Field_unk_02006ce2(12, 0);
    Field_unk_02006bee();
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk_02006bf4();
    if (Field_unk_02006bd2(0x928) != 0) {
        Field_unk_02006ce4(0x1eb2);
        Field_unk_02004fae(8);
        Field_unk_02006d1e(8, 0xd000, 60);
        Field_unk_02006cb6(8, 4);
        Field_unk_02004fc8(8);
        Field_unk_02006cc4(8, 3);
    } else {
        if (Field_unk_02006c0c(0x925) != 0) {
            Field_unk_02006d1e_a(0x1e06);
            Field_unk_02006d36(8, 0);
        } else {
            if (Field_TestFlag921(0x921) != 0) {
                Field_unk_02006d38(0x1dcd);
                Field_unk_02006d50(8, 0);
                record = Field_unk_02006c3e(0x925);
                if (record != 0) {
                    goto L_020007cc;
                }
                record = Field_TestFlag924(0x924);
                if (record == 0) {
                    goto L_020007cc;
                }
                {
                    u16 *target = (u16 *)((*(u8 *volatile *)gWork + 0x172));
                    s32 shown = 1;

                    *target = shown;
                }
            } else {
                Field_unk_02006d6a(0x1d30);
                Field_unk_02006d82(8, 0);
            }
        }
    }
    L_020007cc:;
    Field_unk_02006c9e();
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk_02006cc4_a();
    if (Field_unk_02006ca2(0x925) != 0) {
        Field_unk_02006d86(8, 2);
        Field_unk_02006dbc(0x1e13);
        Field_unk_02005086(8);
        Field_unk_02006dac(8, 0, 10);
        Field_unk_02006ddc(8, 0);
        if (Field_unk_02006d14(0, 0) == 0) {
            Field_unk_02006cfe(40);
            Field_unk_020050b0(8);
            Field_unk_020050ce(8, 0x3000);
            Field_unk_02006e0e(8, 0);
            goto L_0200088e;
        }
        bump_step(2);
        Field_unk_02006e28(8, 0);
        Field_unk_02006e4c(8, 0x3000, 0);
    } else {
        Field_unk_02006e2c(0x1d4e);
        Field_unk_02006e44(8, 0);
    }
    L_0200088e:;
    Field_unk_02006d60();
}

void Dialogue_ShowLine1E19Or1D50(void)
{
    Field_unk_02006d74();
    if (Field_unk_02006d52((s32)&Value_00000925) != 0) {
        Field_unk_02006e64((s32)&Value_00001e19);
        Field_unk_02006e8c(10, 0);
    } else {
        Field_unk_02006e74((s32)&Value_00001d50);
        Field_unk_02006e8c_a(10, 0);
    }
    Field_unk_02006da8();
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

    if (Field_unk_02006d94(0x300) != 0) {
        u8 *obj = Field_unk_02001bd8();
        u8 *p;

        Field_unk_02006dca();
        Field_unk_02001a98(obj);
        Field_unk_02006eb6((s32)&Value_00001e9e);
        Field_unk_02005180(8);
        Field_unk_02006e6c(obj, 2);
        p = Field_unk_02006e0a(0);
        if (p != 0) {
            Field_unk_02006e4c_a(obj, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk_02006e7a(obj);
        Field_unk_02006e8c_b(obj, 0, 0);
        Field_unk_02006e18();
    } else if (Field_unk_02006df0((s32)&Value_0000092b) != 0) {
        Field_unk_02002184(8, (s32)&Value_00001e78, 0x990);
    } else if (Field_unk_02006e02((s32)&Value_0000092a) != 0) {
        Field_unk_02002184(8, (s32)&Value_00001e78, (s32)&Value_00000917);
    } else if (Field_unk_02006e12((s32)&Value_00000929) != 0) {
        Field_unk_02002184(8, (s32)&Value_00001e78, (s32)&Value_00000935);
    } else {
        Field_unk_02002190(8, (s32)&Value_00001e78, (s32)&Value_0000092c);
    }
}

void Dialogue_RunActorTenFlaggedDialogue(void)
{
    if (Field_unk_02006e5c(0x300) != 0) {
        u8 *o = Field_unk_02001ca0();
        u8 *p;

        Field_unk_02006e92();
        Field_unk_02001b60(o);
        Field_unk_02006f7e((s32)&Value_00001e9f);
        Field_unk_02005248(10);
        Field_unk_02006f34(o, 2);
        p = Field_unk_02006ed2(0);
        if (p != 0) {
            Field_unk_02006f14(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk_02006f42(o);
        Field_unk_02006f54(o, 0, 0);
        Field_unk_02006ee0();
    } else if (Field_unk_02006eb8((s32)&Value_0000092b) != 0) {
        Field_unk_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000992);
    } else if (Field_unk_02006ec8((s32)&Value_0000092a) != 0) {
        Field_unk_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000919);
    } else if (Field_unk_02006ed8((s32)&Value_00000929) != 0) {
        Field_unk_0200224a(10, (s32)&Value_00001e7b, (s32)&Value_00000937);
    } else {
        Field_unk_02002256(10, (s32)&Value_00001e7b, (s32)&Value_0000092e);
    }
}

void Scene_RunActor11FlagDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_unk_02006f28(0x8A0) != 0) {
        Field_unk_02006f58();
        Field_unk_0200708a(11, 0x102);
        Field_unk_02006f60(40);
        Field_unk_0200704e((s32)&Value_00001f47);
        Field_unk_02007066(11, 0);
        Field_unk_02006f82();
    } else if (Field_unk_02006f5c(0x300) != 0) {
        u8 *o = Field_unk_02001da0();
        u8 *p;

        Field_unk_02006f92();
        Field_unk_02001c60(o);
        Field_unk_0200707e((s32)&Value_00001ea0);
        Field_unk_02005348(11);
        Field_unk_02007034(o, 2);
        p = Field_unk_02006fd2(0);
        if (p != 0) {
            Field_unk_02007014(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk_02007042(o);
        Field_unk_02007054(o, 0, 0);
        Field_unk_02006fe0();
    } else if (Field_unk_02006fb8((s32)&Value_0000092b) != 0) {
        Field_unk_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_00000993);
    } else if (Field_unk_02006fc8((s32)&Value_0000092a) != 0) {
        Field_unk_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_0000091a);
    } else if (Field_unk_02006fd8((s32)&Value_00000929) != 0) {
        Field_unk_0200234a(11, (s32)&Value_00001e7e, (s32)&Value_00000938);
    } else {
        Field_unk_02002356(11, (s32)&Value_00001e7e, (s32)&Value_0000092f);
    }
}

void Scene_RunScene3b1SequenceA(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_unk_02007050();
    if (Field_unk_02007030(0x300) != 0) {
        rec7 = Field_unk_02001e74();
        Field_unk_02001d2e();
        Field_unk_0200714c(0x1ea1);
        Field_unk_02005416(12);
        Field_unk_02007102(rec7, 2);
        record = Field_unk_020070a0(0);
        if (record != 0) {
            Field_unk_020070e2(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Field_unk_02007110(rec7);
        Field_unk_02007122(rec7, 0, 0);
    } else {
        Field_unk_02007164(12, 2);
        Field_unk_020070aa(20);
        Field_unk_02007198(0x1e81);
        Field_unk_020071a8(12, 0);
        if (Field_unk_020070e0(0, 0) == 0) {
            Field_unk_02005476(12);
            Field_unk_02007162(12, 2);
            record = Field_unk_02007100(0);
            if (record != 0) {
                Field_unk_02007142(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Field_unk_02007170(12);
            Field_unk_02007182(12, 0, 0);
            Field_TestFlag300(0x300);
            if (Field_TestFlag92b(0x92b) != 0) {
                Field_TestFlag994(0x994);
                goto L_02000c9a;
            }
            if (Field_TestFlag92a(0x92a) != 0) {
                Field_TestFlag91b(0x91b);
                goto L_02000c9a;
            }
            if (Field_TestFlag929(0x929) != 0) {
                Field_TestFlag939(0x939);
                goto L_02000c9a;
            }
            Field_TestFlag930(0x930);
        } else {
            bump_step(1);
            Field_unk_02005504(12);
        }
    }
    L_02000c9a:;
    Field_unk_0200716c();
}

void Scene_RunScene3b1SequenceB(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_unk_02007194();
    if (Field_unk_02007174(0x300) != 0) {
        rec7 = Field_unk_02001fb8();
        Field_unk_02001e72();
        Field_unk_02007290(0x1ea2);
        Field_unk_0200555a(9);
        Field_unk_02007246(rec7, 2);
        record = Field_unk_020071e4(0);
        if (record != 0) {
            Field_unk_02007226(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ((void (*)())Field_unk_02007254)(rec7);
        Field_TestFlag936(rec7, 0, 0);
    } else {
        Field_unk_020072ce(0x1e84);
        ((void (*)())Field_unk_020072f0)(9, 0, 60);
        Field_unk_020072b8(9, 1);
        Field_unk_020072f0_a(9, 0);
        if (Field_unk_02007228(0, 0) == 0) {
            Field_unk_020055be(9);
            Field_unk_020072aa(9, 2);
            record = Field_unk_02007248(0);
            if (record != 0) {
                Field_unk_0200728a(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Field_unk_020072b8_a(9);
            Field_unk_020072ca(9, 0, 0);
            Field_TestFlag300(0x300);
            if (Field_TestFlag92b(0x92b) != 0) {
                Field_TestFlag991(0x991);
                goto L_02000de0;
            }
            if (Field_TestFlag92a(0x92a) != 0) {
                Field_TestFlag918(0x918);
                goto L_02000de0;
            }
            if (Field_TestFlag929(0x929) != 0) {
                Field_TestFlag936(0x936);
                goto L_02000de0;
            }
            Field_TestFlag92d(0x92d);
        } else {
            bump_step(1);
            Field_unk_0200564a(9);
        }
    }
    L_02000de0:;
    Field_unk_020072b2();
}

void Dialogue_RunActorThirteenFlag300Branch(void)
{
    if (Field_unk_020072bc(0x300) != 0) {
        u8 *o = Field_unk_02002100();
        u8 *p;

        Field_unk_020072f2();
        Field_unk_02001fc0(o);
        Field_unk_020073de((s32)&Value_00001ea3);
        Field_unk_020056a8(13);
        Field_unk_02007394(o, 2);
        p = Field_unk_02007332(0);
        if (p != 0) {
            Field_unk_02007374(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk_020073a2(o);
        Field_unk_020073b4(o, 0, 0);
        Field_unk_02007340();
    } else if (Field_unk_02007318((s32)&Value_0000092b) != 0) {
        Field_unk_020026aa(13, (s32)&Value_00001e88, (s32)&Value_00000995);
    } else if (Field_unk_02007328((s32)&Value_0000092a) != 0) {
        Field_unk_020026aa(13, (s32)&Value_00001e88, (s32)&Value_0000091c);
    } else if (Field_unk_02007338((s32)&Value_00000929) != 0) {
        Field_unk_020026aa(13, (s32)&Value_00001e88, (s32)&Value_0000093a);
    } else {
        Field_unk_020026b6(13, (s32)&Value_00001e88, (s32)&Value_00000931);
    }
}

void Scene_RunFlag300BranchDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_unk_02007388(0x300) != 0) {
        u8 *o = Field_unk_020021cc();
        u8 *p;

        Field_unk_020073be();
        Field_unk_0200208c(o);
        Field_unk_020074aa((s32)&Value_00001ea4);
        Field_unk_02005774(14);
        Field_unk_02007460(o, 2);
        p = Field_unk_020073fe(0);
        if (p != 0) {
            Field_unk_02007440(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk_0200746e(o);
        Field_unk_02007480(o, 0, 0);
        Field_unk_0200740c();
    } else if (Field_unk_020073e4((s32)&Value_0000092b) != 0) {
        Field_unk_02002776(14, (s32)&Value_00001e8b, (s32)&Value_00000996);
    } else if (Field_unk_020073f4((s32)&Value_0000092a) != 0) {
        Field_unk_02002776(14, (s32)&Value_00001e8b, (s32)&Value_0000091d);
    } else if (Field_unk_02007404((s32)&Value_00000929) != 0) {
        Field_unk_02002776(14, (s32)&Value_00001e8b, (s32)&Value_0000093b);
    } else {
        Field_unk_02002782(14, (s32)&Value_00001e8b, (s32)&Value_00000932);
    }
}

void Scene_RunActor15FlagDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_unk_02007454(0x300) != 0) {
        u8 *o = Field_unk_02002298();
        u8 *p;

        Field_unk_0200748a();
        Field_unk_02002158(o);
        Field_unk_02007576((s32)&Value_00001ea5);
        Field_unk_02005840(15);
        Field_unk_0200752c(o, 2);
        p = Field_unk_020074ca(0);
        if (p != 0) {
            Field_unk_0200750c(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_unk_0200753a(o);
        Field_unk_0200754c(o, 0, 0);
        Field_unk_020074d8();
    } else if (Field_unk_020074b0((s32)&Value_0000092b) != 0) {
        Field_unk_02002842(15, (s32)&Value_00001e8e, (s32)&Value_00000997);
    } else if (Field_unk_020074c0((s32)&Value_0000092a) != 0) {
        Field_unk_02002842(15, (s32)&Value_00001e8e, (s32)&Value_0000091e);
    } else if (Field_unk_020074d0((s32)&Value_00000929) != 0) {
        Field_unk_02002842(15, (s32)&Value_00001e8e, (s32)&Value_0000093c);
    } else {
        Field_unk_0200284e(15, (s32)&Value_00001e8e, (s32)&Value_00000933);
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

    if (Field_unk_02007520(0x300) != 0) {
        obj = Field_unk_02002364();
        Field_unk_02007556();
        Field_unk_02002224(obj);
        Field_unk_02007642(0x1ea6);
        Field_unk_0200590c(16);
        Field_unk_020075f8(obj, 2);

        actor = Field_unk_02007596(0);
        if (actor != 0) {
            Field_unk_020075d8(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Field_unk_02007606(obj);
        Field_unk_02007618(obj, 0, 0);
        Field_unk_020075a4();
    } else {
        if (Field_unk_0200757c(0x92b) != 0) {
            Field_unk_0200290e(16, 0x1e91, 0x998);
        } else if (Field_unk_0200758c(0x92a) != 0) {
            Field_unk_0200290e(16, 0x1e91, 0x91f);
        } else if (Field_unk_0200759c(0x929) != 0) {
            Field_unk_0200290e(16, 0x1e91, 0x93d);
        } else {
            Field_unk_0200291a(16, 0x1e91, 0x934);
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
    u8 *obj = Field_unk_02007772(0);
    u32 ofs = gOv17[bucket];
    s32 x = *(s16 *)(obj + 10) + ((s32)ofs >> 16);
    s32 z = *(s16 *)(obj + 18) + (s32)(s16)ofs;

    if (Field_unk_020023f0(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(obj + 12);
        point[2] = z << 16;

        if (Field_unk_0200773c(obj, point) != 0) {
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

    if (Field_unk_02007784((s32)&Value_0000092b) != 0) {
        ret = 3;
    } else if (Field_unk_02007792((s32)&Value_0000092a) != 0) {
        ret = 2;
    } else if (Field_unk_020077a0((s32)&Value_00000929) != 0) {
        ret = 1;
    }

    return Field_unk_020062b6(ret, 1);
}

/* Story selector owner at 0x02001324, 84 bytes; eight calls. Per-site call
 * veneers (raw asm confirms each callee slot uses a distinct local stub
 * even across the three near-identical "twin" owners at 0x1324/1378/13cc). */
void Dialogue_ShowLine1ECETo1ED0(void)
{
    Field_unk_020077f0();
    if (Field_unk_020077ce(0x92c)) Field_unk_020078e0(0x1ece);
    else if (Field_unk_020077e0(0x935)) Field_unk_020078f2(0x1ecf);
    else Field_unk_020078fa(0x1ed0);
    Field_unk_02007912(0x12, 0); Field_unk_0200782e();
}

/* Story selector owner at 0x02001378, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x13cc with distinct local stub addresses). */
void Dialogue_RunActor19TwoFlagLineA(void)
{
    Field_unk_02007844();
    if (Field_unk_02007822(0x92d)) Field_unk_02007934(0x1ece);
    else if (Field_unk_02007834(0x936)) Field_unk_02007946(0x1ecf);
    else Field_unk_0200794e(0x1ed0);
    Field_unk_02007966(0x13, 0); Field_unk_02007882();
}

/* Story selector owner at 0x020013cc, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x1378 with distinct local stub addresses). */
void Dialogue_RunActor20TwoFlagLine(void)
{
    Field_unk_02007898();
    if (Field_unk_02007876(0x92e)) Field_unk_02007988(0x1ece);
    else if (Field_unk_02007888(0x937)) Field_unk_0200799a(0x1ecf);
    else Field_unk_020079a2(0x1ed0);
    Field_unk_020079ba(0x14, 0); Field_unk_020078d6();
}

/* Story selector owner at 0x02001420, 60 bytes; six calls. */
void Dialogue_ShowLine1ED1Or1ED2(void)
{
    Field_unk_020078ec();
    if (Field_TestFlag92f(0x92f)) Field_unk_020079dc(0x1ed1);
    else Field_unk_020079e4(0x1ed2);
    Field_unk_020079fc(21, 0); Field_unk_02007918();
}

/* Story selector owner at 0x0200145c, 84 bytes; eight calls. */
void Dialogue_RunActor22TwoFlagLine(void)
{
    Field_unk_02007928();
    if (Field_unk_02007908(0x930)) Field_unk_02007a1a(0x1ece);
    else if (Field_unk_0200791a(0x939)) Field_unk_02007a2c(0x1ecf);
    else Field_unk_02007a34(0x1ed0);
    Field_unk_02007a4c(22, 0); Field_unk_02007968_a();
}

/* Story selector owner at 0x020014b0, 84 bytes; eight calls. */
void Dialogue_RunActor23BranchedDialogue(void)
{
    Field_unk_0200797c();
    if (Field_TestFlag931(0x931)) Field_unk_02007a6c(0x1ece);
    else if (Field_unk_0200796c(0x93a)) Field_unk_02007a7e(0x1ecf);
    else Field_unk_02007a86(0x1ed0);
    Field_unk_02007a9e(23, 0); Field_unk_020079ba_a();
}

/* Story selector owner at 0x02001504, 84 bytes; eight calls. */
void Dialogue_RunActor24BranchedDialogue(void)
{
    Field_unk_020079d0();
    if (Field_unk_020079ae(0x932)) Field_unk_02007ac0(0x1ece);
    else if (Field_TestFlag93b(0x93b)) Field_unk_02007ad2(0x1ecf);
    else Field_unk_02007ada(0x1ed0);
    Field_unk_02007af2(24, 0); Field_unk_02007a0e();
}

/* Story selector owner at 0x02001558, 60 bytes; six calls. */
void Dialogue_RunActor25FlaggedLine(void)
{
    Field_unk_02007a24();
    if (Field_unk_02007a02(0x933)) Field_unk_02007b14(0x1ed1);
    else Field_unk_02007b1c(0x1ed2);
    Field_unk_02007b34(25, 0); Field_TestFlag935();
}

/* Second-phase story selector at 0x02001594, 84 bytes; eight calls. */
void Dialogue_RunActor18TwoFlagLine(void)
{
    Field_unk_02007a60();
    if (Field_TestFlag92c(0x92c)) Field_unk_02007b50(0x1edb);
    else if (Field_TestFlag935(0x935)) Field_unk_02007b62(0x1edc);
    else Field_unk_02007b6a(0x1edd);
    Field_unk_02007b82(18, 0); Field_unk_02007a9e_a();
}

/* Second-phase story selector at 0x020015e8, 84 bytes; eight calls. */
void Dialogue_RunActor19TwoFlagLineB(void)
{
    Field_unk_02007ab4();
    if (Field_unk_02007a92(0x92d)) Field_unk_02007ba4(0x1edb);
    else if (Field_unk_02007aa4(0x936)) Field_unk_02007bb6(0x1edc);
    else Field_unk_02007bbe(0x1edd);
    Field_unk_02007bd6(19, 0); Field_unk_02007af2_a();
}

/* Second-phase story selector at 0x0200163c, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Field_unk_02007b08();
    if (Field_TestFlag92e(0x92e)) Field_unk_02007bf8(0x1edb);
    else if (Field_TestFlag937(0x937)) Field_unk_02007c0a(0x1edc);
    else Field_unk_02007c12(0x1edd);
    Field_unk_02007c2a(20, 0); Field_unk_02007b46();
}

/* Second-phase story selector at 0x02001690, 60 bytes; six calls. */
void Dialogue_RunActor21FlaggedLine(void)
{
    Field_unk_02007b5c();
    if (Field_unk_02007b3a(0x92f)) Field_unk_02007c4c(0x1ede);
    else Field_unk_02007c54(0x1edf);
    Field_unk_02007c6c(21, 0); Field_unk_02007b88();
}

/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void Dialogue_RunActor22BranchedDialogue(void)
{
    Field_unk_02007b98();
    if (Field_unk_02007b78(0x930)) Field_unk_02007c8a(0x1edb);
    else if (Field_unk_02007b8a(0x939)) Field_unk_02007c9c(0x1edc);
    else Field_unk_02007ca4(0x1edd);
    Field_unk_02007cbc(22, 0); Field_unk_02007bd8();
}

/* Second-phase story selector at 0x02001720, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Field_unk_02007bec();
    if (Field_unk_02007bca(0x931)) Field_unk_02007cdc(0x1edb);
    else if (Field_TestFlag93a(0x93a)) Field_unk_02007cee(0x1edc);
    else Field_unk_02007cf6(0x1edd);
    Field_unk_02007d0e_a(23, 0); Field_unk_02007c2a_a();
}

/* Second-phase story selector at 0x02001774, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Field_unk_02007c40();
    if (Field_TestFlag932(0x932)) Field_unk_02007d30(0x1edb);
    else if (Field_unk_02007c30(0x93b)) Field_unk_02007d42(0x1edc);
    else Field_unk_02007d4a(0x1edd);
    Field_unk_02007d62(24, 0); Field_unk_02007c7e();
}

/* Second-phase story selector at 0x020017c8, 60 bytes; six calls. */
void Dialogue_ShowLine1EDEOr1EDF(void)
{
    Field_unk_02007c94();
    if (Field_TestFlag933(0x933)) Field_unk_02007d84(0x1ede);
    else Field_TestFlag8a0(0x1edf);
    Field_unk_02007da4(25, 0); Field_unk_02007cc0();
}

void Scene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk_02007cd6();
    Field_unk_02007dbc(a1);
    Field_unk_02007dcc(a0, 0);
    if (Field_unk_02007d04(0, 0) == 0) {
        Field_unk_0200609a(a0);
        Field_unk_02007d86(a0, 2);
        record = Field_unk_02007d24(0);
        if (record != 0) {
            Field_unk_02007d66(a0, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Field_unk_02007d94(a0);
        Field_unk_02007da6(a0, 0, 0);
        Field_unk_02007d0e(0x300);
        Field_unk_02007d14(a2);
    } else {
        bump_step(1);
        Field_unk_020060f0(a0);
    }
    Field_unk_02007d58();
}

void Scene_RunScene3b1SequenceC(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    record = Field_unk_02007d82(0);
    if ((u32)((*(volatile u16 *)(record + 6) + -0x2000) << 16) > -0x40000000) {
        if (Field_unk_02007d50(0x928) != 0) {
            if (Field_TestFlag93e(0x93e) == 0) {
                Field_unk_02007f1c(17);
                goto L_0200194e;
            }
        }
        Field_unk_02007f24(15);
    } else {
        Field_unk_02007d9a();
        if (Field_TestFlag93e(0x93e) != 0) {
            Field_unk_02007e8a(0x1f81);
        } else {
            if (Field_TestFlag8a0(0x8a0) != 0) {
                Field_unk_02007e9e(0x1f48);
            } else {
                if (Field_TestFlag928(0x928) != 0) {
                    Field_unk_02007eb0(0x1f7f);
                } else {
                    if (Field_TestFlag925(0x925) != 0) {
                        Field_unk_02007ec2(0x1f7d);
                    } else {
                        Field_unk_02007eca(0x1f7b);
                    }
                }
            }
        }
        if (Field_TestFlag928(0x928) != 0) {
            record = Field_unk_02007dd2(0x93e);
            if (record != 0) {
                goto L_02001942;
            }
            Field_unk_02007ef6(17, 0);
        } else {
            L_02001942:;
            Field_unk_02007f00(15, 0);
        }
        Field_unk_02007e1c();
    }
    L_0200194e:;
}

void Scene_RunScene3b1SequenceD(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Field_TestFlag301(0x301) != 0) {
        Field_unk_02007ede();
        Field_unk_02007ff4(8);
        Field_unk_02007eb6(0x1e48, 1, 8);
        Field_unk_02007f28(0, 0x19999, 0xcccc);
        Field_unk_02007f6c(0, 0x198, 134);
        Field_unk_020062c6(0, 0x4000);
        Field_unk_02007f1a();
    }
}

/*
 * Gated on flag 0x922.  Every callee slot has its own local call stub;
 * Field_unk_02006336 and Field_unk_02006336_a are the same stub declared twice without
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

    if (Field_unk_02007f06(0x922) == 0)
        return;

    Field_unk_02007f36();
    Field_unk_020080ba();
    Field_unk_02008072(0x19999, 0x3333);
    Field_unk_02006336(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Field_unk_02008038(0x1d26);

    Field_unk_02006302(8);
    Field_unk_02006308(10);
    Field_unk_02006326(8, 0x3000);
    Field_unk_02006318(8);
    Field_unk_02006336_a(10, 0xd000);
    Field_unk_02006328(10);
    Field_unk_02006346(9, 0x5000);
    Field_unk_02006338(9);

    Field_unk_020080a6(8, 0, 20);
    Field_unk_02006348(8);
    Field_unk_02006366(9, 0x8000);
    Field_unk_02006358(9);
    Field_unk_0200635e(10);
    Field_unk_02006364(8);
    Field_unk_02006382(10, 0xb000);
    Field_unk_02006374(8);

    Field_TestFlag920(0x920);
    Field_unk_02007fe4();
}

void Scene_RunThreeActorPresentation(void)
{
    s32 request_a;
    s32 request_b;
    s32 action;

    if (Field_TestFlag911(0x911) == 0) {
    } else {
        Field_unk_02008018();
        Field_unk_0200819c();
        Field_unk_02008154(0x26666, 0x4ccc);
        Field_unk_02006418( 0x5b70000, -1, 0x1d00000, 0x10000014);
        Field_unk_020080f4(13, 1);
        Field_unk_02008122(0x1d56);
        Field_unk_020063f0(0x200d);
        Field_unk_02006412(12, 0xd000);
        request_a = 0x800c;
        Field_unk_02008188(12, 0x102, 20);
        Field_unk_02008120(12, 2);
        Field_unk_0200641a(request_a);
        Field_unk_02008136(14, 1);
        Field_unk_02008180(0xa00e, 0, 20);
        Field_unk_02006448(12, 0);
        request_b = 0xa00e;
        Field_unk_020081bc(12, 0x101, 40);
        Field_unk_020081c6(14, 0x103, 40);
        Field_unk_0200815e(14, 3);
        Field_unk_02006458(request_b);
        Field_unk_020081e6(12, 0x102);
        Field_unk_020080bc(40);
        Field_unk_0200817c(12, 3);
        Field_unk_02006476(request_a);
        Field_unk_02008192(14, 1);
        Field_unk_02006484(request_b);
        Field_unk_020064a6(14, 0xb000);
        Field_unk_02006498(request_b);
        GetAscendingMotionStep(12, 0xd000);
        Field_unk_02008228(12, 0x100, 30);
        Field_unk_020081c0(12, 1);
        Field_unk_020064ba(request_a);
        Field_unk_020081b6(13, 4);
        Field_unk_020064c8(0x200d);
        Field_unk_020081dc(13, 2);
        Field_unk_020064d6(0x200d);
        Field_unk_020081c2(12, 4);
        Field_unk_020064e4(request_a);
        Field_unk_020081e0(14, 4);
        Field_unk_020064f2(request_b);
        Field_unk_02006514(14, 0x8000);
        Field_unk_02008214(14, 2);
        Field_unk_02008266(request_b, 0, 20);
        Field_unk_02008280(12, 0, 0);
        Field_unk_020082a4(12, 0x102, 80);
        Field_unk_02008286(request_a, 0, 20);
        Field_unk_020082b8(14, 0x103, 0);
        Field_unk_020082c2(13, 0x103, 60);
        Field_unk_0200825a(14, 2);
        Field_unk_02006554(request_b);
        Field_unk_02006570(14, 0xb000);
        Field_unk_02008278(14, 1);
        Field_unk_0200656a(request_b);
        Field_unk_0200658c(13, 0x3000);
        Field_unk_020082fe(13, 0x101, 0);
        Field_unk_02008308(12, 0x101, 60);
        Field_unk_020082a8(13, 1);
        Field_unk_0200659a(13);
        Field_unk_02008320(14, 0x103, 40);
        Field_unk_020082c0(14, 1);
        Field_unk_020065b2(request_b);
        Field_unk_02008320_a(12, 0xd000, 0);
        Field_unk_0200832c(13, 0x5000, 40);
        Field_unk_02008336(12, 0, 0);
        Field_unk_020065ee(13, 0x3000);
        Field_unk_020082f6(12, 2);
        Field_unk_02008340(request_a, 0, 20);
        Field_unk_0200835c(14, 0x4000, 40);
        Field_unk_020065fe(request_b);
        Field_unk_02008312(12, 2);
        Field_unk_02008322(13, 2);
        Field_unk_02008268(60);
        Field_unk_02008330(13, 1);
        Field_unk_02006622(13);
        Field_unk_0200831e(14, 3);
        Field_unk_02006630(request_b);
        Field_unk_020083b8(12, 0x102, 40);
        Field_unk_02008358(12, 2);
        Field_unk_0200664a(request_a);
        Field_unk_02008346(13, 3);
        Field_unk_02006658(13);
        Field_unk_020083c6(14, 0xb000, 40);
        Field_unk_0200834e(14, 3);
        Field_unk_02008366(13, 3);
        Field_unk_02008308_a(14, 0x19999, 0xcccc);
        Field_unk_02008312_a(13, 0x19999, 0xcccc);
        action = (s32)gOv18;
        Field_unk_02008324(14, action);
        Field_unk_0200832c_a(13, action);
        Field_unk_020082f2(20);
        Field_unk_0200840e(12, 0x4000, 0);
        Field_unk_02008340_a(0, 0x26666, 0x13333);
        *(u8 *)(Field_unk_02008336_a(0) + 90) &= 254;
        Field_unk_02008394(0, 184, 0x208);
        Field_unk_0200832a(1);
        {
            u8 *record = Field_unk_02008358_a(0);
            u32 flag = 1;

            flag = flag | record[90];
            record[90] = (u8)flag;
        }
        Field_unk_02008454(0, 0x8000, 20);
        Field_unk_02008460(0, 0x4000, 0);
        Field_unk_0200840a(12, 4, 20);
        Field_unk_02008476(0, 0xa000, 20);
        Field_unk_02008426(12, 2);
        Field_unk_02006720(12);
        Field_unk_020083b6(12, 0x19999, 0xcccc);
        Field_unk_020083c6_a(12, action);
        Field_TestFlag922(40);
        Field_unk_020084a8(0, 0x4000, 0);
        Field_unk_020083e6(12);
        Field_TestFlag922(0x922);
        Field_unk_020083b8_a();
    }
}

void Scene_RunExtendedActorChoreography(void)
{
    s32 request_a;
    s32 request_b;
    s32 request_c;

    Field_unk_020085ae(28);
    Field_unk_0200854e(0x26666, 0x4ccc);
    Field_unk_02006814(0x1c80000, -1, 0x2880000, 0x10000014);
    Field_unk_020084f0(9, 1);
    Field_unk_0200851e(0x1d93);
    Field_unk_020067e8(9);
    Field_unk_02008558(0, 0xd000, 0);
    Field_unk_02008564(10, 0xd000, 0);
    Field_unk_0200856e(11, 0, 0);
    Field_unk_0200857a(12, 0x3000, 0);
    Field_unk_02008586(13, 0x8000, 40);
    Field_unk_020085a8(9, 0x103, 40);
    Field_unk_02008540(9, 2);
    Field_unk_0200683a(9);
    Field_unk_020085a8_a(12, 0, 0);
    Field_unk_020085b4(11, 0xd000, 0);
    Field_unk_020085c0(13, 0xd000, 20);
    Field_unk_02008578(11, 1);
    Field_unk_0200686e(0x100b);
    Field_unk_020085f6(13, 0x102, 20);
    Field_unk_0200858e(13, 2);
    Field_unk_02006888(13);
    Field_unk_0200860e(9, 0x105, 60);
    Field_unk_02006898(9);
    Field_unk_02008620(12, 0x104, 20);
    Field_unk_020068aa(0x900c);
    Field_unk_020085c6(8, 1);
    Field_unk_020085a0(8, 3);
    Field_unk_020068c4(8);
    Field_unk_020068e0(12, 0x3000);
    Field_unk_020068d2(0x900c);
    Field_unk_020068f4(11, 0xb000);
    Field_unk_020085dc(11, 3);
    Field_unk_02008542(10);
    Field_unk_0200860a(13, 1);
    Field_unk_020085f2(13, 3);
    Field_unk_02006904(13);
    Field_unk_02008674(13, 0x8000, 0);
    Field_unk_02008680(12, 0x5000, 0);
    Field_unk_0200693e(11, 0x5000);
    Field_unk_020085c0_a(13, 0x6666, 0x3333);
    Field_unk_020085ca(12, 0xcccc, 0x6666);
    Field_unk_02008608(12, 0x1bc, 0x29c);
    Field_unk_0200861e(13, 0x1d8, 0x29c);
    Field_unk_02008634_a(12);
    Field_unk_0200864c(12, 1);
    Field_unk_020085c2(80);
    Field_unk_02006990(12, 0xd000);
    Field_unk_02008702(12, 0x101, 60);
    Field_unk_020086a2(11, 1);
    Field_unk_020085e8(20);
    Field_unk_020086f2(0x400b, 0, 40);
    Field_unk_020086ba(11, 2);
    Field_unk_02008714(11, 0xd000, 0);
    Field_unk_020069b6(0x100b);
    Field_unk_02008724(12, 0xd000, 0);
    Field_unk_02008746(9, 0x101, 60);
    Field_unk_020086b6_a(11, 4);
    Field_unk_0200862c_a(20);
    Field_unk_020069de(0x100b);
    Field_unk_020086ca(9, 3);
    Field_unk_020069ec(9);
    Field_unk_0200875a(13, 0xd000, 0);
    Field_unk_02008784(13, 0x102);
    Field_unk_0200870e(13, 2, 20);
    Field_unk_02006a10(13);
    Field_unk_0200870c(9, 3);
    request_a = 0x100c;
    Field_unk_02006a20(9);
    Field_unk_02006a3c(11, 0xd000);
    Field_unk_02008744(12, 1);
    Field_unk_02006a36(request_a);
    Field_unk_020087bc(8, 0x105, 40);
    Field_unk_0200872c(8, 3);
    Field_unk_02006a4e(8);
    Field_unk_020087d6(13, 0x102, 40);
    Field_unk_02008768(13, 4, 0);
    Field_unk_02006a6a_a(13);
    Field_unk_02008756(9, 3);
    Field_unk_02006a78_a(9);
    Field_unk_02008794(11, 1);
    Field_unk_02006a86(0x100b);
    Field_unk_0200880e_a(8, 0x102, 40);
    Field_unk_020087e6(8, 0);
    Field_unk_020087ae(11, 2);
    Field_unk_02008800(0x100b, 0, 40);
    Field_unk_02008834(9, 0x100, 0);
    Field_unk_02008826(9, 0x5000, 20);
    Field_unk_020087d6_a(9, 2);
    Field_unk_02008828(9, 0, 20);
    Field_unk_020087c0(11, 3);
    Field_unk_02008736(20);
    Field_unk_0200886a(12, 0x100, 40);
    Field_unk_02008802(12, 2);
    request_b = 0x400b;
    Field_unk_02006afe(request_a);
    Field_unk_0200886c(11, 0x5000, 20);
    Field_unk_02006b0e(request_b);
    Field_unk_02008822(12, 2);
    Field_unk_02006b1c(request_a);
    Field_unk_02008818(11, 3);
    Field_unk_02008840(11, 1);
    Field_unk_02006b32(request_b);
    Field_unk_020088ba(12, 0x102, 60);
    Field_unk_0200885a_a(9, 1);
    Field_unk_02006b4c(9);
    Field_unk_020088d2(11, 0x101, 40);
    Field_unk_020088c4(11, 0xd000, 20);
    Field_unk_0200884c(9, 3);
    Field_unk_02006b6e(9);
    Field_unk_020088f4(11, 0x103, 20);
    Field_unk_0200888c(11, 2);
    Field_unk_02006b86(0x100b);
    Field_unk_0200890e(9, 0x108, 40);
    Field_unk_02006b98(9);
    Field_unk_020088b4(8, 1);
    Field_unk_0200888c_a(8, 3);
    Field_unk_02006bae(8);
    Field_unk_02008958(9, 0xd000, 40);
    Field_unk_02006bfa(9);
    Field_unk_02008916(12, 1);
    Field_unk_0200885c(20);
    Field_unk_02006c0e(request_a);
    Field_unk_0200897c(11, 0x5000, 0);
    Field_unk_02008986(9, 0x5000, 0);
    Field_unk_02008992(13, 0x8000, 0);
    Field_unk_0200899e(0, 0xc000, 0);
    Field_unk_020089a8(10, 0xb000, 40);
    Field_unk_02008960(11, 1);
    Field_unk_02006c52(request_b);
    Field_unk_0200893e(12, 3);
    Field_unk_020089b8(request_a, 0, 20);
    Field_unk_02008980(9, 2);
    Field_unk_02006c72(9);
    Field_unk_020089fa(12, 0x108, 40);
    Field_unk_0200896a(12, 3);
    Field_unk_02006c8c(request_a);
    Field_unk_02008988(8, 3);
    Field_unk_02006c9a(8);
    Field_unk_02008a0a(8, 0x8000, 20);
    Field_unk_02008aa0(19);
    Field_unk_020089c0(8, 2);
    request_c = 0x8008;
    Field_unk_02008a3e(8, 0x100, 80);
    Field_unk_02006cc8(request_c);
    Field_unk_02008a4e(12, 0x101, 0);
    Field_unk_02008a58(11, 0x101, 0);
    Field_unk_02008a62(13, 0x101, 0);
    Field_unk_02008a6c(10, 0x101, 0);
    Field_unk_02008a76(0, 0x101, 40);
    Field_unk_02008a68(12, 0xd000, 0);
    Field_unk_02008a72(11, 0xd000, 0);
    Field_unk_02008a7c(13, 0xb000, 0);
    Field_unk_02008a86(10, 0xb000, 0);
    Field_unk_02008a92(0, 0xc000, 40);
    Field_unk_02008abc(8, 0x102);
    Field_unk_02008a46(8, 4, 40);
    Field_unk_02008a56(8, 2);
    Field_unk_02008a9e(request_c, 0);
    Field_unk_020089e8(8, 0x19999, 0xcccc);
    Field_unk_02008a2a(8, 0x1db, 0x256);
    Field_unk_02008ad6(8, 0x8000, 0);
    Field_unk_02008a0c(9, 0x10000, 0x8000);
    Field_unk_02008a50(9, 0x1ce, 0x26a);
    Field_unk_02006da8_a(9, 0xb000);
    Field_unk_02008b1c(9, 0x100, 40);
    Field_unk_02008ab4(9, 2);
    Field_unk_02006dae(0x8009);
    Field_unk_02008b34(11, 0x101, 60);
    Field_unk_02006dbe(11);
    Field_unk_02008b46(12, 0x102, 20);
    Field_unk_02006dd0(request_a);
    Field_unk_02008b56(8, 0x103, 20);
    Field_unk_02008ae8(8, 4, 0);
    Field_unk_02008b52(8, 0x5000, 20);
    Field_unk_02006df4(8);
    Field_unk_02008bee(28);
    Field_unk_02008b0e_a(8, 3);
    Field_unk_02006e08(8);
    Field_unk_02008b8e(13, 0x101, 60);
    Field_unk_02006e18_a(13);
    Field_unk_02006e34(8, 0x3000);
    Field_unk_02008b1c_a(8, 4);
    Field_unk_02006e2e(8);
    Field_unk_02008b00(12, 0x1bc, 0x274);
    Field_unk_02006e58(12, 0xd000);
    Field_unk_02006e4a(0x900c);
    Field_unk_02008bb8(8, 0x5000, 20);
    Field_unk_02008b50(8, 3);
    Field_unk_02006e62(8);
    Field_unk_02008bea(11, 0x102, 60);
    Field_unk_02006e74_a(0x100b);
    Field_unk_02008bfa(13, 0x107, 40);
    Field_unk_02008b92(13, 2);
    Field_unk_02006e8c_c(13);
    Field_unk_02006ea8(9, 0x3000);
    Field_unk_02008b90(9, 4);
    Field_unk_02006ea2(0x1009);
    Field_unk_02006ebe(12, 0);
    Field_unk_02008bc6(8, 1);
    Field_unk_02006eb8_a(8);
    Field_TestFlag921(11, 0, 0);
    Field_unk_02008c48(12, 0x105, 0);
    Field_unk_02008c52(9, 0x105, 60);
    Field_unk_02008c6a(0x13333, 0x2666);
    Field_unk_02006f32(0x1d00000, -1, 0x2a80000, 0x10000000);
    Field_unk_02008c0e(10, 1);
    Field_unk_02006f16(10, 0);
    Field_unk_02006f08(10);
    Field_unk_02008c76(0, 0, 0);
    Field_unk_02008c9a(10, 0x102, 40);
    Field_unk_02006f24(10);
    Field_unk_02008c94(10, 0x8000, 20);
    Field_unk_02008cb8(10, 0x100, 0);
    Field_unk_02008c4a(10, 4, 40);
    Field_unk_02006f4c(10);
    Field_unk_02008c68(10, 1);
    Field_unk_02006f5a(10);
    Field_unk_02008bf4(13, 0x10000, 0x8000);
    Field_unk_02008c30(13, 0x1b6, 0x293);
    Field_unk_02008ce4(8, 0x8000, 0);
    Field_unk_02008cee(9, 0xb000, 0);
    Field_unk_02008cf8(12, 0x3000, 0);
    Field_unk_02008d02(11, 0xb000, 0);
    Field_unk_02008d98(17);
    Field_unk_02008c3e(10, 0x10000, 0x8000);
    Field_unk_02008c82(10, 0x1e8, 0x2ae);
    Field_unk_02008d2c(10, 0xb000, 0);
    Field_unk_02008ca2(13);
    Field_unk_02008cba(13, 1);
    Field_unk_02008d44(13, 0xd000, 0);
    Field_unk_02008dc8();
    Field_TestFlag921(0x921);
}

void Scene_RunBranchingActorPresentation(void)
{
    u8 *record;
    s32 request_a;
    s32 request_b;
    s32 value;
    s32 request_c;
    s32 action;

    Field_unk_02008caa();
    Field_unk_02008e2e();
    Field_unk_02008d78(8, 0, 0);
    Field_unk_02008de4(8, 0x100, 40);
    request_a = 0x1008;
    Field_unk_02008d7e(8, 3);
    Field_unk_02008db4(0x1ddb);
    Field_unk_0200707e_a(request_a);
    Field_unk_02008d92(9, 1);
    Field_unk_02008d9a(12, 1);
    Field_unk_02008da2(11, 1);
    Field_unk_02008daa(13, 1);
    Field_unk_02008dba(10, 1);
    Field_unk_02008e16(9, 0xd000, 0);
    Field_unk_02008e22(12, 0xd000, 0);
    Field_unk_02008e2e_a(11, 0xd000, 0);
    Field_unk_02008e3a(13, 0xd000, 0);
    Field_unk_02008e46(10, 0xb000, 20);
    Field_unk_02008dfe(8, 1);
    Field_unk_02008e36(request_a, 0);
    if (Field_unk_02008d6e(0, 0) == 0) {
        Field_unk_02008e1a(9, 2);
        Field_unk_0200710c_b(0x9009);
        Field_unk_02008e94(8, 0x108, 40);
        Field_TestFlag939(request_a);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
        Field_unk_02008e6e(9, 1);
        Field_unk_02007160(0x9009);
        Field_unk_02008e74(8, 2);
        Field_unk_0200716e(0x9008);
    }
    Field_unk_02008ef4(13, 0x105, 40);
    Field_unk_02008f0c(0xcccc, 0x1999);
    Field_unk_02008f26(0x1d80000, -1, 0x27c0000, 1);
    Field_unk_02008e2c(13, 0x10000, 0x8000);
    Field_unk_02008e74_a(13, 0x1d8, 0x296);
    Field_unk_020071cc(13, 0xb000);
    Field_unk_020071be(13);
    Field_unk_020071e0(8, 0x5000);
    Field_unk_02008ec8(8, 3);
    Field_unk_02008ed0(9, 3);
    Field_unk_02008f4a(11, 0, 0);
    Field_unk_02008f56(13, 0x8000, 20);
    Field_unk_02008ede(11, 3);
    Field_unk_02008ef6(13, 3);
    Field_unk_02008e5c(20);
    Field_unk_02008f24(12, 1);
    Field_TestFlag300(12, 0x3000);
    Field_unk_02008f7c(0x100c, 0, 20);
    Field_unk_02008f96(11, 0xb000, 20);
    Field_unk_02008fba(11, 0x101, 40);
    Field_unk_02007246_a(11);
    request_b = 0x900c;
    Field_unk_02007264(12, 0xd000);
    Field_unk_02008f3c(12, 4);
    Field_unk_0200725e(request_b);
    Field_unk_0200727a(13, 0xb000);
    Field_unk_02008f82(13, 1);
    Field_unk_02007274(13);
    Field_unk_02008ffc(9, 0x100, 20);
    Field_unk_0200729c(9, 0x3000);
    Field_unk_02008fa4(9, 1);
    Field_unk_02007296(9);
    Field_unk_02008f92(12, 3);
    Field_unk_020072a4(request_b);
    Field_unk_02008fc0(8, 2);
    Field_unk_020072b2_a(8);
    Field_unk_020072ce_a(12, 0xd000);
    Field_unk_02008fb6(12, 3);
    Field_unk_020072c8(request_b);
    Field_unk_02008fe4_a(11, 2);
    Field_unk_020072ec(11, 0xb000);
    Field_unk_020072de(11);
    Field_unk_020072fa(12, 0);
    Field_unk_020072ec_a(request_b);
    Field_unk_0200905a(8, 0x3000, 0);
    Field_unk_02009064(9, 0, 0);
    Field_unk_0200906e_a(11, 0xd000, 0);
    Field_unk_02009078_a(13, 0xd000, 0);
    Field_unk_02008fae(0, 0x10000, 0x8000);
    Field_unk_02008ff4(0, 0x1e6, 0x260);
    Field_unk_020090a0(0, 0x8000, 0);
    record = Pointer1(Field_unk_02008fbe, 0);
    if (record != 0) {
        Field_unk_0200902c(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk_02008fea(1, 0x10000, 0x8000);
    Field_unk_02009030(1, 0x1e6, 0x270);
    Field_unk_020090dc(1, 0x8000, 0);
    record = Pointer1(Field_unk_02008ffa, 1);
    if (record != 0) {
        Field_unk_02009068(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk_02009026(2, 0x10000, 0x8000);
    Field_unk_0200906c(2, 0x1e6, 0x280);
    Field_unk_02009118(2, 0x8000, 0);
    record = Pointer1(Field_unk_02009036, 2);
    if (record != 0) {
        Field_unk_020090a4(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk_02009062(3, 0x10000, 0x8000);
    Field_unk_020090a8(3, 0x1e6, 0x290);
    Field_unk_02009154(3, 0x8000, 20);
    Field_unk_02009178(12, 0x108, 40);
    Field_unk_02007402(request_b);
    Field_unk_0200911e(9, 1);
    Field_unk_02007410(0x1009);
    Field_unk_0200910c(8, 3);
    Field_unk_02007434(8, 0x5000);
    Field_unk_02007426(8);
    Field_unk_02007442(8, 0x3000);
    Field_unk_0200917a(8, 0);
    if (Field_unk_020090b2(0, 0) == 1) {
        Field_unk_02009156(8, 2);
        Field_unk_02007450(8);
        Field_unk_0200914c(12, 3);
        Field_unk_0200745e(request_b);
        Field_unk_02009172(9, 1);
        Field_unk_020091c4(0x9009, 0, 40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 3;
        Field_unk_020091d2(8, 3);
        Field_unk_02009224(8, 0, 40);
    }
    Field_unk_020091ec(13, 1);
    Field_unk_020074de(13);
    Field_unk_020091fa(8, 1);
    value = 176;
    Field_unk_02007508(8, 0x5000);
    Field_unk_020074fe(8);
    Field_unk_0200921a(13, 1);
    Field_unk_02007522(13, (value << 8));
    Field_unk_0200926c(13, 0, 20);
    Field_unk_02009214(8, 3);
    Field_unk_020091b6(8, 0xcccc, 0x6666);
    request_c = 0x4008;
    Field_unk_020091fe(8, 0x1d8, 0x278);
    Field_unk_02007540(request_c);
    Field_unk_020092c6(13, 0x103, 40);
    Field_unk_0200925e(13, 2);
    Field_unk_02007558(13);
    Field_unk_02009244(8, 4);
    Field_unk_020092be(request_c, 0, 40);
    Field_unk_02009286(11, 1);
    Field_unk_0200758e(11, (value << 8));
    Field_unk_02007580(0x100b);
    Field_unk_02009308(10, 0x102, 20);
    Field_unk_02009222(10, 0x26666, 0x13333);
    Field_unk_020092a4(10, 2, 0);
    Field_unk_02009270(10, 0x1ce, 0x2a2);
    Field_unk_020075c8(10, (value << 8));
    Field_unk_020092c8(10, 2);
    Field_unk_020075c2(10);
    Field_unk_020075de(9, 0x5000);
    Field_unk_020092c6_a(9, 4);
    Field_unk_020075d8_a(9);
    Field_unk_020092d4(8, 3);
    Field_unk_020075e6(request_c);
    Field_unk_0200936e(13, 0x102, 40);
    Field_unk_02009350(13, 0, 40);
    Field_unk_0200761a(9, 0x3000);
    Field_unk_0200931a(9, 2);
    Field_unk_02007614(0x1009);
    Field_unk_02007630(12, 0);
    Field_unk_0200938c(8, 0x8000, 0);
    Field_unk_02009396(9, 0x5000, 0);
    Field_unk_020093a0(11, (value << 8), 0);
    Field_unk_020093aa(13, (value << 8), 0);
    Field_unk_020093b4(10, (value << 8), 20);
    Field_unk_0200936c(12, 1);
    Field_unk_020093b6(0x100c, 0, 20);
    Field_unk_020093e8(8, 0x101, 40);
    Field_unk_0200768a(8, 0xd000);
    Field_unk_020093c2(0x1008, 0);
    if (Field_unk_020092fa(0, 0) == 0) {
        Field_unk_02009386(8, 3);
        Field_unk_02007698(0x1008);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Field_unk_020076c0(0x1008);
    }
    Field_unk_020093bc(0, 3);
    Field_unk_020093c4(8, 3);
    Field_unk_020076d6(0x1008);
    Field_unk_020076f4(8, 0x8000);
    Field_unk_020076e6(0x4008);
    Field_unk_0200776c(2, 0, 0);
    Field_unk_020093dc(12, 3);
    Field_unk_020093e4(11, 3);
    Field_unk_020093ec(9, 3);
    Field_unk_0200941c(10, 2);
    Field_unk_0200942c(13, 2);
    Field_unk_02009372(20);
    action = (s32)gOv19;
    Field_unk_020093bc_a(10, action);
    Field_unk_02009382(4);
    Field_unk_020093ca(11, action);
    Field_unk_02009390(4);
    Field_unk_020093d8(12, action);
    Field_unk_0200939e(4);
    Field_unk_020093e6(9, action);
    Field_unk_0200943e(3, 2);
    record = Pointer1(Field_unk_020093dc_a, 2);
    if (record != 0) {
        Field_unk_0200941e(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_unk_0200944c(3);
    Field_unk_0200945e(3, 0, 0);
    Field_unk_0200946e(2, 2);
    record = Pointer1(Field_unk_0200940c, 1);
    if (record != 0) {
        Field_unk_0200944e(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_unk_0200947c(2);
    Field_unk_0200948e(2, 0, 0);
    Field_unk_0200949e(1, 2);
    record = Pointer1(Field_unk_0200943c, 0);
    if (record != 0) {
        Field_unk_0200947e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_unk_020094ac(1);
    Field_unk_020094be(1, 0, 0);
    Field_unk_0200947e_a(13, action);
    Field_unk_020094bc(8, 0x1c8, 0x288);
    Field_unk_02007814(8, 0);
    Field_unk_02009452(232);
    Field_TestFlag925(0x925);
    Field_unk_02009474();
}

/*
 * A four-way gated cascade. Each gate either runs its own setter sequence
 * and returns or falls through to the next, and all four share one exit.
 * The 336-byte owner at 0x0200351c includes its eight trailing pool words.
 */
void State_RunFlagGatedSetupCascade(void)
{
    if (Field_unk_020099c8((s32)&Value_0000093e) != 0) {
        Field_unk_02009a7e(8, 0, 0);
        Field_unk_02009a88(9, 0, 0);
        Field_unk_02009a92(10, 0, 0);
        Field_unk_02009a9c(11, 0, 0);
        Field_unk_02009aa6(12, 0, 0);
        Field_unk_02007efa(14, 0, 0);
        return;
    }

    if (Field_unk_02009a0a(0x8a0) != 0) {
        Field_unk_02007e0e(8, 0x98, 0x1bc, 0x3000);
        Field_unk_02009a86(8, 0x0200e958);
        Field_unk_02007e32(10, 0xb8, 0x1e0, 0xb000);
        Field_unk_02007e40(12, 0xaa, 0x1e8, 0xb000);
        Field_unk_02007e4c(13, 0x88, 0x1e8, 0xd000);
        Field_unk_02007e58(15, 0x78, 0x1e0, 0xd000);
        Field_unk_02007e64(14, 0xb8, 0x20e, 0xb000);
        Field_unk_02007e74(11, 0x88, 0x248, 0x8000);
        Field_unk_02009aec(11, 0x0200e840);
        return;
    }

    {
        s32 t = Field_unk_02009a94((s32)&Value_00000928);
        if (t != 0) {
            Field_unk_02008600(t);
            return;
        }
    }

    if (Field_unk_02009aa4((s32)&Value_00000925) != 0) {
        Field_unk_02007efa(18, 0, 0);
        return;
    }

    if (Field_unk_02009aba((s32)&Value_00000911) != 0 &&
        Field_unk_02009ac4((s32)&Value_00000922) != 0) {
        Field_unk_02007f1a_a(14, 0, 0);
        Field_unk_02009b84(12, 0, 0);
    }
}

void RunSceneSelectionChain(void)
{
    Field_unk_02009a92_a(1);
    Field_unk_02006e2a();
    if (Field_unk_02009b1c(2366) != 0) {
        Field_unk_02007f72(4, 4, 0);
        Field_unk_02007f2a(8, 412, 222, 12288);
        Field_unk_02007f3a(9, 458, 161, 32768);
    } else {
        if (Field_unk_02009b54(2208) != 0) {
            Field_unk_02009c0e(8, 30932992, 9961472);
            Field_unk_02009c1e(9, 5);
            Field_unk_02007fc0(4, 4, 0);
        } else {
            if (Field_unk_02009b80(2347) != 0) {
                Field_unk_02007fd6(16, 0, 0);
                Field_unk_02007fe0(4, 4, 0);
                Field_unk_02007952(3);
            } else {
                if (Field_unk_02009ba6_a(2346) != 0) {
                    Field_unk_02007ffc(16, 0, 0);
                    Field_unk_02008006(4, 3, 0);
                    Field_unk_02007978(2);
                } else {
                    if (Field_unk_02009bcc(2345) != 0) {
                        Field_unk_02008022(16, 0, 0);
                        Field_unk_0200802c(4, 2, 0);
                        Field_unk_0200799e(1);
                    } else {
                        if (Field_unk_02009bf2(2344) != 0) {
                            Field_unk_02008048(16, 0, 0);
                            Field_unk_02009cb2(10, 0, 0);
                            Field_unk_020079c4(0);
                        } else {
                            Field_unk_02009cca(9, 5);
                            if (Field_unk_02009c20(2341) != 0 && Field_unk_02009c2a(2342) == 0) {
                                Field_unk_0200703e();
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
        u8 *obj = Field_unk_02009ca8(i);
        u32 v = obj[0x59];
        obj[0x59] = (u8)((v | bit) | zero);
    }
}

void Scene_RunFourActorCoordinatePresentation(void)
{
    u8 *record;
    s32 mode;

    Field_unk_02009d84();
    Field_unk_020081ae(25, 0, 0);
    Field_unk_020081b8(24, 1, 0);
    Field_unk_0200818e(0x1b80000, -1, 0xa80000, 0x1000001);
    Field_unk_02008188_a(27, 0x1b8, 164, 0x5000);
    Field_unk_0200819e(8, 0x1ac, 190, 0xd000);
    Field_unk_020081ae_a(9, 0x1c4, 190, 0xb000);
    Field_unk_02009e76(9, 1);
    mode = 128;
    Field_unk_020081c6_a(0, 0x1b8, 134, 0x8000);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = (mode << 1);
    Field_unk_02009f7a();
    Field_unk_02009e4c(0, 0xcccc, 0x6666);
    Field_unk_02009e90(0, 0x198, 134);
    Field_unk_02009e9c(0, 0x198, 148);
    Field_unk_02009ea8(0, 0x1a8, 148);
    Field_unk_02009f54(0, 0x4000, 20);
    Field_unk_02009f0c(27, 1);
    Field_unk_02009f3a(0x1e27);
    Field_unk_02008204(27);
    Field_unk_02009f20(8, 1);
    Field_unk_02008212(8);
    Field_unk_02009f0e(27, 3);
    Field_unk_02008220(27);
    Field_unk_0200823c(27, 0xd000);
    record = Pointer1(Field_unk_02009eaa, 0);
    if (record != 0) {
        Field_unk_02009f18(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk_02009ed2(1, 0xcccc, 0x6666);
    Field_unk_02009f14(1, 0x1b8, 148);
    Field_unk_02009fc0(1, 0x4000, 0);
    record = Pointer1(Field_unk_02009ede, 1);
    if (record != 0) {
        Field_unk_02009f4c(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk_02009f06(2, 0xcccc, 0x6666);
    Field_unk_02009f4a(2, 0x1c8, 148);
    Field_unk_02009ff6(2, 0x4000, 0);
    record = Pointer1(Field_unk_02009f14_a, 2);
    if (record != 0) {
        Field_unk_02009f82(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_unk_02009f3c(3, 0xcccc, 0x6666);
    Field_unk_02009f80(3, 0x1d8, 148);
    Field_unk_0200a02e(3, 0x4000, 20);
    Field_unk_02008352(0, 0, 60);
    Field_unk_0200835c_a(1, 0x4000, 20);
    Field_unk_02008366_a(2, 1, 20);
    Field_unk_0200a058(27, 0x5000, 20);
    Field_unk_020082fa(27);
    Field_unk_0200a00e(9, 1);
    Field_unk_0200a088(9, (mode << 1), 40);
    Field_unk_02008312_b(9);
    Field_unk_0200a026(1, 3);
    Field_unk_0200a0a0(1, 0x103, 60);
    Field_unk_0200a020(27, 3);
    Field_unk_02008332(27);
    Field_unk_0200a04e(10, 1);
    Field_unk_0200a026_a(10, 3);
    Field_unk_02008348(10);
    Field_unk_0200a034(8, 3);
    Field_unk_0200a03c(9, 3);
    Field_unk_0200a044_a(11, 3);
    Field_unk_0200a04c_a(12, 3);
    Field_unk_0200a064(13, 3);
    Field_unk_020083f6(0, 0, 40);
    Field_unk_02008400(2, 1, 0);
    Field_unk_0200840a_a(1, 0x4000, 20);
    Field_unk_0200a08a_a(27, 4);
    Field_unk_0200839c(27);
    Field_unk_0200a124(8, 0x102, 60);
    Field_unk_0200a0bc(8, 1);
    Field_unk_020083b6_a(8);
    Field_unk_0200a0b2(27, 3);
    Field_unk_020083c4(27);
    Field_unk_0200a132(8, 0, 0);
    Field_unk_0200a13c(9, 0x8000, 40);
    Field_unk_0200a160(8, 0x102, 0);
    Field_unk_0200a16c(8, 0x102, 40);
    Field_unk_0200a10c(27, 1);
    Field_unk_0200a0e4(27, 3);
    Field_unk_0200a15e(27, 0, 20);
    Field_unk_0200a0f6(8, 3);
    Field_unk_0200a10e(9, 3);
    Field_unk_0200a074(40);
    Field_unk_0200a1a6(9, (mode << 1), 20);
    Field_unk_02008446(9, 0xb000);
    Field_unk_02008438(9);
    Field_unk_02008456(27, 0x3000);
    Field_unk_0200a1c8(27, 0x101, 60);
    Field_unk_0200a1aa(27, 0, 60);
    Field_unk_0200a1de(27, 0x106, 20);
    Field_unk_0200847e(27, 0xb000);
    Field_unk_0200a166(27, 3);
    Field_unk_02008478(27);
    Field_unk_020084fe(3, 2, 80);
    Field_unk_0200849e(8, 0xd000);
    Field_unk_0200a19e(8, 2);
    Field_unk_02008498(8);
    Field_unk_0200a194(9, 3);
    Field_unk_0200a1b4(9, 2);
    Field_unk_020084ae(9);
    Field_unk_020084ca(27, 0x5000);
    Field_unk_0200a1b2_b(27, 3);
    Field_unk_0200a1da(27, 1);
    Field_unk_020084cc(27);
    Field_unk_0200a162(27, 0xcccc, 0x6666);
    Field_unk_0200a1a6_a(27, 0x198, 158);
    Field_unk_0200a1b2_a(27, 0x198, 148);
    Field_unk_0200a25c(27, 0, 20);
    Field_unk_0200a214(27, 1);
    Field_unk_02008506(27);
    Field_unk_0200858c(1, 0x8000, 20);
    Field_unk_02008596(2, 1, 0);
    Field_unk_0200a1ea(27, 0x198, 134);
    Field_unk_0200a1ec_a(27, 0x1b8, 134);
    Field_unk_0200a18a(40);
    Field_unk_020085dc_a(9, 10, 0);
    Field_TestFlag926(0x926);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_unk_0200a1dc();
    Field_unk_02008606(15, 0, 1);
    Field_unk_0200a2a6(8, 1);
    Field_unk_0200a1ec(20);
    Field_unk_0200a22e(8, 0xcccc, 0x6666);
    Field_unk_0200a272(8, 0x1d4, 0x266);
    Field_unk_0200a280(8, 0x1d8, 0x254);
    Field_unk_0200a32c(8, 0x8000, 20);
    Field_unk_0200a2d6(8, 4, 20);
    rec7 = Field_unk_0200833a();
    Field_unk_0200a232(20);
    Field_unk_0200a3d8(214);
    Field_unk_0200a1d8(rec7, 0x200e738);
    Field_unk_0200a246(40);
    Field_unk_0200a2ee(8, 3);
    Field_unk_0200a254(20);
    Field_unk_0200a2d2(8, 0x1d2, 0x270);
    Field_unk_0200862c(8, 0x5000);
    Field_unk_0200a32c_a(8, 2);
    Field_unk_0200a362(0x1e3b);
    Field_unk_0200a384(8, 0, 20);
    Field_unk_020086b6(9, 11, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk_0200a2b8();
    Field_unk_020086e2(15, 1, 1);
    Field_unk_0200a3d6(8, 0x5000, 40);
    Field_unk_0200a386(8, 2);
    Field_unk_0200a3bc(0x1e3d);
    Field_unk_0200a3de(8, 0, 20);
    Field_unk_02008710(9, 11, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk_0200a300();
    Field_unk_0200872a(24, 0, 0);
    Field_unk_02008734(18, 0, 0);
    Field_unk_0200a39e(0, 0, 0);
    Field_unk_0200a3aa(16, 0x960000, 0x24a0000);
    Field_unk_02008720(0x9c0000, -1, 0x2180000, 0x1000001);
    Field_unk_02008766(8, 0, 0);
    Field_unk_0200a380(16, 0xcccc, 0x6666);
    Field_unk_0200a3c2(16, 168, 0x242);
    Field_unk_0200a3cc(16, 168, 0x22a);
    Field_unk_0200a478(16, 0x8000, 20);
    Field_unk_0200a428(16, 2);
    Field_unk_0200a45e(0x1e3c);
    Field_unk_0200a480(16, 0, 20);
    Field_unk_020087b2(9, 12, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk_0200a3b8();
    Field_unk_0200a442(0, 0, 0);
    Field_unk_02008f02();
    Field_unk_0200a452(18, 0x960000, 0x24a0000);
    Field_unk_020087c8(0x9c0000, -1, 0x2180000, 0x1000001);
    Field_unk_0200880e(8, 0, 0);
    Field_unk_0200a428_a(18, 0xcccc, 0x6666);
    Field_unk_0200a46a(18, 168, 0x242);
    Field_unk_0200a474(18, 168, 0x22a);
    Field_unk_0200a520(18, 0x8000, 20);
    Field_unk_0200a4d0(18, 2);
    Field_unk_0200a506(0x1e3c);
    Field_unk_0200a528(18, 0, 20);
    Field_unk_0200885a(9, 12, 0);
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

    Field_unk_0200a460();
    Field_unk_0200a4f0(9, 5);
    Field_unk_02008892(24, 1, 0);
    Field_unk_0200a4fc(0, 0, 0);
    Field_unk_020088a6(17, 0, 0);
    Field_unk_02008634(0);
    Field_unk_020088b6(8, 1, 20);
    Field_unk_0200a5ce(0x6666, 0xccc);
    Field_unk_0200a5e8(0x1b80000, -1, 0xb00000, 1);
    Field_unk_0200a4ae(20);
    Field_unk_0200a546(9, 7);
    Field_unk_0200a4bc(30);
    Field_unk_0200a662(0xbc);
    Field_unk_0200a4c8(30);
    Field_unk_0200867e(16);
    Field_unk_0200a4d4(0x50);
    Field_unk_0200868a(0);
    Field_unk_0200a4e0(0x3c);
    Field_unk_0200a578_a(9, 7);
    Field_unk_0200a4ee(30);
    Field_unk_0200a694(0xbc);
    Field_unk_0200a4fa(30);
    Field_unk_020086b0(16);
    Field_unk_0200a506_a(0x50);
    Field_unk_020086bc(0);
    Field_unk_0200a512(0x5a);
    Field_unk_0200a6b8(0xbc);
    Field_unk_0200a51e(30);

    *(u32 *)(gWork + (224 << 1)) = (224 << 1) + 67;

    Field_unk_0200895e(9, 0, 0);

    if (Field_unk_0200a51c((s32)&Value_0000092b) != 0) {
        Field_unk_0200a69e(20);
    } else if (Field_unk_0200a52e((s32)&Value_0000092a) != 0) {
        Field_unk_0200a6b0(18);
    } else if (Field_unk_0200a540((s32)&Value_00000929) != 0) {
        Field_unk_0200a6c2(17);
    } else if (Field_unk_0200a552((s32)&Value_00000928) != 0) {
        Field_unk_0200a6d4(16);
    } else {
        Field_unk_0200a6dc(13);
    }
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk_0200a5b4();
    Field_unk_020089de(15, 1, 1);
    Field_unk_0200a67e(8, 1);
    Field_unk_0200a5c4(10);
    Field_unk_0200a6e0(8, 0x3000, 20);
    Field_unk_0200a690(8, 2);
    Field_unk_0200a6c6((s32)gVal);
    Field_unk_0200a6e8(8, 0, 20);
    Field_unk_02008a1a(9, 14, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_unk_0200a608();
    Field_unk_0200a752(-1, -1, -1, 0);
    Field_unk_0200a578(1);
    Field_unk_02008a4a(15, 1, 1);
    Field_unk_0200a6ea(8, 1);
    Field_unk_0200a718(0x1e43);
    Field_unk_020089e2(8);
    Field_unk_0200a752_a(8, 0xd000, 40);
    Field_unk_02008a74(9, 15, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_200e8e4;

    Field_unk_0200a664();
    Field_unk_02008a8e(24, 1, 0);
    Field_unk_02008a98(25, 1, 0);
    Field_unk_02007968();
    Field_unk_02008aa6(19, 11, 12);
    Field_unk_0200a716(10, 6);
    Field_unk_0200a6ce(12, 0x200e840);
    base5_200e8e4 = (s32)gOv15;
    Field_unk_0200a6d8(36, base5_200e8e4);
    Field_unk_0200a6e0_a(37, base5_200e8e4);
    Field_unk_0200a6e8_a(38, base5_200e8e4);
    Field_unk_0200a788(36, 3);
    Field_unk_0200a790(37, 3);
    Field_unk_0200a798(38, 3);
    Field_unk_020092e8();
    Field_unk_0200a6d8_a();
}

/*
 * A flat setter sequence, no branches; the final call takes no arguments.
 * The 60-byte owner at 0x02004218 includes its one pool word, the address
 * taken as Value_00000928.
 */
void Scene_RunActors24And25Setup(void)
{
    extern u8 *gWork;

    Field_unk_0200a6e4();
    Field_unk_02008b0e(24, 1, 0);
    Field_unk_02008b18(25, 0, 0);
    Field_unk_02008b22(19, 11, 12);
    Field_unk_02009532();
    Field_unk_0200a6ec((s32)&Value_00000928);
    Field_unk_0200a718_a();
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

    if (Field_unk_0200ab18((s32)&Value_00000928) != 0) {
        u8 *obj = Field_unk_0200962c(0, 0);
        Field_unk_0200abda(obj, 0xcd << 17, 0xac << 16);
        Field_unk_02008f84(7, obj, src);
        Field_unk_0200abee(10, 0, 0);
    } else {
        Field_unk_02008f9a(5, 10, src);
    }

    if (Field_unk_0200ab58((s32)&Value_00000929) != 0) {
        u8 *obj = Field_unk_0200966c(1, 0);
        Field_unk_0200ac1a(obj, 0xeb << 17, 0xac << 16);
        *(u32 *)(Field_unk_0200abc0(obj) + 24) = 0xffff0000;
        Field_unk_02008fce(7, obj, src);
        Field_unk_0200ac38(11, 0, 0);
    } else {
        Field_unk_02008fe4(6, 11, src);
    }

    if (Field_unk_0200aba2((s32)&Value_0000092a) != 0) {
        u8 *obj = Field_unk_020096b6(2, 0);
        Field_unk_0200ac64(obj, 0xcd << 17, 0xcc << 16);
        Field_unk_0200900e(7, obj, src);
        Field_unk_0200ac78(12, 0, 0);
    } else {
        Field_unk_02009024(5, 12, src);
    }

    if (Field_unk_0200abe2((s32)&Value_0000092b) != 0) {
        u8 *obj = Field_unk_020096f6(3, 0);
        Field_unk_0200aca4(obj, 0xeb << 17, 0xcc << 16);
        *(u32 *)(Field_unk_0200ac4a(obj) + 24) = 0xffff0000;
        Field_unk_02009058(7, obj, src);
        Field_unk_0200acc2(13, 0, 0);
    } else {
        Field_unk_0200906e(6, 13, src);
    }

    Field_unk_02009078(5, 14, src);
    Field_unk_02009082(6, 15, src);
    Field_unk_0200908c(5, 16, src);
    Field_unk_02009096(6, 17, src);
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
    Field_unk_0200ae2a(a, 0);
    Field_unk_0200ad38(10);
}

void Scene_CallPairWith10(s32 a, u16 b)
{
    Field_unk_0200ae5a(a, b, 10);
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
 * call but not masked, so they stay full `s32`. `Field_unk_0200ad8a` returns a
 * pointer whose halfword at offset 6 is then set to `d`.
 */
void OvObj_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d)
{
    Field_unk_0200ade4(a, b << 16, c << 16, d);
    *(s16 *)((u8 *)Field_unk_0200ad8a(a) + 6) = d;
}

void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags)
{
    u32 selected;

    Field_SetMode(x, y, z, ~flags & 1);
    selected = flags & 0x1111;
    if ((flags & 0x10000000) != 0)
        Field_unk_0200aed0();
    if ((flags & 0x01000000) != 0)
        Field_unk_0200ad3e();
    Field_unk_0200ad9c(selected);
}

void Scene_RunSceneStep(s32 step, u32 arg, u32 opt)
{
    extern const s32 gOv26[];
    extern u8 gWork[];

    u32 slot;

    switch (step) {
    case 0:
        Field_unk_0200af44(0, 0, 0);
        Field_unk_0200af50(1, 0x8000, 0);
        Value(2, 0, 0);
        Field_unk_0200af8a(3, 0x8000, opt);
        break;
    case 1:
        Field_unk_0200af6c(0, arg, 0);
        Field_unk_0200af76(1, arg, 0);
        Field_unk_0200af80(2, arg, 0);
        Field_unk_0200af8a(3, arg, opt);
        break;
    case 2:
        Field_unk_0200af14(0, 3);
        Field_unk_0200af1c(1, 3);
        Field_unk_0200af24(2, 3);
        Field_unk_0200af2c(3, 3);
        if (arg != 0) {
            Field_unk_0200af4e(3);
        }
        if (opt == 0) {
            break;
        }
        Field_unk_0200aedc(opt);
        break;
    case 3:
        Field_unk_0200afe8(0, 0x102);
        Field_unk_0200aff2(1, 0x102);
        Field_unk_0200affc(2, 0x102);
        Field_unk_0200b006(3, 0x102);
        Field_unk_0200aedc(opt);
        break;
    case 4:
        for (slot = 0; slot < arg; slot++) {
            Field_unk_0200af7c(slot + 10, 0, 0);
        }
        break;
    case 5:
        {
            u8 *rec;

            rec = Field_unk_0200af28(arg);
            SetPose(rec, 0x5000);
        }
        Field_unk_0200afb2(arg, 5);
        Field_unk_0200afc2(arg, opt);
        break;
    case 6:
        {
            u8 *rec;

            rec = Field_unk_0200af38(arg);
            SetPose(rec, 0x5000);
            *(s32 *)(rec + 24) = -0x10000;
        }
        Field_unk_0200afb2(arg, 5);
        Field_unk_0200afc2(arg, opt);
        break;
    case 7:
        {
            u8 *rec;

            rec = Value4(arg);
            SetPose(rec, 0x5000);
        }
        Field_unk_0200924a(arg);
        if (opt == 0) {
            Field_unk_0200afe4(arg, 0);
        }
        break;
    case 8:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x202;
        Field_unk_0200b0d0();
        if (arg != 0) {
            Value5();
        }
        Field_unk_0200af6e(0);
        break;
    case 9:
        Field_unk_0200b0ec();
        Field_unk_0200b0f8();
        if (arg == 0) {
            break;
        }
        Field_unk_0200b0dc(arg);
        break;
    case 10:
        Field_unk_020093b8(24, 1, 0);
        Field_unk_020093c2_a(25, 0, 0);
        Field_unk_020082ba(0);
        Field_unk_02009382_a(0, 0x1b0, 168, 0x4000);
        Field_unk_02009390_a(1, 0x1c0, 168, 0x4000);
        Field_unk_0200939e_a(2, 0x1a8, 152, 0x4000);
        Field_unk_020093ac(3, 0x1ca, 152, 0x4000);
        break;
    case 11:
        if (arg != 0) {
            u8 *rec;

            Field_unk_0200b07a(13, 1);
            rec = Field_unk_0200b018(13);
            SetPose(rec, 0x3000);
            rec = Field_unk_0200b024(13);
            *(s32 *)(rec + 24) = 0x10000;
        }
        {
            u8 *rec;

            Field_unk_0200b09a(14, 1);
            rec = Field_unk_0200b038(14);
            SetPose(rec, 0x5000);
            Field_unk_0200b0b2(15, 1);
            rec = Field_unk_0200b050(15);
            SetPose(rec, 0x3000);
            rec = Field_unk_0200b060(15);
            *(s32 *)(rec + 24) = 0x10000;
            Field_unk_0200b0d6_a(16, 1);
            rec = Field_unk_0200b074(16);
            SetPose(rec, 0x5000);
            Value6(17, 1);
            rec = Field_unk_0200b086(17);
            SetPose(rec, 0x3000);
            rec = Field_unk_0200b090(17);
            *(s32 *)(rec + 24) = 0x10000;
        }
        Field_unk_0200b100(28, 0x19a0000, 0xae0000);
        Field_unk_0200b10e(29, 0x1d60000, 0xae0000);
        Field_unk_0200b11c(30, 0x19a0000, 0xce0000);
        Field_unk_0200b12a(31, 0x1d60000, 0xce0000);
        Field_unk_0200b138(32, 0x19a0000, 0x11e0000);
        Field_unk_0200b146(33, 0x1d60000, 0x11e0000);
        Field_unk_0200b154(34, 0x19a0000, 0x13c0000);
        Field_unk_0200b162(35, 0x1d60000, 0x13c0000);
        Field_unk_0200b040(1);
        if (arg != 0) {
            Field_unk_0200b200(13, 0xb000, 0);
        }
        Value(14, 0xd000, 0);
        Field_unk_0200b218(15, 0xb000, 0);
        Field_unk_0200b224(16, 0xd000, 0);
        Field_unk_020094de(17, 0xb000);
        break;
    case 12:
        {
            u8 *rec;

            rec = Field_unk_0200b14e(arg);
            Field_unk_0200b1c0(arg, 1);
            if (opt != 0) {
                SetPose(rec, 0x3000);
            } else {
                SetPose(rec, 0x5000);
            }
            *(s32 *)(rec + 24) = 0x10000;
        }
        break;
    case 13:
        Field_unk_0200b1da(9, 0, 0);
        Field_unk_0200b1e4(12, 0, 0);
        Field_unk_0200b1ee(11, 0, 0);
        Field_unk_0200b1f8(13, 0, 0);
        Field_unk_0200b21e(10, 0, 0);
        break;
    case 14:
        Field_unk_0200b214(14, 0, 0);
        Field_unk_0200b21e(13, 0, 0);
        break;
    case 15:
        Field_unk_020095ca(24, 1, 0);
        Field_unk_0200b234(9, 0, 0);
        Field_unk_0200b23e(10, 0, 0);
        Field_unk_02009596(8, 0x1bc, 0x266, 0xd000);
        Field_unk_0200b258(0, 0, 0);
        if (arg != 0) {
            Field_unk_020092e8_a();
        }
        Field_unk_020095d6(0x1c00000, 0x200000, 0x2700000, 0x1000001);
        if (opt == 0) {
            break;
        }
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x202;
        Field_unk_0200b372();
        Field_unk_0200b386();
        Value7(20);
        break;
    case 16:
        Field_unk_0200b2a0(8, 0, 0);
        Field_unk_0200b2aa(9, 0, 0);
        Field_unk_0200b2b8(27, 0x1b60000, 0x980000);
        break;
    case 17:
        slot = 0;
        do {
            Field_unk_0200b2ca(slot + 28, 0, 0);
            slot++;
        } while (slot <= 7);
        break;
    case 18:
        Field_unk_0200962a(12, 152, 0x214, 0xb000);
        Field_unk_0200963c(8, 134, 0x1ea, 0x3000);
        Field_unk_0200964a(9, 166, 0x1ea, 0x5000);
        Field_unk_0200965a(10, 182, 0x1f8, 0x5000);
        Field_unk_02009666(11, 118, 0x1f8, 0x3000);
        Scene_RunSceneStep(14, 0, 0);
        break;
    case 19:
        Field_unk_0200967e(8, 0x1a0, 0x148, 0);
        Field_unk_02009694(9, 0x1c0, 0x160, 0xd000);
        Field_unk_020096aa(10, 0x1c6, 248, 0x3000);
        Field_unk_020096ba(arg, 0x198, 0x122, 0);
        Field_unk_020096c8(opt, 0x198, 0x156, 0);
        Field_unk_020096d8(13, 0x1a4, 0x164, 0xd000);
        Field_unk_020096e6(14, 0x198, 0x130, 0);
        Field_unk_020096f6_a(15, 0x1a2, 0x17a, 0xd000);
        Field_unk_02009706(16, 0x1b8, 0x106, 0x3000);
        Field_unk_02009712(17, 0x1c0, 0x17a, 0xd000);
        break;
    case 20:
        for (slot = arg; slot <= opt; slot++) {
            Field_unk_0200b342(slot);
        }
        break;
    case 21:
        Field_unk_0200978a(20, 0x92c, 0x93d);
        Field_unk_02009794(20, 0x917, 0x91f);
        Field_unk_020097a0(20, 0x990, 0x998);
        Field_unk_0200b370(0x300);
        Field_unk_0200b376(0x301);
        Field_unk_0200b37c(0x302);
        break;
    case 22:
        Field_unk_0200b2f4(1);
        Field_unk_020097c6(23, 0, 0);
        Field_unk_0200b3e6(12, gOv26);
        break;
    case 23:
        Field_unk_0200b3de(1);
        Field_unk_0200b3e4(2);
        Field_unk_0200b3ea(3);
        break;
    case 24:
        Field_unk_0200b50e(-1, -1, -1, 0);
        Field_unk_0200b334(1);
        if (arg != 0) {
            *(u8 *)(Field_unk_0200b52c() + 0x55) = 0;
        }
        break;
    case 25:
        Field_unk_0200b402(gOv22);
        Field_unk_0200b350(1);
        if (arg == 1) {
            Field_unk_0200b41a(gOv23);
            Field_unk_0200b368(1);
        } else if (arg == 2) {
            Field_unk_0200b41a(gOv24);
            Field_unk_0200b368(1);
        } else if (arg == 3) {
            Field_unk_0200b42c(gOv25);
            Field_unk_0200b37a(1);
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
    Field_unk_0200a044(8, 0x92c);
    Field_unk_0200a04c(8, 0x935);
    Field_unk_0200a054(8, 0x917);
    Field_unk_0200a05e(8, 0x990);
}

/*
 * Scan slots 0 through 8 inclusive.  On the first a1 that Field_unk_0200b4e4
 * accepts, call Field_unk_0200b59a and stop.  a0 and a1 advance together.
 */
void Actor_RunFirstMatchingSlot(s32 a0, s32 a1)
{
    unsigned int i = 0;

    do {
        if (Field_unk_0200b4e4(a1)!= 0) {
            Field_unk_0200b59a(a0, 0, 0);
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

    rec8 = Field_unk_0200a018(0, 0);
    Field_unk_0200b53e();
    Field_unk_02009968(24, 1, 0);
    Field_unk_02009972(25, 2, 0);
    Field_unk_02008842();
    Field_unk_02009980(19, rec8, 12);
    Field_unk_0200b5f0(10, 6);
    base5_200e840 = (s32)gOv26;
    Field_unk_0200b5aa(rec8, base5_200e840);
    Field_unk_0200b5a0(11);
    Field_unk_0200b5b8(12, base5_200e840);
    base5_200e8e4 = (s32)gOv15;
    Field_unk_0200b5c2(36, base5_200e8e4);
    Field_unk_0200b5ca(37, base5_200e8e4);
    Field_unk_0200a1b2();
    Field_unk_0200b5a2();
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

    Field_unk_020099a6(0x1b80000, -1, 0xb00000, 0x1000001);
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
    Field_unk_0200b5b6(1); /* main:080000c0 */
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
    Field_unk_0200b7ee(10); /* main:0808a1d8 */
    /* Text/dialog resource pointer, passed by base address and by base+1. */
    base5_1e46 = (s32)gVal2;
    Field_unk_0200b6b2(base5_1e46, 1, 10); /* main:08015210 */
    Battle_WaitMode0_2(10);
    Field_unk_02009b1a(0, 0, 40);
    Field_unk_02009b26(1, 0x4000, 20);
    Motion_SetSpeedLim_1(0x39999, 0x7333); /* main:0808a208 */
    Field_unk_02009b00(0x1b80000, -1, 0x1400000, 0x10000014);
    Motion_SetVarCbObj_1(8, 2); /* main:0808a138 */
    Field_unk_02009ae8(8, 0xd000);
    SceneWork_SetStepValue_1((base5_1e46 + 1)); /* main:0808a170 */
    Field_unk_02009ae0(8);
    Motion_ArmCb_5(8, 0, 20);
    Field_unk_02009b3a(0x1b80000, -1, 0x860000, 0x10000000);
    /* Second text/dialog resource pointer, shared across three calls. */
    base5_200e7c8 = (s32)gOv16;
    Motion_EnableActCb_1(1, base5_200e7c8);
    Motion_EnableActCb_2(2, base5_200e7c8);
    Motion_MarkActiveAndSetActionCallback_1(3, base5_200e7c8); /* main:0808a0b0 */
    Battle_WaitMode0_3(40);
    GameFlag_Set_1(0x301);
    Field_unk_02009ba6(23, 0, 0);
    GameFlag_Clear_1(0x12f);
}

void Scene_RunFormationAndEffectPresentation(void)
{
    s32 action;

    Field_unk_02009ba2(0, 0x1bc, 0x12c, 0);
    Field_unk_02009bb2(1, 0x1ca, 0x136, 0);
    Field_unk_02009bc0(2, 0x1bc, 0x14a, 0);
    Field_unk_02009bd2(3, 0x1b0, 0x136, 0);
    Field_unk_02009be2(27, 0x1b8, 134, 0x8000);
    Field_unk_02009bf6(10, 0x1c6, 248, 0x3000);
    Field_unk_0200b8c0(10, 6);
    Field_unk_02009c2e(0x1b80000, -1, 0x1340000, 0x1000001);
    Field_unk_0200b9b6();
    Field_unk_0200b9ca();
    Field_unk_0200b850(20);
    Field_unk_0200b96c(0, 0x2000, 0);
    Field_unk_0200b978(1, 0xa000, 0);
    Field_unk_0200b984(2, 0xa000, 0);
    Field_unk_0200b990(3, 0x2000, 40);
    Field_unk_02009cb2_a(2, 1, 20);
    Field_unk_0200b978_a(0x1e6e);
    Field_unk_02009c42(27);
    Field_unk_02009cce(1, 0xc000, 0);
    Field_unk_0200b9e6(0x26666, 0x4ccc);
    Field_unk_0200b9fc(0x1b80000, -1, 0xb00000, 1);
    Field_unk_0200b8fe(27, 0x19999, 0xcccc);
    Field_unk_0200b942(27, 0x198, 134);
    Field_unk_0200b94e(27, 0x198, 152);
    Field_unk_0200b95a(27, 0x1a8, 164);
    Field_unk_0200ba2a(0x19999, 0x3333);
    Field_unk_0200ba40(0x1b80000, -1, 0x12c0000, 1);
    Field_unk_0200b97c(27, 0x1a8, 222);
    Field_unk_0200b98a(27, 0x1a8, 0x106);
    Field_unk_0200ba34(27, 0x3000, 20);
    Field_unk_0200b9ec(27, 1);
    Field_unk_02009cde(27);
    Field_unk_02009d64(2, 1, 20);
    Field_unk_0200b9e4(27, 3);
    Field_unk_0200ba0c(27, 1);
    Field_unk_02009cfe(27);
    Field_unk_02009d84_a(3, 2, 60);
    Field_unk_02009d90(1, 0xe000, 60);
    Field_unk_0200ba82(27, 0, 40);
    Field_unk_0200ba3a(27, 1);
    Field_unk_0200ba12(27, 2);
    Field_unk_0200b9ee(27, 0x1b0, 0x10c);
    Field_unk_0200b9fc_a(27, 0x1c4, 0x10c);
    Field_unk_0200ba34_a(27, 1);
    Field_unk_02009d6e(27, 0xd000);
    Field_unk_0200ba6e(27, 2);
    Field_unk_0200bac0(27, 0, 20);
    Field_unk_02009df2(1, 0xc000, 20);
    Field_unk_0200ba72(27, 4);
    Field_unk_0200b9d8(40);
    Field_unk_0200bae2(27, 0, 80);
    Field_unk_0200baaa(27, 1);
    Field_unk_0200b9f0(20);
    Field_unk_0200ba98(27, 3);
    Field_unk_0200b9fe(10);
    Field_unk_02009dc8(27, 0x5000);
    Field_unk_0200bb00(27, 0);
    if (Field_unk_0200ba38(0, 0) == 0) {
        Field_unk_0200bac4(27, 3);
        Field_unk_02009dd6(27);
    } else {
        Field_unk_0200bad4(27, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Field_unk_02009df6(27);
        Field_unk_02009e7c(3, 2, 40);
        Field_unk_0200bb1c(27, 1);
        Field_unk_0200baf4(27, 3);
        Field_unk_02009e16(27);
    }
    Field_unk_02009e9c_a(2, 1, 20);
    action = (s32)gOv20;
    Field_unk_0200babe(1, action);
    Field_unk_0200bac6(2, action);
    Field_unk_0200bade(3, action);
    Field_unk_0200bbce(0x9999, 0x1333);
    Field_unk_0200bbe8(0x1b80000, -1, 0xb00000, 1);
    Field_unk_0200baee(0, 0x10000, 0x8000);
    Field_unk_0200bb34(0, 0x1a8, 0x110);
    Field_unk_0200bb38(0, 0x1a8, 164);
    Field_unk_0200bad6(60);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    Field_unk_02009f16(9, 0, 0);
    Field_TestFlag301(0x301);
    Field_TestFlag927(0x927);
    Field_unk_0200bc58(4);
}

void Scene_RunActors24And25SetupWithValue929(void)
{
    extern u8 *gWork;

    s32 handle = Field_unk_0200a634(0, 0);

    Field_unk_0200bb5a();
    Field_unk_02009f84(24, 1, 0);
    Field_unk_02009f8e(25, 0, 0);
    Field_unk_02008e84(0);
    Field_unk_02009f9e(19, handle, 12);
    Field_unk_0200bc08(11, 0, 0);
    Field_unk_0200a9b8();
    Field_unk_0200bb72((s32)&Value_00000929);
    Field_unk_0200bb9e();
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

    rec8 = Field_unk_0200a690_a(0, 0);
    rec2 = Field_unk_0200a69a(1, 0);
    Field_unk_0200bbc0();
    Field_unk_02009fea(24, 1, 0);
    Field_unk_02009ff4(25, 3, 0);
    Field_unk_02008ec4();
    Field_unk_0200a002(19, rec8, rec2);
    Field_unk_0200bc72(10, 6);
    base5_200e840 = (s32)gOv26;
    Field_unk_0200bc2c(rec8, base5_200e840);
    Field_unk_0200bc22(11);
    Field_unk_0200bc3a(rec2, base5_200e840);
    Field_unk_0200bc30(12);
    base5_200e8e4 = (s32)gOv15;
    Field_unk_0200bc4a(36, base5_200e8e4);
    Field_unk_0200bc52(37, base5_200e8e4);
    Field_unk_0200bcf2(36, 3);
    Field_unk_0200bcfa(37, 3);
    Field_unk_0200a84a();
    Field_unk_0200bc3a_a();
}

/*
 * A flat setter sequence, no branches. The 108-byte owner at 0x02005780
 * includes its one pool word, the address taken as Value_0000092a.
 */
void Scene_RunActors24And25SetupWithValue92a(void)
{
    extern u8 *gWork;

    s32 handle = Field_unk_0200a730(0, 0);
    s32 other = Field_unk_0200a73a(1, 0);

    Field_unk_0200bc60();
    Field_unk_0200a08a(24, 1, 0);
    Field_unk_0200a094(25, 0, 0);
    Field_unk_02008f8a(0);
    Field_unk_0200a0a4(19, handle, other);
    Field_unk_0200bd0e(11, 0, 0);
    Field_unk_0200bd18(12, 0, 0);
    Field_unk_0200aac8();
    Field_unk_0200bc82((s32)&Value_0000092a);
    Field_unk_0200bcae();
}

void Scene_RunExtendedFormationPresentation(void)
{
    s32 slot_b;
    s32 slot_c;
    s32 slot_a;
    u8 *record;
    s32 action;

    slot_a = Field_unk_0200a7a2(0, 0);
    slot_b = Field_unk_0200a7ac(1, 0);
    slot_c = Field_unk_0200a7b6(2, 0);
    Field_unk_0200bcdc();
    Field_unk_0200a106(10, 0, 0);
    Field_unk_0200a110(17, 0, 0);
    Field_unk_0200bd7e(8, 0x1d80000, 0x980000);
    Field_unk_0200bd8e(9, 5);
    Field_unk_0200bd94(27, 0x1b80000, 0x860000);
    Field_unk_0200bdec(27, 15);
    record = Field_unk_0200bd42(27);
    Field_unk_0200bce0(record, 0);
    Field_unk_02009ed8(16);
    Field_unk_0200a126(0x1b60000, -1, 0xae0000, 0x1000001);
    Field_unk_0200a16c_a(8, 1, 20);
    Field_unk_0200beea(19);
    Field_unk_0200bef0(181);
    Field_unk_0200bd28(0x20000, 0x20000, 0x10000);
    Field_unk_0200bd66(10);
    Field_unk_0200bd38(-1, -1, 0xe666);
    Field_unk_0200bd76(80);
    Field_unk_0200bf1c(181);
    Field_unk_0200bd54(0x20000, 0x20000, 0x10000);
    Field_TestFlag11a(10);
    Field_unk_0200bd64(-1, -1, 0xe666);
    Field_unk_0200bf42(63);
    Field_TestFlag11a(0x11a);
    Field_unk_0200bee6(3, 0x102);
    Field_unk_0200bdbe(40);
    Field_unk_0200a186(3, 0x6000);
    Field_unk_0200beb4(0x1ec1);
    Field_unk_0200bed6(3, 0, 40);
    Field_unk_0200a188(27);
    Field_unk_0200bef8(0, 0x2000, 0);
    Field_unk_0200bf04(1, 0xa000, 0);
    Field_unk_0200bf0e(2, 0, 0);
    Field_unk_0200bf1a(3, 0xe000, 40);
    Field_unk_0200bf24(0, 0x6000, 0);
    Field_unk_0200bf30(1, 0xe000, 0);
    Field_unk_0200bf3a(2, 0x6000, 0);
    Field_unk_0200bf46(3, 0x8000, 40);
    Field_unk_0200bf52(2, 0xe000, 0);
    Field_unk_0200bf76(2, 0x100, 60);
    Field_unk_0200a218(2, 0x2000);
    Field_unk_0200bf20(2, 1);
    Field_unk_0200a212(2);
    Field_unk_0200bf26(0, 1);
    Field_unk_0200bf2e(1, 1);
    Field_unk_0200bf3e(3, 1);
    Field_unk_0200be84(10);
    Field_unk_0200bfa0(0, 0xc000, 0);
    Field_unk_0200bfac(1, 0xc000, 0);
    Field_unk_0200bfb8(2, 0xe000, 0);
    Field_unk_0200bfc4(3, 0xa000, 20);
    Field_unk_0200bf94(27, 0);
    record = Field_unk_0200beea_a(27);
    Field_unk_0200be88(record, 1);
    Field_unk_0200bf0e_a(27, 0x10000, 0x8000);
    Field_unk_0200bf52_a(27, 0x1ae, 134);
    Field_unk_0200a2ac(27, 0x3000);
    Field_unk_0200bfac_a(27, 2);
    Field_unk_0200a2a6_a(27);
    Field_unk_0200bfba(slot_a, 1);
    Field_unk_0200bfc2(slot_b, 1);
    Field_unk_0200bfca(slot_c, 1);
    Field_unk_0200bfda(13, 1);
    Field_unk_0200c054(slot_a, 0x102);
    Field_unk_0200c05e(slot_b, 0x102);
    Field_unk_0200c068(slot_c, 0x102);
    Field_unk_0200c072(13, 0x102);
    Field_unk_0200bf48(40);
    Field_unk_0200a37a(12, slot_a, 0);
    Field_unk_0200a384_a(12, slot_b, 1);
    Field_unk_0200a38e(12, slot_c, 0);
    Field_unk_0200a398(11, 1, 0);
    Field_unk_0200c08c(slot_a, 0xd000, 0);
    Field_unk_0200c098(slot_b, 0xb000, 0);
    Field_unk_0200c0a4(slot_c, 0xd000, 0);
    Field_unk_0200c0ae(0, 0, 0);
    Field_unk_0200c0ba(1, 0x8000, 0);
    Field_unk_0200c0c4(2, 0, 0);
    Field_unk_0200c0d0(3, 0x8000, 40);
    Field_unk_0200c080(27, 2);
    Field_unk_0200c0c8(27, 0);
    Field_unk_0200c0ec(0, 0xc000, 0);
    Field_unk_0200c0f8(1, 0xc000, 0);
    Field_unk_0200c104(2, 0xc000, 0);
    Field_unk_0200c110(3, 0xc000, 20);
    Field_unk_0200c07e(27, 0x1b8, 134);
    Field_unk_0200c0a2(27, 0, 0);
    Field_unk_0200a3e2(1, 0x8000);
    Field_unk_0200c0ea(1, 1);
    Field_unk_0200a3dc(1);
    Field_unk_0200c14a(2, 0, 0);
    Field_unk_0200a402(3, 0x8000);
    Field_unk_0200c0da(0, 3);
    Field_unk_0200c0e2(1, 3);
    Field_unk_0200c0ea_a(2, 3);
    Field_unk_0200c102(3, 3);
    Field_unk_0200c0a6(1, 0x10000, 0x8000);
    Field_unk_0200c0b2(2, 0x10000, 0x8000);
    Field_unk_0200c0be(3, 0x10000, 0x8000);
    action = (s32)gOv21;
    Field_unk_0200c0d0_a(1, action);
    Field_unk_0200c0d8(2, action);
    Field_unk_0200c0f0(3, action);
    Field_TestFlag302(0x302);
    *(s32 *)0x0200ff84 = 0;
    Field_unk_0200c01e((s32)Field_unk_02005c48, 0xc80);
    Field_unk_0200a4e8(23, 0, 0);
    Field_unk_0200c0f6(27);
    Field_TestFlag12f(0x12f);
    Field_TestFlag927(0x927);
    Field_unk_0200c0e6();
}

void RunActorsEightAndNineMapEvent(void)
{
    Field_unk_0200c170();
    Field_unk_0200a59a(15, 1, 0);
    Field_unk_0200a554(9, 468, 616, 32768);
    Field_unk_0200a5b6(8, 1, 20);
    Field_unk_0200c256(9, 2);
    Field_unk_0200c19c(20);
    SetDirection(8, 53248, 80);
    Field_unk_0200c2c2(8, 0, 20);
    Field_unk_0200c25a(8, 3);
    Field_unk_0200c1c0(20);
    Field_unk_0200a5f2(9, 21, 0);
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

    rec2 = Field_unk_0200b0cc(0, 0);
    rec8 = Field_unk_0200b0d6(1, 0);
    rec7 = Field_unk_0200b0e0(2, 0);
    rec4 = Field_unk_0200b0ea(3, 0);
    Field_unk_0200c610();
    Field_unk_0200aa3a(10, 0, 0);
    Field_unk_0200a9f2(8, 0x1d8, 144, 0x5000);
    Field_unk_0200aa06(27, 0x198, 142, 0x3000);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x201;
    Field_unk_0200c7b8();
    Field_unk_0200c7cc();
    Field_unk_0200c652(40);
    Field_unk_0200c71a(27, 1);
    Field_unk_0200c748(0x1f78);
    Field_unk_0200aa12(27);
    Field_unk_0200c726(rec2, 2);
    Field_unk_0200c72e(rec8, 2);
    Field_unk_0200c736(rec7, 2);
    Field_unk_0200c746(rec4, 2);
    Field_unk_0200c68c(20);
    Field_unk_0200c7a6(rec2, 0, 0);
    Field_unk_0200c7b2(rec8, 0x8000, 0);
    Field_unk_0200c7bc(rec7, 0, 0);
    Field_unk_0200c7c8(rec4, 0x8000, 40);
    Field_unk_0200c6fe(rec2, 0x10000, 0x8000);
    Field_unk_0200c70c(rec8, 0x10000, 0x8000);
    Field_unk_0200c71a_a(rec7, 0x10000, 0x8000);
    Field_unk_0200c728(rec4, 0x10000, 0x8000);
    Field_unk_0200c764(rec2, 0x1d6, 172);
    Field_unk_0200c770(rec8, 0x19a, 172);
    Field_unk_0200c77c(rec7, 0x1d6, 204);
    Field_unk_0200c790(rec4, 0x19a, 204);
    Field_unk_0200c7b8_a(rec2, 1);
    Field_unk_0200c7c0(rec8, 1);
    Field_unk_0200c7c8_a(rec7, 1);
    Field_unk_0200c854(rec8, 0xd000, 0);
    Field_unk_0200c860(rec2, 0xb000, 0);
    Field_unk_0200c86c(rec4, 0xd000, 0);
    Field_unk_0200c878(rec7, 0xb000, 20);
    Field_unk_0200c830(27, 1);
    Field_unk_0200ab22(27);
    Field_unk_0200c80e(rec2, 3);
    Field_unk_0200c816(rec8, 3);
    Field_unk_0200c81e(rec7, 3);
    Field_unk_0200c836(rec4, 3);
    Field_unk_0200ab48(27);
    Field_unk_0200c834(rec2, 3);
    Field_unk_0200c83c(rec8, 3);
    Field_unk_0200c844(rec7, 3);
    Field_unk_0200c85e(rec4, 3);
    Field_unk_0200c8da(27, 0, 0);
    Field_unk_0200ab92(0, 0x8000);
    Field_unk_0200c87a(0, 3);
    Field_unk_0200c882(27, 3);
    Field_unk_0200c826(27, 0x10000, 0x8000);
    Field_unk_0200c868(27, 0x198, 132);
    Field_unk_0200c874(27, 0x1bc, 132);
    Field_unk_0200c896(27, 0, 0);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x202;
    Field_unk_0200c992();
    Field_unk_0200c99e();
    Field_unk_0200c712(0x92c, 0x935);
    Field_unk_0200c71c(0x917, 0x990);
    Field_TestFlag8a0(0x8a0);
    Field_unk_0200c996(10);
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
        if (Field_unk_0200c876(p)!= 0) {
            Field_unk_0200c890(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (Field_unk_0200c896_a(p)!= 0) {
            Field_unk_0200c8b0(p);
            break;
        }
        cnt_b++;
    }

    Field_unk_0200c8b0_a(b + cnt_a);
    Field_unk_0200c8b8(a + cnt_b);
}
