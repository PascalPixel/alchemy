/// The leader rises out of the water, unless the party keeps its place.
void Leader_RiseFromWater(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 flag = Event_GetFlag(FLAG_KEEP_PARTY_POSITION);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Actor_SetPositionFixed(*owner, ((s16 *)&leader->x)[1] << 16,
                           (((s16 *)&leader->z)[1] << 16) - 0x100000);
    FieldActor_SetSpriteColor(Actor_Get(*owner), 9);
    Actor_SetSpriteFlags(Actor_Get(*owner), 0);
    Event_StartScreenTransition();
    Event_PlaySound(SE_SPARKLING_PASSAGE);
    leader->update = (void (*)(void))Actor_EmitMist;
    Actor_SetSpeed(*owner, 0.4);
    Actor_WalkByAndWait(*owner, 0, 8);
    FieldActor_SetSpriteColor(Actor_Get(*owner), 0);
    Actor_SetSpriteFlags(Actor_Get(*owner), 1);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    offset = 10;
    Actor_WalkByAndWait(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Event_NoOp();
    Event_WaitTransitionFrames();
    Event_End();
  }
}
