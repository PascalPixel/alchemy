// DjinnFigures: the row of four figures the Status and Djinn menus show, placed by an event and
// eased in and out. Part of menu.c.

#include "menu_internal.h"

// Where a new row's figures stand until their screen places them: the top row's at (FIGURE_START_X,
// TOP_ROW_START_Y), the bottom row's at FIGURE_START_X, parked below the screen
// (MENU_FIGURE_HIDDEN_Y).
#define FIGURE_START_X  16
#define TOP_ROW_START_Y 32

// Puts up the top row, the Status screen's: one walking Djinni figure of each element
// (DjinnFiguresSpriteIds), in place of any figures there, placed each tick by
// mnDjinnFiguresUpdateAllPositions.
void mnDjinnFiguresInitTopRow(void *window, Sint32 unused)
{
	ITEM_MENU_STATE *state = MenuState;
	Sint32           index;

	for (index = 0; index < 4; index++)
	{
		SPR *figure = state->figures[index];

		if (figure != NULL)
		{
			fdSpriteDestroyStub(figure);
			state->figures[index] = NULL;
		}
	}

	for (index = 0; index < 4; index++)
	{
		SPR *figure = fdSpriteCreateStub(DjinnFiguresSpriteIds[index]);

		if (figure != NULL)
		{
			fdSpriteStartAnimationStub(figure, ANIM_WALK);
		}
		state->figures[index] = figure;
		state->figureX[index] = FIGURE_START_X;
		state->figureY[index] = TOP_ROW_START_Y;
	}
	sySetEvent(mnDjinnFiguresUpdateAllPositions, EVENT_PASS_TICK);
}

// Releases the four sprites and removes their event.
#define DJINN_FIGURES_DEFINE_RELEASE(name, update)                                                 \
	void name(void)                                                                                \
	{                                                                                              \
		ITEM_MENU_STATE *state = MenuState;                                                        \
		Sint32           index;                                                                    \
                                                                                                   \
		for (index = 0; index < 4; index++)                                                        \
		{                                                                                          \
			SPR *figure = state->figures[index];                                                   \
                                                                                                   \
			if (figure != NULL)                                                                    \
			{                                                                                      \
				fdSpriteDestroyStub(figure);                                                       \
				state->figures[index] = NULL;                                                      \
			}                                                                                      \
		}                                                                                          \
		syCloseEvent(update);                                                                      \
	}

// Releases the top row's four figures and removes their event.
DJINN_FIGURES_DEFINE_RELEASE(mnDjinnFiguresReleaseTopRow, mnDjinnFiguresUpdateAllPositions)

// The figures' full size, and the height they are placed from: 482 pixels.
#define FIGURE_FULL_SCALE ((Uint32)toFIXED(1))
#define FIGURE_ORIGIN_Y   toFIXED(482)
// The bit of a figure's y that mnDjinnFiguresSetSlotPos sets to turn it west.
#define FIGURE_WEST_FLAG  0x8000

// Event while the top row is up: places its four figures at full size where the screen has put
// them.
void mnDjinnFiguresUpdateAllPositions(void)
{
	ITEM_MENU_STATE *state = MenuState;
	Sint32           index;
	SPR_SCALE        scale;
	SPR_PLACEMENT    placement;

	index = 0;
	while (1)
	{
		SPR *figure = state->figures[index];

		if (figure != NULL)
		{
			Uint32 vertical        = FIGURE_ORIGIN_Y - toFIXED(state->memberY[index]);
			figure->image.priority = 0;
			scale.x                = FIGURE_FULL_SCALE;
			scale.y                = FIGURE_FULL_SCALE;
			placement.x            = toFIXED((Sint32)state->figureX[index]);
			placement.height       = vertical;
			placement.y            = toFIXED((Sint32)state->figureY[index]) + vertical;
			placement.groundHeight = 0;
			fdRenderApplyProjectedPlacementStub(figure, &placement, &scale, FACING_SOUTH);
		}
		index += 1;
		if (index > 3)
			break;
	}
}

// Event while the bottom row is up: places its figures at their fixed scale or growing toward full
// size, facing south, or west for a figure whose y carries FIGURE_WEST_FLAG
// (mnDjinnFiguresSetSlotPos).
void mnDjinnFiguresUpdateBottomRow(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32           index;
	SPR_SCALE        scale;
	SPR_PLACEMENT    placement;

	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	for (index = 0; index <= 3; index++)
	{
		SPR *figure = menu->figures[index];

		if (figure != NULL)
		{
			Uint32 projectedY = FIGURE_ORIGIN_Y - toFIXED(menu->memberY[index]);
			Sint32 phase;
			Sint32 facing;

			figure->image.priority = 0;
			// A negative phase is a fixed scale of -phase; otherwise the scale closes a third of
			// its gap to full size (toFIXED(1)) each tick.
			phase                  = menu->figureScales[index];
			if (phase < 0)
			{
				scale.x = -phase;
				scale.y = -phase;
			}
			else
			{
				scale.x                   = phase + (Sint32)(toFIXED(1) - phase) / 3;
				scale.y                   = scale.x;
				menu->figureScales[index] = scale.x;
			}
			placement.x            = toFIXED(menu->figureX[index]);
			placement.height       = projectedY;
			placement.y            = toFIXED(menu->figureY[index]) + projectedY;
			placement.groundHeight = 0;
			if (menu->figureY[index] & FIGURE_WEST_FLAG)
				facing = FACING_WEST;
			else
				facing = FACING_SOUTH;
			fdRenderApplyProjectedPlacementStub(figure, &placement, &scale, facing);
		}
	}
}

// Puts up the bottom row, the Djinn menu's: one walking Djinni figure of each element at full size,
// in place of any there, parked below the screen until the menu moves one.
void mnDjinnFiguresInitBottomRow(Sint32 window, Sint32 unused)
{
	ITEM_MENU_STATE *state = MenuState;
	Sint32           index;

	for (index = 0; index < 4; index++)
	{
		SPR *figure = state->figures[index];

		if (figure != NULL)
		{
			fdSpriteDestroyStub(figure);
			state->figures[index] = NULL;
		}
	}
	for (index = 0; index < 4; index++)
	{
		SPR *figure = fdSpriteCreateStub(DjinnFiguresSpriteIds[index]);

		if (figure != NULL)
			fdSpriteStartAnimationStub(figure, ANIM_WALK);
		state->figures[index]      = figure;
		state->figureScales[index] = toFIXED(1);
		state->figureX[index]      = FIGURE_START_X;
		state->figureY[index]      = MENU_FIGURE_HIDDEN_Y;
	}
	sySetEvent(mnDjinnFiguresUpdateBottomRow, EVENT_PASS_TICK);
}

// Places figure index at (x, y). With negative set, y keeps its low bits and is marked negative
// from bit 15 up, and the bottom row faces that figure west; no caller sets it. Returns where
// figureY[index] lies in the menu state, or index when there is no figure.
Sint32 mnDjinnFiguresSetSlotPos(Sint32 index, Sint32 x, Sint32 y, Sint32 negative)
{
	ITEM_MENU_STATE *state = MenuState;

	if (state->figures[index] != NULL)
	{
		state->figureX[index] = x;
		state->figureY[index] = negative != 0 ? y | (Sint32)0xffff8000 : y;
		return offsetof(ITEM_MENU_STATE, figureY) + index * 2;
	}
	return index;
}

// Sets figure index's scale phase (16.16): negative for a fixed scale of -phase, otherwise the
// scale it grows from toward full size.
Sint32 mnDjinnFiguresSetSlotPhase(Sint32 index, FIXED phase)
{
	((ITEM_MENU_STATE *)MenuState)->figureScales[index] = phase;
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Makes figure index the Djinni figure of element (DjinnFiguresSpriteIds) playing `animation`.
// Returns 1.
Sint32 mnDjinnFiguresReplaceSlot(Sint32 index, Sint32 element, Sint32 animation)
{
	ITEM_MENU_STATE *state = MenuState;

	if (state->figures[index] != NULL)
	{
		fdSpriteDestroyStub(state->figures[index]);
		state->figures[index] = NULL;
	}
	{
		SPR *figure = fdSpriteCreateStub(DjinnFiguresSpriteIds[element]);

		if (figure != NULL)
			fdSpriteStartAnimationStub(figure, animation);
		state->figures[index] = figure;
	}
	return 1;
}

// Releases the bottom row's four figures and removes their event.
DJINN_FIGURES_DEFINE_RELEASE(mnDjinnFiguresReleaseBottomRow, mnDjinnFiguresUpdateBottomRow)

// Has each party figure stand (ANIM_STAND). Nothing calls it.
UNUSED void mnStartPartyFiguresStanding(void)
{
	ITEM_MENU_STATE *menu = MenuState;
	Sint32           index;

	for (index = 0; index < menu->partyCount; ++index)
	{
		fdSpriteStartAnimationStub(menu->memberSprites[index], ANIM_STAND);
	}
}

#undef DJINN_FIGURES_DEFINE_RELEASE
#undef FIGURE_FULL_SCALE
#undef FIGURE_ORIGIN_Y
#undef FIGURE_START_X
#undef TOP_ROW_START_Y
