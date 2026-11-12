/// @file
/// The scene code of overlay 0x64a that its scene tables belong to, in ROM order; the rest of the overlay is in clear/.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "bg_scroll.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "main_routine_work.h"
#include "actor.h"
#include "ram_buffer.h"
#include "resource.h"
#include "event_inline.h"
#include "input.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

struct ClearWork {
  u8 unk00[20];
  u16 mode;
};

extern char ResourceId_LogoOnBlack;
extern u16 gData_020024f4[];

void Fade_ToBlack(s32 vblanks);
void Func_020021f8(void *source, void *destination);

static __inline__ void Decode_(void *source, void *destination) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps the buffer address
   * a constant of each call, as ☀️'s system call layer does; as a direct call
   * the address is kept in r5; as Func_020021f8 called directly, 21 of 103
   * instructions differ in tla-en (Func_0200005c). */
  Func_020021f8(source, destination);
}

/// The logo background, as the title's Title_LoadBackground, then the party
/// leader's motion flags cleared.
// As ☀️'s Clear_LoadBackground.
void Func_0200005c(void) {
  u8 *res;
  s32 id;
  u16 *map;
  u32 x;
  u32 y;
  u16 tile;
  struct BgScroll *scroll;
  s32 blank;

  id = (s32)&ResourceId_LogoOnBlack;
  Fade_ToBlack(0);
  REG_BG2CNT = 0x681;
  gBgScroll[2].y = 0;
  blank = 0x1ff;
  res = Resource_GetTableEntry(id);
  Dma_Set(res, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  res += 0x1c0;
  Decode_(res, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800,
          DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);

  map = (u16 *)0x06003000;
  tile = 0x1a0;
  // Written with goto: as for loops, the pinned column counter keeps its exit
  // test ahead of the loop, and the scroll loop's zero leaves it.
  y = 0;
row:
  x = 0;
column:
  *map++ = tile++;
  x++;
  if (x <= 29)
    goto column;
  *map++ = blank;
  *map++ = blank;
  y++;
  if (y <= 19)
    goto row;
  scroll = gBgScroll;
  y = 0;
scroll_pair:
  scroll->y = 0;
  scroll->x = 0;
  scroll++;
  y++;
  if (y <= 3)
    goto scroll_pair;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct ClearWork *)Ram_HeapSlots->mapWork)->mode = 0x1400;
  {
    struct Actor *leader;
    /* FAKEMATCH(pin): the flag's zero pinned to r5 as a halfword, so that it
     * loads from the pool after the call, as the native has; as a plain store
     * it reuses the scroll loop's zero kept in r5; as leader->motionFlags = 0,
     * 4 of 103 instructions differ in tla-en. */
    register u16 zero asm("r5");

    leader = Actor_Get(gMainRoutineWork.currentOwner);
    zero = 0;
    leader->motionFlags = zero;
  }
}

/// Queues an alpha blend of BG0 and the sprites over the rest whose first
/// weight rises by one every second frame (the second weight 16 less), and
/// removes the callback once the first weight passes 15.
// As ☀️'s Clear_UpdateBlend.
void Func_0200015c(void) {
  s32 frame;
  u32 level;

  frame = gData_020024f4[0] + 1;
  gData_020024f4[0] = frame;
  level = (u16)frame >> 1;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - (u16)level) << 8) | (u16)level,
                      IO_WRITE_16);
  if (level > 15)
    FieldTask_Remove(Func_0200015c);
}

// The two secret button codes: unlocked, held and progress of each.
extern s16 gData_020024f6;
extern s16 gData_020024f8;
extern s16 gData_020024fa;
extern s16 gData_020024fc;
extern s16 gData_020024fe;
extern s16 gData_02002500;
extern const u16 gData_02002410[];
extern const u16 gData_02002418[];

/// Follows the two secret button codes: each held key that matches the next
/// key of a code advances it, a wrong key starts it again, and finishing a
/// code unlocks it with a sound.
void Func_020001f4(void) {
  if (gData_020024f6 == 0) {
    /* FAKEMATCH(pin): the held keys pointer pinned to r4, with the key pinned
     * to r3 below; as an unpinned local, 12 of 92 instructions differ in
     * tla-en. */
    register volatile u32 *held asm("r4") = &gInput.held;

    if (gData_020024fe != 0) {
      /* FAKEMATCH(pin): the key pinned to r3, with the pointer above; as an
       * unpinned local, 6 of 92 instructions differ in tla-en. */
      register u32 keys asm("r3") = *held;

      if (keys == 0)
        gData_020024fe = keys;
    } else if (*held != 0) {
      if (*held == gData_02002410[gData_020024fa]) {
        /* FAKEMATCH(dead): the two stores in a once-only block, which keeps
         * them together; as plain statements, 4 of 92 instructions differ in
         * tla-en. */
        do {
          gData_020024fa++;
          gData_020024fe = 1;
        } while (0);
        if (gData_02002410[gData_020024fa] == 0) {
          gData_020024f6 = 1;
          Event_PlaySound(110);
        }
      } else {
        gData_020024fa = 0;
      }
    }
  }
  if (gData_020024f8 == 0) {
    /* FAKEMATCH(pin): the held keys pointer pinned to r4, as for the first
     * code; as an unpinned local, 12 of 92 instructions differ in tla-en. */
    register volatile u32 *held asm("r4") = &gInput.held;

    if (gData_02002500 != 0) {
      /* FAKEMATCH(pin): the key pinned to r3, as for the first code; as an
       * unpinned local, 6 of 92 instructions differ in tla-en. */
      register u32 keys asm("r3") = *held;

      if (keys == 0)
        gData_02002500 = keys;
    } else if (*held != 0) {
      if (*held == gData_02002418[gData_020024fc]) {
        /* FAKEMATCH(dead): the two stores in a once-only block, as for the
         * first code; as plain statements, 4 of 92 instructions differ in
         * tla-en. */
        do {
          gData_020024fc++;
          gData_02002500 = 1;
        } while (0);
        if (gData_02002418[gData_020024fc] == 0) {
          gData_020024f8 = 1;
          Event_PlaySound(110);
        }
      } else {
        gData_020024fc = 0;
      }
    }
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020002e0(void) {
  struct MainRoutineWork *party;
  s32 mode;

  if (Event_GetFlag(324) == 0)
    return 0;
  party = &gMainRoutineWork;

  if (FIELD_AT(party, s16, 0x25e) == 2)
    return 0;
  mode = party->scene ^ (s32)&SceneId_WorldMap;
  return -((u32)(-mode | mode) >> 31);
}
