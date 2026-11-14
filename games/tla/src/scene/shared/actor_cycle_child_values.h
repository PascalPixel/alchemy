/// Cycles an actor's child values, running WorldMap_SpawnParticle every fourth
/// frame.
void Actor_CycleChildValues(struct Actor *object) {
  if (gFrameCount & 1) {
    Actor_SetSpriteColorMode(object, gData_02004fc8[gData_02006be4]);
    gData_02006be4 = (gData_02006be4 + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    WorldMap_SpawnParticle(object);
  }
}
