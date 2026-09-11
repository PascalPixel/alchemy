#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/actor_transfer.h"

#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define ACTOR 8

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} StorySpawnRecord;

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

struct StoryDialogueWork {
    u8 reserved000[370];
    u16 story_result;
};

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct StorySelection {
    u8 reserved000[500];
    s32 actor_id;
};

struct StorySelectionActor {
    u8 reserved00[6];
    u16 presentation;
};

struct StoryProgressWork {
    u8 reserved000[386];
    u16 state_one_marker;
};

/* Complete prologue-less shared-state activation leaf through its pool. */
struct StorySharedState {
    u8 reserved00[52];
    u8 active;
};

struct StoryCompletionWork {
    u8 reserved000[386];
    s16 scene_value;
};

/*
 * Per-frame step of a thirty-two frame rise -- resource_371.  The halfword
 * counter at +0x64 is incremented in place and compared as a signed sixteen
 * bit value; past 31 the effect is handed off and nothing else happens.  The
 * anchor pointer at +0x68 is loaded before the frame test and must stay
 * there.  The angle is the frame shifted left by ten, and the callee that
 * turns it into a 16.16 amplitude is identified only by that use.
 */
struct StoryVerticalEffectActor {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct StoryVerticalEffectActor *anchor;  /* 0x68 */
};

/*
 * The mirror of the rise: the same counter at +0x64 and anchor at +0x68,
 * with the second amplitude store negated, the depth offset subtracted
 * rather than added, and a fixed depth bias of 0x100000.  Its magnitude
 * callee, like the one above, is identified only by use -- one angle in, one
 * 16.16 magnitude out.
 */
struct StoryVerticalEffectActor {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct StoryVerticalEffectActor *anchor;  /* 0x68 */
};

extern u8 gOv[];
extern u8 gOv2[];
extern u8 Value_00000847;
extern u8 Value_000002f1;
extern u8 gOv3[];
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
extern u8 Value_0000085a;
extern u8 gOv15[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern u8 gVal5[];
extern u8 gVal6[];
extern u8 gVal7[];
extern u8 gOv16[];
extern s32 gCell[][1];
extern u8 gOv17[];
extern u8 gUnk[];
extern u8 gOv18[];
extern u8 gOv19[];
extern u8 gOv20[];
extern u8 gOv21[];
extern s32 gOv22[];
extern struct StorySharedState *gIw;

u8 *Story_Run95();

struct Object *Story_Run96(u8 *);
struct Object *Story_Run97(s32);

struct Object *Story_Run98(u8 *);
struct Object *Story_Run99(s32);

u8 *Story_Run100(s32);

u8 *Story_Run101();

u8 *Story_Run102();

u8 *Story_Run103();

u8 *Story_Run104();

u8 *Story_Run105();

u8 *Story_Run106();
u8 *Story_Run107();

u8 *Story_Run108();

u8 *Story_Run109();

u8 *Story_Run110();

struct StorySelectionActor *Story_Run111(s32 actor);

void *Story_Run112(s32 actor);

/*
 * Story-progress triggers -- resource_371.  Each fires one event once the
 * progress word has reached nine tenths of the level word; each seventy-two
 * byte owner includes its three pool words.
 */

/* Actor callbacks for the story transfer scene. */

/*
 * Each Func_ name labels the call word of one call site rather than a
 * runtime address; the declarations stay old-style because the interfaces
 * are unknown.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The actor index every call in this scene step operates on. */

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

/*
 * The progress word is gCell2[284] read as a whole word and the level
 * word is the workspace at +428; both offsets are built in one register, so
 * the order in which the locals are declared is what reproduces the
 * reference.  Each Func_ name labels the call word of one call site rather
 * than a runtime address, so the same service appears under several names,
 * and the declarations stay old-style because the interfaces are unknown and
 * argument counts differ between call sites.
 */
void StoryProgress_TriggerEvent0808(void)
{
    extern s16 gCell2[];
    extern u8 *gWork;

    u8 *workspace = gWork;
    s16 *state_table = gCell2;
    s32 *progress = (s32 *)&state_table[284];
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Story_Apply(*level * 9, 10)) {
        if ((u32)Story_Check24() < 0x8000) {
            Story_Apply2(0x808, 3);
            *(s32 *)(workspace + 424) = 0;
        } else {
            *progress = *level;
        }
    }
}

void StoryProgress_TriggerEvent0809(void)
{
    extern s16 gCell2[];
    extern u8 *gWork;

    s16 *state_table = gCell2;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = gWork;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Story_Check25(*level * 9, 10)) {
        Story_Run113(0x809, 42);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryProgress_TriggerEvent080A(void)
{
    extern s16 gCell2[];
    extern u8 *gWork;

    s16 *state_table = gCell2;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = gWork;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Story_Check26(*level * 9, 10)) {
        Story_Run114(0x80a, 24);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryActor_AdvanceTimer(u8 *actor)
{
    extern u32 gIw2;

    u16 *timer = (u16 *)(actor + 0x64);

    /*
     * Arm order decides the branch sense: the fall-through is the increment
     * and the taken branch is the call.  Swapping the arms inverts the test.
     */
    if (*(s16 *)timer <= 0) {
        *timer = (u16)(*timer + 1);
    } else {
        Story_Run115(actor);
    }
}

void StoryActor_ConfigureSpawnedObject(u8 *actor)
{
    extern u32 gIw2;

    s32 fixed_scale;
    u8 *spawned_actor;
    u8 *spawned_record;

    if ((gIw2 & 4) != 0) {
        fixed_scale = 0x14ccc;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    } else {
        fixed_scale = 0x10000;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    }

    if ((gIw2 & 2) == 0) {
        return;
    }

    {
        s32 x = *(s32 *)(actor + 0x08);
        s32 y = *(s32 *)(actor + 0x0c);
        s32 z = *(s32 *)(actor + 0x10);
        spawned_actor = Story_Run95(0x11d, x, y, z);
    }
    Story_Run116(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Story_Run117(spawned_actor, 0);
        Story_Run118(spawned_actor, 1);
        *(u16 *)(spawned_actor + 0x64) = zero_value;
        *(s32 *)(spawned_actor + 0x6c) = (s32)gOv;
    }
}

s32 StoryActor_Initialize(u8 *actor)
{
    extern u32 gIw2;

    u8 *actor_flags;
    s32 fixed_scale;

    if (Story_Check27(0x30) != 0) {
        return 0;
    }
    if (Story_Check28(0x16E) != 0) {
        return 0;
    }
    *(s32 *)(actor + 0x6C) = (s32)gOv2;
    actor_flags = actor + 0x55;
    *actor_flags = 0;
    actor_flags += 0xF;
    *(u16 *)actor_flags = 0;
    actor_flags += 2;
    *(u16 *)actor_flags = 0;
    fixed_scale = 0x8000;
    *(s32 *)(actor + 0x18) = fixed_scale;
    *(s32 *)(actor + 0x1C) = fixed_scale;
    return 0;
}

u8 *SceneData_GetTableD27C(void)
{
    return gOv3;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableDA2C(void)
{
    return gOv4;
}

s32 StoryActor_ApplyFlaggedMode(u8 *actor)
{
    extern u32 gIw2;

    Story_Run119();
    if (Story_Check29((s32)&Value_00000847) != 0) {
        Story_Apply3(actor, 2);
    }
    return 1;
}

s32 StoryActor_ResetPosition(u8 *actor)
{
    extern u32 gIw2;

    s32 zero;
    Story_Apply4(actor, 0);
    Story_Apply5(actor, 10);
    {
        u8 *mode_flags = actor + 0x59;
        zero = 0;
        *mode_flags = zero;
    }
    if (Story_Check30(0x8A0) != 0) {
        Story_Do30((s32)&Value_000002f1);
        *(s32 *)(actor + 8) = zero;
        *(s32 *)(actor + 12) = zero;
    }
    return 0;
}

/*
 * The popped register is r1, so r0 survives the return and is the result.
 * The owner includes its alignment halfword and its one pool word, the
 * address of gIw2 -- a live status word, not overlay image data.
 * The exclusive or against the bit just tested clears bit 0 only.  value is
 * a byte, so value >> 8 is zero and both branches do the same clear and
 * store; that shape is deliberate and decides the register allocation.
 */
s32 StoryActor_ClearActiveFlag(u8 *actor)
{
    extern u32 gIw2;

    u8 *active_flags = actor + 0x54;
    u8 value = *active_flags;
    u32 active_bit = 1 + (value >> 8);

    if ((active_bit & value) != 0 && (gIw2 & active_bit) != 0) {
        u8 cleared = 1;

        if (active_flags) {
            cleared ^= value;
            *active_flags = (u8)cleared;
        } else {
            cleared ^= value;
            *active_flags = (u8)cleared;
        }
    }
    return 1;
}

/* Each Func_ name identifies one call site's loader-relocated call word rather
 * than a runtime address, so several names here reach the same import. The
 * declarations are old-style because the interfaces are not established. */

/*
 * Selects the placement and spawn table for the current scene. The selector is
 * the signed halfword gCell2[225]; only 49 through 80 are covered and
 * everything else takes the default arm, which calls Story_Run120 before
 * returning. Cases 49 and 64 are conditional and fall through to the default
 * when their test fails. The case arms are in the order the reference uses,
 * not ascending, and that order is what reproduces it.
 */
u8 *StoryScene_SelectPlacementTable(void)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    s16 *scene_table = gCell2;
    s32 scene_id = scene_table[225];

    switch (scene_id) {
    case 49:
        if (Story_Check31(0x94f) == 0 && Story_Check32(0x941) != 0) {
            return gOv8;
        }
        break;
    case 64:
        if (Story_Check33(0x85a) == 0) {
            return gOv7;
        }
        break;
    case 65:
    case 70:
        return gOv9;
    case 71:
        return gOv11;
    case 72:
        return gOv13;
    case 73:
        return gOv14;
    case 66:
    case 67:
    case 68:
    case 69:
    case 75:
        return gOv10;
    case 80:
        return gOv12;
    default:
        break;
    }

    Story_Run120(0x235);
    return gOv6;
}

/* Publishes one of two branch values at +0x170 of the scene state, chosen by
 * comparing the other actor's x against the subject's. */
void StoryScene_SetBranchValueFromX(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Story_Run96(actor_object - 0x64);
    scene_table = gCell2;
    other_actor = Story_Run97(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->x < subject_actor->x) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Story_Do31(0x7B);
}

/* The same branch value, chosen on z instead of x. */
void StoryScene_SetBranchValueFromZ(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Story_Run98(actor_object - 0x64);
    scene_table = gCell2;
    other_actor = Story_Run99(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->z < subject_actor->z) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Story_Do32(0x7B);
}

void State_SetValues130_6_47(void)
{
    extern volatile s32 gIw2;

    Story_Place138(0x82, 6, 0x2F);
}

void State_ApplyValues150And46And11(void)
{
    extern volatile s32 gIw2;

    Story_Place139(0x96, 0x2E, 0x0B);
}

void State_ApplyValues116And56And21(void)
{
    extern volatile s32 gIw2;

    Story_Place140(0x74, 0x38, 0x15);
}

void State_ApplyValues151And25And54(void)
{
    extern volatile s32 gIw2;

    Story_Place141(0x97, 0x19, 0x36);
}

void Scene_RunStep7D3B1E(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    Story_Place142(0x7D, 0x3B, 0x1E);
}

u8 *SceneData_GetTableE3F4(void)
{
    return gOv5;
}

void State_ApplyFlag85aBranch(void)
{
    extern volatile s32 gIw2;

    if (Story_Check34((s32)&Value_0000085a) == 0) {
        Story_Do33(101);
    } else {
        Story_Do34(123);
        Story_Do35(3);
    }
}

void Scene_RunStep74(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    Story_Run122();
    Story_Do36(74);
}

void RunEventScript01(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Story_Run123();
    Story_Run124();
    Story_Run125();
    Story_Run126();
    Story_Do(0x94f);
    Story_Place(11, 0x16e00000, 0x49c0000);
    Story_Run127(11, 24, 8);
    Story_Run128(11);
    Story_Run129(60);
    Story_Place2(12, 0x16e00000, 0x49c0000);
    Story_Run130(12, 12, 24);
    Story_Run131(30);
    Story_Place3(11, 0x5000, 0);
    Story_Place4(12, 0xd000, 0);
    Story_Run132(60);
    Story_Run133(11, 3);
    Story_Run134(12, 3);
    Story_Run135(120);
    Story_Place5(8, 0x16f80000, 0x4b80000);
    Story_Run136(60);
    Story_Run137(12, 2);
    record = Story_Check(8);
    if (record != 0) {
        Story_Run138(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run139(12);
    Story_Run140(12, 0, 0);
    ((void (*)())Story_Check35)(60);
    Story_Run141(11, 2);
    record = Story_Check2(8);
    if (record != 0) {
        Story_Run142(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run143(11);
    Story_Run144(11, 0, 0);
    ((void (*)())Story_Check36)(60);
    Story_Run145(0, 2);
    record = Story_Check3(8);
    if (record != 0) {
        Story_Run146(0, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run147(0);
    Story_Run148(0, 0, 0);
    ((void (*)())Story_Check37)(60);
    Story_Place6(8, 0x8000, 0x4000);
    Story_Run149(8, 56, 8);
    Story_Run150(8);
    Story_Run151(8, 40, 40);
    Story_Run152(8);
    Story_Run153(8, 8, 88);
    Story_Run154(8);
    Story_Run155();
    Story_Run156(108);
    Story_Run157();
}

void Scene_RunActorTransferSequence(void)
{
    u8 *actor;
    u8 *record;
    s32 scale;
    s32 action;

    actor = Pointer1(Story_Run101, 15);
    Story_Run158();
    Story_Run(0x14000, 1);
    Story_Run159(4);
    Story_Run160();
    Story_Run161();
    Story_Run162();
    Story_Run2(-1, -1, -1, 0);
    Story_Run163(1);
    Story_Place7(0, 0x19999, 0xcccc);
    Story_Place8(1, 0x19999, 0xcccc);
    Story_Place9(0, 0x16fc, 0x628);
    Story_Place10(0, 0x6000, 0);
    Story_Place11(8, 0x16d80000, 0x6280000);
    Story_Run164(1);
    Story_Run165(8, 15);
    record = Story_Run102(8);
    Story_Run166(record, 0);
    Story_Place12(10, 0x19999, 0x6666);
    Story_Place13(11, 0x19999, 0x6666);
    Story_Place14(12, 0x19999, 0x6666);
    Story_Place15(13, 0x19999, 0x6666);
    Story_Run167(141);
    Story_Run3(10, 0x200c9e4);
    Story_Run168(20);
    Story_Run4(11, 0x200cb50);
    Story_Run169(20);
    Story_Run5(12, 0x200cc74);
    Story_Run170(20);
    Story_Run6(13, (s32)gOv18);
    Story_Do2(0x121);
    record = Pointer1(Story_Run103, 0);
    if (record != 0) {
        Story_Run171(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Story_Place16(1, 0x1704, 0x640);
    Story_Place17(0, 0x2000, 0);
    Story_Place18(1, 0xa000, 20);
    Story_Place19(0, 0x101, 0);
    Story_Place20(1, 0x101, 60);
    Story_Place21(8, 0x16d80000, 0x6380000);
    Story_Run172(1);
    Story_Do3(0x1215);
    Story_Run173(8, 0, 10);
    Story_Place22(0, 0x6000, 0);
    Story_Place23(1, 0x6000, 40);
    Story_Place24(0, 0x2000, 0);
    Story_Place25(1, 0xa000, 60);
    Story_Place26(0, 0x6000, 0);
    Story_Place27(1, 0x6000, 10);
    Story_Run174(141);
    Story_Run7(10, 0x200ca78);
    Story_Run175(20);
    Story_Run8(11, 0x200cbe4);
    Story_Run176(20);
    Story_Run9(12, 0x200cd08);
    Story_Run177(10);
    Story_Place28(0, 0x4000, 0);
    Story_Place29(1, 0x4000, 10);
    Story_Run10(13, 0x200ce2c);
    Story_Do4(0x121);
    Story_Run178(20);
    Story_Run179();
    Story_Run180(10, 1);
    Story_Run181(11, 1);
    Story_Run182(12, 1);
    Story_Run183(13, 1);
    Story_Run11(0x16080000, -1, 0x6f80000, 1);
    Story_Run184();
    Story_Run185(20);
    Story_Run186();
    Story_Place30(9, 0x16080000, 0x6d80000);
    Story_Run187(1);
    Story_Place31(9, 0x13333, 0x9999);
    Story_Place32(9, 0x1608, 0x6c8);
    Story_Place33(9, 0x15f8, 0x6c8);
    Story_Place34(9, 0x15f8, 0x6f8);
    Story_Run188(20);
    Story_Run189(9, 2);
    Story_Run190(20);
    Story_Run12(9, 0x102);
    Story_Run191(60);
    Story_Run192(9, 0, 20);
    Story_Run193(9, 3);
    Story_Run194(9, 0, 20);
    Story_Place35(8, 0x16180000, 0x6f80000);
    Story_Run195(1);
    Story_Run196(8, 0);
    record = Story_Run104(8);
    Story_Run197(record, 1);
    Story_Place36(8, 0xcccc, 0x6666);
    Story_Place37(8, 0x1608, 0x6f8);
    Story_Run198(20);
    Story_Run199(8, 2);
    Story_Place38(0x2008, 0, 10);
    Story_Place39(8, 0x3000, 60);
    Story_Place40(8, 0x8000, 10);
    Story_Run13(8, 0x102);
    Story_Run200(60);
    Story_Place41(9, 0x3000, 0);
    Story_Place42(8, 0x3000, 40);
    Story_Run14(8, 0x102);
    Story_Run201(60);
    Story_Run202(8, 2);
    Story_Place43(0x2008, 0, 40);
    Story_Run203(9, 1);
    Story_Run204(9, 0, 10);
    Story_Run205(9, 0, 10);
    Story_Place44(8, 0x105, 60);
    Story_Place45(0x2008, 0, 10);
    Story_Run206(8, 1);
    Story_Run207(8, 3);
    Story_Place46(0x2008, 0, 10);
    Story_Place47(9, 0x101, 60);
    Story_Run208(9, 0, 20);
    Story_Run209(8, 3);
    Story_Run210();
    Story_Run211(107);
    Story_Run15(0x40000, 0x40000);
    Story_Run212();
    Story_Do5(0x121);
    Story_Place48(8, 0x100, 0);
    Story_Place49(9, 0x100, 0);
    Story_Place50(8, 0x8000, 0);
    Story_Run213(9, 0, 40);
    Story_Place51(8, 0xb000, 0);
    Story_Place52(9, 0xb000, 0);
    Story_Run16(0x10000, 0x2000);
    Story_Run17(0x15e80000, -1, 0x6c80000, 1);
    Story_Run214();
    Story_Place53(14, 0x15a80000, 0x6a80000);
    Story_Run215(1);
    Story_Place54(14, 0x4ccc, 0x2666);
    Story_Run18(14, 0x200cebc);
    Story_Run216(160);
    *(s32 *)(actor + 72) = 0x1999;
    *(s32 *)(actor + 68) = 0x1999;
    *(s32 *)(actor + 24) = 0x18000;
    *(s32 *)(actor + 28) = 0x18000;
    {
        s32 shown = 0;

        *(u16 *)(actor + 100) = shown;
    }
    *(s32 *)(actor + 12) = 0x400000;
    {
        u8 *target = *(u8 **)(actor + 80);
        s32 shown = 0xf000;

        *(u16 *)(target + 30) = shown;
    }
    Story_Run217(actor, 0);
    Story_Run218(actor, 2);
    Story_Run219(1);
    record = Story_Run105(15);
    Story_Run220(record, 0);
    Story_Run19(0x20097a5, 0xc80);
    do {
        Story_Run221(1);
    } while (*(s16 *)(actor + 100) == 0);
    record = Story_Run106(15);
    Story_Run222(record, 0);
    record = Story_Run107(14);
    Story_Run223(record, 0);
    Story_Run224(10);
    scale = 192;
    record = Story_Run108(9);
    *(s32 *)(record + 40) = (scale << 11);
    record = Pointer1(Story_Run109, 8);
    *(s32 *)(record + 40) = (scale << 11);
    Story_Run225(145);
    Story_Run20(0x40000, 0x40000);
    Story_Run226();
    Story_Run227();
    Story_Run228(60);
    Story_Run21(0x20000, 0x4000);
    Story_Run22(0x16080000, -1, 0x6f80000, 1);
    Story_Run229();
    Story_Run230();
    Story_Run23(9, 0x102);
    Story_Run24(8, 0x102);
    Story_Run231(60);
    Story_Do6(0x20097a5);
    Story_Run232(1);
    Story_Run233(14, 0, 0);
    Story_Run234(15, 0, 0);
    Story_Place55(8, 0x8000, 10);
    Story_Run235(8, 4, 40);
    Story_Place56(0x2008, 0, 10);
    Story_Run236(9, 0, 10);
    Story_Run237(9, 0, 20);
    Story_Place57(8, 0xc000, 40);
    Story_Place58(0x2008, 0, 20);
    Story_Run238(9, 4, 20);
    Story_Run239(9, 0, 10);
    Story_Run240(8, 1);
    Story_Place59(8, 0x8000, 10);
    Story_Place60(0x2008, 0, 10);
    Story_Run25(9, 0x102);
    Story_Run241(80);
    Story_Run242(8, 3);
    Story_Run243(20);
    Story_Run244(9, 1);
    Story_Run245(9, 3);
    Story_Place61(8, 0x1618, 0x6f8);
    Story_Run246(8, 0, 0);
    Story_Place62(9, 0x15f8, 0x6c8);
    Story_Place63(9, 0x1608, 0x6c8);
    Story_Place64(9, 0x1608, 0x6d8);
    Story_Run247(9, 0, 0);
    Story_Run248(141);
    Story_Run26(10, (s32)gOv19);
    Story_Run27(11, 0x200cc30);
    Story_Run249(40);
    Story_Run28(12, 0x200cd54);
    Story_Run250(40);
    Story_Run29(13, (s32)gOv20);
    Story_Run251();
    Story_Place65(0, 0x170c0000, 0x6280000);
    Story_Place66(1, 0x17140000, 0x6400000);
    Story_Run30(0x40000, 0x8000);
    Story_Run31(0x16d80000, -1, 0x6480000, 1);
    Story_Run252();
    action = (s32)gOv21;
    Story_Run253(10, action);
    Story_Run254(20);
    Story_Run32(0x6666, 0xccc);
    Story_Run33(0x16d80000, -1, 0x6080000, 1);
    Story_Run255(11, action);
    Story_Run256(20);
    Story_Run257(12, action);
    Story_Run258(20);
    Story_Place67(0, 0x8000, 0);
    Story_Place68(1, 0x8000, 0);
    Story_Run259(13, action);
    Story_Run260(40);
    Story_Place69(0, 0xc000, 0);
    Story_Place70(1, 0xc000, 0);
    Story_Run261(13);
    Story_Do7(0x121);
    Story_Run34(0x40000, 0x8000);
    Story_Run35(0x16f80000, -1, 0x6480000, 1);
    Story_Run262();
    Story_Place71(0, 0x2000, 0);
    Story_Place72(1, 0xa000, 80);
    Story_Run263(0, 3);
    Story_Run264(1, 3);
    Story_Run265(20);
    Story_Run266(1, 2);
    record = Pointer1(Story_Run110, 0);
    if (record != 0) {
        Story_Run267(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run268(1);
    Story_Run269(1, 0, 0);
    Story_Run36(0xcccc, 0x1999);
    Story_Run37(0x16d80000, -1, 0x6480000, 1);
    Story_Place73(0, 0xcccc, 0x6666);
    Story_Place74(0, 0x16d8, 0x628);
    Story_Run270();
    Story_Run271();
    Story_Do8(0x85a);
    Story_Run272(3);
    Story_Run273();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 record;

    Story_Run38(0x160c0000, -1, 0x6f80000, 1);
    Story_Run274(4);
    Story_Run39(0x16040000, -1, 0x6fc0000, 1);
    Story_Run275(4);
    Story_Run40(0x160c0000, -1, 0x6f40000, 1);
    Story_Run276(4);
    Story_Run41(0x160c0000, -1, 0x6fc0000, 1);
    Story_Run277(4);
    Story_Run42(0x16040000, -1, 0x6f40000, 1);
    Story_Run278(4);
    Story_Run43(0x16080000, -1, 0x6f80000, 1);
    Story_Run279(4);
    Story_Run44(0x160a0000, -1, 0x6f80000, 1);
    Story_Run280(4);
    Story_Run45(0x16060000, -1, 0x6fa0000, 1);
    Story_Run281(4);
    Story_Run46(0x160a0000, -1, 0x6f60000, 1);
    Story_Run282(4);
    Story_Run47(0x160a0000, -1, 0x6fa0000, 1);
    Story_Run283(4);
    Story_Run48(0x16060000, -1, 0x6f60000, 1);
    Story_Run284(4);
    Story_Run49(0x16080000, -1, 0x6f80000, 1);
    Story_Run285(4);
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 record;

    Story_Run50(0x15ec0000, -1, 0x6c80000, 1);
    Story_Run286(4);
    Story_Run51(0x15e40000, -1, 0x6cc0000, 1);
    Story_Run287(4);
    Story_Run52(0x15ec0000, -1, 0x6c40000, 1);
    Story_Run288(4);
    Story_Run53(0x15ec0000, -1, 0x6cc0000, 1);
    Story_Run289(4);
    Story_Run54(0x15e40000, -1, 0x6c40000, 1);
    Story_Run290(4);
    Story_Run55(0x15e80000, -1, 0x6c80000, 1);
    Story_Run291(4);
    Story_Run56(0x15ea0000, -1, 0x6c80000, 1);
    Story_Run292(4);
    Story_Run57(0x15e60000, -1, 0x6ca0000, 1);
    Story_Run293(4);
    Story_Run58(0x15ea0000, -1, 0x6c60000, 1);
    Story_Run294(4);
    Story_Run59(0x15ea0000, -1, 0x6ca0000, 1);
    Story_Run295(4);
    Story_Run60(0x15e60000, -1, 0x6c60000, 1);
    Story_Run296(4);
    Story_Run61(0x15e80000, -1, 0x6c80000, 1);
    Story_Run297(4);
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check4(15);
    record = Story_Check5(14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (Story_Check6(0x200) == 0) {
            Story_Run298(145);
            Story_Run299(rec7, 3);
            Story_Do9(0x200);
            {
                u16 *target = (u16 *)(rec7 + 100);
                s32 shown = 1;

                *target = shown;
            }
        }
    }
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check7(8);
    Story_Run300();
    Story_Run62(-1, -1, -1, 0);
    Story_Run301(1);
    Story_Run302(0, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Story_Run303(8, 1);
    Story_Run304();
    Story_Place75(8, 0x6666, 0x3333);
    Story_Place76(8, 0x14a8, 0x918);
    Story_Run305();
    Story_Run306();
    Story_Do10(0x927);
    Story_Run307(102);
    Story_Run308();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check8(8);
    Story_Run309();
    Story_Run63(-1, -1, -1, 0);
    Story_Run310(1);
    Story_Run311(0, 0, 0);
    Story_Place77(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Story_Run312(1);
    Story_Run313(8, 1);
    Story_Run314();
    Story_Place78(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Story_Run315(8, 0x200d158);
    do {
        Story_Run316(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Story_Run317();
    Story_Run318();
    Story_Do11(0x927);
    Story_Run319(103);
    Story_Run320();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check9(8);
    Story_Run321();
    Story_Run64(-1, -1, -1, 0);
    Story_Run322(1);
    Story_Run323(0, 0, 0);
    Story_Place79(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Story_Run324(1);
    Story_Run325(8, 1);
    Story_Run326();
    Story_Place80(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Story_Run327(8, 0x200d158);
    do {
        Story_Run328(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Story_Run329();
    Story_Run330();
    Story_Do12(0x927);
    Story_Run331(104);
    Story_Run332();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check10(8);
    Story_Run333();
    Story_Run65(-1, -1, -1, 0);
    Story_Run334(1);
    Story_Run335(0, 0, 0);
    Story_Place81(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Story_Run336(1);
    Story_Run337(8, 1);
    Story_Run338();
    Story_Place82(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Story_Run339(8, 0x200d158);
    do {
        Story_Run340(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Story_Run341();
    Story_Run342();
    Story_Do13(0x927);
    Story_Run343(105);
    Story_Run344();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check11(8);
    Story_Run345();
    Story_Run66(-1, -1, -1, 0);
    Story_Run346(1);
    Story_Run347(0, 0, 0);
    Story_Place83(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Story_Run348(1);
    Story_Run349(8, 1);
    Story_Run350();
    Story_Place84(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    if (Story_Run67() == 11) {
        Story_Run351(8, 0x200d1b8);
    } else {
        Story_Run352(8, 0x200d158);
    }
    do {
        Story_Run353(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Story_Run354();
    Story_Run355();
    Story_Do14(0x927);
    Story_Run356(106);
    Story_Run357();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check12(8);
    Story_Run358();
    Story_Run68(-1, -1, -1, 0);
    Story_Run359(1);
    Story_Run360(0, 0, 0);
    Story_Place85(8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Story_Run361(1);
    Story_Run362(8, 1);
    Story_Run363();
    Story_Place86(8, 0x6666, 0x3333);
    Story_Place87(8, 0x13c8, 0x918);
    Story_Run364();
    Story_Run365();
    Story_Do15(0x93e);
    Story_Do16(0x927);
    Story_Run366(107);
    Story_Run367();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 record;

    Story_Run368();
    Story_Run69(0x10000, 6);
    Story_Run369();
    Story_Run370();
    Story_Run371(8, 2);
    Story_Do17(0xc66);
    Story_Run372(8, 0);
    Story_Run373(30);
    Story_Run374(111);
    Story_Run375(0, 2);
    Story_Do18(0x16f);
    Story_Do19(0x171);
    Story_Run376();
    Story_Run377(8, 4, 30);
    Story_Do20(0xc67);
    Story_Run378(8, 0);
    Story_Do21(0x16f);
    Story_Do22(0x171);
    Story_Run379();
    Story_Run380(30);
    Story_Run381(12, 6);
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check13(10);
    Story_Run382();
    Story_Run70(-1, -1, -1, 0);
    Story_Run383(1);
    Story_Run384(0, 0, 0);
    Story_Run385(1);
    *(volatile s32 *)(rec7 + 24) = 0x18000;
    *(volatile s32 *)(rec7 + 28) = 0x18000;
    {
        s32 shown = 0x4000;

        *(volatile u16 *)(rec7 + 6) = shown;
    }
    Story_Run386();
    Story_Run387();
    Story_Run388(20);
    Story_Place88(10, 0x15680000, 0x8380000);
    Story_Run389(1);
    Story_Run390(141);
    Story_Place89(10, 0x19999, 0x6666);
    Story_Run391(10, 2);
    Story_Place90(10, 0x156d, 0x858);
    Story_Run71(0x6666, 0xccc);
    Story_Run72(0x15b80000, -1, 0x8580000, 1);
    Story_Place91(10, 0x159e, 0x858);
    Story_Place92(10, 0x15a8, 0x86e);
    Story_Place93(10, 0x15e8, 0x878);
    Story_Run392(10, 1);
    Story_Do23(0x121);
    Story_Run393(20);
    Story_Place94(0, 0x15d80000, 0x8780000);
    Story_Run394(1);
    Story_Run395(0, 6, 0);
    Story_Place95(0, 0x20000, 0x10000);
    Story_Place96(0, 0x15c8, 0x878);
    Story_Run396(0, 0, 40);
    Story_Run397(141);
    Story_Run398(10, 2);
    Story_Place97(10, 0x15f8, 0x878);
    Story_Place98(0, 0xe000, 0);
    Story_Place99(10, 0x15f8, 0x838);
    Story_Place100(0, 0xc000, 0);
    Story_Place101(10, 0x15bd, 0x838);
    Story_Place102(10, 0x15b8, 0x853);
    Story_Place103(0, 0xa000, 0);
    Story_Place104(10, 0x1572, 0x858);
    Story_Place105(10, 0x1568, 0x838);
    Story_Run399(10, 0, 0);
    Story_Do24(0x121);
    Story_Run400(40);
    Story_Run73(0x15d80000, -1, 0x8580000, 1);
    Story_Place106(0, 0xcccc, 0x6666);
    Story_Place107(0, 0x15d8, 0x858);
    Story_Run401();
    Story_Run402();
    Story_Run403(20);
    Story_Run404();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 record;

    Story_Run405();
    Story_Run406(55, 0, 0);
    Story_Do25(0x2642);
    Story_Check14(*(volatile s32 *)gOv15, 0);
    Story_Place108(55, 0x3000, 0);
    Story_Run407();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 record;

    Story_Run408();
    Story_Run409();
    Story_Do26(0x2643);
    Story_Check15(*(volatile s32 *)gOv15, 0);
    Story_Run410();
    Story_Place109(0, 0x10000, 0x8000);
    Story_Place110(0, 0x1778, 0xd48);
    Story_Run411();
}

/* Runs dialogue 0x264c and publishes the story result when flag 0x234 is
 * set. */
void StoryScene_ShowRewardDialogue(void)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    Story_Run412();
    Story_Run413();
    Story_Apply6(0x264c, 1);
    if (Story_Check38(0x234) != 0) {
        gWork->story_result = 1;
    }
    Story_Run414();
    Story_Run415();
}

void Scene_RunActorPresentationSequence(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 record;
    s32 base2_2000240;
    s32 base3_2000240;

    base2_2000240 = (s32)gCell2;
    base3_2000240 = (s32)gCell2;
    Story_Run416(*(u8 *)((base2_2000240 + 0x205)), *(u8 *)((base3_2000240 + 0x206)));
    Story_Run417();
    Story_Run74(0x10000, 0x12c);
    Story_Run75(-1, -1, -1, 0);
    Story_Run418(5, 19);
    Story_Run419(8, 5);
    Story_Run420(0, 0, 0);
    Story_Run421(1);
    Story_Run76(0x18000, 16);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_Run77(0x10003, 1);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    Story_Run422();
    Story_Run423();
    Story_Run424();
    Story_Run425(40);
    Story_Run426(5, 1);
    Story_Run427(20);
    Story_Do27(0x2913);
    Story_Run428(5, 0, 20);
    Story_Run429(8, 2);
    Story_Run430(20);
    Story_Run431(8, 0, 20);
    Story_Place111(5, 0x107, 20);
    Story_Run432(5, 0, 20);
    Story_Place112(8, 0x105, 80);
    Story_Run433(8, 0, 10);
    Story_Run434(5, 2);
    Story_Run435(5, 0, 20);
    Story_Place113(8, 0x105, 100);
    Story_Place114(5, 0x105, 40);
    Story_Run436(5, 0, 20);
    Story_Run437(8, 0, 10);
    Story_Place115(5, 0x102, 20);
    Story_Run438(5, 0, 10);
    Story_Run78(8, 0x102);
    Story_Run439(80);
    Story_Run440(8, 0, 20);
    Story_Place116(5, 0x105, 80);
    Story_Run441(8, 0, 120);
    Story_Run442(5, 1);
    Story_Run443(5, 0, 40);
    Story_Run444(8, 0, 20);
    Story_Place117(5, 0x105, 40);
    Story_Run445(5, 0, 120);
    Story_Place118(9, 0x6666, 0x3333);
    Story_Place119(9, 0x1ddc0000, 0xd840000);
    Story_Place120(9, 0x1d94, 0xd8c);
    Story_Place121(9, 0x1d88, 0xda0);
    Story_Run446(20);
    Story_Place122(0x6009, 0, 20);
    Story_Place123(8, 0x101, 0);
    Story_Place124(5, 0x101, 60);
    Story_Run447(9, 3);
    Story_Run79(0x6009, 0);
    Story_Run448();
    Story_Run449(20);
    Story_Run80(9, 0x200cf7c);
    Story_Run450(80);
    Story_Run451(8, 1);
    Story_Run452(8, 4, 40);
    Story_Run453(5, 1);
    Story_Run454(5, 4, 60);
    Story_Place125(8, 0x3000, 0);
    Story_Place126(5, 0xb000, 40);
    Story_Place127(8, 0x9999, 0x4ccc);
    Story_Place128(5, 0x9999, 0x4ccc);
    Story_Run81(8, 0x200d01c);
    Story_Run455(20);
    Story_Run82(0xb333, 0x1666);
    Story_Run83(0x1e380000, -1, 0xdc80000, 1);
    Story_Run456(5, 0x200d0a8);
    do {
        Story_Run457(10, 6);
        Story_Run458(6, 8);
        Story_Run459(1);
    } while (*(s16 *)(Story_Check39(5) + 100) == 0);
    Story_Run460(20);
    Story_Place129(9, 0x8000, 20);
    Story_Run84(8, 0x102);
    Story_Run85(5, 0x102);
    Story_Run461(40);
    Story_Run462();
    Story_SetRect(5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
    Story_Run463(20);
    Story_Run464();
    Story_Run86(0x10000, 0x2000);
    Story_Run87(0x1e580000, -1, 0xdc80000, 1);
    Story_Place130(9, 0x3000, 0);
    Story_Place131(8, 0x19999, 0xcccc);
    Story_Place132(5, 0x19999, 0xcccc);
    Story_Place133(8, 0x1e7c, 0xdb8);
    Story_Place134(5, 0x1e6c, 0xdd8);
    Story_Run465(8, 1);
    Story_Run466();
    Story_Run467(80);
    Story_Run468(8, 1);
    Story_Run469(8, 0, 20);
    Story_Run470(5, 2);
    Story_Place135(0x1005, 0, 40);
    Story_Place136(8, 0x8000, 20);
    Story_Run471(8, 2);
    Story_Run472(8, 0, 60);
    Story_Run473();
    Story_Run474(17);
    Story_Run475(0, 0);
    Story_Run476(120);
    Story_Run477(120);
    Story_Run478((s32)gVal, 10);
}

/* Stages the scene transition and arms the timed callback that drives it. */
void StoryScene_StartTransition(void)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    Story_Run479();
    Story_Do37(141);
    Story_Apply7(0, 0);
    Story_Apply8(0, 0);
    Story_Do38(1);
    Story_Do39(2);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 1;
    Story_Run480();
    Story_Run481();
    Story_Apply9(0, 15);
    Story_Apply10(Story_Run100(0), 0);
    Story_Do40(1);
    Story_Apply11(0x40000, 0x8000);
    {
        s32 transition_delay = 3200;
        void *transition_callback = (void *)0x0200b1fd;
        Story_Apply12(transition_callback, transition_delay);
    }
    Story_Apply13(0, 0);
    Story_Apply14(0x10004, 1);
    Story_Apply15(0x10000, 2);
    Story_Do41(40);
    Story_Do42(240);
    Story_Apply16(0, 0);
    Story_Do43(80);
    Story_Do44(90);
    Story_Do45(109);
    Story_Do46(282);
    Story_Run482();
}

/* Drives actor 8 through a series of position/threshold setup calls and
 * advances the shared scene phase before the scene runs. */
void Scene_RunActorEightApproach(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 actor;

    Battle_Reset_1();
    Motion_CamBounds_1(-1, -1, -1, 0);
    Story_Run483(1);
    Object_SetModeById_1(ACTOR, 2);
    Motion_SetHPosTerrain_1(ACTOR, 0x13080000, 0x3280000);
    actor = Scene_GetRecord_1(ACTOR);
    {
        /* Write 0xa000 to the halfword at +6 of the actor record. */
        s32 value = 0xa000;

        *(u16 *)(actor + 6) = value;
    }
    Story_Run484(1);
    RatioHistory_RecordAndScheduleCallback_1(0x13333, 1);
    Motion_SetHPosTerrain_2(0, 0, 0);
    Motion_SetPosReset_1(ACTOR, 1);
    Story_Run485(1);
    SCENE_PHASE = 0x100;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1(ACTOR, 0x6666, 0x3333);
    Motion_SetPositionAndCommit_1(ACTOR, 0x12d8, 0x2c8);
    Motion_SetPositionAndCommit_2(ACTOR, 0x12a8, 0x268);
    Motion_SetSpeed_2(ACTOR, 0x4ccc, 0x2666);
    Motion_SetPositionAndCommit_3(ACTOR, 0x12a8, 0x1d8);
    Motion_SetSpeed_3(ACTOR, 0x3333, 0x1999);
    Motion_SetPositionAndCommit_4(ACTOR, 0x1298, 0x1c8);
    Motion_SetSpeed_4(ACTOR, 0x1999, 0xccc);
    Motion_SetPositionAndCommit_5(ACTOR, 0x1298, 0x1b8);
    Object_SetModeById_2(ACTOR, 1);
    Battle_WaitMode0_2(40);
    Story_Run486();
    Motion_SetSpeed_5();
    Story_Run487(110);
}

/* Totals slots 0 and 2 against slots 1 and 3 and returns the difference. */
s32 StoryScene_ComputeOpposingSlotDelta(void)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    s32 positive_total = Story_Check40(0);
    s32 negative_total;

    positive_total += Story_Check41(2);
    negative_total = Story_Check42(1);
    negative_total += Story_Check43(3);
    return positive_total - negative_total;
}

/* Complete reference-actor-54 selected-actor setup wrapper. */
s32 StoryReward_LookupBySelection(u32 selection)
{
    extern volatile s32 gIw2;

    s32 flag_base = 0;
    u32 offset;

    switch (selection) {
    case 0:
        flag_base = 0x92C;
        break;
    case 1:
        flag_base = 0x935;
        break;
    case 2:
        flag_base = 0x917;
        break;
    case 3:
        flag_base = 0x990;
        break;
    }
    for (offset = 0; offset < 9; offset++) {
        if (Story_Check44(flag_base + offset) != 0) return gOv22[offset];
    }
    return 0;
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_Check16(8);
    Story_Run488(60);
    Story_Run489();
    Story_Run88(0x9999, 1);
    *(s32 *)(rec7 + 24) = 0x13333;
    *(s32 *)(rec7 + 28) = 0x13333;
    Story_Run490(8, 1);
    Story_Run491(1);
    Story_Run492(0, 15);
    record = Story_Check45(0);
    Story_Run493(record, 0);
    record = Story_Check46(8);
    Story_Run494(record, 0);
    Story_Place137(8, 0x6666, 0x3333);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Story_Check17(8, 0x200d218);
    Story_Check18(0x200b4a1, 0xc80);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_Run89(0x10003, 1);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c8) = 32;
    Story_Run495();
    Story_Run496(120);
    Story_Run90(0x16666, 0x12c);
    Story_Do28(0x10e);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c8) = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Story_Run497();
    Story_Run498();
    Story_Run499(111);
}

void StoryScene_UpdateSelectedActorProgress(void)
{
    extern volatile s32 gIw2;

    extern struct StorySelection gCell2;
    extern struct StoryProgressWork *gWork;

    struct StorySelectionActor *actor;
    struct StoryProgressWork *scene;
    s32 progress;

    actor = Story_Run111(gCell2.actor_id);
    scene = gWork;
    actor->presentation = (u16)(gIw2 << 12);

    progress = Story_Check47(0x2f8);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (Story_Check48(0x106) == 0) {
            progress -= 1;
        }
    }
    Story_Apply17(0x2f8, progress);
}

void Scene_RunOpeningAuxiliarySequence(s32 a0)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = gCell2;
    p6 = *(volatile s32 *)(base + 500);
    rec7 = Story_Check19((s32)p6);
    rec2 = Story_Check20(0x2f0);
    if (rec2 == 0) {
        Story_Run500();
        Story_Run91((s32)p6, 0x101);
        Story_Run501((s32)p6, 9);
        record = Story_Check21(a0);
        if (record != 0) {
            Story_Run502((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Story_Run503((s32)p6);
        Story_Run504(244);
        Story_Check22(0x200b679, 0xc80);
        rec7[85] = rec2;
        {
            s32 shifted = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;

            Story_Run505((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), shifted, *(volatile s32 *)((s32)rec7 + 16));
        }
        Story_Run506((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Story_Do29(0x2f0);
        Story_Run92(0x2f8, 180);
        Story_Run507();
        *(u16 *)((*(u8 *volatile *)gWork + 0x17c)) = rec2;
    }
}

void StoryScene_SetReferenceActor(void)
{
    extern volatile s32 gIw2;

    Story_Do47(54);
}

void StoryScene_ActivateSharedState(void)
{
    extern volatile s32 gIw2;

    gIw->active = 1;
}

/* Publish the actor-98 scene state and restore its selected actor. */
void StoryScene_CompleteActor98(void)
{
    extern volatile s32 gIw2;

    extern u8 gCell2[];
    extern struct StoryCompletionWork *gWork;

    u8 *selected_actor;

    if (gWork->scene_value == 99) {
        gWork->scene_value = 0;
    }
    Story_Do48(0x2f0);
    Story_Do49(0x2f1);
    Story_Apply18(0x2f8, 0);
    Story_Apply19(98, 5);
    gCell2[0x22b] = 3;
    Story_Apply20(98, 7);
    selected_actor = Story_Run112(*(s32 *)(gCell2 + 500));
    selected_actor[85] = 2;
}

/* Restore the blend registers using the active display bank's mask. */
void Effect_RestoreBlendRegisters(void)
{
    extern u16 gOv23;
    extern u32 gIw2;

    Story_Apply21(0x04000050, 0x3f41);
    if ((gIw2 & 2) != 0) {
        Story_Apply22(0x04000052, gOv23 | 0x0c);
    } else {
        Story_Apply23(0x04000052, gOv23 | 0x10);
    }
}

void Scene_RunLateSequence(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_Run121();

    s32 record;
    s32 sx;
    s32 sy;
    u32 mode;

    record = Story_Check49(gCell[125][0]);
    sx = *(s16 *)(record + 10);
    sy = *(s16 *)(record + 18);
    if (Story_Check23(*(volatile s32 *)0x03001e40, 3) == 0) {
        mode = (u32)(Story_Check50() << 2) >> 16;
        switch (mode) {
        case 0:
            Story_Run93((sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 1:
            Story_Run508((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        case 2:
            Story_Run93((sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 3:
            Story_Run94((sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        }
    }
}

void Actor_UpdateObjectByCounterBits(u8 *obj)
{
    extern s32 gIw2;
    void Story_Run121(u8 *, s32);

    if ((gIw2 & 2) != 0) {
        Story_Run121(obj, 7);
    } else {
        Story_Apply24(obj, 0);
    }
    if ((gIw2 & 15) == 0) {
        Story_Do50(obj);
    }
}

void OvObj_UpdateOnFrameParity(u8 *obj)
{
    extern volatile u32 gIw2;

    if ((gIw2 & 1) != 0) {
        Story_Apply25(obj, Story_Apply26((s32)(gIw2 >> 1), 6));
    }
    if ((gIw2 & 15) == 0) {
        Story_Do51(obj);
    }
}

void State_RunSlotStepOnOddFrames(s32 arg0)
{
    extern volatile s32 gIw2;

    if ((gIw2 & 1) != 0) {
        s32 slot = Story_Apply27((u32)gIw2 >> 1, 6);

        Story_Apply28(arg0, slot);
    }
}

void StoryEffect_AnimateVerticalPositive(struct StoryVerticalEffectActor *effect)
{
    struct StoryVerticalEffectActor *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Story_Run509(effect);
        return;
    }

    vertical_amplitude = Story_Check51(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z + (0x10000 - vertical_amplitude) * 5 + 0x80000;
}

void StoryEffect_AnimateVerticalNegative(struct StoryVerticalEffectActor *effect)
{
    struct StoryVerticalEffectActor *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Story_Run510(effect);
        return;
    }

    vertical_amplitude = Story_Check52(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = -vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z - (0x10000 - vertical_amplitude) * 5 + 0x100000;
}
