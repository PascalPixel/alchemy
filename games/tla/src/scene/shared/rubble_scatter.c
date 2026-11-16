/// Scatters dust and rubble around the leader's feet; with spread, they fly
/// outward.
void Rubble_Scatter(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;
  FIXED y;

  do {
    y = leader->y.fixed - ((Random_Below(14)) << 16);
    effect = (struct FieldEffect *)FieldActor_Create(
        SPRITE_DUST_CLOUD, (i << 17) + leader->x.fixed - toFIXED(8),
        y - toFIXED(2), leader->z.fixed);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gRubble_ScatterScript);
      effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x =
            Iwram_MulFX(Trig_Cos(i * DEGtoANG(45)), FIXED_ONE * 6 / 5);
        effect->velocity.z = Math_Sin(i * DEGtoANG(45));
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -FIXED_ONE / 32;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}
