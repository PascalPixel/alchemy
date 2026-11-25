/// @file
/// DjinnFigures: the row of four figures the Status and Djinn menus show,
/// placed by a task and eased in and out.
// Part of menu.c.

#include "menu_internal.h"

/// Where a new row's figures stand until their screen places them: the top
/// row's at (FIGURE_START_X, TOP_ROW_START_Y), the bottom row's at
/// FIGURE_START_X, parked below the screen (MENU_FIGURE_HIDDEN_Y).
#define FIGURE_START_X 16
#define TOP_ROW_START_Y 32

/// Puts up the top row, the Status screen's: one walking Djinni figure of each
/// element (gDjinnFigures_SpriteIds), in place of any figures there, placed
/// each frame by DjinnFigures_UpdateAllPositions.
void DjinnFigures_InitializeTopRow(void *window, s32 unused) {
  struct ItemMenuState *state = gMenuState;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct Sprite *figure = state->figures[index];

    if (figure != NULL) {
      Sprite_DestroyFar(figure);
      state->figures[index] = NULL;
    }
  }

  for (index = 0; index < 4; index++) {
    struct Sprite *figure = Sprite_CreateFar(gDjinnFigures_SpriteIds[index]);

    if (figure != NULL) {
      Sprite_StartAnimationFar(figure, ANIM_WALK);
    }
    state->figures[index] = figure;
    state->figureX[index] = FIGURE_START_X;
    state->figureY[index] = TOP_ROW_START_Y;
  }
  Task_Add(DjinnFigures_UpdateAllPositions, TASK_PASS_FRAME);
}

/// Releases the four sprites and removes their task.
#define DJINN_FIGURES_DEFINE_RELEASE(name, update)                             \
  void name(void) {                                                            \
    struct ItemMenuState *state = gMenuState;                                  \
    s32 index;                                                                 \
                                                                               \
    for (index = 0; index < 4; index++) {                                      \
      struct Sprite *figure = state->figures[index];                           \
                                                                               \
      if (figure != NULL) {                                                    \
        Sprite_DestroyFar(figure);                                             \
        state->figures[index] = NULL;                                          \
      }                                                                        \
    }                                                                          \
    Task_Remove(update);                                                       \
  }

/// Releases the top row's four figures and removes their task.
DJINN_FIGURES_DEFINE_RELEASE(DjinnFigures_ReleaseTopRow,
                             DjinnFigures_UpdateAllPositions)

/// The figures' full size, and the height they are placed from: 482 pixels.
#define FIGURE_FULL_SCALE ((u32)FIXED_ONE)
#define FIGURE_ORIGIN_Y toFIXED(482)
/// The bit of a figure's y that DjinnFigures_SetSlotPosition sets to turn it
/// west.
#define FIGURE_WEST_FLAG 0x8000

/// Task while the top row is up: places its four figures at full size where
/// the screen has put them.
void DjinnFigures_UpdateAllPositions(void) {
  struct ItemMenuState *state = gMenuState;
  s32 index;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  index = 0;
  while (1) {
    struct Sprite *figure = state->figures[index];

    if (figure != NULL) {
      u32 vertical = FIGURE_ORIGIN_Y - toFIXED(state->memberY[index]);
      figure->image.priority = 0;
      scale.x = FIGURE_FULL_SCALE;
      scale.y = FIGURE_FULL_SCALE;
      placement.x = toFIXED((s32)state->figureX[index]);
      placement.height = vertical;
      placement.y = toFIXED((s32)state->figureY[index]) + vertical;
      placement.groundHeight = 0;
      Render_ApplyProjectedPlacementFar(figure, &placement, &scale,
                                        FACING_SOUTH);
    }
    index += 1;
    if (index > 3)
      break;
  }
}

/// Task while the bottom row is up: places its figures at their fixed scale or
/// growing toward full size, facing south, or west for a figure whose y
/// carries FIGURE_WEST_FLAG (DjinnFigures_SetSlotPosition).
void DjinnFigures_UpdateBottomRow(void) {
  struct ItemMenuState *menu = gMenuState;
  s32 index;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  PageArrows_DrawFar(PAGE_ARROW_VRAM);
  for (index = 0; index <= 3; index++) {
    struct Sprite *figure = menu->figures[index];

    if (figure != NULL) {
      u32 projectedY = FIGURE_ORIGIN_Y - toFIXED(menu->memberY[index]);
      s32 phase;
      s32 facing;

      figure->image.priority = 0;
      // A negative phase is a fixed scale of -phase; otherwise the scale
      // closes a third of its gap to full size (FIXED_ONE) each frame.
      phase = menu->figureScales[index];
      if (phase < 0) {
        scale.x = -phase;
        scale.y = -phase;
      } else {
        scale.x = phase + (s32)(FIXED_ONE - phase) / 3;
        scale.y = scale.x;
        menu->figureScales[index] = scale.x;
      }
      placement.x = toFIXED(menu->figureX[index]);
      placement.height = projectedY;
      placement.y = toFIXED(menu->figureY[index]) + projectedY;
      placement.groundHeight = 0;
      if (menu->figureY[index] & FIGURE_WEST_FLAG)
        facing = FACING_WEST;
      else
        facing = FACING_SOUTH;
      Render_ApplyProjectedPlacementFar(figure, &placement, &scale, facing);
    }
  }
}

/// Puts up the bottom row, the Djinn menu's: one walking Djinni figure of each
/// element at full size, in place of any there, parked below the screen until
/// the menu moves one.
void DjinnFigures_InitializeBottomRow(s32 window, s32 unused) {
  struct ItemMenuState *state = gMenuState;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct Sprite *figure = state->figures[index];

    if (figure != NULL) {
      Sprite_DestroyFar(figure);
      state->figures[index] = NULL;
    }
  }
  for (index = 0; index < 4; index++) {
    struct Sprite *figure = Sprite_CreateFar(gDjinnFigures_SpriteIds[index]);

    if (figure != NULL)
      Sprite_StartAnimationFar(figure, ANIM_WALK);
    state->figures[index] = figure;
    state->figureScales[index] = FIXED_ONE;
    state->figureX[index] = FIGURE_START_X;
    state->figureY[index] = MENU_FIGURE_HIDDEN_Y;
  }
  Task_Add(DjinnFigures_UpdateBottomRow, TASK_PASS_FRAME);
}

/// Places figure index at (x, y). With negative set, y keeps its low bits and
/// is marked negative from bit 15 up, and the bottom row faces that figure
/// west; no caller sets it. Returns where figureY[index] lies in the menu
/// state, or index when there is no figure.
s32 DjinnFigures_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative) {
  struct ItemMenuState *state = gMenuState;

  if (state->figures[index] != NULL) {
    state->figureX[index] = x;
    state->figureY[index] = negative != 0 ? y | (s32)0xffff8000 : y;
    return offsetof(struct ItemMenuState, figureY) + index * 2;
  }
  return index;
}

/// Sets figure index's scale phase (16.16): negative for a fixed scale of
/// -phase, otherwise the scale it grows from toward full size.
s32 DjinnFigures_SetSlotPhase(s32 index, FIXED phase) {
  ((struct ItemMenuState *)gMenuState)->figureScales[index] = phase;
#ifdef UBFIX
  return 0;
#endif
}

/// Makes figure index the Djinni figure of element (gDjinnFigures_SpriteIds)
/// playing `animation`. Returns 1.
s32 DjinnFigures_ReplaceSlot(s32 index, s32 element, s32 animation) {
  struct ItemMenuState *state = gMenuState;

  if (state->figures[index] != NULL) {
    Sprite_DestroyFar(state->figures[index]);
    state->figures[index] = NULL;
  }
  {
    struct Sprite *figure = Sprite_CreateFar(gDjinnFigures_SpriteIds[element]);

    if (figure != NULL)
      Sprite_StartAnimationFar(figure, animation);
    state->figures[index] = figure;
  }
  return 1;
}

/// Releases the bottom row's four figures and removes their task.
DJINN_FIGURES_DEFINE_RELEASE(DjinnFigures_ReleaseBottomRow,
                             DjinnFigures_UpdateBottomRow)

/// Has each party figure stand (ANIM_STAND). Nothing calls it.
UNUSED void Menu_StartPartyFiguresStanding(void) {
  struct ItemMenuState *menu = gMenuState;
  s32 index;

  for (index = 0; index < menu->partyCount; ++index) {
    Sprite_StartAnimationFar(menu->memberSprites[index], ANIM_STAND);
  }
}

#undef DJINN_FIGURES_DEFINE_RELEASE
#undef FIGURE_FULL_SCALE
#undef FIGURE_ORIGIN_Y
#undef FIGURE_START_X
#undef TOP_ROW_START_Y
