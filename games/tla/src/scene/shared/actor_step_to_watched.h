/// Steps an actor toward its target; 0 when it gets there.
s32 Actor_StepToWatched(struct Actor *object) {
  u8 left;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  left = --BYTE_AT(object, 98);
  return left != 0;
}
