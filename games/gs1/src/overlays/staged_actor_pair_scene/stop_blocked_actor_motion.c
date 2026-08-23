#include "staged_actor_pair_scene.h"

typedef struct { s32 x; s32 y; s32 z; } ScenePosition;
typedef struct { u8 filler0[0x28]; s16 *kind; } ActorSpriteRef;
typedef struct {
    u8 filler0[6];
    u16 direction_and_kind;
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x10];
    s32 move_delta_x;
    u8 filler28[4];
    s32 move_delta_z;
    u8 filler30[8];
    s32 move_target_x;
    u8 filler3C[4];
    s32 move_target_z;
    u8 filler44[0xC];
    ActorSpriteRef *sprite;
} MovingActor;

extern s32 Data_020096c0[];
extern s32 Data_02009700[];
extern MovingActor *Func_0200090a(ScenePosition *, MovingActor *);
extern s32 Func_02001ada(MovingActor *, ScenePosition *);

s32 StagedActorPairScene_StopBlockedMotion(MovingActor *actor)
{
    ScenePosition destination;
    u32 direction;
    s32 step;
    MovingActor *blocker;

    direction = actor->direction_and_kind >> 12;
    step = Data_020096c0[direction];
    destination.x = actor->x + (step & 0xffff0000);
    destination.y = actor->y;
    step = step << 16;
    destination.z = actor->z + step;
    blocker = Func_0200090a(&destination, actor);
    if (blocker != 0) {
        u32 allowed_index = 0;
        s32 blocker_kind = *blocker->sprite->kind;
        s32 *allowed_kinds = Data_02009700;

        do {
            if (blocker_kind == *allowed_kinds++) goto done;
            allowed_index++;
        } while (allowed_index <= 5);
        actor->move_delta_x = 0;
        actor->move_delta_z = 0;
        actor->move_target_x = 0x80000000;
        actor->move_target_z = 0x80000000;
    }
    step = Data_020096c0[direction];
    destination.x = actor->x + (step & 0xffff0000);
    destination.y = actor->y;
    step = step << 16;
    destination.z = actor->z + step;
    if (Func_02001ada(actor, &destination) > 0) {
        actor->move_delta_x = 0;
        actor->move_delta_z = 0;
        actor->move_target_x = 0x80000000;
        actor->move_target_z = 0x80000000;
    }
done:
    return 0;
}
