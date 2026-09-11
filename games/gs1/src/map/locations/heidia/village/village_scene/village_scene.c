#include "types.h"
#include "staged_actor.h"

/* map/locations/heidia/village/village_scene/actor_activation.c */
#define StopBlockedActorMotion Func_020002a8
#define ActivateSceneActor17 Func_020014bc
#define ActivateSceneActor18 Func_02001650
#define FinishActor20SceneSequence Func_02004d0c
#define ActivateFiveActorGroupFromFlags Func_02005520
typedef struct MovingActor {
    u8 pad0[6];
    u16 kind;       /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    u8 pad14[0x10];
    s32 vx;         /* 0x24 */
    u8 pad28[4];
    s32 vy;         /* 0x2c */
    u8 pad30[8];
    s32 a;          /* 0x38 */
    u8 pad3c[4];
    s32 b;          /* 0x40 */
} MovingActor;

typedef struct BlockingActor {
    u8 pad0[0x50];
    s32 *info;      /* 0x50 */
} BlockingActor;

extern s32 Data_0200ded8[];
extern s32 Data_0200df18[];
extern u8 *Data_03001ebc;

BlockingActor *Func_02000342(s32 *position, MovingActor *actor);
s32 Func_02005902(MovingActor *actor, s32 *position);
void Func_02002fe6(s32);
void Func_02006afe(void);
void Func_0200317a(s32);
void Func_02006c92(void);
s32 Func_0200a322(s32);
void Func_0200a404(s32);
void Func_0200a41c(s32, s32);
void Func_0200a4c6(void);
void Func_02009936(void);
s32 Func_0200ab38(s32);
void Func_020061c2(s32);
s32 Func_0200ab48(s32);
void Func_020061d2(s32);
s32 Func_0200ab58(s32);
void Func_020061e2(s32);
s32 Func_0200ab68(s32);
void Func_020061f2(s32);
s32 Func_0200ab7a(s32);
void Func_02006204(s32);

s32 StopBlockedActorMotion(MovingActor *actor)
{
    s32 destination[3];
    u32 direction_index = actor->kind >> 12;
    s32 step = Data_0200ded8[direction_index];
    BlockingActor *blocking_actor;

    destination[0] = actor->x + (step & 0xffff0000);
    destination[1] = actor->y;
    step <<= 16;
    destination[2] = actor->z + step;
    blocking_actor = Func_02000342(destination, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = ((s16 *)*(s32 *)((u8 *)blocking_actor->info + 0x28))[kind_index];

        {
            s32 *p = Data_0200df18;

            do {
                if (blocking_kind == *p++) {
                    goto done;
                }
                kind_index++;
            } while (kind_index <= 5);
        }
        actor->vx = 0;
        actor->vy = 0;
        actor->a = 0x80000000;
        actor->b = 0x80000000;
    }
    step = Data_0200ded8[direction_index];
    destination[0] = actor->x + (step & 0xffff0000);
    destination[1] = actor->y;
    step <<= 16;
    destination[2] = actor->z + step;
    if (Func_02005902(actor, destination) > 0) {
        actor->vx = 0;
        actor->vy = 0;
        actor->a = 0x80000000;
        actor->b = 0x80000000;
    }
done:
    return 0;
}

void ActivateSceneActor17(void)
{
    Func_02002fe6(17);
    Func_02006afe();
}

void ActivateSceneActor18(void)
{
    Func_0200317a(18);
    Func_02006c92();
}

void FinishActor20SceneSequence(void)
{
    if (Func_0200a322(0x226)) {
        Func_0200a404(0x2435);
        Func_0200a41c(20, 0);
    } else {
        s16 *q = (s16 *)(Data_03001ebc + 382);

        *q = 0;
        Func_0200a4c6();
        Func_02009936();
    }
}

void ActivateFiveActorGroupFromFlags(void)
{
    if (Func_0200ab38(0x350)) {
        Func_020061c2(0);
    }
    if (Func_0200ab48(0x351)) {
        Func_020061d2(1);
    }
    if (Func_0200ab58(0x352)) {
        Func_020061e2(2);
    }
    if (Func_0200ab68(0x353)) {
        Func_020061f2(3);
    }
    if (Func_0200ab7a(0x354)) {
        Func_02006204(4);
    }
}
#undef StopBlockedActorMotion
#undef ActivateSceneActor17
#undef ActivateSceneActor18
#undef FinishActor20SceneSequence
#undef ActivateFiveActorGroupFromFlags

/* map/locations/heidia/village/village_scene/actor_presentation.c */
#define PlaceActorTwelveAndFinishScene Func_02000c78
void Func_02006326(s32, s32, s32);
void Func_020017d6();

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02006326(actor, x, y);
}

void PlaceActorTwelveAndFinishScene(void)
{
    PlaceActor(12, 0x00f80000, 0x01780000);
    Func_020017d6();
}
#undef PlaceActorTwelveAndFinishScene

/* map/locations/heidia/village/village_scene/actor_setup.c */
#define ConfigureSceneActor9 Func_020012e0
#define ConfigureInteractionRegionA Func_0200226c
#define ConfigureInteractionRegionB Func_020022a0
#define ConfigureInteractionRegionC Func_020022d4
#define ConfigurePrimaryInteractionRegions Func_02002410
#define ConfigureSecondaryInteractionRegions Func_02002468
#define ConfigureSceneActor26 Func_02004b68
#define ConfigureSceneActor14 Func_02004b84
#define ConfigureSceneActor13 Func_02004ba4
#define ConfigureSceneActor12Variant Func_02004bc4
#define ConfigureSceneActor18 Func_02004be4
#define ConfigureActor13Interaction Func_02004d50
#define ConfigureActor13SceneResource Func_02004d88
extern u8 Value_00002440;
extern u8 Value_00002459;
extern u8 Value_0000256c;

void Func_02006914(void);
void Func_0200695a(s32);
void Func_02006984(s32, s32, s32);
void Func_020069a4(s32, s32);
void Func_020069d6(s32, s32, s32);
void Func_02006a2a(s32, s32, s32);
void Func_02002e3c(s32);
void Func_02006954(void);
void Func_02007852(s32, s32, s32, s32, s32, s32);
void Func_0200786c(s32, s32, s32, s32, s32, s32);
void Func_02007886(s32, s32, s32, s32, s32, s32);
void Func_020078a0(s32, s32, s32, s32, s32, s32);
void Func_020078ba(s32, s32, s32, s32, s32, s32);
void Func_020078d4(s32, s32, s32, s32, s32, s32);
void Func_020079f6(s32, s32, s32, s32, s32, s32);
void Func_02007a0a(s32, s32, s32, s32, s32, s32);
void Func_02007a24(s32, s32, s32, s32, s32, s32);
void Func_02007a36(s32, s32, s32, s32, s32, s32);
void Func_02007a4e(s32, s32, s32, s32, s32, s32);
void Func_02007a62(s32, s32, s32, s32, s32, s32);
void Func_02007a7c(s32, s32, s32, s32, s32, s32);
void Func_02007a8e(s32, s32, s32, s32, s32, s32);
void Func_0200a30a(s32, s32, s32);
void Func_0200a190(s32);
void Func_0200a254(s32, s32);
void Func_0200a27a(s32);
void Func_0200a292(s32, s32);
void Func_0200a274(s32, s32);
void Func_0200a29a(s32);
void Func_0200a2b2(s32, s32);
void Func_0200a294(s32, s32);
void Func_0200a2ba(s32);
void Func_0200a2d2(s32, s32);
void Func_0200a2d2_a(s32);
s32 Func_0200a2f2(s32, s32);
void Func_0200a440(s32);
void Func_0200a458(s32, s32);
s32 Func_0200a36e(s32);
void Func_0200a376(s32, s32);
void Func_0200a476(s32);
void Func_0200a48e(s32, s32);

void ConfigureSceneActor9(void)
{
    Func_02006914();
    Func_0200695a(9);
    Func_02006984(9, 0, 0);
    Func_020069a4(9, 0);
    Func_020069d6(9, 0, 0);
    Func_02006a2a(9, 256, 0);
    Func_02002e3c(10);
    Func_02006954();
}

void ConfigureInteractionRegionA(void)
{
    Func_02007852(2, 82, 1, 2, 21, 81);
    Func_0200786c(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
    Func_02007886(2, 84, 1, 2, 6, 55);
    Func_020078a0(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
    Func_020078ba(2, 86, 1, 2, 27, 62);
    Func_020078d4(26, 16, 1, 1, 27, 17);
}

void ConfigurePrimaryInteractionRegions(void)
{
    Func_020079f6(5, 77, 1, 2, 17, 82);
    Func_02007a0a(5, 77, 1, 2, 3, 55);
    Func_02007a24(15, 33, 1, 1, 17, 35);
    Func_02007a36(3, 8, 1, 1, 3, 10);
}

void ConfigureSecondaryInteractionRegions(void)
{
    Func_02007a4e(8, 77, 1, 2, 17, 82);
    Func_02007a62(8, 77, 1, 2, 3, 55);
    Func_02007a7c(18, 35, 1, 1, 17, 35);
    Func_02007a8e(2, 10, 1, 1, 3, 10);
}

void ConfigureSceneActor26(void)
{
    Func_0200a30a(26, 1, 5);
    Func_0200a190(0x94e);
}

void ConfigureSceneActor14(void)
{
    Func_0200a254(14, 2);
    Func_0200a27a(0x2441);
    Func_0200a292(14, 0);
}

void ConfigureSceneActor13(void)
{
    Func_0200a274(13, 2);
    Func_0200a29a((s32)&Value_00002440);
    Func_0200a2b2(13, 0);
}

void ConfigureSceneActor12Variant(void)
{
    Func_0200a294(12, 2);
    Func_0200a2ba(0x243f);
    Func_0200a2d2(12, 0);
}

void ConfigureSceneActor18(void)
{
    Func_0200a2d2_a((s32)&Value_00002459);
    Func_0200a2f2(18, 0);
}

void ConfigureActor13Interaction(void)
{
    u8 *interaction_resources = &Value_0000256c;

    Func_0200a440((s32)interaction_resources);
    Func_0200a458(0x800d, 0);
    if (Func_0200a36e(234) != -1) {
        Func_0200a376((s32)(interaction_resources + 2), 1);
    }
}

void ConfigureActor13SceneResource(void)
{
    Func_0200a476(0x256d);
    Func_0200a48e(13, 0);
}
#undef ConfigureSceneActor9
#undef ConfigureInteractionRegionA
#undef ConfigureInteractionRegionB
#undef ConfigureInteractionRegionC
#undef ConfigurePrimaryInteractionRegions
#undef ConfigureSecondaryInteractionRegions
#undef ConfigureSceneActor26
#undef ConfigureSceneActor14
#undef ConfigureSceneActor13
#undef ConfigureSceneActor12Variant
#undef ConfigureSceneActor18
#undef ConfigureActor13Interaction
#undef ConfigureActor13SceneResource

/* map/locations/heidia/village/village_scene/messages.c */
#define ShowLockedDoorMessageOnce Func_02000f94
#define ShowLockedDoorMessage Func_02000fb4
#define ShowGeraldDontMessage Func_02000fc8
#define ShowThatWasExcitingMessage Func_02000fe4
#define ShowSorryIScaredYouMessage Func_02001000
#define ShowWellMessage Func_0200101c
extern s32 Value_00000953;
extern s32 Value_00000f2a;
extern s32 Value_00000f2b;
extern s32 Value_00000f2c;
extern s32 Value_00000f2d;

s32 Func_020065a2(s32);
void Func_020065aa(s32, s32);
void Func_020065bc(s32, s32);
s32 Func_02002008(s32, s32);
void Func_020065f2(s32);
s32 Func_02002024(s32, s32);
void Func_0200660e(s32);
s32 Func_02002040(s32, s32);
void Func_0200662a(s32);
s32 Func_0200205c(s32, s32);
void Func_02006646(s32);

void ShowLockedDoorMessageOnce(void)
{
    if (Func_020065a2(0xea) == -1) {
        Func_020065aa((s32)&Value_00000953, 1);
    }
}

void ShowLockedDoorMessage(void)
{
    Func_020065bc((s32)&Value_00000953, 1);
}

void ShowGeraldDontMessage(void)
{
    if (Func_02002008(8, 8) != 0) {
        Func_020065f2((s32)&Value_00000f2a);
    }
}

void ShowThatWasExcitingMessage(void)
{
    if (Func_02002024(9, 7) != 0) {
        Func_0200660e((s32)&Value_00000f2b);
    }
}

void ShowSorryIScaredYouMessage(void)
{
    if (Func_02002040(10, 6) != 0) {
        Func_0200662a((s32)&Value_00000f2c);
    }
}

void ShowWellMessage(void)
{
    if (Func_0200205c(11, 5) != 0) {
        Func_02006646((s32)&Value_00000f2d);
    }
}
#undef ShowLockedDoorMessageOnce
#undef ShowLockedDoorMessage
#undef ShowGeraldDontMessage
#undef ShowThatWasExcitingMessage
#undef ShowSorryIScaredYouMessage
#undef ShowWellMessage

/* map/locations/heidia/village/village_scene/no_op.c */
#define NoOpSceneCallbackA Func_02001098
#define NoOpSceneCallbackB Func_0200109c
#define NoOpSceneCallbackC Func_020010a0
#define NoOpSceneCallbackD Func_020010a4
#define NoOpInteractionCallback Func_02002528
#define NoOpActorCallback Func_02004d4c
void NoOpSceneCallbackA(void)
{
}

void NoOpSceneCallbackB(void)
{
}

void NoOpSceneCallbackC(void)
{
}

void NoOpSceneCallbackD(void)
{
}

void NoOpInteractionCallback(void)
{
}

void NoOpActorCallback(void)
{
}
#undef NoOpSceneCallbackA
#undef NoOpSceneCallbackB
#undef NoOpSceneCallbackC
#undef NoOpSceneCallbackD
#undef NoOpInteractionCallback
#undef NoOpActorCallback

/* map/locations/heidia/village/village_scene/object_inspection.c */
#define InspectOrdinaryObject Func_02001dc4
#define InspectEmptyChest Func_02001e2c
#define InspectEmptySceneObject Func_020020cc
#define InspectVillageWell Func_02002308
#define InspectWardrobe Func_020024c0
#define InspectFirewood Func_020024e4
#define InspectBooks Func_02002504
extern u8 Data_02000240[];

void Func_02007474(s32, s32);
void Func_0200757a(s32);
void Func_020074f6(s32, s32, s32);
void Func_020039c8(s32);
void Func_02007564(s32, s32);
void Func_020074ae(s32, s32, s32);
void Func_0200744a(void);
void Func_02007430(s32);
void Func_020074dc(s32, s32);
void Func_020075e2(s32);
void Func_0200755e(s32, s32, s32);
void Func_02003a30(s32);
void Func_020075cc(s32, s32);
void Func_02007516(s32, s32, s32);
void Func_020074b2(void);
void Func_02007498(s32);
void Func_0200777c(s32, s32);
void Func_02007882(s32);
void Func_020077fe(s32, s32, s32);
void Func_02003cd0(s32);
void Func_0200786c_a(s32, s32);
void Func_020077b6(s32, s32, s32);
void Func_02007752(void);
void Func_02007738(s32);
void Func_020045d8();
void Func_0200791c();
void Func_0200792c();
s32 Func_02007930();
void Func_02007932();
void Func_02007960();
void Func_0200797a();
void Func_0200798a();
void Func_02007990();
void Func_02007ada();
void Func_02007ade(s32);
void Func_02007aec(s32);
void Func_02007b76(s32, s32, s32);
void Func_02007b02(s32);
void Func_020047c2(void);
void Func_02007b98(s32, s32, s32);
void Func_02007b22(s32);
void Func_02007b32(s32);
void Func_02007bbc(s32, s32, s32);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void InspectOrdinaryObject(void)
{
    extern u8 *Data_03001ebc;

    Func_02007474(0, 1);
    Func_0200757a(113);
    Func_020074f6(15, 256, 60);
    Func_020039c8(15);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_02007564(98, 2);
    Func_020074ae(15, 0, 0);
    Func_0200744a();
    Func_02007430(2380);
}

void InspectEmptyChest(void)
{
    extern u8 *Data_03001ebc;

    Func_020074dc(0, 1);
    Func_020075e2(113);
    Func_0200755e(11, 256, 60);
    Func_02003a30(11);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_020075cc(98, 2);
    Func_02007516(11, 0, 0);
    Func_020074b2();
    Func_02007498(2377);
}

void InspectEmptySceneObject(void)
{
    extern u8 *Data_03001ebc;

    Func_0200777c(0, 1);
    Func_02007882(113);
    Func_020077fe(16, 256, 60);
    Func_02003cd0(16);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_0200786c_a(98, 2);
    Func_020077b6(16, 0, 0);
    Func_02007752();
    Func_02007738(2379);
}

void InspectVillageWell(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (*(s16 *)((*(u8 *volatile *)Data_03001ebc + 0xcb8)) != 0) {
        if (Value1(Func_02007930, 0x947) == 0) {
            Call2(Func_0200792c, 0x1528, 1);
            Func_02007ada(188);
            v5 = 17;
            v6 = 82;
            Func_02007960(1);
            Call6(Func_0200791c, 6, 77, 1, 2, v5, v6);
            Func_0200797a(5);
            Call6(Func_02007932, 7, 77, 1, 2, v5, v6);
            Func_02007990(1);
            Func_020045d8();
            Call1(Func_0200798a, 0x947);
        }
    }
}

void InspectWardrobe(void)
{
    Func_02007ade(2372);
    Func_02007aec(535);
    Func_02007b76(8, 0, 0);
}

void InspectFirewood(void)
{
    Func_02007b02(2373);
    Func_020047c2();
    Func_02007b98(9, 0, 0);
}

void InspectBooks(void)
{
    Func_02007b22(2374);
    Func_02007b32(536);
    Func_02007bbc(10, 0, 0);
}
#undef InspectOrdinaryObject
#undef InspectEmptyChest
#undef InspectEmptySceneObject
#undef InspectVillageWell
#undef InspectWardrobe
#undef InspectFirewood
#undef InspectBooks

/* map/locations/heidia/village/village_scene/scene_data.c */
#define SelectPrimarySceneData Func_020009dc
#define GetEmptySceneData Func_02000a30
#define SelectSecondarySceneData Func_02000a34
#define SelectTertiarySceneData Func_02000a80
#define SelectQuaternarySceneData Func_02000af8
#define SelectActor25SceneVariant Func_02004928
#define SelectActor24SceneVariant Func_02004964
extern s16 Data_02000240[];
extern u8 Value_000000a0;
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 Data_0200e2a4[];
extern u8 Data_0200e4b4[];
extern u8 Data_0200e754[];
extern u8 Data_0200e814[];
extern u8 Value_000000a3;
extern u8 Data_0200e910[];
extern u8 Data_0200e97c[];
extern u8 Data_0200e8a4[];
extern u8 Value_0000006a;
extern u8 Data_0200e9d0[];
extern u8 Data_0200ee08[];
extern u8 Data_0200ec28[];
extern u8 Data_0200eac0[];
extern u8 Data_0200ee98[];
extern u8 Data_0200e9b8[];
extern u8 Data_0200eff4[];
extern u8 Data_0200f258[];
extern u8 Data_0200f528[];
extern u8 Data_0200f63c[];

s32 Func_02009f3e(s32);
void Func_0200a020(s32);
void Func_0200a038(s32, s32);
void Func_0200a030(s32);
void Func_0200a048(s32, s32);
s32 Func_02009f7a(s32);
void Func_0200a05c(s32);
void Func_0200a074(s32, s32);
void Func_0200a06c(s32);
void Func_0200a084(s32, s32);

/* No scene data is available for this slot. */

s32 SelectPrimarySceneData(void)
{
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200e2a4;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e4b4;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200e754;
    }
    return (s32)Data_0200e814;
}

s32 GetEmptySceneData(void) { return 0; }

s32 SelectSecondarySceneData(void)
{
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e910;
    }
    if (scene_variant == (s32)&Value_000000a2 || scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200e97c;
    }
    return (s32)Data_0200e8a4;
}

s32 SelectTertiarySceneData(void)
{
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_0000006a) {
        return (s32)Data_0200e9d0;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200ee08;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200ec28;
    }
    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200eac0;
    }
    if (scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200ee98;
    }
    return (s32)Data_0200e9b8;
}

s32 SelectQuaternarySceneData(void)
{
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200eff4;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200f258;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200f528;
    }
    return (s32)Data_0200f63c;
}

void SelectActor25SceneVariant(void)
{
    if (Func_02009f3e(0x941)) {
        Func_0200a020(0x2568);
        Func_0200a038(25, 0);
    } else {
        Func_0200a030(0x2458);
        Func_0200a048(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    if (Func_02009f7a(0x941)) {
        Func_0200a05c(0x2569);
        Func_0200a074(24, 0);
    } else {
        Func_0200a06c(0x244e);
        Func_0200a084(24, 0);
    }
}
#undef SelectPrimarySceneData
#undef GetEmptySceneData
#undef SelectSecondarySceneData
#undef SelectTertiarySceneData
#undef SelectQuaternarySceneData
#undef SelectActor25SceneVariant
#undef SelectActor24SceneVariant

/* map/locations/heidia/village/village_scene/scene_scripts.c */
#define GameFlag_IsSet_1(a0) Value1(Func_02007dc6, a0)
#define GameFlag_Set_1(a0) Value1(Func_02007dde, a0)
#define Battle_Reset_1(args...) Func_02007dfa(args)
#define GameFlag_IsSet_2(a0) Value1(Func_02007de0, a0)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02007e3c, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02007e78, a0, a1, a2)
#define Motion_SetAngleToward_1(args...) Func_02007eca(args)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02007f22, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02007f1e, a0, a1, a2)
#define Motion_Launch_1(args...) Func_02007ed8(args)
#define Scene_GetRecord_1(args...) Func_02007e76(args)
#define Battle_WaitMode0_1(args...) Func_02007e5a(args)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02007e94, a0, a1, a2)
#define Motion_ResetPosMode2_1(a0, a1, a2) Value3(Func_02007ec8, a0, a1, a2)
#define Motion_CommitPos_1(args...) Func_02007ee6(args)
#define Battle_WaitMode0_2(args...) Func_02007e7c(args)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02007f68, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02007f74, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_02007f80, a0, a1, a2)
#define Motion_ArmCb_5(a0, a1, a2) Call3(Func_02007f8c, a0, a1, a2)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_02007f32, a0, a1, a2)
#define Motion_SetSpeedLim_1(a0, a1) Value2(Func_02007fce, a0, a1)
#define SceneWork_SetStepValue_1(args...) Func_02007f8e(args)
#define BattleEv_RunWait_1(args...) Func_02007fa6(args)
#define Motion_ResetPosMode2_2(a0, a1, a2) Value3(Func_02007f3c, a0, a1, a2)
#define Motion_CommitPos_2(args...) Func_02007f5a(args)
#define Motion_ArmCb_6(a0, a1, a2) Value3(Func_02007fd6, a0, a1, a2)
#define Battle_WaitMode0_3(args...) Func_02007efc(args)
#define Motion_OffsetPositionAndResetMotion_1(a0, a1, a2) Value3(Func_02007f70, a0, a1, a2)
#define Motion_CommitPos_3(args...) Func_02007f7e(args)
#define Battle_WaitMode0_4(args...) Func_02007f14(args)
#define Audio_PlayCue_1(args...) Func_0200809a(args)
#define UiText_DrawMessage_1(args...) Func_02007efa(args)
#define Motion_OffsetPositionAndResetMotion_2(a0, a1, a2) Value3(Func_02007f96, a0, a1, a2)
#define Battle_WaitMode0_5(args...) Func_02007f38(args)
#define Motion_SetVarCb_1(args...) Func_02007fd8(args)
#define Motion_SetVarCb_2(args...) Func_02007fe0(args)
#define Motion_SetVarCb_3(args...) Func_02007fe8(args)
#define Motion_SetVarCb_4(args...) Func_02007ff0(args)
#define Battle_WaitMode0_6(args...) Func_02007f5e(args)
#define Motion_SetAngleToward_2(args...) Func_02008016(args)
#define SceneWork_SetStepValue_2(args...) Func_02008034(args)
#define BattleEv_RunWait_2(args...) Func_0200804c(args)
#define Battle_WaitMode0_7(args...) Func_02007f94(args)
#define SharedWorkData_SetFirstAndSecondFields_1(args...) Func_020080e6_a(args)
#define BattleFx_ComputeWeightedResultAndDispatch_1(args...) Func_020080e6_b(args)
#define Object_SetModeById_1(args...) Func_0200805e(args)
#define Audio_PlayCue_2(args...) Func_02008164(args)
#define Battle_WaitMode0_8(args...) Func_02007fea(args)
#define SceneWork_SetStepValue_3(args...) Func_020080b2(args)
#define BattleEv_RunWait_3(args...) Func_020080ca(args)
#define Object_LinkPair_1(args...) Func_020080b4_a(args)
#define Battle_WaitMode0_9(args...) Func_0200800a(args)
#define Motion_ArmCb_7(a0, a1, a2) Call3(Func_020080f6, a0, a1, a2)
#define Motion_Launch_2(args...) Func_020080b0(args)
#define Scene_GetRecord_2(args...) Func_0200804e(args)
#define SceneWork_SetStepValue_4(args...) Func_020080f2(args)
#define BattleEv_RunWait_4(args...) Func_0200810a(args)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02008078, a0, a1, a2)
#define Motion_SetPosReset_2(a0, a1, a2) Call3(Func_020080b4_b, a0, a1, a2)
#define Motion_SetAngleToward_3(args...) Func_02008106(args)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_0200809c, a0, a1, a2)
#define Motion_ResetPosMode2_3(a0, a1, a2) Value3(Func_020080d0, a0, a1, a2)
#define Motion_CommitPos_4(args...) Func_020080ee(args)
#define Battle_WaitMode0_10(args...) Func_02008084(args)
#define SceneWork_SetStepValue_5(args...) Func_0200814a(args)
#define BattleEv_RunWait_5(args...) Func_02008162_a(args)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Value3(Func_0200818e, a0, a1, a2)
#define Battle_WaitMode0_11(args...) Func_020080a4(args)
#define Audio_PlayCue_3(args...) Func_0200822a(args)
#define SceneWork_SetStepValue_6(args...) Func_02008170(args)
#define BattleEv_RunWait_6(args...) Func_02008188_a(args)
#define Battle_WaitMode0_12(args...) Func_020080be(args)
#define Motion_CallWaitAnim_1(args...) Func_0200814e(args)
#define Battle_WaitMode0_13(args...) Func_020080cc(args)
#define Object_SetModeById_2(args...) Func_02008154_a(args)
#define Motion_SetVarCb_5(args...) Func_02008174_a(args)
#define Battle_WaitMode0_14(args...) Func_020080e2(args)
#define Motion_SetSpeed_5(a0, a1, a2) Call3(Func_0200811c, a0, a1, a2)
#define Motion_ResetPosMode2_4(a0, a1, a2) Value3(Func_02008150, a0, a1, a2)
#define Motion_CommitPos_5(args...) Func_0200816e(args)
#define Object_SetModeById_3(args...) Func_02008186(args)
#define Battle_WaitMode0_15(args...) Func_0200810c(args)
#define Motion_ArmCb_8(a0, a1, a2) Value3(Func_020081f8, a0, a1, a2)
#define Battle_WaitMode0_16(args...) Func_0200811e(args)
#define Motion_ArmCb_9(a0, a1, a2) Value3(Func_0200820a, a0, a1, a2)
#define Battle_WaitMode0_17(args...) Func_02008130(args)
#define Motion_SetAngleToward_4(args...) Func_020081e2(args)
#define Battle_WaitMode0_18(args...) Func_02008140(args)
#define SceneWork_SetStepValue_7(args...) Func_02008206(args)
#define BattleEv_RunWait_7(args...) Func_0200821e(args)
#define Battle_WaitMode0_19(args...) Func_02008154_b(args)
#define Motion_CallWaitAnim_2(args...) Func_020081e4(args)
#define Battle_WaitMode0_20(args...) Func_02008162_b(args)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Value3(Func_0200825e, a0, a1, a2)
#define Battle_WaitMode0_21(args...) Func_02008174_b(args)
#define SceneWork_SetStepValue_8(args...) Func_0200823a(args)
#define BattleEv_RunWait_8(args...) Func_02008252(args)
#define Battle_WaitMode0_22(args...) Func_02008188_b(args)
#define Motion_CallWaitAnim_3(args...) Func_02008218(args)
#define Battle_WaitMode0_23(args...) Func_02008196(args)
#define Motion_CallWaitAnim_4(args...) Func_02008226_a(args)
#define Battle_WaitMode0_24(args...) Func_020081a4(args)
#define Motion_ResetPosMode2_5(a0, a1, a2) Value3(Func_02008208, a0, a1, a2)
#define Motion_CommitPos_6(args...) Func_02008226_b(args)
#define Object_LinkPair_2(args...) Func_02008270(args)
#define Battle_WaitMode0_25(args...) Func_020081c6(args)
#define SceneWork_SetStepValue_9(args...) Func_0200828c(args)
#define BattleEv_RunWait_9(args...) Func_020082a4(args)
#define Battle_WaitMode0_26(args...) Func_020081da(args)
#define Object_SetModeById_4(args...) Func_02008262(args)
#define Battle_WaitMode0_27(args...) Func_020081e8(args)
#define SceneWork_SetStepValue_10(args...) Func_020082ae(args)
#define BattleEv_RunWait_10(args...) Func_020082c6(args)
#define Motion_SetHPosTerrain_2(a0, a1, a2) Value3(Func_0200827c, a0, a1, a2)
#define Audio_PlayCue_4(args...) Func_0200838a(args)
#define SceneWork_SetStepValue_11(args...) Func_020082d2(args)
#define BattleEv_RunWait_11(args...) Func_020082ea(args)
#define Motion_SetAngleToward_5(args...) Func_020082cc(args)
#define Motion_SetAngleToward_6(args...) Func_020082d6(args)
#define Motion_SetAngleToward_7(args...) Func_020082e0(args)
#define Battle_WaitMode0_28(args...) Func_0200823e(args)
#define Motion_ArmCb_10(a0, a1, a2) Call3(Func_0200832a, a0, a1, a2)
#define Motion_SetAngleToward_8(args...) Func_020082fc(args)
#define Battle_WaitMode0_29(args...) Func_0200825a(args)
#define Audio_PlayCue_5(args...) Func_020083e0_a(args)
#define Motion_SetSpeedLim_2(a0, a1) Call2(Func_02008374, a0, a1)
#define Motion_PlaceLookedUpObjectWithinCameraBounds_1(args...) Func_02008394(args)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_02008390(args)
#define Motion_SetSpeed_6(a0, a1, a2) Call3(Func_020082b2, a0, a1, a2)
#define Motion_ResetPosMode2_6(a0, a1, a2) Call3(Func_020082e8_a, a0, a1, a2)
#define Motion_SetPosReset_3(args...) Func_02008398(args)
#define Motion_CommitPos_7(args...) Func_0200830e(args)
#define Motion_SetPosReset_4(args...) Func_020083a6(args)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_020083a2, a0, a1, a2)
#define BattleFx_SpawnLinked_5(a0, a1, a2) Call3(Func_020083ae, a0, a1, a2)
#define BattleFx_SpawnLinked_6(a0, a1, a2) Call3(Func_020083ba, a0, a1, a2)
#define BattleFx_SpawnLinked_7(a0, a1, a2) Call3(Func_020083c6, a0, a1, a2)
#define BattleFx_SpawnLinked_8(a0, a1, a2) Value3(Func_020083d2, a0, a1, a2)
#define Battle_WaitMode0_30(args...) Func_020082e8_b(args)
#define Motion_SetAngleToward_9(args...) Func_0200839a(args)
#define Motion_SetVarCb_6(args...) Func_02008392(args)
#define Battle_WaitMode0_31(args...) Func_02008300(args)
#define SceneWork_SetStepValue_12(args...) Func_020083c8(args)
#define BattleEv_RunWait_12(args...) Func_020083e0_b(args)
#define Motion_SetAngleToward_10(args...) Func_020083c2(args)
#define SceneWork_SetStepValue_13(args...) Func_020083e2_a(args)
#define BattleEv_RunWait_13(args...) Func_020083fa(args)
#define Battle_WaitMode0_32(args...) Func_02008330(args)
#define Motion_SetAngleToward_11(args...) Func_020083e2_b(args)
#define Battle_WaitMode0_33(args...) Func_02008340(args)
#define SceneWork_SetStepValue_14(args...) Func_02008408(args)
#define BattleEv_RunWait_14(args...) Func_02008420(args)
#define Object_LinkPair_3(args...) Func_0200840a(args)
#define Object_LinkPair_4(args...) Func_02008414(args)
#define Battle_WaitMode0_34(args...) Func_0200836a(args)
#define Motion_SetAngleToward_12(args...) Func_0200841c(args)
#define Motion_SetAngleToward_13(args...) Func_02008426(args)
#define Motion_SetAngleToward_14(args...) Func_02008430(args)
#define Motion_SetAngleToward_15(args...) Func_0200843a(args)
#define Motion_SetVarCb_7(args...) Func_02008432(args)
#define Battle_WaitMode0_35(args...) Func_020083a0(args)
#define SceneWork_SetStepValue_15(args...) Func_02008468_a(args)
#define BattleEv_RunWait_15(args...) Func_02008480(args)
#define BattleFx_SpawnLinked_9(a0, a1, a2) Value3(Func_020084aa, a0, a1, a2)
#define Battle_WaitMode0_36(args...) Func_020083c0(args)
#define Object_SetModeById_5(args...) Func_02008448_a(args)
#define SceneWork_SetStepValue_16(args...) Func_02008490(args)
#define BattleEv_RunWait_16(args...) Func_020084a8(args)
#define Motion_ResetPosMode2_7(a0, a1, a2) Value3(Func_0200843c, a0, a1, a2)
#define Motion_CommitPos_8(args...) Func_02008478(args)
#define Motion_ArmCb_11(a0, a1, a2) Value3(Func_020084f4_a, a0, a1, a2)
#define SceneWork_SetStepValue_17(args...) Func_020084dc_a(args)
#define BattleEv_RunWait_17(args...) Func_020084f4_b(args)
#define Motion_ResetPosMode2_8(a0, a1, a2) Value3(Func_02008488, a0, a1, a2)
#define Motion_CommitPos_9(args...) Func_020084a6(args)
#define Motion_ArmCb_12(a0, a1, a2) Value3(Func_02008522_a, a0, a1, a2)
#define Battle_WaitMode0_37(args...) Func_02008448_b(args)
#define SceneWork_SetStepValue_18(args...) Func_02008510(args)
#define BattleEv_RunWait_18(args...) Func_02008528(args)
#define BattleFx_SpawnLinked_10(a0, a1, a2) Value3(Func_02008552, a0, a1, a2)
#define Battle_WaitMode0_38(args...) Func_02008468_b(args)
#define SceneWork_SetStepValue_19(args...) Func_02008530(args)
#define BattleEv_RunWait_19(args...) Func_02008548(args)
#define Motion_ResetPosMode2_9(a0, a1, a2) Value3(Func_020084dc_b, a0, a1, a2)
#define Motion_CommitPos_10(args...) Func_020084fa(args)
#define Motion_SetAngleToward_16(args...) Func_0200853c(args)
#define Motion_CallWaitAnim_5(args...) Func_02008524(args)
#define Battle_WaitMode0_39(args...) Func_020084a2(args)
#define SceneWork_SetStepValue_20(args...) Func_0200856a(args)
#define BattleEv_RunWait_20(args...) Func_02008582_a(args)
#define Object_SetModeById_6(args...) Func_0200853a(args)
#define SceneWork_SetStepValue_21(args...) Func_02008582_b(args)
#define BattleEv_RunWait_21(args...) Func_0200859a(args)
#define BattleFx_SpawnLinked_11(a0, a1, a2) Call3(Func_020085c6_a, a0, a1, a2)
#define BattleFx_SpawnLinked_12(a0, a1, a2) Call3(Func_020085d2, a0, a1, a2)
#define BattleFx_SpawnLinked_13(a0, a1, a2) Call3(Func_020085de, a0, a1, a2)
#define BattleFx_SpawnLinked_14(a0, a1, a2) Call3(Func_020085ea, a0, a1, a2)
#define BattleFx_SpawnLinked_15(a0, a1, a2) Value3(Func_020085f6, a0, a1, a2)
#define Battle_WaitMode0_40(args...) Func_0200850c(args)
#define SceneWork_SetStepValue_22(args...) Func_020085d4(args)
#define BattleEv_RunWait_22(args...) Func_020085ec(args)
#define Battle_WaitMode0_41(args...) Func_02008522_b(args)
#define BattleFx_SpawnLinked_16(a0, a1, a2) Value3(Func_0200861c, a0, a1, a2)
#define Battle_WaitMode0_42(args...) Func_02008532(args)
#define SceneWork_SetStepValue_23(args...) Func_020085fa_a(args)
#define BattleEv_RunWait_23(args...) Func_02008612_a(args)
#define Motion_SetVarCb_8(args...) Func_020085e2_a(args)
#define Battle_WaitMode0_43(args...) Func_02008550(args)
#define SceneWork_SetStepValue_24(args...) Func_02008618(args)
#define BattleEv_RunWait_24(args...) Func_02008630(args)
#define Motion_ResetPosMode2_10(a0, a1, a2) Value3(Func_020085c6_b, a0, a1, a2)
#define Motion_CommitPos_11(args...) Func_020085e4(args)
#define Motion_ArmCb_13(a0, a1, a2) Value3(Func_02008660, a0, a1, a2)
#define Battle_WaitMode0_44(args...) Func_02008586(args)
#define Motion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Func_020085fa_b, a0, a1, a2)
#define Motion_CommitPos_12(args...) Func_02008608(args)
#define Battle_WaitMode0_45(args...) Func_0200859e(args)
#define Audio_PlayCue_6(args...) Func_02008724(args)
#define UiText_DrawMessage_2(args...) Func_02008584(args)
#define Motion_OffsetPositionAndResetMotion_4(a0, a1, a2) Call3(Func_02008620, a0, a1, a2)
#define Motion_SetAngleToward_17(args...) Func_0200866a(args)
#define Audio_PlayCue_7(args...) Func_0200874c(args)
#define SceneWork_SetStepValue_25(args...) Func_02008694_a(args)
#define BattleEv_RunWait_25(args...) Func_020086ac(args)
#define Battle_WaitMode0_46(args...) Func_020085e2_b(args)
#define Motion_SetAngleToward_18(args...) Func_02008694_b(args)
#define Motion_SetAngleToward_19(args...) Func_0200869e(args)
#define Motion_SetAngleToward_20(args...) Func_020086a8(args)
#define Motion_SetAngleToward_21(args...) Func_020086b2(args)
#define Motion_SetAngleToward_22(args...) Func_020086bc(args)
#define GameFlag_Set_2(a0) Value1(Func_02008612_b, a0)
#define Battle_WaitMode0_47(args...) Func_02008636(args)
#define SharedWorkData_SetFirstAndSecondFields_2(args...) Func_02008788_a(args)
#define BattleFx_ComputeWeightedResultAndDispatch_2(args...) Func_02008788_b(args)
#define Battle_SchedShoulder_1(args...) Func_02008664(args)
#define STATE_TABLE_022B Data_02000240[0x22B]
#define SHARED_RECORD_FIELD_448 (*(u32 *)(*(u8 **)0x03001ebc + 448))
#define Scene_RunScene3bfSequenceA Func_0200169c
#define RunActor17SceneStep Func_02001708
#define TriggerSceneStage95FromActor12 Func_02001718
#define Scene_RunScene3bfSequenceB Func_0200175c
#define Scene_RunScene3bfSequenceC Func_020017bc
#define RunActor9ScriptedSequence Func_02001ab0
#define RunActorScriptedSequenceA Func_02001b24
#define RunActorScriptedSequenceB Func_02001c4c
#define RunActorScriptedSequenceC Func_02001ca4
#define RunActorScriptedSequenceD Func_02001d60
#define RunActor12InteractionSequence Func_02002134
#define RunSecondaryMapInteraction Func_0200238c
#define PlayStoryScene Func_020027b0
#define RunActor20SceneSequence Func_02004bfc

extern u8 Value_0000240d;
extern u8 Value_0000241e;
extern u8 Value_00002428;
extern u8 Value_0000247d;
extern u8 Value_00002464;
extern u8 Value_0000247e;
extern u8 Value_000000a3;
extern u8 Data_0000242e[];
extern u8 Data_00002430[];

s32 Func_02002d10();
s32 Func_02002fae();
s32 Func_02006cb8();
void Func_02006cec();
s32 Func_02006cf4_a();
s32 Func_02006cf4_b();
void Func_02003232(s32);
void Func_02006d4a(void);
s32 Func_02003006(s32);
void Func_02006cd4(s32);
s32 Func_02003054();
void Func_02006d28();
void Func_02006d2e();
s32 Func_02006d76();
void Func_02006da2();
s32 Func_020030b4();
void Func_02006d88();
void Func_02006d8e();
s32 Func_02006dd6();
void Func_02006e02();
void Func_020070e4(void);
void Func_0200714e_a();
void Func_0200714e_b();
void Func_02007126(s32, s32);
void Func_0200713c_a();
void Func_0200713c_b();
void Func_0200717c(s32, s32);
void Func_020071cc(s32);
void Func_020071e4(s32, s32);
void Func_02007212(s32, s32, s32);
void Func_020071e8(s32);
void Func_02007200(s32, s32);
void Func_02007266(s32);
void Func_0200728a(void);
void Func_0200715a(void);
void Func_0200715e(void);
void Func_02007252(s32, s32, s32);
void Func_020071d4(s32, s32, s32);
void Func_020071ac(s32, s32);
void Func_020071fc(s32, s32);
void Func_0200722e_a();
void Func_0200722e_b();
void Func_0200720e(s32, s32);
void Func_02007200_a(s32, s32, s32);
void Func_020071d8(s32, s32);
void Func_020071ee_a();
void Func_020071ee_b();
void Func_0200727e(s32);
void Func_02007296(s32, s32);
void Func_02007278(s32, s32, s32);
void Func_020072ce(s32, s32, s32);
void Func_020072a4(s32);
void Func_020072bc(s32, s32);
void Func_02007340(void);
void Func_020071f6(s32);
void Func_0200732c(s32);
void Func_02007210(void);
void Func_0200731c();
void Func_02007338();
void Func_02007344();
void Func_0200735c();
void Func_0200735e();
void Func_02007376_a();
void Func_02007376_b();
void Func_02007388();
void Func_0200738e();
void Func_02007396(s32);
void Func_020073ae(s32, s32);
void Func_02007386_a(s32, s32);
void Func_02007386_b(s32, s32);
void Func_020073ac(s32);
void Func_020073c4_a();
void Func_020073c4_b();
void Func_020073dc(s32, s32);
void Func_02007452(s32);
void Func_0200746a(s32, s32);
void Func_0200742a(s32, s32);
void Func_02007468(s32);
void Func_02007480(s32, s32);
void Func_02007458_a(s32, s32);
void Func_02007458_b(s32, s32);
void Func_0200747e(s32);
void Func_02007496_a();
void Func_02007496_b();
void Func_020074ae_a(s32, s32);
void Func_02007768(void);
void Func_020077d2(s32, s32, s32);
void Func_020077aa(s32, s32);
void Func_020077fa(s32, s32);
void Func_0200782c(s32, s32, s32);
void Func_0200790a(s32);
void Func_02007886_a(s32, s32, s32);
void Func_0200785e(s32);
void Func_02007876(s32, s32);
void Func_020078a4(s32, s32, s32);
void Func_0200787a(s32);
void Func_02007892(s32, s32);
void Func_02007916(void);
void Func_020077cc(s32);
void Func_02007902(s32);
void Func_020077e6(void);
void Func_020077ce(s32);
void Func_02004690();
void Func_020079a0();
void Func_020079b0();
s32 Func_020079b4();
void Func_020079b6();
void Func_020079e4();
void Func_020079fe();
void Func_02007a0e();
void Func_02007a14();
void Func_02007b5e();
void Func_02004d2a();
void Func_02004f02();
void Func_02004f66();
void Func_02004ff4();
void Func_02005054();
void Func_02005596();
void Func_02005690();
void Func_02005708();
void Func_020057ec();
s32 Func_02007dc6();
void Func_02007dde();
s32 Func_02007de0();
void Func_02007dfa();
void Func_02007e14();
void Func_02007e3c();
void Func_02007e5a();
s32 Func_02007e76();
void Func_02007e78();
void Func_02007e7c();
void Func_02007e94();
void Func_02007ec8();
void Func_02007eca();
void Func_02007ed8();
void Func_02007ee6();
void Func_02007efa();
void Func_02007efc();
void Func_02007f14();
void Func_02007f1e();
void Func_02007f22();
void Func_02007f32();
void Func_02007f38();
void Func_02007f3c();
void Func_02007f5a();
void Func_02007f5e();
void Func_02007f68();
void Func_02007f70();
void Func_02007f74();
void Func_02007f7e();
void Func_02007f80();
void Func_02007f8c();
void Func_02007f8e();
void Func_02007f94();
void Func_02007f96();
void Func_02007fa6();
void Func_02007fce();
void Func_02007fd6();
void Func_02007fd8();
void Func_02007fe0();
void Func_02007fe8();
void Func_02007fea();
void Func_02007fec();
void Func_02007ff0();
void Func_0200800a();
void Func_02008016();
void Func_02008034();
void Func_0200804c();
s32 Func_0200804e();
void Func_0200805e();
void Func_02008078();
void Func_02008084();
void Func_0200809a();
void Func_0200809c();
void Func_020080a4();
void Func_020080b0();
void Func_020080b2();
void Func_020080b4_a();
void Func_020080b4_b();
void Func_020080be();
void Func_020080ca();
void Func_020080cc();
void Func_020080d0();
void Func_020080e2();
void Func_020080e6_a();
void Func_020080e6_b();
void Func_020080ee();
void Func_020080f2();
void Func_020080f6();
void Func_02008106();
void Func_0200810a();
void Func_0200810c();
void Func_0200811c();
void Func_0200811e();
void Func_02008130();
void Func_02008140();
void Func_0200814a();
void Func_0200814e();
void Func_02008150();
void Func_02008154_a();
void Func_02008154_b();
void Func_02008162_a();
void Func_02008162_b();
void Func_02008164();
void Func_0200816e();
void Func_02008170();
void Func_02008174_a();
void Func_02008174_b();
void Func_02008186();
void Func_02008188_a();
void Func_02008188_b();
void Func_0200818e();
void Func_02008196();
void Func_020081a4();
void Func_020081c6();
void Func_020081da();
void Func_020081e2();
void Func_020081e4();
void Func_020081e8();
void Func_020081f8();
void Func_02008206();
void Func_02008208();
void Func_0200820a();
void Func_02008218();
void Func_0200821e();
void Func_02008226_a();
void Func_02008226_b();
void Func_0200822a();
void Func_0200823a();
void Func_0200823e();
void Func_02008252();
void Func_0200825a();
void Func_0200825e();
void Func_02008262();
void Func_02008270();
void Func_0200827c();
void Func_0200828c();
void Func_020082a4();
void Func_020082ae();
void Func_020082b2();
void Func_020082c6();
void Func_020082cc();
void Func_020082d2();
void Func_020082d6();
void Func_020082e0();
void Func_020082e8_a();
void Func_020082e8_b();
void Func_020082ea();
void Func_020082fc();
void Func_02008300();
void Func_0200830e();
void Func_0200832a();
void Func_02008330();
void Func_02008340();
void Func_0200836a();
void Func_02008374();
void Func_0200838a();
void Func_02008390();
void Func_02008392();
void Func_02008394();
void Func_02008398();
void Func_0200839a();
void Func_020083a0();
void Func_020083a2();
void Func_020083a6();
void Func_020083ae();
void Func_020083ba();
void Func_020083c0();
void Func_020083c2();
void Func_020083c6();
void Func_020083c8();
void Func_020083d2();
void Func_020083e0_a();
void Func_020083e0_b();
void Func_020083e2_a();
void Func_020083e2_b();
void Func_020083fa();
void Func_02008408();
void Func_0200840a();
void Func_02008414();
void Func_0200841c();
void Func_02008420();
void Func_02008426();
void Func_02008430();
void Func_02008432();
void Func_0200843a();
void Func_0200843c();
void Func_02008448_a();
void Func_02008448_b();
void Func_02008468_a();
void Func_02008468_b();
void Func_02008478();
void Func_02008480();
void Func_02008488();
void Func_02008490();
void Func_020084a2();
void Func_020084a6();
void Func_020084a8();
void Func_020084aa();
void Func_020084dc_a();
void Func_020084dc_b();
void Func_020084f4_a();
void Func_020084f4_b();
void Func_020084fa();
void Func_0200850c();
void Func_02008510();
void Func_02008522_a();
void Func_02008522_b();
void Func_02008524();
void Func_02008528();
void Func_02008530();
void Func_02008532();
void Func_0200853a();
void Func_0200853c();
void Func_02008548();
void Func_02008550();
void Func_02008552();
void Func_0200856a();
void Func_02008582_a();
void Func_02008582_b();
void Func_02008584();
void Func_02008586();
void Func_0200859a();
void Func_0200859e();
void Func_020085c6_a();
void Func_020085c6_b();
void Func_020085d2();
void Func_020085d4();
void Func_020085de();
void Func_020085e2_a();
void Func_020085e2_b();
void Func_020085e4();
void Func_020085ea();
void Func_020085ec();
void Func_020085f6();
void Func_020085fa_a();
void Func_020085fa_b();
void Func_02008608();
void Func_02008612_a();
void Func_02008612_b();
void Func_02008618();
void Func_0200861c();
void Func_02008620();
void Func_02008630();
void Func_02008636();
void Func_02008660();
void Func_02008664();
void Func_0200866a();
void Func_02008694_a();
void Func_02008694_b();
void Func_0200869e();
void Func_020086a8();
void Func_020086ac();
void Func_020086b2();
void Func_020086bc();
void Func_02008724();
void Func_0200874c();
void Func_02008788_a();
void Func_02008788_b();
s32 Func_0200a212();
s32 Func_0200a23a();
void Func_0200a24a();
void Func_0200a272();
void Func_0200a2a4();
void Func_0200a2ac();
void Func_0200a2ae();
void Func_0200a2b2_a();
void Func_0200a2ea();
void Func_0200a2f0();
void Func_0200a2f4_a();
void Func_0200a2f4_b();
void Func_0200a300();
s32 Func_0200a302();
void Func_0200a30c();
void Func_0200a324();
void Func_0200a32c();
void Func_0200a33a();
void Func_0200a352();
void Func_0200a356();
void Func_0200a36e_a();
void Func_0200a37a();
void Func_0200a380();
void Func_0200a392_a();
void Func_0200a392_b();
s32 Func_0200a3a2();
void Func_0200a3ac();
void Func_0200a3bc_a();
void Func_0200a3bc_b();
void Func_0200a3c2();
void Func_0200a3d2();

/*
 * Actor script -- resource_3bf.  It walks a run of consecutive resource
 * identifiers starting at 0x241e, re-supplying the actor handle to every
 * service that takes one, and returns nothing.
 */

/*
 * Symbol names use the offsets as printed while absolute data addresses keep
 * their raw pool values; the module's own pool constants sit 0x8000 above
 * the printed offsets.  Func_02007376 is reached with two different argument
 * shapes -- once as (handle, 0) and once as (identifier) -- so these services
 * keep old-style declarations rather than an invented common prototype.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Byte 0x22B of Data_02000240, a flag/state table indexed by other code. */

/* Word at +448 of the record pointed to by the global at 0x03001ebc. */

static __inline__ s32 Value0(s32 (*f)())
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    return f(a0);
}

/*
 * Keeping this call in its own helper preserves the interleaving of the
 * handle move with the two-instruction constant.
 */
static __inline__ void Call_02007388(s32 handle)
{
    Func_02007388(handle, 258, 60);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200238c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_02000240[];

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02004bfc(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

#if defined(GS1_EDITION_JA)
#define Value_00002421 Value_000025aa
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define Value_00002421 Value_00002403
#endif

extern u8 Value_00002421;

void Scene_RunScene3bfSequenceA(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1(Func_02006cb8, 0x214) == 0) {
        if (Value0(Func_02002d10) == 0) {
            if (Data_02000240[294] == 0) {
                if (Value1(Func_02002fae, 17) != 0) {
                    Call1(Func_02006cec, 0x215);
                    Call1(Func_02006cf4_a, 0x214);
                }
            }
            if (Value1(Func_02006cf4_b, 0x214) != 0) {
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 94;

                    *target = shown;
                }
            }
        }
    }
}

void RunActor17SceneStep(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    Func_02003232(17);
    Func_02006d4a();
}

void TriggerSceneStage95FromActor12(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *scene_state = Data_03001ebc;

    if (Func_02003006(12) != 0 && Data_02000240[294] == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Func_02006cd4(0x02009719);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 95;
        *scene_stage = next_stage;
    }
}

void Scene_RunScene3bfSequenceB(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1(Func_02006d76, 0x225) == 0) {
        if (Value1(Func_02003054, 13) != 0) {
            if (Data_02000240[294] == 0) {
                Call1(Func_02006da2, 0x225);
                Call1(Func_02006d28, 0x200975d);
                Call1(Func_02006d2e, 0x20097bd);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
            }
        }
    }
}

void Scene_RunScene3bfSequenceC(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *p5;

    p5 = *(volatile s32 *)&Data_03001ebc;
    if (Value1(Func_02006dd6, 0x225) == 0) {
        if (Value1(Func_020030b4, 21) != 0) {
            if (Data_02000240[294] == 0) {
                Call1(Func_02006e02, 0x225);
                Call1(Func_02006d88, 0x20097bd);
                Call1(Func_02006d8e, 0x200975d);
                {
                    volatile u16 *target = (volatile u16 *)(((s32)p5 + 0x182));
                    s32 shown = 96;

                    *target = shown;
                }
            }
        }
    }
}

void RunActor9ScriptedSequence(void)
{
    Func_020070e4();
    Func_0200714e_a(9, 0, 0);
    Func_02007126(9, 1);
    Func_0200713c_a(9);
    Func_0200717c(9, 0);
    Func_0200713c_b(0, 1);
    {
        u8 *t = &Value_0000240d;

        Func_020071cc((s32)t);
        Func_020071e4(9, 0);
        Func_02007212(0, 258, 60);
        Func_020071e8((s32)(t + 1));
    }
    Func_02007200(9, 0);
    Func_02007266(60);
    Func_0200728a();
    Func_0200714e_b();
}

void RunActorScriptedSequenceA(s32 actor_id)
{
    Func_0200715a();
    Func_0200715e();
    Func_02007252(actor_id, 256, 1);
    Func_020071d4(actor_id, 0, 0);
    Func_020071ac(actor_id, 1);
    Func_020071fc(actor_id, 0);
    Func_0200722e_a(actor_id, 0, 0);
    Func_0200720e(0, 1);
    Func_02007200_a(actor_id, 0, 0);
    Func_020071d8(actor_id, 1);
    Func_020071ee_a(actor_id);
    Func_0200722e_b(actor_id, 0);
    Func_020071ee_b(0, 1);
    {
        u8 *t = &Value_0000240d;

        Func_0200727e((s32)t);
        Func_02007296(actor_id, 0);
        Func_02007278(0, actor_id, 0);
        Func_020072ce(0, 258, 60);
        Func_020072a4((s32)(t + 1));
    }
    Func_020072bc(actor_id, 0);
    Func_02007340();
    Func_020071f6(60);
    Func_0200732c(60);
    Func_02007210();
}

/*
 * The resource run is taken as the address of Value_0000241e rather than as
 * an integer constant, which preserves its pointer identity and materialises
 * it after the first call.
 */
void RunActorScriptedSequenceB(s32 handle)
{
    u8 *id;

    Func_0200731c(handle, 1);
    id = &Value_0000241e;
    Func_02007344((s32)id);
    Func_0200735c(handle, 0);
    Call_02007388(handle);
    Func_0200735e((s32)(id + 1));
    Func_02007376_a(handle, 0);
    id += 2;
    Func_02007338(handle, 4);
    Func_02007376_b((s32)id);
    Func_0200738e(handle, 0);
}

void RunActorScriptedSequenceC(s32 actor_id)
{
    u8 *t = &Value_00002421;

    Func_02007396((s32)t);
    Func_020073ae(actor_id, 0);
    Func_02007386_a(actor_id, 1);
    Func_020073ac((s32)(t + 1));
    Func_020073c4_a(actor_id, 0);
    Func_02007386_b(actor_id, 4);
    Func_020073c4_b((s32)(t + 2));
    Func_020073dc(actor_id, 0);
}

void RunActorScriptedSequenceD(s32 actor_id)
{
    u8 *t = &Value_00002428;

    Func_02007452((s32)t);
    Func_0200746a(actor_id, 0);
    Func_0200742a(actor_id, 4);
    Func_02007468((s32)(t + 1));
    Func_02007480(actor_id, 0);
    Func_02007458_a(actor_id, 1);
    Func_0200747e((s32)(t + 2));
    Func_02007496_a(actor_id, 0);
    Func_02007458_b(actor_id, 3);
    Func_02007496_b((s32)(t + 3));
    Func_020074ae_a(actor_id, 0);
}

void RunActor12InteractionSequence(void)
{
    Func_02007768();
    Func_020077d2(0, 0, 0);
    Func_020077aa(0, 1);
    Func_020077fa(0, 1);
    Func_0200782c(12, 0, 0);
    Func_0200790a(113);
    Func_02007886_a(12, 256, 60);
    {
        u8 *t = &Value_0000240d;

        Func_0200785e((s32)t);
        Func_02007876(12, 0);
        Func_020078a4(0, 258, 50);
        Func_0200787a((s32)(t + 1));
    }
    Func_02007892(12, 0);
    Func_02007916();
    Func_020077cc(60);
    Func_02007902(60);
    Func_020077e6();
    Func_020077ce(548);
}

void RunSecondaryMapInteraction(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (*(s16 *)((*(u8 *volatile *)Data_03001ebc + 0xcb8)) != 0) {
        if (Value1(Func_020079b4, 0x948) == 0) {
            Call2(Func_020079b0, 0x1528, 1);
            Func_02007b5e(188);
            v5 = 3;
            v6 = 55;
            Func_020079e4(1);
            Call6(Func_020079a0, 6, 77, 1, 2, v5, v6);
            Func_020079fe(5);
            Call6(Func_020079b6, 7, 77, 1, 2, v5, v6);
            Func_02007a14(1);
            Func_02004690();
            Call1_0200238c(Func_02007a0e, 0x948);
        }
    }
}

/* Runs one of two long fixed sequences (chosen by a status check), each
 * placing actors, panning/animating them, and running dialogue lines built
 * from a base text pointer plus a running line offset. */
void PlayStoryScene(void)
{
    extern u8 Data_02000240[];

    s32 text_line;

    if (GameFlag_IsSet_1(769) != 0) {
        Func_020057ec();
    }
    GameFlag_Set_1(624);
    Battle_Reset_1();
    if (GameFlag_IsSet_2(2370) != 0) {
        Motion_SetSpeed_1(0, 32768, 16384);
        Motion_SetPosReset_1(0, 456, 216);
        Motion_SetAngleToward_1(0, 12, 0);
        Func_02004d2a();
        BattleFx_SpawnLinked_1(12, 256, 60);
        Motion_ArmCb_1(12, 32768, 0);
        Motion_Launch_1(12, 4, 0);
        Func_02007e14(Scene_GetRecord_1(12), 1);
        Battle_WaitMode0_1(30);
        Motion_SetSpeed_2(2, 45875, 22937);
        Motion_ResetPosMode2_1(2, 464, 192);
        Motion_CommitPos_1(2);
        Battle_WaitMode0_2(30);
        Motion_ArmCb_2(0, 16384, 0);
        Motion_ArmCb_3(2, 16384, 0);
        Motion_ArmCb_4(1, 16384, 0);
        Motion_ArmCb_5(3, 16384, 0);
        Motion_SetHPosTerrain_1(13, 29884416, 20971520);
        Motion_SetSpeedLim_1(131072, 16384);
        text_line = (s32)&Value_0000247d;
        SceneWork_SetStepValue_1(text_line);
        BattleEv_RunWait_1(13, 0);
        Motion_ResetPosMode2_2(13, 458, 272);
        Motion_CommitPos_2(13);
        Motion_ArmCb_6(13, 20480, 0);
        Battle_WaitMode0_3(40);
        Motion_OffsetPositionAndResetMotion_1(13, -8, 8);
        Motion_CommitPos_3(13);
        Battle_WaitMode0_4(60);
        Audio_PlayCue_1(155);
        UiText_DrawMessage_1(text_line + 1, 1);
        Motion_OffsetPositionAndResetMotion_2(13, 8, -8);
        Func_02004f02();
        Battle_WaitMode0_5(120);
        Motion_SetVarCb_1(0, 2);
        Motion_SetVarCb_2(2, 2);
        Motion_SetVarCb_3(1, 2);
        Motion_SetVarCb_4(3, 2);
        Battle_WaitMode0_6(20);
        text_line += 2;
        Func_02005054();
        Motion_SetAngleToward_2(13, 0, 0);
        SceneWork_SetStepValue_2(text_line);
        BattleEv_RunWait_2(13, 0);
        Func_02004ff4();
        SHARED_RECORD_FIELD_448 = 512;
        Battle_WaitMode0_7(1);
        do {
            STATE_TABLE_022B = 3;
        } while (0);
        SharedWorkData_SetFirstAndSecondFields_1((s32)&Value_000000a3, 4);
        BattleFx_ComputeWeightedResultAndDispatch_1(98, 4);
    } else {
        Object_SetModeById_1(0, 1);
        Audio_PlayCue_2(17);
        Battle_WaitMode0_8(30);
        text_line = (s32)&Value_00002464;
        SceneWork_SetStepValue_3(text_line);
        BattleEv_RunWait_3(12, 0);
        Object_LinkPair_1(0, 12, 0);
        Battle_WaitMode0_9(140);
        Motion_ArmCb_7(12, 32768, 0);
        Motion_Launch_2(12, 4, 0);
        Func_02007fec(Scene_GetRecord_2(12), 1);
        SceneWork_SetStepValue_4(text_line + 1);
        BattleEv_RunWait_4(12, 0);
        Motion_SetSpeed_3(0, 32768, 16384);
        Motion_SetPosReset_2(0, 456, 216);
        Motion_SetAngleToward_3(0, 12, 0);
        Func_02004f66();
        Motion_SetSpeed_4(2, 45875, 22937);
        Motion_ResetPosMode2_3(2, 464, 192);
        Motion_CommitPos_4(2);
        Battle_WaitMode0_10(30);
        SceneWork_SetStepValue_5(text_line + 2);
        BattleEv_RunWait_5(2, 0);
        BattleFx_SpawnLinked_2(12, 256, 0);
        Battle_WaitMode0_11(110);
        Audio_PlayCue_3(60);
        SceneWork_SetStepValue_6(text_line + 3);
        BattleEv_RunWait_6(12, 0);
        Battle_WaitMode0_12(30);
        Motion_CallWaitAnim_1(2, 3);
        Battle_WaitMode0_13(20);
        Object_SetModeById_2(2, 1);
        Motion_SetVarCb_5(12, 1);
        Battle_WaitMode0_14(20);
        Motion_SetSpeed_5(12, 26214, 13107);
        Motion_ResetPosMode2_4(12, 520, 208);
        Motion_CommitPos_5(12);
        Object_SetModeById_3(12, 1);
        Battle_WaitMode0_15(20);
        Motion_ArmCb_8(12, 45056, 0);
        Battle_WaitMode0_16(30);
        Motion_ArmCb_9(12, 20480, 0);
        Battle_WaitMode0_17(30);
        Motion_SetAngleToward_4(12, 2, 0);
        Battle_WaitMode0_18(20);
        SceneWork_SetStepValue_7(text_line + 4);
        BattleEv_RunWait_7(12, 0);
        Battle_WaitMode0_19(40);
        Motion_CallWaitAnim_2(2, 3);
        Battle_WaitMode0_20(20);
        BattleFx_SpawnLinked_3(12, 264, 0);
        Battle_WaitMode0_21(120);
        SceneWork_SetStepValue_8(text_line + 5);
        BattleEv_RunWait_8(12, 0);
        Battle_WaitMode0_22(25);
        Motion_CallWaitAnim_3(2, 3);
        Battle_WaitMode0_23(30);
        Motion_CallWaitAnim_4(12, 3);
        Battle_WaitMode0_24(40);
        Motion_ResetPosMode2_5(2, 480, 200);
        Motion_CommitPos_6(2);
        Object_LinkPair_2(2, 12, 0);
        Battle_WaitMode0_25(60);
        SceneWork_SetStepValue_9(text_line + 6);
        BattleEv_RunWait_9(2, 0);
        Battle_WaitMode0_26(20);
        Object_SetModeById_4(12, 4);
        Battle_WaitMode0_27(80);
        SceneWork_SetStepValue_10(text_line + 7);
        BattleEv_RunWait_10(12, 0);
        Motion_SetHPosTerrain_2(13, 29884416, 20971520);
        Audio_PlayCue_4(19);
        SceneWork_SetStepValue_11(text_line + 8);
        BattleEv_RunWait_11(13, 0);
        Motion_SetAngleToward_5(0, 13, 0);
        Motion_SetAngleToward_6(2, 13, 0);
        Motion_SetAngleToward_7(1, 13, 0);
        Battle_WaitMode0_28(5);
        Motion_ArmCb_10(3, 16384, 0);
        Motion_SetAngleToward_8(12, 13, 0);
        Battle_WaitMode0_29(30);
        Audio_PlayCue_5(61);
        Motion_SetSpeedLim_2(131072, 16384);
        Motion_PlaceLookedUpObjectWithinCameraBounds_1(13, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        Motion_SetSpeed_6(13, 52428, 26214);
        Motion_ResetPosMode2_6(13, 456, 304);
        Motion_SetPosReset_3(13, 1);
        Motion_CommitPos_7(13);
        Motion_SetPosReset_4(1, 1);
        BattleFx_SpawnLinked_4(0, 258, 0);
        BattleFx_SpawnLinked_5(2, 258, 0);
        BattleFx_SpawnLinked_6(1, 258, 0);
        BattleFx_SpawnLinked_7(3, 258, 0);
        BattleFx_SpawnLinked_8(12, 258, 0);
        Battle_WaitMode0_30(60);
        Motion_SetAngleToward_9(12, 13, 0);
        Motion_SetVarCb_6(12, 2);
        Battle_WaitMode0_31(60);
        SceneWork_SetStepValue_12(text_line + 9);
        BattleEv_RunWait_12(12, 0);
        Motion_SetAngleToward_10(13, 12, 0);
        SceneWork_SetStepValue_13(text_line + 10);
        BattleEv_RunWait_13(13, 0);
        Battle_WaitMode0_32(60);
        Motion_SetAngleToward_11(13, 2, 0);
        Battle_WaitMode0_33(30);
        SceneWork_SetStepValue_14(text_line + 11);
        BattleEv_RunWait_14(13, 0);
        Object_LinkPair_3(3, 2, 0);
        Object_LinkPair_4(0, 1, 0);
        Battle_WaitMode0_34(60);
        Motion_SetAngleToward_12(0, 13, 0);
        Motion_SetAngleToward_13(2, 13, 0);
        Motion_SetAngleToward_14(1, 13, 0);
        Motion_SetAngleToward_15(3, 13, 0);
        Motion_SetVarCb_7(13, 1);
        Battle_WaitMode0_35(60);
        SceneWork_SetStepValue_15(text_line + 12);
        BattleEv_RunWait_15(13, 0);
        BattleFx_SpawnLinked_9(1, 259, 0);
        Battle_WaitMode0_36(60);
        Object_SetModeById_5(13, 4);
        SceneWork_SetStepValue_16(text_line + 13);
        BattleEv_RunWait_16(13, 0);
        Motion_ResetPosMode2_7(1, 456, 248);
        Motion_CommitPos_8(1);
        Motion_ArmCb_11(1, 16384, 0);
        SceneWork_SetStepValue_17(text_line + 14);
        BattleEv_RunWait_17(1, 0);
        Motion_ResetPosMode2_8(2, 472, 216);
        Motion_CommitPos_9(2);
        Motion_ArmCb_12(2, 16384, 0);
        Battle_WaitMode0_37(10);
        SceneWork_SetStepValue_18(text_line + 15);
        BattleEv_RunWait_18(2, 0);
        BattleFx_SpawnLinked_10(12, 261, 0);
        Battle_WaitMode0_38(60);
        SceneWork_SetStepValue_19(text_line + 16);
        BattleEv_RunWait_19(12, 0);
        Motion_ResetPosMode2_9(3, 440, 216);
        Motion_CommitPos_10(3);
        Motion_SetAngleToward_16(3, 13, 0);
        Motion_CallWaitAnim_5(3, 3);
        Battle_WaitMode0_39(10);
        SceneWork_SetStepValue_20(text_line + 17);
        BattleEv_RunWait_20(3, 0);
        Object_SetModeById_6(13, 4);
        SceneWork_SetStepValue_21(text_line + 18);
        BattleEv_RunWait_21(13, 0);
        BattleFx_SpawnLinked_11(0, 258, 0);
        BattleFx_SpawnLinked_12(1, 258, 0);
        BattleFx_SpawnLinked_13(3, 258, 0);
        BattleFx_SpawnLinked_14(2, 258, 0);
        BattleFx_SpawnLinked_15(13, 264, 0);
        Battle_WaitMode0_40(60);
        SceneWork_SetStepValue_22(text_line + 19);
        BattleEv_RunWait_22(13, 0);
        Battle_WaitMode0_41(20);
        BattleFx_SpawnLinked_16(1, 259, 0);
        Battle_WaitMode0_42(60);
        SceneWork_SetStepValue_23(text_line + 20);
        BattleEv_RunWait_23(1, 0);
        Motion_SetVarCb_8(13, 1);
        Battle_WaitMode0_43(60);
        SceneWork_SetStepValue_24(text_line + 21);
        BattleEv_RunWait_24(13, 0);
        Motion_ResetPosMode2_10(13, 456, 280);
        Motion_CommitPos_11(13);
        Motion_ArmCb_13(13, 20480, 0);
        Battle_WaitMode0_44(80);
        Motion_OffsetPositionAndResetMotion_3(13, -8, 8);
        Motion_CommitPos_12(13);
        Battle_WaitMode0_45(60);
        Audio_PlayCue_6(155);
        UiText_DrawMessage_2((s32)&Value_0000247e, 1);
        Motion_OffsetPositionAndResetMotion_4(13, 8, -8);
        Motion_SetAngleToward_17(13, 11, 0);
        Func_02005596();
        Audio_PlayCue_7(52);
        SceneWork_SetStepValue_25(text_line + 23);
        BattleEv_RunWait_25(13, 0);
        Battle_WaitMode0_46(60);
        Motion_SetAngleToward_18(0, 11, 0);
        Motion_SetAngleToward_19(1, 11, 0);
        Motion_SetAngleToward_20(2, 11, 0);
        Motion_SetAngleToward_21(3, 11, 0);
        Motion_SetAngleToward_22(12, 11, 0);
        Func_02005708();
        Func_02005690();
        GameFlag_Set_2(2370);
        SHARED_RECORD_FIELD_448 = 512;
        Battle_WaitMode0_47(1);
        do {
            STATE_TABLE_022B = 3;
        } while (0);
        SharedWorkData_SetFirstAndSecondFields_2((s32)&Value_000000a3, 4);
        BattleFx_ComputeWeightedResultAndDispatch_2(98, 4);
    }
    Battle_SchedShoulder_1();
}

void RunActor20SceneSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 base5_242e;
    s32 base5_2430;

    if (Value1(Func_0200a212, 0x226) != 0) {
        Call1_02004bfc(Func_0200a2f4_a, 0x2434);
        Func_0200a30c(20, 0);
    } else {
        Func_0200a24a();
        Func_0200a2f4_b(20, 0, 0);
        if (Value1(Func_0200a23a, 0x227) == 0) {
            Func_0200a2f0(20, 4, 0);
            Func_0200a2ae(20);
            Func_0200a2ac(20);
            Func_0200a272(20);
            base5_242e = (s32)Data_0000242e;
            Func_0200a33a(base5_242e);
            Func_0200a352(20, 0);
            Call3(Func_0200a380, 20, 0x102, 30);
            Func_0200a356((base5_242e + 1));
            Func_0200a36e_a(20, 0);
            Func_0200a2a4(30);
            Func_0200a32c(20, 4);
            Func_0200a2b2_a(30);
        }
        base5_2430 = (s32)Data_00002430;
        Func_0200a37a(base5_2430);
        Func_0200a392_a(20, 0);
        Call3(Func_0200a3bc_a, 20, 0x101, 40);
        Func_0200a392_b((base5_2430 + 1));
        Value2(Func_0200a3a2, 20, 0);
        if (Value2(Func_0200a302, 0, 0) == 0) {
            Func_0200a3ac((base5_2430 + 2));
            Func_0200a3bc_b(20, 0);
            Call1_02004bfc(Func_0200a2ea, 0x226);
        } else {
            Func_0200a3c2((base5_2430 + 3));
            Func_0200a3d2(20, 0);
        }
        Call1_02004bfc(Func_0200a300, 0x227);
        Func_0200a324();
    }
}
#undef GameFlag_IsSet_1
#undef GameFlag_Set_1
#undef Battle_Reset_1
#undef GameFlag_IsSet_2
#undef Motion_SetSpeed_1
#undef Motion_SetPosReset_1
#undef Motion_SetAngleToward_1
#undef BattleFx_SpawnLinked_1
#undef Motion_ArmCb_1
#undef Motion_Launch_1
#undef Scene_GetRecord_1
#undef Battle_WaitMode0_1
#undef Motion_SetSpeed_2
#undef Motion_ResetPosMode2_1
#undef Motion_CommitPos_1
#undef Battle_WaitMode0_2
#undef Motion_ArmCb_2
#undef Motion_ArmCb_3
#undef Motion_ArmCb_4
#undef Motion_ArmCb_5
#undef Motion_SetHPosTerrain_1
#undef Motion_SetSpeedLim_1
#undef SceneWork_SetStepValue_1
#undef BattleEv_RunWait_1
#undef Motion_ResetPosMode2_2
#undef Motion_CommitPos_2
#undef Motion_ArmCb_6
#undef Battle_WaitMode0_3
#undef Motion_OffsetPositionAndResetMotion_1
#undef Motion_CommitPos_3
#undef Battle_WaitMode0_4
#undef Audio_PlayCue_1
#undef UiText_DrawMessage_1
#undef Motion_OffsetPositionAndResetMotion_2
#undef Battle_WaitMode0_5
#undef Motion_SetVarCb_1
#undef Motion_SetVarCb_2
#undef Motion_SetVarCb_3
#undef Motion_SetVarCb_4
#undef Battle_WaitMode0_6
#undef Motion_SetAngleToward_2
#undef SceneWork_SetStepValue_2
#undef BattleEv_RunWait_2
#undef Battle_WaitMode0_7
#undef SharedWorkData_SetFirstAndSecondFields_1
#undef BattleFx_ComputeWeightedResultAndDispatch_1
#undef Object_SetModeById_1
#undef Audio_PlayCue_2
#undef Battle_WaitMode0_8
#undef SceneWork_SetStepValue_3
#undef BattleEv_RunWait_3
#undef Object_LinkPair_1
#undef Battle_WaitMode0_9
#undef Motion_ArmCb_7
#undef Motion_Launch_2
#undef Scene_GetRecord_2
#undef SceneWork_SetStepValue_4
#undef BattleEv_RunWait_4
#undef Motion_SetSpeed_3
#undef Motion_SetPosReset_2
#undef Motion_SetAngleToward_3
#undef Motion_SetSpeed_4
#undef Motion_ResetPosMode2_3
#undef Motion_CommitPos_4
#undef Battle_WaitMode0_10
#undef SceneWork_SetStepValue_5
#undef BattleEv_RunWait_5
#undef BattleFx_SpawnLinked_2
#undef Battle_WaitMode0_11
#undef Audio_PlayCue_3
#undef SceneWork_SetStepValue_6
#undef BattleEv_RunWait_6
#undef Battle_WaitMode0_12
#undef Motion_CallWaitAnim_1
#undef Battle_WaitMode0_13
#undef Object_SetModeById_2
#undef Motion_SetVarCb_5
#undef Battle_WaitMode0_14
#undef Motion_SetSpeed_5
#undef Motion_ResetPosMode2_4
#undef Motion_CommitPos_5
#undef Object_SetModeById_3
#undef Battle_WaitMode0_15
#undef Motion_ArmCb_8
#undef Battle_WaitMode0_16
#undef Motion_ArmCb_9
#undef Battle_WaitMode0_17
#undef Motion_SetAngleToward_4
#undef Battle_WaitMode0_18
#undef SceneWork_SetStepValue_7
#undef BattleEv_RunWait_7
#undef Battle_WaitMode0_19
#undef Motion_CallWaitAnim_2
#undef Battle_WaitMode0_20
#undef BattleFx_SpawnLinked_3
#undef Battle_WaitMode0_21
#undef SceneWork_SetStepValue_8
#undef BattleEv_RunWait_8
#undef Battle_WaitMode0_22
#undef Motion_CallWaitAnim_3
#undef Battle_WaitMode0_23
#undef Motion_CallWaitAnim_4
#undef Battle_WaitMode0_24
#undef Motion_ResetPosMode2_5
#undef Motion_CommitPos_6
#undef Object_LinkPair_2
#undef Battle_WaitMode0_25
#undef SceneWork_SetStepValue_9
#undef BattleEv_RunWait_9
#undef Battle_WaitMode0_26
#undef Object_SetModeById_4
#undef Battle_WaitMode0_27
#undef SceneWork_SetStepValue_10
#undef BattleEv_RunWait_10
#undef Motion_SetHPosTerrain_2
#undef Audio_PlayCue_4
#undef SceneWork_SetStepValue_11
#undef BattleEv_RunWait_11
#undef Motion_SetAngleToward_5
#undef Motion_SetAngleToward_6
#undef Motion_SetAngleToward_7
#undef Battle_WaitMode0_28
#undef Motion_ArmCb_10
#undef Motion_SetAngleToward_8
#undef Battle_WaitMode0_29
#undef Audio_PlayCue_5
#undef Motion_SetSpeedLim_2
#undef Motion_PlaceLookedUpObjectWithinCameraBounds_1
#undef Object_CommitPositionThenWaitIfModeZero_1
#undef Motion_SetSpeed_6
#undef Motion_ResetPosMode2_6
#undef Motion_SetPosReset_3
#undef Motion_CommitPos_7
#undef Motion_SetPosReset_4
#undef BattleFx_SpawnLinked_4
#undef BattleFx_SpawnLinked_5
#undef BattleFx_SpawnLinked_6
#undef BattleFx_SpawnLinked_7
#undef BattleFx_SpawnLinked_8
#undef Battle_WaitMode0_30
#undef Motion_SetAngleToward_9
#undef Motion_SetVarCb_6
#undef Battle_WaitMode0_31
#undef SceneWork_SetStepValue_12
#undef BattleEv_RunWait_12
#undef Motion_SetAngleToward_10
#undef SceneWork_SetStepValue_13
#undef BattleEv_RunWait_13
#undef Battle_WaitMode0_32
#undef Motion_SetAngleToward_11
#undef Battle_WaitMode0_33
#undef SceneWork_SetStepValue_14
#undef BattleEv_RunWait_14
#undef Object_LinkPair_3
#undef Object_LinkPair_4
#undef Battle_WaitMode0_34
#undef Motion_SetAngleToward_12
#undef Motion_SetAngleToward_13
#undef Motion_SetAngleToward_14
#undef Motion_SetAngleToward_15
#undef Motion_SetVarCb_7
#undef Battle_WaitMode0_35
#undef SceneWork_SetStepValue_15
#undef BattleEv_RunWait_15
#undef BattleFx_SpawnLinked_9
#undef Battle_WaitMode0_36
#undef Object_SetModeById_5
#undef SceneWork_SetStepValue_16
#undef BattleEv_RunWait_16
#undef Motion_ResetPosMode2_7
#undef Motion_CommitPos_8
#undef Motion_ArmCb_11
#undef SceneWork_SetStepValue_17
#undef BattleEv_RunWait_17
#undef Motion_ResetPosMode2_8
#undef Motion_CommitPos_9
#undef Motion_ArmCb_12
#undef Battle_WaitMode0_37
#undef SceneWork_SetStepValue_18
#undef BattleEv_RunWait_18
#undef BattleFx_SpawnLinked_10
#undef Battle_WaitMode0_38
#undef SceneWork_SetStepValue_19
#undef BattleEv_RunWait_19
#undef Motion_ResetPosMode2_9
#undef Motion_CommitPos_10
#undef Motion_SetAngleToward_16
#undef Motion_CallWaitAnim_5
#undef Battle_WaitMode0_39
#undef SceneWork_SetStepValue_20
#undef BattleEv_RunWait_20
#undef Object_SetModeById_6
#undef SceneWork_SetStepValue_21
#undef BattleEv_RunWait_21
#undef BattleFx_SpawnLinked_11
#undef BattleFx_SpawnLinked_12
#undef BattleFx_SpawnLinked_13
#undef BattleFx_SpawnLinked_14
#undef BattleFx_SpawnLinked_15
#undef Battle_WaitMode0_40
#undef SceneWork_SetStepValue_22
#undef BattleEv_RunWait_22
#undef Battle_WaitMode0_41
#undef BattleFx_SpawnLinked_16
#undef Battle_WaitMode0_42
#undef SceneWork_SetStepValue_23
#undef BattleEv_RunWait_23
#undef Motion_SetVarCb_8
#undef Battle_WaitMode0_43
#undef SceneWork_SetStepValue_24
#undef BattleEv_RunWait_24
#undef Motion_ResetPosMode2_10
#undef Motion_CommitPos_11
#undef Motion_ArmCb_13
#undef Battle_WaitMode0_44
#undef Motion_OffsetPositionAndResetMotion_3
#undef Motion_CommitPos_12
#undef Battle_WaitMode0_45
#undef Audio_PlayCue_6
#undef UiText_DrawMessage_2
#undef Motion_OffsetPositionAndResetMotion_4
#undef Motion_SetAngleToward_17
#undef Audio_PlayCue_7
#undef SceneWork_SetStepValue_25
#undef BattleEv_RunWait_25
#undef Battle_WaitMode0_46
#undef Motion_SetAngleToward_18
#undef Motion_SetAngleToward_19
#undef Motion_SetAngleToward_20
#undef Motion_SetAngleToward_21
#undef Motion_SetAngleToward_22
#undef GameFlag_Set_2
#undef Battle_WaitMode0_47
#undef SharedWorkData_SetFirstAndSecondFields_2
#undef BattleFx_ComputeWeightedResultAndDispatch_2
#undef Battle_SchedShoulder_1
#undef STATE_TABLE_022B
#undef SHARED_RECORD_FIELD_448
#undef Scene_RunScene3bfSequenceA
#undef RunActor17SceneStep
#undef TriggerSceneStage95FromActor12
#undef Scene_RunScene3bfSequenceB
#undef Scene_RunScene3bfSequenceC
#undef RunActor9ScriptedSequence
#undef RunActorScriptedSequenceA
#undef RunActorScriptedSequenceB
#undef RunActorScriptedSequenceC
#undef RunActorScriptedSequenceD
#undef RunActor12InteractionSequence
#undef RunSecondaryMapInteraction
#undef PlayStoryScene
#undef RunActor20SceneSequence
#undef Value_00002421

/* map/locations/heidia/village/village_scene/trigger_tests.c */
#define TryStartActorInteraction Func_02001038
#define IsPlayerInAccidentTriggerArea Func_02001108
#define AreSceneActorsInPassingLane Func_02001324
#define IsPlayerInSecondaryTriggerArea Func_020014cc
#define IsPlayerOutsideSceneRectangle Func_02001660
#define IsSceneActorVerticallyNearPlayer Func_0200181c
#define IsSceneActorHorizontallyNearPlayer Func_02001880
#define IsActorInteractionAvailable Func_020018e4
#define IsSceneActorWithinFourSteps Func_02001918
#define IsSceneActorWithinTriggerBox Func_02001980
#define TriggerScene41AtVillagePath Func_020019e8
#define TriggerScene40AtVillagePath Func_02001a44
typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

typedef struct SceneActor_02001a44 {
    u8 pad0[6];
    u16 dir;        /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor_02001a44;

extern u8 *Data_03001ebc;

void Func_02006678(void);
s32 Func_020067d2(s32, s32, s32);
s32 Func_0200669c(s32, s32);
void Func_02006714(s32, s32);
void Func_02006820(s32);
void Func_02006728(s32, s32);
void Func_02006636(s32);
void Func_020066c2(void);
SceneActor *Func_0200675e(s32);
SceneActor *Func_0200697a(s32);
SceneActor *Func_02006982(s32);
SceneActor *Func_02006b22(s32);
SceneActor *Func_02006cb6(s32);
SceneActor *Func_02006e70(void);
SceneActor *Func_02006e78(s32);
SceneActor *Func_02006ed4(void);
SceneActor *Func_02006edc(s32);
s32 Func_0200326a(void);
s32 Func_02003114(s32);
s32 Func_02003186(s32);
SceneActor *Func_02006f6c(void);
SceneActor *Func_02006f74(s32);
SceneActor *Func_02006fd4(void);
SceneActor *Func_020070da(void);
SceneActor *Func_0200703e(s32);
s32 Func_02007006(s32);
void Func_02002960(void);
SceneActor *Func_0200709a(s32);
s32 Func_02007064(s32);
void Func_02002920(void);

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
    s32 started = 0;
    s32 interaction;

    Func_02006678();
    interaction = Func_020067d2(0, actor_id, interaction_id);
    if (Func_0200669c(interaction_id, 0) != -1) {
        Func_02006714(actor_id, 2);
        started = 1;
    } else {
        Func_02006820(0x7d);
        Func_02006728(actor_id, 5);
    }
    Func_02006636(interaction);
    Func_020066c2();
    return started;
}

s32 IsPlayerInAccidentTriggerArea(void)
{
    SceneActor *player = Func_0200675e(0);
    s32 z = player->z;
    s32 x;
    s32 zz, xx;

    if (z < 0) {
        z += 0xfffff;
    }
    x = player->x;
    zz = z >> 20;
    if (x < 0) {
        x += 0xfffff;
    }
    xx = x >> 20;
    if ((u32)(zz - 5) <= 2 && xx <= 10) {
        return 1;
    }
    if ((u32)(xx - 8) <= 1 && zz > 22) {
        return 1;
    }
    return 0;
}

s32 AreSceneActorsInPassingLane(void)
{
    SceneActor *player = Func_0200697a(0);
    SceneActor *passing_actor = Func_02006982(17);
    s32 ox = player->x;
    s32 pz;
    s32 px;
    s32 oxx, pzz, pxx;

    if (ox < 0) {
        ox += 0xfffff;
    }
    oxx = ox >> 20;
    pz = passing_actor->z;
    if (pz < 0) {
        pz += 0xfffff;
    }
    px = passing_actor->x;
    pzz = pz >> 20;
    if (px < 0) {
        px += 0xfffff;
    }
    pxx = px >> 20;
    if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    return 0;
}

s32 IsPlayerInSecondaryTriggerArea(void)
{
    SceneActor *player = Func_02006b22(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28) {
        return 1;
    }
    if (xx == 41 && zz > 37 && zz <= 41) {
        return 1;
    }
    if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40) {
        return 1;
    }
    return 0;
}

s32 IsPlayerOutsideSceneRectangle(void)
{
    SceneActor *player = Func_02006cb6(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
        return 0;
    }
    return 1;
}

s32 IsSceneActorVerticallyNearPlayer(void)
{
    SceneActor *scene_actor = Func_02006e70();
    SceneActor *player = Func_02006e78(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 z_distance = actor_z - player_z;

    if (z_distance >= -6 && z_distance <= 6 && actor_x - 1 < player_x && actor_x + 1 > player_x) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorHorizontallyNearPlayer(void)
{
    SceneActor *scene_actor = Func_02006ed4();
    SceneActor *player = Func_02006edc(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;

    if (x_distance < -6 || x_distance > 6) {
        return 0;
    }
    if (actor_z - 2 < player_z && actor_z + 2 > player_z) {
        return 1;
    }
    return 0;
}

s32 IsActorInteractionAvailable(s32 actor_id)
{
    if (Func_0200326a() == 0) {
        return 0;
    }
    if (Func_02003114(actor_id)!= 0) {
        return 1;
    }
    {
        s32 result = Func_02003186(actor_id);

        /* branchless "result != 0" */
        return (u32)(result | -result) >> 31;
    }
}

s32 IsSceneActorWithinFourSteps(void)
{
    SceneActor *scene_actor = Func_02006f6c();
    SceneActor *player = Func_02006f74(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    actor_z += 1;
    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance + z_distance <= 4) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorWithinTriggerBox(void)
{
    SceneActor *scene_actor = Func_02006fd4();
    SceneActor *player = Func_020070da();
    s32 actor_x = scene_actor->x / 0x100000;
    s32 actor_z = scene_actor->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance > 7 || z_distance > 5) {
        return 0;
    }
    return 1;
}

void TriggerScene41AtVillagePath(void)
{
    SceneActor *player = Func_0200703e(0);

    if (Func_02007006(859) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 43 && player_z > 28 && player_z <= 31) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 41;

            *q = v;
            Func_02002960();
        }
    }
}

void TriggerScene40AtVillagePath(void)
{
    SceneActor_02001a44 *player = Func_0200709a(0);

    if (Func_02007064(856) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 16 && player_z > 55 && player_z <= 58
            && (player->dir == 0xc000 || player->dir == 0x4000)) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 40;

            *q = v;
            Func_02002920();
        }
    }
}
#undef TryStartActorInteraction
#undef IsPlayerInAccidentTriggerArea
#undef AreSceneActorsInPassingLane
#undef IsPlayerInSecondaryTriggerArea
#undef IsPlayerOutsideSceneRectangle
#undef IsSceneActorVerticallyNearPlayer
#undef IsSceneActorHorizontallyNearPlayer
#undef IsActorInteractionAvailable
#undef IsSceneActorWithinFourSteps
#undef IsSceneActorWithinTriggerBox
#undef TriggerScene41AtVillagePath
#undef TriggerScene40AtVillagePath

/* map/locations/heidia/village/village_scene/village.c */
#define StagedActorStepTable Data_0200ded8
#define GetStagedActor Func_02005724
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02005780
#define SetStagedActorMode Func_02005760
#define SelectStagedActorSlot Func_02005748
#define StartStagedActorEffect Func_0200596e
#define StartNextStagedActorMove Func_02005796
#define StartLeadStagedActorMove Func_020057a6
#define FinishStagedActorMove Func_020057b4
#define FinishStagedActorEffect Func_02005990
#define SetStagedActorTransition Func_020057cc
#define AdvanceStagedActorPair Func_020000c4
#define FindClearActorPosition Func_02000474
#define StartDoraHouseAccidentScene Func_020010a8
#define TurnActorToSceneDirection Func_02001be4


typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

typedef struct Req {
    s32 kind;       /* 0x00 */
    s32 f4;         /* 0x04 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    s32 f14;        /* 0x14 */
} Req;

extern s32 Data_0200df30[];
extern s32 Value_00000f2e;
extern s32 Data_0200dfa4;

struct StagedActor *Func_02005724(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02005780(struct StagedActor *arg0, s32 *arg1);
void Func_02005760(struct StagedActor *arg0, s32 arg1);
void Func_02005748(s32 arg0);
void Func_0200596e(s32 arg0);
void Func_02005796(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020057a6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020057b4(struct StagedActor *arg0);
void Func_02005990(void);
void Func_020057cc(struct StagedActor *arg0, s32 arg1);
Obj *Func_020007de(s32 *, s32 *, Req *);
s32 Func_02005b3c(Obj *, s32 *);
void Func_02006736(s32, s32, s32);
void Func_02006754(s32);
void Func_020067d0(s32, s32, s32);
void Func_020066f6(s32);
void Func_0200677e(s32, s32);
void Func_02006866(s32, s32);
void Func_0200678e(s32, s32);
void Func_0200672e(s32, s32);
void Func_0200670c(s32);
void Func_0200679e(s32, s32, s32);
void Func_020072be(s32, s32, s32);
void Func_020072c8(s32, s32, s32);
void Func_0200386a(s32);
void Func_020038ca(s32);
void Func_0200391e(s32);
void Func_02003996(s32);
void Func_020038e2(s32);

void AdvanceStagedActorPair(void)
{
    extern u32 Data_0200ded8[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 FindClearActorPosition(Req *request)
{
    extern s32 Data_0200ded8[];

    s32 out;
    s32 vec[3];
    Obj *r;
    s32 n;
    s32 nx, ny;
    u8 *flag;
    s32 i, j;
    s32 w1, w2;
    s32 off;

    request->f14 = 0;
    r = Func_020007de(&out, &request->f4, request);
    if (r == 0) {
        return 0;
    }
    flag = (u8 *)r + 0x22;
    *flag = 2;
    {
        s32 k = request->kind;

        n = 0;
        off = k * 4;
        w1 = Data_0200df30[off + 1];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = Data_0200df30[off + 3];
        if (w2 < 0) {
            w2 = -w2;
        }
        ny = (w1 + w2) >> 4;
        w1 = Data_0200df30[off];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = Data_0200df30[off + 2];
        if (w2 < 0) {
            w2 = -w2;
        }
        nx = (w1 + w2) >> 4;
    }
    vec[0] = r->x + (Data_0200ded8[out] & 0xffff0000);
    {
        s32 ry = r->y;

        vec[1] = ry;
        vec[2] = r->z + (Data_0200ded8[out] << 16);
        request->y = ry;
    }
    for (;;) {
        request->z = vec[2] + (Data_0200df30[request->kind * 4 + 1] << 16);
        for (j = 0; j < ny; j++) {
            request->x = vec[0] + (Data_0200df30[request->kind * 4] << 16);
            for (i = 0; i < nx; i++) {
                if (Func_02005b3c(r, &request->x) == 2) {
                    goto found;
                }
                request->x += 0x100000;
            }
            request->z += 0x100000;
        }
        n++;
        vec[0] += Data_0200ded8[out] & 0xffff0000;
        vec[2] += Data_0200ded8[out] << 16;
    }
found:
    *flag = 0;
    if (n == 0) {
        return 0;
    }
    request->x = r->x + (Data_0200ded8[out] & 0xffff0000) * n;
    request->y = r->y;
    request->z = r->z + (Data_0200ded8[out] << 16) * n;
    return 1;
}

void StartDoraHouseAccidentScene(void)
{
    Func_02006736(0, 0x108, 0x318);
    Func_02006754(0);
    Func_020067d0(0, 0x4000, 0);
    Func_020066f6(10);
    Func_0200677e(0, 1);
    Func_02006866(0xea, 3);
    Func_0200678e(0, 1);
    Func_0200672e(0xea, 0);
    Func_0200670c((s32)&Value_00000f2e);
    Func_0200679e(8, 0, 0);
}

void TurnActorToSceneDirection(s32 actor_id)
{
    Func_020072be(actor_id, 0, 0);
    Func_020072c8(0, actor_id, 0);
    switch (Data_0200dfa4 & 3) {
    case 0:
        Func_0200386a(actor_id);
        break;
    case 1:
        Func_020038ca(actor_id);
        break;
    case 2:
        Func_0200391e(actor_id);
        break;
    case 3:
        Func_02003996(actor_id);
        break;
    default:
        Func_020038e2(actor_id);
        break;
    }
}
#undef StagedActorStepTable
#undef GetStagedActor
#undef FindNextStagedActor
#undef FindBlockingStagedActor
#undef FindElevatedBlockingStagedActor
#undef CanStartStagedActorMove
#undef SetStagedActorMode
#undef SelectStagedActorSlot
#undef StartStagedActorEffect
#undef StartNextStagedActorMove
#undef StartLeadStagedActorMove
#undef FinishStagedActorMove
#undef FinishStagedActorEffect
#undef SetStagedActorTransition
#undef AdvanceStagedActorPair
#undef FindClearActorPosition
#undef StartDoraHouseAccidentScene
#undef TurnActorToSceneDirection
