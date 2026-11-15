/// The leader drops in from above through a cracked tile, lands with a burst of
/// dust and a shake.
void CrackedTile_DropIn(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  u32 i;
  struct EffectOptions options;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y.fixed = toFIXED(130);
  actor->gravity = toFIXED(1) / 2;
  actor->bounce = 0;
  actor->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 16 / 25;
  options.startScaleY = FIXED_ONE * 16 / 25;
  i = 0;
  do {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16() * (toFIXED(3) / 4) >> 16) - (toFIXED(3) / 8);
    velocity[2] += (Random16() * (toFIXED(3) / 8) >> 16) - (toFIXED(3) / 16);
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      velocity[0], velocity[1], velocity[2], 0x1090001,
                      &options);
    i++;
  } while (i <= 16);
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  actor->gravity = toFIXED(1);
  actor->bounce = FIXED_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 1);
  Event_End();
}
