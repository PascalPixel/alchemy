/// The leader walks 8 pixels north in a trail of mist, then turns blue and
/// loses his shadow as the screen closes, and leaves by exit `entrance`, with
/// the sparkling sound of a hidden way: Aqua Rock's and the Apojii Islands'
/// passages through water. Leader_RiseFromWater is the arrival.
void Leader_SinkAway(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  s32 *owner = &gMainRoutineState.leaderActorId;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Event_PlaySound(SE_SPARKLING_PASSAGE);
  leader->update = (ObjectUpdate)Actor_EmitMist;
  leader->motionFlags = 0;
  leader->speedLimit = toFIXED(1) / 5;
  Actor_SetAnimation(*owner, 2);
  Actor_SetDestinationOffset(*owner, 0, -8);
  FieldActor_WaitForMoveById(*owner);
  FieldActor_SetSpriteColor(Actor_Get(*owner), SPRITE_COLOR_BLUE_TINT);
  Actor_SetSpriteFlags(Actor_Get(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
  Event_End();
}
