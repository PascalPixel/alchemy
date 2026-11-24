/// Unless the party returns to where it was, the leader appears in the
/// waterfall 16 pixels north of the entrance, tinted blue, as the screen opens,
/// then comes south out of it in a trail of mist and takes his own colours and
/// shadow back on the way.
void Leader_RiseFromWater(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    leader->motionFlags = 0;
    Actor_SetPositionFX(gMainRoutineState.leaderActorId,
                        INTtoFIXED(leader->x.part[0].integer),
                        INTtoFIXED(leader->z.part[0].integer) - toFIXED(16));
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineState.leaderActorId),
                              SPRITE_COLOR_BLUE_TINT);
    Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
    Event_StartScreenTransition();
    Event_PlaySound(SE_SPARKLING_PASSAGE);
    leader->update = (ObjectUpdate)Actor_EmitMist;
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.4);
    Actor_WalkByAndWait(gMainRoutineState.leaderActorId, 0, 8);
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineState.leaderActorId),
                              SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId),
                         SPRITE_FLAG_SHADOW);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    Actor_WalkByAndWait(gMainRoutineState.leaderActorId, 0, 10);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->update = NULL;
    Event_NoOp();
    Event_WaitTransitionFrames();
    Event_End();
  }
}
