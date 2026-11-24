/// A flame thrower's spark (FLAME_SPARK): a red dust cloud thrown from
/// `position`, a little east or west, up by chance and south at `speed` and
/// more, spinning, blended over the room (FlameThrower_UpdateSpark). Unlike the
/// flames it never keeps its picture out of sight.
void FlameThrower_SendSpark(s32 *position, s32 speed, s32 offScreen) {
  struct Actor *actor = FieldActor_Create(
      SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));
  s32 spin;

  if (actor != NULL) {
    actor->bounce = Trig_Cos((u16)(Random16() * 3 >> 4) + TRIG_TURN * 3 / 16);
    actor->gravity = (u16)Random16();
    actor->sceneWord = (u16)(Random16() << 1) + speed;
    spin = (u16)Random16() + 0xffff8000;
    actor->timer = spin;
    Actor_SetupScriptedFor(actor, 2);
    actor->update = (ObjectUpdate)FlameThrower_UpdateSpark;
    Actor_SetBlendMode(actor, 1);
  }
}
