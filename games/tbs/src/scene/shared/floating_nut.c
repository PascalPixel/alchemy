#include "scene/orbit.h"

// Crossbone Isle, Kalay, Kolima Forest (SceneIdKolimaForestGlade), Lunpa, McCoy's palace garden,
// Mogall Forest and Vale: a Nut floating where it lies, until it is taken.

// The Nut's callback, each tick: it sways up to two pixels either side of its spot and dips up to
// two pixels below it, at a random pace.
static Sint32 scUpdateFloatingNut(ORBITING_MAP_ACTOR *nut)
{
	SPR  *sprite = (SPR *)nut->sprite;
	FIXED dip    = sySin(nut->orbitAngle) * 2;

	if (dip > 0)
		dip = -dip;
	nut->x            = nut->orbitCenterX + syCos(nut->orbitAngle) * 2;
	nut->y            = nut->orbitCenterY + dip;
	sprite->rotation  = syCos(nut->orbitAngle + DEGtoANG(180)) / 8;
	// The pace: a sixty-fourth of a turn and two random steps under 1/128 each.
	nut->orbitAngle  += syRandomBelow(512) + syRandomBelow(512) + TRIG_TURN / 64;
	return 0;
}

// Draws actor id as the Nut's icon and starts it floating, 32 pixels up unless the party returns to
// where it was.
void scFloatNut(Sint32 id)
{
	ORBITING_MAP_ACTOR     *actor;
	ORBITING_MAP_ACTOR_SPR *sprite;
	ICON_TRANSFER          *glyph;

	actor                 = (ORBITING_MAP_ACTOR *)fdActorGetById(id);
	sprite                = actor->sprite;
	sprite->priority      = 1;
	sprite->fullColor     = 0;
	sprite->paletteNumber = 0;

	sprite->count         = 0;
	fdActorSetSpriteFlags((ACTOR *)actor, 0);
	actor->retainSpriteCache = 0;
	actor->motionFlags       = 0;

	if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
		actor->y += toFIXED(32);

	actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	actor->frozen         = 1;

	glyph                 = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawItem(ITEM_NUT);
	syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);

	actor->orbitCenterX      = actor->x;
	actor->orbitAngle        = 0;
	actor->orbitCenterY      = actor->y;
	actor->retainSpriteCache = 1;
	actor->callback          = (Uint32)scUpdateFloatingNut;
	actor->arrivalAxis       = 0;
}
