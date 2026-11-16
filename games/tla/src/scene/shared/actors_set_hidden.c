/// Sets or clears bit 0x10 of count actors' priority flags from first.
void Actors_SetHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~ACTOR_PRIORITY_NOT_DRAWN;
    else
      actor->priorityFlags |= ACTOR_PRIORITY_NOT_DRAWN;
  }
}
