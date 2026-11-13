#ifndef ALCHEMY_SHOP_INTERNAL_H
#define ALCHEMY_SHOP_INTERNAL_H

/// @file
/// What every part of the shop object shares: its includes and the types two
/// or more parts use. shop.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves. It places
/// no bytes.

#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "heap_release.h"
#include "resource.h"
#include "task.h"
#include "edition.h"
#include "render_input.h"
#include "main_routine_work.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene.h"
#include "io_reg.h"
#include "input.h"
#include "text/msg_ids.h"
#include "version.h"
#include "shop.h"
#include "minigame.h"
#include "battle_unit.h"
#include "item.h"
#include "battle_system.h"
#include "game_flags.h"
#include "owner_state.h"
#include "fixed_math.h"
#include "field_event.h"
#include "item_icon.h"
#include "character_ids.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/menu.h"
#include "gamepak/far_call/window.h"
#include "module/shop/shop.h"
#include "module/tla.h"

struct FieldObject {
  u8 unk000[0x34];
  u16 savedX;
  u16 savedY;
  s16 x;
  u16 y;
  u8 unk03c[0x14];
  struct ObjectSpriteInfo *sprite;
};

/* VIEW(shop): routine Inventory_EquipFar as void Inventory_EquipFar(s32 owner, s32 slot); gamepak/far_call/game.h has s32 Inventory_EquipFar(s32 owner, s32 slot). */
extern void Inventory_EquipFar__shop(s32 owner, s32 slot) __asm__("Inventory_EquipFar");
/* VIEW(shop): routine Actor_GetByIdFar as void *Actor_GetByIdFar(s32 id); gamepak/far_call/field_sequence.h has struct Actor *Actor_GetByIdFar(s32 id). */
extern void *Actor_GetByIdFar__shop(s32 id) __asm__("Actor_GetByIdFar");
void WindowSprite_RedrawSavedRectFar(void *window);
/* VIEW(shop): routine Text_DrawMessageAsTilesFar as void Text_DrawMessageAsTilesFar(s32 message, void *window, s32 x, s32 y); gamepak/far_call/window.h has void Text_DrawMessageAsTilesFar(s32 message, struct RenderInput *window, s32 x, s32 y). */
extern void Text_DrawMessageAsTilesFar__shop(s32 message, void *window, s32 x, s32 y) __asm__("Text_DrawMessageAsTilesFar");

#endif
