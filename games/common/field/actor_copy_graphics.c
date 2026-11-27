/* fdActorCopyGraphics, the same in both games. Both games'
   field/script_commands_actor_commands.c include it where the function
   stands. */

/* Makes the target actor's sprite show the source actor's graphics (its VRAM
   block and tile). Neither actor is checked for NULL. */
void fdActorCopyGraphics(Uint32 tgtId, Uint32 srcId)
{
	ACTOR *actor;
	SPR	  *spr;
	Uint8  vramBlock;
	Uint32 tile;

	actor = fdActorGetById(srcId);
	spr = actor->spr;
	vramBlock = spr->slot;
	tile = spr->image.tile;

	actor = fdActorGetById(tgtId);
	spr = actor->spr;
	spr->slot = vramBlock;
	spr->image.tile = tile;
}
