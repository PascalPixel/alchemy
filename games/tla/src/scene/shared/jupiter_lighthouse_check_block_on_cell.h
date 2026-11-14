/// Opens the door for good once the pillar stands on its button, cell (8, 37).
void JupiterLighthouse_CheckBlockOnCell(void) {
  struct Actor *object = Actor_Get(ACTOR_BUTTON_PILLAR);

  if (!Event_GetFlag(FLAG_BUTTON_PILLAR_PLACED) && object->x >> 20 == 8 &&
      object->z >> 20 == 37) {
    Event_SetFlag(FLAG_BUTTON_PILLAR_PLACED);
    JupiterLighthouse_OpenDoor(1);
  }
}
