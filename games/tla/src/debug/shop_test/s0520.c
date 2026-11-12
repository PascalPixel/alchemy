/// @file
/// Overlay 0x6ba's code from Func_02000520 on, in ROM order.

#include "types.h"
#include "owner_state.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "version.h"
#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "input.h"
#include "dma.h"
#include "io_reg.h"
#include "actor.h"

#include "fixed_math.h"
#include "field.h"

void Unit_AddPp(s32 owner, s32 amount);
void Inventory_Break(s32 owner, s32 slot);
struct ItemDefinition *Func_02000d48(s32 item);
void Func_02000dc0(void);

void Func_02000520(void) {
  u8 *unit;
  u8 *item;
  s32 i;

  FieldMessage_Show(MsgTheBodyIs, 1);
  FieldUnit_AddHp(4, -100);
  FieldUnit_AddHp(5, -100);
  FieldUnit_AddHp(6, -33);
  Unit_AddPp(4, -50);
  Unit_AddPp(5, -40);
  Unit_AddPp(6, -35);
  unit = Unit_Get(4);
  unit[0x131] = 1;
  unit[0x140] = 1;
  unit = Unit_Get(5);
  unit[0x130] = 1;
  unit[0x131] = 2;
  unit = Unit_Get(4);
  for (i = 0; i <= 14; i++) {
    item = Func_02000d48(((struct OwnerInventoryState *)unit)->inventory[i]);
    if (((struct OwnerInventoryState *)unit)->inventory[i] == 0)
      break;
    if (item[12] == 2) {
      Inventory_Break(4, i);
      break;
    }
  }
  unit = Unit_Get(5);
  for (i = 0; i <= 14; i++) {
    item = Func_02000d48(((struct OwnerInventoryState *)unit)->inventory[i]);
    if (((struct OwnerInventoryState *)unit)->inventory[i] == 0)
      break;
    if (item[12] == 2) {
      Inventory_Break(5, i);
      break;
    }
  }
  Func_02000dc0();
}

s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn);

void Func_020005fc(s32 owner, s32 element, s32 djinn, s32 activate) {
  GameFlag_SetBit(element * 20 + djinn + 48);
  Djinn_AddToOwner(owner, element, djinn);
  if (activate == 1)
    Djinn_Activate(owner, element, djinn);
}

#if !TLA_PATCH_1
/// Gives each of four characters a set of Djinn from 7, until patch 1.
void Func_02000638(void) {
  s32 djinn;

  FieldMessage_Show((s32)MsgYouGotA, 1);
  for (djinn = 7; djinn < 13; djinn++)
    Func_020005fc(4, 0, djinn, 1);
  for (djinn = 7; djinn < 14; djinn++)
    Func_020005fc(5, 2, djinn, 1);
  for (djinn = 7; djinn < 13; djinn++)
    Func_020005fc(6, 3, djinn, 1);
  for (djinn = 7; djinn < 14; djinn++)
    Func_020005fc(7, 1, djinn, 1);
  Func_02000dc0();
}
#else
void Func_02000638(void) {
  s32 element;
  s32 djinn;

  FieldMessage_Show((s32)MsgYouGotA, 1);
  for (element = 0; element < 4; element++) {
    for (djinn = 0; djinn < 18; djinn++)
      Djinn_AddToLeastLoadedOwner(element, djinn);
  }
  Func_02000dc0();
}
#endif

void Func_02000664(void) {}

void Func_02000e40(s32 kind, s32 value);
void Func_02000e48(void);
void Func_02000e50(s32 kind, s32 value);
void Func_02000e60(s32 *a, s32 *b);
void Func_02000d20(s32 index);
void Func_02000d38(s32 value);
void Func_02000dd0(s32 value);
void Func_02000d28(void);
void Func_02000d30(void);

void Func_02000668(void) {
  PartyInventory_GiveItem(11, 0);
  Func_02000d38(22);
  Func_02000dd0(22);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_02000db0(void);

/// The scene's first entry: sets screen effect 256 and event mode 2, puts
/// objects 16 to 19 in mode 5 and 30 to 33 in mode 6, and runs
/// Func_02000db0 when the party came in by entrance 99.
s32 Func_02000688(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 *effect = &work->screenEffect;
  struct MainRoutineWork *party = &gMainRoutineWork;

  *effect = 256;
  party->eventMode = 2;
  Actor_SetModeById(16, 5);
  Actor_SetModeById(17, 5);
  Actor_SetModeById(18, 5);
  Actor_SetModeById(19, 5);
  Actor_SetModeById(30, 6);
  Actor_SetModeById(31, 6);
  Actor_SetModeById(32, 6);
  Actor_SetModeById(33, 6);
  if (party->entrance == 99)
    Func_02000db0();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Unit_Calculate(s32 owner);

void Func_02000704(s32 levels) {
  s16 owners[16];
  s32 count = FieldParty_ListMembers(owners);
  s32 i;

  for (i = 0; i < count;) {
    u16 owner = owners[i++];

    Unit_LevelUpTo(owner,
                   ((struct BattleUnit *)Unit_Get(owner))->level + levels);
    Unit_Calculate(owner);
  }
}

extern const u8 gData_02000e80[];

void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Text_DrawMessage(s32 message, s32 window, s32 x, s32 y);
void Text_DrawStringAtOffset(const void *text, void *window, s32 x, s32 y);
void Text_DrawStringInWindow(const void *text, void *window, s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 a, void *window, s32 x, s32 y);
void WindowSprite_RedrawSavedRect(void *window);
void Window_Close(void *window, s32 mode);
void Func_02000704(s32 levels);

/// The level-raising window: shows the current member's name and level,
/// raises every active member 5 levels on Start or Select and 1 on A, and
/// closes on B.
// ☀️'s debug EV11_TALK.
void Func_02000748(void) {
  u8 *unit = Unit_Get(gMainRoutineWork.currentOwner);
  void *window = Window_Create(0, 0, 30, 9, 2);
  const u8 *message = MsgRaiseEveryoneS;
  s32 redraw;

  Text_DrawMessage(message, window, 0, 0);
  Text_DrawMessage(message + 1, window, 0, 16);
  redraw = 1;
  Text_DrawMessage(message + 2, window, 0, 32);
  for (;;) {
    if (redraw) {
      WindowSprite_RedrawSavedRect(window);
      Text_DrawStringAtOffset(unit, window, 0, 48);
      Text_DrawStringInWindow(gData_02000e80, window, 48, 48);
      redraw = 0;
      Text_DrawNumberInWindow(unit[15], 0, window, 72, 48);
    }
    wait(1);
    if ((gInput.pressed & 8) || (gInput.pressed & 4)) {
      Func_02000704(5);
      Sound_Play(93);
      redraw = 1;
    }
    if (gInput.pressed & 1) {
      Func_02000704(1);
      Sound_Play(91);
      redraw = 1;
    }
    if (gInput.pressed & 2)
      break;
  }
  Sound_Play(113);
  Window_Close(window, 2);
}

extern const u8 gData_02000e84[];
extern const u8 gData_02000e90[];
extern const u8 gData_02000ea8[];
extern u8 MsgItemName[];
extern u8 MsgItemPlainName[];

struct ItemSummary {
  u8 unk00[6];
  u16 icon;
};

void WindowSprite_PrepareForRedraw(void *window);
void Text_DrawNumberAtOffset(s32 value, s32 digits, void *window, s32 x, s32 y);
void Text_DrawCharacterAtOffset(s32 message, void *window, s32 x, s32 y);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 item);
void ItemMenu_DrawItemDetails(void *window, s32 item);
void Window_Close(void *window, s32 release);

#define ITEM_COUNT 500

void Func_02000848(void) {
  void *window;
  void *details;
  s32 item;
  s32 redraw;
  s32 entryCount;
  s8 step;

  entryCount = ITEM_COUNT;
  step = 0;
  item = 1;
  redraw = 1;
  window = Window_Create(0, 0, 30, 7, 2);
  details = Window_Create(0, 8, 13, 10, 2);
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
  Sound_Play(112);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      WindowSprite_PrepareForRedraw(window);
      WindowSprite_PrepareForRedraw(details);
      Text_DrawStringInWindow(gData_02000e84, window, 0, 0);
      Text_DrawNumberAtOffset(item, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gData_02000e90, window, 0, 32);
        Text_DrawCharacterAtOffset((s32)(item + MsgItemName), window, 120, 0);
        Text_DrawMessage((s32)(item + MsgItemPlainName), window, 0, 16);
        ItemMenu_DrawItemDetails(details, item);
      } else {
        Text_DrawStringInWindow(gData_02000ea8, window, 0, 32);
      }
    }
    wait(1);
    if (gInput.pressed & 1) {
      if (PartyInventory_Add(item) == -1) {
        Sound_Play(113);
        break;
      }
      Sound_Play(175);
    }
    if (gInput.pressed & 2) {
      Sound_Play(113);
      break;
    }
    if (gInput.repeat & 0x80) {
      step = 1;
      item++;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x40) {
      step = -1;
      item--;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x10) {
      step = 1;
      item += 10;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x20) {
      step = -1;
      item -= 10;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x100) {
      step = 1;
      item += 30;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x200) {
      step = -1;
      item -= 30;
      redraw = 1;
      Sound_Play(111);
    }
    if (step == -1) {
      for (item = (item + ITEM_COUNT) % entryCount;
           Func_02000d48(item & 0x1ff)->icon == 0;
           item = (item + ITEM_COUNT - 1) % entryCount)
        ;
    }
    if (step == 1) {
      for (item = (item + ITEM_COUNT) % entryCount;
           Func_02000d48(item & 0x1ff)->icon == 0;
           item = (item + ITEM_COUNT + 1) % entryCount)
        ;
    }
    step = 0;
  }
  Window_Close(window, 2);
  Window_Close(details, 2);
}

void Func_02000aac(void) { Func_02000d28(); }

void Func_02000ab4(void) { Func_02000d30(); }

void Func_02000abc(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  if (mode == 1)
    Map_CopyCellsRect(a, b, c, d, e, f);
}

void Func_02000ae0(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;
  s32 scale;

  object = Actor_GetById(id);
  if (mode == 1) {
    Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
    object->scaleX = 0;
    object->scaleY = 0;
  }
  if (mode == 2) {
    while (object->scaleX < Q16_ONE) {
      wait(1);
      scale = object->scaleX;
      scale += 0x51e;
      object->scaleX = scale;
      object->scaleY = scale;
    }
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
  }
}

void Func_02000b50(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_GetById(id);
  if (mode == 2) {
    Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
    object->y = 0x200000;
    object->motionFlags = 3;
    object->gravity = 0x170a;
    wait(50);
  }
}

void Func_02000e20(s32 scene, s32 entrance);

/// Mode 1 places object kind 354 at (x, z) in pixels; mode 2 grows object
/// kind 252 there from 0x6666 to full size while it turns, then turns it
/// opposite the leader's facing and passes scene and entrance on to
/// Func_02000e20.
void Func_02000ba0(s32 mode, s32 x, s32 z, s32 scene, s32 entrance) {
  struct Actor *object;

  if (mode == 1) {
    object = FieldActor_Create(354, x << 16, 0, z << 16);
    if (object != NULL) {
      Actor_SetSingleChildFlags(object, 0);
      object->priorityFlags = 2;
    }
  }
  if (mode == 2) {
    object = ((struct Actor *)Import_ActorCreate(252, x << 16, 0, z << 16));
    if (object != NULL) {
      s32 scale;

      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = 0x60000;
      object->scaleX = 0x6666;
      object->scaleY = 0x6666;
      while (object->scaleX < Q16_ONE) {
        wait(1);
        scale = object->scaleX + 0x51e;
        {
          /* FAKEMATCH(temporary): the turn is added as a word, so its step
           * is built inside the loop as in the native; as
           * object->angle += 0x1000, 22 of 86 instructions differ in
           * tla-en. */
          s32 angle = object->angle + 0x1000;

          object->angle = angle;
        }
        object->scaleX = scale;
        object->scaleY = scale;
      }
      object->scaleX = Q16_ONE;
      object->scaleY = Q16_ONE;
      object->angle = Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x8000;
      Func_02000e20(scene, entrance);
    }
  }
}
