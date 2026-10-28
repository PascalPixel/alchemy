#include "types.h"
#include "map.h"
#include "metadata_lookup.h"
#include "object_runtime.h"
#include "ram_buffer.h"

/// Writes the object's position on screen, in pixels, to position[0] and
/// position[1]; an animated object is raised by its frame box. Returns -1 when
/// the object does not exist.
s32 Func_080d5bec(s32 objectId, s32 *position) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);
  s32 *camera;
  s32 cameraX;
  s32 cameraZ;
  s32 x;
  s32 z;

  if (object == 0)
    return -1;
  /* FAKEMATCH: the view position read as two adjacent words through one
   * pointer, as in The Broken Seal, so both loads share one base; read as
   * map->viewX and map->viewZ it differs 26 (64 instructions for 62). */
  camera = &Ram_HeapSlots->mapWork->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / 0x10000;
  *position = z / 0x10000;
  if ((object->animationKind & 15) == 1)
    *position -=
        (s8)Resource_GetMetadataRecordFarFar(
            *((struct ObjectAnimationState *)object->animation)->value28)
            ->boxX;
  return 0;
}
