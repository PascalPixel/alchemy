/* @file */
/* Cottage Rooms B and the world map, where nothing reaches it. */

/* Unused: an actor's update function that steps it through the six colour */
/* modes from SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG, one every two frames. */
UNUSED static void scCycleColModesOnOddFrames(ACTOR *actor) {
	if((FrameCount & 1) != 0) {
		s32 slot = (FrameCount >> 1) % 6;

		fdActorSetColMode(actor, slot);
	}
}
