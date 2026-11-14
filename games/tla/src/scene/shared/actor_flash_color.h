/// Flashes an actor in colour 10 on the frames its mask picks.
void MagmaRockInside_FlashStatue(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    FieldActor_SetSpriteColor(object, 10);
  else
    FieldActor_SetSpriteColor(object, 0);
}
