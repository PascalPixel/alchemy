/// Stone head `id` bursts out of the wall of handholds under Felix in a shower
/// of rocks as the screen shakes. He flails, drops onto whatever is under him,
/// the view settles on him, and he is left walking rather than climbing;
/// `flag` then keeps the head out.
void StoneHead_BurstOut(s32 id, s32 flag) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 gravity = leader->gravity;
  struct Actor *head;
  s32 i;
  s32 frame;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  head = Actor_Get(id);
  StoneHead_Place(id, leader->x.fixed, leader->y.fixed, leader->z.fixed);
  Actor_StartScript(head, gStoneHead_RiseScript);
  for (i = 0; i < STATUE_ROCKS; i++) {
    struct Actor *rock =
        FieldActor_Create(SPRITE_ROCK, leader->x.fixed,
                          leader->y.fixed + toFIXED(16), leader->z.fixed);

    if (rock != 0) {
      s32 angle;
      s32 speed;
      u32 size;
      s32 scale;

      Actor_StartScript(rock, gDebris_Script);
      angle = Random16();
      speed = Random16();
      size = Random16();
      scale = (size >> 1) + FIXED_ONE / 8;
      rock->velocityY = Iwram_MulFX(speed * 8, Trig_Cos(angle));
      rock->velocityX = Iwram_MulFX(speed * 8, Math_Sin(angle));
      rock->acceleration = 0;
      rock->scaleX = scale;
      rock->scaleY = scale;
      rock->gravity = toFIXED(33) / 100;
      rock->bounce = FIXED_ONE / 2;
    }
  }
  Event_PlaySound(SE_STATUE_RISE);
  Event_PlaySound(SE_FLOOR_SWITCH);
  MapGrid_SetShake(2, 2, 0.9);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SelectAnimation(leader, ANIM_LEADER_FLAIL);
  FieldEvent_Wait(40);
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->gravity = toFIXED(33) / 100;
  for (frame = 0; frame < 180; frame++) {
    System_Wait(1);
    if (leader->y.fixed <= leader->groundHeight)
      break;
  }
  Event_PlaySound(SE_THUMP);
  leader->gravity = gravity;
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_REMOVE);
  Actor_SelectAnimation(leader, ANIM_LEADER_STILL);
  MapGrid_SetShake(1, 4, 1);
  System_Wait(10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  System_Wait(20);
  FieldCamera_MoveToFX(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
  Actor_SelectAnimation(leader, ANIM_STAND);
  leader->velocityY = toFIXED(4);
  System_Wait(20);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  Event_End();
  Event_SetFlag(flag);
}
