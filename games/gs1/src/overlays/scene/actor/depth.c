#include "types.h"
#include "scene.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct OverlayActorPosition {
    u8 pad00[8];
    s32 depth_fixed;
};

struct OverlayActorState {
    u8 pad00[35];
    u8 flags;
};

extern u8 gWork[];

void *AcquireOverlayObject(s32, s32, s32, s32);

struct OverlayActorPosition *Actor_unk3_4();

struct OverlayActorState *Actor_unk4_4();

u8 *Actor_unk5_4(s32 group);

/* Wait at most sixty frames for the object to reach the requested height. */

/*
 * resource_387: issue a scene request and then wait.
 */

/* Old-style: the two imports are called with different arities. */

/* resource_387 three-call story-flag wrapper at 0x020004d4. */

void Actor_unk6_4();          /* Wait this many frames. */

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
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

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

/*
 * The owner exists for the argument shuffle: the frame count is saved before
 * the first call clobbers its register, so it survives to reach the second.
 * The twenty-two byte owner loads no literal and has no pool.
 */
void Scene_RequestAndWaitFrames(s32 selector, s32 frames)
{
    Actor_unk7_4(selector, 0);
    Actor_unk6_4(frames);
}

/* Contiguous unnamed leaf-owner run for resource_387. */

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable92f8(void)
{
    return (void *)0x020092f8;
}

/* resource_387 zero-return leaf at 0x02000334. */
int SceneData_ReturnZero(void)
{
    return 0;
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9358(void)
{
    return (void *)0x02009358;
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9368(void)
{
    return (void *)0x02009368;
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    s32 record;
    s32 v3;

    record = Actor_Check(9);
    v3 = *(volatile s32 *)(record + 8) / 0x100000;
    Actor_Do(0x861);
    Actor_unk2_2(0x862);
    if (v3 == 15) {
        Actor_SetRect(47, 18, 1, 2, 16, 18);
    } else if (v3 == 16) {
        Actor_unk2_5(48, 18, 1, 2, v3, 18);
        Actor_unk3_2(0x861);
    } else {
        Actor_unk3_5(47, 18, 1, 2, 16, 18);
        Actor_unk4_2(0x862);
    }
}

void Scene_RunScene387SequenceC(void)
{
    s32 record;
    s32 v3;
    s32 v5;

    record = Actor_unk2(10);
    v3 = *(volatile s32 *)(record + 8) / 0x100000;
    if (v3 == 23) {
        Actor_unk8_4(10);
        *(u8 *)(Actor_unk6(10) + 35) = 2;
        v5 = 0;
        *(u8 *)(Actor_unk7(10) + 85) = v5;
        Actor_unk9_4(Actor_unk8(10), 0);
        Actor_unk4_5(54, 17, 1, 1, v3, 17);
        Actor_unk5_2(0x863);
    }
}

void Scene_RunScene387SequenceD(void)
{
    u8 *p5;
    s32 v5;

    p5 = *(u8 **)gWork;
    Actor_unk10_4();
    Actor_unk11_4(0, 8);
    Actor_unk12_4(20);
    Actor_Place(0, 0x3333, 0x1999);
    Actor_unk2_3(9, 0x3333, 0x1999);
    Actor_unk13_4(185);
    v5 = (11 - (*(s16 *)(p5 + 0x16c) << 1)) << 4;
    Actor_unk14_4(0, v5, 0);
    Actor_unk15_4(9, v5, 0);
    Actor_unk16_4(0);
    Actor_unk17_4(9);
    Actor_unk18_4(20);
    Actor_unk19_4(0, 1);
    Actor_unk20_4();
    Actor_unk21_4();
    Actor_unk22_4();
}

/* resource_387 deliberate no-op leaf at 0x020004cc. */
void Resource387_NoOpCallbackA(void)
{
}

/* resource_387 deliberate no-op leaf at 0x020004d0. */
void Resource387_NoOpCallbackB(void)
{
}

void Scene_RunStepWithValue866(void)
{
    Actor_unk23_4();
    Actor_unk8_2(0x866);
    Actor_unk24_4();
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9488(void)
{
    return (void *)0x02009488;
}

void Scene_RunScene387SequenceA(void)
{
    u32 i;
    s32 record;

    Actor_unk25_4();
    Actor_unk26_4();
    Actor_unk27_4(30);
    Actor_unk6_2(0x138f);
    Actor_unk3_3(0, 0xcccc, 0x6666);
    Actor_unk4_3(1, 0xcccc, 0x6666);
    Actor_unk5_3(0, 0xe000, 20);
    record = Actor_unk3(0);
    if (record != 0) {
        Actor_unk28_4(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Actor_unk6_3(1, 0x108, 168);
    Actor_unk7_3(1, 0x6000, 20);
    Actor_unk29_4(20);
    Actor_unk30_4(1, 4);
    Actor_unk31_4(20);
    Actor_unk32_4(1, 0, 20);
    Actor_unk33_4(0, 3);
    Actor_unk34_4(20);
    if (Actor_unk4(0x855) == 0) {
        Actor_unk35_4(1, 2);
        record = Actor_unk5(0);
        if (record != 0) {
            Actor_unk36_4(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_unk37_4(1);
        Actor_unk38_4(1, 0, 0);
        Actor_unk39_4();
    } else {
        Actor_unk8_3(2, 0x1680000, 0xf80000);
        Actor_unk9_3(2, 0xcccc, 0x6666);
        Actor_unk10_3(2, 0x110, 248);
        Actor_unk11_3(2, 0x110, 208);
        Actor_unk12_3(2, 0xa000, 20);
        Actor_unk40_4(2, 2);
        Actor_unk41_4(20);
        Actor_unk42_4(2, 0, 20);
        Actor_unk13_3(0, 0x2000, 0);
        Actor_unk14_3(1, 0x4000, 0);
        Actor_unk43_4(20);
        Actor_unk15_3(0, 0x100, 0);
        Actor_unk16_3(1, 0x100, 60);
        Actor_unk44_4(2, 3);
        Actor_unk17_3(2, 0x108, 200);
        Actor_unk45_4(0, 248, 168);
        Actor_unk46_4(2, 248, 184);
        Actor_unk47_4(0);
        Actor_unk18_3(0, 0x6000, 0);
        Actor_unk19_3(1, 0x6000, 0);
        Actor_unk48_4(2, 232, 184);
        Actor_unk49_4(20);
        Actor_unk20_3(2, 0x105, 60);
        Actor_unk21_3(2, 0xe000, 20);
        Actor_unk50_4(2, 4);
        Actor_unk51_4(20);
        Actor_unk52_4(2, 0, 20);
        Actor_unk53_4(0, 3);
        Actor_unk54_4(1, 3);
        Actor_unk55_4(20);
        Actor_unk22_3(2, 0x8000, 20);
        Actor_unk56_4(2, 0, 120);
        Actor_unk23_3(0, 0x105, 0);
        Actor_unk24_3(1, 0x105, 60);
        Actor_unk57_4(0, 1, 0);
        Actor_unk58_4(60);
        Actor_unk25_3(0, 0x6000, 0);
        Actor_unk26_3(1, 0x6000, 0);
        Actor_unk59_4(60);
        Actor_unk27_3(2, 0x106, 0);
        Actor_unk60_4(2, 1);
        Actor_unk61_4(30);
        Actor_unk62_4(2, 0, 30);
        Actor_unk28_3(2, 0xe000, 20);
        Actor_unk63_4(0, 2);
        Actor_unk64_4(1, 2);
        Actor_unk65_4(20);
        Actor_unk66_4(2, 0, 20);
        Actor_unk67_4(0, 3);
        Actor_unk68_4(1, 3);
        Actor_unk69_4(20);
        Actor_unk70_4(2, 3);
        Actor_unk71_4(20);
        Actor_unk29_3(2, 0x8000, 20);
        Actor_unk72_4();
        Actor_unk73_4(2, 1);
        Actor_unk74_4(20);
        Actor_unk75_4();
        Actor_unk30_3(0, 0x100, 0);
        Actor_unk31_3(1, 0x100, 60);
        Actor_unk76_4(1, 2, 0);
        Actor_unk77_4(20);
        Actor_unk78_4(1, 20);
        Actor_unk32_3(2, 0x108, 184);
        Actor_unk79_4(10);
        Actor_unk80_4(2, 1, 0);
        Actor_unk81_4(1, 2, 0);
        Actor_unk82_4(0, 2, 0);
        Actor_unk83_4(20);
        Actor_unk84_4(2, 3);
        Actor_unk85_4(20);
        Actor_unk86_4(2, 60);
        Actor_unk33_3(0, 0x105, 0);
        Actor_unk34_3(1, 0x105, 60);
        Actor_unk35_3(0, 0x101, 0);
        Actor_unk36_3(1, 0x101, 0);
        Actor_unk87_4(60);
        Actor_unk37_3(1, 0x8000, 0);
        Actor_unk88_4(0, 0, 0);
        Actor_unk89_4(60);
        Actor_unk38_3(1, 0x4000, 0);
        Actor_unk39_3(0, 0x2000, 0);
        Actor_unk90_4(10);
        Actor_unk91_4(1, 20);
        Actor_unk40_3(2, 0x105, 0);
        Actor_unk92_4(60);
        Actor_unk93_4(2, 4);
        Actor_unk94_4(20);
        Actor_unk95_4(2, 20);
        Actor_unk96_4(0, 2, 0);
        Actor_unk97_4(1, 2, 0);
        Actor_unk41_3(0, 0x102, 0);
        Actor_unk42_3(1, 0x102, 0);
        Actor_unk98_4(60);
        Actor_unk99_4(2, 2);
        Actor_unk100_4(20);
        Actor_unk101_4(2, 30);
        Actor_unk43_3(0, 0x101, 0);
        Actor_unk44_3(1, 0x101, 0);
        Actor_unk102_4(80);
        Actor_unk103_4(2, 3);
        Actor_unk104_4(20);
        Actor_unk105_4(2, 20);
        Actor_unk106_4(0, 1);
        Actor_unk107_4(1, 1);
        Actor_Run(0, 0x102);
        Actor_unk2_4(1, 0x102);
        Actor_unk108_4(60);
        Actor_unk109_4(2, 4);
        Actor_unk110_4(20);
        Actor_unk111_4(2, 20);
        Actor_unk112_4(0, 0, 0);
        Actor_unk45_3(1, 0x8000, 0);
        Actor_unk113_4(80);
        Actor_unk114_4(0, 2, 0);
        Actor_unk115_4(1, 2, 0);
        Actor_unk116_4(30);
        Actor_unk117_4(2, 3);
        Actor_unk118_4(20);
        Actor_unk119_4(2, 30);
        Actor_unk120_4(2, 4);
        Actor_unk121_4(20);
        Actor_unk122_4(2, 20);
        Actor_unk123_4(0, 2);
        Actor_unk124_4(1, 2);
        Actor_unk125_4(20);
        Actor_unk126_4(2, 3);
        Actor_unk127_4(20);
        Actor_unk128_4(2, 40);
        Actor_unk129_4(0, 3);
        Actor_unk130_4(1, 3);
        Actor_unk131_3(20);
        Actor_unk132_3(2, 1);
        Actor_unk133_3(60);
        Actor_unk134_3();
        Actor_unk135_3(2, 1);
        Actor_unk136_3(20);
        Actor_unk137_3(2, 248, 184);
        Actor_unk138_3(20);
        Actor_unk139_3(2, 20);
        Actor_unk46_3(0, 0x8000, 0);
        Actor_unk47_3(1, 0x8000, 0);
        Actor_unk140_3(120);
        Actor_unk141_3(2, 30);
        Actor_unk142_3(0, 2, 0);
        Actor_unk143_3(1, 2, 0);
        Actor_unk144_3(2, 0, 0);
        Actor_unk145_3(20);
        Actor_unk146_3(0, 3);
        Actor_unk147_3(1, 3);
        Actor_unk148_3(2, 3);
        Actor_unk149_3(50);
        Actor_unk48_3(1, 0xcccc, 0x6666);
        Actor_unk49_3(2, 0xcccc, 0x6666);
        Actor_unk150_3(1, 248, 168);
        Actor_unk151_3(2, 248, 168);
        Actor_unk152_3(2, 0, 0);
        Actor_unk153_3(1);
        Actor_unk154_3(1, 0, 0);
        Actor_unk5_5(74, 11, 1, 1, 73, 11);
        Actor_unk7_2(0x865);
        Actor_unk155_2();
    }
}

void Overlay387_ConfigureActorEightAtDepth(void)
{
    s32 depth;
    s32 span;
    struct OverlayActorState *state;

    Actor_unk156_2();
    depth = Actor_unk3_4(8)->depth_fixed >> 20;
    if (depth == 11) {
        Actor_unk157_2(8);
        state = Actor_unk4_4(8);
        state->flags |= 2;
        span = 12;
        Actor_unk158_2(39, 12, 3, 1, 8, span);
        Actor_unk159_2(43, 11, 3, 1, span, depth);
        Actor_unk160_2(2144);
    }
    Actor_unk161_2();
}

/* Turn the object's attached presentation state by one eighth-turn. */
void OvObj_TurnStateByEighth(u8 *obj)
{
    u8 *state = *(u8 **)(obj + 80);
    s32 v = *(u16 *)(state + 30) - 0x800;

    *(u16 *)(state + 30) = v;
}

void OvObj_WaitForHeight(u8 *obj, s32 height)
{
    s32 cnt = 60;
    while (cnt != 0) {
        Actor_unk9_2(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= height)
            break;
    }
}

/* Apply a value to every matching member of a fifteen-slot group. */
void Actor_ApplyValueAndMatchingSlots(s32 group, s32 value)
{
    u8 *work = Actor_unk5_4(group);
    s32 i;
    Actor_Apply(group, value);
    for (i = 0; i < 15; i++) {
        if (*(u16 *)(work + 216 + i * 2) == value)
            Actor_Apply2(group, i);
    }
}
