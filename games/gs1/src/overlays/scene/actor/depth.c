#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/depth.h"

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

struct OverlayActorPosition *Actor_Run3();

struct OverlayActorState *Actor_Run4();

u8 *Actor_Run5(s32 group);

/* Wait at most sixty frames for the object to reach the requested height. */

/*
 * resource_387: issue a scene request and then wait.
 */

/* Old-style: the two imports are called with different arities. */

/* resource_387 three-call story-flag wrapper at 0x020004d4. */

void Actor_Run6();          /* Wait this many frames. */

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
    Actor_Run7(selector, 0);
    Actor_Run6(frames);
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
    Actor_Do2(0x862);
    if (v3 == 15) {
        Actor_SetRect(47, 18, 1, 2, 16, 18);
    } else if (v3 == 16) {
        Actor_SetRect2(48, 18, 1, 2, v3, 18);
        Actor_Do3(0x861);
    } else {
        Actor_SetRect3(47, 18, 1, 2, 16, 18);
        Actor_Do4(0x862);
    }
}

void Scene_RunScene387SequenceC(void)
{
    s32 record;
    s32 v3;
    s32 v5;

    record = Actor_Check2(10);
    v3 = *(volatile s32 *)(record + 8) / 0x100000;
    if (v3 == 23) {
        Actor_Run8(10);
        *(u8 *)(Actor_Check6(10) + 35) = 2;
        v5 = 0;
        *(u8 *)(Actor_Check7(10) + 85) = v5;
        Actor_Run9(Actor_Check8(10), 0);
        Actor_SetRect4(54, 17, 1, 1, v3, 17);
        Actor_Do5(0x863);
    }
}

void Scene_RunScene387SequenceD(void)
{
    u8 *p5;
    s32 v5;

    p5 = *(u8 **)gWork;
    Actor_Run10();
    Actor_Run11(0, 8);
    Actor_Run12(20);
    Actor_Place(0, 0x3333, 0x1999);
    Actor_Place2(9, 0x3333, 0x1999);
    Actor_Run13(185);
    v5 = (11 - (*(s16 *)(p5 + 0x16c) << 1)) << 4;
    Actor_Run14(0, v5, 0);
    Actor_Run15(9, v5, 0);
    Actor_Run16(0);
    Actor_Run17(9);
    Actor_Run18(20);
    Actor_Run19(0, 1);
    Actor_Run20();
    Actor_Run21();
    Actor_Run22();
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
    Actor_Run23();
    Actor_Do8(0x866);
    Actor_Run24();
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

    Actor_Run25();
    Actor_Run26();
    Actor_Run27(30);
    Actor_Do6(0x138f);
    Actor_Place3(0, 0xcccc, 0x6666);
    Actor_Place4(1, 0xcccc, 0x6666);
    Actor_Place5(0, 0xe000, 20);
    record = Actor_Check3(0);
    if (record != 0) {
        Actor_Run28(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Actor_Place6(1, 0x108, 168);
    Actor_Place7(1, 0x6000, 20);
    Actor_Run29(20);
    Actor_Run30(1, 4);
    Actor_Run31(20);
    Actor_Run32(1, 0, 20);
    Actor_Run33(0, 3);
    Actor_Run34(20);
    if (Actor_Check4(0x855) == 0) {
        Actor_Run35(1, 2);
        record = Actor_Check5(0);
        if (record != 0) {
            Actor_Run36(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_Run37(1);
        Actor_Run38(1, 0, 0);
        Actor_Run39();
    } else {
        Actor_Place8(2, 0x1680000, 0xf80000);
        Actor_Place9(2, 0xcccc, 0x6666);
        Actor_Place10(2, 0x110, 248);
        Actor_Place11(2, 0x110, 208);
        Actor_Place12(2, 0xa000, 20);
        Actor_Run40(2, 2);
        Actor_Run41(20);
        Actor_Run42(2, 0, 20);
        Actor_Place13(0, 0x2000, 0);
        Actor_Place14(1, 0x4000, 0);
        Actor_Run43(20);
        Actor_Place15(0, 0x100, 0);
        Actor_Place16(1, 0x100, 60);
        Actor_Run44(2, 3);
        Actor_Place17(2, 0x108, 200);
        Actor_Run45(0, 248, 168);
        Actor_Run46(2, 248, 184);
        Actor_Run47(0);
        Actor_Place18(0, 0x6000, 0);
        Actor_Place19(1, 0x6000, 0);
        Actor_Run48(2, 232, 184);
        Actor_Run49(20);
        Actor_Place20(2, 0x105, 60);
        Actor_Place21(2, 0xe000, 20);
        Actor_Run50(2, 4);
        Actor_Run51(20);
        Actor_Run52(2, 0, 20);
        Actor_Run53(0, 3);
        Actor_Run54(1, 3);
        Actor_Run55(20);
        Actor_Place22(2, 0x8000, 20);
        Actor_Run56(2, 0, 120);
        Actor_Place23(0, 0x105, 0);
        Actor_Place24(1, 0x105, 60);
        Actor_Run57(0, 1, 0);
        Actor_Run58(60);
        Actor_Place25(0, 0x6000, 0);
        Actor_Place26(1, 0x6000, 0);
        Actor_Run59(60);
        Actor_Place27(2, 0x106, 0);
        Actor_Run60(2, 1);
        Actor_Run61(30);
        Actor_Run62(2, 0, 30);
        Actor_Place28(2, 0xe000, 20);
        Actor_Run63(0, 2);
        Actor_Run64(1, 2);
        Actor_Run65(20);
        Actor_Run66(2, 0, 20);
        Actor_Run67(0, 3);
        Actor_Run68(1, 3);
        Actor_Run69(20);
        Actor_Run70(2, 3);
        Actor_Run71(20);
        Actor_Place29(2, 0x8000, 20);
        Actor_Run72();
        Actor_Run73(2, 1);
        Actor_Run74(20);
        Actor_Run75();
        Actor_Place30(0, 0x100, 0);
        Actor_Place31(1, 0x100, 60);
        Actor_Run76(1, 2, 0);
        Actor_Run77(20);
        Actor_Run78(1, 20);
        Actor_Place32(2, 0x108, 184);
        Actor_Run79(10);
        Actor_Run80(2, 1, 0);
        Actor_Run81(1, 2, 0);
        Actor_Run82(0, 2, 0);
        Actor_Run83(20);
        Actor_Run84(2, 3);
        Actor_Run85(20);
        Actor_Run86(2, 60);
        Actor_Place33(0, 0x105, 0);
        Actor_Place34(1, 0x105, 60);
        Actor_Place35(0, 0x101, 0);
        Actor_Place36(1, 0x101, 0);
        Actor_Run87(60);
        Actor_Place37(1, 0x8000, 0);
        Actor_Run88(0, 0, 0);
        Actor_Run89(60);
        Actor_Place38(1, 0x4000, 0);
        Actor_Place39(0, 0x2000, 0);
        Actor_Run90(10);
        Actor_Run91(1, 20);
        Actor_Place40(2, 0x105, 0);
        Actor_Run92(60);
        Actor_Run93(2, 4);
        Actor_Run94(20);
        Actor_Run95(2, 20);
        Actor_Run96(0, 2, 0);
        Actor_Run97(1, 2, 0);
        Actor_Place41(0, 0x102, 0);
        Actor_Place42(1, 0x102, 0);
        Actor_Run98(60);
        Actor_Run99(2, 2);
        Actor_Run100(20);
        Actor_Run101(2, 30);
        Actor_Place43(0, 0x101, 0);
        Actor_Place44(1, 0x101, 0);
        Actor_Run102(80);
        Actor_Run103(2, 3);
        Actor_Run104(20);
        Actor_Run105(2, 20);
        Actor_Run106(0, 1);
        Actor_Run107(1, 1);
        Actor_Run(0, 0x102);
        Actor_Run2(1, 0x102);
        Actor_Run108(60);
        Actor_Run109(2, 4);
        Actor_Run110(20);
        Actor_Run111(2, 20);
        Actor_Run112(0, 0, 0);
        Actor_Place45(1, 0x8000, 0);
        Actor_Run113(80);
        Actor_Run114(0, 2, 0);
        Actor_Run115(1, 2, 0);
        Actor_Run116(30);
        Actor_Run117(2, 3);
        Actor_Run118(20);
        Actor_Run119(2, 30);
        Actor_Run120(2, 4);
        Actor_Run121(20);
        Actor_Run122(2, 20);
        Actor_Run123(0, 2);
        Actor_Run124(1, 2);
        Actor_Run125(20);
        Actor_Run126(2, 3);
        Actor_Run127(20);
        Actor_Run128(2, 40);
        Actor_Run129(0, 3);
        Actor_Run130(1, 3);
        Actor_Run131(20);
        Actor_Run132(2, 1);
        Actor_Run133(60);
        Actor_Run134();
        Actor_Run135(2, 1);
        Actor_Run136(20);
        Actor_Run137(2, 248, 184);
        Actor_Run138(20);
        Actor_Run139(2, 20);
        Actor_Place46(0, 0x8000, 0);
        Actor_Place47(1, 0x8000, 0);
        Actor_Run140(120);
        Actor_Run141(2, 30);
        Actor_Run142(0, 2, 0);
        Actor_Run143(1, 2, 0);
        Actor_Run144(2, 0, 0);
        Actor_Run145(20);
        Actor_Run146(0, 3);
        Actor_Run147(1, 3);
        Actor_Run148(2, 3);
        Actor_Run149(50);
        Actor_Place48(1, 0xcccc, 0x6666);
        Actor_Place49(2, 0xcccc, 0x6666);
        Actor_Run150(1, 248, 168);
        Actor_Run151(2, 248, 168);
        Actor_Run152(2, 0, 0);
        Actor_Run153(1);
        Actor_Run154(1, 0, 0);
        Actor_SetRect5(74, 11, 1, 1, 73, 11);
        Actor_Do7(0x865);
        Actor_Run155();
    }
}

void Overlay387_ConfigureActorEightAtDepth(void)
{
    s32 depth;
    s32 span;
    struct OverlayActorState *state;

    Actor_Run156();
    depth = Actor_Run3(8)->depth_fixed >> 20;
    if (depth == 11) {
        Actor_Run157(8);
        state = Actor_Run4(8);
        state->flags |= 2;
        span = 12;
        Actor_Run158(39, 12, 3, 1, 8, span);
        Actor_Run159(43, 11, 3, 1, span, depth);
        Actor_Run160(2144);
    }
    Actor_Run161();
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
        Actor_Do9(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= height)
            break;
    }
}

/* Apply a value to every matching member of a fifteen-slot group. */
void Actor_ApplyValueAndMatchingSlots(s32 group, s32 value)
{
    u8 *work = Actor_Run5(group);
    s32 i;
    Actor_Apply(group, value);
    for (i = 0; i < 15; i++) {
        if (*(u16 *)(work + 216 + i * 2) == value)
            Actor_Apply2(group, i);
    }
}
