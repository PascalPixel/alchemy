/* gmGetArea, the same in both games. Both games' game/main_routine.c include
   it where the function stands. */

/* Returns the area sceneId belongs to. The scenes of one area share their
   music, area name and the flags kept while inside it. */
Sint32 gmGetArea(Sint32 sceneId)
{
	return ScenesWork[sceneId].area;
}
