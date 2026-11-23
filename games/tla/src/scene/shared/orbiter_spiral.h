/// Turns an orbiting object by half its speed and puts it out from its anchor,
/// raised 8 pixels, by its radius.
static inline void Orbiter_Step(struct Orbiter *orbiter, struct Actor *anchor) {
  orbiter->angle += (s16)orbiter->speed >> 1;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + toFIXED(8);
  orbiter->z = anchor->z.fixed;
  FieldVector_AddPolarOffsetFX(orbiter->orbitRadius, orbiter->angle,
                               &orbiter->x);
}

/// Steps an orbiting object round its anchor: returns 0 once its countdown
/// runs out; else, unless its radius is down to 2 pixels, it leaves a spark,
/// closes 4 pixels and steps again, leaving another and closing 4 more.
/// Returns 1 while it runs.
s32 Orbiter_Spiral(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  Orbiter_Step(orbiter, anchor);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->orbitRadius == toFIXED(2))
    return 1;
  OrbitSpark_Spawn(orbiter);
  orbiter->orbitRadius -= toFIXED(4);
  Orbiter_Step(orbiter, anchor);
  OrbitSpark_Spawn(orbiter);
  orbiter->orbitRadius -= toFIXED(4);
  return 1;
}
