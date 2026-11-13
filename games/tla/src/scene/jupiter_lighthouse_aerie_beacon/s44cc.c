/// @file
/// Scene code of overlay 0x6a5, in ROM order.

#include "../shared/whirlwind.h"
#include "../shared/jupiter_lighthouse.h"
#include "../shared/effects.h"
#include "types.h"
#include "system.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "frame.h"
#include "field_event.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_aerie_beacon.h"
#include "module/scene/scene.h"

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Whirlwind_Update(struct GustObject *object);
void Func_02004bd4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN_UPPER(Whirlwind_SpawnUpper, Func_02005754, Whirlwind_Update, Func_02004bd4)

DEFINE_ACTOR_EMIT_RISING_SPARKS_B(Actor_EmitRisingSparksB, Func_02004bd4)

void Animation_ApplyChildValues(struct Actor *object, s32 value);

static __inline__ void Func_02001c10_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Test(flag); }

void Func_02004644(void) {
  struct Actor *left = Actor_GetById(9);
  struct Actor *right = Actor_GetById(10);
  struct Actor *rider = Actor_GetById(23);
  struct FieldSprite *sprite = rider->sprite;

  Func_02001c10_(24, 0x2000000, 0xf80000);
  Func_02001c10_(23, 0x2000000, 0xf80000);
  Func_02001c10_(9, 0x1d80000, 0x1000000);
  Func_02001c10_(10, 0x2280000, 0x1000000);
  Actor_GetById(24)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y = rider->terrainHeight;
  left->motionFlags = 0;
  left->y = left->terrainHeight;
  right->motionFlags = 0;
  right->y = right->terrainHeight;
  if (GameFlag_Test_(0xa0a)) {
    left->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += -0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Actor_GetById(9), 4);
    Animation_ApplyChildValues(Actor_GetById(11), 4);
  }
  if (GameFlag_Test_(0xa0b)) {
    right->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(Actor_GetById(10), 4);
    Animation_ApplyChildValues(Actor_GetById(12), 4);
  }
  if (GameFlag_Test_(0xa0a) && GameFlag_Test_(0xa0b)) {
    left->y += 0x199900;
    right->y += 0x199900;
    rider->y += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
  }
}

/* VIEW(s44cc): variable gData_02006b70 as s16 gData_02006b70; module/scene/scene.h has u8 gData_02006b70[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_aerie_beacon/s44cc.c:217: `gData_02006b70' has an incomplete type. */
extern s16 gData_02006b70__s44cc __asm__("gData_02006b70");

void Func_0200571c(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, Data_02006ab0,
    gLightSpriteCount, gData_02006a70, gData_02006b70__s44cc, gLightSpritePriority,
    Func_020059bc, Func_0200571c)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gData_02006a70, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gData_02006b70__s44cc, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gData_02006a70, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gData_02006b70__s44cc, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gData_02006a70, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gData_02006b70__s44cc, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD_COUNTED(LightSprites_AddCounted, gData_02006a70, gLightSpriteCount)

void Func_02004b48(s32 value) { gLightSpritePriority = value; }

#include "../../field/object/set_sprite_priority.c"
