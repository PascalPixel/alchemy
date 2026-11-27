/* The epilogue: the party's last words, line after line, each with the
   speaker's portrait in a tick beside it, over the ending's images, from
   "Finally! We've reached Vale." (MsgFinallyWeVe) to The End; then it waits for
   a button, or three minutes, and goes on to the file screen. */

/* This unit declares its own form of mnEndingImageStart, below. */
#define OWN_mnEndingImageStart

#include "scene/script_layer.h"
#include "field/field.h"
#include "game/game.h"
#include "menu/menu.h"
#include "macros.h"
#include "scene/scene.h"
#include "window/window.h"
#include "field/script_inline.h"

/* The ROM's callers here are compiled with no declaration in scope, as against
   this form: no parameter list and an Sint32 result, where the definition takes
   nothing and returns nothing. */
Sint32 mnEndingImageStart();

/* The portrait frame's VRAM cache entry. */
Sint32 EpiloguePortraitFrameEntry BSS_DATA ALIGNED(16);
/* Unused: 268 bytes after the tick's entry that the epilogue never touches. */
Uint8 EpilogueUnusedAfterPortraitFrameEntry[268] BSS_DATA;
/* The tick drawn round each portrait, a sprite under the portrait. */
EFFECT_SPR EpiloguePortraitFrame BSS_DATA;
Sint32 EpilogueMessage			 BSS_DATA; /* the next line to show */
Sint16 EpiloguePortraitY		 BSS_DATA; /* the portrait's top, in pixels */
Sint32 EpiloguePortraitFrameTile BSS_DATA; /* the tick's first tile */
/* Nonzero while a portrait is shown, and the tick drawn round it. */
Sint8 EpiloguePortraitShown BSS_DATA;
/* the portrait's left, in pixels */
Sint16 EpiloguePortraitX BSS_DATA ALIGNED(4);

/* The colours of the epilogue's name panel (green_panel.s). */
extern const Uint16 EpilogueNamePanelPalette[];

/* mnEndingImageShow as the epilogue shows each of its pictures, through an
   inline as its waits go through syFieldScriptWait (script_inline.h). */
static inline void scFieldEndingImageShow(
	Sint32 resource, Uint8 *map, Sint32 pos)
{
	mnEndingImageShow(resource, map, pos);
}

#include "text/msg_ids.h"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Each tick while a portrait is shown, draws its tick round it. */
void scEpiloguePlacePortraitFrame(void)
{
	if(EpiloguePortraitShown != 0) {
		EpiloguePortraitFrame.x = toFIXED(EpiloguePortraitX);
		EpiloguePortraitFrame.y = toFIXED(EpiloguePortraitY);
		fdEffectSprDraw(&EpiloguePortraitFrame);
	}
}

#include "shared/scene_entrances_mapless.c"

Uint32 SceneExits[] = {
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_TABLE_END},
};

/* Loads the portraits' tick, a window border round a green panel
   (RESOURCE_GREEN_PANEL) drawn in portrait slot 0's palette, and starts
   scEpiloguePlacePortraitFrame. */
void scEpilogueLoadPortraitFrame(void)
{
	void	   *buf = syHeapCallAlternatePool(syHeapAlloc, 64 * 64 / 2);
	EFFECT_SPR *spr;
	Sint32	   *slot;
	Sint32	   *tileSlot;
	Sint32		value;

	syDMAXCopy(EpilogueNamePanelPalette,
		(void *)&OBJ_PLTT_COLOR(PORTRAIT_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	spr = &EpiloguePortraitFrame;
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_GREEN_PANEL), buf);
	slot = &EpiloguePortraitFrameEntry;
	value = syVRAMCacheFindFreeEntry();
	*slot = value;
	tileSlot = &EpiloguePortraitFrameTile;
	value = mnVRAMLoad(value, 64 * 64 / 2, buf);
	*tileSlot = value;
	syFieldEffectSprInit(spr, 0, 0, OBJ_SIZE_64X64, value);
	spr->depth = OAM_DEPTH_WINDOW;
	spr->oam.pri = 0;
	spr->oam.fullCol = 0;
	spr->oam.pal = PORTRAIT_PALETTE;
	syHeapRelease(buf);
	EpiloguePortraitShown = 0;
	syFieldSetTickEvent(scEpiloguePlacePortraitFrame, EVENT_PASS_TICK);
}

/* A speaker (a sprite number) or'd with this shows the line with no portrait: a
   voice heard from off the image. */
#define EPILOGUE_NO_PORTRAIT 0x8000

/* The epilogue's message windows appear at once, without MESSAGE_WINDOW_ZOOM,
   and set an option bit that mnMessageOpenWindow never reads. */
#define EPILOGUE_WINDOW_OPTIONS 4

/* scEpilogueShowLineAt's mode for a line of seven or eight rows at the foot of
   the screen: its window is moved a row down and given no size of its own. */
#define EPILOGUE_TALL_LINE_AT_FOOT 2

/* Opens the epilogue's message window at tile (x, y) and, unless the speaker
   has EPILOGUE_NO_PORTRAIT, the speaker's portrait at tile (portraitX,
   portraitY) with its tick round it, the window lowered in mode
   EPILOGUE_TALL_LINE_AT_FOOT. Then it waits for the text, closes both windows
   and waits the given ticks. */
void scEpilogueShowLineAt(Sint32 speaker, Sint32 x, Sint32 y, Sint32 portraitX,
	Sint32 portraitY, Sint32 ticks, Sint32 mode)
{
	Sint32	noPortrait;
	WINDOW *win;
	WINDOW *portrait;

	portrait = 0;
	noPortrait = EPILOGUE_NO_PORTRAIT & speaker;
	speaker &= EPILOGUE_NO_PORTRAIT - 1;
	win = mnMessageOpenWindow(EpilogueMessage, x, y,
		MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(speaker)) |
			EPILOGUE_WINDOW_OPTIONS);
	if(!noPortrait) {
		portrait = mnPortraitOpenWindow(speaker, 0, portraitX, portraitY);
		EpiloguePortraitShown = 1;
		EpiloguePortraitX = portraitX << 3;
		EpiloguePortraitY = portraitY << 3;
	}
	if(mode == EPILOGUE_TALL_LINE_AT_FOOT) {
		win->y++;
		win->width = 0;
		win->height = 0;
	}
	while(!mnMessageAllTyped())
		syWait(1);
	syWait(1);
	mnWindowClose(win, 2);
	if(!noPortrait) {
		mnWindowClose(portrait, 2);
		EpiloguePortraitShown = noPortrait;
	}
	if(ticks > 0) {
		Sint32 i = ticks;

		do {
			syWait(1);
		} while(--i != 0);
	}
	EpilogueMessage++;
}

/* Where on the screen scEpilogueShowLine puts a line. */
typedef enum EPILOGUE_LINE_POS {
	LINE_AT_TOP,
	LINE_IN_MIDDLE,
	LINE_AT_FOOT, /* below the image */
} EPILOGUE_LINE_POS;

/* Shows the epilogue's next line, spoken by speaker (a sprite, whose portrait
   and voice it takes), at the given position, centred across the screen or,
   beside a portrait, across the columns right of it; then holds it ticks
   more. */
void scEpilogueShowLine(Sint32 speaker, Sint32 pos, Sint32 ticks)
{
	Sint32 x;
	Sint32 y;
	Sint32 width;
	Sint32 height;
	Sint32 mode = 0;
	Sint32 portraitX;
	Sint32 portraitY = 0;
	Sint32 half;
	Sint32 noPortrait;

	/* Default: x, y and the portrait's column start at 0. */
	portraitX = 0;
	x = portraitX;
	y = portraitX;
	noPortrait = speaker & EPILOGUE_NO_PORTRAIT;
	mnTextGetMessageSize(EpilogueMessage, &x, &y, &width, &height);
	if(width <= DISPLAY_TILE_WIDTH - PORTRAIT_WINDOW_TILES - 1) {
		if(!noPortrait)
			x = (DISPLAY_TILE_WIDTH - PORTRAIT_WINDOW_TILES - width) / 2 +
				PORTRAIT_WINDOW_TILES;
		else
			x = (DISPLAY_TILE_WIDTH - width) / 2;
		portraitX = x - PORTRAIT_WINDOW_TILES;
	} else {
		x = (DISPLAY_TILE_WIDTH - width) / 2;
		portraitX = x;
	}
	half = (height - 1) / 2;
	if(pos == LINE_AT_TOP) {
		y = 3;
		portraitY = half + 1;
	} else if(pos == LINE_IN_MIDDLE) {
		y = 14 - half;
		portraitY = y + half - 2;
	} else if(pos == LINE_AT_FOOT) {
		if(half == 3) {
			y = 13;
			portraitY = 15;
			mode = EPILOGUE_TALL_LINE_AT_FOOT;
		} else {
			y = 16;
			portraitY = 15;
		}
	}
	scEpilogueShowLineAt(speaker, x, y, portraitX, portraitY, ticks, mode);
}

/* Runs the whole epilogue, image after image, then sets out for the file
   screen, which offers to save the cleared game. Returns 0. */
Sint32 scInit(void)
{
	MAIN_ROUTINE_STATE *state;
	Sint32				savedSpeed;
	Sint32				cnt;
	ACTOR			   *leader;
	WIN_STATE		   *winState;
	BG_SCROLL		   *scroll;

	state = &MainRoutineStateWork;
	savedSpeed = state->msgSpeed;
	leader = fdActorGet(state->leaderActorId);
	leader->motionFlg = 0;
	state->msgSpeed = MESSAGE_SPEED_NORMAL;
	mnEndingImageStart();
	mnWindowSystemInitField();
	winState = (WIN_STATE *)RamHeapSlots->winState;
	winState->holdTilemapUpload = 1;
	winState->textMode = 1;
	EpilogueMessage = (Sint32)MsgFinallyWeVe;
	scEpilogueLoadPortraitFrame();
	syFieldScriptWait(30);
	scEpilogueShowLine(SPRITE_JENNA, LINE_AT_TOP, 40);
	scEpilogueShowLine(SPRITE_GARET, LINE_IN_MIDDLE, 40);
	scEpilogueShowLine(SPRITE_SHEBA, LINE_AT_TOP, 40);
	scEpilogueShowLine(SPRITE_IVAN, LINE_IN_MIDDLE, 40);
#if LANGUAGE == LANGUAGE_GERMAN
	scEpilogueShowLine(SPRITE_IVAN, LINE_IN_MIDDLE, 40);
#endif
	scEpilogueShowLine(SPRITE_MIA, LINE_AT_TOP, 30);
	scEpilogueShowLine(SPRITE_PIERS, LINE_IN_MIDDLE, 30);
	scEpilogueShowLine(SPRITE_KRADEN, LINE_AT_TOP, 40);
	scEpilogueShowLine(SPRITE_JENNAS_FATHER, LINE_IN_MIDDLE, 30);
	scEpilogueShowLine(SPRITE_JENNAS_MOTHER, LINE_IN_MIDDLE, 40);
	scEpilogueShowLine(SPRITE_ISAAC, LINE_AT_TOP, 40);
	scEpilogueShowLine(SPRITE_KYLE, LINE_IN_MIDDLE, 30);
	scEpilogueShowLine(SPRITE_ISAAC, LINE_AT_TOP, 30);
	scEpilogueShowLine(SPRITE_ISAAC, LINE_AT_TOP, 30);
	scEpilogueShowLine(SPRITE_JENNA, LINE_IN_MIDDLE, 40);
	scEpilogueShowLine(SPRITE_FELIX, LINE_AT_TOP, 30);
	scEpilogueShowLine(SPRITE_GARET, LINE_IN_MIDDLE, 30);
	fdScriptPlaySnd(MUS_EPILOGUE);
	scroll = BgScrollWork;
	scroll[3].y = 16;
	scroll[2].y = 16;
	scFieldEndingImageShow(
		RESOURCE_ENDING_CLIFF_DAY_PICTURE, RamMapMetatileBuffer, 0);
	syFieldScriptWait(100);
	scEpilogueShowLine(SPRITE_JENNAS_FATHER, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_JENNAS_MOTHER, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_KYLE, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_GARET, LINE_AT_FOOT, 40);
#if LANGUAGE == LANGUAGE_GERMAN
	scEpilogueShowLine(SPRITE_JENNA, LINE_AT_FOOT, 40);
#endif
	scEpilogueShowLine(SPRITE_JENNA, LINE_AT_FOOT, 40);
	/* The image pans 120 pixels right, to the three on the cliff. */
	mnEndingImageSetTargetRow(120);
	syWait(1);
	mnEndingImageWaitForScroll();
	syFieldScriptWait(30);
	/* The sunset over the cliff, shown already panned to them. */
	scFieldEndingImageShow(
		RESOURCE_ENDING_CLIFF_SUNSET_PICTURE, RamMapMetatileBuffer, 120);
	syFieldScriptWait(120);
	scEpilogueShowLine(SPRITE_JENNA, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_PIERS, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_KRADEN, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_FELIX, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_FELIX, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_FELIX, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_FELIX, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_FELIX, LINE_AT_FOOT, 30);
#if LANGUAGE == LANGUAGE_GERMAN
	scEpilogueShowLine(SPRITE_FELIX, LINE_AT_FOOT, 30);
#endif
	scEpilogueShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_KYLE, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_GARET, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_GARET, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_SHEBA, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_JENNA, LINE_AT_FOOT, 30);
#if LANGUAGE == LANGUAGE_GERMAN
	scEpilogueShowLine(SPRITE_JENNA, LINE_AT_FOOT, 30);
#endif
	scEpilogueShowLine(SPRITE_MIA, LINE_AT_FOOT, 40);
#if LANGUAGE == LANGUAGE_GERMAN
	scEpilogueShowLine(SPRITE_GARET, LINE_AT_FOOT, 30);
#endif
	scEpilogueShowLine(SPRITE_GARET, LINE_AT_FOOT, 30);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	scEpilogueShowLine(
		EPILOGUE_NO_PORTRAIT | SPRITE_MAN_IN_TEAL_SHIRT, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 30);
	scEpilogueShowLine(EPILOGUE_NO_PORTRAIT | SPRITE_WOMAN_IN_GREEN_HEADSCARF,
		LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 30);
	scEpilogueShowLine(
		EPILOGUE_NO_PORTRAIT | SPRITE_BOY_WITH_ORANGE_HAIR, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_GARET, LINE_AT_FOOT, 30);
	scFieldEndingImageShow(
		RESOURCE_ENDING_VILLAGERS_PICTURE_A, RamMapMetatileBuffer, 0);
	syFieldScriptWait(60);
	fdScriptPlaySnd(MUS_EPILOGUE_VILLAGERS);
	syFieldScriptWait(30);
	scEpilogueShowLine(SPRITE_MAN_IN_TEAL_SHIRT, LINE_AT_FOOT, 30);
#if LANGUAGE == LANGUAGE_GERMAN
	scEpilogueShowLine(SPRITE_MAN_IN_TEAL_SHIRT, LINE_AT_FOOT, 30);
#endif
	scEpilogueShowLine(SPRITE_WOMAN_IN_GREEN_HEADSCARF, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_KYLE, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_YOUNG_MAN_WITH_RED_HAIR, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_KRADEN, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_GARET, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_KRADEN, LINE_AT_FOOT, 40);
	/* The villagers' image pans 200 pixels right, to Dora by the trees. */
	mnEndingImageSetTargetRow(200);
	syWait(1);
	mnEndingImageWaitForScroll();
	syFieldScriptWait(30);
	scEpilogueShowLine(SPRITE_KYLE, LINE_AT_FOOT, 20);
	scEpilogueShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 40);
	scEpilogueShowLine(SPRITE_DORA, LINE_AT_FOOT, 20);
	scEpilogueShowLine(SPRITE_KYLE, LINE_AT_FOOT, 30);
	scEpilogueShowLine(SPRITE_DORA, LINE_AT_FOOT, 40);
	/* No image: the villagers fade to black before the embrace. */
	scFieldEndingImageShow(0, RamMapMetatileBuffer, 0);
	syFieldScriptWait(60);
	scroll[3].y = 0;
	scroll[2].y = 0;
	mnEndingImageLowerTopStrip(0);
	scFieldEndingImageShow(
		RESOURCE_ENDING_EMBRACE_PICTURE, RamMapMetatileBuffer, 0);
	syFieldScriptWait(60);
	/* The embrace stays until a button is pressed. */
	while(PerWork.push == 0)
		syWait(1);
	scFieldEndingImageShow(
		RESOURCE_ENDING_REUNION_PICTURE, RamMapMetatileBuffer, 0);
	syFieldScriptWait(120);
	/* The reunion pans 120 pixels right, along the party. */
	mnEndingImageSetTargetRow(120);
	syWait(1);
	mnEndingImageWaitForScroll();
	syFieldScriptWait(180);
	scFieldEndingImageShow(
		RESOURCE_ENDING_SUNSET_PLAIN_PICTURE, RamMapMetatileBuffer, 0);
	syFieldScriptWait(300);
	scFieldEndingImageShow(
		RESOURCE_ENDING_THE_END_PICTURE, RamMapMetatileBuffer, 0);
	syFieldScriptWait(180);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	MainRoutineStateWork.msgSpeed = savedSpeed;
	syFieldScriptWait(300);
	/* The End stays until a button is pressed, or for three minutes. */
	cnt = 0;
	if(PerWork.push == 0) {
		do {
			syWait(1);
			cnt++;
			if(cnt >= 3 * 60 * TICKS_PER_SECOND)
				break;
		} while(PerWork.push == 0);
	}
	scFieldEndingImageShow(0, RamMapMetatileBuffer, 0);
	syFieldScriptWait(60);
	syIOWrite16(0, &REG_BLDCNT);
	syIOWrite16(DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON, &REG_DISPCNT);
	syWindowLoadGraphics();
	syPrintClear();
	OamCopyEnabled = 1;
	fdScriptClearFlag(FLAG_CANNOT_SAVE_HERE);
	fdScriptSetDestination(SCENE_CLEAR, CLEAR_FROM_EPILOGUE);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}
