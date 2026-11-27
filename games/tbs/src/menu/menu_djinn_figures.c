/* DjinnFigures: the row of four figures the Status and Djinn menus show, placed
   by an event and eased in and out. Part of menu.c. */

#include "menu_internal.h"

/* Where a new row's figures stand until their screen places them: the top row's
   at (FIGURE_START_X, TOP_ROW_START_Y), the bottom row's at FIGURE_START_X,
   parked below the screen (MENU_FIGURE_HIDDEN_Y). */
#define FIGURE_START_X	16
#define TOP_ROW_START_Y 32

/* Puts up the top row, the Status screen's: one walking Djinni figure of each
   element (DjinnFiguresSpriteIds), in place of any figures there, placed each
   tick by mnDjinnFiguresUpdateAllPoss. */
void mnDjinnFiguresInitTopRow(void *win, Sint32 unused)
{
	ITEM_MENU_STATE *state = MenuState;
	Sint32			 idx;

	for(idx = 0; idx < 4; idx++) {
		SPR *figure = state->figures[idx];

		if(figure != NULL) {
			fdSprDestroyStub(figure);
			state->figures[idx] = NULL;
		}
	}

	for(idx = 0; idx < 4; idx++) {
		SPR *figure = fdSprCreateStub(DjinnFiguresSpriteIds[idx]);

		if(figure != NULL) {
			fdSprStartAnimationStub(figure, ANIM_WALK);
		}
		state->figures[idx] = figure;
		state->figureX[idx] = FIGURE_START_X;
		state->figureY[idx] = TOP_ROW_START_Y;
	}
	sySetEvent(mnDjinnFiguresUpdateAllPoss, EVENT_PASS_TICK);
}

/* Releases the four sprites and removes their event. */
#define DJINN_FIGURES_DEFINE_RELEASE(name, update) \
	void name(void) \
	{ \
		ITEM_MENU_STATE *state = MenuState; \
		Sint32			 idx; \
\
		for(idx = 0; idx < 4; idx++) { \
			SPR *figure = state->figures[idx]; \
\
			if(figure != NULL) { \
				fdSprDestroyStub(figure); \
				state->figures[idx] = NULL; \
			} \
		} \
		syCloseEvent(update); \
	}

/* Releases the top row's four figures and removes their event. */
DJINN_FIGURES_DEFINE_RELEASE(
	mnDjinnFiguresReleaseTopRow, mnDjinnFiguresUpdateAllPoss)

/* The figures' full size, and the height they are placed from: 482 pixels. */
#define FIGURE_FULL_SCALE ((Uint32)toFIXED(1))
#define FIGURE_ORIGIN_Y	  toFIXED(482)
/* The bit of a figure's y that mnDjinnFiguresSetSlotPos sets to turn it
   west. */
#define FIGURE_WEST_FLAG  0x8000

/* Event while the top row is up: places its four figures at full size where the
   screen has put them. */
void mnDjinnFiguresUpdateAllPoss(void)
{
	ITEM_MENU_STATE *state = MenuState;
	Sint32			 idx;
	SPR_SCALE		 scale;
	SPR_PLACEMENT	 placement;

	idx = 0;
	while(1) {
		SPR *figure = state->figures[idx];

		if(figure != NULL) {
			Uint32 vertical = FIGURE_ORIGIN_Y - toFIXED(state->memberY[idx]);
			figure->image.pri = 0;
			scale.x = FIGURE_FULL_SCALE;
			scale.y = FIGURE_FULL_SCALE;
			placement.x = toFIXED((Sint32)state->figureX[idx]);
			placement.height = vertical;
			placement.y = toFIXED((Sint32)state->figureY[idx]) + vertical;
			placement.groundHeight = 0;
			fdRenderApplyProjectedPlacementStub(
				figure, &placement, &scale, FACING_SOUTH);
		}
		idx += 1;
		if(idx > 3)
			break;
	}
}

/* Event while the bottom row is up: places its figures at their fixed scale or
   growing toward full size, facing south, or west for a figure whose y carries
   FIGURE_WEST_FLAG (mnDjinnFiguresSetSlotPos). */
void mnDjinnFiguresUpdateBottomRow(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 idx;
	SPR_SCALE		 scale;
	SPR_PLACEMENT	 placement;

	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	for(idx = 0; idx <= 3; idx++) {
		SPR *figure = menu->figures[idx];

		if(figure != NULL) {
			Uint32 projectedY = FIGURE_ORIGIN_Y - toFIXED(menu->memberY[idx]);
			Sint32 phase;
			Sint32 facing;

			figure->image.pri = 0;
			/* A negative phase is a fixed scale of -phase; otherwise the scale
			   closes a third of its gap to full size (toFIXED(1)) each
			   tick. */
			phase = menu->figureScales[idx];
			if(phase < 0) {
				scale.x = -phase;
				scale.y = -phase;
			} else {
				scale.x = phase + (Sint32)(toFIXED(1) - phase) / 3;
				scale.y = scale.x;
				menu->figureScales[idx] = scale.x;
			}
			placement.x = toFIXED(menu->figureX[idx]);
			placement.height = projectedY;
			placement.y = toFIXED(menu->figureY[idx]) + projectedY;
			placement.groundHeight = 0;
			if(menu->figureY[idx] & FIGURE_WEST_FLAG)
				facing = FACING_WEST;
			else
				facing = FACING_SOUTH;
			fdRenderApplyProjectedPlacementStub(
				figure, &placement, &scale, facing);
		}
	}
}

/* Puts up the bottom row, the Djinn menu's: one walking Djinni figure of each
   element at full size, in place of any there, parked below the screen until
   the menu moves one. */
void mnDjinnFiguresInitBottomRow(Sint32 win, Sint32 unused)
{
	ITEM_MENU_STATE *state = MenuState;
	Sint32			 idx;

	for(idx = 0; idx < 4; idx++) {
		SPR *figure = state->figures[idx];

		if(figure != NULL) {
			fdSprDestroyStub(figure);
			state->figures[idx] = NULL;
		}
	}
	for(idx = 0; idx < 4; idx++) {
		SPR *figure = fdSprCreateStub(DjinnFiguresSpriteIds[idx]);

		if(figure != NULL)
			fdSprStartAnimationStub(figure, ANIM_WALK);
		state->figures[idx] = figure;
		state->figureScales[idx] = toFIXED(1);
		state->figureX[idx] = FIGURE_START_X;
		state->figureY[idx] = MENU_FIGURE_HIDDEN_Y;
	}
	sySetEvent(mnDjinnFiguresUpdateBottomRow, EVENT_PASS_TICK);
}

/* Places figure index at (x, y). With negative set, y keeps its low bits and is
   marked negative from bit 15 up, and the bottom row faces that figure west; no
   caller sets it. Returns where figureY[index] lies in the menu state, or index
   when there is no figure. */
Sint32 mnDjinnFiguresSetSlotPos(Sint32 idx, Sint32 x, Sint32 y, Sint32 negative)
{
	ITEM_MENU_STATE *state = MenuState;

	if(state->figures[idx] != NULL) {
		state->figureX[idx] = x;
		state->figureY[idx] = negative != 0 ? y | (Sint32)0xffff8000 : y;
		return offsetof(ITEM_MENU_STATE, figureY) + idx * 2;
	}
	return idx;
}

/* Sets figure index's scale phase (16.16): negative for a fixed scale of
   -phase, otherwise the scale it grows from toward full size. */
Sint32 mnDjinnFiguresSetSlotPhase(Sint32 idx, FIXED phase)
{
	((ITEM_MENU_STATE *)MenuState)->figureScales[idx] = phase;
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

/* Makes figure index the Djinni figure of element (DjinnFiguresSpriteIds)
   playing `anm`. Returns 1. */
Sint32 mnDjinnFiguresReplaceSlot(Sint32 idx, Sint32 element, Sint32 anm)
{
	ITEM_MENU_STATE *state = MenuState;

	if(state->figures[idx] != NULL) {
		fdSprDestroyStub(state->figures[idx]);
		state->figures[idx] = NULL;
	}
	{
		SPR *figure = fdSprCreateStub(DjinnFiguresSpriteIds[element]);

		if(figure != NULL)
			fdSprStartAnimationStub(figure, anm);
		state->figures[idx] = figure;
	}
	return 1;
}

/* Releases the bottom row's four figures and removes their event. */
DJINN_FIGURES_DEFINE_RELEASE(
	mnDjinnFiguresReleaseBottomRow, mnDjinnFiguresUpdateBottomRow)

/* Has each party figure stand (ANIM_STAND). Nothing calls it. */
UNUSED void mnStartPartyFiguresStanding(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32			 idx;

	for(idx = 0; idx < menu->partyCnt; ++idx) {
		fdSprStartAnimationStub(menu->memberSprs[idx], ANIM_STAND);
	}
}

#undef DJINN_FIGURES_DEFINE_RELEASE
#undef FIGURE_FULL_SCALE
#undef FIGURE_ORIGIN_Y
#undef FIGURE_START_X
#undef TOP_ROW_START_Y
