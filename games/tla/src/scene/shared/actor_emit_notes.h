/// Four notes rise from an actor, one each quarter of a second.
void Actor_EmitNotes(s32 id) {
  struct Actor *object = Actor_GetById(id);

  LemuriaInteriors_EmitNote(object->x.fixed + px(8), object->y.fixed + px(28),
                            object->z.fixed, 0x1000, 15);
  Sound_Play(SE_LIGHT_APPEAR);
  Event_Wait(15);
  LemuriaInteriors_EmitNote(object->x.fixed - px(8), object->y.fixed + px(28),
                            object->z.fixed, 0xf000, 15);
  Sound_Play(SE_LIGHT_APPEAR);
  Event_Wait(15);
  LemuriaInteriors_EmitNote(object->x.fixed + px(8), object->y.fixed + px(28),
                            object->z.fixed, 0x1000, 15);
  Sound_Play(SE_LIGHT_APPEAR);
  Event_Wait(15);
  LemuriaInteriors_EmitNote(object->x.fixed - px(8), object->y.fixed + px(28),
                            object->z.fixed, 0xf000, 15);
  Sound_Play(SE_LIGHT_APPEAR);
  Event_Wait(15);
}
