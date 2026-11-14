/// Moves an orbiter two steps inward round its anchor.
s32 Orbiter_Spiral(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;
  s16 *angle;

  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + px(8);
  orbiter->z = anchor->z;
  FieldVector_AddPolarOffset(orbiter->radius, orbiter->angle, &orbiter->x);
  /* FAKEMATCH(temporary): the angle's address, taken after the first push,
   * sets r2 from r8 before the second push reads it. */
  angle = &orbiter->angle;
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->radius == 0x20000)
    return 1;
  Func_02001364(orbiter);
  orbiter->radius -= 0x40000;
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + px(8);
  orbiter->z = anchor->z;
  FieldVector_AddPolarOffset(orbiter->radius, *angle, &orbiter->x);
  Func_02001364(orbiter);
  orbiter->radius -= 0x40000;
  return 1;
}
