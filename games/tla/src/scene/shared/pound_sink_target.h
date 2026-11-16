/// Pound's work on a listed actor, an entry of (actor, kind, flag) ended by
/// 0xffff: while it is the target, it sinks a step a frame for the first eight
/// frames, and on the last (mode 1) its flag is set and its cell opened.
void Pound_SinkTarget(u16 *list, s32 mode, s32 count) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  while (*list != 0xffff) {
    u32 entry = *list;
    s32 id = work->secondActorId;

    if (id == entry) {
      struct Actor *actor = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        actor->scaleY -= FIXED_ONE * 31 / 256;
      if (mode == 1) {
        GameFlag_Set(flag);
        Actor_SetCellOpen(id, a, flag, actor->layer);
        actor->scaleY = FIXED_ONE;
      }
      break;
    }
    list += 3;
  }
}
