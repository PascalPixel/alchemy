#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/party_intro.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "create_configured_overlay_object.h"
#include "overlay_object.h"

typedef struct {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} S;

union MotionWork {
    struct {
        u32 unk_00[2];
        s32 x, y, z;
        u32 unk_14;
        s32 accum_x, accum_y;
        u32 unk_20[4];
        s32 rate_x, rate_y;
        u32 unk_38[3];
        s32 velocity_x, velocity_y, velocity_z;
        u16 *record;
        u8 unk_54[16];
        u16 angle_step;
    } fields;
    u8 bytes[102];
};

/*
 * Configurable spawn/copy owner at 0x0200013c.  Its code runs through the
 * unwind at 0x02000306; the following three pool words belong to this owner:
 * 0x0200cbc4, 0x02008105, and 0xffff0000.  The next owner begins at 0x02000314.
 */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 link;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct Descriptor {
    s32 pad00[3];
    s32 duration;
};

extern struct Descriptor *ConfigurableEffectDescriptors[];
extern s32 gOv[];
extern s32 gOv2[];
extern s32 gOv3[];
extern s32 gOv4[];
extern u8 gVal[];
extern u8 gWork[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern u8 gVal5[];
extern u8 gVal6[];
extern u8 gVal7[];
extern void Story_Run71(s32 arg0);                        /* 0x0200433c -> 080f9010 */
extern void Story_Run72(s32 arg0, s32 arg1);              /* 0x02004344 -> 0808a398 */
extern void Story_Run73(s32 arg0, s32 arg1);              /* 0x0200434c -> 08015040 */
extern s32 Story_Run74(s32 arg0);                         /* 0x02004352 -> 08077248 */
extern s32 Story_Run75(s32 arg0);                         /* 0x0200435c -> 08077248 */
extern void Story_Run76(s32 arg0, s32 arg1);              /* 0x0200436a -> 08015040 */
extern s32 Story_Run77(s32 *out_first, s32 *out_second);  /* 0x02004372 -> 080b0058 */
extern void Story_Run78(s32 arg0, s32 arg1);              /* 0x02004382 -> 080772b0 */
extern void Story_Run79(s32 arg0);                        /* 0x0200438a -> 08077030 */
extern void Story_Run80(s32 arg0);                        /* 0x02004390 -> 08077030 */
extern void Story_Run81(s32 arg0);                        /* 0x02004396 -> 08077030 */
extern void Story_Run82(s32 arg0);                        /* 0x0200439c -> 08077030 */

void *AcquireOverlayObject(s32, s32, s32, s32);

struct Effect *GetPrimaryEffectRecord(s32 index);
struct Effect *SpawnConfiguredEffectObject(s32 kind, s32 x, s32 y, s32 z);

u8 *Scene_GetRecord_1();

struct ObjectRuntime *Story_Run83(u32);
struct ObjectRuntime *Story_Run84(u32);

u8 *Story_Run85();

struct ObjectRuntime *Story_Run86(u32);

struct ObjectRuntime *Story_Run87(u32);

struct ObjectRuntime *Story_Run88(u32);

u8 *Story_Run89();
u8 *Story_Run90();
u8 *Story_Run91();

struct ObjectRuntime *Story_Run92(u32);

/* Overlay-local import veneers, retained per call site. */

/*
 * resource_380: the talk handler for actor 9, one of a bank of per-actor
 * published callbacks. The handler is published through a script record
 * rather than called, so nothing here reaches it by name.
 */

/*
 * resource_380: the talk handler for actor 5, published through a script
 * record rather than called.
 */

/*
 * Overlay resource_380: the per-actor talk handlers of the party
 * introduction scene.
 */

/*
 * Callback for resource_380 that forwards to Story_Run93 and nothing else.
 * A script record holds its Thumb pointer; nothing calls it directly. The
 * twelve-byte owner includes the zero alignment halfword after the return and
 * has no literal pool. Story_Run93 takes no arguments -- nothing is loaded
 * into r0 before the call.
 */

/*
 * resource_380 owner at 0x02004248, 24 bytes including the trailing
 * alignment halfword: a two-call convenience wrapper.  Both Func_ names are
 * the overlay's import-veneer entries for main-ROM code, not runtime
 * addresses.  The callees' roles beyond call shape remain open.
 */

/* One symbol per call site, named at that site's call word rather than at a
 * runtime address. A callee reached from several sites therefore appears
 * several times here under different names. */

/* Inline calls keep argument constants local; direct calls permit sharing. */

struct ObjectRuntime;

void OvObj_SetHandleFlagBits2And3(s32 obj, u32 v)
{
    S *h = *(S **)(obj + 0x50);
    h->f1 = v;
}

void *OvObj_PrepareObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void Effect_UpdateMotion(union MotionWork *work)
{
    u16 *record;
    work->fields.x += work->fields.velocity_x;
    work->fields.y += work->fields.velocity_y;
    work->fields.z += work->fields.velocity_z;
    work->fields.accum_x += work->fields.rate_x;
    work->fields.accum_y += work->fields.rate_y;
    record = work->fields.record;
    record[15] += work->fields.angle_step;
}

void Overlay380_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                   u32 flags, const struct Options *options)
{
    u32 table_offset;
    struct Effect *party;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mode_block;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;

    party = GetPrimaryEffectRecord(0);
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = SpawnConfiguredEffectObject(options->kind, x, y, z);
    } else {
        effect = SpawnConfiguredEffectObject(222, x, y, z);
    }
    if (effect == 0)
        return;

    block = effect->sprite;
    mode_block = block;

    SetConfiguredEffectVariant(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    ApplyConfiguredEffectDescriptor(effect, ConfigurableEffectDescriptors[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0)
        return;

    if ((flags & 0x10000) != 0)
        SetConfiguredEffectLink(effect, options->link);

    if ((flags & 0x20000) != 0) {
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct Descriptor *descriptor =
            ConfigurableEffectDescriptors[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = ComputeConfiguredEffectRateFromDelta(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = ComputeConfiguredEffectRateFromOffset(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate34 = ComputeConfiguredEffectSecondRate(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        SetConfiguredEffectCallbackMode(effect, 1);
        ApplyConfiguredEffectCallbackArgument(effect, (struct Descriptor *)options->callback_arg);
    }

    if ((flags & 0x400000) != 0)
        block->angle = options->angle;
    if ((flags & 0x800000) != 0)
        *tag = options->step;
    if ((flags & 0x1000000) != 0)
        effect->callback = options->callback;
}

s32 UpdateOverlayObjectAngle(struct OverlayObject *object)
{
    struct OverlayObject *linked_object = object->linked_object;
    if (linked_object != NULL) {
        s32 angle_delta;
        u16 angle;
        object->unknown_5a = object->unknown_5a & 0xFE;
        angle_delta = CalculateAngleFromCoordinateDelta(
            linked_object->coordinate_10 - object->coordinate_10,
            linked_object->coordinate_08 - object->coordinate_08);
        angle = object->angle;
        angle_delta -= angle;
        angle_delta <<= 16;
        angle_delta >>= 16;
        if (angle_delta != 0) {
            if (angle_delta > 0x1000) {
                angle_delta = 0x1000;
            }
            if (angle_delta < -0x1000) {
                angle_delta = -0x1000;
            }
            object->angle = angle + angle_delta;
        }
    }
    return 1;
}

s32 *SceneData_GetTableCd88(void)
{
    return gOv;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 *SceneData_GetTableCDB8(void)
{
    return gOv2;
}

s32 *SceneData_GetTableCdc4(void)
{
    return gOv3;
}

s32 *SceneData_GetTableCFA4(void)
{
    return gOv4;
}

void Story_Run94(void)
{
    u32 i;
    s32 obj;
    s32 pos;
    s32 mes;

    Story_Run95();
    Story_Run96(141);
    for (i = 0; i != 6; i++) {
        Story_Run(0x404a4e, 1);
        Story_Run97(8);
        Story_Run98(8);
        Story_Run2(0x10000, 1);
        Story_Run99(8);
        Story_Run100(8);
        if (i == 1) {
            Story_Place(0x10000, 0x10000, 0x10000);
        }
    }
    Story_Place2(0x10000, 0x20000, 0x10000);
    Story_Run101(30);
    Story_Run102(0, 0, 0);
    Story_Run3(0x59999, 0xb333);
    Story_Run4(0x1d80000, -1, 0x620000, 1);
    Story_Run103();
    Story_Place3(0x10000, 0x20000, 0x10000);
    Story_Run104(20);
    Story_Run105(144);
    Story_Run106(0x200d088, 84, 4);
    Story_SetRect(0, 0, 1, 1, 29, 4);
    Story_SetRect2(87, 42, 29, 6, 1, 2);
    Story_Run107(40);
    Story_Run108(0, 0, 0);
    Story_Run5(0x1570000, -1, 0x1710000, 1);
    Story_Run109();
    Story_Place4(0x10000, 0x20000, 0x10000);
    Story_Run110(20);
    Story_Run111(144);
    pos = 21;
    Story_Run112(0x200d088, 76, 21);
    Story_SetRect3(0, 0, 1, 1, pos, pos);
    Story_SetRect4(87, 42, 21, 23, 1, 2);
    Story_Run113(40);
    Story_Run114(0, 0, 0);
    Story_Run6(0x33333, 0x6666);
    Story_Run7(0x1570000, -1, 0x1f10000, 1);
    Story_Run115();
    Story_Place5(0x10000, 0x20000, 0x10000);
    Story_Run116(20);
    Story_Run117(144);
    Story_Run118(0x200d088, 76, 29);
    Story_SetRect5(0, 0, 1, 1, pos, 29);
    Story_SetRect6(87, 42, 21, 31, 1, 2);
    Story_Run119(40);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Story_Run120();
    Story_Run121();
    Story_Run8(0x2c80000, -1, 0x980000, 0);
    Story_Run122();
    Story_Run123(1);
    Story_Place6(0x20000, 0x10000, 0x10000);
    Story_Run124();
    Story_Run125();
    Story_Run126(40);
    Story_Do(0x121);
    Story_Place7(-1, -1, 0xe666);
    Story_Run127();
    Story_Run128(20);
    Story_SetRect7(0, 40, 43, 46, 3, 3);
    Story_Run129(20);
    obj = Story_Check29(221, 0x2c80000, 0x100000, 0x900000);
    Story_Run130(40);
    Story_Run131(obj, 1);
    mes = (s32)gVal;
    Story_Check(mes, 1);
    Story_Place8(9, 0xe000, 0);
    Story_Place9(5, 0xe000, 20);
    Story_Run132();
    Story_Run133();
    Story_Run9(0x1ce0000, -1, 0x15e0000, 0);
    Story_Run134();
    Story_Run135(1);
    Story_Run136();
    Story_Run137();
    Story_Run138(40);
    Story_Run139(9, 4, 30);
    Story_Run140(mes - 2);
    Story_Run141(9, 20);
    Story_Run142();
    Story_Run143();
    Story_Run144(0x2c80000, -1, 0x980000, 0);
    Story_Run145();
    Story_Run146(1);
    Story_Run147();
    Story_Run148();
    Story_Do2(0x83d);
    Story_Run149();
}

void Scene_RunSevenSceneChain(void)
{
    Story_Run150();
    Story_Run151();
    Story_Run152();
    Story_Run153();
    Story_Run154();
    Story_Run155();
    Story_Run156();
    Story_Do20(0x83E);
    Story_Run157();
    Story_Run158();
}

void Story_Run159(void)
{
    u32 i;
    s32 obj;

    Story_Run160(141);
    for (i = 0; i != 6; i++) {
        Story_Run10(0x4049d2, 1);
        Story_Run161(8);
        Story_Run162(8);
        Story_Run11(0x10000, 1);
        Story_Run163(8);
        Story_Run164(8);
        if (i == 1) {
            Story_Place10(0x10000, 0x10000, 0x10000);
        }
    }
    Story_Place11(0x20000, 0x10000, 0x10000);
    Story_Run165(30);
    Story_Run12(0x26666, 0x4ccc);
    Story_Run13(0xa70000, -1, 0x2110000, 1);
    Story_Run166();
    Story_Place12(0x10000, 0x20000, 0x10000);
    Story_Run167(20);
    Story_Run168(144);
    Story_Run169(0x200d088, 65, 31);
    Story_SetRect8(0, 0, 1, 1, 10, 31);
    Story_SetRect9(87, 42, 10, 33, 1, 2);
    Story_Run170(40);
    Story_Run171(0, 0, 0);
    Story_Run14(0x66666, 0xcccc);
    Story_Run15(0x1870000, -1, 0xb10000, 1);
    Story_Run172();
    Story_Place13(0x10000, 0x20000, 0x10000);
    Story_Run173(20);
    Story_Run174(144);
    Story_Run175(0x200d088, 79, 9);
    Story_SetRect10(0, 0, 1, 1, 24, 9);
    Story_SetRect11(87, 42, 24, 11, 1, 2);
    Story_Run176(40);
    Story_Run177(0, 0, 0);
    Story_Run16(0x26666, 0x4ccc);
    Story_Run17(0x2470000, -1, 0xc10000, 1);
    Story_Run178();
    Story_Place14(0x10000, 0x20000, 0x10000);
    Story_Run179(20);
    Story_Run180(144);
    Story_Run181(0x200d088, 91, 10);
    Story_SetRect12(0, 0, 1, 1, 36, 10);
    Story_SetRect13(87, 42, 36, 12, 1, 2);
    Story_Run182(40);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Story_Run183();
    Story_Run184();
    Story_Run18(0xe80000, -1, 0x1dd0000, 0);
    Story_Run185();
    Story_Run186(1);
    Story_Place15(0x20000, 0x10000, 0x10000);
    Story_Run187();
    Story_Run188();
    Story_Run189(40);
    Story_Do3(0x121);
    Story_Place16(-1, -1, 0xe666);
    Story_Run190();
    Story_Run191(20);
    Story_SetRect14(0, 40, 13, 66, 3, 3);
    Story_Run192(20);
    obj = Story_Check2(223, 0xe80000, 0x100000, 0x1d00000);
    Story_Run193(40);
    Story_Run194(obj, 1);
    Story_Run19(0x1077, 1);
}

/* Runs a sequence of position/scale/timing calls for actor pair 0 and 1,
 * copying a stored pair of 32-bit fields (offsets +8, +16) from actor 0's
 * record onto actor 1 partway through, then runs an analogous sequence for
 * actors 5, 9, 10 and 11. */
void Scene_StagePairedActors(void)
{
    u32 i;
    u8 *record;

    Audio_PlayCue_1(17);
    Motion_SetSpeed_1(0, 0x8000, 0x4000);
    Motion_SetPosReset_1(0, 231, 0x1ea);
    Motion_ArmCb_1(0, 0xc000, 30);
    Motion_CallWaitAnim_1(0, 3);
    Battle_WaitMode0_1(180);
    Motion_SetVarCbObj_1(0, 2);
    Battle_WaitMode0_2(80);
    BattleFx_SpawnLinked_1(0, 0x101, 60);
    Motion_SetSpeed_2(0, 0x8000, 0x4000);
    Motion_SetPosReset_2(0, 246, 0x1df);
    Motion_ArmCb_2(0, 0xe000, 10);
    /* Copy actor 0's stored fields at +8 and +16 onto actor 1, if a record
     * for actor 0 exists. */
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Motion_SetHPosTerrain_1(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Motion_SetSpeed_3(1, 0x10000, 0x8000);
    Motion_SetPosReset_3(1, 0x101, 0x1eb);
    Motion_ArmCb_3(0, 0x2000, 0);
    Motion_ArmCb_4(1, 0xa000, 40);
    Motion_SetVarCb_1(0, 2);
    Motion_SetVarCbObj_2(1, 2);
    Battle_WaitMode0_3(20);
    BattleFx_SpawnLinked_2(0, 0x101, 0);
    BattleFx_SpawnLinked_3(1, 0x101, 80);
    Object_SetModeById_1(0, 3);
    Motion_CallWaitAnim_2(1, 4);
    Motion_SetSpeed_4(0, 0x13333, 0x9999);
    Motion_SetSpeed_5(1, 0x13333, 0x9999);
    Motion_ResetPosMode2_1(0, 0x109, 0x1c5);
    Motion_SetPosReset_4(1, 0x11a, 0x1d5);
    Object_SetModeById_2(0, 1);
    Motion_ArmCb_5(0, 0xe000, 0);
    Motion_ArmCb_6(1, 0xe000, 40);
    BattleFx_SpawnLinked_4(0, 0x100, 0);
    BattleFx_SpawnLinked_5(1, 0x100, 0);
    Motion_Launch_1(0, 6, 0);
    Motion_Launch_2(1, 6, 60);
    Motion_SetHPosTerrain_2(5, 0x1db0000, 0x14c0000);
    Motion_SetHPosTerrain_3(9, 0x1eb0000, 0x14c0000);
    Motion_SetHPosTerrain_4(11, 0x1cb0000, 0x15c0000);
    Motion_SetHPosTerrain_5(10, 0x1fb0000, 0x15c0000);
    Motion_SetSpeedLim_1(0x73333, 0xe666);
    Motion_CamBounds_1(0x1e50000, -1, 0x1590000, 1);
    Motion_ArmCb_7(5, 0x6000, 0);
    Motion_ArmCb_8(9, 0x5000, 0);
    Motion_ArmCb_9(11, 0x5000, 0);
    Motion_ArmCb_10(10, 0x5000, 0);
    Object_CommitPositionThenWaitIfModeZero_1();
    Battle_WaitMode0_4(40);
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Story_Place17(5, 0x6000, 0);
    Story_Place18(9, 0x5000, 0);
    Story_Place19(10, 0x8000, 0);
    Story_Place20(12, 0x5000, 40);
    Story_Place21(11, 0xd000, 40);
    Story_Run195(11, 2);
    Story_Run196(11, 20);
    Story_Run197(12, 2);
    Story_Run20(12, 0x102);
    Story_Run198(60);
    Story_Run199(12, 10);
    Story_Run200(10, 12, 0);
    Story_Run201(5, 12, 0);
    Story_Run202(9, 12, 0);
    Story_Run203(40);
    Story_Run204(10, 1);
    Story_Place22(10, 0x8000, 10);
    Story_Run205(10, 3);
    Story_Run206(10, 10);
    Story_Run207(11, 0, 10);
    Story_Run208(11, 3);
    Story_Run209(10);
    Story_Run210(5, 0, 0);
    Story_Place23(9, 0x8000, 20);
    Story_Run211(5, 2);
    Story_Run212(9, 2);
    Story_Run213(10);
    Story_Place24(5, 0xc000, 0);
    Story_Place25(9, 0xb000, 10);
    Story_Place26(10, 0xb000, 40);
    Story_Run214(10, 3);
    Story_Run215(10, 10);
    Story_Place27(5, 0x100, 20);
    Story_Run21(12, 0x102);
    Story_Run216(12, 3);
    Story_Run217(40);
    Story_Place28(1, 0x15a0000, 0x1b80000);
    Story_Run218(1);
    Story_Run219(1, 0);
    Story_Place29(0, 0x2000, 0);
    Story_Place30(1, 0xa000, 20);
    Story_Run220();
    Story_Run221();
    Story_Place31(1, 0x1180000, 0x1d60000);
    Story_Run22(0x1050000, -1, 0x1d20000, 0);
    Story_Run222();
    Story_Run223(1);
    Story_Run224();
    Story_Run225();
    Story_Run226(20);
    Story_Place32(0, 0x3000, 10);
    Story_Place33(1, 0x102, 0);
    Story_Run227(1, 3);
    Story_Run228(1, 0);
    Story_Run229(10);
    Story_Run230(1, 4);
    Story_Run231(10);
    Story_Run232(1, 0);
    Story_Run233(10);
    Story_Do4(0x109b);
    Story_Run234(11, 0);
    Story_Place34(11, 0xd000, 10);
    Story_Run235(0, 2);
    Story_Run236(0, 2);
    Story_Run237(10);
    Story_Place35(0, 0xe000, 0);
    Story_Place36(1, 0xe000, 20);
    Story_Run238();
    Story_Run239();
    Story_Run23(0x1dd0000, -1, 0x14e0000, 0);
    Story_Run240();
    Story_Run241(1);
    Story_Run242();
    Story_Run243();
    Story_Run244(20);
    Story_Run245(9, 2);
    Story_Run246(10);
    Story_Place37(9, 0x8000, 30);
    Story_Run247(9, 20);
    Story_Run248(5, 0, 40);
    Story_Run249(5, 4);
    Story_Run250(20);
    Story_Run251(5, 10);
    Story_Run252(12, 2);
    Story_Run253(80);
    Story_Run254(12, 20);
    Story_Run255(5, 2);
    Story_Place38(5, 0xc000, 0);
    Story_Place39(9, 0xb000, 20);
    Story_Run256(12, 3);
    Story_Run257(12, 20);
    Story_Run258(5, 2);
    Story_Run259(9, 2);
    Story_Place40(12, 0xb000, 40);
}

void Story_Run260(void)
{
    u8 *obj;
    s32 other;
    s32 tbl;
    s32 left;
    s32 cnt;
    s32 mes_a;
    s32 mes_b;

    Story_Run261(161);
    Story_Run262(12, 3);
    Story_Run263(40);
    other = Story_Check3(12);
    if (other != 0) {
        Story_Run264(13, *(volatile s32 *)(other + 8), *(volatile s32 *)(other + 16));
    }
    Story_Run265(12, 0, 0);
    Story_Run266(20);
    Story_Place41(13, 0x3000, 40);
    Story_Run267(5, 3);
    Story_Run268(5, 3);
    Story_Run269(40);
    Story_Run270(5, 2);
    Story_Run271(5, 20);
    Story_Run272(13, 3);
    Story_Run273(10);
    Story_Run274(5, 3);
    Story_Place42(9, 0x8000, 10);
    Story_Run275(9, 2);
    Story_Run276(9, 40);
    Story_Run277(5, 3);
    Story_Run278(40);
    Story_Place43(9, 0xb000, 40);
    Story_Run279(13, 2);
    Story_Run280(13, 20);
    Story_Run281(5, 1);
    Story_Run282(40);
    Story_Run283(13, 3);
    Story_Run284(10);
    Story_Run285(13, 40);
    Story_Run286(10, 1);
    Story_Run287(10, 3);
    Story_Run288(10);
    Story_Run289(10, 10);
    Story_Run290(11, 1);
    Story_Run291(11, 3);
    Story_Run292(11, 80);
    Story_Run293(13, 2);
    Story_Run294(13, 40);
    Story_Run295(5, 2);
    Story_Run296(10);
    Story_Run297(5, 10);
    Story_Run298(13, 2);
    Story_Run299(9, 2);
    Story_Run300(80);
    Story_Run301(5, 4);
    Story_Run302(20);
    Story_Run303(5, 80);
    Story_Run304(13, 4);
    Story_Run305(13, 80);
    Story_Run306(5, 2);
    Story_Run307(4);
    Story_Run308(5, 20);
    Story_Run309(10, 1);
    Story_Run310(10, 3);
    Story_Run311(10, 10);
    Story_Run312(11, 1);
    Story_Run313(11, 10);
    Story_Run314(10, 1);
    Story_Run315(10, 10);
    Story_Place44(9, 0x3000, 80);
    Story_Place45(9, 0x105, 80);
    Story_Run316(11, 1);
    Story_Place46(11, 0x5000, 40);
    Story_Run317(11, 2);
    Story_Run318(11, 20);
    Story_Run319();
    Story_Run320();
    Story_Run24(0x1050000, -1, 0x1d20000, 0);
    Story_Run321();
    Story_Run322(1);
    Story_Run323();
    Story_Run324();
    Story_Run325(20);
    Story_Place47(0, 0x3000, 0);
    Story_Place48(1, 0xb000, 20);
    Story_Run326(0, 2);
    Story_Run327(1, 2);
    Story_Place49(0, 0x8000, 0x4000);
    Story_Place50(1, 0x8000, 0x4000);
    Story_Place51(0, 244, 0x1de);
    Story_Place52(1, 0x104, 0x1ea);
    Story_Run328(0);
    Story_Run329(0, 1);
    Story_Place53(0, 0x3000, 0);
    Story_Place54(1, 0xb000, 20);
    Story_Run330(0, 2);
    Story_Run331(1, 4);
    Story_Run332(20);
    tbl = Story_Check30(1);
    cnt = 0;
    tbl += 216;
    left = 14;
    do {
        u32 id = *(volatile u16 *)(tbl)& 0x1ff;
        tbl = (tbl + 2);
        if (id - 220 > 1) {
            if (id != 223) {
                goto L_02001a86;
            }
        }
        cnt = (cnt + 1);
        L_02001a86:;
        left = (left - 1);
    } while (left >= 0);
    Story_Check4(1, 0);
    if (Story_Check5(0, 0) == 0) {
        mes_a = (s32)gVal2;
        ((void (*)())Story_Check31)(mes_a);
        Story_Run333(1, 3);
        Story_Run334(10);
        if (cnt <= 2) {
            Story_Run335(1, 30);
            Story_Place55(1, 252, 0x1e6);
            Story_Run336(1, 2);
            Story_Run337(10);
            Story_Run338((mes_a + 1), 1, 0);
            goto L_02001bdc;
        }
        Story_Do5(0x10b4);
        Story_Run339(1, 30);
    } else {
        if (cnt <= 2) {
            mes_b = (s32)gVal3;
            Story_Run340(mes_b);
            Story_Run341(1, 3);
            Story_Run342(1, 4);
            Story_Run343(1, 10);
            Story_Run344(1, 4);
            Story_Run345(1, 1);
            Story_Place56(1, 0x20000, 0x10000);
            obj = Story_Check6(0);
            obj[90] &= 254;
            Story_Place57(1, 244, 0x1de);
            Story_Place58(0, 0x18000, 0xc000);
            Story_Run346(0, 6, 0);
            Story_Place59(0, 218, 0x1d7);
            Story_Run347(0);
            Story_Run348((mes_b + 1), 1, 0);
            Story_Run349(0, 2);
            Story_Run350(0, 0, 30);
            {
                u8 value = *(volatile u8 *)&obj[90];

                obj[90] = (u8)(value | 1);
            }
        } else {
            Story_Do6(0x10b5);
            Story_Run351(1, 3);
            Story_Run352(1, 4);
            Story_Run353(1, 10);
            Story_Run354(1, 4);
            Story_Place60(0, 0xe000, 30);
        }
    }
    L_02001bdc:;
    Story_Run25(0x8000, 0x1000);
    Story_Run355(1, 1);
    Story_Run356();
    Story_Place61(1, 0x8000, 30);
    Story_Run357(1, 2);
    Story_Place62(1, 0x8000, 0x4000);
    obj = Story_Check7(1);
    obj[90] &= 254;
    Story_Place63(1, 0x108, 0x1e2);
    {
        u8 value = *(volatile u8 *)&obj[90];

        obj[90] = (u8)(value | 1);
    }
    Story_Place64(1, 0x116, 0x1e0);
    *(volatile s32 *)((s32)obj + 48) = 0x30000;
    *(volatile s32 *)((s32)obj + 52) = 0x20000;
    Story_Run358(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Story_Run359(1, 7);
    Story_Place65(1, 0x138, 0x1d6);
    Story_Run360(1, 1);
    Story_Run361(30);
    Story_Run362(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Story_Run363(1, 7);
    Story_Place66(1, 0x156, 0x1d6);
    Story_Run364(1, 1);
    Story_Run365(30);
    Story_Run366(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Story_Run367(1, 7);
    Story_Place67(1, 0x178, 0x1d6);
    Story_Run368(1, 1);
}

void Scene_RunActorFourteenGuestScene(void)
{
    u32 i;
    u8 *rec;
    s32 record;
    s32 base6_10b6;
    s32 base5_200a;
    s32 none;
    s32 v5;

    Story_Run369(1, 3);
    Story_Run370(10);
    Story_Place68(1, 0x3000, 0);
    record = Story_Check32(14);
    Story_Run371(record, 0);
    Story_Run372(14, 15);
    Story_Place69(14, 0x1880000, 0x1c60000);
    Story_Run373();
    Story_Place70(1, 0xd000, 10);
    Story_Run374(1, 2);
    Story_Place71(1, 0x100, 40);
    Story_Place72(14, 0x5000, 10);
    Story_Run375(14, 2);
    Story_Run376(20);
    base6_10b6 = (s32)gVal4;
    Story_Run377(base6_10b6);
    Story_Run378(14, 0);
    Story_Place73(10, 0x1d50000, 0x15c0000);
    base5_200a = 0x200a;
    Story_Run379(20);
    Story_Run380(base5_200a, 10);
    Story_Run381(base5_200a, 40);
    Story_Place74(10, 0x1fb0000, 0x15c0000);
    Story_Run382(1, 2);
    Story_Run383(40);
    Story_Run384(1, 3);
    Story_Run385(40);
    Story_Place75(1, 0x8000, 0x4000);
    Story_Place76(1, 0x185, 0x1d4);
    Story_Place77(1, 0xd000, 60);
    Story_Run386(1, 20);
    Story_Run387((base6_10b6 + 4), 1, 10);
    Story_Place78(1, 0x8000, 0x4000);
    rec = Story_Check8(1);
    rec[90] &= 254;
    none = 0;
    Story_Place79(1, 0x178, 0x1d6);
    Story_Run388(30);
    {
        u8 value = *(volatile u8 *)&rec[90];

        rec[90] = (u8)(value | 1);
    }
    Story_Run389(14, 4);
    Story_Run390(10);
    Story_Run391((base6_10b6 + 5));
    Story_Run392(14, 20);
    Story_Place80(1, 0x101, 60);
    Story_Run393(14, 3);
    Story_Run394(14, 20);
    Story_Place81(1, 0x102, 60);
    Story_Run395(14, 3);
    Story_Run396(20);
    Story_Place82(14, 0xc000, 20);
    Story_Run26(14, 0x100);
    record = Story_Check33(14);
    Story_Run397(record, 0);
    rec = Story_Check9(14);
    rec[85] = none;
    v5 = 0;
    Story_Run398(220);
    for (i = 0; i != 30; i++) {
        *(volatile s32 *)((s32)rec + 12) += 0x10000;
        Story_Run399(1);
        v5 = i;
    }
    rec[85] = 5;
    Story_Run400(1, 2);
    Story_Run401(1, 10);
    Story_Place83(14, 0x101, 60);
    Story_Place84(14, 0x5000, 10);
    Story_Run402(1, 20);
    Story_Run403(14, 1);
    Story_Run404(14, 20);
    Story_Place85(1, 0x103, 20);
    Story_Run405(1, 30);
    Story_Place86(14, 0x105, 80);
    Story_Place87(14, 0xd000, 40);
    Story_Place88(10, 0x5000, 10);
    Story_Run406();
    Story_Run407();
    Story_Run27(0x1dd0000, -1, 0x14e0000, 0);
    Story_Run408();
    Story_Run409(1);
    Story_Run410();
    Story_Run411();
    Story_Run412(20);
    Story_Run413(10, 4);
    Story_Run414(10, 10);
    Story_Check10(11, 0);
    Story_Run28(0x66666, 0xcccc);
    Story_Run29(0x1760000, -1, 0x1d60000, 1);
    Story_Run415();
    Story_Place89(14, 0x5000, 0);
    Story_Place90(1, 0xe000, 10);
    Story_Run416(1, 2);
    if (Story_Check11(1, 0) != 0) {
        s32 code;

        Story_Run417(10);
        Story_Check12(14, 4);
        do {
            code = 0x10c3;
        } while (0);
        for (;;) {
            Story_Do7(code);
            Story_Check13(14, 0);
            if (Story_Check14(1, 0) != 0)
                break;
            Story_Run418(20);
            Story_Check15(14, 4);
            Story_Run419(10);
            code = 0x10c6;
        }
    }
    Story_Run420(30);
    Story_Run421(14, 3);
    Story_Run422(20);
    Story_Do8(0x10c4);
    Story_Run423(14, 30);
    Story_Run424(14, 3);
    Story_Run425(10);
    Story_Run426(14, 30);
    rec[85] = 0;
    Story_Place91(14, 0x26666, 0x13333);
    Story_Run30((s32)rec, 0x1cc0000, 0, 0x1680000);
    Story_Run427(14);
    Story_Run428(14, 0);
    record = Story_Check34(14);
    Story_Run429(record, 1);
    Story_Run430(30);
    Story_Run431(1, 1);
    Story_Run432();
    Story_Run433(40);
    Story_Place92(1, 0x103, 40);
    Story_Run434(1, 3);
    Story_Run435(20);
    rec = Story_Check16(1);
    {
        u8 value = *(volatile u8 *)&rec[90];

        rec[90] = (u8)(value | 1);
    }
    *(volatile s32 *)((s32)rec + 48) = 0x30000;
    *(volatile s32 *)((s32)rec + 52) = 0x20000;
    Story_Run436(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Story_Run437(1, 7);
    Story_Place93(1, 0x156, 0x1d6);
    Story_Run438(1, 1);
    Story_Run439(30);
    Story_Run440(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Story_Run441(1, 7);
    Story_Place94(1, 0x138, 0x1d6);
    Story_Run442(1, 1);
    Story_Run443(30);
    Story_Run444(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Story_Run445(1, 7);
    Story_Place95(1, 0x116, 0x1e0);
    Story_Run446(1, 1);
    Story_Run447(30);
    Story_Run31(0x8000, 0x1000);
    Story_Run448(0, 1);
    Story_Place96(1, 0x19999, 0xcccc);
    Story_Run449(0, 1, 0);
    Story_Run450(30);
    Story_Run451(1, 3);
    Story_Run452(0, 4);
    Story_Run453(1, 2);
    Story_Run454(0, 3);
    Story_Run455(20);
    Story_Run456(1, 2);
    {
        s32 slot = Story_Check17(0);

        if (slot != 0) {
            Story_Run457(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
        }
    }
    Story_Run458(1);
    Story_Run459(1, 0, 0);
    Story_Run460(220);
    Story_Run461(221);
    Story_Run462(223);
}

void Scene_RunSixPassEffectSequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    Story_Run463();
    Story_Run464(141);
    v5 = 0;
    do {
        Story_Run32(0x4039d2, 1);
        Story_Run465(8);
        Story_Run466(8);
        Story_Run33(0x10000, 1);
        Story_Run467(8);
        Story_Run468(8);
        if (v5 == 1) {
            Story_Place97(0x10000, 0x10000, 0x10000);
        }
        v5 = ((u32)((v5 + 1) << 24) >> 24);
    } while (v5 != 6);
    Story_Do9(0x121);
    Story_Place98(-1, -1, 0xe666);
    Story_SetRect15(0, 40, 13, 46, 3, 3);
    Story_Run469(20);
    rec7 = Story_Check18(222, 0xe80000, 0x100000, 0x900000);
    Story_Run470(40);
    Story_Run471(rec7, 1);
    Story_Run34(0x1078, 1);
    Story_Place99(5, 0x1330000, 0x1150000);
    Story_Place100(9, 0x1330000, 0x1150000);
    Story_Place101(11, 0x1330000, 0x1150000);
    Story_Place102(10, 0x1330000, 0x1150000);
    Story_Place103(14, 0x1330000, 0x1150000);
    Story_Place104(0, 0x13333, 0x9999);
    Story_Run472(0, 232, 156);
    Story_Run473(10);
    record = Story_Check19(0);
    if (record != 0) {
        Story_Run474(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Story_Place105(1, 0x13333, 0x9999);
    Story_Run475(1, 218, 172);
    Story_Run476(1, 0, 0);
    Story_Run477(20);
    Story_Run478(145);
    Story_Place106(0x40000, 0x40000, 0x10000);
    Story_Run479(20);
    Story_Place107(0x10000, 0x10000, 0x10000);
    Story_Run480(40);
    Story_Place108(0, 0xd000, 0);
    Story_Place109(1, 0x5000, 50);
    Story_Run481(144);
    Story_Place110(0x30000, 0x30000, 0x10000);
    Story_Place111(0, 0x8000, 0);
    Story_Run482(1, 0, 50);
    Story_Place112(0x10000, 0x10000, 0x10000);
    Story_Run483(0, 0, 0);
    Story_Place113(1, 0x8000, 50);
    Story_Place114(0, 0xb000, 0);
    Story_Place115(1, 0xd000, 0);
    Story_Run484(144);
    Story_Place116(0x30000, 0x30000, 0x10000);
    Story_Run485(30);
    Story_Run486(0, 2, 0);
    Story_Run487(1, 2, 20);
    Story_Run488(0, 6, 0);
    Story_Run489(1, 6, 40);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_Run35();
    Story_Run490();
    Story_Run491(2);
}

/*
 * A scripted-scene bracket around a two-level story-flag test. When flag
 * 0x83e is set the actor speaks one line and the handler ends; otherwise flag
 * 0x83c chooses between two alternative lines and the longer path also plays
 * a motion, waits ten frames and closes. The two tests read with opposite
 * polarity. The trailing zero arguments have no established meaning. The
 * 108-byte owner includes its five-word literal pool.
 */
void Scene_RunActorNineDialogueByFlags(void)
{
    Story_Run492();
    if (Story_Check35(0x83e)) {
        Story_Do21(0x10cb);
        Story_Apply(9, 0);
    } else {
        if (Story_Check36(0x83c) == 0)
            Story_Do22(0x1079);
        else
            Story_Do23(0x107b);
        Story_Place305(9, 0, 0);
        Story_Do24(10);
        Story_Apply2(9, 0);
    }
    Story_Run493();
}

/*
 * A scripted-scene bracket around a two-level story-flag test. Flag 0x83e
 * selects a single line; otherwise flag 0x83c chooses between two lines and
 * the longer path plays a motion, waits ten frames and closes. The two tests
 * read with opposite polarity. The trailing zero arguments have no
 * established meaning. The 108-byte owner includes its five-word pool.
 */
void Dialogue_RunActorFiveTalk(void)
{
    Story_Run494();
    if (Story_Check37(0x83e)) {
        Story_Do25(0x10c9);
        Story_Apply3(5, 0);
    } else {
        if (Story_Check38(0x83c) == 0)
            Story_Do26(0x107a);
        else
            Story_Do27(0x107c);
        Story_Place306(5, 0, 0);
        Story_Do28(10);
        Story_Apply4(5, 0);
    }
    Story_Run495();
}

/*
 * Talk handler for actor 10: the scripted-scene bracket around one line of
 * dialogue, with no branch and no state. It is published into a script record
 * rather than called from the image.
 *
 * The thirty-two-byte owner includes its one pool word, which carries the
 * line id, and the zero alignment halfword after it. The second argument of
 * the tail call is zero at every site in this bank and its role is not
 * established.
 */
void Dialogue_RunActorTenDialogue(void)
{

    Story_Run496();
    Story_Do29(0x10ca);
    Story_Apply5(10, 0);
    Story_Run497();
}

/*
 * Talk handler for actor 11, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void Dialogue_RunActor11TalkLine(void)
{

    Story_Run498();
    Story_Do30(0x10c7);
    Story_Apply6(11, 0);
    Story_Run499();
}

/*
 * Talk handler for actor 13, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void Dialogue_ShowLine10C8(void)
{

    Story_Run500();
    Story_Do31(0x10c8);
    Story_Apply7(13, 0);
    Story_Run501();
}

/*
 * Talk handler for actor 14, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void Dialogue_RunActor14TalkLine(void)
{

    Story_Run502();
    Story_Do32(0x10cc);
    Story_Apply8(14, 0);
    Story_Run503();
}

/*
 * Same bracket and line call as the talk handlers, but the tail pairs actors
 * 9 and 10 through the overlay's own call.
 */
void Dialogue_RunLine1072WithPair9And10(void)
{
    Story_Run504();
    Story_Do33(0x1072);
    Story_Apply9(9, 10);
    Story_Run505();
}

void Scene_Forward(void)
{
    Story_Run93();
}

void Lifted(void)
{
    u16 i;
    u8 *p10;
    u8 *p10b;
    u8 *p9;
    u8 *p9b;
    u8 *p9c;
    u8 *rec;
    u8 *rec2;
    u8 *record;
    u8 *work;
    s32 base5_4009;
    s32 v6;
    s32 base5_8009;
    s32 zero;

    Story_Run506();
    Story_SetRect16(27, 16, 5, 1, 27, 17);
    Story_Run36(0x1d70000, -1, 0x1050000, 0);
    Story_Run507();
    Story_Run508();
    rec2 = (u8 *)Story_Run83(8);
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    rec = (u8 *)Story_Run84(0);
    p9 = *(u8 **)(rec + 80) + 38;
    *p9 = 0;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Story_Run37(0, 0x100);
    Story_Place117(0, 0x1d70000, 0x1220000);
    p10 = rec + 85;
    *p10 = 0;
    *(s32 *)(rec + 12) = 0x280000;
    work = *(u8 **)gWork;
    *(s32 *)(((s32)work + 0x1c0)) = 0x203;
    *(s32 *)(((s32)work + 0x1c8)) = 32;
    Story_Run509();
    Story_Run510();
    Story_Run511(20);
    Story_Place118(8, 0x1d70000, 0x1220000);
    Story_Run512(190);
    Story_Run513(0, 2);
    for (i = 0; i != 90; i++) {
        *(s32 *)(rec + 12) += -0x1999;
        *(s32 *)(rec + 24) += 0x28f;
        *(s32 *)(rec + 28) += 0x28f;
        *(s32 *)(rec2 + 24) += 0x28f;
        *(s32 *)(rec2 + 28) += 0x28f;
        Story_Run514(1);
    }
    *p10 = 5;
    Story_Run515(80);
    Story_Run38(0x4ccc, 0x999);
    Story_Run39(0x1d70000, -1, 0x1220000, 1);
    for (i = 0; i != 60; i++) {
        *(s32 *)(rec + 12) += -0x8000;
        Story_Run516(1);
    }
    *p10 = 3;
    Story_Run517(20);
    Story_Run518(0, 1);
    {
        u8 *record = Story_Run85(0);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Story_Run519(0, 0);
    {
        s32 shown = 1;

        *p9 = shown;
    }
    ((void (*)())Story_Check39)(8, 0, 0);
    Story_Run520();
    Story_Run521(20);
    Story_Run522(0, 1);
    Story_Place119(0, 0x8000, 0x4000);
    Story_Place120(0, 0x1d7, 0x136);
    Story_Run523(20);
    Story_Run524(0, 2);
    Story_Run525(20);
    zero = 0;
    record = (u8 *)Story_Run86(0);
    if ((s32)record != 0) {
        Story_Run526(5, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    record = (u8 *)Story_Run87(0);
    if ((s32)record != 0) {
        Story_Run527(1, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    Story_Place121(1, 0x8000, 0x4000);
    Story_Place122(5, 0x8000, 0x4000);
    Story_Place123(5, 0x1c5, 0x12e);
    Story_Place124(1, 0x1e9, 0x12e);
    Story_Run528(5, 1);
    Story_Place125(1, 0x3000, 0);
    Story_Place126(5, 0x6000, 10);
    Story_Place127(0, 0x100, 0);
    Story_Place128(5, 0x100, 0);
    Story_Place129(1, 0x100, 0);
    Story_Run529(0, 2, 0);
    Story_Run530(5, 2, 0);
    Story_Run531(1, 2, 40);
    Story_Run532(0, 3);
    Story_Run533(5, 3);
    Story_Run534(1, 3);
    Story_Run535(40);
    Story_Run40(0, 0x102);
    Story_Run41(5, 0x102);
    Story_Run42(1, 0x102);
    Story_Run536(60);
    Story_Place130(0, 0x13333, 0x9999);
    Story_Place131(5, 0x13333, 0x9999);
    Story_Place132(1, 0x13333, 0x9999);
    Story_Place133(0, 0x1d7, 0x15a);
    Story_Place134(5, 0x1af, 0x152);
    Story_Place135(1, 0x1ff, 0x152);
    Story_Run537(0);
    Story_Run538(0, 1);
    Story_Run539(5);
    Story_Run540(5, 1);
    Story_Run541(1);
    Story_Run542(1, 1);
    Story_Place136(0, 0x4ccc, 0x2666);
    Story_Place137(5, 0x4ccc, 0x2666);
    Story_Place138(1, 0x4ccc, 0x2666);
    Story_Place139(5, 0xc000, 0);
    Story_Place140(1, 0xc000, 0);
    Story_Place141(0, 0xc000, 60);
    Story_Place142(5, 0x8000, 0);
    Story_Run543(1, 0, 0);
    Story_Place143(0, 0x4000, 60);
    Story_Run544(5, 0, 0);
    Story_Place144(1, 0x8000, 40);
    Story_Run545(0, 0, 40);
    Story_Run546(0, 3);
    Story_Run547(1, 2);
    Story_Run548(20);
    Story_Run549(1, 4);
    Story_Run550(20);
    Story_Place145(0, 0x8000, 40);
    Story_Run551(0, 3);
    Story_Run552(5, 2);
    Story_Run553(20);
    Story_Place146(5, 0x101, 60);
    Story_Place147(0, 0x4000, 0);
    Story_Place148(1, 0x3000, 0);
    Story_Place149(5, 0x6000, 60);
    rec = (u8 *)Story_Run88(9);
    p9b = *(u8 **)(rec + 80) + 38;
    *p9b = zero;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    Story_Run43(9, 0x100);
    Story_Place150(9, 0x1d70000, 0x1220000);
    p10b = rec + 85;
    *p10b = zero;
    *(s32 *)(rec + 12) = 0x280000;
    Story_Run554(1);
    Story_Do10(0x103c);
    Story_Run555(9, 0);
    Story_Run556(0, 4, 0);
    Story_Run557(5, 4, 0);
    Story_Run558(1, 4, 40);
    Story_Place151(0, 0xc000, 0);
    Story_Place152(5, 0xe000, 0);
    Story_Place153(1, 0xb000, 0);
    Story_Run44(0x20000, 0x4000);
    Story_Run45(0x1d70000, -1, 0x1350000, 1);
    Story_Run559();
    Story_Place154(8, 0x1d70000, 0x1220000);
    Story_Run560(190);
    Story_Run561(9, 2);
    for (i = 0; i != 90; i++) {
        *(s32 *)(rec + 12) += -0x1999;
        *(s32 *)(rec + 24) += 0x28f;
        *(s32 *)(rec + 28) += 0x28f;
        *(s32 *)(rec2 + 24) += 0x28f;
        *(s32 *)(rec2 + 28) += 0x28f;
        Story_Run562(1);
    }
    *p10b = 5;
    Story_Run563(80);
    for (i = 0; i != 60; i++) {
        *(s32 *)(rec + 12) += -0x8000;
        Story_Run564(1);
    }
    *p10b = 3;
    Story_Run565(30);
    Story_Run566(9, 1);
    {
        u8 *record = Story_Run90(9);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Story_Run567(9, 0);
    {
        s32 shown = 1;

        *p9b = shown;
    }
    Story_Run568(8, 0, 0);
    Story_Do11(30);
    Story_Place155(9, 0x13333, 0x9999);
    Story_Place156(9, 0x1d7, 0x132);
    Story_Run569(20);
    Story_Place157(9, 0x100, 0);
    Story_Run570(9, 2, 80);
    Story_Run571(9, 3);
    Story_Run572(30);
    Story_Place158(9, 0x8000, 30);
    Story_Run573(9, 2);
    Story_Run574(20);
    Story_Run575(9, 0, 30);
    Story_Run576(9, 2);
    Story_Run577(20);
    Story_Place159(9, 0x4000, 30);
    Story_Run578(9, 3);
    Story_Run579(30);
    Story_Run580(9, 20);
    Story_Place160(9, 0x26666, 0x13333);
    Story_Place161(9, 0x1a7, 0x132);
    Story_Place162(9, 0xb000, 0);
    Story_Place163(0, 0xb000, 0);
    Story_Place164(5, 0xa000, 0);
    Story_Place165(1, 0xb000, 30);
    Story_Run581(9, 2);
    Story_Run582(30);
    Story_Run583(9, 3);
    Story_Run584(40);
    Story_Place166(9, 0x207, 0x132);
    Story_Place167(9, 0xd000, 0);
    Story_Place168(0, 0xd000, 0);
    Story_Place169(5, 0xe000, 0);
    Story_Place170(1, 0xd000, 20);
    Story_Run585(9, 2);
    Story_Run586(30);
    base5_4009 = (s32)gVal6;
    Story_Run587(9, 4);
    Story_Run588(40);
    Story_Run589(base5_4009, 30);
    Story_Run590(0, 5, 0);
    Story_Run591(40);
    Story_Run592(0, 2);
    Story_Run593(5, 2);
    Story_Place171(1, 0x101, 40);
    Story_Run594(1, 40);
    Story_Place172(9, 0x5000, 20);
    Story_Run595(base5_4009, 20);
    Story_Place173(0, 0xd000, 0);
    Story_Place174(5, 0xe000, 20);
    Story_Place175(0, 0x101, 0);
    Story_Place176(5, 0x101, 40);
    Story_Run596(9, 4);
    Story_Run597(base5_4009, 10);
    Story_Run598(1, 3);
    Story_Run599(5, 3);
    Story_Run600(0, 3);
    Story_Run601(20);
    Story_Run602(9, 3);
    Story_Run603(40);
    Story_Run46(0x8000, 0x1000);
    Story_Run47(0x2150000, -1, 0x1530000, 1);
    Story_Place177(9, 0x8000, 0x4000);
    Story_Place178(9, 0x215, 0x153);
    Story_Run604();
    Story_Run605(0, 0, 0);
    Story_Run606(5, 0, 0);
    Story_Run607(1, 0, 20);
    Story_Place179(9, 0xd000, 40);
    Story_Run608(9, 0, 40);
    Story_Place180(9, 0x3000, 60);
    Story_Run609(9, 3);
    Story_Run610(30);
    Story_Run611(9, 1);
    Story_Place181(9, 0x19999, 0xcccc);
    Story_Place182(9, 0x1c7, 0x168);
    Story_Run612(20);
    Story_Place183(0, 0x4000, 0);
    Story_Place184(5, 0x2000, 0);
    Story_Place185(1, 0x5000, 0);
    Story_Place186(9, 0x1c7, 0x168);
    Story_Run613(20);
    Story_Place187(9, 0x5000, 30);
    Story_Run614(9, 3);
    Story_Run615(30);
    Story_Place188(9, 0x1d7, 0x168);
    Story_Run616(20);
    Story_Place189(9, 0x3000, 30);
    Story_Place190(9, 0x100, 0);
    Story_Run617(9, 2);
    Story_Run618(30);
    Story_Run619(9, 30);
    Story_Run620(9, 3);
    Story_Run621(20);
    Story_Run622(9, 30);
    Story_Place191(5, 0xcccc, 0x6666);
    Story_Place192(5, 0x1b8, 0x15a);
    Story_Run623(10);
    Story_Place193(5, 0x2000, 10);
    Story_Run624(5, 20);
    Story_Place194(1, 0xcccc, 0x6666);
    Story_Place195(1, 0x1ef, 0x15a);
    Story_Run625();
    Story_Run626(10);
    Story_Place196(1, 0x6000, 10);
    Story_Check20(1, 0);
    Story_Run627(5, 0, 0);
    Story_Place197(1, 0x8000, 0);
    if (Story_Check21(0, 0) == 1) {
        bump_step(1);
    }
    Story_Place198(9, 0xc000, 30);
    Story_Run628(9, 30);
    Story_Do12(0x1048);
    Story_Place199(5, 0x2000, 0);
    Story_Place200(1, 0x6000, 30);
    Story_Run629(9, 2);
    Story_Run630(9, 10);
    Story_Run631(9, 3);
    Story_Run632(9, 20);
    Story_Run633(9, 2);
    Story_Run634(9, 10);
    Story_Run48(9, 0x102);
    Story_Run635(40);
    Story_Run636(9, 10);
    Story_Place201(9, 0x2000, 20);
    Story_Run637(0, 2);
    Story_Run638(5, 2);
    Story_Run639(1, 2);
    Story_Run640(10);
    Story_Place202(0, 0x2000, 0);
    Story_Place203(5, 0x2000, 0);
    Story_Place204(1, 0x2000, 20);
    Story_Run641(9);
    Story_Run642(0);
    Story_Run643(5);
    Story_Run644(1);
    Story_Run49(0x80000, 0x10000);
    Story_Run50(0x2c70000, -1, 0x1d00000, 1);
    Story_Run645();
    Story_Place205(9, 0x24d0000, 0x1610000);
    Story_Run646(40);
    Story_Run51(0x1009, 0);
    Story_Place206(9, 0x1d70000, 0x1680000);
    Story_Run647(80);
    Story_Run52(0x1d70000, -1, 0x1720000, 1);
    Story_Run648();
    Story_Run649(10);
    Story_Run650(9, 3);
    Story_Run651(10);
    Story_Place207(9, 0xd000, 20);
    Story_Place208(0, 0xe000, 0);
    Story_Place209(5, 0xe000, 0);
    Story_Place210(1, 0xe000, 20);
    Story_Run53(0x2c70000, -1, 0x930000, 1);
    Story_Run652();
    Story_Place211(9, 0x2540000, 0xee0000);
    Story_Run653(40);
    Story_Run54(0x1009, 0);
    Story_Place212(9, 0x1d70000, 0x1680000);
    Story_Run654(80);
    Story_Run55(0x1d70000, -1, 0x1720000, 1);
    Story_Run655();
    Story_Run656(10);
    Story_Run657(9, 2);
    Story_Run658(10);
    Story_Run659(9, 4, 0);
    Story_Place213(9, 0xb000, 20);
    Story_Run660(0, 1);
    Story_Run661(5, 1);
    Story_Run662(1, 1);
    Story_Place214(0, 0xa000, 0);
    Story_Place215(5, 0xa000, 0);
    Story_Place216(1, 0xa000, 20);
    Story_Run56(0xe70000, -1, 0x930000, 1);
    Story_Run663();
    Story_Place217(9, 0x1340000, 0xfa0000);
    Story_Run664(40);
    Story_Run57(0x2009, 0);
    Story_Place218(9, 0x1d70000, 0x1680000);
    Story_Run665(80);
    Story_Run58(0x1d70000, -1, 0x1720000, 1);
    Story_Run666();
    Story_Run667(10);
    Story_Run668(9, 3);
    Story_Run669(10);
    Story_Run670(9, 6, 0);
    Story_Place219(9, 0x5000, 20);
    Story_Run671(0, 2);
    Story_Run672(5, 2);
    Story_Run673(1, 2);
    Story_Place220(0, 0x6000, 0);
    Story_Place221(5, 0x6000, 0);
    Story_Place222(1, 0x6000, 20);
    Story_Run59(0xe70000, -1, 0x1d00000, 1);
    Story_Run674();
    Story_Place223(9, 0x1320000, 0x16a0000);
    Story_Run675(40);
    Story_Run60(0x2009, 0);
    Story_Place224(9, 0x1d70000, 0x1680000);
    Story_Run676(80);
    Story_Run61(0x1d70000, -1, 0x1720000, 1);
    Story_Run677();
    Story_Run678(30);
    Story_Place225(9, 0x104, 60);
    Story_Place226(0, 0x4000, 0);
    Story_Place227(5, 0x2000, 0);
    Story_Place228(1, 0x6000, 40);
    Story_Place229(5, 0x102, 40);
    Story_Run679(5, 1);
    Story_Run680(5, 20);
    Story_Run681(9, 4, 0);
    Story_Place230(9, 0xb000, 20);
    Story_Run682(9, 2);
    Story_Run62(0xa009, 10);
    Story_Run683(9, 3);
    Story_Run63(0xa009, 20);
    Story_Place231(0, 0x101, 0);
    Story_Place232(5, 0x101, 0);
    Story_Place233(1, 0x101, 60);
    Story_Place234(9, 0xc000, 20);
    Story_Run684(9, 4);
    Story_Check22(0x8009, 0);
    if (Story_Check23(0, 0) == 1) {
        bump_step(1);
    }
    Story_Run685(20);
    Story_Place235(1, 0x8000, 10);
    Story_Run686(1, 10);
    Story_Do13(0x1056);
    Story_Place236(1, 0x6000, 20);
    Story_Run687(9, 4, 40);
    Story_Run688(0x8009, 10);
    Story_Place237(0, 0x101, 0);
    Story_Place238(5, 0x101, 0);
    Story_Place239(1, 0x101, 80);
    Story_Run64(9, 0x102);
    Story_Run689(40);
    Story_Run690(0x8009, 40);
    Story_Place240(0, 0x106, 0);
    Story_Place241(5, 0x106, 0);
    Story_Place242(1, 0x106, 60);
    Story_Run691(5, 0, 0);
    Story_Place243(1, 0x8000, 20);
    Story_Check24(0x8009, 0);
    if (Story_Check25(0, 0) == 1) {
        bump_step(1);
    }
    Story_Run692(9, 1);
    Story_Run693(20);
    Story_Place244(5, 0x2000, 0);
    Story_Place245(1, 0x5000, 20);
    Story_Run694(0x8009, 40);
    Story_Do14(0x105b);
    Story_Run695(5, 4);
    Story_Run696(5, 10);
    Story_Place246(9, 0xb000, 10);
    Story_Run697(9, 1);
    Story_Run698(9, 4, 40);
    Story_Run65(0xa009, 10);
    Story_Run699(0, 1);
    Story_Run700(5, 1);
    Story_Run701(1, 1);
    Story_Run702(40);
    Story_Place247(9, 0xc000, 10);
    Story_Run703(9, 1);
    Story_Run704(0x8009, 40);
    Story_Place248(0, 0x105, 0);
    Story_Place249(5, 0x105, 0);
    Story_Place250(1, 0x105, 120);
    Story_Place251(1, 0x107, 0);
    Story_Run705(1, 4, 40);
    Story_Run706(1, 10);
    Story_Run707(9, 1);
    Story_Run708(40);
    Story_Place252(9, 0x4000, 80);
    Story_Run709(0x8009, 10);
    Story_Place253(1, 0x8000, 0);
    Story_Run710(0, 0, 30);
    Story_Run711(0, 2);
    Story_Run712(1, 2);
    Story_Run713(20);
    Story_Run714(5, 0, 0);
    Story_Place254(0, 0x8000, 30);
    Story_Run715(0, 2);
    Story_Run716(5, 2);
    Story_Run717(20);
    Story_Place255(0, 0x4000, 0);
    Story_Place256(5, 0x2000, 0);
    Story_Place257(1, 0x5000, 40);
    Story_Run66(0, 0x102);
    Story_Run67(5, 0x102);
    Story_Run68(1, 0x102);
    Story_Run718(80);
    Story_Run719(9, 3);
    Story_Run720(20);
    Story_Run721(0x8009, 40);
    rec = (u8 *)Story_Run92(9);
    v6 = 192;
    Story_Run722(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Story_Run723(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Story_Place258(9, 0x1d7, 0x18b);
    Story_Run724(6);
    Story_Place259(9, 0x4ccc, 0x2666);
    p9c = rec + 90;
    *p9c &= 254;
    Story_Place260(9, 0x1d9, 0x18b);
    Story_Run725(9);
    Story_Run726(9, 2);
    Story_Place261(9, 0x1d5, 0x18b);
    Story_Run727(9);
    Story_Run728(9, 2);
    Story_Place262(9, 0x1d7, 0x18b);
    Story_Run729(9);
    Story_Run730(9, 10);
    Story_Place263(9, 0xcccc, 0x6666);
    Story_Place264(9, 0x1d7, 0x19b);
    Story_Place265(9, 0x4ccc, 0x2666);
    *p9c &= 254;
    Story_Place266(9, 0x1da, 0x19b);
    Story_Run731(9);
    Story_Run732(9, 3);
    Story_Place267(9, 0x1d4, 0x19b);
    Story_Run733(9);
    Story_Run734(9, 3);
    Story_Place268(9, 0x1d7, 0x19b);
    Story_Run735(9);
    Story_Run69(9, 0x102);
    Story_Run736(9, 3);
    Story_Run737(9, 10);
    Story_Place269(9, 0x3333, 0x1999);
    Story_Place270(9, 0x1d7, 0x18b);
    Story_Run738(9, 1);
    Story_Run739(30);
    {
        s32 flags = 1 | *p9c;
        *p9c = flags;
    }
    Story_Place271(9, 0xc000, 60);
    Story_Run740(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Story_Run741(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Story_Place272(9, 0x1d7, 0x168);
    Story_Run742(6);
    Story_Run743(40);
    Story_Place273(9, 0x102, 80);
    Story_Place274(0, 0x102, 0);
    Story_Place275(5, 0x102, 0);
    Story_Place276(1, 0x102, 80);
    Story_Run744(5, 1);
    Story_Place277(5, 0x107, 40);
    Story_Run745(5, 20);
    Story_Place278(9, 0xb000, 40);
    Story_Run746(9, 4);
    Story_Run70(0xa009, 30);
    Story_Run747(5, 0, 30);
    Story_Place279(5, 0x106, 60);
    Story_Place280(5, 0x2000, 30);
    Story_Run748(5, 20);
    Story_Run749(0, 4, 0);
    Story_Run750(1, 4, 0);
    Story_Place281(0, 0x8000, 0);
    Story_Place282(1, 0x8000, 40);
    Story_Run751(0, 1);
    Story_Run752(1, 1);
    Story_Run753(40);
    Story_Run754(0, 0, 30);
    Story_Run755(0, 2);
    Story_Run756(1, 2);
    Story_Run757(30);
    Story_Place283(9, 0xd000, 30);
    Story_Run758(9, 1);
    Story_Run759(20);
    Story_Run760(0x8009, 20);
    Story_Place284(0, 0x4000, 0);
    Story_Place285(1, 0x5000, 30);
    Story_Run761(0, 2);
    Story_Run762(1, 2);
    Story_Run763(20);
    Story_Run764(5, 3);
    Story_Run765(20);
    Story_Run766(5, 0, 30);
    Story_Check26(5, 0);
    Story_Place286(0, 0x8000, 0);
    while (Story_Check27(0, 0) != 0) {
        Story_Do15(0x1068);
        Story_Place287(5, 0x107, 0);
        Story_Run767(5, 4, 60);
        Story_Run768(5, 0);
    }
    Story_Do16(0x1069);
    Story_Run769(20);
    Story_Run770(5, 3);
    Story_Run771(20);
    Story_Place288(5, 0x2000, 30);
    Story_Run772(5, 10);
    Story_Place289(0, 0x4000, 10);
    Story_Place290(9, 0xb000, 30);
    Story_Run773(9, 3);
    Story_Run774(20);
    Story_Place291(9, 0xc000, 30);
    Story_Place292(9, 0x3333, 0x1999);
    Story_Place293(9, 0x1d7, 0x160);
    Story_Run775(20);
    base5_8009 = (s32)gVal7;
    Story_Run776(9, 2);
    Story_Run777(20);
    Story_Run778(base5_8009, 60);
    *p9c &= 254;
    Story_Place294(9, 0x1c8, 0x168);
    Story_Run779(20);
    Story_Run780(5, 2);
    Story_Run781(20);
    Story_Do17(0x106d);
    Story_Run782(5, 30);
    Story_Place295(9, 0xb000, 30);
    Story_Run783(9, 3);
    Story_Run784(20);
    Story_Place296(1, 0x101, 60);
    Story_Run785(1, 10);
    Story_Place297(9, 0xd000, 20);
    Story_Run786(9, 3);
    Story_Run787(base5_8009, 20);
    Story_Run788(0, 0, 0);
    Story_Place298(1, 0x8000, 40);
    Story_Place299(0, 0x101, 0);
    Story_Place300(1, 0x101, 40);
    Story_Run789(9, 2);
    Story_Run790(base5_8009, 10);
    Story_Place301(0, 0x4000, 0);
    Story_Place302(1, 0x5000, 30);
    Story_Run791(9, 3);
    Story_Run792(10);
    Story_Run793(base5_8009, 30);
    Story_Run794(1, 3);
    Story_Run795(0, 3);
    Story_Run796(20);
    Story_Run797(0, 0, 0);
    Story_Place303(1, 0x8000, 30);
    Story_Run798(1, 2);
    Story_Run799(30);
    Story_Run800(0, 3);
    Story_Run801(10);
    Story_Place304(1, 0x10000, 0x8000);
    Story_Run802(1, 2);
    record = Story_Check28(0);
    if ((s32)record != 0) {
        Story_Run803(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Story_Run804(1);
    Story_Run805(1, 0, 0);
    Story_Do18(0x83b);
    Story_Run806(5);
    Story_Run807();
    Story_SetRect17(8, 0, 5, 1, 27, 17);
    *(s32 *)((*(u8 **)gWork + 0x1c8)) = 16;
    Story_Do19(0x12f);
    Story_Run808();
}

void Scene_RunSplitPairSteps(s32 a, s32 b)
{
    Story_Apply10(a, 0);
    Story_Do34(b);
}

/*
 * Drain until room: save the s16 counter at scene workspace + 472, prime two
 * channels, then loop while fewer than four of thirty slots are free,
 * requesting more and passing on any event pair that is not -1. On exit it
 * flushes four times with id 224 and restores the saved counter. The 148-byte
 * owner includes its three-word literal pool. Callee roles are not established.
 */
void State_PumpUntilSlotsFree(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s16 saved = *(s16 *)(work + 472);
    s32 first;
    s32 second;
    s32 cnt;

    Story_Run71(0x53);
    Story_Run72(224, 3);
    Story_Run73(0x111b, 1);
    do {
        cnt = 30 - Story_Run74(0);
        cnt -= Story_Run75(1);

        if (cnt <= 3) {
            Story_Run76(0x111c, 1);
            if (Story_Run77(&second, &first) != -1)
                Story_Run78(second, first);
        }
    } while (cnt <= 3);
    Story_Run79(224);
    Story_Run80(224);
    Story_Run81(224);
    Story_Run82(224);
    *(s16 *)(work + 472) = saved;
}
