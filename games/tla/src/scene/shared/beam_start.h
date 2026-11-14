/// Starts the beam over its targets and stops.
void FlameThrowers_Start(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gInventorySnapshot + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
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
        ((u8 *)object)[98] = one = 1;
        if ((flags & 0x80) == 0)
          object->collisionFlags = 0;
        target->active = one;
        target->flags = flags;
        target->object = Actor_Get(id);
        target->height = height << 16;
        target->index = i;
        target++;
      }
    }
    if (stops != NULL) {
      for (i = 0; i < 4; i++) {
        value = *stops++;
        beam->stops[i].unk00 = value;
        if (value <= 0)
          break;
        beam->stops[i].unk02 = *stops++;
      }
    }
  }
  beam->unk10 = a2;
  beam->unk0a = a3;
  if (REG_BLDCNT == 0) {
    alpha = 0xc08;
    REG_BLDALPHA = alpha;
    control = 0x3f10;
    REG_BLDCNT = control;
  }
  Task_AddFrame(FlameThrowers_Puff, 0x480);
}
