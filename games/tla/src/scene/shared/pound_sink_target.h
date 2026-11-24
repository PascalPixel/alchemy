/// Pound's state on a listed actor, an entry of (actor, kind, flag) ended by
/// ACTOR_LIST_END: while it is the target, it sinks a step a frame for the
/// first eight frames, and on the last (mode 1) its flag is set and its cell
/// opened.
void Pound_SinkTarget(u16 *list, s32 mode, s32 count) {
  struct FieldEffectState *state = Ram_HeapSlots->fieldEffectState;

  for (; *list != ACTOR_LIST_END; list += 3) {
    u32 entry = *list;
    s32 id = state->secondActorId;

    if (id == entry) {
      struct Actor *actor = Actor_GetById(id);
      s32 kind;
      s32 flag;

      list++;
      kind = list[0];
      flag = list[1];
      if (count < 8)
        actor->scaleY -= FIXED_ONE * 31 / 256;
      if (mode == 1) {
        GameFlag_Set(flag);
        Actor_SetCellOpen(id, kind, flag, actor->layer);
        actor->scaleY = FIXED_ONE;
      }
      break;
    }
  }
}
