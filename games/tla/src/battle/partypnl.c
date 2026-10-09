#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"

#define BattleLayout_HighlightPartyPanels ((void (*)(u16 *))BattleLayout_HighlightPartyPanels)
void Battle_DrawPartyPanelsWithEmptyList(void)
{
	/* FAKEMATCH: the incoming r9 chain spill preserves the ABI save/store omitted by ordinary top-level C. */
	volatile u32 frame;
	u16			 data[2];
	/* FAKEMATCH: capturing the incoming static chain preserves the r9 save before either call. */
	register u32 chain asm("r9");

	frame = chain;
	data[0] = 255;
	BattleLayout_HighlightPartyPanels(data);
	BattlePres_SetActorModesFar(data, 1);
}
