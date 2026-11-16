/// Sets the flame throwers going: each listed actor with its flags and reach,
/// the actors their flames stop at, what is called back when one does and the
/// step trigger a hit raises; then blends the flames over the room and starts
/// their puffs.
void FlameThrowers_Start(u16 *targets, u16 *stops, s32 onBlocked,
                         s32 hitTrigger) {
  struct FlameHitWork *work = RIDE_WORK;
  struct RideEntry *entry = work->entries;
  s32 id;
  s32 i;
  s32 value;
  s32 one;
  s32 flags;
  s32 height;
  struct Actor *object;
  s32 alpha;
  s32 control;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    for (i = 0; i < 4 && (id = *targets++) > 0; i++) {
      flags = *targets;
      height = flags & 15;
      flags &= 0xf0;
      object = Actor_Get(id);
      targets++;
      if (object != NULL) {
        Actor_SetSpriteFlags(object, 0);
        *(u8 *)&object->riseCounter = one = 1;
        if ((flags & FLAME_SPARK) == 0)
          object->collisionFlags = 0;
        entry->active = one;
        entry->flags = flags;
        entry->actor = Actor_Get(id);
        entry->reach = toFIXED(height);
        entry->timer = i;
        entry++;
      }
    }
    if (stops != NULL) {
      for (i = 0; i < 4; i++) {
        value = *stops++;
        work->slots[i].id = value;
        if (value <= 0)
          break;
        work->slots[i].blocked = *stops++;
      }
    }
  }
  work->onBlocked = (void (*)(void))onBlocked;
  work->hitTrigger = hitTrigger;
  if (REG_BLDCNT == 0) {
    alpha = BLEND_ALPHA(8, 12);
    REG_BLDALPHA = alpha;
    control = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    REG_BLDCNT = control;
  }
  Task_AddFrame(FlameThrowers_Puff, TASK_PASS_VBLANK);
}
