#include "types.h"
#include "scene.h"

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
extern void Story_unk71_3(s32 arg0);                        /* 0x0200433c -> 080f9010 */
extern void Story_unk72_3(s32 arg0, s32 arg1);              /* 0x02004344 -> 0808a398 */
extern void Story_unk73_3(s32 arg0, s32 arg1);              /* 0x0200434c -> 08015040 */
extern s32 Story_unk74_3(s32 arg0);                         /* 0x02004352 -> 08077248 */
extern s32 Story_unk75_3(s32 arg0);                         /* 0x0200435c -> 08077248 */
extern void Story_unk76_3(s32 arg0, s32 arg1);              /* 0x0200436a -> 08015040 */
extern s32 Story_unk77_3(s32 *out_first, s32 *out_second);  /* 0x02004372 -> 080b0058 */
extern void Story_unk78_3(s32 arg0, s32 arg1);              /* 0x02004382 -> 080772b0 */
extern void Story_unk79_3(s32 arg0);                        /* 0x0200438a -> 08077030 */
extern void Story_unk80_3(s32 arg0);                        /* 0x02004390 -> 08077030 */
extern void Story_unk81_3(s32 arg0);                        /* 0x02004396 -> 08077030 */
extern void Story_unk82_3(s32 arg0);                        /* 0x0200439c -> 08077030 */

void *AcquireOverlayObject(s32, s32, s32, s32);

struct Effect *GetPrimaryEffectRecord(s32 index);
struct Effect *SpawnConfiguredEffectObject(s32 kind, s32 x, s32 y, s32 z);

u8 *Scene_GetRecord_1();

struct ObjectRuntime *Story_unk83_3(u32);
struct ObjectRuntime *Story_unk84_3(u32);

u8 *Story_unk85_3();

struct ObjectRuntime *Story_unk86_3(u32);

struct ObjectRuntime *Story_unk87_3(u32);

struct ObjectRuntime *Story_unk88_3(u32);

u8 *Story_unk89_3();
u8 *Story_unk90_3();
u8 *Story_unk91_3();

struct ObjectRuntime *Story_unk92_3(u32);

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
 * Callback for resource_380 that forwards to Story_unk93_3 and nothing else.
 * A script record holds its Thumb pointer; nothing calls it directly. The
 * twelve-byte owner includes the zero alignment halfword after the return and
 * has no literal pool. Story_unk93_3 takes no arguments -- nothing is loaded
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

void Scene_RunMultiStageEffectPresentation(void)
{
    u32 i;
    s32 obj;
    s32 pos;
    s32 mes;

    Story_unk95_3();
    Story_unk96_3(141);
    for (i = 0; i != 6; i++) {
        Story_Run(0x404a4e, 1);
        Story_unk97_3(8);
        Story_unk98_3(8);
        Story_unk2_4(0x10000, 1);
        Story_unk99_3(8);
        Story_unk100_3(8);
        if (i == 1) {
            Story_Place(0x10000, 0x10000, 0x10000);
        }
    }
    Story_unk2_3(0x10000, 0x20000, 0x10000);
    Story_unk101_3(30);
    Story_unk102_3(0, 0, 0);
    Story_unk3_4(0x59999, 0xb333);
    Story_unk4_4(0x1d80000, -1, 0x620000, 1);
    Story_unk103_3();
    Story_unk3_3(0x10000, 0x20000, 0x10000);
    Story_unk104_3(20);
    Story_unk105_3(144);
    Story_unk106_3(0x200d088, 84, 4);
    Story_SetRect(0, 0, 1, 1, 29, 4);
    Story_unk2_5(87, 42, 29, 6, 1, 2);
    Story_unk107_3(40);
    Story_unk108_3(0, 0, 0);
    Story_unk5_4(0x1570000, -1, 0x1710000, 1);
    Story_unk109_3();
    Story_unk4_3(0x10000, 0x20000, 0x10000);
    Story_unk110_3(20);
    Story_unk111_3(144);
    pos = 21;
    Story_unk112_3(0x200d088, 76, 21);
    Story_unk3_5(0, 0, 1, 1, pos, pos);
    Story_unk4_5(87, 42, 21, 23, 1, 2);
    Story_unk113_3(40);
    Story_unk114_3(0, 0, 0);
    Story_unk6_4(0x33333, 0x6666);
    Story_unk7_4(0x1570000, -1, 0x1f10000, 1);
    Story_unk115_3();
    Story_unk5_3(0x10000, 0x20000, 0x10000);
    Story_unk116_3(20);
    Story_unk117_3(144);
    Story_unk118_3(0x200d088, 76, 29);
    Story_unk5_5(0, 0, 1, 1, pos, 29);
    Story_unk6_5(87, 42, 21, 31, 1, 2);
    Story_unk119_3(40);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Story_unk120_3();
    Story_unk121_3();
    Story_unk8_4(0x2c80000, -1, 0x980000, 0);
    Story_unk122_3();
    Story_unk123_3(1);
    Story_unk6_3(0x20000, 0x10000, 0x10000);
    Story_unk124_3();
    Story_unk125_3();
    Story_unk126_3(40);
    Story_Do(0x121);
    Story_unk7_3(-1, -1, 0xe666);
    Story_unk127_3();
    Story_unk128_3(20);
    Story_unk7_5(0, 40, 43, 46, 3, 3);
    Story_unk129_3(20);
    obj = Story_unk29(221, 0x2c80000, 0x100000, 0x900000);
    Story_unk130_3(40);
    Story_unk131_3(obj, 1);
    mes = (s32)gVal;
    Story_Check(mes, 1);
    Story_unk8_3(9, 0xe000, 0);
    Story_unk9_3(5, 0xe000, 20);
    Story_unk132_3();
    Story_unk133_3();
    Story_unk9_4(0x1ce0000, -1, 0x15e0000, 0);
    Story_unk134_3();
    Story_unk135_3(1);
    Story_unk136_3();
    Story_unk137_3();
    Story_unk138_3(40);
    Story_unk139_3(9, 4, 30);
    Story_unk140_3(mes - 2);
    Story_unk141_3(9, 20);
    Story_unk142_3();
    Story_unk143_3();
    Story_unk144_3(0x2c80000, -1, 0x980000, 0);
    Story_unk145_3();
    Story_unk146_3(1);
    Story_unk147_3();
    Story_unk148_3();
    Story_unk2_2(0x83d);
    Story_unk149_3();
}

void Scene_RunSevenSceneChain(void)
{
    Story_unk150_3();
    Story_unk151_3();
    Story_unk152_3();
    Story_unk153_3();
    Story_unk154_3();
    Story_unk155_3();
    Story_unk156_3();
    Story_unk20_2(0x83E);
    Story_unk157_3();
    Story_unk158_3();
}

void Scene_RunPartyIntroduction(void)
{
    u32 i;
    s32 obj;

    Story_unk160_3(141);
    for (i = 0; i != 6; i++) {
        Story_unk10_4(0x4049d2, 1);
        Story_unk161_3(8);
        Story_unk162_3(8);
        Story_unk11_4(0x10000, 1);
        Story_unk163_3(8);
        Story_unk164_3(8);
        if (i == 1) {
            Story_unk10_3(0x10000, 0x10000, 0x10000);
        }
    }
    Story_unk11_3(0x20000, 0x10000, 0x10000);
    Story_unk165_3(30);
    Story_unk12_4(0x26666, 0x4ccc);
    Story_unk13_4(0xa70000, -1, 0x2110000, 1);
    Story_unk166_3();
    Story_unk12_3(0x10000, 0x20000, 0x10000);
    Story_unk167_3(20);
    Story_unk168_3(144);
    Story_unk169_3(0x200d088, 65, 31);
    Story_unk8_5(0, 0, 1, 1, 10, 31);
    Story_unk9_5(87, 42, 10, 33, 1, 2);
    Story_unk170_3(40);
    Story_unk171_3(0, 0, 0);
    Story_unk14_4(0x66666, 0xcccc);
    Story_unk15_4(0x1870000, -1, 0xb10000, 1);
    Story_unk172_3();
    Story_unk13_3(0x10000, 0x20000, 0x10000);
    Story_unk173_3(20);
    Story_unk174_3(144);
    Story_unk175_3(0x200d088, 79, 9);
    Story_unk10_5(0, 0, 1, 1, 24, 9);
    Story_unk11_5(87, 42, 24, 11, 1, 2);
    Story_unk176_3(40);
    Story_unk177_3(0, 0, 0);
    Story_unk16_4(0x26666, 0x4ccc);
    Story_unk17_4(0x2470000, -1, 0xc10000, 1);
    Story_unk178_3();
    Story_unk14_3(0x10000, 0x20000, 0x10000);
    Story_unk179_3(20);
    Story_unk180_3(144);
    Story_unk181_3(0x200d088, 91, 10);
    Story_unk12_5(0, 0, 1, 1, 36, 10);
    Story_unk13_5(87, 42, 36, 12, 1, 2);
    Story_unk182_3(40);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Story_unk183_3();
    Story_unk184_3();
    Story_unk18_4(0xe80000, -1, 0x1dd0000, 0);
    Story_unk185_3();
    Story_unk186_3(1);
    Story_unk15_3(0x20000, 0x10000, 0x10000);
    Story_unk187_3();
    Story_unk188_3();
    Story_unk189_3(40);
    Story_unk3_2(0x121);
    Story_unk16_3(-1, -1, 0xe666);
    Story_unk190_3();
    Story_unk191_2(20);
    Story_unk14_5(0, 40, 13, 66, 3, 3);
    Story_unk192_2(20);
    obj = Story_unk2(223, 0xe80000, 0x100000, 0x1d00000);
    Story_unk193_2(40);
    Story_unk194_2(obj, 1);
    Story_unk19_4(0x1077, 1);
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

    Story_unk17_3(5, 0x6000, 0);
    Story_unk18_3(9, 0x5000, 0);
    Story_unk19_3(10, 0x8000, 0);
    Story_unk20_3(12, 0x5000, 40);
    Story_unk21_3(11, 0xd000, 40);
    Story_unk195_2(11, 2);
    Story_unk196_2(11, 20);
    Story_unk197_2(12, 2);
    Story_unk20_4(12, 0x102);
    Story_unk198_2(60);
    Story_unk199_2(12, 10);
    Story_unk200_2(10, 12, 0);
    Story_unk201_2(5, 12, 0);
    Story_unk202_2(9, 12, 0);
    Story_unk203_2(40);
    Story_unk204_2(10, 1);
    Story_unk22_3(10, 0x8000, 10);
    Story_unk205_2(10, 3);
    Story_unk206_2(10, 10);
    Story_unk207_2(11, 0, 10);
    Story_unk208_2(11, 3);
    Story_unk209_2(10);
    Story_unk210_2(5, 0, 0);
    Story_unk23_3(9, 0x8000, 20);
    Story_unk211_2(5, 2);
    Story_unk212_2(9, 2);
    Story_unk213_2(10);
    Story_unk24_3(5, 0xc000, 0);
    Story_unk25_3(9, 0xb000, 10);
    Story_unk26_3(10, 0xb000, 40);
    Story_unk214_2(10, 3);
    Story_unk215_2(10, 10);
    Story_unk27_3(5, 0x100, 20);
    Story_unk21_4(12, 0x102);
    Story_unk216_2(12, 3);
    Story_unk217_2(40);
    Story_unk28_3(1, 0x15a0000, 0x1b80000);
    Story_unk218_2(1);
    Story_unk219_2(1, 0);
    Story_unk29_3(0, 0x2000, 0);
    Story_unk30_3(1, 0xa000, 20);
    Story_unk220_2();
    Story_unk221_2();
    Story_unk31_3(1, 0x1180000, 0x1d60000);
    Story_unk22_4(0x1050000, -1, 0x1d20000, 0);
    Story_unk222_2();
    Story_unk223_2(1);
    Story_unk224_2();
    Story_unk225_2();
    Story_unk226_2(20);
    Story_unk32_3(0, 0x3000, 10);
    Story_unk33_3(1, 0x102, 0);
    Story_unk227_2(1, 3);
    Story_unk228_2(1, 0);
    Story_unk229_2(10);
    Story_unk230_2(1, 4);
    Story_unk231_2(10);
    Story_unk232_2(1, 0);
    Story_unk233_2(10);
    Story_unk4_2(0x109b);
    Story_unk234_2(11, 0);
    Story_unk34_3(11, 0xd000, 10);
    Story_unk235_2(0, 2);
    Story_unk236_2(0, 2);
    Story_unk237_2(10);
    Story_unk35_3(0, 0xe000, 0);
    Story_unk36_3(1, 0xe000, 20);
    Story_unk238_2();
    Story_unk239_2();
    Story_unk23_4(0x1dd0000, -1, 0x14e0000, 0);
    Story_unk240_2();
    Story_unk241_2(1);
    Story_unk242_2();
    Story_unk243_2();
    Story_unk244_2(20);
    Story_unk245_2(9, 2);
    Story_unk246_2(10);
    Story_unk37_3(9, 0x8000, 30);
    Story_unk247_2(9, 20);
    Story_unk248_2(5, 0, 40);
    Story_unk249_2(5, 4);
    Story_unk250_2(20);
    Story_unk251_2(5, 10);
    Story_unk252_2(12, 2);
    Story_unk253_2(80);
    Story_unk254_2(12, 20);
    Story_unk255_2(5, 2);
    Story_unk38_3(5, 0xc000, 0);
    Story_unk39_3(9, 0xb000, 20);
    Story_unk256_2(12, 3);
    Story_unk257_2(12, 20);
    Story_unk258_2(5, 2);
    Story_unk259_2(9, 2);
    Story_unk40_3(12, 0xb000, 40);
}

void Scene_RunCountedActorSequence(void)
{
    u8 *obj;
    s32 other;
    s32 tbl;
    s32 left;
    s32 cnt;
    s32 mes_a;
    s32 mes_b;

    Story_unk261_2(161);
    Story_unk262_2(12, 3);
    Story_unk263_2(40);
    other = Story_unk3(12);
    if (other != 0) {
        Story_unk264_2(13, *(volatile s32 *)(other + 8), *(volatile s32 *)(other + 16));
    }
    Story_unk265_2(12, 0, 0);
    Story_unk266_2(20);
    Story_unk41_3(13, 0x3000, 40);
    Story_unk267_2(5, 3);
    Story_unk268_2(5, 3);
    Story_unk269_2(40);
    Story_unk270_2(5, 2);
    Story_unk271_2(5, 20);
    Story_unk272_2(13, 3);
    Story_unk273_2(10);
    Story_unk274_2(5, 3);
    Story_unk42_3(9, 0x8000, 10);
    Story_unk275_2(9, 2);
    Story_unk276_2(9, 40);
    Story_unk277_2(5, 3);
    Story_unk278_2(40);
    Story_unk43_3(9, 0xb000, 40);
    Story_unk279_2(13, 2);
    Story_unk280_2(13, 20);
    Story_unk281_2(5, 1);
    Story_unk282_2(40);
    Story_unk283_2(13, 3);
    Story_unk284_2(10);
    Story_unk285_2(13, 40);
    Story_unk286_2(10, 1);
    Story_unk287_2(10, 3);
    Story_unk288_2(10);
    Story_unk289_2(10, 10);
    Story_unk290_2(11, 1);
    Story_unk291_2(11, 3);
    Story_unk292_2(11, 80);
    Story_unk293_2(13, 2);
    Story_unk294_2(13, 40);
    Story_unk295_2(5, 2);
    Story_unk296_2(10);
    Story_unk297_2(5, 10);
    Story_unk298_2(13, 2);
    Story_unk299_2(9, 2);
    Story_unk300_2(80);
    Story_unk301_2(5, 4);
    Story_unk302_2(20);
    Story_unk303_2(5, 80);
    Story_unk304_2(13, 4);
    Story_unk305_2(13, 80);
    Story_unk306_2(5, 2);
    Story_unk307(4);
    Story_unk308(5, 20);
    Story_unk309(10, 1);
    Story_unk310(10, 3);
    Story_unk311(10, 10);
    Story_unk312(11, 1);
    Story_unk313(11, 10);
    Story_unk314(10, 1);
    Story_unk315(10, 10);
    Story_unk44_3(9, 0x3000, 80);
    Story_unk45_3(9, 0x105, 80);
    Story_unk316(11, 1);
    Story_unk46_3(11, 0x5000, 40);
    Story_unk317(11, 2);
    Story_unk318(11, 20);
    Story_unk319();
    Story_unk320();
    Story_unk24_4(0x1050000, -1, 0x1d20000, 0);
    Story_unk321();
    Story_unk322(1);
    Story_unk323();
    Story_unk324();
    Story_unk325(20);
    Story_unk47_3(0, 0x3000, 0);
    Story_unk48_3(1, 0xb000, 20);
    Story_unk326(0, 2);
    Story_unk327(1, 2);
    Story_unk49_3(0, 0x8000, 0x4000);
    Story_unk50_3(1, 0x8000, 0x4000);
    Story_unk51_3(0, 244, 0x1de);
    Story_unk52_2(1, 0x104, 0x1ea);
    Story_unk328(0);
    Story_unk329(0, 1);
    Story_unk53_2(0, 0x3000, 0);
    Story_unk54_2(1, 0xb000, 20);
    Story_unk330(0, 2);
    Story_unk331(1, 4);
    Story_unk332(20);
    tbl = Story_unk30(1);
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
    Story_unk4(1, 0);
    if (Story_unk5(0, 0) == 0) {
        mes_a = (s32)gVal2;
        ((void (*)())Story_unk31)(mes_a);
        Story_unk333(1, 3);
        Story_unk334(10);
        if (cnt <= 2) {
            Story_unk335(1, 30);
            Story_unk55_2(1, 252, 0x1e6);
            Story_unk336(1, 2);
            Story_unk337(10);
            Story_unk338((mes_a + 1), 1, 0);
            goto L_02001bdc;
        }
        Story_unk5_2(0x10b4);
        Story_unk339(1, 30);
    } else {
        if (cnt <= 2) {
            mes_b = (s32)gVal3;
            Story_unk340(mes_b);
            Story_unk341(1, 3);
            Story_unk342(1, 4);
            Story_unk343(1, 10);
            Story_unk344(1, 4);
            Story_unk345(1, 1);
            Story_unk56_2(1, 0x20000, 0x10000);
            obj = Story_unk6(0);
            obj[90] &= 254;
            Story_unk57_2(1, 244, 0x1de);
            Story_unk58_2(0, 0x18000, 0xc000);
            Story_unk346(0, 6, 0);
            Story_unk59_2(0, 218, 0x1d7);
            Story_unk347(0);
            Story_unk348((mes_b + 1), 1, 0);
            Story_unk349(0, 2);
            Story_unk350(0, 0, 30);
            {
                u8 value = *(volatile u8 *)&obj[90];

                obj[90] = (u8)(value | 1);
            }
        } else {
            Story_unk6_2(0x10b5);
            Story_unk351(1, 3);
            Story_unk352(1, 4);
            Story_unk353(1, 10);
            Story_unk354(1, 4);
            Story_unk60_2(0, 0xe000, 30);
        }
    }
    L_02001bdc:;
    Story_unk25_4(0x8000, 0x1000);
    Story_unk355(1, 1);
    Story_unk356();
    Story_unk61_2(1, 0x8000, 30);
    Story_unk357(1, 2);
    Story_unk62_2(1, 0x8000, 0x4000);
    obj = Story_unk7(1);
    obj[90] &= 254;
    Story_unk63_2(1, 0x108, 0x1e2);
    {
        u8 value = *(volatile u8 *)&obj[90];

        obj[90] = (u8)(value | 1);
    }
    Story_unk64_2(1, 0x116, 0x1e0);
    *(volatile s32 *)((s32)obj + 48) = 0x30000;
    *(volatile s32 *)((s32)obj + 52) = 0x20000;
    Story_unk358(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Story_unk359(1, 7);
    Story_unk65_2(1, 0x138, 0x1d6);
    Story_unk360(1, 1);
    Story_unk361(30);
    Story_unk362(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Story_unk363(1, 7);
    Story_unk66_2(1, 0x156, 0x1d6);
    Story_unk364(1, 1);
    Story_unk365(30);
    Story_unk366(153);
    *(volatile s32 *)((s32)obj + 40) = 0x60000;
    Story_unk367(1, 7);
    Story_unk67_2(1, 0x178, 0x1d6);
    Story_unk368(1, 1);
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

    Story_unk369(1, 3);
    Story_unk370(10);
    Story_unk68_2(1, 0x3000, 0);
    record = Story_unk32(14);
    Story_unk371(record, 0);
    Story_unk372(14, 15);
    Story_unk69_2(14, 0x1880000, 0x1c60000);
    Story_unk373();
    Story_unk70_2(1, 0xd000, 10);
    Story_unk374(1, 2);
    Story_unk71_2(1, 0x100, 40);
    Story_unk72_2(14, 0x5000, 10);
    Story_unk375(14, 2);
    Story_unk376(20);
    base6_10b6 = (s32)gVal4;
    Story_unk377(base6_10b6);
    Story_unk378(14, 0);
    Story_unk73_2(10, 0x1d50000, 0x15c0000);
    base5_200a = 0x200a;
    Story_unk379(20);
    Story_unk380(base5_200a, 10);
    Story_unk381(base5_200a, 40);
    Story_unk74_2(10, 0x1fb0000, 0x15c0000);
    Story_unk382(1, 2);
    Story_unk383(40);
    Story_unk384(1, 3);
    Story_unk385(40);
    Story_unk75_2(1, 0x8000, 0x4000);
    Story_unk76_2(1, 0x185, 0x1d4);
    Story_unk77_2(1, 0xd000, 60);
    Story_unk386(1, 20);
    Story_unk387((base6_10b6 + 4), 1, 10);
    Story_unk78_2(1, 0x8000, 0x4000);
    rec = Story_unk8(1);
    rec[90] &= 254;
    none = 0;
    Story_unk79_2(1, 0x178, 0x1d6);
    Story_unk388(30);
    {
        u8 value = *(volatile u8 *)&rec[90];

        rec[90] = (u8)(value | 1);
    }
    Story_unk389(14, 4);
    Story_unk390(10);
    Story_unk391((base6_10b6 + 5));
    Story_unk392(14, 20);
    Story_unk80_2(1, 0x101, 60);
    Story_unk393(14, 3);
    Story_unk394(14, 20);
    Story_unk81_2(1, 0x102, 60);
    Story_unk395(14, 3);
    Story_unk396(20);
    Story_unk82_2(14, 0xc000, 20);
    Story_unk26_4(14, 0x100);
    record = Story_unk33(14);
    Story_unk397(record, 0);
    rec = Story_unk9(14);
    rec[85] = none;
    v5 = 0;
    Story_unk398(220);
    for (i = 0; i != 30; i++) {
        *(volatile s32 *)((s32)rec + 12) += 0x10000;
        Story_unk399(1);
        v5 = i;
    }
    rec[85] = 5;
    Story_unk400(1, 2);
    Story_unk401(1, 10);
    Story_unk83_2(14, 0x101, 60);
    Story_unk84_2(14, 0x5000, 10);
    Story_unk402(1, 20);
    Story_unk403(14, 1);
    Story_unk404(14, 20);
    Story_unk85_2(1, 0x103, 20);
    Story_unk405(1, 30);
    Story_unk86_2(14, 0x105, 80);
    Story_unk87_2(14, 0xd000, 40);
    Story_unk88_2(10, 0x5000, 10);
    Story_unk406();
    Story_unk407();
    Story_unk27_4(0x1dd0000, -1, 0x14e0000, 0);
    Story_unk408();
    Story_unk409(1);
    Story_unk410();
    Story_unk411();
    Story_unk412(20);
    Story_unk413(10, 4);
    Story_unk414(10, 10);
    Story_unk10(11, 0);
    Story_unk28_4(0x66666, 0xcccc);
    Story_unk29_4(0x1760000, -1, 0x1d60000, 1);
    Story_unk415();
    Story_unk89_2(14, 0x5000, 0);
    Story_unk90_2(1, 0xe000, 10);
    Story_unk416(1, 2);
    if (Story_unk11(1, 0) != 0) {
        s32 code;

        Story_unk417(10);
        Story_unk12(14, 4);
        do {
            code = 0x10c3;
        } while (0);
        for (;;) {
            Story_unk7_2(code);
            Story_unk13(14, 0);
            if (Story_unk14(1, 0) != 0)
                break;
            Story_unk418(20);
            Story_unk15(14, 4);
            Story_unk419(10);
            code = 0x10c6;
        }
    }
    Story_unk420(30);
    Story_unk421(14, 3);
    Story_unk422(20);
    Story_unk8_2(0x10c4);
    Story_unk423(14, 30);
    Story_unk424(14, 3);
    Story_unk425(10);
    Story_unk426(14, 30);
    rec[85] = 0;
    Story_unk91_2(14, 0x26666, 0x13333);
    Story_unk30_4((s32)rec, 0x1cc0000, 0, 0x1680000);
    Story_unk427(14);
    Story_unk428(14, 0);
    record = Story_unk34(14);
    Story_unk429(record, 1);
    Story_unk430(30);
    Story_unk431(1, 1);
    Story_unk432();
    Story_unk433(40);
    Story_unk92_2(1, 0x103, 40);
    Story_unk434(1, 3);
    Story_unk435(20);
    rec = Story_unk16(1);
    {
        u8 value = *(volatile u8 *)&rec[90];

        rec[90] = (u8)(value | 1);
    }
    *(volatile s32 *)((s32)rec + 48) = 0x30000;
    *(volatile s32 *)((s32)rec + 52) = 0x20000;
    Story_unk436(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Story_unk437(1, 7);
    Story_unk93_2(1, 0x156, 0x1d6);
    Story_unk438(1, 1);
    Story_unk439(30);
    Story_unk440(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Story_unk441(1, 7);
    Story_unk94_2(1, 0x138, 0x1d6);
    Story_unk442(1, 1);
    Story_unk443(30);
    Story_unk444(153);
    *(volatile s32 *)((s32)rec + 40) = 0x60000;
    Story_unk445(1, 7);
    Story_unk95_2(1, 0x116, 0x1e0);
    Story_unk446(1, 1);
    Story_unk447(30);
    Story_unk31_4(0x8000, 0x1000);
    Story_unk448(0, 1);
    Story_unk96_2(1, 0x19999, 0xcccc);
    Story_unk449(0, 1, 0);
    Story_unk450(30);
    Story_unk451(1, 3);
    Story_unk452(0, 4);
    Story_unk453(1, 2);
    Story_unk454(0, 3);
    Story_unk455(20);
    Story_unk456(1, 2);
    {
        s32 slot = Story_unk17(0);

        if (slot != 0) {
            Story_unk457(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
        }
    }
    Story_unk458(1);
    Story_unk459(1, 0, 0);
    Story_unk460(220);
    Story_unk461(221);
    Story_unk462(223);
}

void Scene_RunSixPassEffectSequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    Story_unk463();
    Story_unk464(141);
    v5 = 0;
    do {
        Story_unk32_4(0x4039d2, 1);
        Story_unk465(8);
        Story_unk466(8);
        Story_unk33_4(0x10000, 1);
        Story_unk467(8);
        Story_unk468(8);
        if (v5 == 1) {
            Story_unk97_2(0x10000, 0x10000, 0x10000);
        }
        v5 = ((u32)((v5 + 1) << 24) >> 24);
    } while (v5 != 6);
    Story_unk9_2(0x121);
    Story_unk98_2(-1, -1, 0xe666);
    Story_unk15_5(0, 40, 13, 46, 3, 3);
    Story_unk469(20);
    rec7 = Story_unk18(222, 0xe80000, 0x100000, 0x900000);
    Story_unk470(40);
    Story_unk471(rec7, 1);
    Story_unk34_4(0x1078, 1);
    Story_unk99_2(5, 0x1330000, 0x1150000);
    Story_unk100_2(9, 0x1330000, 0x1150000);
    Story_unk101_2(11, 0x1330000, 0x1150000);
    Story_unk102_2(10, 0x1330000, 0x1150000);
    Story_unk103_2(14, 0x1330000, 0x1150000);
    Story_unk104_2(0, 0x13333, 0x9999);
    Story_unk472(0, 232, 156);
    Story_unk473(10);
    record = Story_unk19(0);
    if (record != 0) {
        Story_unk474(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Story_unk105_2(1, 0x13333, 0x9999);
    Story_unk475(1, 218, 172);
    Story_unk476(1, 0, 0);
    Story_unk477(20);
    Story_unk478(145);
    Story_unk106_2(0x40000, 0x40000, 0x10000);
    Story_unk479(20);
    Story_unk107_2(0x10000, 0x10000, 0x10000);
    Story_unk480(40);
    Story_unk108_2(0, 0xd000, 0);
    Story_unk109_2(1, 0x5000, 50);
    Story_unk481(144);
    Story_unk110_2(0x30000, 0x30000, 0x10000);
    Story_unk111_2(0, 0x8000, 0);
    Story_unk482(1, 0, 50);
    Story_unk112_2(0x10000, 0x10000, 0x10000);
    Story_unk483(0, 0, 0);
    Story_unk113_2(1, 0x8000, 50);
    Story_unk114_2(0, 0xb000, 0);
    Story_unk115_2(1, 0xd000, 0);
    Story_unk484(144);
    Story_unk116_2(0x30000, 0x30000, 0x10000);
    Story_unk485(30);
    Story_unk486(0, 2, 0);
    Story_unk487(1, 2, 20);
    Story_unk488(0, 6, 0);
    Story_unk489(1, 6, 40);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_unk35_4();
    Story_unk490();
    Story_unk491(2);
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
    Story_unk492();
    if (Story_unk35(0x83e)) {
        Story_unk21_2(0x10cb);
        Story_Apply(9, 0);
    } else {
        if (Story_unk36(0x83c) == 0)
            Story_unk22_2(0x1079);
        else
            Story_unk23_2(0x107b);
        Story_unk305(9, 0, 0);
        Story_unk24_2(10);
        Story_Apply2(9, 0);
    }
    Story_unk493();
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
    Story_unk494();
    if (Story_unk37(0x83e)) {
        Story_unk25_2(0x10c9);
        Story_Apply3(5, 0);
    } else {
        if (Story_unk38(0x83c) == 0)
            Story_unk26_2(0x107a);
        else
            Story_unk27_2(0x107c);
        Story_unk306(5, 0, 0);
        Story_unk28_2(10);
        Story_Apply4(5, 0);
    }
    Story_unk495();
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

    Story_unk496();
    Story_unk29_2(0x10ca);
    Story_Apply5(10, 0);
    Story_unk497();
}

/*
 * Talk handler for actor 11, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void Dialogue_RunActor11TalkLine(void)
{

    Story_unk498();
    Story_unk30_2(0x10c7);
    Story_Apply6(11, 0);
    Story_unk499();
}

/*
 * Talk handler for actor 13, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void Dialogue_ShowLine10C8(void)
{

    Story_unk500();
    Story_unk31_2(0x10c8);
    Story_Apply7(13, 0);
    Story_unk501();
}

/*
 * Talk handler for actor 14, published into a script record rather than
 * called. The thirty-two-byte owner includes its one pool word, which carries
 * the line id, and the zero alignment halfword after it.
 */
void Dialogue_RunActor14TalkLine(void)
{

    Story_unk502();
    Story_unk32_2(0x10cc);
    Story_Apply8(14, 0);
    Story_unk503();
}

/*
 * Same bracket and line call as the talk handlers, but the tail pairs actors
 * 9 and 10 through the overlay's own call.
 */
void Dialogue_RunLine1072WithPair9And10(void)
{
    Story_unk504();
    Story_unk33_2(0x1072);
    Story_Apply9(9, 10);
    Story_unk505();
}

void Scene_Forward(void)
{
    Story_unk93_3();
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

    Story_unk506();
    Story_unk16_5(27, 16, 5, 1, 27, 17);
    Story_unk36_4(0x1d70000, -1, 0x1050000, 0);
    Story_unk507();
    Story_unk508();
    rec2 = (u8 *)Story_unk83_3(8);
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    rec = (u8 *)Story_unk84_3(0);
    p9 = *(u8 **)(rec + 80) + 38;
    *p9 = 0;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Story_unk37_4(0, 0x100);
    Story_unk117_2(0, 0x1d70000, 0x1220000);
    p10 = rec + 85;
    *p10 = 0;
    *(s32 *)(rec + 12) = 0x280000;
    work = *(u8 **)gWork;
    *(s32 *)(((s32)work + 0x1c0)) = 0x203;
    *(s32 *)(((s32)work + 0x1c8)) = 32;
    Story_unk509();
    Story_unk510();
    Story_unk511(20);
    Story_unk118_2(8, 0x1d70000, 0x1220000);
    Story_unk512(190);
    Story_unk513(0, 2);
    for (i = 0; i != 90; i++) {
        *(s32 *)(rec + 12) += -0x1999;
        *(s32 *)(rec + 24) += 0x28f;
        *(s32 *)(rec + 28) += 0x28f;
        *(s32 *)(rec2 + 24) += 0x28f;
        *(s32 *)(rec2 + 28) += 0x28f;
        Story_unk514(1);
    }
    *p10 = 5;
    Story_unk515(80);
    Story_unk38_4(0x4ccc, 0x999);
    Story_unk39_4(0x1d70000, -1, 0x1220000, 1);
    for (i = 0; i != 60; i++) {
        *(s32 *)(rec + 12) += -0x8000;
        Story_unk516(1);
    }
    *p10 = 3;
    Story_unk517(20);
    Story_unk518(0, 1);
    {
        u8 *record = Story_unk85_3(0);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Story_unk519(0, 0);
    {
        s32 shown = 1;

        *p9 = shown;
    }
    ((void (*)())Story_unk39)(8, 0, 0);
    Story_unk520();
    Story_unk521(20);
    Story_unk522(0, 1);
    Story_unk119_2(0, 0x8000, 0x4000);
    Story_unk120_2(0, 0x1d7, 0x136);
    Story_unk523(20);
    Story_unk524(0, 2);
    Story_unk525(20);
    zero = 0;
    record = (u8 *)Story_unk86_3(0);
    if ((s32)record != 0) {
        Story_unk526(5, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    record = (u8 *)Story_unk87_3(0);
    if ((s32)record != 0) {
        Story_unk527(1, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    Story_unk121_2(1, 0x8000, 0x4000);
    Story_unk122_2(5, 0x8000, 0x4000);
    Story_unk123_2(5, 0x1c5, 0x12e);
    Story_unk124_2(1, 0x1e9, 0x12e);
    Story_unk528(5, 1);
    Story_unk125_2(1, 0x3000, 0);
    Story_unk126_2(5, 0x6000, 10);
    Story_unk127_2(0, 0x100, 0);
    Story_unk128_2(5, 0x100, 0);
    Story_unk129_2(1, 0x100, 0);
    Story_unk529(0, 2, 0);
    Story_unk530(5, 2, 0);
    Story_unk531(1, 2, 40);
    Story_unk532(0, 3);
    Story_unk533(5, 3);
    Story_unk534(1, 3);
    Story_unk535(40);
    Story_unk40_4(0, 0x102);
    Story_unk41_4(5, 0x102);
    Story_unk42_4(1, 0x102);
    Story_unk536(60);
    Story_unk130_2(0, 0x13333, 0x9999);
    Story_unk131_2(5, 0x13333, 0x9999);
    Story_unk132_2(1, 0x13333, 0x9999);
    Story_unk133_2(0, 0x1d7, 0x15a);
    Story_unk134_2(5, 0x1af, 0x152);
    Story_unk135_2(1, 0x1ff, 0x152);
    Story_unk537(0);
    Story_unk538(0, 1);
    Story_unk539(5);
    Story_unk540(5, 1);
    Story_unk541(1);
    Story_unk542(1, 1);
    Story_unk136_2(0, 0x4ccc, 0x2666);
    Story_unk137_2(5, 0x4ccc, 0x2666);
    Story_unk138_2(1, 0x4ccc, 0x2666);
    Story_unk139_2(5, 0xc000, 0);
    Story_unk140_2(1, 0xc000, 0);
    Story_unk141_2(0, 0xc000, 60);
    Story_unk142_2(5, 0x8000, 0);
    Story_unk543(1, 0, 0);
    Story_unk143_2(0, 0x4000, 60);
    Story_unk544(5, 0, 0);
    Story_unk144_2(1, 0x8000, 40);
    Story_unk545(0, 0, 40);
    Story_unk546(0, 3);
    Story_unk547(1, 2);
    Story_unk548(20);
    Story_unk549(1, 4);
    Story_unk550(20);
    Story_unk145_2(0, 0x8000, 40);
    Story_unk551(0, 3);
    Story_unk552(5, 2);
    Story_unk553(20);
    Story_unk146_2(5, 0x101, 60);
    Story_unk147_2(0, 0x4000, 0);
    Story_unk148_2(1, 0x3000, 0);
    Story_unk149_2(5, 0x6000, 60);
    rec = (u8 *)Story_unk88_3(9);
    p9b = *(u8 **)(rec + 80) + 38;
    *p9b = zero;
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    *(s32 *)(rec2 + 24) = 0x1999;
    *(s32 *)(rec2 + 28) = 0x1999;
    Story_unk43_4(9, 0x100);
    Story_unk150_2(9, 0x1d70000, 0x1220000);
    p10b = rec + 85;
    *p10b = zero;
    *(s32 *)(rec + 12) = 0x280000;
    Story_unk554(1);
    Story_unk10_2(0x103c);
    Story_unk555(9, 0);
    Story_unk556(0, 4, 0);
    Story_unk557(5, 4, 0);
    Story_unk558(1, 4, 40);
    Story_unk151_2(0, 0xc000, 0);
    Story_unk152_2(5, 0xe000, 0);
    Story_unk153_2(1, 0xb000, 0);
    Story_unk44_4(0x20000, 0x4000);
    Story_unk45_4(0x1d70000, -1, 0x1350000, 1);
    Story_unk559();
    Story_unk154_2(8, 0x1d70000, 0x1220000);
    Story_unk560(190);
    Story_unk561(9, 2);
    for (i = 0; i != 90; i++) {
        *(s32 *)(rec + 12) += -0x1999;
        *(s32 *)(rec + 24) += 0x28f;
        *(s32 *)(rec + 28) += 0x28f;
        *(s32 *)(rec2 + 24) += 0x28f;
        *(s32 *)(rec2 + 28) += 0x28f;
        Story_unk562(1);
    }
    *p10b = 5;
    Story_unk563(80);
    for (i = 0; i != 60; i++) {
        *(s32 *)(rec + 12) += -0x8000;
        Story_unk564(1);
    }
    *p10b = 3;
    Story_unk565(30);
    Story_unk566(9, 1);
    {
        u8 *record = Story_unk90_3(9);
        s32 flags = 1 | record[35];
        record[35] = flags;
    }
    Story_unk567(9, 0);
    {
        s32 shown = 1;

        *p9b = shown;
    }
    Story_unk568(8, 0, 0);
    Story_unk11_2(30);
    Story_unk155_2(9, 0x13333, 0x9999);
    Story_unk156_2(9, 0x1d7, 0x132);
    Story_unk569(20);
    Story_unk157_2(9, 0x100, 0);
    Story_unk570(9, 2, 80);
    Story_unk571(9, 3);
    Story_unk572(30);
    Story_unk158_2(9, 0x8000, 30);
    Story_unk573(9, 2);
    Story_unk574(20);
    Story_unk575(9, 0, 30);
    Story_unk576(9, 2);
    Story_unk577(20);
    Story_unk159_2(9, 0x4000, 30);
    Story_unk578(9, 3);
    Story_unk579(30);
    Story_unk580(9, 20);
    Story_unk160_2(9, 0x26666, 0x13333);
    Story_unk161_2(9, 0x1a7, 0x132);
    Story_unk162_2(9, 0xb000, 0);
    Story_unk163_2(0, 0xb000, 0);
    Story_unk164_2(5, 0xa000, 0);
    Story_unk165_2(1, 0xb000, 30);
    Story_unk581(9, 2);
    Story_unk582(30);
    Story_unk583(9, 3);
    Story_unk584(40);
    Story_unk166_2(9, 0x207, 0x132);
    Story_unk167_2(9, 0xd000, 0);
    Story_unk168_2(0, 0xd000, 0);
    Story_unk169_2(5, 0xe000, 0);
    Story_unk170_2(1, 0xd000, 20);
    Story_unk585(9, 2);
    Story_unk586(30);
    base5_4009 = (s32)gVal6;
    Story_unk587(9, 4);
    Story_unk588(40);
    Story_unk589(base5_4009, 30);
    Story_unk590(0, 5, 0);
    Story_unk591(40);
    Story_unk592(0, 2);
    Story_unk593(5, 2);
    Story_unk171_2(1, 0x101, 40);
    Story_unk594(1, 40);
    Story_unk172_2(9, 0x5000, 20);
    Story_unk595(base5_4009, 20);
    Story_unk173_2(0, 0xd000, 0);
    Story_unk174_2(5, 0xe000, 20);
    Story_unk175_2(0, 0x101, 0);
    Story_unk176_2(5, 0x101, 40);
    Story_unk596(9, 4);
    Story_unk597(base5_4009, 10);
    Story_unk598(1, 3);
    Story_unk599(5, 3);
    Story_unk600(0, 3);
    Story_unk601(20);
    Story_unk602(9, 3);
    Story_unk603(40);
    Story_unk46_4(0x8000, 0x1000);
    Story_unk47_4(0x2150000, -1, 0x1530000, 1);
    Story_unk177_2(9, 0x8000, 0x4000);
    Story_unk178_2(9, 0x215, 0x153);
    Story_unk604();
    Story_unk605(0, 0, 0);
    Story_unk606(5, 0, 0);
    Story_unk607(1, 0, 20);
    Story_unk179_2(9, 0xd000, 40);
    Story_unk608(9, 0, 40);
    Story_unk180_2(9, 0x3000, 60);
    Story_unk609(9, 3);
    Story_unk610(30);
    Story_unk611(9, 1);
    Story_unk181_2(9, 0x19999, 0xcccc);
    Story_unk182_2(9, 0x1c7, 0x168);
    Story_unk612(20);
    Story_unk183_2(0, 0x4000, 0);
    Story_unk184_2(5, 0x2000, 0);
    Story_unk185_2(1, 0x5000, 0);
    Story_unk186_2(9, 0x1c7, 0x168);
    Story_unk613(20);
    Story_unk187_2(9, 0x5000, 30);
    Story_unk614(9, 3);
    Story_unk615(30);
    Story_unk188_2(9, 0x1d7, 0x168);
    Story_unk616(20);
    Story_unk189_2(9, 0x3000, 30);
    Story_unk190_2(9, 0x100, 0);
    Story_unk617(9, 2);
    Story_unk618(30);
    Story_unk619(9, 30);
    Story_unk620(9, 3);
    Story_unk621(20);
    Story_unk622(9, 30);
    Story_unk191(5, 0xcccc, 0x6666);
    Story_unk192(5, 0x1b8, 0x15a);
    Story_unk623(10);
    Story_unk193(5, 0x2000, 10);
    Story_unk624(5, 20);
    Story_unk194(1, 0xcccc, 0x6666);
    Story_unk195(1, 0x1ef, 0x15a);
    Story_unk625();
    Story_unk626(10);
    Story_unk196(1, 0x6000, 10);
    Story_unk20(1, 0);
    Story_unk627(5, 0, 0);
    Story_unk197(1, 0x8000, 0);
    if (Story_unk21(0, 0) == 1) {
        bump_step(1);
    }
    Story_unk198(9, 0xc000, 30);
    Story_unk628(9, 30);
    Story_unk12_2(0x1048);
    Story_unk199(5, 0x2000, 0);
    Story_unk200(1, 0x6000, 30);
    Story_unk629(9, 2);
    Story_unk630(9, 10);
    Story_unk631(9, 3);
    Story_unk632(9, 20);
    Story_unk633(9, 2);
    Story_unk634(9, 10);
    Story_unk48_4(9, 0x102);
    Story_unk635(40);
    Story_unk636(9, 10);
    Story_unk201(9, 0x2000, 20);
    Story_unk637(0, 2);
    Story_unk638(5, 2);
    Story_unk639(1, 2);
    Story_unk640(10);
    Story_unk202(0, 0x2000, 0);
    Story_unk203(5, 0x2000, 0);
    Story_unk204(1, 0x2000, 20);
    Story_unk641(9);
    Story_unk642(0);
    Story_unk643(5);
    Story_unk644(1);
    Story_unk49_4(0x80000, 0x10000);
    Story_unk50_4(0x2c70000, -1, 0x1d00000, 1);
    Story_unk645();
    Story_unk205(9, 0x24d0000, 0x1610000);
    Story_unk646(40);
    Story_unk51_4(0x1009, 0);
    Story_unk206(9, 0x1d70000, 0x1680000);
    Story_unk647(80);
    Story_unk52_3(0x1d70000, -1, 0x1720000, 1);
    Story_unk648();
    Story_unk649(10);
    Story_unk650(9, 3);
    Story_unk651(10);
    Story_unk207(9, 0xd000, 20);
    Story_unk208(0, 0xe000, 0);
    Story_unk209(5, 0xe000, 0);
    Story_unk210(1, 0xe000, 20);
    Story_unk53_3(0x2c70000, -1, 0x930000, 1);
    Story_unk652();
    Story_unk211(9, 0x2540000, 0xee0000);
    Story_unk653(40);
    Story_unk54_3(0x1009, 0);
    Story_unk212(9, 0x1d70000, 0x1680000);
    Story_unk654(80);
    Story_unk55_3(0x1d70000, -1, 0x1720000, 1);
    Story_unk655();
    Story_unk656(10);
    Story_unk657(9, 2);
    Story_unk658(10);
    Story_unk659(9, 4, 0);
    Story_unk213(9, 0xb000, 20);
    Story_unk660(0, 1);
    Story_unk661(5, 1);
    Story_unk662(1, 1);
    Story_unk214(0, 0xa000, 0);
    Story_unk215(5, 0xa000, 0);
    Story_unk216(1, 0xa000, 20);
    Story_unk56_3(0xe70000, -1, 0x930000, 1);
    Story_unk663();
    Story_unk217(9, 0x1340000, 0xfa0000);
    Story_unk664(40);
    Story_unk57_3(0x2009, 0);
    Story_unk218(9, 0x1d70000, 0x1680000);
    Story_unk665(80);
    Story_unk58_3(0x1d70000, -1, 0x1720000, 1);
    Story_unk666();
    Story_unk667(10);
    Story_unk668(9, 3);
    Story_unk669(10);
    Story_unk670(9, 6, 0);
    Story_unk219(9, 0x5000, 20);
    Story_unk671(0, 2);
    Story_unk672(5, 2);
    Story_unk673(1, 2);
    Story_unk220(0, 0x6000, 0);
    Story_unk221(5, 0x6000, 0);
    Story_unk222(1, 0x6000, 20);
    Story_unk59_3(0xe70000, -1, 0x1d00000, 1);
    Story_unk674();
    Story_unk223(9, 0x1320000, 0x16a0000);
    Story_unk675(40);
    Story_unk60_3(0x2009, 0);
    Story_unk224(9, 0x1d70000, 0x1680000);
    Story_unk676(80);
    Story_unk61_3(0x1d70000, -1, 0x1720000, 1);
    Story_unk677();
    Story_unk678(30);
    Story_unk225(9, 0x104, 60);
    Story_unk226(0, 0x4000, 0);
    Story_unk227(5, 0x2000, 0);
    Story_unk228(1, 0x6000, 40);
    Story_unk229(5, 0x102, 40);
    Story_unk679(5, 1);
    Story_unk680(5, 20);
    Story_unk681(9, 4, 0);
    Story_unk230(9, 0xb000, 20);
    Story_unk682(9, 2);
    Story_unk62_3(0xa009, 10);
    Story_unk683(9, 3);
    Story_unk63_3(0xa009, 20);
    Story_unk231(0, 0x101, 0);
    Story_unk232(5, 0x101, 0);
    Story_unk233(1, 0x101, 60);
    Story_unk234(9, 0xc000, 20);
    Story_unk684(9, 4);
    Story_unk22(0x8009, 0);
    if (Story_unk23(0, 0) == 1) {
        bump_step(1);
    }
    Story_unk685(20);
    Story_unk235(1, 0x8000, 10);
    Story_unk686(1, 10);
    Story_unk13_2(0x1056);
    Story_unk236(1, 0x6000, 20);
    Story_unk687(9, 4, 40);
    Story_unk688(0x8009, 10);
    Story_unk237(0, 0x101, 0);
    Story_unk238(5, 0x101, 0);
    Story_unk239(1, 0x101, 80);
    Story_unk64_3(9, 0x102);
    Story_unk689(40);
    Story_unk690(0x8009, 40);
    Story_unk240(0, 0x106, 0);
    Story_unk241(5, 0x106, 0);
    Story_unk242(1, 0x106, 60);
    Story_unk691(5, 0, 0);
    Story_unk243(1, 0x8000, 20);
    Story_unk24(0x8009, 0);
    if (Story_unk25(0, 0) == 1) {
        bump_step(1);
    }
    Story_unk692(9, 1);
    Story_unk693(20);
    Story_unk244(5, 0x2000, 0);
    Story_unk245(1, 0x5000, 20);
    Story_unk694(0x8009, 40);
    Story_unk14_2(0x105b);
    Story_unk695(5, 4);
    Story_unk696(5, 10);
    Story_unk246(9, 0xb000, 10);
    Story_unk697(9, 1);
    Story_unk698(9, 4, 40);
    Story_unk65_3(0xa009, 10);
    Story_unk699(0, 1);
    Story_unk700(5, 1);
    Story_unk701(1, 1);
    Story_unk702(40);
    Story_unk247(9, 0xc000, 10);
    Story_unk703(9, 1);
    Story_unk704(0x8009, 40);
    Story_unk248(0, 0x105, 0);
    Story_unk249(5, 0x105, 0);
    Story_unk250(1, 0x105, 120);
    Story_unk251(1, 0x107, 0);
    Story_unk705(1, 4, 40);
    Story_unk706(1, 10);
    Story_unk707(9, 1);
    Story_unk708(40);
    Story_unk252(9, 0x4000, 80);
    Story_unk709(0x8009, 10);
    Story_unk253(1, 0x8000, 0);
    Story_unk710(0, 0, 30);
    Story_unk711(0, 2);
    Story_unk712(1, 2);
    Story_unk713(20);
    Story_unk714(5, 0, 0);
    Story_unk254(0, 0x8000, 30);
    Story_unk715(0, 2);
    Story_unk716(5, 2);
    Story_unk717(20);
    Story_unk255(0, 0x4000, 0);
    Story_unk256(5, 0x2000, 0);
    Story_unk257(1, 0x5000, 40);
    Story_unk66_3(0, 0x102);
    Story_unk67_3(5, 0x102);
    Story_unk68_3(1, 0x102);
    Story_unk718(80);
    Story_unk719(9, 3);
    Story_unk720(20);
    Story_unk721(0x8009, 40);
    rec = (u8 *)Story_unk92_3(9);
    v6 = 192;
    Story_unk722(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Story_unk723(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Story_unk258(9, 0x1d7, 0x18b);
    Story_unk724(6);
    Story_unk259(9, 0x4ccc, 0x2666);
    p9c = rec + 90;
    *p9c &= 254;
    Story_unk260(9, 0x1d9, 0x18b);
    Story_unk725(9);
    Story_unk726(9, 2);
    Story_unk261(9, 0x1d5, 0x18b);
    Story_unk727(9);
    Story_unk728(9, 2);
    Story_unk262(9, 0x1d7, 0x18b);
    Story_unk729(9);
    Story_unk730(9, 10);
    Story_unk263(9, 0xcccc, 0x6666);
    Story_unk264(9, 0x1d7, 0x19b);
    Story_unk265(9, 0x4ccc, 0x2666);
    *p9c &= 254;
    Story_unk266(9, 0x1da, 0x19b);
    Story_unk731(9);
    Story_unk732(9, 3);
    Story_unk267(9, 0x1d4, 0x19b);
    Story_unk733(9);
    Story_unk734(9, 3);
    Story_unk268(9, 0x1d7, 0x19b);
    Story_unk735(9);
    Story_unk69_3(9, 0x102);
    Story_unk736(9, 3);
    Story_unk737(9, 10);
    Story_unk269(9, 0x3333, 0x1999);
    Story_unk270(9, 0x1d7, 0x18b);
    Story_unk738(9, 1);
    Story_unk739(30);
    {
        s32 flags = 1 | *p9c;
        *p9c = flags;
    }
    Story_unk271(9, 0xc000, 60);
    Story_unk740(6);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Story_unk741(153);
    *(s32 *)(rec + 40) = (v6 << 11);
    Story_unk272(9, 0x1d7, 0x168);
    Story_unk742(6);
    Story_unk743(40);
    Story_unk273(9, 0x102, 80);
    Story_unk274(0, 0x102, 0);
    Story_unk275(5, 0x102, 0);
    Story_unk276(1, 0x102, 80);
    Story_unk744(5, 1);
    Story_unk277(5, 0x107, 40);
    Story_unk745(5, 20);
    Story_unk278(9, 0xb000, 40);
    Story_unk746(9, 4);
    Story_unk70_3(0xa009, 30);
    Story_unk747(5, 0, 30);
    Story_unk279(5, 0x106, 60);
    Story_unk280(5, 0x2000, 30);
    Story_unk748(5, 20);
    Story_unk749(0, 4, 0);
    Story_unk750(1, 4, 0);
    Story_unk281(0, 0x8000, 0);
    Story_unk282(1, 0x8000, 40);
    Story_unk751(0, 1);
    Story_unk752(1, 1);
    Story_unk753(40);
    Story_unk754(0, 0, 30);
    Story_unk755(0, 2);
    Story_unk756(1, 2);
    Story_unk757(30);
    Story_unk283(9, 0xd000, 30);
    Story_unk758(9, 1);
    Story_unk759(20);
    Story_unk760(0x8009, 20);
    Story_unk284(0, 0x4000, 0);
    Story_unk285(1, 0x5000, 30);
    Story_unk761(0, 2);
    Story_unk762(1, 2);
    Story_unk763(20);
    Story_unk764(5, 3);
    Story_unk765(20);
    Story_unk766(5, 0, 30);
    Story_unk26(5, 0);
    Story_unk286(0, 0x8000, 0);
    while (Story_unk27(0, 0) != 0) {
        Story_unk15_2(0x1068);
        Story_unk287(5, 0x107, 0);
        Story_unk767(5, 4, 60);
        Story_unk768(5, 0);
    }
    Story_unk16_2(0x1069);
    Story_unk769(20);
    Story_unk770(5, 3);
    Story_unk771(20);
    Story_unk288(5, 0x2000, 30);
    Story_unk772(5, 10);
    Story_unk289(0, 0x4000, 10);
    Story_unk290(9, 0xb000, 30);
    Story_unk773(9, 3);
    Story_unk774(20);
    Story_unk291(9, 0xc000, 30);
    Story_unk292(9, 0x3333, 0x1999);
    Story_unk293(9, 0x1d7, 0x160);
    Story_unk775(20);
    base5_8009 = (s32)gVal7;
    Story_unk776(9, 2);
    Story_unk777(20);
    Story_unk778(base5_8009, 60);
    *p9c &= 254;
    Story_unk294(9, 0x1c8, 0x168);
    Story_unk779(20);
    Story_unk780(5, 2);
    Story_unk781(20);
    Story_unk17_2(0x106d);
    Story_unk782(5, 30);
    Story_unk295(9, 0xb000, 30);
    Story_unk783(9, 3);
    Story_unk784(20);
    Story_unk296(1, 0x101, 60);
    Story_unk785(1, 10);
    Story_unk297(9, 0xd000, 20);
    Story_unk786(9, 3);
    Story_unk787(base5_8009, 20);
    Story_unk788(0, 0, 0);
    Story_unk298(1, 0x8000, 40);
    Story_unk299(0, 0x101, 0);
    Story_unk300(1, 0x101, 40);
    Story_unk789(9, 2);
    Story_unk790(base5_8009, 10);
    Story_unk301(0, 0x4000, 0);
    Story_unk302(1, 0x5000, 30);
    Story_unk791(9, 3);
    Story_unk792(10);
    Story_unk793(base5_8009, 30);
    Story_unk794(1, 3);
    Story_unk795(0, 3);
    Story_unk796(20);
    Story_unk797(0, 0, 0);
    Story_unk303(1, 0x8000, 30);
    Story_unk798(1, 2);
    Story_unk799(30);
    Story_unk800(0, 3);
    Story_unk801(10);
    Story_unk304(1, 0x10000, 0x8000);
    Story_unk802(1, 2);
    record = Story_unk28(0);
    if ((s32)record != 0) {
        Story_unk803(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Story_unk804(1);
    Story_unk805(1, 0, 0);
    Story_unk18_2(0x83b);
    Story_unk806(5);
    Story_unk807();
    Story_unk17_5(8, 0, 5, 1, 27, 17);
    *(s32 *)((*(u8 **)gWork + 0x1c8)) = 16;
    Story_unk19_2(0x12f);
    Story_unk808();
}

void Scene_RunSplitPairSteps(s32 a, s32 b)
{
    Story_Apply10(a, 0);
    Story_unk34_2(b);
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

    Story_unk71_3(0x53);
    Story_unk72_3(224, 3);
    Story_unk73_3(0x111b, 1);
    do {
        cnt = 30 - Story_unk74_3(0);
        cnt -= Story_unk75_3(1);

        if (cnt <= 3) {
            Story_unk76_3(0x111c, 1);
            if (Story_unk77_3(&second, &first) != -1)
                Story_unk78_3(second, first);
        }
    } while (cnt <= 3);
    Story_unk79_3(224);
    Story_unk80_3(224);
    Story_unk81_3(224);
    Story_unk82_3(224);
    *(s16 *)(work + 472) = saved;
}
