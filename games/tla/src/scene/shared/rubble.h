/// @file
/// The dust a crumbling pillar throws up around the leader.

extern u32 gRubble_SprayScript[];

/// Eight clouds of dust rise in a row across the leader, 2 pixels apart, and
/// fly out from him in eight directions when `spread` is set.
void Rubble_Spray(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  void *frame = 0;
  u32 i = 0;
  FIXED y;

  do {
    y = leader->y.fixed - toFIXED(Random_Below(14));
    effect = (struct FieldEffect *)FieldActor_Create(
        SPRITE_DUST_CLOUD, i * toFIXED(2) + leader->x.fixed - toFIXED(8),
        y - toFIXED(2), leader->z.fixed);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation((struct Actor *)effect, 2);
      Actor_StartScript(effect, gRubble_SprayScript);
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
      effect->velocity.y = -(Random_Below(toFIXED(1) / 4) + toFIXED(3) / 8);
      effect->spin = Random_Below(TRIG_TURN / 128) - TRIG_TURN / 256;
      effect->scaleRateX = -(FIXED_ONE / 32);
      effect->scaleRateY = -(FIXED_ONE * 3 / 128);
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

extern u32 gRubble_BurstScript[];

/// Throws a ring of dust clouds out from the leader as a pillar lands, the
/// first thrown twice.
void Rubble_Burst(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  void *frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)FieldActor_Create(
        SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed, leader->z.fixed);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation((struct Actor *)effect, 2);
      Actor_StartScript(effect, gRubble_BurstScript);
      effect->velocity.x = Trig_Cos(i * (TRIG_TURN / DUST_RING_PUFFS));
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(i * (TRIG_TURN / DUST_RING_PUFFS));
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += Random_Below(toFIXED(3) / 4) - toFIXED(3) / 8;
      effect->velocity.z += Random_Below(toFIXED(3) / 8) - toFIXED(3) / 16;
      effect->spin = Random_Below(TRIG_TURN / 16) - TRIG_TURN / 32;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= DUST_RING_PUFFS);
}
