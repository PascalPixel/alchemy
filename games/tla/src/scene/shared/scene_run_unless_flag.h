/// Runs YampiDesert_HideSlideActors unless flag 0x20d is set, setting it when
/// the party keeps its place.
void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(0x20d) == 0)
      YampiDesert_HideSlideActors();
  } else {
    GameFlag_Set(0x20d);
  }
}
