/* Lifts an actor 32 pixels over its ground as a platform others may stand on,
   and raises the map metatile under it 48 pixels, a gap the leader jumps across
   onto it. */
void scActorRaiseFromMetatile(Sint32 id)
{
	ACTOR *actor;
	Sint32 height;

	actor = fdActorGet(id);
	actor->motionFlg = ACTOR_FLOATS;
	height = actor->groundHeight;
	actor->bounce = 0;
	actor->y.fixed = height + toFIXED(32);
	actor->priFlg |= ACTOR_PRIORITY_PLATFORM;
	height =
		fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
	fdMapGridSetMetatileHeight(actor->x.fixed, actor->z.fixed, 0,
		METATILE_HEIGHT_STEPS(height) + METATILE_HEIGHT_STEPS(toFIXED(48)));
	fdMapGridSetMetatileAttributesAt(
		actor->x.fixed, actor->z.fixed, 0, MAP_METATILE_JUMP);
}
