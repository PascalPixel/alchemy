/// Spawns a spark thrown up at `speed`.
void Ride_SpawnSpark(s32 *position, s32 speed, s32 mode) {
  struct Actor *actor = Import_ActorCreate_(
      SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));
  s32 spin;

  if (actor != NULL) {
    actor->bounce = Trig_Cos((u16)(Random16() * 3 >> 4) + TRIG_TURN * 3 / 16);
    actor->gravity = (u16)Random16();
    actor->riseBaseY = (u16)(Random16() << 1) + speed;
    spin = (u16)Random16() + 0xffff8000;
    actor->timer = spin;
    Actor_SetupScriptedFor(actor, 2);
    actor->update = (void (*)(void))Ride_UpdateDrift;
    Actor_SetBlendMode(actor, 1);
  }
}
