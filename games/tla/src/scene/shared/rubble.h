/// @file
/// The rubble a crumbling pillar throws up around the leader.

extern u32 gRubble_SprayScript[];

/// Eight puffs of dust in a row over the leader, flung outwards when `spread`
/// is set.
void Rubble_Spray(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  void *frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - px(8), y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation((struct Actor *)effect, 2);
      Actor_StartScript(effect, gRubble_SprayScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

extern u32 gRubble_BurstScript[];

/// Seventeen puffs of dust thrown out in a ring from the leader as a pillar
/// lands.
void Rubble_Burst(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  void *frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                     leader->z);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation((struct Actor *)effect, 2);
      Actor_StartScript(effect, gRubble_BurstScript);
      effect->velocity.x = Trig_Cos(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}
