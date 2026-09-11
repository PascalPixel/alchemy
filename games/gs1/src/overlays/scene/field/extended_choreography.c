#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/field/extended_choreography.h"

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

struct SceneActor *Field_Run38(s32 actor_id);

u8 *Field_Run39(void);

u8 *Field_Run40(s32 n);

u8 *Field_Run41(void);

u8 *Field_Run42(s32 n);

u8 *Field_Run43(void);

u8 *Field_Run44(s32 n);

u8 *Field_Run45(void);

u8 *Field_Run46(s32 n);

u8 *Field_Run47();

u8 *Field_Run48();

u8 *Field_Run49();

u8 *Field_Run50();

u8 *Field_Run51();

u8 *Field_Run52();

u8 *Field_Run53(void);

u8 *Field_Run54(s32 n);

u8 *Field_Run55();

u8 *Field_Run56(void);

u8 *Field_Run57(s32 n);

u8 *Field_Run58();

u8 *Field_Run59();
u8 *Field_Run60();

u8 *Field_Run61();

u8 *Field_Run62();

u8 *Field_Run63();

u8 *Field_Run64();

u8 *Field_Run65();

u8 *Field_Run66();

u8 *Field_Run67();

u8 *Field_Run68();

u8 *Field_Run69();

u8 *Field_Run70();

u8 *Field_Run71();

void *Field_Run72();

u8 *Field_Run73();

u8 *Field_Run74();

void Value();
u8 *Value4();

u8 *Field_Run75();
u8 *Field_Run76();
u8 *Field_Run77();

u8 *Field_Run78();
u8 *Field_Run79();
u8 *Field_Run80();

u8 *Field_Run81();
u8 *Field_Run82();

void Value5();
void Value6();

u8 *Field_Run83();

void Value_0200e5d0_b();
void Value7();

u8 *Field_Run84();

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
 * emits it last.  Field_Run55 names the in-overlay entry point, not its
 * veneer.
 */
static __inline__ void SetDirection(s32 actor, s32 direction, s32 duration)
{
    Field_Place255(actor, direction, duration);
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

s32 Field_Run85(void);      /* site 0x1086 */

void Field_Run86();         /* site 0x1092 */

void Field_Run87();         /* site 0x109e */

s32 Field_Run88();          /* site 0x107e; story flag test */

void Field_Run89();         /* site 0x108c */

void Field_Run90();         /* site 0x1098 */

void Field_Run91();         /* site 0x10a6 */

u8 *Field_Run92();          /* site 0x10ac; returns the actor record */

void Field_Run93();         /* site 0x10be */

void Field_Run94();         /* site 0x10c4 */

void Field_Run95();         /* site 0x10ce */

void Field_Run96();         /* site 0x10d2 */

s32 Field_Run97();          /* site 0x10da; story flag test */

s32 Field_Run98();          /* site 0x10ea; story flag test */

s32 Field_Run99();          /* site 0x10fa; story flag test */

void Field_Run100();         /* site 0x1108; the three selected variants share it */

void Field_Run101();         /* site 0x1114; the default variant has its own site */

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
    u32 v = (u32)(Field_Check66(o) << 6) >> 16;

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

    actor = Field_Run38(9);
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
    Field_Run102(work, 0);
    rec->mode9 = 1;
    rec->mode15 = 1;
    work->f35 = (work->f35 & ~1) | 2;
    Field_Run103(work, 15);
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
        if (Field_Check67(2208) != 0) {
            return gOv7;
        }
        if (Field_Check68(0x928) != 0 && Field_Check69(0x93e) == 0) {
            return gOv6;
        }
        return gOv5;
    case 4:
    case 23:
        if (Field_Check70(0x93e) != 0) {
            return gOv14;
        }
        return gOv11;
    case 5:
        if (Field_Check71(2208) != 0) {
            return gOv9;
        }
        if (Field_Check72(0x93e) != 0) {
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

    Field_Run104();
    Field_Run105();
    if (Field_Check(0x921) != 0) {
        Field_Do(0x1dd4);
        Field_Run106(10, 0);
    } else {
        if (Field_Check2(0x922) != 0) {
            Field_Do2(0x1d91);
            Field_Check3(10, 0);
            if (Field_Check4(0, 0) == 0) {
                Field_Run107();
                goto L_020006ea;
            }
            Field_Run108(10, 2);
            Field_Run109(10, 0);
            Field_Place(10, 0xd000, 0);
        } else {
            Field_Do3(0x1d31);
            Field_Run110(10, 0);
        }
    }
    L_020006ea:;
    Field_Run111();
}

void Dialogue_RunActor12Line(void)
{
    Field_Run112();
    Field_Do69((s32)&Value_00001dd1);
    Field_Apply2(12, 0);
    Field_Run113();
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_Run114();
    if (Field_Check5(0x928) != 0) {
        Field_Do4(0x1eb2);
        Field_Run115(8);
        Field_Place2(8, 0xd000, 60);
        Field_Run116(8, 4);
        Field_Run117(8);
        Field_Run118(8, 3);
    } else {
        if (Field_Check6(0x925) != 0) {
            Field_Do5(0x1e06);
            Field_Run119(8, 0);
        } else {
            if (Field_Check7(0x921) != 0) {
                Field_Do6(0x1dcd);
                Field_Run120(8, 0);
                record = Field_Check8(0x925);
                if (record != 0) {
                    goto L_020007cc;
                }
                record = Field_Check9(0x924);
                if (record == 0) {
                    goto L_020007cc;
                }
                {
                    u16 *target = (u16 *)((*(u8 *volatile *)gWork + 0x172));
                    s32 shown = 1;

                    *target = shown;
                }
            } else {
                Field_Do7(0x1d30);
                Field_Run121(8, 0);
            }
        }
    }
    L_020007cc:;
    Field_Run122();
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_Run123();
    if (Field_Check10(0x925) != 0) {
        Field_Run124(8, 2);
        Field_Do8(0x1e13);
        Field_Run125(8);
        Field_Run126(8, 0, 10);
        Field_Check11(8, 0);
        if (Field_Check12(0, 0) == 0) {
            Field_Run127(40);
            Field_Run128(8);
            Field_Check13(8, 0x3000);
            Field_Run129(8, 0);
            goto L_0200088e;
        }
        bump_step(2);
        Field_Run130(8, 0);
        Field_Place3(8, 0x3000, 0);
    } else {
        Field_Do9(0x1d4e);
        Field_Run131(8, 0);
    }
    L_0200088e:;
    Field_Run132();
}

void Dialogue_ShowLine1E19Or1D50(void)
{
    Field_Run133();
    if (Field_Check73((s32)&Value_00000925) != 0) {
        Field_Do70((s32)&Value_00001e19);
        Field_Apply3(10, 0);
    } else {
        Field_Do71((s32)&Value_00001d50);
        Field_Apply4(10, 0);
    }
    Field_Run134();
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

    if (Field_Check74(0x300) != 0) {
        u8 *obj = Field_Run45();
        u8 *p;

        Field_Run135();
        Field_Do72(obj);
        Field_Do73((s32)&Value_00001e9e);
        Field_Do74(8);
        Field_Apply5(obj, 2);
        p = Field_Run46(0);
        if (p != 0) {
            Field_Place256(obj, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_Do75(obj);
        Field_Place257(obj, 0, 0);
        Field_Run136();
    } else if (Field_Check75((s32)&Value_0000092b) != 0) {
        Field_Place258(8, (s32)&Value_00001e78, 0x990);
    } else if (Field_Check76((s32)&Value_0000092a) != 0) {
        Field_Place258(8, (s32)&Value_00001e78, (s32)&Value_00000917);
    } else if (Field_Check77((s32)&Value_00000929) != 0) {
        Field_Place258(8, (s32)&Value_00001e78, (s32)&Value_00000935);
    } else {
        Field_Place259(8, (s32)&Value_00001e78, (s32)&Value_0000092c);
    }
}

void Dialogue_RunActorTenFlaggedDialogue(void)
{
    if (Field_Check78(0x300) != 0) {
        u8 *o = Field_Run53();
        u8 *p;

        Field_Run137();
        Field_Do76(o);
        Field_Do77((s32)&Value_00001e9f);
        Field_Do78(10);
        Field_Apply6(o, 2);
        p = Field_Run54(0);
        if (p != 0) {
            Field_Place260(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_Do79(o);
        Field_Place261(o, 0, 0);
        Field_Run138();
    } else if (Field_Check79((s32)&Value_0000092b) != 0) {
        Field_Place262(10, (s32)&Value_00001e7b, (s32)&Value_00000992);
    } else if (Field_Check80((s32)&Value_0000092a) != 0) {
        Field_Place262(10, (s32)&Value_00001e7b, (s32)&Value_00000919);
    } else if (Field_Check81((s32)&Value_00000929) != 0) {
        Field_Place262(10, (s32)&Value_00001e7b, (s32)&Value_00000937);
    } else {
        Field_Place263(10, (s32)&Value_00001e7b, (s32)&Value_0000092e);
    }
}

void Scene_RunActor11FlagDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_Check82(0x8A0) != 0) {
        Field_Run139();
        Field_Apply7(11, 0x102);
        Field_Do80(40);
        Field_Do81((s32)&Value_00001f47);
        Field_Apply8(11, 0);
        Field_Run140();
    } else if (Field_Check83(0x300) != 0) {
        u8 *o = Field_Run39();
        u8 *p;

        Field_Run141();
        Field_Do82(o);
        Field_Do83((s32)&Value_00001ea0);
        Field_Do84(11);
        Field_Apply9(o, 2);
        p = Field_Run40(0);
        if (p != 0) {
            Field_Place264(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_Do85(o);
        Field_Place265(o, 0, 0);
        Field_Run142();
    } else if (Field_Check84((s32)&Value_0000092b) != 0) {
        Field_Place266(11, (s32)&Value_00001e7e, (s32)&Value_00000993);
    } else if (Field_Check85((s32)&Value_0000092a) != 0) {
        Field_Place266(11, (s32)&Value_00001e7e, (s32)&Value_0000091a);
    } else if (Field_Check86((s32)&Value_00000929) != 0) {
        Field_Place266(11, (s32)&Value_00001e7e, (s32)&Value_00000938);
    } else {
        Field_Place267(11, (s32)&Value_00001e7e, (s32)&Value_0000092f);
    }
}

void Scene_RunScene3b1SequenceA(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_Run143();
    if (Field_Check14(0x300) != 0) {
        rec7 = Field_Run();
        Field_Run144();
        Field_Do10(0x1ea1);
        Field_Run145(12);
        Field_Run146(rec7, 2);
        record = Field_Check15(0);
        if (record != 0) {
            Field_Run147(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Field_Run148(rec7);
        Field_Run149(rec7, 0, 0);
    } else {
        Field_Run150(12, 2);
        Field_Run151(20);
        Field_Do11(0x1e81);
        Field_Check16(12, 0);
        if (Field_Check17(0, 0) == 0) {
            Field_Run152(12);
            Field_Run153(12, 2);
            record = Field_Check18(0);
            if (record != 0) {
                Field_Run154(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Field_Run155(12);
            Field_Run156(12, 0, 0);
            Field_Do12(0x300);
            if (Field_Check19(0x92b) != 0) {
                Field_Do13(0x994);
                goto L_02000c9a;
            }
            if (Field_Check20(0x92a) != 0) {
                Field_Do14(0x91b);
                goto L_02000c9a;
            }
            if (Field_Check21(0x929) != 0) {
                Field_Do15(0x939);
                goto L_02000c9a;
            }
            Field_Do16(0x930);
        } else {
            bump_step(1);
            Field_Run157(12);
        }
    }
    L_02000c9a:;
    Field_Run158();
}

void Scene_RunScene3b1SequenceB(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_Run159();
    if (Field_Check22(0x300) != 0) {
        rec7 = Field_Run2();
        Field_Run160();
        Field_Do17(0x1ea2);
        Field_Run161(9);
        Field_Run162(rec7, 2);
        record = Field_Check23(0);
        if (record != 0) {
            Field_Run163(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ((void (*)())Field_Check87)(rec7);
        Field_Run164(rec7, 0, 0);
    } else {
        Field_Do18(0x1e84);
        ((void (*)())Field_Check88)(9, 0, 60);
        Field_Run165(9, 1);
        Field_Check24(9, 0);
        if (Field_Check25(0, 0) == 0) {
            Field_Run166(9);
            Field_Run167(9, 2);
            record = Field_Check26(0);
            if (record != 0) {
                Field_Run168(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Field_Run169(9);
            Field_Run170(9, 0, 0);
            Field_Do19(0x300);
            if (Field_Check27(0x92b) != 0) {
                Field_Do20(0x991);
                goto L_02000de0;
            }
            if (Field_Check28(0x92a) != 0) {
                Field_Do21(0x918);
                goto L_02000de0;
            }
            if (Field_Check29(0x929) != 0) {
                Field_Do22(0x936);
                goto L_02000de0;
            }
            Field_Do23(0x92d);
        } else {
            bump_step(1);
            Field_Run171(9);
        }
    }
    L_02000de0:;
    Field_Run172();
}

void Dialogue_RunActorThirteenFlag300Branch(void)
{
    if (Field_Check89(0x300) != 0) {
        u8 *o = Field_Run56();
        u8 *p;

        Field_Run173();
        Field_Do86(o);
        Field_Do87((s32)&Value_00001ea3);
        Field_Do88(13);
        Field_Apply10(o, 2);
        p = Field_Run57(0);
        if (p != 0) {
            Field_Place268(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_Do89(o);
        Field_Place269(o, 0, 0);
        Field_Run174();
    } else if (Field_Check90((s32)&Value_0000092b) != 0) {
        Field_Place270(13, (s32)&Value_00001e88, (s32)&Value_00000995);
    } else if (Field_Check91((s32)&Value_0000092a) != 0) {
        Field_Place270(13, (s32)&Value_00001e88, (s32)&Value_0000091c);
    } else if (Field_Check92((s32)&Value_00000929) != 0) {
        Field_Place270(13, (s32)&Value_00001e88, (s32)&Value_0000093a);
    } else {
        Field_Place271(13, (s32)&Value_00001e88, (s32)&Value_00000931);
    }
}

void Scene_RunFlag300BranchDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_Check93(0x300) != 0) {
        u8 *o = Field_Run41();
        u8 *p;

        Field_Run175();
        Field_Do90(o);
        Field_Do91((s32)&Value_00001ea4);
        Field_Do92(14);
        Field_Apply11(o, 2);
        p = Field_Run42(0);
        if (p != 0) {
            Field_Place272(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_Do93(o);
        Field_Place273(o, 0, 0);
        Field_Run176();
    } else if (Field_Check94((s32)&Value_0000092b) != 0) {
        Field_Place274(14, (s32)&Value_00001e8b, (s32)&Value_00000996);
    } else if (Field_Check95((s32)&Value_0000092a) != 0) {
        Field_Place274(14, (s32)&Value_00001e8b, (s32)&Value_0000091d);
    } else if (Field_Check96((s32)&Value_00000929) != 0) {
        Field_Place274(14, (s32)&Value_00001e8b, (s32)&Value_0000093b);
    } else {
        Field_Place275(14, (s32)&Value_00001e8b, (s32)&Value_00000932);
    }
}

void Scene_RunActor15FlagDialogue(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    if (Field_Check97(0x300) != 0) {
        u8 *o = Field_Run43();
        u8 *p;

        Field_Run177();
        Field_Do94(o);
        Field_Do95((s32)&Value_00001ea5);
        Field_Do96(15);
        Field_Apply12(o, 2);
        p = Field_Run44(0);
        if (p != 0) {
            Field_Place276(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Field_Do97(o);
        Field_Place277(o, 0, 0);
        Field_Run178();
    } else if (Field_Check98((s32)&Value_0000092b) != 0) {
        Field_Place278(15, (s32)&Value_00001e8e, (s32)&Value_00000997);
    } else if (Field_Check99((s32)&Value_0000092a) != 0) {
        Field_Place278(15, (s32)&Value_00001e8e, (s32)&Value_0000091e);
    } else if (Field_Check100((s32)&Value_00000929) != 0) {
        Field_Place278(15, (s32)&Value_00001e8e, (s32)&Value_0000093c);
    } else {
        Field_Place279(15, (s32)&Value_00001e8e, (s32)&Value_00000933);
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

    if (Field_Run88(0x300) != 0) {
        obj = Field_Run85();
        Field_Run89();
        Field_Run86(obj);
        Field_Run90(0x1ea6);
        Field_Run87(16);
        Field_Run91(obj, 2);

        actor = Field_Run92(0);
        if (actor != 0) {
            Field_Run93(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Field_Run94(obj);
        Field_Run95(obj, 0, 0);
        Field_Run96();
    } else {
        if (Field_Run97(0x92b) != 0) {
            Field_Run100(16, 0x1e91, 0x998);
        } else if (Field_Run98(0x92a) != 0) {
            Field_Run100(16, 0x1e91, 0x91f);
        } else if (Field_Run99(0x929) != 0) {
            Field_Run100(16, 0x1e91, 0x93d);
        } else {
            Field_Run101(16, 0x1e91, 0x934);
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
    u8 *obj = Field_Run58(0);
    u32 ofs = gOv17[bucket];
    s32 x = *(s16 *)(obj + 10) + ((s32)ofs >> 16);
    s32 z = *(s16 *)(obj + 18) + (s32)(s16)ofs;

    if (Field_Check101(x, z) != 0) {
        return 0;
    }

    {
        s32 point[3];
        point[0] = x << 16;
        point[1] = *(s32 *)(obj + 12);
        point[2] = z << 16;

        if (Field_Check102(obj, point) != 0) {
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

    if (Field_Check103((s32)&Value_0000092b) != 0) {
        ret = 3;
    } else if (Field_Check104((s32)&Value_0000092a) != 0) {
        ret = 2;
    } else if (Field_Check105((s32)&Value_00000929) != 0) {
        ret = 1;
    }

    return Field_Check106(ret, 1);
}

/* Story selector owner at 0x02001324, 84 bytes; eight calls. Per-site call
 * veneers (raw asm confirms each callee slot uses a distinct local stub
 * even across the three near-identical "twin" owners at 0x1324/1378/13cc). */
void Dialogue_ShowLine1ECETo1ED0(void)
{
    Field_Run179();
    if (Field_Run180(0x92c)) Field_Run181(0x1ece);
    else if (Field_Run182(0x935)) Field_Run183(0x1ecf);
    else Field_Run184(0x1ed0);
    Field_Run185(0x12, 0); Field_Run186();
}

/* Story selector owner at 0x02001378, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x13cc with distinct local stub addresses). */
void Dialogue_RunActor19TwoFlagLineA(void)
{
    Field_Run187();
    if (Field_Run188(0x92d)) Field_Run189(0x1ece);
    else if (Field_Run190(0x936)) Field_Run191(0x1ecf);
    else Field_Run192(0x1ed0);
    Field_Run193(0x13, 0); Field_Run194();
}

/* Story selector owner at 0x020013cc, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x1378 with distinct local stub addresses). */
void Dialogue_RunActor20TwoFlagLine(void)
{
    Field_Run195();
    if (Field_Run196(0x92e)) Field_Run197(0x1ece);
    else if (Field_Run198(0x937)) Field_Run199(0x1ecf);
    else Field_Run200(0x1ed0);
    Field_Run201(0x14, 0); Field_Run202();
}

/* Story selector owner at 0x02001420, 60 bytes; six calls. */
void Dialogue_ShowLine1ED1Or1ED2(void)
{
    Field_Run203();
    if (Field_Run204(0x92f)) Field_Run205(0x1ed1);
    else Field_Run206(0x1ed2);
    Field_Run207(21, 0); Field_Run208();
}

/* Story selector owner at 0x0200145c, 84 bytes; eight calls. */
void Dialogue_RunActor22TwoFlagLine(void)
{
    Field_Run209();
    if (Field_Run210(0x930)) Field_Run211(0x1ece);
    else if (Field_Run212(0x939)) Field_Run213(0x1ecf);
    else Field_Run214(0x1ed0);
    Field_Run215(22, 0); Field_Run216();
}

/* Story selector owner at 0x020014b0, 84 bytes; eight calls. */
void Dialogue_RunActor23BranchedDialogue(void)
{
    Field_Run217();
    if (Field_Run218(0x931)) Field_Run219(0x1ece);
    else if (Field_Run220(0x93a)) Field_Run221(0x1ecf);
    else Field_Run222(0x1ed0);
    Field_Run223(23, 0); Field_Run224();
}

/* Story selector owner at 0x02001504, 84 bytes; eight calls. */
void Dialogue_RunActor24BranchedDialogue(void)
{
    Field_Run225();
    if (Field_Run226(0x932)) Field_Run227(0x1ece);
    else if (Field_Run228(0x93b)) Field_Run229(0x1ecf);
    else Field_Run230(0x1ed0);
    Field_Run231(24, 0); Field_Run232();
}

/* Story selector owner at 0x02001558, 60 bytes; six calls. */
void Dialogue_RunActor25FlaggedLine(void)
{
    Field_Run233();
    if (Field_Run234(0x933)) Field_Run235(0x1ed1);
    else Field_Run236(0x1ed2);
    Field_Run237(25, 0); Field_Run238();
}

/* Second-phase story selector at 0x02001594, 84 bytes; eight calls. */
void Dialogue_RunActor18TwoFlagLine(void)
{
    Field_Run239();
    if (Field_Run240(0x92c)) Field_Run241(0x1edb);
    else if (Field_Run242(0x935)) Field_Run243(0x1edc);
    else Field_Run244(0x1edd);
    Field_Run245(18, 0); Field_Run246();
}

/* Second-phase story selector at 0x020015e8, 84 bytes; eight calls. */
void Dialogue_RunActor19TwoFlagLineB(void)
{
    Field_Run247();
    if (Field_Run248(0x92d)) Field_Run249(0x1edb);
    else if (Field_Run250(0x936)) Field_Run251(0x1edc);
    else Field_Run252(0x1edd);
    Field_Run253(19, 0); Field_Run254();
}

/* Second-phase story selector at 0x0200163c, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Field_Run255();
    if (Field_Run256(0x92e)) Field_Run257(0x1edb);
    else if (Field_Run258(0x937)) Field_Run259(0x1edc);
    else Field_Run260(0x1edd);
    Field_Run261(20, 0); Field_Run262();
}

/* Second-phase story selector at 0x02001690, 60 bytes; six calls. */
void Dialogue_RunActor21FlaggedLine(void)
{
    Field_Run263();
    if (Field_Run264(0x92f)) Field_Run265(0x1ede);
    else Field_Run266(0x1edf);
    Field_Run267(21, 0); Field_Run268();
}

/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void Dialogue_RunActor22BranchedDialogue(void)
{
    Field_Run269();
    if (Field_Run270(0x930)) Field_Run271(0x1edb);
    else if (Field_Run272(0x939)) Field_Run273(0x1edc);
    else Field_Run274(0x1edd);
    Field_Run275(22, 0); Field_Run276();
}

/* Second-phase story selector at 0x02001720, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Field_Run277();
    if (Field_Run278(0x931)) Field_Run279(0x1edb);
    else if (Field_Run280(0x93a)) Field_Run281(0x1edc);
    else Field_Run282(0x1edd);
    Field_Run283(23, 0); Field_Run284();
}

/* Second-phase story selector at 0x02001774, 84 bytes; eight calls. */
void Dialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Field_Run285();
    if (Field_Run286(0x932)) Field_Run287(0x1edb);
    else if (Field_Run288(0x93b)) Field_Run289(0x1edc);
    else Field_Run290(0x1edd);
    Field_Run291(24, 0); Field_Run292();
}

/* Second-phase story selector at 0x020017c8, 60 bytes; six calls. */
void Dialogue_ShowLine1EDEOr1EDF(void)
{
    Field_Run293();
    if (Field_Run294(0x933)) Field_Run295(0x1ede);
    else Field_Run296(0x1edf);
    Field_Run297(25, 0); Field_Run298();
}

void Scene_RunPrimarySequence(s32 a0, s32 a1, s32 a2)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_Run299();
    Field_Run300(a1);
    Field_Check30(a0, 0);
    if (Field_Check31(0, 0) == 0) {
        Field_Run301(a0);
        Field_Run302(a0, 2);
        record = Field_Check32(0);
        if (record != 0) {
            Field_Run303(a0, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Field_Run304(a0);
        Field_Run305(a0, 0, 0);
        Field_Do24(0x300);
        Field_Run306(a2);
    } else {
        bump_step(1);
        Field_Run307(a0);
    }
    Field_Run308();
}

void Scene_RunScene3b1SequenceC(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    record = Field_Check107(0);
    if ((u32)((*(volatile u16 *)(record + 6) + -0x2000) << 16) > -0x40000000) {
        if (Field_Check33(0x928) != 0) {
            if (Field_Check34(0x93e) == 0) {
                Field_Run309(17);
                goto L_0200194e;
            }
        }
        Field_Run310(15);
    } else {
        Field_Run311();
        if (Field_Check35(0x93e) != 0) {
            Field_Do25(0x1f81);
        } else {
            if (Field_Check36(0x8a0) != 0) {
                Field_Do26(0x1f48);
            } else {
                if (Field_Check37(0x928) != 0) {
                    Field_Do27(0x1f7f);
                } else {
                    if (Field_Check38(0x925) != 0) {
                        Field_Do28(0x1f7d);
                    } else {
                        Field_Do29(0x1f7b);
                    }
                }
            }
        }
        if (Field_Check39(0x928) != 0) {
            record = Field_Check40(0x93e);
            if (record != 0) {
                goto L_02001942;
            }
            Field_Run312(17, 0);
        } else {
            L_02001942:;
            Field_Run313(15, 0);
        }
        Field_Run314();
    }
    L_0200194e:;
}

void Scene_RunScene3b1SequenceD(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Field_Check41(0x301) != 0) {
        Field_Run315();
        Field_Run316(8);
        Field_Place4(0x1e48, 1, 8);
        Field_Place5(0, 0x19999, 0xcccc);
        Field_Place6(0, 0x198, 134);
        Field_Check42(0, 0x4000);
        Field_Run317();
    }
}

/*
 * Gated on flag 0x922.  Every callee slot has its own local call stub;
 * Field_Run318 and Field_Run319 are the same stub declared twice without
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

    if (Field_Check108(0x922) == 0)
        return;

    Field_Run320();
    Field_Run321();
    Field_Apply13(0x19999, 0x3333);
    Field_Run318(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Field_Do98(0x1d26);

    Field_Do99(8);
    Field_Do100(10);
    Field_Apply14(8, 0x3000);
    Field_Do101(8);
    Field_Run319(10, 0xd000);
    Field_Do102(10);
    Field_Apply15(9, 0x5000);
    Field_Do103(9);

    Field_Place280(8, 0, 20);
    Field_Do104(8);
    Field_Apply16(9, 0x8000);
    Field_Do105(9);
    Field_Do106(10);
    Field_Do107(8);
    Field_Apply17(10, 0xb000);
    Field_Do108(8);

    Field_Do109(0x920);
    Field_Run322();
}

void Scene_RunThreeActorPresentation(void)
{
    s32 request_a;
    s32 request_b;
    s32 action;

    if (Field_Check43(0x911) == 0) {
    } else {
        Field_Run323();
        Field_Run324();
        Field_Run3(0x26666, 0x4ccc);
        Field_Run325( 0x5b70000, -1, 0x1d00000, 0x10000014);
        Field_Run326(13, 1);
        Field_Do30(0x1d56);
        Field_Run327(0x200d);
        Field_Run328(12, 0xd000);
        request_a = 0x800c;
        Field_Place7(12, 0x102, 20);
        Field_Run329(12, 2);
        Field_Run330(request_a);
        Field_Run331(14, 1);
        Field_Place8(0xa00e, 0, 20);
        Field_Run332(12, 0);
        request_b = 0xa00e;
        Field_Place9(12, 0x101, 40);
        Field_Place10(14, 0x103, 40);
        Field_Run333(14, 3);
        Field_Run334(request_b);
        Field_Run4(12, 0x102);
        Field_Run335(40);
        Field_Run336(12, 3);
        Field_Run337(request_a);
        Field_Run338(14, 1);
        Field_Run339(request_b);
        Field_Run340(14, 0xb000);
        Field_Run341(request_b);
        Field_Run342(12, 0xd000);
        Field_Place11(12, 0x100, 30);
        Field_Run343(12, 1);
        Field_Run344(request_a);
        Field_Run345(13, 4);
        Field_Run346(0x200d);
        Field_Run347(13, 2);
        Field_Run348(0x200d);
        Field_Run349(12, 4);
        Field_Run350(request_a);
        Field_Run351(14, 4);
        Field_Run352(request_b);
        Field_Run353(14, 0x8000);
        Field_Run354(14, 2);
        Field_Run355(request_b, 0, 20);
        Field_Run356(12, 0, 0);
        Field_Place12(12, 0x102, 80);
        Field_Run357(request_a, 0, 20);
        Field_Place13(14, 0x103, 0);
        Field_Place14(13, 0x103, 60);
        Field_Run358(14, 2);
        Field_Run359(request_b);
        Field_Run360(14, 0xb000);
        Field_Run361(14, 1);
        Field_Run362(request_b);
        Field_Run363(13, 0x3000);
        Field_Place15(13, 0x101, 0);
        Field_Place16(12, 0x101, 60);
        Field_Run364(13, 1);
        Field_Run365(13);
        Field_Place17(14, 0x103, 40);
        Field_Run366(14, 1);
        Field_Run367(request_b);
        Field_Run368(12, 0xd000, 0);
        Field_Place18(13, 0x5000, 40);
        Field_Place19(12, 0, 0);
        Field_Run369(13, 0x3000);
        Field_Run370(12, 2);
        Field_Run371(request_a, 0, 20);
        Field_Place20(14, 0x4000, 40);
        Field_Run372(request_b);
        Field_Run373(12, 2);
        Field_Run374(13, 2);
        Field_Run375(60);
        Field_Run376(13, 1);
        Field_Run377(13);
        Field_Run378(14, 3);
        Field_Run379(request_b);
        Field_Place21(12, 0x102, 40);
        Field_Run5(12, 2);
        Field_Run380(request_a);
        Field_Run381(13, 3);
        Field_Run382(13);
        Field_Run383(14, 0xb000, 40);
        Field_Run384(14, 3);
        Field_Run385(13, 3);
        Field_Place22(14, 0x19999, 0xcccc);
        Field_Place23(13, 0x19999, 0xcccc);
        action = (s32)gOv18;
        Field_Run386(14, action);
        Field_Run387(13, action);
        Field_Run388(20);
        Field_Place24(12, 0x4000, 0);
        Field_Place25(0, 0x26666, 0x13333);
        *(u8 *)(Field_Run59(0) + 90) &= 254;
        Field_Place26(0, 184, 0x208);
        Field_Run389(1);
        {
            u8 *record = Field_Run60(0);
            u32 flag = 1;

            flag = flag | record[90];
            record[90] = (u8)flag;
        }
        Field_Run390(0, 0x8000, 20);
        Field_Place27(0, 0x4000, 0);
        Field_Run391(12, 4, 20);
        Field_Place28(0, 0xa000, 20);
        Field_Run392(12, 2);
        Field_Run393(12);
        Field_Place29(12, 0x19999, 0xcccc);
        Field_Run394(12, action);
        Field_Run395(40);
        Field_Place30(0, 0x4000, 0);
        Field_Run396(12);
        Field_Do31(0x922);
        Field_Run397();
    }
}

void Scene_RunExtendedActorChoreography(void)
{
    s32 request_a;
    s32 request_b;
    s32 request_c;

    Field_Run398(28);
    Field_Run6(0x26666, 0x4ccc);
    Field_Run399(0x1c80000, -1, 0x2880000, 0x10000014);
    Field_Run400(9, 1);
    Field_Do32(0x1d93);
    Field_Run401(9);
    Field_Place31(0, 0xd000, 0);
    Field_Place32(10, 0xd000, 0);
    Field_Run402(11, 0, 0);
    Field_Place33(12, 0x3000, 0);
    Field_Place34(13, 0x8000, 40);
    Field_Place35(9, 0x103, 40);
    Field_Run403(9, 2);
    Field_Run404(9);
    Field_Run405(12, 0, 0);
    Field_Place36(11, 0xd000, 0);
    Field_Place37(13, 0xd000, 20);
    Field_Run406(11, 1);
    Field_Run407(0x100b);
    Field_Place38(13, 0x102, 20);
    Field_Run408(13, 2);
    Field_Run409(13);
    Field_Place39(9, 0x105, 60);
    Field_Run410(9);
    Field_Place40(12, 0x104, 20);
    Field_Do33(0x900c);
    Field_Run411(8, 1);
    Field_Run412(8, 3);
    Field_Run413(8);
    Field_Run414(12, 0x3000);
    Field_Do34(0x900c);
    Field_Run415(11, 0xb000);
    Field_Run416(11, 3);
    Field_Run417(10);
    Field_Run418(13, 1);
    Field_Run419(13, 3);
    Field_Run420(13);
    Field_Place41(13, 0x8000, 0);
    Field_Place42(12, 0x5000, 0);
    Field_Run421(11, 0x5000);
    Field_Place43(13, 0x6666, 0x3333);
    Field_Place44(12, 0xcccc, 0x6666);
    Field_Place45(12, 0x1bc, 0x29c);
    Field_Place46(13, 0x1d8, 0x29c);
    Field_Run422(12);
    Field_Run423(12, 1);
    Field_Run424(80);
    Field_Run425(12, 0xd000);
    Field_Place47(12, 0x101, 60);
    Field_Run426(11, 1);
    Field_Run427(20);
    Field_Place48(0x400b, 0, 40);
    Field_Run428(11, 2);
    Field_Run429(11, 0xd000, 0);
    Field_Run430(0x100b);
    Field_Run431(12, 0xd000, 0);
    Field_Place49(9, 0x101, 60);
    Field_Run432(11, 4);
    Field_Run433(20);
    Field_Run434(0x100b);
    Field_Run435(9, 3);
    Field_Run436(9);
    Field_Run437(13, 0xd000, 0);
    Field_Run7(13, 0x102);
    Field_Run438(13, 2, 20);
    Field_Run439(13);
    Field_Run440(9, 3);
    request_a = 0x100c;
    Field_Run441(9);
    Field_Run442(11, 0xd000);
    Field_Run443(12, 1);
    Field_Run444(request_a);
    Field_Place50(8, 0x105, 40);
    Field_Run445(8, 3);
    Field_Run446(8);
    Field_Place51(13, 0x102, 40);
    Field_Run447(13, 4, 0);
    Field_Run448(13);
    Field_Run449(9, 3);
    Field_Run450(9);
    Field_Run451(11, 1);
    Field_Run452(0x100b);
    Field_Place52(8, 0x102, 40);
    Field_Run453(8, 0);
    Field_Run454(11, 2);
    Field_Run455(0x100b, 0, 40);
    Field_Place53(9, 0x100, 0);
    Field_Run456(9, 0x5000, 20);
    Field_Run457(9, 2);
    Field_Run458(9, 0, 20);
    Field_Run459(11, 3);
    Field_Run460(20);
    Field_Place54(12, 0x100, 40);
    Field_Run461(12, 2);
    request_b = 0x400b;
    Field_Run462(request_a);
    Field_Run463(11, 0x5000, 20);
    Field_Run464(request_b);
    Field_Run465(12, 2);
    Field_Run466(request_a);
    Field_Run467(11, 3);
    Field_Run468(11, 1);
    Field_Run469(request_b);
    Field_Place55(12, 0x102, 60);
    Field_Run470(9, 1);
    Field_Run471(9);
    Field_Place56(11, 0x101, 40);
    Field_Run472(11, 0xd000, 20);
    Field_Run473(9, 3);
    Field_Run474(9);
    Field_Place57(11, 0x103, 20);
    Field_Run475(11, 2);
    Field_Run476(0x100b);
    Field_Place58(9, 0x108, 40);
    Field_Run477(9);
    Field_Run478(8, 1);
    Field_Run479(8, 3);
    Field_Run480(8);
    Field_Run481(9, 0xd000, 40);
    Field_Run482(9);
    Field_Run483(12, 1);
    Field_Run484(20);
    Field_Run485(request_a);
    Field_Run486(11, 0x5000, 0);
    Field_Run487(9, 0x5000, 0);
    Field_Place59(13, 0x8000, 0);
    Field_Place60(0, 0xc000, 0);
    Field_Run488(10, 0xb000, 40);
    Field_Run489(11, 1);
    Field_Run490(request_b);
    Field_Run491(12, 3);
    Field_Run492(request_a, 0, 20);
    Field_Run493(9, 2);
    Field_Run494(9);
    Field_Place61(12, 0x108, 40);
    Field_Run495(12, 3);
    Field_Run496(request_a);
    Field_Run497(8, 3);
    Field_Run498(8);
    Field_Place62(8, 0x8000, 20);
    Field_Run499(19);
    Field_Run500(8, 2);
    request_c = 0x8008;
    Field_Place63(8, 0x100, 80);
    Field_Run501(request_c);
    Field_Place64(12, 0x101, 0);
    Field_Place65(11, 0x101, 0);
    Field_Place66(13, 0x101, 0);
    Field_Place67(10, 0x101, 0);
    Field_Place68(0, 0x101, 40);
    Field_Run502(12, 0xd000, 0);
    Field_Run503(11, 0xd000, 0);
    Field_Run504(13, 0xb000, 0);
    Field_Run505(10, 0xb000, 0);
    Field_Place69(0, 0xc000, 40);
    Field_Run8(8, 0x102);
    Field_Run506(8, 4, 40);
    Field_Run507(8, 2);
    Field_Run508(request_c, 0);
    Field_Place70(8, 0x19999, 0xcccc);
    Field_Place71(8, 0x1db, 0x256);
    Field_Place72(8, 0x8000, 0);
    Field_Place73(9, 0x10000, 0x8000);
    Field_Place74(9, 0x1ce, 0x26a);
    Field_Run509(9, 0xb000);
    Field_Place75(9, 0x100, 40);
    Field_Run510(9, 2);
    Field_Do35(0x8009);
    Field_Place76(11, 0x101, 60);
    Field_Run511(11);
    Field_Place77(12, 0x102, 20);
    Field_Run512(request_a);
    Field_Place78(8, 0x103, 20);
    Field_Run513(8, 4, 0);
    Field_Run514(8, 0x5000, 20);
    Field_Run515(8);
    Field_Run516(28);
    Field_Run517(8, 3);
    Field_Run518(8);
    Field_Place79(13, 0x101, 60);
    Field_Run519(13);
    Field_Run520(8, 0x3000);
    Field_Run521(8, 4);
    Field_Run522(8);
    Field_Place80(12, 0x1bc, 0x274);
    Field_Run523(12, 0xd000);
    Field_Do36(0x900c);
    Field_Run524(8, 0x5000, 20);
    Field_Run525(8, 3);
    Field_Run526(8);
    Field_Place81(11, 0x102, 60);
    Field_Run527(0x100b);
    Field_Place82(13, 0x107, 40);
    Field_Run528(13, 2);
    Field_Run529(13);
    Field_Run530(9, 0x3000);
    Field_Run531(9, 4);
    Field_Do37(0x1009);
    Field_Run532(12, 0);
    Field_Run533(8, 1);
    Field_Run534(8);
    Field_Run535(11, 0, 0);
    Field_Place83(12, 0x105, 0);
    Field_Place84(9, 0x105, 60);
    Field_Run9(0x13333, 0x2666);
    Field_Run536(0x1d00000, -1, 0x2a80000, 0x10000000);
    Field_Run537(10, 1);
    Field_Run538(10, 0);
    Field_Run539(10);
    Field_Run540(0, 0, 0);
    Field_Place85(10, 0x102, 40);
    Field_Run541(10);
    Field_Place86(10, 0x8000, 20);
    Field_Place87(10, 0x100, 0);
    Field_Run542(10, 4, 40);
    Field_Run543(10);
    Field_Run544(10, 1);
    Field_Run545(10);
    Field_Place88(13, 0x10000, 0x8000);
    Field_Place89(13, 0x1b6, 0x293);
    Field_Place90(8, 0x8000, 0);
    Field_Run546(9, 0xb000, 0);
    Field_Run547(12, 0x3000, 0);
    Field_Run548(11, 0xb000, 0);
    Field_Run549(17);
    Field_Place91(10, 0x10000, 0x8000);
    Field_Place92(10, 0x1e8, 0x2ae);
    Field_Run550(10, 0xb000, 0);
    Field_Run551(13);
    Field_Run552(13, 1);
    Field_Run553(13, 0xd000, 0);
    Field_Run554();
    Field_Do38(0x921);
}

void Scene_RunBranchingActorPresentation(void)
{
    u8 *record;
    s32 request_a;
    s32 request_b;
    s32 value;
    s32 request_c;
    s32 action;

    Field_Run555();
    Field_Run556();
    Field_Run557(8, 0, 0);
    Field_Place93(8, 0x100, 40);
    request_a = 0x1008;
    Field_Run558(8, 3);
    Field_Do39(0x1ddb);
    Field_Run559(request_a);
    Field_Run560(9, 1);
    Field_Run561(12, 1);
    Field_Run562(11, 1);
    Field_Run563(13, 1);
    Field_Run564(10, 1);
    Field_Place94(9, 0xd000, 0);
    Field_Place95(12, 0xd000, 0);
    Field_Place96(11, 0xd000, 0);
    Field_Place97(13, 0xd000, 0);
    Field_Place98(10, 0xb000, 20);
    Field_Run565(8, 1);
    Field_Check44(request_a, 0);
    if (Field_Check45(0, 0) == 0) {
        Field_Run566(9, 2);
        Field_Do40(0x9009);
        Field_Place99(8, 0x108, 40);
        Field_Run567(request_a);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 2;
        Field_Run568(9, 1);
        Field_Do41(0x9009);
        Field_Run569(8, 2);
        Field_Do42(0x9008);
    }
    Field_Place100(13, 0x105, 40);
    Field_Run10(0xcccc, 0x1999);
    Field_Run11(0x1d80000, -1, 0x27c0000, 1);
    Field_Place101(13, 0x10000, 0x8000);
    Field_Place102(13, 0x1d8, 0x296);
    Field_Run570(13, 0xb000);
    Field_Run571(13);
    Field_Run572(8, 0x5000);
    Field_Run573(8, 3);
    Field_Run574(9, 3);
    Field_Run575(11, 0, 0);
    Field_Place103(13, 0x8000, 20);
    Field_Run576(11, 3);
    Field_Run577(13, 3);
    Field_Run578(20);
    Field_Run579(12, 1);
    Field_Run580(12, 0x3000);
    Field_Place104(0x100c, 0, 20);
    Field_Run581(11, 0xb000, 20);
    Field_Place105(11, 0x101, 40);
    Field_Run582(11);
    request_b = 0x900c;
    Field_Run583(12, 0xd000);
    Field_Run584(12, 4);
    Field_Run585(request_b);
    Field_Run586(13, 0xb000);
    Field_Run587(13, 1);
    Field_Run588(13);
    Field_Place106(9, 0x100, 20);
    Field_Run589(9, 0x3000);
    Field_Run590(9, 1);
    Field_Run591(9);
    Field_Run592(12, 3);
    Field_Run593(request_b);
    Field_Run594(8, 2);
    Field_Run595(8);
    Field_Run596(12, 0xd000);
    Field_Run597(12, 3);
    Field_Run598(request_b);
    Field_Run599(11, 2);
    Field_Run600(11, 0xb000);
    Field_Run601(11);
    Field_Run602(12, 0);
    Field_Run603(request_b);
    Field_Run604(8, 0x3000, 0);
    Field_Run605(9, 0, 0);
    Field_Run606(11, 0xd000, 0);
    Field_Run607(13, 0xd000, 0);
    Field_Place107(0, 0x10000, 0x8000);
    Field_Place108(0, 0x1e6, 0x260);
    Field_Place109(0, 0x8000, 0);
    record = Pointer1(Field_Run61, 0);
    if (record != 0) {
        Field_Run608(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_Place110(1, 0x10000, 0x8000);
    Field_Place111(1, 0x1e6, 0x270);
    Field_Place112(1, 0x8000, 0);
    record = Pointer1(Field_Run62, 1);
    if (record != 0) {
        Field_Run609(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_Place113(2, 0x10000, 0x8000);
    Field_Place114(2, 0x1e6, 0x280);
    Field_Place115(2, 0x8000, 0);
    record = Pointer1(Field_Run63, 2);
    if (record != 0) {
        Field_Run610(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_Place116(3, 0x10000, 0x8000);
    Field_Place117(3, 0x1e6, 0x290);
    Field_Place118(3, 0x8000, 20);
    Field_Place119(12, 0x108, 40);
    Field_Run611(request_b);
    Field_Run612(9, 1);
    Field_Do43(0x1009);
    Field_Run613(8, 3);
    Field_Run614(8, 0x5000);
    Field_Run615(8);
    Field_Run616(8, 0x3000);
    Field_Check46(8, 0);
    if (Field_Check47(0, 0) == 1) {
        Field_Run617(8, 2);
        Field_Run618(8);
        Field_Run619(12, 3);
        Field_Run620(request_b);
        Field_Run621(9, 1);
        Field_Place120(0x9009, 0, 40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 3;
        Field_Run622(8, 3);
        Field_Run623(8, 0, 40);
    }
    Field_Run624(13, 1);
    Field_Run625(13);
    Field_Run626(8, 1);
    value = 176;
    Field_Run627(8, 0x5000);
    Field_Run628(8);
    Field_Run629(13, 1);
    Field_Run630(13, (value << 8));
    Field_Run631(13, 0, 20);
    Field_Run632(8, 3);
    Field_Place121(8, 0xcccc, 0x6666);
    request_c = 0x4008;
    Field_Place122(8, 0x1d8, 0x278);
    Field_Run633(request_c);
    Field_Place123(13, 0x103, 40);
    Field_Run634(13, 2);
    Field_Run635(13);
    Field_Run636(8, 4);
    Field_Run637(request_c, 0, 40);
    Field_Run638(11, 1);
    Field_Run639(11, (value << 8));
    Field_Do44(0x100b);
    Field_Place124(10, 0x102, 20);
    Field_Place125(10, 0x26666, 0x13333);
    Field_Run640(10, 2, 0);
    Field_Place126(10, 0x1ce, 0x2a2);
    Field_Run641(10, (value << 8));
    Field_Run642(10, 2);
    Field_Run643(10);
    Field_Run644(9, 0x5000);
    Field_Run645(9, 4);
    Field_Run646(9);
    Field_Run647(8, 3);
    Field_Run648(request_c);
    Field_Place127(13, 0x102, 40);
    Field_Run649(13, 0, 40);
    Field_Run650(9, 0x3000);
    Field_Run651(9, 2);
    Field_Do45(0x1009);
    Field_Run652(12, 0);
    Field_Place128(8, 0x8000, 0);
    Field_Run653(9, 0x5000, 0);
    Field_Run654(11, (value << 8), 0);
    Field_Run655(13, (value << 8), 0);
    Field_Run656(10, (value << 8), 20);
    Field_Run657(12, 1);
    Field_Place129(0x100c, 0, 20);
    Field_Place130(8, 0x101, 40);
    Field_Run658(8, 0xd000);
    Field_Check48(0x1008, 0);
    if (Field_Check49(0, 0) == 0) {
        Field_Run659(8, 3);
        Field_Do46(0x1008);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Field_Do47(0x1008);
    }
    Field_Run660(0, 3);
    Field_Run661(8, 3);
    Field_Do48(0x1008);
    Field_Run12(8, 0x8000);
    Field_Do49(0x4008);
    Field_Run662(2, 0, 0);
    Field_Run663(12, 3);
    Field_Run664(11, 3);
    Field_Run665(9, 3);
    Field_Run666(10, 2);
    Field_Run667(13, 2);
    Field_Run668(20);
    action = (s32)gOv19;
    Field_Run669(10, action);
    Field_Run670(4);
    Field_Run671(11, action);
    Field_Run672(4);
    Field_Run673(12, action);
    Field_Run674(4);
    Field_Run675(9, action);
    Field_Run676(3, 2);
    record = Pointer1(Field_Run66, 2);
    if (record != 0) {
        Field_Run677(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_Run678(3);
    Field_Run679(3, 0, 0);
    Field_Run680(2, 2);
    record = Pointer1(Field_Run64, 1);
    if (record != 0) {
        Field_Run681(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_Run682(2);
    Field_Run683(2, 0, 0);
    Field_Run684(1, 2);
    record = Pointer1(Field_Run65, 0);
    if (record != 0) {
        Field_Run685(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Field_Run686(1);
    Field_Run687(1, 0, 0);
    Field_Run688(13, action);
    Field_Place131(8, 0x1c8, 0x288);
    Field_Run689(8, 0);
    Field_Run690(232);
    Field_Do50(0x925);
    Field_Run691();
}

/*
 * A four-way gated cascade. Each gate either runs its own setter sequence
 * and returns or falls through to the next, and all four share one exit.
 * The 336-byte owner at 0x0200351c includes its eight trailing pool words.
 */
void State_RunFlagGatedSetupCascade(void)
{
    if (Field_Check109((s32)&Value_0000093e) != 0) {
        Field_Run692(8, 0, 0);
        Field_Run693(9, 0, 0);
        Field_Run694(10, 0, 0);
        Field_Run695(11, 0, 0);
        Field_Run696(12, 0, 0);
        Field_Run697(14, 0, 0);
        return;
    }

    if (Field_Check110(0x8a0) != 0) {
        Field_Run698(8, 0x98, 0x1bc, 0x3000);
        Field_Run699(8, 0x0200e958);
        Field_Run700(10, 0xb8, 0x1e0, 0xb000);
        Field_Run701(12, 0xaa, 0x1e8, 0xb000);
        Field_Run702(13, 0x88, 0x1e8, 0xd000);
        Field_Run703(15, 0x78, 0x1e0, 0xd000);
        Field_Run704(14, 0xb8, 0x20e, 0xb000);
        Field_Run705(11, 0x88, 0x248, 0x8000);
        Field_Run706(11, 0x0200e840);
        return;
    }

    {
        s32 t = Field_Check111((s32)&Value_00000928);
        if (t != 0) {
            Field_Run707(t);
            return;
        }
    }

    if (Field_Check112((s32)&Value_00000925) != 0) {
        Field_Run697(18, 0, 0);
        return;
    }

    if (Field_Check113((s32)&Value_00000911) != 0 &&
        Field_Check114((s32)&Value_00000922) != 0) {
        Field_Run708(14, 0, 0);
        Field_Run709(12, 0, 0);
    }
}

void RunSceneSelectionChain(void)
{
    Field_Run710(1);
    Field_Run711();
    if (Field_Check115(2366) != 0) {
        Field_Run712(4, 4, 0);
        Field_Run713(8, 412, 222, 12288);
        Field_Run714(9, 458, 161, 32768);
    } else {
        if (Field_Check116(2208) != 0) {
            Field_Place132(8, 30932992, 9961472);
            Field_Run715(9, 5);
            Field_Run716(4, 4, 0);
        } else {
            if (Field_Check117(2347) != 0) {
                Field_Run717(16, 0, 0);
                Field_Run718(4, 4, 0);
                Field_Run719(3);
            } else {
                if (Field_Check118(2346) != 0) {
                    Field_Run720(16, 0, 0);
                    Field_Run721(4, 3, 0);
                    Field_Run722(2);
                } else {
                    if (Field_Check119(2345) != 0) {
                        Field_Run723(16, 0, 0);
                        Field_Run724(4, 2, 0);
                        Field_Run725(1);
                    } else {
                        if (Field_Check120(2344) != 0) {
                            Field_Run726(16, 0, 0);
                            Field_Run727(10, 0, 0);
                            Field_Run728(0);
                        } else {
                            Field_Run729(9, 5);
                            if (Field_Check121(2341) != 0 && Field_Check122(2342) == 0) {
                                Field_Run730();
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
        u8 *obj = Field_Run55(i);
        u32 v = obj[0x59];
        obj[0x59] = (u8)((v | bit) | zero);
    }
}

void Scene_RunFourActorCoordinatePresentation(void)
{
    u8 *record;
    s32 mode;

    Field_Run731();
    Field_Run732(25, 0, 0);
    Field_Run733(24, 1, 0);
    Field_Run734(0x1b80000, -1, 0xa80000, 0x1000001);
    Field_Run735(27, 0x1b8, 164, 0x5000);
    Field_Run736(8, 0x1ac, 190, 0xd000);
    Field_Run737(9, 0x1c4, 190, 0xb000);
    Field_Run738(9, 1);
    mode = 128;
    Field_Run739(0, 0x1b8, 134, 0x8000);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = (mode << 1);
    Field_Run740();
    Field_Place133(0, 0xcccc, 0x6666);
    Field_Place134(0, 0x198, 134);
    Field_Place135(0, 0x198, 148);
    Field_Place136(0, 0x1a8, 148);
    Field_Place137(0, 0x4000, 20);
    Field_Run741(27, 1);
    Field_Do51(0x1e27);
    Field_Run742(27);
    Field_Run743(8, 1);
    Field_Run744(8);
    Field_Run745(27, 3);
    Field_Run746(27);
    Field_Run747(27, 0xd000);
    record = Pointer1(Field_Run67, 0);
    if (record != 0) {
        Field_Run748(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_Place138(1, 0xcccc, 0x6666);
    Field_Run749(1, 0x1b8, 148);
    Field_Place139(1, 0x4000, 0);
    record = Pointer1(Field_Run68, 1);
    if (record != 0) {
        Field_Run750(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_Place140(2, 0xcccc, 0x6666);
    Field_Place141(2, 0x1c8, 148);
    Field_Place142(2, 0x4000, 0);
    record = Pointer1(Field_Run69, 2);
    if (record != 0) {
        Field_Run751(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Field_Place143(3, 0xcccc, 0x6666);
    Field_Place144(3, 0x1d8, 148);
    Field_Place145(3, 0x4000, 20);
    Field_Run752(0, 0, 60);
    Field_Run753(1, 0x4000, 20);
    Field_Run754(2, 1, 20);
    Field_Run755(27, 0x5000, 20);
    Field_Run756(27);
    Field_Run757(9, 1);
    Field_Run758(9, (mode << 1), 40);
    Field_Run759(9);
    Field_Run760(1, 3);
    Field_Place146(1, 0x103, 60);
    Field_Run761(27, 3);
    Field_Run762(27);
    Field_Run763(10, 1);
    Field_Run764(10, 3);
    Field_Run765(10);
    Field_Run766(8, 3);
    Field_Run767(9, 3);
    Field_Run768(11, 3);
    Field_Run769(12, 3);
    Field_Run770(13, 3);
    Field_Run771(0, 0, 40);
    Field_Run772(2, 1, 0);
    Field_Run773(1, 0x4000, 20);
    Field_Run774(27, 4);
    Field_Run775(27);
    Field_Place147(8, 0x102, 60);
    Field_Run776(8, 1);
    Field_Run777(8);
    Field_Run778(27, 3);
    Field_Run779(27);
    Field_Run780(8, 0, 0);
    Field_Run781(9, 0x8000, 40);
    Field_Place148(8, 0x102, 0);
    Field_Place149(8, 0x102, 40);
    Field_Run782(27, 1);
    Field_Run783(27, 3);
    Field_Run784(27, 0, 20);
    Field_Run785(8, 3);
    Field_Run786(9, 3);
    Field_Run787(40);
    Field_Run788(9, (mode << 1), 20);
    Field_Run789(9, 0xb000);
    Field_Run790(9);
    Field_Run791(27, 0x3000);
    Field_Place150(27, 0x101, 60);
    Field_Run792(27, 0, 60);
    Field_Place151(27, 0x106, 20);
    Field_Run793(27, 0xb000);
    Field_Run794(27, 3);
    Field_Run795(27);
    Field_Run796(3, 2, 80);
    Field_Run797(8, 0xd000);
    Field_Run798(8, 2);
    Field_Run799(8);
    Field_Run800(9, 3);
    Field_Run801(9, 2);
    Field_Run802(9);
    Field_Run803(27, 0x5000);
    Field_Run804(27, 3);
    Field_Run805(27, 1);
    Field_Run806(27);
    Field_Place152(27, 0xcccc, 0x6666);
    Field_Place153(27, 0x198, 158);
    Field_Place154(27, 0x198, 148);
    Field_Run807(27, 0, 20);
    Field_Run808(27, 1);
    Field_Run809(27);
    Field_Run810(1, 0x8000, 20);
    Field_Run811(2, 1, 0);
    Field_Place155(27, 0x198, 134);
    Field_Run812(27, 0x1b8, 134);
    Field_Run813(40);
    Field_Run814(9, 10, 0);
    Field_Do52(0x926);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    Field_Run815();
    Field_Run816(15, 0, 1);
    Field_Run817(8, 1);
    Field_Run818(20);
    Field_Place156(8, 0xcccc, 0x6666);
    Field_Place157(8, 0x1d4, 0x266);
    Field_Place158(8, 0x1d8, 0x254);
    Field_Place159(8, 0x8000, 20);
    Field_Run819(8, 4, 20);
    rec7 = Field_Run13();
    Field_Run820(20);
    Field_Run821(214);
    Field_Run822(rec7, 0x200e738);
    Field_Run823(40);
    Field_Run824(8, 3);
    Field_Run825(20);
    Field_Place160(8, 0x1d2, 0x270);
    Field_Check50(8, 0x5000);
    Field_Run826(8, 2);
    Field_Do53(0x1e3b);
    Field_Run827(8, 0, 20);
    Field_Run828(9, 11, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_Run829();
    Field_Run830(15, 1, 1);
    Field_Place161(8, 0x5000, 40);
    Field_Run831(8, 2);
    Field_Do54(0x1e3d);
    Field_Run832(8, 0, 20);
    Field_Run833(9, 11, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_Run834();
    Field_Run835(24, 0, 0);
    Field_Run836(18, 0, 0);
    Field_Run837(0, 0, 0);
    Field_Place162(16, 0x960000, 0x24a0000);
    Field_Run14(0x9c0000, -1, 0x2180000, 0x1000001);
    Field_Run838(8, 0, 0);
    Field_Place163(16, 0xcccc, 0x6666);
    Field_Place164(16, 168, 0x242);
    Field_Place165(16, 168, 0x22a);
    Field_Place166(16, 0x8000, 20);
    Field_Run839(16, 2);
    Field_Do55(0x1e3c);
    Field_Run840(16, 0, 20);
    Field_Run841(9, 12, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_Run842();
    Field_Run843(0, 0, 0);
    Field_Run844();
    Field_Place167(18, 0x960000, 0x24a0000);
    Field_Run15(0x9c0000, -1, 0x2180000, 0x1000001);
    Field_Run845(8, 0, 0);
    Field_Place168(18, 0xcccc, 0x6666);
    Field_Place169(18, 168, 0x242);
    Field_Place170(18, 168, 0x22a);
    Field_Place171(18, 0x8000, 20);
    Field_Run846(18, 2);
    Field_Do56(0x1e3c);
    Field_Run847(18, 0, 20);
    Field_Run848(9, 12, 0);
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

    Field_Run849();
    Field_Run850(9, 5);
    Field_Run851(24, 1, 0);
    Field_Run852(0, 0, 0);
    Field_Run853(17, 0, 0);
    Field_Run854(0);
    Field_Run855(8, 1, 20);
    Field_Run856(0x6666, 0xccc);
    Field_Run857(0x1b80000, -1, 0xb00000, 1);
    Field_Run858(20);
    Field_Run859(9, 7);
    Field_Run860(30);
    Field_Run861(0xbc);
    Field_Run862(30);
    Field_Run863(16);
    Field_Run864(0x50);
    Field_Run865(0);
    Field_Run866(0x3c);
    Field_Run867(9, 7);
    Field_Run868(30);
    Field_Run869(0xbc);
    Field_Run870(30);
    Field_Run871(16);
    Field_Run872(0x50);
    Field_Run873(0);
    Field_Run874(0x5a);
    Field_Run875(0xbc);
    Field_Run876(30);

    *(u32 *)(gWork + (224 << 1)) = (224 << 1) + 67;

    Field_Run877(9, 0, 0);

    if (Field_Check123((s32)&Value_0000092b) != 0) {
        Field_Run878(20);
    } else if (Field_Check124((s32)&Value_0000092a) != 0) {
        Field_Run879(18);
    } else if (Field_Check125((s32)&Value_00000929) != 0) {
        Field_Run880(17);
    } else if (Field_Check126((s32)&Value_00000928) != 0) {
        Field_Run881(16);
    } else {
        Field_Run882(13);
    }
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_Run883();
    Field_Run884(15, 1, 1);
    Field_Run885(8, 1);
    Field_Run886(10);
    Field_Place172(8, 0x3000, 20);
    Field_Run887(8, 2);
    Field_Run888((s32)gVal);
    Field_Run889(8, 0, 20);
    Field_Run890(9, 14, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;

    Field_Run891();
    Field_Run16(-1, -1, -1, 0);
    Field_Run892(1);
    Field_Run893(15, 1, 1);
    Field_Run894(8, 1);
    Field_Do57(0x1e43);
    Field_Run895(8);
    Field_Place173(8, 0xd000, 40);
    Field_Run896(9, 15, 0);
}

void Scene_RunScene3b1(void)
{
    extern u8 gOv26[];
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_200e8e4;

    Field_Run897();
    Field_Run898(24, 1, 0);
    Field_Run899(25, 1, 0);
    Field_Run900();
    Field_Run901(19, 11, 12);
    Field_Run902(10, 6);
    Field_Check51(12, 0x200e840);
    base5_200e8e4 = (s32)gOv15;
    Field_Run903(36, base5_200e8e4);
    Field_Check52(37, base5_200e8e4);
    Field_Check53(38, base5_200e8e4);
    Field_Run904(36, 3);
    Field_Run905(37, 3);
    Field_Apply18(38, 3);
    Field_Run906();
    Field_Run907();
}

/*
 * A flat setter sequence, no branches; the final call takes no arguments.
 * The 60-byte owner at 0x02004218 includes its one pool word, the address
 * taken as Value_00000928.
 */
void Scene_RunActors24And25Setup(void)
{
    extern u8 *gWork;

    Field_Run908();
    Field_Run909(24, 1, 0);
    Field_Run910(25, 0, 0);
    Field_Run911(19, 11, 12);
    Field_Run912();
    Field_Run913((s32)&Value_00000928);
    Field_Run914();
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

    if (Field_Check127((s32)&Value_00000928) != 0) {
        u8 *obj = Field_Run47(0, 0);
        Field_Run915(obj, 0xcd << 17, 0xac << 16);
        Field_Run916(7, obj, src);
        Field_Run917(10, 0, 0);
    } else {
        Field_Run918(5, 10, src);
    }

    if (Field_Check128((s32)&Value_00000929) != 0) {
        u8 *obj = Field_Run48(1, 0);
        Field_Run919(obj, 0xeb << 17, 0xac << 16);
        *(u32 *)(Field_Run49(obj) + 24) = 0xffff0000;
        Field_Run920(7, obj, src);
        Field_Run921(11, 0, 0);
    } else {
        Field_Run922(6, 11, src);
    }

    if (Field_Check129((s32)&Value_0000092a) != 0) {
        u8 *obj = Field_Run50(2, 0);
        Field_Run923(obj, 0xcd << 17, 0xcc << 16);
        Field_Run924(7, obj, src);
        Field_Run925(12, 0, 0);
    } else {
        Field_Run926(5, 12, src);
    }

    if (Field_Check130((s32)&Value_0000092b) != 0) {
        u8 *obj = Field_Run51(3, 0);
        Field_Run927(obj, 0xeb << 17, 0xcc << 16);
        *(u32 *)(Field_Run52(obj) + 24) = 0xffff0000;
        Field_Run928(7, obj, src);
        Field_Run929(13, 0, 0);
    } else {
        Field_Run930(6, 13, src);
    }

    Field_Run931(5, 14, src);
    Field_Run932(6, 15, src);
    Field_Run933(5, 16, src);
    Field_Run934(6, 17, src);
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
    Field_Run935(a, 0);
    Field_Run936(10);
}

void Scene_CallPairWith10(s32 a, u16 b)
{
    Field_Run937(a, b, 10);
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
 * call but not masked, so they stay full `s32`. `Field_Run72` returns a
 * pointer whose halfword at offset 6 is then set to `d`.
 */
void OvObj_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d)
{
    Field_Run938(a, b << 16, c << 16, d);
    *(s16 *)((u8 *)Field_Run72(a) + 6) = d;
}

void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags)
{
    u32 selected;

    Field_SetMode(x, y, z, ~flags & 1);
    selected = flags & 0x1111;
    if ((flags & 0x10000000) != 0)
        Field_Run939();
    if ((flags & 0x01000000) != 0)
        Field_Run940();
    Field_Do110(selected);
}

void Scene_RunSceneStep(s32 step, u32 arg, u32 opt)
{
    extern const s32 gOv26[];
    extern u8 gWork[];

    u32 slot;

    switch (step) {
    case 0:
        Field_Run941(0, 0, 0);
        Field_Place174(1, 0x8000, 0);
        Value(2, 0, 0);
        Field_Place175(3, 0x8000, opt);
        break;
    case 1:
        Field_Run942(0, arg, 0);
        Field_Run943(1, arg, 0);
        Field_Run944(2, arg, 0);
        Field_Place175(3, arg, opt);
        break;
    case 2:
        Field_Run945(0, 3);
        Field_Run946(1, 3);
        Field_Run947(2, 3);
        Field_Run948(3, 3);
        if (arg != 0) {
            Field_Run949(3);
        }
        if (opt == 0) {
            break;
        }
        Field_Run950(opt);
        break;
    case 3:
        Field_Run17(0, 0x102);
        Field_Run18(1, 0x102);
        Field_Run19(2, 0x102);
        Field_Run20(3, 0x102);
        Field_Run950(opt);
        break;
    case 4:
        for (slot = 0; slot < arg; slot++) {
            Field_Run951(slot + 10, 0, 0);
        }
        break;
    case 5:
        {
            u8 *rec;

            rec = Field_Run73(arg);
            SetPose(rec, 0x5000);
        }
        Field_Run952(arg, 5);
        Field_Run953(arg, opt);
        break;
    case 6:
        {
            u8 *rec;

            rec = Field_Run74(arg);
            SetPose(rec, 0x5000);
            *(s32 *)(rec + 24) = -0x10000;
        }
        Field_Run952(arg, 5);
        Field_Run953(arg, opt);
        break;
    case 7:
        {
            u8 *rec;

            rec = Value4(arg);
            SetPose(rec, 0x5000);
        }
        Field_Run954(arg);
        if (opt == 0) {
            Field_Run955(arg, 0);
        }
        break;
    case 8:
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x202;
        Field_Run956();
        if (arg != 0) {
            Value5();
        }
        Field_Run957(0);
        break;
    case 9:
        Field_Run958();
        Field_Run959();
        if (arg == 0) {
            break;
        }
        Field_Run960(arg);
        break;
    case 10:
        Field_Run961(24, 1, 0);
        Field_Run962(25, 0, 0);
        Field_Run963(0);
        Field_Run964(0, 0x1b0, 168, 0x4000);
        Field_Run965(1, 0x1c0, 168, 0x4000);
        Field_Run966(2, 0x1a8, 152, 0x4000);
        Field_Run967(3, 0x1ca, 152, 0x4000);
        break;
    case 11:
        if (arg != 0) {
            u8 *rec;

            Field_Run968(13, 1);
            rec = Field_Run75(13);
            SetPose(rec, 0x3000);
            rec = Field_Run76(13);
            *(s32 *)(rec + 24) = 0x10000;
        }
        {
            u8 *rec;

            Field_Run969(14, 1);
            rec = Field_Run77(14);
            SetPose(rec, 0x5000);
            Field_Run970(15, 1);
            rec = Field_Run78(15);
            SetPose(rec, 0x3000);
            rec = Field_Run79(15);
            *(s32 *)(rec + 24) = 0x10000;
            Field_Run971(16, 1);
            rec = Field_Run80(16);
            SetPose(rec, 0x5000);
            Value6(17, 1);
            rec = Field_Run81(17);
            SetPose(rec, 0x3000);
            rec = Field_Run82(17);
            *(s32 *)(rec + 24) = 0x10000;
        }
        Field_Place176(28, 0x19a0000, 0xae0000);
        Field_Place177(29, 0x1d60000, 0xae0000);
        Field_Place178(30, 0x19a0000, 0xce0000);
        Field_Place179(31, 0x1d60000, 0xce0000);
        Field_Place180(32, 0x19a0000, 0x11e0000);
        Field_Place181(33, 0x1d60000, 0x11e0000);
        Field_Place182(34, 0x19a0000, 0x13c0000);
        Field_Place183(35, 0x1d60000, 0x13c0000);
        Field_Run972(1);
        if (arg != 0) {
            Field_Place184(13, 0xb000, 0);
        }
        Field_Place185(14, 0xd000, 0);
        Field_Place186(15, 0xb000, 0);
        Field_Place187(16, 0xd000, 0);
        Field_Run973(17, 0xb000);
        break;
    case 12:
        {
            u8 *rec;

            rec = Field_Run83(arg);
            Field_Run974(arg, 1);
            if (opt != 0) {
                SetPose(rec, 0x3000);
            } else {
                SetPose(rec, 0x5000);
            }
            *(s32 *)(rec + 24) = 0x10000;
        }
        break;
    case 13:
        Field_Run975(9, 0, 0);
        Field_Run976(12, 0, 0);
        Field_Run977(11, 0, 0);
        Field_Run978(13, 0, 0);
        Field_Run979(10, 0, 0);
        break;
    case 14:
        Field_Run980(14, 0, 0);
        Field_Run979(13, 0, 0);
        break;
    case 15:
        Field_Run981(24, 1, 0);
        Field_Run982(9, 0, 0);
        Field_Run983(10, 0, 0);
        Field_Run984(8, 0x1bc, 0x266, 0xd000);
        Field_Run985(0, 0, 0);
        if (arg != 0) {
            Field_Run986();
        }
        Field_Run987(0x1c00000, 0x200000, 0x2700000, 0x1000001);
        if (opt == 0) {
            break;
        }
        *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x202;
        Field_Run988();
        Field_Run989();
        Value7(20);
        break;
    case 16:
        Field_Run990(8, 0, 0);
        Field_Run991(9, 0, 0);
        Field_Place188(27, 0x1b60000, 0x980000);
        break;
    case 17:
        slot = 0;
        do {
            Field_Run992(slot + 28, 0, 0);
            slot++;
        } while (slot <= 7);
        break;
    case 18:
        Field_Run993(12, 152, 0x214, 0xb000);
        Field_Run994(8, 134, 0x1ea, 0x3000);
        Field_Run995(9, 166, 0x1ea, 0x5000);
        Field_Run996(10, 182, 0x1f8, 0x5000);
        Field_Run997(11, 118, 0x1f8, 0x3000);
        Scene_RunSceneStep(14, 0, 0);
        break;
    case 19:
        Field_Run998(8, 0x1a0, 0x148, 0);
        Field_Run999(9, 0x1c0, 0x160, 0xd000);
        Field_Run1000(10, 0x1c6, 248, 0x3000);
        Field_Run1001(arg, 0x198, 0x122, 0);
        Field_Run1002(opt, 0x198, 0x156, 0);
        Field_Run1003(13, 0x1a4, 0x164, 0xd000);
        Field_Run1004(14, 0x198, 0x130, 0);
        Field_Run1005(15, 0x1a2, 0x17a, 0xd000);
        Field_Run1006(16, 0x1b8, 0x106, 0x3000);
        Field_Run1007(17, 0x1c0, 0x17a, 0xd000);
        break;
    case 20:
        for (slot = arg; slot <= opt; slot++) {
            Field_Run1008(slot);
        }
        break;
    case 21:
        Field_Run1009(20, 0x92c, 0x93d);
        Field_Run1010(20, 0x917, 0x91f);
        Field_Run1011(20, 0x990, 0x998);
        Field_Run1012(0x300);
        Field_Run1013(0x301);
        Field_Run1014(0x302);
        break;
    case 22:
        Field_Run1015(1);
        Field_Run1016(23, 0, 0);
        Field_Run1017(12, gOv26);
        break;
    case 23:
        Field_Run1018(1);
        Field_Run1019(2);
        Field_Run1020(3);
        break;
    case 24:
        Field_Run21(-1, -1, -1, 0);
        Field_Run1021(1);
        if (arg != 0) {
            *(u8 *)(Field_Run84() + 0x55) = 0;
        }
        break;
    case 25:
        Field_Run1022(gOv22);
        Field_Run1023(1);
        if (arg == 1) {
            Field_Run1024(gOv23);
            Field_Run1025(1);
        } else if (arg == 2) {
            Field_Run1024(gOv24);
            Field_Run1025(1);
        } else if (arg == 3) {
            Field_Run1026(gOv25);
            Field_Run1027(1);
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
    Field_Run1028(8, 0x92c);
    Field_Run1029(8, 0x935);
    Field_Run1030(8, 0x917);
    Field_Run1031(8, 0x990);
}

/*
 * Scan slots 0 through 8 inclusive.  On the first a1 that Field_Check131
 * accepts, call Field_Run1032 and stop.  a0 and a1 advance together.
 */
void Actor_RunFirstMatchingSlot(s32 a0, s32 a1)
{
    unsigned int i = 0;

    do {
        if (Field_Check131(a1)!= 0) {
            Field_Run1032(a0, 0, 0);
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

    rec8 = Field_Check54(0, 0);
    Field_Run1033();
    Field_Run1034(24, 1, 0);
    Field_Run1035(25, 2, 0);
    Field_Run1036();
    Field_Place189(19, rec8, 12);
    Field_Run1037(10, 6);
    base5_200e840 = (s32)gOv26;
    Field_Run1038(rec8, base5_200e840);
    Field_Run1039(11);
    Field_Check55(12, base5_200e840);
    base5_200e8e4 = (s32)gOv15;
    Field_Run1040(36, base5_200e8e4);
    Field_Run1041(37, base5_200e8e4);
    Field_Run1042();
    Field_Run1043();
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

    Field_Run1044(0x1b80000, -1, 0xb00000, 0x1000001);
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
    Field_Run1045(1); /* main:080000c0 */
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
    Field_Run1046(10); /* main:0808a1d8 */
    /* Text/dialog resource pointer, passed by base address and by base+1. */
    base5_1e46 = (s32)gVal2;
    Field_Run1047(base5_1e46, 1, 10); /* main:08015210 */
    Battle_WaitMode0_2(10);
    Field_Run1048(0, 0, 40);
    Field_Place190(1, 0x4000, 20);
    Motion_SetSpeedLim_1(0x39999, 0x7333); /* main:0808a208 */
    Field_Run22(0x1b80000, -1, 0x1400000, 0x10000014);
    Motion_SetVarCbObj_1(8, 2); /* main:0808a138 */
    Field_Run23(8, 0xd000);
    SceneWork_SetStepValue_1((base5_1e46 + 1)); /* main:0808a170 */
    Field_Run1049(8);
    Motion_ArmCb_5(8, 0, 20);
    Field_Run24(0x1b80000, -1, 0x860000, 0x10000000);
    /* Second text/dialog resource pointer, shared across three calls. */
    base5_200e7c8 = (s32)gOv16;
    Motion_EnableActCb_1(1, base5_200e7c8);
    Motion_EnableActCb_2(2, base5_200e7c8);
    Motion_MarkActiveAndSetActionCallback_1(3, base5_200e7c8); /* main:0808a0b0 */
    Battle_WaitMode0_3(40);
    GameFlag_Set_1(0x301);
    Field_Run1050(23, 0, 0);
    GameFlag_Clear_1(0x12f);
}

void Scene_RunFormationAndEffectPresentation(void)
{
    s32 action;

    Field_Run1051(0, 0x1bc, 0x12c, 0);
    Field_Run1052(1, 0x1ca, 0x136, 0);
    Field_Run1053(2, 0x1bc, 0x14a, 0);
    Field_Run1054(3, 0x1b0, 0x136, 0);
    Field_Run1055(27, 0x1b8, 134, 0x8000);
    Field_Run1056(10, 0x1c6, 248, 0x3000);
    Field_Run1057(10, 6);
    Field_Run1058(0x1b80000, -1, 0x1340000, 0x1000001);
    Field_Run1059();
    Field_Run1060();
    Field_Run1061(20);
    Field_Place191(0, 0x2000, 0);
    Field_Place192(1, 0xa000, 0);
    Field_Place193(2, 0xa000, 0);
    Field_Place194(3, 0x2000, 40);
    Field_Run1062(2, 1, 20);
    Field_Do58(0x1e6e);
    Field_Run1063(27);
    Field_Run1064(1, 0xc000, 0);
    Field_Run25(0x26666, 0x4ccc);
    Field_Run26(0x1b80000, -1, 0xb00000, 1);
    Field_Place195(27, 0x19999, 0xcccc);
    Field_Place196(27, 0x198, 134);
    Field_Place197(27, 0x198, 152);
    Field_Place198(27, 0x1a8, 164);
    Field_Run27(0x19999, 0x3333);
    Field_Run28(0x1b80000, -1, 0x12c0000, 1);
    Field_Place199(27, 0x1a8, 222);
    Field_Place200(27, 0x1a8, 0x106);
    Field_Run1065(27, 0x3000, 20);
    Field_Run1066(27, 1);
    Field_Run1067(27);
    Field_Run1068(2, 1, 20);
    Field_Run1069(27, 3);
    Field_Run1070(27, 1);
    Field_Run1071(27);
    Field_Run1072(3, 2, 60);
    Field_Run1073(1, 0xe000, 60);
    Field_Run1074(27, 0, 40);
    Field_Run1075(27, 1);
    Field_Run1076(27, 2);
    Field_Place201(27, 0x1b0, 0x10c);
    Field_Place202(27, 0x1c4, 0x10c);
    Field_Run1077(27, 1);
    Field_Run1078(27, 0xd000);
    Field_Run1079(27, 2);
    Field_Run1080(27, 0, 20);
    Field_Run1081(1, 0xc000, 20);
    Field_Run1082(27, 4);
    Field_Run1083(40);
    Field_Run1084(27, 0, 80);
    Field_Run1085(27, 1);
    Field_Run1086(20);
    Field_Run1087(27, 3);
    Field_Run1088(10);
    Field_Run1089(27, 0x5000);
    Field_Check56(27, 0);
    if (Field_Check57(0, 0) == 0) {
        Field_Run1090(27, 3);
        Field_Run1091(27);
    } else {
        Field_Run1092(27, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        Field_Run1093(27);
        Field_Run1094(3, 2, 40);
        Field_Run1095(27, 1);
        Field_Run1096(27, 3);
        Field_Run1097(27);
    }
    Field_Run1098(2, 1, 20);
    action = (s32)gOv20;
    Field_Run1099(1, action);
    Field_Run1100(2, action);
    Field_Run1101(3, action);
    Field_Run29(0x9999, 0x1333);
    Field_Run30(0x1b80000, -1, 0xb00000, 1);
    Field_Place203(0, 0x10000, 0x8000);
    Field_Place204(0, 0x1a8, 0x110);
    Field_Place205(0, 0x1a8, 164);
    Field_Run1102(60);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
    Field_Run1103(9, 0, 0);
    Field_Do59(0x301);
    Field_Do60(0x927);
    Field_Run1104(4);
}

void Scene_RunActors24And25SetupWithValue929(void)
{
    extern u8 *gWork;

    s32 handle = Field_Check132(0, 0);

    Field_Run1105();
    Field_Run1106(24, 1, 0);
    Field_Run1107(25, 0, 0);
    Field_Run1108(0);
    Field_Run1109(19, handle, 12);
    Field_Run1110(11, 0, 0);
    Field_Run1111();
    Field_Run1112((s32)&Value_00000929);
    Field_Run1113();
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

    rec8 = Field_Check58(0, 0);
    rec2 = Field_Check59(1, 0);
    Field_Run1114();
    Field_Run1115(24, 1, 0);
    Field_Run1116(25, 3, 0);
    Field_Run1117();
    Field_Place206(19, rec8, rec2);
    Field_Run1118(10, 6);
    base5_200e840 = (s32)gOv26;
    Field_Run1119(rec8, base5_200e840);
    Field_Run1120(11);
    Field_Run1121(rec2, base5_200e840);
    Field_Run1122(12);
    base5_200e8e4 = (s32)gOv15;
    Field_Run1123(36, base5_200e8e4);
    Field_Check60(37, base5_200e8e4);
    Field_Run1124(36, 3);
    Field_Apply19(37, 3);
    Field_Run1125();
    Field_Run1126();
}

/*
 * A flat setter sequence, no branches. The 108-byte owner at 0x02005780
 * includes its one pool word, the address taken as Value_0000092a.
 */
void Scene_RunActors24And25SetupWithValue92a(void)
{
    extern u8 *gWork;

    s32 handle = Field_Check133(0, 0);
    s32 other = Field_Check134(1, 0);

    Field_Run1127();
    Field_Run1128(24, 1, 0);
    Field_Run1129(25, 0, 0);
    Field_Run1130(0);
    Field_Run1131(19, handle, other);
    Field_Run1132(11, 0, 0);
    Field_Run1133(12, 0, 0);
    Field_Run1134();
    Field_Run1135((s32)&Value_0000092a);
    Field_Run1136();
}

void Scene_RunExtendedFormationPresentation(void)
{
    s32 slot_b;
    s32 slot_c;
    s32 slot_a;
    u8 *record;
    s32 action;

    slot_a = Field_Check61(0, 0);
    slot_b = Field_Check135(1, 0);
    slot_c = Field_Check62(2, 0);
    Field_Run1137();
    Field_Run1138(10, 0, 0);
    Field_Run1139(17, 0, 0);
    Field_Place207(8, 0x1d80000, 0x980000);
    Field_Run1140(9, 5);
    Field_Place208(27, 0x1b80000, 0x860000);
    Field_Run1141(27, 15);
    record = Field_Run70(27);
    Field_Run1142(record, 0);
    Field_Run1143(16);
    Field_Run1144(0x1b60000, -1, 0xae0000, 0x1000001);
    Field_Run1145(8, 1, 20);
    Field_Do61(19);
    Field_Run1146(181);
    Field_Place209(0x20000, 0x20000, 0x10000);
    Field_Run1147(10);
    Field_Place210(-1, -1, 0xe666);
    Field_Run1148(80);
    Field_Run1149(181);
    Field_Place211(0x20000, 0x20000, 0x10000);
    Field_Run1150(10);
    Field_Place212(-1, -1, 0xe666);
    Field_Run1151(63);
    Field_Do62(0x11a);
    Field_Run31(3, 0x102);
    Field_Run1152(40);
    Field_Run1153(3, 0x6000);
    Field_Do63(0x1ec1);
    Field_Run1154(3, 0, 40);
    Field_Run1155(27);
    Field_Place213(0, 0x2000, 0);
    Field_Place214(1, 0xa000, 0);
    Field_Run1156(2, 0, 0);
    Field_Place215(3, 0xe000, 40);
    Field_Run1157(0, 0x6000, 0);
    Field_Place216(1, 0xe000, 0);
    Field_Run1158(2, 0x6000, 0);
    Field_Place217(3, 0x8000, 40);
    Field_Place218(2, 0xe000, 0);
    Field_Place219(2, 0x100, 60);
    Field_Run1159(2, 0x2000);
    Field_Run1160(2, 1);
    Field_Run1161(2);
    Field_Run1162(0, 1);
    Field_Run1163(1, 1);
    Field_Run1164(3, 1);
    Field_Run1165(10);
    Field_Place220(0, 0xc000, 0);
    Field_Place221(1, 0xc000, 0);
    Field_Place222(2, 0xe000, 0);
    Field_Place223(3, 0xa000, 20);
    Field_Run1166(27, 0);
    record = Field_Run71(27);
    Field_Run1167(record, 1);
    Field_Place224(27, 0x10000, 0x8000);
    Field_Place225(27, 0x1ae, 134);
    Field_Run1168(27, 0x3000);
    Field_Run1169(27, 2);
    Field_Run1170(27);
    Field_Run1171(slot_a, 1);
    Field_Run1172(slot_b, 1);
    Field_Run1173(slot_c, 1);
    Field_Run1174(13, 1);
    Field_Run32(slot_a, 0x102);
    Field_Run33(slot_b, 0x102);
    Field_Run34(slot_c, 0x102);
    Field_Run35(13, 0x102);
    Field_Run1175(40);
    Field_Run1176(12, slot_a, 0);
    Field_Run1177(12, slot_b, 1);
    Field_Run1178(12, slot_c, 0);
    Field_Run1179(11, 1, 0);
    Field_Place226(slot_a, 0xd000, 0);
    Field_Place227(slot_b, 0xb000, 0);
    Field_Place228(slot_c, 0xd000, 0);
    Field_Run1180(0, 0, 0);
    Field_Place229(1, 0x8000, 0);
    Field_Run1181(2, 0, 0);
    Field_Place230(3, 0x8000, 40);
    Field_Run1182(27, 2);
    Field_Run1183(27, 0);
    Field_Place231(0, 0xc000, 0);
    Field_Place232(1, 0xc000, 0);
    Field_Place233(2, 0xc000, 0);
    Field_Place234(3, 0xc000, 20);
    Field_Place235(27, 0x1b8, 134);
    Field_Run1184(27, 0, 0);
    Field_Run1185(1, 0x8000);
    Field_Run1186(1, 1);
    Field_Run1187(1);
    Field_Run1188(2, 0, 0);
    Field_Run1189(3, 0x8000);
    Field_Run1190(0, 3);
    Field_Run1191(1, 3);
    Field_Run1192(2, 3);
    Field_Run1193(3, 3);
    Field_Place236(1, 0x10000, 0x8000);
    Field_Place237(2, 0x10000, 0x8000);
    Field_Place238(3, 0x10000, 0x8000);
    action = (s32)gOv21;
    Field_Run1194(1, action);
    Field_Run1195(2, action);
    Field_Run1196(3, action);
    Field_Do64(0x302);
    *(s32 *)0x0200ff84 = 0;
    Field_Check63((s32)Field_Run1197, 0xc80);
    Field_Run1198(23, 0, 0);
    Field_Run1199(27);
    Field_Do65(0x12f);
    Field_Do66(0x927);
    Field_Run1200();
}

void RunActorsEightAndNineMapEvent(void)
{
    Field_Run1201();
    Field_Run1202(15, 1, 0);
    Field_Run1203(9, 468, 616, 32768);
    Field_Run1204(8, 1, 20);
    Field_Run1205(9, 2);
    Field_Run1206(20);
    SetDirection(8, 53248, 80);
    Field_Run1207(8, 0, 20);
    Field_Run1208(8, 3);
    Field_Run1209(20);
    Field_Run1210(9, 21, 0);
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

    rec2 = Field_Check64(0, 0);
    rec8 = Field_Check136(1, 0);
    rec7 = Field_Check137(2, 0);
    rec4 = Field_Check65(3, 0);
    Field_Run1211();
    Field_Run1212(10, 0, 0);
    Field_Run1213(8, 0x1d8, 144, 0x5000);
    Field_Run1214(27, 0x198, 142, 0x3000);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x201;
    Field_Run1215();
    Field_Run1216();
    Field_Run1217(40);
    Field_Run1218(27, 1);
    Field_Do67(0x1f78);
    Field_Run1219(27);
    Field_Run1220(rec2, 2);
    Field_Run1221(rec8, 2);
    Field_Run1222(rec7, 2);
    Field_Run1223(rec4, 2);
    Field_Run1224(20);
    Field_Run1225(rec2, 0, 0);
    Field_Place239(rec8, 0x8000, 0);
    Field_Run1226(rec7, 0, 0);
    Field_Place240(rec4, 0x8000, 40);
    Field_Place241(rec2, 0x10000, 0x8000);
    Field_Place242(rec8, 0x10000, 0x8000);
    Field_Place243(rec7, 0x10000, 0x8000);
    Field_Place244(rec4, 0x10000, 0x8000);
    Field_Place245(rec2, 0x1d6, 172);
    Field_Place246(rec8, 0x19a, 172);
    Field_Place247(rec7, 0x1d6, 204);
    Field_Place248(rec4, 0x19a, 204);
    Field_Run1227(rec2, 1);
    Field_Run1228(rec8, 1);
    Field_Run1229(rec7, 1);
    Field_Place249(rec8, 0xd000, 0);
    Field_Place250(rec2, 0xb000, 0);
    Field_Place251(rec4, 0xd000, 0);
    Field_Place252(rec7, 0xb000, 20);
    Field_Run1230(27, 1);
    Field_Run1231(27);
    Field_Run1232(rec2, 3);
    Field_Run1233(rec8, 3);
    Field_Run1234(rec7, 3);
    Field_Run1235(rec4, 3);
    Field_Run1236(27);
    Field_Run1237(rec2, 3);
    Field_Run1238(rec8, 3);
    Field_Run1239(rec7, 3);
    Field_Run1240(rec4, 3);
    Field_Run1241(27, 0, 0);
    Field_Check138(0, 0x8000);
    Field_Run1242(0, 3);
    Field_Run1243(27, 3);
    Field_Place253(27, 0x10000, 0x8000);
    Field_Run1244(27, 0x198, 132);
    Field_Place254(27, 0x1bc, 132);
    Field_Run1245(27, 0, 0);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c0) = 0x202;
    Field_Run1246();
    Field_Run1247();
    Field_Run36(0x92c, 0x935);
    Field_Run37(0x917, 0x990);
    Field_Do68(0x8a0);
    Field_Run1248(10);
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
        if (Field_Check139(p)!= 0) {
            Field_Run1249(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (Field_Check140(p)!= 0) {
            Field_Run1250(p);
            break;
        }
        cnt_b++;
    }

    Field_Run1251(b + cnt_a);
    Field_Run1252(a + cnt_b);
}
