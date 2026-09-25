#include "TYPES.H"
#include "FIELD_EVENT.H"

void Engine_ObjectCommitPosition(struct FieldActor *object);
void Main_08009060(s32 handle);
struct FieldActor *Main_0808a400(s32 actor);

#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))

/* Settles the pushed object on the centre of its cell, then lifts the leader
 * onto it: the leader hops up a cell and a half and turns to face along the
 * row, drawn in front of the background while it jumps. */
void ArutinYama_SettleAndMountLeader(struct FieldActor *object)
{
    s32 x;
    s32 z;
    s32 center_z;
    struct FieldSprite *sprite;
    struct FieldActor *leader;

    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(232);
    x = object->x.fixed & 0xfff00000;
    center_z = 0x80000;
    z = object->z.fixed & 0xfff00000;
    object->acceleration = 0x20000;
    x += center_z;
    center_z += z;
    Engine_ObjectSetPosition(object, x, object->y.fixed, center_z);
    Engine_ObjectCommitPosition(object);
    object->unknown_22 = 0;
    object->x.fixed = x;
    object->z.fixed = center_z;
    object->velocity_x = 0;
    object->velocity_z = 0;
    Engine_ObjectSetAnimation(object, 2);
    Engine_TaskWait(15);
    Engine_ObjectSetAnimation(object, 1);
    Engine_TaskWait(30);
    sprite = object->sprite;
    sprite->part_count = 1;
    Main_08009060(*(s32 *)((u8 *)sprite + 44));
    *(s32 *)((u8 *)sprite + 44) = 0;
    ((u8 *)sprite)[37] = 1;
    leader = Main_0808a400(gGameState.selected_actor);
    Engine_AudioPlayCue(152);
    leader->x.fixed = x;
    leader->velocity_y = 0x60000;
    *(s32 *)((u8 *)leader + 72) = 0x10000;
    leader->z.fixed = center_z;
    SPRITE_OF(leader)->priority = 0;
    Engine_ObjectSetAnimation(leader, 7);
    Engine_ObjectSetPosition(leader, x, leader->y.fixed, z + 0x180000);
    leader->facing = 0x4000;
    Engine_TaskWait(20);
    SPRITE_OF(leader)->priority = 2;
    Engine_AudioPlayCue(159);
}
