#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field_event.h"
#include "field/event.h"

// Defined in the old style, with no prototype: Idejima's calls after it are
// then compiled as calls to an undeclared routine, which pass their arguments
// as int and load the second first.
// clang-format off
void DriftScene_SpawnObject(x, y, z, value)
  s32 x, y, z;
  s16 value;
{
  // clang-format on
  struct Actor *object;
  struct FieldSprite *sprite;

  object = Import_ActorCreate(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Sound_Play(SE_LIGHT_APPEAR);
    Import_ActorSetMode(object, 1);
    Import_ActorStartScript(object, gDriftScene_SpawnScript);
    object->motionFlags = 0;
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
