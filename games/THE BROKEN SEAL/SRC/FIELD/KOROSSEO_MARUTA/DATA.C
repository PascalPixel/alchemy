#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "EDITION.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"
#include "OBJCMD.H"
#include "text/MSG_IDS.H"

void ColossoLogRollingStage_SetSceneEventValues(void);

s32 KorosseoMaruta_Record30Script[] = {
	SC_SET(SW_Y, PIXELS(1)),
	SC_WAIT(10),
	SC_SET(SW_Y, 0),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 KorosseoMaruta_LogPattern[] = {
	0x5070507, 0x6040b07, 0xa040604, 0x7050705, 0x4060c05, 0xa060406,
};

u32 gColossoEarlySequenceData[] = {
	0, 0x80004000, 0x80008000, 0xc000,
};

s32 gColossoSceneEventEffect[] = {
	SC_MOVE(PIXELS(1088), 0, PIXELS(184)),
	SC_SYNC,
	SC_MOVE(PIXELS(1130), 0, PIXELS(184)),
	SC_SYNC,
	SC_CALL(ColossoLogRollingStage_SetSceneEventValues),
	SC_LABEL(1),
	SC_SET(SW_Y, PIXELS(1)),
	SC_WAIT(10),
	SC_SET(SW_Y, 0),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u16 Korosseo_PortraitPaletteOffsets[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	0x2000, 0x2020, 0x4060, 0x4040, 128,
#else
	0x2000, 0x2020, 0x4060, 0xa040, 128,
#endif
};

s16 gColossoModeScript2[] = {
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
	30, 0x7fff, 30, -1,
};

s16 gColossoModeScript3B[] = {
	0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
	0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
	0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
	336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
	6, 0x1000, 320, 6, 0x7fff, 6, -1,
};
