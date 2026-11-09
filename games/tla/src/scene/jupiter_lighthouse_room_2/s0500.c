#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

static inline void CopyCells(s32 srcX, s32 srcY, s32 width, s32 height,
                             s32 destX, s32 destY) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each stack
   * argument in its own register (differ 10 as direct calls). */
  Func_0802b450(srcX, srcY, width, height, destX, destY);
}

/// Actor id comes down: once it rests on the terrain (or after 30 frames) cue
/// 188 plays, and at cell (20, 39) it sets flag 0xa02 and copies map cells in
/// three steps, each with cue 167.
void Func_02000500(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  struct MainRoutineWork *party;
  s32 i;

  object->motionFlags = 3;
  Actor_GetById(id)->priorityFlags |= 1;
  party = &gMainRoutineWork;
  Actor_GetById(party->currentOwner)->priorityFlags |= 1;
  if (object->x >> 20 == 20 && object->z >> 20 == 39)
    Actor_SetSpritePriority(id, 3);
  /* FAKEMATCH: the memory barrier reads the position again for the terrain
   * height, as the native does (differ 18 without). */
  asm("" ::: "memory");
  if (object->terrainHeight !=
      Map_GetTerrainHeight(object->terrainId, object->x, object->z)) {
    wait(2);
    for (i = 0; i <= 29 &&
                (object->y > object->terrainHeight || object->velocityY != 0);
         i++)
      wait(1);
    Sound_Play(188);
  }
  if (object->x >> 20 == 20 && object->z >> 20 == 39) {
    GameFlag_SetBit(0xa02);
    Actor_SetSingleChildFlags(Actor_GetById(id), 0);
    Actor_SetSpritePriority(id, 3);
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
    Map_CopyCellAttributeRect(28, 39, 1, 1, 20, 39);
    Func_0802b450(28, 39, 1, 1, 20, 39);
    Func_0802b450(92, 39, 1, 1, 84, 39);
    Event_Wait(30);
    Sound_Play(167);
    Func_0802b450(92, 40, 1, 1, 84, 40);
    Event_Wait(20);
    Sound_Play(167);
    CopyCells(92, 41, 1, 1, 84, 41);
    Event_Wait(20);
    Sound_Play(167);
    Func_0802b450(93, 41, 1, 1, 85, 41);
    Func_080dbdd4(168 << 17, 164 << 18, 2, 236);
  }
}
