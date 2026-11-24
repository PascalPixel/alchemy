/// @file
/// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
/// what a stage's guide uses to describe the stage. Isaac appears on the stage
/// to show how it is crossed, a blinking pointer moves over the screen, and
/// Isaac is put back where he stood.

/// The sprite outline's colours: the field's own, as field_actors.c sets them
/// up, and those of the warrior a stage's guide shows.
#define FIELD_OUTLINE_EDGE 15
#define FIELD_OUTLINE_FILL 0
#define WARRIOR_OUTLINE_EDGE 1
#define WARRIOR_OUTLINE_FILL 4

/// Isaac (id) appears at pixel (x, z) on the stage, facing south: he fades in
/// through the backgrounds while he stretches from thin and tall to his own
/// shape, settles and nods. While he is shown his sprite is outlined in the
/// warrior's colours. His position and facing are kept for
/// Colosso_RestoreWarrior.
void Colosso_FadeInWarrior(s32 id, s32 x, s32 z) {
  struct ActorSystemState *system;
  struct Actor *actor;
  struct Sprite *sprite;
  s32 i;

  system = gActorSystemState;
  actor = Actor_GetById(id);
  system->edge = WARRIOR_OUTLINE_EDGE;
  system->fill = WARRIOR_OUTLINE_FILL;
  gColosso_WarriorStartX = actor->x.fixed;
  gColosso_WarriorStartZ = actor->z.fixed;
  sprite = (struct Sprite *)actor->sprite;
  gColosso_WarriorStartAngle = actor->facing;
  Actor_SetSpritePriority(id, 2);
  actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  actor->facing = FACING_SOUTH;
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW | SPRITE_FLAG_OUTLINED);
  Actor_SelectAnimation(actor, ANIM_STILL);
  Actor_SelectAnimation(actor, ANIM_STAND);
  Actor_SetPositionFX(id, toFIXED(x), toFIXED(z));
  // BUG: FACING_SOUTH is passed where an actor id belongs; no actor has
  // it, so the leader does not turn.
  Actor_FaceActorById(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 |
                          BLDCNT_TGT2_BG3,
                      IO_WRITE_HALFWORD);
  sprite->image.objectMode = OBJ_MODE_SEMI_TRANSPARENT;
  sprite->shadow.objectMode = OBJ_MODE_SEMI_TRANSPARENT;
  Sound_Play(SE_COLOSSO_WARRIOR_APPEAR);
  for (i = 0; i < BLEND_WEIGHT_MAX; i += 2) {
    actor->scaleX = i * (FIXED_ONE / 16) + FIXED_ONE / 16;
    actor->scaleY = 31 * FIXED_ONE / 16 - i * (FIXED_ONE / 16);
    // BLEND_ALPHA(i + 1, BLEND_WEIGHT_MAX - 1 - i), its bottom weight
    // written first.
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                        ((BLEND_WEIGHT_MAX - 1 - i) << 8) | (i + 1),
                        IO_WRITE_HALFWORD);
    wait(1);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(BLEND_WEIGHT_MAX, 0),
                      IO_WRITE_HALFWORD);
  actor->scaleX = 17 * FIXED_ONE / 16;
  actor->scaleY = 15 * FIXED_ONE / 16;
  Event_Wait(1);
  actor->scaleX = FIXED_ONE;
  actor->scaleY = FIXED_ONE;
  Event_Wait(13);
  sprite->image.objectMode = OBJ_MODE_NORMAL;
  sprite->shadow.objectMode = OBJ_MODE_NORMAL;
  Actor_SetAnimationAndWait(id, ANIM_NOD);
  Event_Wait(20);
}

/// Puts Isaac (id) back where Colosso_FadeInWarrior found him: he stands if
/// he was climbing, or else turns south, nods and waits half a second; then
/// returns to his kept position and facing with his motion stopped, and the
/// outline colours are the field's again.
void Colosso_RestoreWarrior(s32 id) {
  struct ActorSystemState *system;
  struct Actor *actor;

  system = gActorSystemState;
  actor = Actor_GetById(id);
  if (gMainRoutineState.movementMode == 1) { // climbing
    gMainRoutineState.movementMode = 0;
    Actor_SetAnimation(id, ANIM_STAND);
  } else {
    Actor_FaceDirection(id, FACING_SOUTH, 30);
    Actor_SetAnimation(id, ANIM_NOD);
    Event_Wait(30);
  }
  system->fill = FIELD_OUTLINE_FILL;
  system->edge = FIELD_OUTLINE_EDGE;
  actor->x.fixed = gColosso_WarriorStartX;
  actor->z.fixed = gColosso_WarriorStartZ;
  actor->facing = gColosso_WarriorStartAngle;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->layer = 0;
  actor->y.fixed = 0;
  actor->groundHeight = 0;
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Actor_SelectAnimation(actor, ANIM_STILL);
  Actor_SelectAnimation(actor, ANIM_STAND);
  wait(1);
}

/// Stops the actor where it is: no velocity and no destination.
void Actor_ResetMotionFields(s32 actorId) {
  struct Actor *actor = Actor_GetById(actorId);

  Actor_StartIdle(actor);
  actor->velocityX = 0;
  actor->velocityZ = 0;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
}

/// The guide's pointer: one 16x16 sprite centred on a point of the screen,
/// shown for POINTER_SHOWN_FRAMES of every POINTER_BLINK_FRAMES, that can glide
/// from point to point.
#define POINTER_SHOWN_FRAMES 13
#define POINTER_BLINK_FRAMES 20

/// Loads the pointer's picture into a free VRAM block, once.
static void Colosso_LoadPointer(void) {
  if (gColosso_PointerSlot == -1) {
    gColosso_PointerSlot = PointingHand_LoadIntoFreeSlot();
  }
}

/// The pointer's glide: its steps and the step it is on, where it is now, where
/// the glide started and where it ends (screen pixels), its blink counter and
/// its flip.
extern s16 gColosso_PointerSteps;
extern s16 gColosso_PointerStep;
extern s16 gColosso_PointerX;
extern union PointerCoordinate gColosso_PointerStartX;
extern s16 gColosso_PointerEndX;
extern s16 gColosso_PointerY;
extern union PointerCoordinate gColosso_PointerStartY;
extern s16 gColosso_PointerEndY;
extern s16 gColosso_PointerBlink;
extern s16 gColosso_PointerFlip;

/// The pointer's task: glide one step, then draw it while it blinks
/// on (a glide keeps it on).
static void Colosso_UpdatePointer(void) {
  s32 tile;
  s32 total;
  s32 step;
  s32 *oam;
  s32 *dst;
  s32 x;
  s32 y;

  oam = gColosso_PointerOam;
  tile = gVramCacheEntries[gColosso_PointerSlot].offset >> 5;
  total = gColosso_PointerSteps;
  if (total != 0) {
    step = ++gColosso_PointerStep;
    gColosso_PointerX =
        gColosso_PointerStartX.pixels +
        (gColosso_PointerEndX - gColosso_PointerStartX.signedPixels) *
            (s16)step / total;
    gColosso_PointerY =
        gColosso_PointerStartY.pixels +
        (gColosso_PointerEndY - gColosso_PointerStartY.signedPixels) *
            (s16)step / total;
    if ((s16)step >= total)
      gColosso_PointerSteps = 0;
    gColosso_PointerBlink = 0;
  }
  if (++gColosso_PointerBlink <= POINTER_SHOWN_FRAMES) {
    s32 size = OBJ_SIZE_16X16;

    x = gColosso_PointerX;
    y = gColosso_PointerY;
    dst = oam;
    *dst++ = 0;
    *dst++ = (y - 8) | OBJ_X(x - 8) | size | gColosso_PointerFlip * OBJ_HFLIP;
    *dst = tile | OBJ_PRIORITY(1);
    Oam_AddSpriteAtDepth(oam, OAM_DEPTH_FRONT);
  } else if (gColosso_PointerBlink >= POINTER_BLINK_FRAMES) {
    gColosso_PointerBlink = 0;
  }
}

/// Shows the pointer at screen pixel (x, y), mirrored by style & 3 (1 left to
/// right, 2 top to bottom); every caller passes 0.
void Colosso_ShowPointer(u32 x, u32 y, u32 style) {
  Colosso_LoadPointer();

  gColosso_PointerX = x;
  gColosso_PointerY = y;
  gColosso_PointerFlip = (style & 3);
  gColosso_PointerBlink = 0;
  gColosso_PointerSteps = 0;

  Task_AddFrame(Colosso_UpdatePointer, TASK_PASS_FRAME);
}

/// Glides the pointer to screen pixel (x, y) over duration frames.
void Colosso_MovePointer(u32 x, u32 y, u32 duration) {
  gColosso_PointerEndX = x;
  gColosso_PointerEndY = y;
  *(u16 *)&gColosso_PointerStartX = gColosso_PointerX;
  *(u16 *)&gColosso_PointerStartY = gColosso_PointerY;
  gColosso_PointerSteps = duration;
  gColosso_PointerStep = 0;

  Task_AddFrame(Colosso_UpdatePointer, TASK_PASS_FRAME);
}

/// Hides the pointer and frees its VRAM block.
void Colosso_HidePointer(void) {
  Task_Remove(Colosso_UpdatePointer);
  VramCache_FreeEntry(gColosso_PointerSlot);
  gColosso_PointerSlot = -1;
}

/// Runs the actor at 2 pixels a frame towards pixel (x, z) and returns
/// at once.
void Colosso_WalkActorTo(s32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    FIXED speed = toFIXED(2);
    s32 unpaused = 0;

    actor->speedLimit = speed;
    actor->acceleration = speed >> 1;
    actor->paused = unpaused;
    Actor_StartIdle(actor);
    Actor_SelectAnimation(actor, ANIM_LEADER_RUN);
    Actor_SetMoveTarget(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
  }
}

/// Runs the actor at 1.25 pixels a frame to pixel (x, z), waits until
/// it arrives and stands it there.
void Colosso_WalkActorToAndStand(s32 actorId, s32 x, s32 z) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL) {
    FIXED speed = toFIXED(5) / 4;
    s32 unpaused = 0;

    actor->speedLimit = speed;
    actor->acceleration = speed >> 1;
    actor->paused = unpaused;
    Actor_StartIdle(actor);
    Actor_SelectAnimation(actor, ANIM_LEADER_RUN);
    Actor_SetMoveTarget(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
    Actor_WaitForMove(actor);
    Actor_SelectAnimation(actor, ANIM_STAND);
  }
}
