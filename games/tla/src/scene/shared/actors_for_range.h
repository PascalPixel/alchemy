/// Runs Actor_Hide for each id from 9 to `9 + 6`, then Scene_RunUnlessFlag.
void YampiDesert_SetUpRoom12SlideActors(void) {
  u32 i;

  for (i = 0; i <= 6; i++)
    Actor_Hide(i + 9);
  Scene_RunUnlessFlag();
}
