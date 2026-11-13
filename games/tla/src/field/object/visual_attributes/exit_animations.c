/// @file
/// An actor's screen position, and the backdrop faded from white.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "sounds.h"
#include "module/field/field.h"

/// Writes the object's position on screen, in pixels, to position[0] and
/// position[1]; an animated object is raised by its frame box. Returns -1 when
/// the object does not exist.
s32 Actor_GetScreenPosition(s32 actorId, s32 *position) {
  struct Actor *object = ObjectTable_Get(actorId);
#ifdef UBFIX
  struct MapWork *map;
#else
  s32 *camera; // the view's top left, x then z
#endif
  s32 cameraX;
  s32 cameraZ;
  s32 x;
  s32 z;

  if (object == 0)
    return -1;
  // UB: camera[1] crosses the viewX scalar; UBFIX reads the named pair.
#ifdef UBFIX
  map = Ram_HeapSlots->mapWork;
  cameraX = map->viewX & 0xffff0000;
  cameraZ = map->viewZ & 0xffff0000;
#else
  camera = &Ram_HeapSlots->mapWork->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
#endif
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / Q16_ONE;
  *position = z / Q16_ONE;
  if ((object->animationKind & 15) == 1)
    *position -= (s8)Resource_GetMetadataRecordFarFar(
                     *((struct ObjectAnimationState *)object->sprite)->value28)
                     ->boxX;
  return 0;
}

/// The Broken Seal's Scene_FadeColorFromWhite, playing cue 298 where that
/// plays 288: turns the backdrop (on the world map, a colour of palette 15)
/// white, finishes the screen transition and fades that colour to black
/// over 16 frames.
void Field_StartEncounterTransition(s32 unused) {
  struct EventWork *work;
  s32 i;
  s32 c;

  work = Ram_HeapSlots->eventWork;
  Sound_PlayFar(gMainRoutineWork.sceneCue);
  Sound_PlayFar(SE_298);
  Sound_PlayFar(SE_BEAST_CRY);
  if (work->controlMode == 3) {
    Io_Write16(RGB(31, 31, 31), (u16 *)&BG_PLTT_COLOR(15, 3));
    ScreenTransition_Finish(0x401, 16);
    work->screenStatus = 0;
    wait(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16(RGB(c, c, c), (u16 *)&BG_PLTT_COLOR(15, 3));
      wait(1);
    }
  } else {
    Io_Write16(RGB(31, 31, 31), &PLTT_BACKDROP);
    ScreenTransition_Finish(0x207, 16);
    work->screenStatus = 0;
    wait(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16(RGB(c, c, c), &PLTT_BACKDROP);
      wait(1);
    }
  }
}

void *ResourceMetadata_RegisterFar(void *, s32);

/// The Broken Seal's Exit_PrepareAnimation for any actor: centres actor id on
/// its cell at the cell's south edge, stops it, hides its shadow and plays
/// animation 25 (side 1) or 26 over 18 frames, its sprite gaining the doorway
/// part (metadata 267).
void Exit_PrepareActorAnimation(s32 id, s32 side) {
  u32 zero;
  u8 kind;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  s32 mode;

  object = ObjectTable_Get(id);
  context = object->sprite;
  mode = (side != 1);
  mode = 26 - mode;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  // Through a byte pointer: as context->flags the kind is set first.
  *&context->flags = zero;
  kind = 15;
  effect->param = kind;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  Actor_SelectAnimationFar(object, mode);
  wait(18);
}
