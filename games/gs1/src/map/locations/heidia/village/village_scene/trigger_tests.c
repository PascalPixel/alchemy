#include "types.h"

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
