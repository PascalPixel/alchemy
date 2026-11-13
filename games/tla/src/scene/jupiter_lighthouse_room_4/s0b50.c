/// @file
/// Scene code of overlay 0x69d, in ROM order.

#include "../shared/whirlwind.h"
#include "../shared/jupiter_lighthouse.h"
#include "types.h"
#include "system.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "field_event.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "frame.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_4.h"
#include "module/scene/scene.h"

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Whirlwind_Update(struct GustObject *object);
void Func_02001258(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Func_02001da8, Whirlwind_Update, Func_02001258)

s32 Func_02000c28(struct Actor *object) {
  struct EffectOptions options;
  s32 x;
  s32 y;
  s32 velocityY;

  if ((gFrameCount & 7) == 0) {
    options.priority = 3;
    options.startScaleX = 0xb333;
    options.startScaleY = 0xb333;
    options.palette = 14;
    x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);
    velocityY = (s32)(((((Random16Far() * 5) >> 16) << 16) + 0x50000)) / 10;
    Func_02001258(x, y, object->z, 0, velocityY, 0,
                  EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                      EFFECT_USE_START_SCALE,
                  &options);
  }
  return 0;
}

void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_02000cc8(void) {
  struct Actor *left = Actor_Get(9);
  struct Actor *right = Actor_Get(10);
  struct Actor *rider = Actor_Get(23);
  struct FieldSprite *sprite = rider->sprite;

  Actor_SetPosition(24, 0x2000000, 0xf80000);
  Actor_SetPosition(23, 0x2000000, 0xf80000);
  Actor_SetPosition(9, 0x1d80000, 0x1000000);
  Actor_SetPosition(10, 0x2280000, 0x1000000);
  Actor_Get(24)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y = rider->terrainHeight;
  left->motionFlags = 0;
  left->y = left->terrainHeight;
  right->motionFlags = 0;
  right->y = right->terrainHeight;
  if (Event_GetFlag(0xa0a)) {
    left->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += -0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Actor_Get(9), 4);
    Animation_ApplyChildValues(Actor_Get(11), 4);
  }
  if (Event_GetFlag(0xa0b)) {
    right->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(Actor_Get(10), 4);
    Animation_ApplyChildValues(Actor_Get(12), 4);
  }
  if (Event_GetFlag(0xa0a) && Event_GetFlag(0xa0b)) {
    left->y += 0x199900;
    right->y += 0x199900;
    rider->y += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
  }
}

/* VIEW(s0b50): variable gData_02002760 as struct Actor *gData_02002760[]; module/scene/alhafra.h has s16 gData_02002760[]; tried as the definition's extern s16 gData_02002760[], which the object already declares, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_4/s0b50.c:143: `gData_02002760' undeclared (first use in this function). */
extern struct Actor *gData_02002760__s0b50[] __asm__("gData_02002760");

s32 Func_02001ee0(struct Actor *actor);
void Import_OamAddSpriteAtDepth(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOam,
    gLightSpriteCount, gData_02002760__s0b50, gLightSpritesLarge, gLightSpritePriority,
    Func_02001ee0, Import_OamAddSpriteAtDepth)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gData_02002760__s0b50, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gData_02002760__s0b50, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gData_02002760__s0b50, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount, gData_02002760__s0b50)

void Func_020011cc(s32 value) { gLightSpritePriority = value; }

#include "../../field/object/set_sprite_priority.c"
