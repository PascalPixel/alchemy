/// @file
/// Scene code of overlay 0x698, in ROM order.

#include "../shared/routines.h"
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

DEFINE_FUNC_020003D0(Func_020005b8)

extern const s32 gData_0200219c[];
extern s32 gData_020021bc;

s32 Func_02001b2c(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_FUNC_020005E4(Func_020005e4, Func_02001b2c, Func_020005b8,
                     gData_0200219c, gData_020021bc)

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

DEFINE_FUNC_020007A4(Func_020007a4, Func_020019fc, Func_020005e4, Func_02000eac)

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

DEFINE_FUNC_0200091C(Func_0200091c)

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.
#include "map.h"
#include "vram_tab.h"

extern s16 gData_02002200;
extern u32 gData_02002204[];

void Func_020019d4(struct LightOam *oam, s32 depth);

DEFINE_FUNC_02000AAC(Func_02000aac, gData_02002200, Data_02002244,
                     gData_02002202, gData_02002204, gData_02002304,
                     gData_02002306, Func_02001b24, Func_020019d4)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02000c68, gData_02002204, gData_02002200,
                     Func_02000aac, gData_02002202, gData_02002304,
                     gData_02002306)

DEFINE_FUNC_02000CE8(Func_02000ce8, gData_02002204, gData_02002200,
                     Func_02000aac, gData_02002202, gData_02002304,
                     gData_02002306)

DEFINE_FUNC_02000D68(Func_02000d68, gData_02002204, gData_02002200,
                     Func_02000aac, gData_02002202, gData_02002304,
                     gData_02002306)

DEFINE_FUNC_02000DF0(Func_02000df0, gData_02002202, gData_02002204)

void Func_02000e20(s32 value) { gData_02002306 = value; }

#include "../../field/object/set_sprite_priority.c"
