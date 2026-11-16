/// Spawns a spark thrown up at `speed`.
void Ride_SpawnSpark(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(
      SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));
  s32 spin;

  if (object != NULL) {
    object->bounce = Trig_Cos((u16)(Random16() * 3 >> 4) + 0x3000);
    object->gravity = (u16)Random16();
    object->riseBaseY = (u16)(Random16() << 1) + speed;
    spin = (u16)Random16() + 0xffff8000;
    object->timer = spin;
    Actor_SetupScriptedFor(object, 2);
    object->update = (void (*)(void))Ride_UpdateDrift;
    Actor_SetBlendMode(object, 1);
  }
}
