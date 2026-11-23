/// Task: the drops of rain falling about the stone Douse has struck
/// (GroundParticles_SetAnchor), the overlay's copy of GroundParticles_Update.
/// Each plays its frames of
/// gGroundParticleFrames, falling and then splashing on the ground (held while
/// FLAG_WEATHER_HELD is set), and falls again at a random spot within a screen
/// of that actor, or at the origin when there is none.
void GroundParticles_Draw(void) {
  struct MapWork *view = Ram_HeapSlots->mapGrid;
  struct GroundParticleWork *work = Ram_HeapSlots->weather;
  s32 *camera = &view->viewX;
  struct GroundParticle *particle = work->particles;
  s32 cameraX = camera[0];
  s32 cameraZ = camera[1];
  u32 i;

  for (i = 0; i < ARRAY_COUNT(work->particles); i++, particle++) {
    s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == GROUND_PARTICLE_FREED)
      continue;
    if (GameFlag_Get(FLAG_WEATHER_HELD))
      particle->timer++;
    frame = &gGroundParticleFrames[5 * particle->timer];
    x = (particle->posX - cameraX) / toFIXED(1) + *frame++;
    y = (particle->posZ - particle->posY - cameraZ) / toFIXED(1) + *frame++;
    if (x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tileBase + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Oam_AddSpriteAtDepth(particle, OAM_DEPTH_WINDOW);
    }
    if (particle->timer == 0) {
      s32 *leader = work->leader;

      if (leader != NULL) {
        x = leader[0] + Random16() * DISPLAY_WIDTH - toFIXED(DISPLAY_WIDTH / 2);
        y = leader[2] + Random16() * DISPLAY_HEIGHT -
            toFIXED(DISPLAY_HEIGHT / 2);
        particle->posX = x;
        particle->posZ = y;
        particle->posY = Map_GetTerrainHeight(0, x, y);
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
