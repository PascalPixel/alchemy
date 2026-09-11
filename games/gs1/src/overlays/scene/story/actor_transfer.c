#include "types.h"
#include "scene.h"

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

u8 *Story_unk95_3();

struct Object *Story_unk96_3(u8 *);
struct Object *Story_unk97_3(s32);

struct Object *Story_unk98_3(u8 *);
struct Object *Story_unk99_3(s32);

u8 *Story_unk100_3(s32);

u8 *Story_unk101_3();

u8 *Story_unk102_3();

u8 *Story_unk103_3();

u8 *Story_unk104_3();

u8 *Story_unk105_3();

u8 *Story_unk106_3();
u8 *Story_unk107_3();

u8 *Story_unk108_3();

u8 *Story_unk109_3();

u8 *Story_unk110_3();

struct StorySelectionActor *Story_unk111_3(s32 actor);

void *Story_unk112_3(s32 actor);

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
        if ((u32)Story_unk24() < 0x8000) {
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

    if (*progress >= Story_unk25(*level * 9, 10)) {
        Story_unk113_3(0x809, 42);
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

    if (*progress >= Story_unk26(*level * 9, 10)) {
        Story_unk114_3(0x80a, 24);
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
        Story_unk115_3(actor);
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
        spawned_actor = Story_unk95_3(0x11d, x, y, z);
    }
    Story_unk116_3(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Story_unk117_3(spawned_actor, 0);
        Story_unk118_3(spawned_actor, 1);
        *(u16 *)(spawned_actor + 0x64) = zero_value;
        *(s32 *)(spawned_actor + 0x6c) = (s32)gOv;
    }
}

s32 StoryActor_Initialize(u8 *actor)
{
    extern u32 gIw2;

    u8 *actor_flags;
    s32 fixed_scale;

    if (Story_unk27(0x30) != 0) {
        return 0;
    }
    if (Story_unk28(0x16E) != 0) {
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

    Story_unk119_3();
    if (Story_unk29((s32)&Value_00000847) != 0) {
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
    if (Story_unk30(0x8A0) != 0) {
        Story_unk30_2((s32)&Value_000002f1);
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
 * everything else takes the default arm, which calls Story_unk120_3 before
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
        if (Story_unk31(0x94f) == 0 && Story_unk32(0x941) != 0) {
            return gOv8;
        }
        break;
    case 64:
        if (Story_unk33(0x85a) == 0) {
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

    Story_unk120_3(0x235);
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

    subject_actor = Story_unk96_3(actor_object - 0x64);
    scene_table = gCell2;
    other_actor = Story_unk97_3(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->x < subject_actor->x) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Story_unk31_2(0x7B);
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

    subject_actor = Story_unk98_3(actor_object - 0x64);
    scene_table = gCell2;
    other_actor = Story_unk99_3(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->z < subject_actor->z) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Story_unk32_2(0x7B);
}

void State_SetValues130_6_47(void)
{
    extern volatile s32 gIw2;

    Story_unk138_2(0x82, 6, 0x2F);
}

void State_ApplyValues150And46And11(void)
{
    extern volatile s32 gIw2;

    Story_unk139_2(0x96, 0x2E, 0x0B);
}

void State_ApplyValues116And56And21(void)
{
    extern volatile s32 gIw2;

    Story_unk140_2(0x74, 0x38, 0x15);
}

void State_ApplyValues151And25And54(void)
{
    extern volatile s32 gIw2;

    Story_unk141_2(0x97, 0x19, 0x36);
}

void Scene_RunStep7D3B1E(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    Story_unk142_2(0x7D, 0x3B, 0x1E);
}

u8 *SceneData_GetTableE3F4(void)
{
    return gOv5;
}

void State_ApplyFlag85aBranch(void)
{
    extern volatile s32 gIw2;

    if (Story_unk34((s32)&Value_0000085a) == 0) {
        Story_unk33_2(101);
    } else {
        Story_unk34_2(123);
        Story_unk35_2(3);
    }
}

void Scene_RunStep74(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    Story_unk122_3();
    Story_unk36_2(74);
}

void RunEventScript01(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Story_unk123_3();
    Story_unk124_3();
    Story_unk125_3();
    Story_unk126_3();
    Story_Do(0x94f);
    Story_Place(11, 0x16e00000, 0x49c0000);
    Story_unk127_3(11, 24, 8);
    Story_unk128_3(11);
    Story_unk129_3(60);
    Story_unk2_3(12, 0x16e00000, 0x49c0000);
    Story_unk130_3(12, 12, 24);
    Story_unk131_3(30);
    Story_unk3_3(11, 0x5000, 0);
    Story_unk4_3(12, 0xd000, 0);
    Story_unk132_3(60);
    Story_unk133_3(11, 3);
    Story_unk134_3(12, 3);
    Story_unk135_3(120);
    Story_unk5_3(8, 0x16f80000, 0x4b80000);
    Story_unk136_3(60);
    Story_unk137_3(12, 2);
    record = Story_Check(8);
    if (record != 0) {
        Story_unk138_3(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk139_3(12);
    Story_unk140_3(12, 0, 0);
    ((void (*)())Story_unk35)(60);
    Story_unk141_3(11, 2);
    record = Story_unk2(8);
    if (record != 0) {
        Story_unk142_3(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk143_3(11);
    Story_unk144_3(11, 0, 0);
    ((void (*)())Story_unk36)(60);
    Story_unk145_3(0, 2);
    record = Story_unk3(8);
    if (record != 0) {
        Story_unk146_3(0, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk147_3(0);
    Story_unk148_3(0, 0, 0);
    ((void (*)())Story_unk37)(60);
    Story_unk6_3(8, 0x8000, 0x4000);
    Story_unk149_3(8, 56, 8);
    Story_unk150_3(8);
    Story_unk151_3(8, 40, 40);
    Story_unk152_3(8);
    Story_unk153_3(8, 8, 88);
    Story_unk154_3(8);
    Story_unk155_3();
    Story_unk156_3(108);
    Story_unk157_3();
}

void Scene_RunActorTransferSequence(void)
{
    u8 *actor;
    u8 *record;
    s32 scale;
    s32 action;

    actor = Pointer1(Story_unk101_3, 15);
    Story_unk158_3();
    Story_Run(0x14000, 1);
    Story_unk159_3(4);
    Story_unk160_3();
    Story_unk161_3();
    Story_unk162_3();
    Story_unk2_4(-1, -1, -1, 0);
    Story_unk163_3(1);
    Story_unk7_3(0, 0x19999, 0xcccc);
    Story_unk8_3(1, 0x19999, 0xcccc);
    Story_unk9_3(0, 0x16fc, 0x628);
    Story_unk10_3(0, 0x6000, 0);
    Story_unk11_3(8, 0x16d80000, 0x6280000);
    Story_unk164_3(1);
    Story_unk165_3(8, 15);
    record = Story_unk102_3(8);
    Story_unk166_3(record, 0);
    Story_unk12_3(10, 0x19999, 0x6666);
    Story_unk13_3(11, 0x19999, 0x6666);
    Story_unk14_3(12, 0x19999, 0x6666);
    Story_unk15_3(13, 0x19999, 0x6666);
    Story_unk167_3(141);
    Story_unk3_4(10, 0x200c9e4);
    Story_unk168_3(20);
    Story_unk4_4(11, 0x200cb50);
    Story_unk169_3(20);
    Story_unk5_4(12, 0x200cc74);
    Story_unk170_3(20);
    Story_unk6_4(13, (s32)gOv18);
    Story_unk2_2(0x121);
    record = Pointer1(Story_unk103_3, 0);
    if (record != 0) {
        Story_unk171_3(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Story_unk16_3(1, 0x1704, 0x640);
    Story_unk17_3(0, 0x2000, 0);
    Story_unk18_3(1, 0xa000, 20);
    Story_unk19_3(0, 0x101, 0);
    Story_unk20_3(1, 0x101, 60);
    Story_unk21_3(8, 0x16d80000, 0x6380000);
    Story_unk172_3(1);
    Story_unk3_2(0x1215);
    Story_unk173_3(8, 0, 10);
    Story_unk22_3(0, 0x6000, 0);
    Story_unk23_3(1, 0x6000, 40);
    Story_unk24_3(0, 0x2000, 0);
    Story_unk25_3(1, 0xa000, 60);
    Story_unk26_3(0, 0x6000, 0);
    Story_unk27_3(1, 0x6000, 10);
    Story_unk174_3(141);
    Story_unk7_4(10, 0x200ca78);
    Story_unk175_3(20);
    Story_unk8_4(11, 0x200cbe4);
    Story_unk176_3(20);
    Story_unk9_4(12, 0x200cd08);
    Story_unk177_3(10);
    Story_unk28_3(0, 0x4000, 0);
    Story_unk29_3(1, 0x4000, 10);
    Story_unk10_4(13, 0x200ce2c);
    Story_unk4_2(0x121);
    Story_unk178_3(20);
    Story_unk179_3();
    Story_unk180_3(10, 1);
    Story_unk181_3(11, 1);
    Story_unk182_3(12, 1);
    Story_unk183_3(13, 1);
    Story_unk11_4(0x16080000, -1, 0x6f80000, 1);
    Story_unk184_3();
    Story_unk185_3(20);
    Story_unk186_3();
    Story_unk30_3(9, 0x16080000, 0x6d80000);
    Story_unk187_3(1);
    Story_unk31_3(9, 0x13333, 0x9999);
    Story_unk32_3(9, 0x1608, 0x6c8);
    Story_unk33_3(9, 0x15f8, 0x6c8);
    Story_unk34_3(9, 0x15f8, 0x6f8);
    Story_unk188_3(20);
    Story_unk189_3(9, 2);
    Story_unk190_3(20);
    Story_unk12_4(9, 0x102);
    Story_unk191_2(60);
    Story_unk192_2(9, 0, 20);
    Story_unk193_2(9, 3);
    Story_unk194_2(9, 0, 20);
    Story_unk35_3(8, 0x16180000, 0x6f80000);
    Story_unk195_2(1);
    Story_unk196_2(8, 0);
    record = Story_unk104_3(8);
    Story_unk197_2(record, 1);
    Story_unk36_3(8, 0xcccc, 0x6666);
    Story_unk37_3(8, 0x1608, 0x6f8);
    Story_unk198_2(20);
    Story_unk199_2(8, 2);
    Story_unk38_3(0x2008, 0, 10);
    Story_unk39_3(8, 0x3000, 60);
    Story_unk40_3(8, 0x8000, 10);
    Story_unk13_4(8, 0x102);
    Story_unk200_2(60);
    Story_unk41_3(9, 0x3000, 0);
    Story_unk42_3(8, 0x3000, 40);
    Story_unk14_4(8, 0x102);
    Story_unk201_2(60);
    Story_unk202_2(8, 2);
    Story_unk43_3(0x2008, 0, 40);
    Story_unk203_2(9, 1);
    Story_unk204_2(9, 0, 10);
    Story_unk205_2(9, 0, 10);
    Story_unk44_3(8, 0x105, 60);
    Story_unk45_3(0x2008, 0, 10);
    Story_unk206_2(8, 1);
    Story_unk207_2(8, 3);
    Story_unk46_3(0x2008, 0, 10);
    Story_unk47_3(9, 0x101, 60);
    Story_unk208_2(9, 0, 20);
    Story_unk209_2(8, 3);
    Story_unk210_2();
    Story_unk211_2(107);
    Story_unk15_4(0x40000, 0x40000);
    Story_unk212_2();
    Story_unk5_2(0x121);
    Story_unk48_3(8, 0x100, 0);
    Story_unk49_3(9, 0x100, 0);
    Story_unk50_3(8, 0x8000, 0);
    Story_unk213_2(9, 0, 40);
    Story_unk51_3(8, 0xb000, 0);
    Story_unk52_2(9, 0xb000, 0);
    Story_unk16_4(0x10000, 0x2000);
    Story_unk17_4(0x15e80000, -1, 0x6c80000, 1);
    Story_unk214_2();
    Story_unk53_2(14, 0x15a80000, 0x6a80000);
    Story_unk215_2(1);
    Story_unk54_2(14, 0x4ccc, 0x2666);
    Story_unk18_4(14, 0x200cebc);
    Story_unk216_2(160);
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
    Story_unk217_2(actor, 0);
    Story_unk218_2(actor, 2);
    Story_unk219_2(1);
    record = Story_unk105_3(15);
    Story_unk220_2(record, 0);
    Story_unk19_4(0x20097a5, 0xc80);
    do {
        Story_unk221_2(1);
    } while (*(s16 *)(actor + 100) == 0);
    record = Story_unk106_3(15);
    Story_unk222_2(record, 0);
    record = Story_unk107_3(14);
    Story_unk223_2(record, 0);
    Story_unk224_2(10);
    scale = 192;
    record = Story_unk108_3(9);
    *(s32 *)(record + 40) = (scale << 11);
    record = Pointer1(Story_unk109_3, 8);
    *(s32 *)(record + 40) = (scale << 11);
    Story_unk225_2(145);
    Story_unk20_4(0x40000, 0x40000);
    Story_unk226_2();
    Story_unk227_2();
    Story_unk228_2(60);
    Story_unk21_4(0x20000, 0x4000);
    Story_unk22_4(0x16080000, -1, 0x6f80000, 1);
    Story_unk229_2();
    Story_unk230_2();
    Story_unk23_4(9, 0x102);
    Story_unk24_4(8, 0x102);
    Story_unk231_2(60);
    Story_unk6_2(0x20097a5);
    Story_unk232_2(1);
    Story_unk233_2(14, 0, 0);
    Story_unk234_2(15, 0, 0);
    Story_unk55_2(8, 0x8000, 10);
    Story_unk235_2(8, 4, 40);
    Story_unk56_2(0x2008, 0, 10);
    Story_unk236_2(9, 0, 10);
    Story_unk237_2(9, 0, 20);
    Story_unk57_2(8, 0xc000, 40);
    Story_unk58_2(0x2008, 0, 20);
    Story_unk238_2(9, 4, 20);
    Story_unk239_2(9, 0, 10);
    Story_unk240_2(8, 1);
    Story_unk59_2(8, 0x8000, 10);
    Story_unk60_2(0x2008, 0, 10);
    Story_unk25_4(9, 0x102);
    Story_unk241_2(80);
    Story_unk242_2(8, 3);
    Story_unk243_2(20);
    Story_unk244_2(9, 1);
    Story_unk245_2(9, 3);
    Story_unk61_2(8, 0x1618, 0x6f8);
    Story_unk246_2(8, 0, 0);
    Story_unk62_2(9, 0x15f8, 0x6c8);
    Story_unk63_2(9, 0x1608, 0x6c8);
    Story_unk64_2(9, 0x1608, 0x6d8);
    Story_unk247_2(9, 0, 0);
    Story_unk248_2(141);
    Story_unk26_4(10, (s32)gOv19);
    Story_unk27_4(11, 0x200cc30);
    Story_unk249_2(40);
    Story_unk28_4(12, 0x200cd54);
    Story_unk250_2(40);
    Story_unk29_4(13, (s32)gOv20);
    Story_unk251_2();
    Story_unk65_2(0, 0x170c0000, 0x6280000);
    Story_unk66_2(1, 0x17140000, 0x6400000);
    Story_unk30_4(0x40000, 0x8000);
    Story_unk31_4(0x16d80000, -1, 0x6480000, 1);
    Story_unk252_2();
    action = (s32)gOv21;
    Story_unk253_2(10, action);
    Story_unk254_2(20);
    Story_unk32_4(0x6666, 0xccc);
    Story_unk33_4(0x16d80000, -1, 0x6080000, 1);
    Story_unk255_2(11, action);
    Story_unk256_2(20);
    Story_unk257_2(12, action);
    Story_unk258_2(20);
    Story_unk67_2(0, 0x8000, 0);
    Story_unk68_2(1, 0x8000, 0);
    Story_unk259_2(13, action);
    Story_unk260_2(40);
    Story_unk69_2(0, 0xc000, 0);
    Story_unk70_2(1, 0xc000, 0);
    Story_unk261_2(13);
    Story_unk7_2(0x121);
    Story_unk34_4(0x40000, 0x8000);
    Story_unk35_4(0x16f80000, -1, 0x6480000, 1);
    Story_unk262_2();
    Story_unk71_2(0, 0x2000, 0);
    Story_unk72_2(1, 0xa000, 80);
    Story_unk263_2(0, 3);
    Story_unk264_2(1, 3);
    Story_unk265_2(20);
    Story_unk266_2(1, 2);
    record = Pointer1(Story_unk110_3, 0);
    if (record != 0) {
        Story_unk267_2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk268_2(1);
    Story_unk269_2(1, 0, 0);
    Story_unk36_4(0xcccc, 0x1999);
    Story_unk37_4(0x16d80000, -1, 0x6480000, 1);
    Story_unk73_2(0, 0xcccc, 0x6666);
    Story_unk74_2(0, 0x16d8, 0x628);
    Story_unk270_2();
    Story_unk271_2();
    Story_unk8_2(0x85a);
    Story_unk272_2(3);
    Story_unk273_2();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 record;

    Story_unk38_4(0x160c0000, -1, 0x6f80000, 1);
    Story_unk274_2(4);
    Story_unk39_4(0x16040000, -1, 0x6fc0000, 1);
    Story_unk275_2(4);
    Story_unk40_4(0x160c0000, -1, 0x6f40000, 1);
    Story_unk276_2(4);
    Story_unk41_4(0x160c0000, -1, 0x6fc0000, 1);
    Story_unk277_2(4);
    Story_unk42_4(0x16040000, -1, 0x6f40000, 1);
    Story_unk278_2(4);
    Story_unk43_4(0x16080000, -1, 0x6f80000, 1);
    Story_unk279_2(4);
    Story_unk44_4(0x160a0000, -1, 0x6f80000, 1);
    Story_unk280_2(4);
    Story_unk45_4(0x16060000, -1, 0x6fa0000, 1);
    Story_unk281_2(4);
    Story_unk46_4(0x160a0000, -1, 0x6f60000, 1);
    Story_unk282_2(4);
    Story_unk47_4(0x160a0000, -1, 0x6fa0000, 1);
    Story_unk283_2(4);
    Story_unk48_4(0x16060000, -1, 0x6f60000, 1);
    Story_unk284_2(4);
    Story_unk49_4(0x16080000, -1, 0x6f80000, 1);
    Story_unk285_2(4);
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 record;

    Story_unk50_4(0x15ec0000, -1, 0x6c80000, 1);
    Story_unk286_2(4);
    Story_unk51_4(0x15e40000, -1, 0x6cc0000, 1);
    Story_unk287_2(4);
    Story_unk52_3(0x15ec0000, -1, 0x6c40000, 1);
    Story_unk288_2(4);
    Story_unk53_3(0x15ec0000, -1, 0x6cc0000, 1);
    Story_unk289_2(4);
    Story_unk54_3(0x15e40000, -1, 0x6c40000, 1);
    Story_unk290_2(4);
    Story_unk55_3(0x15e80000, -1, 0x6c80000, 1);
    Story_unk291_2(4);
    Story_unk56_3(0x15ea0000, -1, 0x6c80000, 1);
    Story_unk292_2(4);
    Story_unk57_3(0x15e60000, -1, 0x6ca0000, 1);
    Story_unk293_2(4);
    Story_unk58_3(0x15ea0000, -1, 0x6c60000, 1);
    Story_unk294_2(4);
    Story_unk59_3(0x15ea0000, -1, 0x6ca0000, 1);
    Story_unk295_2(4);
    Story_unk60_3(0x15e60000, -1, 0x6c60000, 1);
    Story_unk296_2(4);
    Story_unk61_3(0x15e80000, -1, 0x6c80000, 1);
    Story_unk297_2(4);
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk4(15);
    record = Story_unk5(14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (Story_unk6(0x200) == 0) {
            Story_unk298_2(145);
            Story_unk299_2(rec7, 3);
            Story_unk9_2(0x200);
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
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk7(8);
    Story_unk300_2();
    Story_unk62_3(-1, -1, -1, 0);
    Story_unk301_2(1);
    Story_unk302_2(0, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Story_unk303_2(8, 1);
    Story_unk304_2();
    Story_unk75_2(8, 0x6666, 0x3333);
    Story_unk76_2(8, 0x14a8, 0x918);
    Story_unk305_2();
    Story_unk306_2();
    Story_unk10_2(0x927);
    Story_unk307(102);
    Story_unk308();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk8(8);
    Story_unk309();
    Story_unk63_3(-1, -1, -1, 0);
    Story_unk310(1);
    Story_unk311(0, 0, 0);
    Story_unk77_2(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Story_unk312(1);
    Story_unk313(8, 1);
    Story_unk314();
    Story_unk78_2(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Story_unk315(8, 0x200d158);
    do {
        Story_unk316(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Story_unk317();
    Story_unk318();
    Story_unk11_2(0x927);
    Story_unk319(103);
    Story_unk320();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk9(8);
    Story_unk321();
    Story_unk64_3(-1, -1, -1, 0);
    Story_unk322(1);
    Story_unk323(0, 0, 0);
    Story_unk79_2(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Story_unk324(1);
    Story_unk325(8, 1);
    Story_unk326();
    Story_unk80_2(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Story_unk327(8, 0x200d158);
    do {
        Story_unk328(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Story_unk329();
    Story_unk330();
    Story_unk12_2(0x927);
    Story_unk331(104);
    Story_unk332();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk10(8);
    Story_unk333();
    Story_unk65_3(-1, -1, -1, 0);
    Story_unk334(1);
    Story_unk335(0, 0, 0);
    Story_unk81_2(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Story_unk336(1);
    Story_unk337(8, 1);
    Story_unk338();
    Story_unk82_2(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Story_unk339(8, 0x200d158);
    do {
        Story_unk340(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Story_unk341();
    Story_unk342();
    Story_unk13_2(0x927);
    Story_unk343(105);
    Story_unk344();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk11(8);
    Story_unk345();
    Story_unk66_3(-1, -1, -1, 0);
    Story_unk346(1);
    Story_unk347(0, 0, 0);
    Story_unk83_2(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Story_unk348(1);
    Story_unk349(8, 1);
    Story_unk350();
    Story_unk84_2(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    if (Story_unk67_3() == 11) {
        Story_unk351(8, 0x200d1b8);
    } else {
        Story_unk352(8, 0x200d158);
    }
    do {
        Story_unk353(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Story_unk354();
    Story_unk355();
    Story_unk14_2(0x927);
    Story_unk356(106);
    Story_unk357();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk12(8);
    Story_unk358();
    Story_unk68_3(-1, -1, -1, 0);
    Story_unk359(1);
    Story_unk360(0, 0, 0);
    Story_unk85_2(8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Story_unk361(1);
    Story_unk362(8, 1);
    Story_unk363();
    Story_unk86_2(8, 0x6666, 0x3333);
    Story_unk87_2(8, 0x13c8, 0x918);
    Story_unk364();
    Story_unk365();
    Story_unk15_2(0x93e);
    Story_unk16_2(0x927);
    Story_unk366(107);
    Story_unk367();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 record;

    Story_unk368();
    Story_unk69_3(0x10000, 6);
    Story_unk369();
    Story_unk370();
    Story_unk371(8, 2);
    Story_unk17_2(0xc66);
    Story_unk372(8, 0);
    Story_unk373(30);
    Story_unk374(111);
    Story_unk375(0, 2);
    Story_unk18_2(0x16f);
    Story_unk19_2(0x171);
    Story_unk376();
    Story_unk377(8, 4, 30);
    Story_unk20_2(0xc67);
    Story_unk378(8, 0);
    Story_unk21_2(0x16f);
    Story_unk22_2(0x171);
    Story_unk379();
    Story_unk380(30);
    Story_unk381(12, 6);
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk13(10);
    Story_unk382();
    Story_unk70_3(-1, -1, -1, 0);
    Story_unk383(1);
    Story_unk384(0, 0, 0);
    Story_unk385(1);
    *(volatile s32 *)(rec7 + 24) = 0x18000;
    *(volatile s32 *)(rec7 + 28) = 0x18000;
    {
        s32 shown = 0x4000;

        *(volatile u16 *)(rec7 + 6) = shown;
    }
    Story_unk386();
    Story_unk387();
    Story_unk388(20);
    Story_unk88_2(10, 0x15680000, 0x8380000);
    Story_unk389(1);
    Story_unk390(141);
    Story_unk89_2(10, 0x19999, 0x6666);
    Story_unk391(10, 2);
    Story_unk90_2(10, 0x156d, 0x858);
    Story_unk71_3(0x6666, 0xccc);
    Story_unk72_3(0x15b80000, -1, 0x8580000, 1);
    Story_unk91_2(10, 0x159e, 0x858);
    Story_unk92_2(10, 0x15a8, 0x86e);
    Story_unk93_2(10, 0x15e8, 0x878);
    Story_unk392(10, 1);
    Story_unk23_2(0x121);
    Story_unk393(20);
    Story_unk94_2(0, 0x15d80000, 0x8780000);
    Story_unk394(1);
    Story_unk395(0, 6, 0);
    Story_unk95_2(0, 0x20000, 0x10000);
    Story_unk96_2(0, 0x15c8, 0x878);
    Story_unk396(0, 0, 40);
    Story_unk397(141);
    Story_unk398(10, 2);
    Story_unk97_2(10, 0x15f8, 0x878);
    Story_unk98_2(0, 0xe000, 0);
    Story_unk99_2(10, 0x15f8, 0x838);
    Story_unk100_2(0, 0xc000, 0);
    Story_unk101_2(10, 0x15bd, 0x838);
    Story_unk102_2(10, 0x15b8, 0x853);
    Story_unk103_2(0, 0xa000, 0);
    Story_unk104_2(10, 0x1572, 0x858);
    Story_unk105_2(10, 0x1568, 0x838);
    Story_unk399(10, 0, 0);
    Story_unk24_2(0x121);
    Story_unk400(40);
    Story_unk73_3(0x15d80000, -1, 0x8580000, 1);
    Story_unk106_2(0, 0xcccc, 0x6666);
    Story_unk107_2(0, 0x15d8, 0x858);
    Story_unk401();
    Story_unk402();
    Story_unk403(20);
    Story_unk404();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 record;

    Story_unk405();
    Story_unk406(55, 0, 0);
    Story_unk25_2(0x2642);
    Story_unk14(*(volatile s32 *)gOv15, 0);
    Story_unk108_2(55, 0x3000, 0);
    Story_unk407();
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 record;

    Story_unk408();
    Story_unk409();
    Story_unk26_2(0x2643);
    Story_unk15(*(volatile s32 *)gOv15, 0);
    Story_unk410();
    Story_unk109_2(0, 0x10000, 0x8000);
    Story_unk110_2(0, 0x1778, 0xd48);
    Story_unk411();
}

/* Runs dialogue 0x264c and publishes the story result when flag 0x234 is
 * set. */
void StoryScene_ShowRewardDialogue(void)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    Story_unk412();
    Story_unk413();
    Story_Apply6(0x264c, 1);
    if (Story_unk38(0x234) != 0) {
        gWork->story_result = 1;
    }
    Story_unk414();
    Story_unk415();
}

void Scene_RunActorPresentationSequence(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 record;
    s32 base2_2000240;
    s32 base3_2000240;

    base2_2000240 = (s32)gCell2;
    base3_2000240 = (s32)gCell2;
    Story_unk416(*(u8 *)((base2_2000240 + 0x205)), *(u8 *)((base3_2000240 + 0x206)));
    Story_unk417();
    Story_unk74_3(0x10000, 0x12c);
    Story_unk75_3(-1, -1, -1, 0);
    Story_unk418(5, 19);
    Story_unk419(8, 5);
    Story_unk420(0, 0, 0);
    Story_unk421(1);
    Story_unk76_3(0x18000, 16);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_unk77_3(0x10003, 1);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    Story_unk422();
    Story_unk423();
    Story_unk424();
    Story_unk425(40);
    Story_unk426(5, 1);
    Story_unk427(20);
    Story_unk27_2(0x2913);
    Story_unk428(5, 0, 20);
    Story_unk429(8, 2);
    Story_unk430(20);
    Story_unk431(8, 0, 20);
    Story_unk111_2(5, 0x107, 20);
    Story_unk432(5, 0, 20);
    Story_unk112_2(8, 0x105, 80);
    Story_unk433(8, 0, 10);
    Story_unk434(5, 2);
    Story_unk435(5, 0, 20);
    Story_unk113_2(8, 0x105, 100);
    Story_unk114_2(5, 0x105, 40);
    Story_unk436(5, 0, 20);
    Story_unk437(8, 0, 10);
    Story_unk115_2(5, 0x102, 20);
    Story_unk438(5, 0, 10);
    Story_unk78_3(8, 0x102);
    Story_unk439(80);
    Story_unk440(8, 0, 20);
    Story_unk116_2(5, 0x105, 80);
    Story_unk441(8, 0, 120);
    Story_unk442(5, 1);
    Story_unk443(5, 0, 40);
    Story_unk444(8, 0, 20);
    Story_unk117_2(5, 0x105, 40);
    Story_unk445(5, 0, 120);
    Story_unk118_2(9, 0x6666, 0x3333);
    Story_unk119_2(9, 0x1ddc0000, 0xd840000);
    Story_unk120_2(9, 0x1d94, 0xd8c);
    Story_unk121_2(9, 0x1d88, 0xda0);
    Story_unk446(20);
    Story_unk122_2(0x6009, 0, 20);
    Story_unk123_2(8, 0x101, 0);
    Story_unk124_2(5, 0x101, 60);
    Story_unk447(9, 3);
    Story_unk79_3(0x6009, 0);
    Story_unk448();
    Story_unk449(20);
    Story_unk80_3(9, 0x200cf7c);
    Story_unk450(80);
    Story_unk451(8, 1);
    Story_unk452(8, 4, 40);
    Story_unk453(5, 1);
    Story_unk454(5, 4, 60);
    Story_unk125_2(8, 0x3000, 0);
    Story_unk126_2(5, 0xb000, 40);
    Story_unk127_2(8, 0x9999, 0x4ccc);
    Story_unk128_2(5, 0x9999, 0x4ccc);
    Story_unk81_3(8, 0x200d01c);
    Story_unk455(20);
    Story_unk82_3(0xb333, 0x1666);
    Story_unk83_3(0x1e380000, -1, 0xdc80000, 1);
    Story_unk456(5, 0x200d0a8);
    do {
        Story_unk457(10, 6);
        Story_unk458(6, 8);
        Story_unk459(1);
    } while (*(s16 *)(Story_unk39(5) + 100) == 0);
    Story_unk460(20);
    Story_unk129_2(9, 0x8000, 20);
    Story_unk84_3(8, 0x102);
    Story_unk85_3(5, 0x102);
    Story_unk461(40);
    Story_unk462();
    Story_SetRect(5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
    Story_unk463(20);
    Story_unk464();
    Story_unk86_3(0x10000, 0x2000);
    Story_unk87_3(0x1e580000, -1, 0xdc80000, 1);
    Story_unk130_2(9, 0x3000, 0);
    Story_unk131_2(8, 0x19999, 0xcccc);
    Story_unk132_2(5, 0x19999, 0xcccc);
    Story_unk133_2(8, 0x1e7c, 0xdb8);
    Story_unk134_2(5, 0x1e6c, 0xdd8);
    Story_unk465(8, 1);
    Story_unk466();
    Story_unk467(80);
    Story_unk468(8, 1);
    Story_unk469(8, 0, 20);
    Story_unk470(5, 2);
    Story_unk135_2(0x1005, 0, 40);
    Story_unk136_2(8, 0x8000, 20);
    Story_unk471(8, 2);
    Story_unk472(8, 0, 60);
    Story_unk473();
    Story_unk474(17);
    Story_unk475(0, 0);
    Story_unk476(120);
    Story_unk477(120);
    Story_unk478((s32)gVal, 10);
}

/* Stages the scene transition and arms the timed callback that drives it. */
void StoryScene_StartTransition(void)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    Story_unk479();
    Story_unk37_2(141);
    Story_Apply7(0, 0);
    Story_Apply8(0, 0);
    Story_unk38_2(1);
    Story_unk39_2(2);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 1;
    Story_unk480();
    Story_unk481();
    Story_Apply9(0, 15);
    Story_Apply10(Story_unk100_3(0), 0);
    Story_unk40_2(1);
    Story_Apply11(0x40000, 0x8000);
    {
        s32 transition_delay = 3200;
        void *transition_callback = (void *)0x0200b1fd;
        Story_Apply12(transition_callback, transition_delay);
    }
    Story_Apply13(0, 0);
    Story_Apply14(0x10004, 1);
    Story_Apply15(0x10000, 2);
    Story_unk41_2(40);
    Story_unk42_2(240);
    Story_Apply16(0, 0);
    Story_unk43_2(80);
    Story_unk44_2(90);
    Story_unk45_2(109);
    Story_unk46_2(282);
    Story_unk482();
}

/* Drives actor 8 through a series of position/threshold setup calls and
 * advances the shared scene phase before the scene runs. */
void Scene_RunActorEightApproach(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 actor;

    Battle_Reset_1();
    Motion_CamBounds_1(-1, -1, -1, 0);
    Story_unk483(1);
    Object_SetModeById_1(ACTOR, 2);
    Motion_SetHPosTerrain_1(ACTOR, 0x13080000, 0x3280000);
    actor = Scene_GetRecord_1(ACTOR);
    {
        /* Write 0xa000 to the halfword at +6 of the actor record. */
        s32 value = 0xa000;

        *(u16 *)(actor + 6) = value;
    }
    Story_unk484(1);
    RatioHistory_RecordAndScheduleCallback_1(0x13333, 1);
    Motion_SetHPosTerrain_2(0, 0, 0);
    Motion_SetPosReset_1(ACTOR, 1);
    Story_unk485(1);
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
    Story_unk486();
    Motion_SetSpeed_5();
    Story_unk487(110);
}

/* Totals slots 0 and 2 against slots 1 and 3 and returns the difference. */
s32 StoryScene_ComputeOpposingSlotDelta(void)
{
    extern s16 gCell2[];
    extern struct StoryDialogueWork *gWork;

    s32 positive_total = Story_unk40(0);
    s32 negative_total;

    positive_total += Story_unk41(2);
    negative_total = Story_unk42(1);
    negative_total += Story_unk43(3);
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
        if (Story_unk44(flag_base + offset) != 0) return gOv22[offset];
    }
    return 0;
}

void Scene_RunScene371(void)
{
    extern u8 gCell2[];
    extern u8 gOv23[];
    extern u8 gWork[];
    void Story_unk121_3();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Story_unk16(8);
    Story_unk488(60);
    Story_unk489();
    Story_unk88_3(0x9999, 1);
    *(s32 *)(rec7 + 24) = 0x13333;
    *(s32 *)(rec7 + 28) = 0x13333;
    Story_unk490(8, 1);
    Story_unk491(1);
    Story_unk492(0, 15);
    record = Story_unk45(0);
    Story_unk493(record, 0);
    record = Story_unk46(8);
    Story_unk494(record, 0);
    Story_unk137_2(8, 0x6666, 0x3333);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Story_unk17(8, 0x200d218);
    Story_unk18(0x200b4a1, 0xc80);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_unk89_3(0x10003, 1);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c8) = 32;
    Story_unk495();
    Story_unk496(120);
    Story_unk90_3(0x16666, 0x12c);
    Story_unk28_2(0x10e);
    *(s32 *)(*(u8 *volatile *)gWork + 0x1c8) = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Story_unk497();
    Story_unk498();
    Story_unk499(111);
}

void StoryScene_UpdateSelectedActorProgress(void)
{
    extern volatile s32 gIw2;

    extern struct StorySelection gCell2;
    extern struct StoryProgressWork *gWork;

    struct StorySelectionActor *actor;
    struct StoryProgressWork *scene;
    s32 progress;

    actor = Story_unk111_3(gCell2.actor_id);
    scene = gWork;
    actor->presentation = (u16)(gIw2 << 12);

    progress = Story_unk47(0x2f8);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (Story_unk48(0x106) == 0) {
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
    void Story_unk121_3();

    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = gCell2;
    p6 = *(volatile s32 *)(base + 500);
    rec7 = Story_unk19((s32)p6);
    rec2 = Story_unk20(0x2f0);
    if (rec2 == 0) {
        Story_unk500();
        Story_unk91_3((s32)p6, 0x101);
        Story_unk501((s32)p6, 9);
        record = Story_unk21(a0);
        if (record != 0) {
            Story_unk502((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Story_unk503((s32)p6);
        Story_unk504(244);
        Story_unk22(0x200b679, 0xc80);
        rec7[85] = rec2;
        {
            s32 shifted = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;

            Story_unk505((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), shifted, *(volatile s32 *)((s32)rec7 + 16));
        }
        Story_unk506((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Story_unk29_2(0x2f0);
        Story_unk92_3(0x2f8, 180);
        Story_unk507();
        *(u16 *)((*(u8 *volatile *)gWork + 0x17c)) = rec2;
    }
}

void StoryScene_SetReferenceActor(void)
{
    extern volatile s32 gIw2;

    Story_unk47_2(54);
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
    Story_unk48_2(0x2f0);
    Story_unk49_2(0x2f1);
    Story_Apply18(0x2f8, 0);
    Story_Apply19(98, 5);
    gCell2[0x22b] = 3;
    Story_Apply20(98, 7);
    selected_actor = Story_unk112_3(*(s32 *)(gCell2 + 500));
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
    void Story_unk121_3();

    s32 record;
    s32 sx;
    s32 sy;
    u32 mode;

    record = Story_unk49(gCell[125][0]);
    sx = *(s16 *)(record + 10);
    sy = *(s16 *)(record + 18);
    if (Story_unk23(*(volatile s32 *)0x03001e40, 3) == 0) {
        mode = (u32)(Story_unk50() << 2) >> 16;
        switch (mode) {
        case 0:
            Story_unk93_3((sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 1:
            Story_unk508((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        case 2:
            Story_unk93_3((sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 3:
            Story_unk94_3((sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        }
    }
}

void Actor_UpdateObjectByCounterBits(u8 *obj)
{
    extern s32 gIw2;
    void Story_unk121_3(u8 *, s32);

    if ((gIw2 & 2) != 0) {
        Story_unk121_3(obj, 7);
    } else {
        Story_Apply24(obj, 0);
    }
    if ((gIw2 & 15) == 0) {
        Story_unk50_2(obj);
    }
}

void OvObj_UpdateOnFrameParity(u8 *obj)
{
    extern volatile u32 gIw2;

    if ((gIw2 & 1) != 0) {
        Story_Apply25(obj, Story_Apply26((s32)(gIw2 >> 1), 6));
    }
    if ((gIw2 & 15) == 0) {
        Story_unk51_2(obj);
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
        Story_unk509(effect);
        return;
    }

    vertical_amplitude = Story_unk51(animation_frame << 10);
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
        Story_unk510(effect);
        return;
    }

    vertical_amplitude = Story_unk52(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = -vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z - (0x10000 - vertical_amplitude) * 5 + 0x100000;
}
