#include "types.h"

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

#define PlaceActorTwelveAndFinishScene Func_02000c78
void Func_02006326(s32, s32, s32);
void Func_020017d6();

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02006326(actor, x, y);
}

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

#define NoOpSceneCallbackA Func_02001098
#define NoOpSceneCallbackB Func_0200109c
#define NoOpSceneCallbackC Func_020010a0
#define NoOpSceneCallbackD Func_020010a4
#define NoOpInteractionCallback Func_02002528
#define NoOpActorCallback Func_02004d4c

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

typedef struct DirectionalSceneActor {
    u8 pad0[6];
    u16 dir;        /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} DirectionalSceneActor;

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

void PlaceActorTwelveAndFinishScene(void)
{
    PlaceActor(12, 0x00f80000, 0x01780000);
    Func_020017d6();
}

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

void ActivateSceneActor17(void)
{
    Func_02002fe6(17);
    Func_02006afe();
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

void ActivateSceneActor18(void)
{
    Func_0200317a(18);
    Func_02006c92();
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
    DirectionalSceneActor *player = Func_0200709a(0);

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

void NoOpInteractionCallback(void)
{
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

void NoOpActorCallback(void)
{
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
