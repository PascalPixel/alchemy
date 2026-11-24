/// @file
/// The crumbs a fireball throws off as it crumbles away
/// (Actor_RollAndCrumble), which three Mars Lighthouse rooms keep a copy of,
/// over the flames' FlameHitState (scene/ride.h).

/// A fireball crumbling away throws off a crumb, a small flame drawn with the
/// kept flame's tiles, that bounces off in a random direction and flickers out
/// (the includer's gCrumbs_Script): as the Mars tower's fireball crashes on the
/// centre wall, it shrinks away in a spray of little flames.
void Actor_ScatterCrumbs(struct Actor *actor) {
  struct FlameHitState *state = RIDE_STATE;
  s32 i;

  if (state->handle == NULL)
    return;
  // DEAD: a loop of one pass, as the ice chips' loop of six; the ROM keeps its
  // decrement and test.
  for (i = 0; i < 1; i++) {
    struct Actor *object;
    s32 a, b, c, d, e;
    FIXED x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x.fixed + a * 16 - b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z.fixed + d * 16 - e * 16;
    y = c * 8 + actor->y.fixed + toFIXED(4);
    object = (struct Actor *)Import_ActorCreate(SPRITE_UNSEEN_MARKER, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;

      angle = Random16();
      radius = Random16() * 8;
      sum = Random16() + Random16();
      scale = (sum >> 2) + FIXED_ONE / 2;
      Actor_StartScript(object, gCrumbs_Script);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + toFIXED(2);
      object->velocityZ = Iwram_MulFX(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulFX(radius, Trig_Sin(angle));
      object->gravity = toFIXED(33) / 100;
      object->scaleX = scale;
      object->scaleY = scale;
      object->acceleration = 0;
      object->bounce = FIXED_ONE / 2;
      sprite = object->sprite;
      source = ((struct Actor *)state->handle)->sprite;
      VramCache_FreeEntry(sprite->slot);
      sprite->retainCacheEntry = 1;
      sprite->slot = state->flameVramBlock;
      SpriteAnimation_Clear(sprite->entries[0]);
      sprite->entries[0] = NULL;
      sprite->count = 0;
      sprite->width = source->width;
      sprite->height = source->height;
      sprite->image.shape = source->image.shape;
      sprite->image.size = source->image.size;
      sprite->image.tile = source->image.tile;
    }
  }
}
