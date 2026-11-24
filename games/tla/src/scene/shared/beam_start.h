/// Sets the flame throwers going, unless the scene starts where the party was:
/// up to four listed actors with their flags and reach, and up to four actors
/// their flames stop at; then what is called back when a flame meets a blocked
/// one and the step trigger a hit raises. Blends the flames over the room and
/// starts their puffs.
void FlameThrowers_Start(u16 *targets, u16 *stops, s32 onBlocked,
                         s32 hitTrigger) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;
  struct FlameThrower *entry = state->entries;
  s32 id;
  s32 i;
  s32 value;
  s32 one;
  s32 flags;
  s32 reach;
  struct Actor *actor;
  s32 alpha;
  s32 control;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    for (i = 0; i < 4 && (id = *targets++) > 0; i++) {
      flags = *targets;
      reach = flags & FLAME_THROWER_REACH_BITS;
      flags &= FLAME_THROWER_FLAG_BITS;
      actor = Actor_Get(id);
      targets++;
      if (actor != NULL) {
        Actor_SetSpriteFlags(actor, 0);
        *(u8 *)&actor->sceneCounter = one = 1;
        if ((flags & FLAME_SPARK) == 0)
          actor->collisionFlags = 0;
        entry->active = one;
        entry->flags = flags;
        entry->actor = Actor_Get(id);
        entry->reach = toFIXED(reach);
        entry->timer = i;
        entry++;
      }
    }
    if (stops != NULL) {
      for (i = 0; i < 4; i++) {
        value = *stops++;
        state->slots[i].id = value;
        if (value <= 0)
          break;
        state->slots[i].blocked = *stops++;
      }
    }
  }
  state->onBlocked = (void (*)(void))onBlocked;
  state->hitTrigger = hitTrigger;
  if (REG_BLDCNT == 0) {
    alpha = BLEND_ALPHA(8, 12);
    REG_BLDALPHA = alpha;
    control = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    REG_BLDCNT = control;
  }
  Task_AddFrame(FlameThrowers_Puff, TASK_PASS_FRAME);
}
