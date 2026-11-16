/// Moves an orbiting actor round its centre as its circle grows.
s32 Orbit_Update(struct Orbit *object) {
  FIXED z;

  object->angle += object->speed;
  object->x = IwramMulFX(Trig_Cos(object->angle), object->orbitRadius);
  z = IwramMulFX(Math_Sin(object->angle), object->orbitRadius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->orbitRadius = object->growth * Math_Sin(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}
