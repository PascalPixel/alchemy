/// @file
/// The Lemurian ship's deck, in two scenes: the deck at the tiller, where the
/// crew takes its places as the leader sails (Room1), and the deck where the
/// party sights a ship, reaches Piers' ship, sets the Black Crystal back in it
/// and sets out for Madra (Room2).

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
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"
#include "io_reg.h"
#include "map.h"
#include "frame.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "text/msg_ids.h"

extern s32 gLemurianShipDeck_WaveScroll;
extern s32 gLemurianShipDeck_BobPhase;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gLemurianShipDeck_Placements[];
extern const u8 gLemurianShipDeck_Room1Placements[];

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov657Room1)
    return gLemurianShipDeck_Room1Placements;
  return gLemurianShipDeck_Placements;
}

/// Once flag 0x985 is set, the leader goes through the cabin door, the door
/// opening first unless it already has (flag 0x201).
void LemurianShipDeck_EnterCabin(void) {
  if (Event_GetFlag(0x985)) {
    u8 *work = Ram_HeapSlots->eventWork;
    u32 i;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    // The loop is written with goto so that it carries no loop notes: the bytes
    // rebuild the zero and the height on every pass, and a for loop hoists both
    // (differ 39).
    i = 8;
  loop:
    {
      struct Actor *object = Actor_Get(i);

      if (object != 0) {
        object->motionFlags = 0;
        object->y = PIXELS(32);
      }
    }
    i++;
    if (i <= 63)
      goto loop;
    if (!Event_GetFlag(0x201)) {
      Event_PlaySound(158);
      MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
    }
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
    FieldEvent_Wait(8);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_End();
  }
}

extern const u8 gLemurianShipDeck_Triggers[];
extern const u8 gLemurianShipDeck_Room2Triggers[];

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov657Room2)
    return gLemurianShipDeck_Room2Triggers;
  return gLemurianShipDeck_Triggers;
}

/// ★ Named from the game: "The door is held closed by a powerful force."
/// (MsgTheDoorIsHeld)
void LemurianShipDeck_TryHeldDoor(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgTheDoorIsHeld, 1);
  Event_End();
}

#define OBJ_PALETTE ((volatile u16 *)&BG_PLTT_COLOR(11, 0))

#define DEFINE_ROTATE_PALETTE_COLORS(name, n1, n2)                             \
  void name(void) {                                                            \
    if ((gFrameCount & 7) == 0) {                                              \
      s16 last = OBJ_PALETTE[n1];                                              \
      s32 i;                                                                   \
                                                                               \
      /* The loop is written with goto so that it carries no loop notes: the   \
       * bytes */                                                              \
      /* rebuild the palette base on every pass, and a for or do loop hoists   \
       * it */                                                                 \
      /* and steps two pointers (differ 30). */                                \
      i = n1;                                                                  \
    loop:                                                                      \
      OBJ_PALETTE[i] = OBJ_PALETTE[i - 1];                                     \
      i--;                                                                     \
      if (i != n2)                                                             \
        goto loop;                                                             \
      OBJ_PALETTE[n2] = last;                                                  \
    }                                                                          \
  }

DEFINE_ROTATE_PALETTE_COLORS(LemurianShipDeck_RotateColors12To15, 15, 12)

DEFINE_ROTATE_PALETTE_COLORS(LemurianShipDeck_RotateColors4To9, 9, 4)

void Map_ClearLayerEntryFlag(s32 a);

/// Arriving by entrance 99: the screen opens and two of the map's layer entries
/// are cleared in turn.
void LemurianShipDeck_ShowArrival(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  FieldTask_AddFrame(LemurianShipDeck_RotateColors4To9, 0x480);
  Event_WaitTransitionFrames();
  FieldEvent_Wait(60);
  Map_ClearLayerEntryFlag(13);
  FieldEvent_Wait(60);
  FieldEvent_Wait(60);
  Map_ClearLayerEntryFlag(14);
  Event_End();
}

#define LAYER_WORD(layer, offset) (*(s32 *)((u8 *)(layer) + (offset)))

/// A task: scrolls the waves' layer half a pixel a frame.
void LemurianShipDeck_ScrollWaves(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x108);
  s32 phase = gLemurianShipDeck_WaveScroll;

  layer[2] = phase;
  phase += 0x8000;
  gLemurianShipDeck_WaveScroll = phase;
  if (phase == 0x200000)
    gLemurianShipDeck_WaveScroll = 0;
}

/// A task: bobs the camera's actor and the first map layer together on a sine
/// wave, its phase advancing 0x200 a frame.
void LemurianShipDeck_BobDeck(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];
  struct Actor *object = EventCamera_GetActor();
  s32 offset = Math_Sin(gLemurianShipDeck_BobPhase) >> 2;

  object->y += offset;
  LAYER_WORD(layer, 12) += offset;
  gLemurianShipDeck_BobPhase += 0x200;
}
