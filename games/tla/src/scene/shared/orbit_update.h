/// Moves an orbiting actor round its centre as its circle grows.
s32 Orbit_Update(struct Orbit *object) {
  s32 z;

  object->angle += object->speed;
  object->x = IwramMulQ16(Trig_Cos(object->angle), object->radius);
  z = IwramMulQ16(Math_Sin(object->angle), object->radius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->radius = object->growth * Math_Sin(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}
