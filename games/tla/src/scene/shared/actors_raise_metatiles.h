/* For each statue of a list of (statue, metatile, cap) rows ended by
   ACTOR_LIST_END: holds it still, without a shadow, and raises the metatile it
   stands on 32 pixels above the ground there, so it stands as a block. */
void scMagmaRockInsideRaiseActorMetatiles(Uint16 *list)
{
	for(; *list != ACTOR_LIST_END; list += 3) {
		ACTOR *actor = fdActorGet(*list);
		Sint32 height =
			fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);

		actor->motionFlg = 0;
		actor->collisionFlg = 0;
		fdActorSetSprFlags(actor, 0);
		scMagmaRockInsideSetMetatileShape(actor->layer,
			FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed),
			METATILE_HEIGHT_STEPS(height) + METATILE_HEIGHT_STEPS(toFIXED(32)));
	}
}
