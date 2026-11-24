/// @file
/// The dust Scoop throws up, each grain growing as it flies.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"

#include "macros.h"

/// Advances one grain of Scoop's dust a frame: its sprite steps through eight
/// pictures of eight tiles from tiles, four frames each, grows by a tenth of
/// its size a frame to full size, is drawn at the grain's place on screen, and
/// the grain moves on (Field_AdvanceWithGravity3D). Sand's
/// GrowingParticle_Update is the same but for its own age check and
/// Field_AdvanceWithGravity2D, so each Psynergy keeps its own.
void ScoopDust_UpdateGrain(struct EffectSprite *sprite,
                           struct FieldPsynergyParticle *particle, s32 tiles) {
  s32 position[3];
  FIXED scale;

  sprite->oam.index = tiles + ((particle->timer >> 2) & 7) * 8;
  scale = sprite->scaleX + FIXED_ONE / 10;
  sprite->scaleX = scale;
  if (scale > FIXED_ONE)
    scale = FIXED_ONE;
  sprite->scaleX = scale;
  sprite->scaleY = scale;
  SET_VECTOR(position, particle->x, particle->y, particle->z);
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  EffectSprite_Draw(sprite);
  Field_AdvanceWithGravity3D(particle, 60, 0);
}
