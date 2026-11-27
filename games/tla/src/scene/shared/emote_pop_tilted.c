#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "types.h"
#include "field/field_script.h"
#include "field/script.h"

/* Pops an emote up at (x, y, z), 16.16 pixels: SPRITE_EMOTE's animation 1,
   tilted by `rotation`, with the SE_LIGHT_APPEAR sound; its script keeps it 46
   ticks and releases it. Idejima and the Shaman Village each pop a pair,
   tilted either way, in a conversation. */
void   syEmotePopTilted(x, y, z, rotation) Sint32 x, y, z;
Sint16 rotation;
{
	ACTOR	  *object;
	FIELD_SPR *spr;

	object = syImportActorCreate(SPRITE_EMOTE, x, y, z);
	if(object != 0) {
		spr = object->spr;
		sySndPlay(SE_LIGHT_APPEAR);
		syImportActorSelectAnimation(object, 1);
		syImportActorStartScript(object, EmotePopTiltedScript);
		object->motionFlg = 0;
		spr->flg = 0;
		spr->rotation = rotation;
	}
}
