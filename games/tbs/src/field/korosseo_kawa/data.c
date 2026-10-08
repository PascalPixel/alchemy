#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_event.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "text/msg_ids.h"

u16 Korosseo_PortraitPaletteOffsets[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	0x2000, 0x2020, 0x4060, 0x4040, 128,
#else
	0x2000, 0x2020, 0x4060, 0xa040, 128,
#endif
};

s16 KorosseoKawa_ModeRecordTwo[] = {
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
	30, 0x7fff, 30, -1,
};

s16 KorosseoKawa_SpanB[] = {
	0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
	0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
	0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
	336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
	6, 0x1000, 320, 6, 0x7fff, 6, -1,
};
