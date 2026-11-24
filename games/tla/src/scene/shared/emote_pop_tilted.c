#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/field_event.h"
#include "field/event.h"

/// Pops an emote up at (x, y, z), 16.16 pixels: SPRITE_EMOTE's animation 1,
/// tilted by `rotation`, with the SE_LIGHT_APPEAR sound; its script keeps it 46
/// frames and releases it. Idejima and the Shaman Village each pop a pair,
/// tilted either way, in a conversation.
void Emote_PopTilted(x, y, z, rotation) s32 x, y, z;
s16 rotation;
{
  struct Actor *object;
  struct FieldSprite *sprite;

  object = Import_ActorCreate(SPRITE_EMOTE, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Sound_Play(SE_LIGHT_APPEAR);
    Import_ActorSelectAnimation(object, 1);
    Import_ActorStartScript(object, gEmote_PopTiltedScript);
    object->motionFlags = 0;
    sprite->flags = 0;
    sprite->rotation = rotation;
  }
}
