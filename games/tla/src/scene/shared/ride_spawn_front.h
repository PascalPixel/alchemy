/// Spawns a ride object moving forward at `speed`.
void Func_02001c74(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02004bd8_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Actor_ResetScriptedFor(object, 2);
    object->update = Ride_UpdateCarried;
    if (mode)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}
