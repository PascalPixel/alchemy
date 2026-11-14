/// Flickers an actor, running WorldMap_SpawnSparkPair every sixteenth frame.
void Func_02003b38(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(object, 7);
  } else {
    Actor_SetSpriteColorMode(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_SpawnSparkPair(object);
  }
}
