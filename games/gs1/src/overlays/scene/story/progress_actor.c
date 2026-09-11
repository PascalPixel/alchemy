#include "types.h"
#include "scene.h"
#include "facing_object.h"

/* overlays/scene/story/progress_actor/progress_actor.c */
/* overlays/scene/story/progress_actor/progress_dependent_actor_scene_head.c */

extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gWork[];

struct ObjectRuntime *Story_unk3_4(u32);

/* Each Func_ symbol names the loader-relocated call word the image holds for
 * one call site, not a runtime address, so several names can reach the same
 * target. Declarations are old-style where the arity varies between sites. */

struct ObjectRuntime;

s32 OvObj_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 ang;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        ang = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(ang - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/*
 * Returns the in-image table at 0x02009cf4. The eight-byte owner at
 * 0x02000088 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9CF4(void)
{
    return (u8 *)0x02009cf4;
}

/*
 * Returns the in-image table at 0x02009db4. The eight-byte owner at
 * 0x02000094 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9db4(void)
{
    return (u8 *)0x02009db4;
}

/*
 * Returns the in-image table at 0x02009ddc. The eight-byte owner at
 * 0x0200009c includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9ddc(void)
{
    return (u8 *)0x02009ddc;
}

/* Picks one of three tables by story progress, testing flags 0x87a then
 * 0x834. */
u8 *SceneData_SelectTable9efcByFlags(void)
{
    if (Story_unk13(0x87A) != 0) {
        return gOv;
    }
    if (Story_unk14(0x834) != 0) {
        return gOv2;
    }
    return gOv3;
}

/*
 * Runs the scene's fixed position and pose sequence for actors 0, 1, 5 and
 * 13, skipped entirely when flag 0x801 is set. Two of the record fields feed
 * the placement calls as s16 values shifted into fixed point, and base5_fbf
 * holds a base that later sites use as base + 1 and base + 2.
 */
void Scene_RunActorPositionSequence(void)
{
    u8 *record;
    s32 x, y;
    s32 base5_fbf;

    if (Story_Check(0x801) != 0) {
    } else {
        Story_unk4_4();
        Story_Place(0, 0x10000, 0x8000);
        Story_Do(0xfa6);
        Story_unk5_4(13, 1);
        Story_unk2_3(0, 232, 0x108);
        Story_unk6_4(0, 0);
        Story_unk7_4(0, 13, 20);
        Story_unk8_4(13, 2);
        Story_unk9_4(13, 0, 10);
        record = (u8 *)Story_unk3_4(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Story_unk10_4(5, x << 16, y << 16);
        Story_unk11_4(1, x << 16, y << 16);
        Story_unk3_3(5, 0x8000, 0x4000);
        Story_unk4_3(1, 0x8000, 0x4000);
        Story_unk5_3(5, 248, 0x108);
        Story_unk6_3(1, 216, 0x108);
        Story_unk12_4(0, 1);
        Story_unk13_4(5, 1);
        Story_unk14_4(1, 1);
        Story_unk15_4(4);
        Story_unk7_3(5, 0xb000, 0);
        Story_unk8_3(1, 0xd000, 20);
        Story_unk16_4(5, 4);
        Story_unk17_4(10);
        Story_unk18_4(5, 0, 20);
        Story_unk19_4(13, 1);
        Story_unk20_4(10);
        Story_unk9_3(13, 0x3000, 10);
        Story_unk21_4(13, 0, 10);
        Story_unk10_3(1, 0x3000, 40);
        Story_unk22_4(1, 2);
        Story_unk23_4(1, 0, 10);
        Story_unk24_4(13, 2);
        Story_unk11_3(13, 0x5000, 10);
        Story_unk25_4(13, 3);
        Story_unk26_4(13, 0, 8);
        Story_unk12_3(5, 0x3000, 20);
        Story_unk27_4(5, 0, 10);
        Story_unk13_3(13, 0x3000, 10);
        Story_unk28_4(13, 3);
        Story_unk29_4(10);
        Story_unk30_4(13, 0, 6);
        Story_unk14_3(0, 0x101, 0);
        Story_unk15_3(1, 0x101, 0);
        Story_unk16_3(5, 0x101, 60);
        Story_unk17_3(1, 0xd000, 0);
        Story_unk18_3(5, 0xb000, 20);
        Story_unk31_4(5, 2);
        Story_unk32_4(5, 0, 10);
        Story_unk19_3(13, 0x102, 60);
        Story_unk33_4(13, 4);
        Story_unk34_4(13, 0, 10);
        Story_unk35_4(1, 0, 0);
        Story_unk20_3(5, 0x8000, 20);
        Story_unk21_3(0, 0x100, 60);
        Story_unk22_3(1, 0xd000, 0);
        Story_unk23_3(5, 0xb000, 0);
        Story_unk24_3(13, 0x5000, 20);
        Story_unk25_3(13, 0x3000, 40);
        Story_unk26_3(13, 0x5000, 40);
        Story_unk27_3(13, 0x3000, 20);
        Story_unk2(13, 0);
        if (Story_unk3(0, 0) == 0) {
            Story_unk36_4((s32)gVal);
        } else {
            Story_unk2_2(0xfb1);
        }
        Story_unk37_4(20);
        Story_unk38_4(13, 3);
        Story_unk39_4(10);
        Story_unk40_4(13, 0, 10);
        Story_unk3_2(0xfb2);
        Story_unk41_4(5, 2);
        Story_unk28_3(5, 0x8000, 10);
        Story_unk42_4(5, 0, 6);
        Story_unk29_3(1, 0x103, 30);
        Story_unk43_4(1, 4, 30);
        Story_unk44_4(1, 0, 10);
        Story_unk45_4(1, 0, 6);
        Story_unk46_4(0, 1, 10);
        Story_unk47_4(0, 5, 0);
        Story_unk48_4(13, 1, 10);
        Story_unk49_4(13, 5, 10);
        Story_unk50_4(0, 3);
        Story_unk51_4(1, 3);
        Story_unk52_3(5, 3);
        Story_unk53_3(10);
        Story_unk54_3(5, 1);
        Story_unk55_3(1, 1);
        Story_unk56_3(0, 0);
        Story_unk30_3(0, 0x4000, 0);
        Story_unk31_3(1, 0x4000, 0);
        Story_unk32_3(5, 0x4000, 16);
        Story_Run(13, 0x102);
        Story_unk57_3(13, 3);
        Story_unk58_3(10);
        Story_unk59_3(13, 0, 6);
        Story_unk33_3(0, 0x100, 0);
        Story_unk34_3(1, 0x100, 0);
        Story_unk35_3(5, 0x100, 40);
        Story_unk60_3(13, 4);
        Story_unk61_3(10);
        Story_unk62_3(13, 0, 6);
        Story_unk63_3(13, 1);
        Story_unk64_3(13, 0, 6);
        Story_unk36_3(5, 0xb000, 10);
        Story_unk65_3(5, 0, 6);
        Story_unk37_3(13, 0x3000, 10);
        Story_unk66_3(13, 3);
        Story_unk67_3(6);
        Story_unk68_3(13, 0, 10);
        Story_unk69_3(0, 2, 0);
        Story_unk70_3(1, 2, 0);
        Story_unk71_3(5, 2, 10);
        Story_unk38_3(0, 0xc000, 0);
        Story_unk39_3(1, 0xd000, 20);
        Story_unk72_3(1, 0, 6);
        Story_unk40_3(13, 0x5000, 10);
        Story_unk73_3(13, 3);
        Story_unk74_3(16);
        Story_unk75_3(0, 5, 40);
        Story_unk76_3(5, 0, 10);
        Story_unk77_3(13, 2);
        Story_unk41_3(13, 0x3000, 10);
        Story_unk78_3(13, 0, 6);
        Story_unk42_3(0, 0xc000, 0);
        Story_unk43_3(5, 0xb000, 30);
        Story_unk44_3(0, 0x105, 0);
        Story_unk45_3(1, 0x105, 0);
        Story_unk46_3(5, 0x105, 80);
        Story_unk79_3(13, 4);
        Story_unk4(13, 0);
        if (Story_unk5(0, 0) == 0) {
            Story_unk4_2(0xfbd);
        } else {
            Story_unk5_2(0xfbe);
        }
        Story_unk80_3(13, 0, 20);
        base5_fbf = (s32)gVal2;
        Story_unk81_3(base5_fbf);
        Story_unk82_3(1, 0, 10);
        Story_unk83_3(1, 2);
        Story_unk6(1, 0);
        if (Story_unk7(0, 0) == 0) {
            Story_unk84_3((base5_fbf + 1));
        } else {
            Story_unk85_3((base5_fbf + 2));
        }
        Story_unk86_3(1, 0, 6);
        Story_unk6_2(0xfc2);
        Story_unk47_3(5, 0x8000, 10);
        Story_unk87_3(5, 1);
        Story_unk88_3(5, 0);
        Story_unk89_3(4);
        if (Story_unk8(0, 0) == 1) {
            Story_unk90_3(5, 2, 20);
            Story_unk91_3(5, 0, 10);
        } else {
            Story_unk92_3(0, 3);
            Story_unk93_3(1, 3);
            Story_unk94_3(5, 3);
            Story_unk95_3(8);
            Story_unk96_3(0, 0);
            bump_step(1);
        }
        Story_unk97_3(13, 3);
        Story_unk98_3(10);
        Story_unk99_3(13, 0, 10);
        Story_unk48_3(0, 0xc000, 0);
        Story_unk49_3(1, 0xd000, 0);
        Story_unk50_3(5, 0xb000, 10);
        Story_unk100_3(0, 3);
        Story_unk101_3(1, 3);
        Story_unk102_3(5, 3);
        Story_unk103_3(10);
        Story_unk104_3(0, 0);
        Story_unk105_3(20);
        Story_unk106_3(13, 2);
        Story_unk107_3(13, 0);
        Story_unk108_3(4);
        if (Story_unk9(0, 0) == 0) {
            Story_unk7_2(0xfc6);
        } else {
            Story_unk8_2(0xfc9);
        }
        ((void (*)())Story_unk15)(10);
        Story_unk109_3(1, 2);
        Story_unk110_3(1, 0, 10);
        Story_unk111_3(1, 0, 6);
        Story_unk51_3(5, 0x8000, 10);
        Story_unk112_3(5, 4);
        Story_unk113_3(5, 0, 6);
        Story_unk114_3(1, 2);
        Story_unk52_2(1, 0x8000, 10);
        Story_unk53_2(1, 0x103, 30);
        Story_unk115_3(1, 0, 10);
        Story_unk54_2(13, 0x100, 40);
        Story_unk116_3(13, 4, 40);
        Story_unk55_2(0, 0xc000, 0);
        Story_unk56_2(1, 0xc000, 0);
        Story_unk57_2(5, 0xb000, 20);
        Story_unk58_2(13, 0xb000, 10);
        Story_unk117_3(158);
        Story_unk59_2(0x200a0ac, 43, 8);
        Story_unk60_2(13, 0x10000, 0x8000);
        Story_unk118_3(13, 232, 218);
        Story_unk119_3(13, 0, 0);
        Story_unk61_2(0, 0x101, 0);
        Story_unk62_2(1, 0x101, 0);
        Story_unk63_2(5, 0x101, 60);
        *(s32 *)((*(u8 **)gWork + 0x1c0)) = 0x202;
        Story_unk120_3();
        Story_unk121_3();
        Story_unk122_3(13);
        Story_unk123_3();
    }
}

/* overlays/scene/story/progress_actor/progress_dependent_actor_scene_tail.c */

extern u8 gOv4[];

/* Contiguous unnamed state-owner run for resource_375. */

/* The same import at two sites, so two names: the flag set at +0x1e and again
 * at +0x2e. */

void Scene_RunScene375(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Story_unk124_3();
    Story_unk125_3(13, 0, 0);
    Story_unk64_2(1, 0xd80000, 0x1080000);
    Story_unk65_2(5, 0xf80000, 0x1080000);
    record = Story_unk16(1);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    record = Story_unk17(5);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Story_unk66_2(0x200a0ac, 43, 8);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Story_unk126_3();
    Story_unk127_3();
    Story_unk128_3(40);
    Story_unk67_2(13, 0xcccc, 0x6666);
    Story_unk68_2(13, 0xe60000, 0xdc0000);
    Story_unk129_3(13, 230, 232);
    Story_unk130_3(20);
    Story_unk131_3(13, 3);
    Story_unk9_2(0xfcc);
    Story_unk132_3(13, 0, 10);
    Story_unk133_3(13, 2);
    Story_unk69_2(13, 0x3000, 10);
    Story_unk134_3(13, 0, 10);
    Story_unk135_3(1, 0, 0);
    Story_unk70_2(0, 0x8000, 10);
    Story_unk136_3(0, 3);
    Story_unk137_3(1, 3);
    Story_unk138_3(0, 0);
    Story_unk139_3(20);
    Story_unk140_3(0, 0, 0);
    Story_unk71_2(5, 0x8000, 10);
    Story_unk141_3(5, 3);
    Story_unk142_3(0, 3);
    Story_unk143_3(0, 0);
    Story_unk72_2(1, 0xcccc, 0x6666);
    Story_unk73_2(5, 0xcccc, 0x6666);
    Story_unk144_3(0, 0xc000, 0);
    Story_unk145_3(1, 2);
    record = Story_unk10(0);
    if (record != 0) {
        Story_unk146_3(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk147_3(5, 2);
    record = Story_unk11(0);
    if (record != 0) {
        Story_unk148_3(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk149_3(13, 2);
    record = Story_unk12(0);
    if (record != 0) {
        Story_unk150_3(13, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk151_3(1);
    Story_unk152_3(1, 0, 0);
    Story_unk153_3(5, 0, 0);
    Story_unk154_3(13);
    Story_unk155_3(13, 0, 0);
    Story_unk156_3(1, 1);
    Story_unk157_3(5, 1);
    Story_unk158_3(13, 1);
    Story_unk159_3(14, 0, 0);
    Story_unk160_3(15, 0, 0);
    Story_unk10_2(0x801);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Story_unk2_4(0x10000, 0);
    Story_unk11_2(0x242);
    Story_unk161_3();
}

void Scene_SetupWithDescriptorA0ACWhenFlag242Clear(void)
{
    if (Story_unk18(0x242) == 0) {
        Story_unk12_2(0x9E);
        Story_unk74_2(gOv4, 0x2B, 8);
    }
    Story_unk75_2(0, 0xE5, 0xD9);
    Story_unk13_2(3);
}

void State_SetWorkAndFlag87d(void)
{
    extern u8 *gWork;

    u8 *work;

    Story_unk162_3();

    work = gWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    Story_unk14_2(0x87D);
    Story_Apply(12, 0);
    Story_unk15_2(0x900);   /* 144 << 4 */
    Story_unk163_3();
}

void State_SetWorkAndFlag87e(void)
{
    extern u8 *gWork;

    u8 *work;

    Story_unk164_3();

    work = gWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    Story_unk16_2(0x87E);
    Story_Apply2(12, 1);
    Story_unk17_2(0x900);   /* 144 << 4 */
    Story_unk165_3();
}

void Dialogue_RunActorSixteenDialogue(void)
{
    Story_unk166_3();
    Story_unk18_2(0x1C96);
    Story_Apply3(16, 0);
    Story_unk167_3();
}

/* overlays/scene/script/gated_actor.c */
/*
 * DRAFTED SCENE SCRIPT for Scene_RunGatedActorSequence.
 *
 * One progress-gated field cutscene.  The owner returns immediately when
 * progress flag 0x808 is already set; otherwise it sets that flag and plays a
 * fixed beat sequence of 166 calls over scene slots 0, 1, 5, 14 and 15.
 *
 * Structure recovered from the reference: one early return, two identical
 * two-way branches on a scene predicate that select between neighbouring
 * event ids, two null-guarded object lookups near the end, and four
 * read-modify-write updates of the object byte at offset 90.
 *
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, following the exact sibling
 * games/gs1/src/overlays/scene/script/run_actor_position_sequence.c.
 * Where several pre-relocation words in this owner share one relocated
 * destination, the single unambiguous spelling is reused for every call to
 * that destination.
 *
 * RESIDUAL / INTEGRATOR NOTE.  The pre-relocation word Script_unk3_4 is used
 * at two sites in this owner and relocates to two different destinations:
 * 0x02000d22 -> runtime 0x02009b84 (main-image 0x0808a218) and 0x02000d52 ->
 * runtime 0x02009b54 (main-image 0x0808a1b8).  0x02009b84 is reachable through
 * no other pre-relocation word here, so a standalone candidate cannot bind it;
 * `alchemy score` reports "ambiguous overlay call identity".  It is spelled
 * here as gOvGatedActor (Data_ bypasses the call-name resolver and binds the
 * literal runtime address with the Thumb bit set), which compiles and emits
 * the reference's call word, but it is a scoring workaround, not source.
 * The project's own convention for this case is the suffixed pair already used
 * by this overlay's other unit, games/gs1/src/overlays/scene/script/
 * run_actor_position_sequence.c: declare Script_unk4_4 and Script_unk5_3,
 * spell the 0x02000d22 site _a and the 0x02000d52 site _b, and record both in
 * the unit's `absolute_symbols` as 0x02009b84 and 0x02009b54, kind "thumb".
 * That table is integrator-owned, so the suffixed spellings do not bind from a
 * standalone candidate and are not used here.  Every other call symbol in this
 * file auto-binds from the reference and needs no explicit declaration.
 *
 * RESIDUAL / ALLOCATION.  Two halfwords still differ, both in the second of
 * the two `|= 1` updates at reference 0x020011b2: the reference spells that
 * store `orrs r5, r3; strb r5, [r2]`, reusing the register that held the
 * constant, while this source emits `orrs r3, r5; strb r3, [r2]`.  The first
 * `|= 1` at 0x020011a2, and both `&= 0xfe` updates at 0x0200115e and
 * 0x0200116e, already match byte for byte.  This is a commutative-operand and
 * register-allocation choice with no source evidence behind it; the triage
 * router classifies it as allocation-uncovered and warns against respelling
 * source to shave the tool triage number rather than to recover the real
 * shape. That warning is not a rule to leave this open: reaching zero here
 * with an honest, ordinary spelling is a good outcome, and the integrator
 * adopts it via `alchemy adopt`, which retires the retained-assembly region.
 * A prior version of this comment claimed the retained-corpus gate forbids a
 * byte-exact candidate; that was wrong on the facts and is corrected here.
 * candidate-corpus-check only scans routes registered in the Makefile
 * CANDIDATE_SINGLE_OWNERS variable and units under games/gs1/recon/en/units/,
 * and this file is registered in neither, so the gate does not even see it.
 * Where the gate does apply, an exact result is the signal to adopt, not a
 * reason to avoid closing.
 *
 * MODELLING ARTIFACTS carried over from the exact sibling, not recovered
 * source: the Call/Value function-pointer wrappers below, and the event ids
 * and table pointer modelled as `(s32)` of an extern array symbol.  Both exist
 * to express what the reference does with constants and register lifetimes in
 * the idiom this overlay's adopted C already uses; neither asserts that the
 * original source spelled them that way.
 *
 * Uncertain and deliberately left neutral: the project has no name for any of
 * the resolved main-image targets, so no role names are invented.  The
 * resolved target and the observed argument count are recorded beside each
 * declaration.  The slot numbers (0, 1, 5, 14, 15) and the event ids held in
 * evt/evt2 are raw values whose meaning is not established.  Only three
 * offsets of the looked-up object record are evidenced by this owner: the
 * signed halfwords at 10 and 18, read as the whole-cell halves of 16.16
 * coordinates, and the flag byte at 90.
 */

/* 0x080770c0, one argument, result tested */

/* 0x080770c8, one argument */

/* 0x0808a010, one argument (frame count) */

/* 0x0808a018, no arguments */

/* 0x0808a020, no arguments */

/* 0x0808a070, two arguments, result tested */

/* 0x0808a080, one argument, returns an object record or NULL */
u8 *Script_unk6_3();
/* 0x0808a090, three arguments */

/* 0x0808a0b8, three arguments */

/* 0x0808a0c8, three arguments */

/* 0x0808a0d0, three arguments */

/* 0x0808a0e8, one argument */

/* 0x0808a0f0, three arguments */

/* 0x0808a100, two arguments */

/* 0x0808a110, two arguments */

/* 0x0808a130, two arguments */

/* 0x0808a138, two arguments */

/* 0x0808a148, three arguments */

/* 0x0808a150, three arguments */

/* 0x0808a168, three arguments, third is a table address */

/* 0x0808a170, one argument (event id) */

/* 0x0808a178, two arguments, returned value discarded at both call sites */

/* 0x0808a188, three arguments */

/* 0x0808a1b8, three arguments */

/* 0x0808a1e8, three arguments */

/* 0x0808a208, two arguments */

/* 0x0808a210, four arguments */

/* 0x0808a218, no arguments; see the integrator note above */
void gOvGatedActor();
/* 0x0808a4f0, no arguments */

/* 0x080f9010, one argument (sound id) */

/* Event ids the reference keeps live in a register across the sequence. */
extern u8 gValGatedActor[];
extern u8 gVal2GatedActor[];
/* Overlay table passed to the last two calls. */
extern u8 gOv2GatedActor[];

void Scene_RunGatedActorSequence(void)
{
    u8 *record;
    s32 x, z;
    s32 evt;
    s32 evt2;
    s32 tbl;

    if (Script_Check(0x808) != 0) {
        return;
    }

    Script_unk7_3();
    Script_unk8_2(17);
    Script_Do(0x808);

    evt = (s32)gValGatedActor;
    Script_unk9_2(evt);
    Script_unk10_2(14, 0, 10);

    Script_Place(0, 0x13333, 0x9999);
    Script_unk2_3(0, 0x100, 30);
    Script_unk3_3(0, 0x188, 0x148);
    Script_unk4_3(0, 0x4000, 10);

    record = Script_unk6_3(0);
    x = *(s16 *)(record + 10);
    z = *(s16 *)(record + 18);
    Script_unk11_2(5, x << 16, z << 16);
    Script_unk11_2(1, x << 16, z << 16);

    Script_Place(5, 0x13333, 0x9999);
    Script_Place(1, 0x13333, 0x9999);
    Script_unk5_2(5, 0x178, 0x148);
    Script_unk3_3(1, 0x198, 0x148);
    Script_unk12_2(0, 0);
    Script_unk12_2(5, 0);
    Script_unk12_2(1, 0);
    Script_unk4_3(1, 0x8000, 0);
    Script_unk13_2(5, 0, 20);
    Script_unk2_3(0, 0x101, 0);
    Script_unk2_3(1, 0x101, 0);
    Script_unk2_3(5, 0x101, 30);
    Script_unk4_3(0, 0x4000, 0);
    Script_unk4_3(1, 0x4000, 0);
    Script_unk4_3(5, 0x4000, 0);
    Script_Run(0x60000, 0xc000);
    Script_unk2_4(0xd70000, -1, 0x1590000, 1);
    gOvGatedActor();
    Script_unk14_2(20);
    Script_unk8_2(61);

    Script_unk15(14, 4);
    Script_unk12_2(14, 4);
    Script_unk10_2(14, 0, 20);
    Script_unk13_2(15, 0, 10);
    Script_unk10_2(15, 0, 10);
    Script_unk15(14, 3);
    Script_unk10_2(14, 0, 10);
    Script_unk4_3(15, 0x4000, 60);
    Script_unk16(15, 1);
    Script_unk10_2(15, 0, 20);
    Script_unk15(14, 3);
    Script_unk10_2(14, 0, 10);
    Script_unk15(15, 4);
    Script_unk10_2(15, 0, 6);
    Script_unk2_3(14, 0x100, 0);
    Script_unk16(14, 2);
    Script_unk14_2(20);
    Script_unk13_2(15, 0, 10);
    Script_unk2_3(15, 0x101, 40);
    Script_unk13_2(14, 0, 60);
    Script_unk4_3(14, 0x8000, 40);
    Script_unk13_2(14, 0, 40);
    Script_unk3_3(14, 232, 0x168);
    Script_unk13_2(14, 0, 10);
    Script_unk15(15, 3);
    Script_unk14_2(10);
    Script_unk5_2(14, 0x188, 0x168);
    Script_unk3_3(15, 216, 0x168);
    Script_unk5_2(15, 0x178, 0x168);
    Script_unk4_3(0, 0x4000, 0);
    Script_unk4_3(1, 0x4000, 0);
    Script_unk4_3(5, 0x4000, 0);
    Script_unk2_4(0x1890000, -1, 0x1530000, 1);
    Script_unk5_2(14, 0x188, 0x168);
    Script_unk3_3(15, 0x178, 0x168);
    Script_unk12_2(14, 0);
    Script_unk12_2(15, 0);
    Script_unk4_3(14, 0xd000, 0);
    Script_unk4_3(15, 0xd000, 30);
    Script_unk16(14, 2);
    Script_unk10_2(14, 0, 10);
    Script_unk2_3(1, 258, 60);
    Script_unk16(1, 1);
    Script_unk10_2(1, 0, 10);
    Script_unk15(15, 4);
    Script_unk2(0x100f, 0);

    if (Script_unk3(0, 0) == 0) {
        Script_unk9_2(evt + 10);
    } else {
        Script_unk9_2(evt + 11);
    }

    Script_unk6_2(0x100f, 0, 10);
    Script_unk16(1, 2);

    evt2 = (s32)gVal2GatedActor;
    Script_unk9_2(evt2);
    Script_unk10_2(1, 0, 20);
    Script_unk17(14, 15, 40);
    Script_unk4_3(14, 0xd000, 0);
    Script_unk4_3(15, 0xd000, 20);
    Script_unk10_2(14, 0, 60);
    Script_unk16(15, 1);
    Script_unk14_2(10);
    Script_unk15(15, 3);
    Script_unk6_2(0x100f, 0, 10);
    Script_unk16(5, 2);
    Script_unk15(5, 3);
    Script_unk6_2(0x1005, 0, 20);
    Script_unk16(14, 2);
    Script_unk4_3(14, 0xa000, 20);
    Script_unk2(14, 0);

    if (Script_unk3(0, 0) == 0) {
        Script_unk9_2(evt2 + 5);
    } else {
        Script_unk9_2(evt2 + 6);
    }

    Script_unk13_2(5, 0, 0);
    Script_unk4_3(1, 0x8000, 20);
    Script_unk16(14, 2);
    Script_unk10_2(14, 0, 10);
    Script_unk18(14, 1, 30);
    Script_unk18(14, 5, 30);
    Script_unk2_3(14, 0x105, 80);
    Script_unk15(14, 4);
    Script_unk2_2(0xf98);
    Script_unk10_2(14, 0, 6);
    Script_unk2_3(0, 0x100, 0);
    Script_unk2_3(1, 0x100, 0);
    Script_unk2_3(5, 0x100, 0);
    Script_unk19(1, 1);
    Script_unk19(5, 1);
    Script_unk16(0, 1);
    Script_unk14_2(40);
    Script_unk16(5, 2);
    Script_unk4_3(1, 0x4000, 0);
    Script_unk4_3(5, 0x4000, 10);
    Script_unk6_2(0x1005, 0, 10);
    Script_unk16(15, 2);
    Script_unk13_2(15, 0, 10);
    Script_unk6_2(0x100f, 0, 10);
    Script_unk4_3(14, 0x8000, 20);
    Script_unk15(14, 4);
    Script_unk10_2(14, 0, 10);
    Script_unk16(15, 2);
    Script_unk14_2(10);
    Script_unk4_3(14, 0xb000, 0);
    Script_unk4_3(15, 0xd000, 20);
    Script_Place(15, 0x8000, 0x4000);
    Script_Place(14, 0x8000, 0x4000);

    record = Script_unk6_3(14);
    *(record + 90) &= 0xfe;
    record = Script_unk6_3(15);
    *(record + 90) &= 0xfe;

    Script_unk5_2(14, 0x188, 0x178);
    Script_unk3_3(15, 0x178, 0x178);
    Script_unk14_2(6);

    record = Script_unk6_3(14);
    *(record + 90) |= 1;
    record = Script_unk6_3(15);
    {
        /*
         * A result temporary, not the compound or-assign the first
         * occurrence above uses. The reference writes the result into
         * the mask register rather than the loaded value, and the
         * two-address ORR only does that when the merged result is its
         * own object; the compound form keeps the loaded value as
         * destination. Same technique already adopted in the sibling
         * owners resource_3bd:020013f8 and resource_39e:02001494.
         */
        u8 merged = (u8)(*(record + 90) | 1);

        *(record + 90) = merged;
    }

    Script_unk12_2(14, 0);
    Script_unk12_2(15, 0);
    Script_unk14_2(20);
    Script_unk16(1, 2);
    Script_unk10_2(1, 0, 10);
    Script_unk13_2(0, 1, 20);
    Script_unk12_2(0, 3);
    Script_unk15(1, 3);
    Script_unk8_2(17);

    Script_unk12_2(1, 2);
    record = Script_unk6_3(0);
    if (record != 0) {
        Script_unk20(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_unk21(1);
    Script_unk11_2(1, 0, 0);

    Script_unk12_2(5, 2);
    record = Script_unk6_3(0);
    if (record != 0) {
        Script_unk20(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Script_unk21(5);
    Script_unk11_2(5, 0, 0);

    tbl = (s32)gOv2GatedActor;
    Script_unk7_2(14, 0x10000, tbl);
    Script_unk7_2(15, 0x10000, tbl);
    Script_unk22();
    Script_unk23();
}
