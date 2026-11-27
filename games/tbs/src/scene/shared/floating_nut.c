#include "scene/orbit.h"

/* Crossbone Isle, Kalay, Kolima Forest (SceneIdKolimaForestGlade), Lunpa,
   McCoy's palace garden, Mogall Forest and Vale: a Nut floating where it lies,
   until it is taken. */

/* The Nut's callback, each tick: it sways up to two pixels either side of its
   spot and dips up to two pixels below it, at a random pace. */
static Sint32 scUpdateFloatingNut(ORBITING_MAP_ACTOR *nut)
{
	SPR	 *spr = (SPR *)nut->spr;
	FIXED dip = sySin(nut->orbitAng) * 2;

	if(dip > 0)
		dip = -dip;
	nut->x = nut->orbitCenterX + syCos(nut->orbitAng) * 2;
	nut->y = nut->orbitCenterY + dip;
	spr->rotation = syCos(nut->orbitAng + DEGtoANG(180)) / 8;
	/* The pace: a sixty-fourth of a turn and two random steps under 1/128
	   each. */
	nut->orbitAng += syRandomBelow(512) + syRandomBelow(512) + TRIG_TURN / 64;
	return 0;
}

/* Draws actor id as the Nut's icon and starts it floating, 32 pixels up unless
   the party returns to where it was. */
void scFloatNut(Sint32 id)
{
	ORBITING_MAP_ACTOR	   *actor;
	ORBITING_MAP_ACTOR_SPR *spr;
	ICON_TRANSFER		   *glyph;

	actor = (ORBITING_MAP_ACTOR *)fdActorGetById(id);
	spr = actor->spr;
	spr->pri = 1;
	spr->fullCol = 0;
	spr->palNum = 0;

	spr->cnt = 0;
	fdActorSetSprFlags((ACTOR *)actor, 0);
	actor->retainSprCache = 0;
	actor->motionFlg = 0;

	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
		actor->y += toFIXED(32);

	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	actor->frozen = 1;

	glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawItem(ITEM_NUT);
	syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES, glyph->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);

	actor->orbitCenterX = actor->x;
	actor->orbitAng = 0;
	actor->orbitCenterY = actor->y;
	actor->retainSprCache = 1;
	actor->func = (Uint32)scUpdateFloatingNut;
	actor->arrivalAxis = 0;
}
