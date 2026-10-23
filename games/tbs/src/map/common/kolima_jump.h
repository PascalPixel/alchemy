/// @file
/// Kolima Forest (MapId_KolimaForestEntrance and MapId_KolimaForestCliff):
/// the hop east onto the map's third placed object event.
#include "types.h"
#include "engine_call.h"
#include "object_event.h"
#include "map_header.h"
#include "save_block.h"
#include "songs.h"

/// Hops the leader to the pixel of the 16.16 position request, unless something
/// blocks the way: the leader crouches, jumps (3 pixels a frame, rising at 4)
/// and lands while that object event plays its animation 7; the leader then
/// dips a pixel twice, its height and its ground height together, and rises
/// back. Returns 1 after a hop.
static s32 PushedObject_RunStepEffect(struct FieldPosition *request) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u8 *flags = &objectEvent->motionFlags;
  u8 saved = *flags;
  s32 ret = FieldObject_CheckMovementCollision(objectEvent, request);

  if (ret == 0) {
    FieldScript_Start();
    ObjectDispatch_SelectAnimation(objectEvent, 6);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    ObjectDispatch_SelectAnimation(objectEvent, 7);
    objectEvent->speed = PIXELS(3);
    objectEvent->acceleration = PIXELS(2);
    objectEvent->velocityY = PIXELS(4);
    *flags &= 0x7e;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    ObjectMotion_SetPositionAndCommit(
        LOCALID_PARTY_LEADER,
        ((union FieldCoordinate *)&request->x)->part.pixel,
        ((union FieldCoordinate *)&request->z)->part.pixel);
    ObjectDispatch_SelectAnimation(objectEvent, 6);
    ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
    *flags = ret;
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_PLACED + 2, 7);
    objectEvent->y.fixed += -PIXELS(1);
    objectEvent->groundHeight += -PIXELS(1);
    WaitFrames(2);
    objectEvent->y.fixed += -PIXELS(1);
    objectEvent->groundHeight += -PIXELS(1);
    WaitFrames(10);
    objectEvent->y.fixed += PIXELS(1);
    objectEvent->groundHeight += PIXELS(1);
    WaitFrames(4);
    objectEvent->y.fixed += PIXELS(1);
    objectEvent->groundHeight += PIXELS(1);
    *flags = saved;
    FieldScript_End();
    return 1;
  }
  return 0;
}

/// Coord event 11, walking east: hop 30 pixels east of the leader. The
/// patched editions hop from the centre of the moving character's cell, 40
/// pixels.
static void KolimaForest_HopEast(void) {
  struct FieldPosition target;
#if !TBS_PATCH_1
  target.x = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed + PIXELS(30);
  target.y = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed;
  target.z = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed;
#else
  // gSaveBlock.leaderLocalId
  s32 off = 500;
  struct ObjectEvent *p =
      ObjectEvent_GetById(*(s32 *)((u8 *)&gSaveBlock + off));
  u32 base = p->x.fixed & 0xfff00000;

  target.x = base + PIXELS(8);
  target.y = p->y.fixed;
  target.z = (p->z.fixed & 0xfff00000) + PIXELS(8);
  target.x = base + PIXELS(40);
#endif
  PushedObject_RunStepEffect(&target);
}
