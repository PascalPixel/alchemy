/* Event: the drops of rain falling about the stone Douse has struck
   (scGroundParticlesSetAnchor), the overlay's copy of fdGroundParticlesUpdate.
   Each plays its ticks of GroundParticleFrames, falling and then splashing on
   the ground (held while FLAG_WEATHER_HELD is set), and falls again at a random
   spot within a screen of that actor, or at the origin when there is none. */
void fdGroundParticlesDraw(void)
{
	MAP_STATE			  *view = RamHeapSlots->mapGrid;
	GROUND_PARTICLE_STATE *state = RamHeapSlots->weather;
	WEATHER_PARTICLE	  *particle = state->particles;
	Sint32				   cameraX = syMapStateView(view)[0];
	Sint32				   cameraZ = syMapStateView(view)[1];
	Uint32				   i;

	for(i = 0; i < ARRAY_COUNT(state->particles); i++, particle++) {
		Sint16 *frame;
		Sint32	x;
		Sint32	y;

		if(--particle->timer == GROUND_PARTICLE_FREED)
			continue;
		if(syGameFlagGet(FLAG_WEATHER_HELD))
			particle->timer++;
		frame = &GroundParticleFrames[5 * particle->timer];
		x = (particle->posX - cameraX) / toFIXED(1) + *frame++;
		y = (particle->posZ - particle->posY - cameraZ) / toFIXED(1) + *frame++;
		if(x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
			particle->priority = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = state->tileBase + *(Uint16 *)frame;
			frame++;
			particle->shape = *(Uint8 *)frame;
			particle->size = *(Uint8 *)(frame + 1);
			syOAMAddSpriteAtDepth(particle, OAM_DEPTH_WINDOW);
		}
		if(particle->timer == 0) {
			Sint32 *leader = state->leader;

			if(leader != NULL) {
				x = leader[0] + syRandom() * DISPLAY_WIDTH -
					toFIXED(DISPLAY_WIDTH / 2);
				y = leader[2] + syRandom() * DISPLAY_HEIGHT -
					toFIXED(DISPLAY_HEIGHT / 2);
				particle->posX = x;
				particle->posZ = y;
				particle->posY = fdMapGetTerrainHeight(0, x, y);
				particle->timer = 16;
			} else {
				particle->posX = 0;
				particle->posZ = 0;
				particle->posY = 0;
				particle->timer = 16;
			}
		}
	}
}
