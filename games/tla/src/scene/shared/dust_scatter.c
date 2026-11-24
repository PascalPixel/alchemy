/// Eight clouds of dust rise in a row across the leader, 2 pixels apart, and
/// fly out from him in eight directions when `spread` is set.
void Dust_Scatter(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i;
  FIXED y;

  for (i = 0; i <= 7; i++) {
    y = leader->y.fixed - toFIXED(Random_Below(14));
    effect = (struct FieldEffect *)FieldActor_Create(
        SPRITE_DUST_CLOUD, i * toFIXED(2) + leader->x.fixed - toFIXED(8),
        y - toFIXED(2), leader->z.fixed);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gDust_ScatterScript);
      effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x =
            Iwram_MulFX(Trig_Cos(i * DEGtoANG(45)), toFIXED(6) / 5);
        effect->velocity.z = Math_Sin(i * DEGtoANG(45));
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + (toFIXED(3) / 8));
      effect->spin = ((Random16() << 9) >> 16) - (TRIG_TURN / 256);
      effect->scaleRateX = -FIXED_ONE / 32;
      effect->scaleRateY = -(FIXED_ONE * 3 / 128);
      effect->update = FieldEffect_AdvanceMotionAtLeaderPriority;
    }
  }
}
