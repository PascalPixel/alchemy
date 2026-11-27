/* Event: the drops of rain falling about the stone Douse has struck
   (scGroundParticlesSetAnchor), the overlay's copy of fdGroundParticlesUpdate.
   Each plays its ticks of GroundParticleFrames, falling and then splashing on
   the ground (held while FLAG_WEATHER_HELD is set), and falls again at a random
   spot within a screen of that actor, or at the origin when there is none. */
void fdGroundParticlesDraw(void)
{
	MAP_STATE			  *view = Ram_HeapSlots->mapGrid;
	GROUND_PARTICLE_STATE *state = Ram_HeapSlots->weather;
	WEATHER_PARTICLE	  *particle = state->particles;
	Sint32				   cameraX = syMapStateView(view)[0];
	Sint32				   cameraZ = syMapStateView(view)[1];
	Uint32				   i;

	for(i = 0; i < ARRAY_COUNT(state->particles); i++, particle++) {
		Sint16 *frm;
		Sint32	x;
		Sint32	y;

		if(--particle->tmr == GROUND_PARTICLE_FREED)
			continue;
		if(syGameFlagGet(FLAG_WEATHER_HELD))
			particle->tmr++;
		frm = &GroundParticleFrames[5 * particle->tmr];
		x = (particle->posX - cameraX) / toFIXED(1) + *frm++;
		y = (particle->posZ - particle->posY - cameraZ) / toFIXED(1) + *frm++;
		if(x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
			particle->pri = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = state->tileBase + *(Uint16 *)frm;
			frm++;
			particle->shape = *(Uint8 *)frm;
			particle->size = *(Uint8 *)(frm + 1);
			syOAMAddSprAtDepth(particle, OAM_DEPTH_WINDOW);
		}
		if(particle->tmr == 0) {
			Sint32 *leader = state->leader;

			if(leader != NULL) {
				x = leader[0] + syRandom() * DISPLAY_WIDTH -
					toFIXED(DISPLAY_WIDTH / 2);
				y = leader[2] + syRandom() * DISPLAY_HEIGHT -
					toFIXED(DISPLAY_HEIGHT / 2);
				particle->posX = x;
				particle->posZ = y;
				particle->posY = fdMapGetTerrainHeight(0, x, y);
				particle->tmr = 16;
			} else {
				particle->posX = 0;
				particle->posZ = 0;
				particle->posY = 0;
				particle->tmr = 16;
			}
		}
	}
}
