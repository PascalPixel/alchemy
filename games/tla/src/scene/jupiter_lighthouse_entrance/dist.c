/// @file
/// Scene code of overlay 0x698, in ROM order.

#include "../shared/whirlwind.h"
#include "../shared/jupiter_lighthouse.h"
#include "../shared/actors.h"
#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "irq.h"
#include "event_work.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "call.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "field_event.h"
#include "task.h"
#include "frame.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_entrance.h"
#include "module/scene/scene.h"

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern const s32 gWhirlwindEndScript[];
extern s32 gWhirlwindRiding;

s32 Func_02001b2c(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Whirlwind_UpdateLower, Func_02001b2c,
                              Position_FlatDistance, gWhirlwindEndScript,
                              gWhirlwindRiding)

void Actor_SetSpriteFlags(struct FieldEffect *effect, s32 flags);

DEFINE_WHIRLWIND_SPAWN_UPPER(Whirlwind_SpawnUpper, Import_ActorSelectAnimation,
                             Whirlwind_UpdateLower, Func_02000eac)

s32 Func_0200087c(struct SceneEffectSource *object) {
  struct SceneEffectParams params;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    params.priority = 3;
    params.startScaleX = 0xb333;
    params.startScaleY = 0xb333;
    params.palette = 14;
    x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random16Far() * 20) >> 16)) << 16);
    speed = ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x50000) / 10;
    Func_02000eac(x, y, object->z, 0, speed, phase, 0xb0000, &params);
  }
  return 0;
}

void Actor_SetSpriteColorMode(struct PositionedActor *object, s32 value);

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.
#include "map.h"
#include "vram_tab.h"

extern s16 gLightSpriteTile;
extern u32 gData_02002204[];

void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, Data_02002244,
                          gLightSpriteCount, gData_02002204, gLightSpritesLarge,
                          gLightSpritePriority, BattleEffect_GetObjectPriority,
                          Import_OamAddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gData_02002204,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gData_02002204,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gData_02002204,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount, gData_02002204)

void Func_02000e20(s32 value) { gLightSpritePriority = value; }

#include "../../field/object/set_sprite_priority.c"
