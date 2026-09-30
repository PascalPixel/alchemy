#include "HEYA.H"
#include "CALL.H"

void Event_SetPairWork1c0(s32 scene, s32 entrance);

/* A floor switch: when the leader steps onto a new entrance tile, copy its
 * cell; the first time, just set its flag, otherwise drop the leader through
 * the floor to map 0x2d at the entrance, spinning as it falls. */
void ToretoHeya_HandleFloorSwitch(s32 flag, s32 src_x, s32 src_y, s32 entrance)
{
    s32 dest_x;
    s32 dest_y;
    s32 leader;
    struct FieldActor *actor;
    s32 cnt;

    dest_x = (gGameState.x >> 20) + 64;
    dest_y = gGameState.z >> 20;
    leader = gGameState.selected_actor;
    actor = Engine_ActorGet(leader);
    if (entrance == *ToretoHeya_PaletteBuffer)
        return;
    *ToretoHeya_PaletteBuffer = entrance;
    if (Engine_GameFlagIsSet(flag) == 0) {
        Engine_MapCopyCells(src_x, src_y, 1, 1, dest_x, dest_y);
        Engine_GameFlagSet(flag);
        return;
    }
    *ToretoHeya_PaletteBuffer = -1;
    Engine_MapCopyCells(src_x, src_y + 1, 1, 1, dest_x, dest_y);
    Engine_AudioPlayCue(206);
    Engine_EventBegin();
    Event_SetPairWork1c0((s32)&SceneId_ToretoHeya, entrance);
    Engine_ActorSetAnimation(leader, 27);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(leader), 0);
    Call2(Engine_ActorSetAttachedEffect, leader, 0x101);
    Engine_EventWait(30);
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    actor->motion_flags = 2;
    *(s32 *)((u8 *)actor + 20) = -0xa00000;
    *(s32 *)((u8 *)actor + 72) = 0x8000;
    Engine_AudioPlayCue(204);
    Engine_EventWait(3);
    actor->unknown_22 = 2;
    Engine_ActorSetSpritePriority(leader, 3);
    for (cnt = 29; cnt >= 0; cnt--) {

        actor->facing += 0x2000;
        Engine_TaskWait(1);
    }
    if (entrance != 50)
        Engine_GameFlagSet(0x122);
}
