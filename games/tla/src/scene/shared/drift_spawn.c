#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/field_event.h"
#include "field/event.h"

void DriftScene_SpawnObject(x, y, z, value) s32 x, y, z;
s16 value;
{
  // clang-format on
  struct Actor *object;
  struct FieldSprite *sprite;

  object = Import_ActorCreate(SPRITE_EMOTE, x, y, z);
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
