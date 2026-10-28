#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "metadata_lookup.h"
#include "scene.h"
#include "actor.h"

s32 BattleEffect_CopyWatchedObjectPosition(struct Actor *object) {
  struct Actor *link;

  link = object->watchedObject;
  if (link != NULL) {
    object->motionFlags = 0;
    object->x = link->x;
    object->y = link->y +
                ((s8)Resource_GetMetadataRecordFarFar(
                     *Scene_FindActorPlacement(object->unk66))
                     ->boxX
                 << 16) +
                0x80000;
    object->terrainHeight = link->terrainHeight;
    object->z = link->z;
  }
  return 0;
}
