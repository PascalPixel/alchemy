/* The exits of the Psynergy test rooms and the shop test room, which each keeps
   a copy of: exit 20 leads to the Psynergy test room's entrance 2. */
Uint32 SceneExits[] = {
	SCENE_EXITS(PsynergyTest),
	SCENE_EXIT(20, PsynergyTest, 2),
	SCENE_EXITS_END,
};
