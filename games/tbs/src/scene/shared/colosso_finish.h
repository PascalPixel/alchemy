/// @file
/// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
/// the end of the race at the arena, where each warrior walks up to a prize
/// and holds it up. The first to arrive takes the better one
/// (gColosso_OpponentFinishScript, gColosso_LeaderFinishScript).

// UB: calls a void function through an s32 function type, the call the bytes
// show; UBFIX calls it as declared.
#ifdef UBFIX
#define COLOSSO_GIVE_AND_EQUIP_ITEM Scene_GiveAndEquipItem
#else
#define COLOSSO_GIVE_AND_EQUIP_ITEM ((s32 (*)(s32, s32))Scene_GiveAndEquipItem)
#endif

/// Does nothing; the arena's step event calls it first.
static void Colosso_NoOp(void) {}

/// Unused: marks the opponent's recorded run finished, as
/// Colosso_UpdatePathOpponent does when it reaches the end of the path.
UNUSED static void Colosso_MarkOpponentFinished(void) {
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneLocalState;

  recorder->mode = PATH_RECORDER_FINISHED;
}

/// Waits until the opponent has run his recorded path to its end.
static void Colosso_WaitForOpponentToFinish(void) {
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneLocalState;

  while (recorder->mode != PATH_RECORDER_FINISHED) {
    wait(1);
  }
}

/// The raised prize's hook (gColosso_PrizeScript): it settles once it has
/// almost stopped rising, and on about one frame in ten a sparkle
/// (gColosso_PrizeSparkleScript) appears somewhere within 16 pixels of it.
static void Colosso_SparkleAroundPrize(struct Actor *prize) {
  s32 position[3];
  u32 chance;

  if (prize->velocityY >= -255 && prize->velocityY <= 255) {
    prize->motionFlags = 0;
  }
  chance = Random_Next();
  if (chance * 100 >> 16 <= 9) {
    struct Actor *sparkle;
    s32 distance;
    s32 angle;

    SET_VECTOR(position, prize->x.fixed, prize->y.fixed, prize->z.fixed);
    distance = Random_Next();
    angle = Random_Next();
    Vector_AddPolarOffset(distance << 4, angle, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      sparkle = FieldActor_Create(SPRITE_SPARKLE, x, y, z);
    }
    if (sparkle != NULL) {
      sparkle->motionFlags = 0;
      Actor_SetSpriteFlags(sparkle, 0);
      Actor_StartScript(sparkle, gColosso_PrizeSparkleScript);
      Actor_SelectAnimation(sparkle, 1);
      Actor_SelectAnimation(sparkle, 0);
    }
  }
}

/// Called from a warrior's finish script once he stands at his prize: the
/// prize (the actor id in his timer) rises to 36 pixels above him with the
/// treasure jingle, and the cleared timer lets Colosso_TakePrize go on.
static s32 Colosso_RaisePrize(struct Actor *warrior) {
  struct Actor *prize = Actor_Get(warrior->timer);

  FieldActor_SetMoveTargetFixed(prize, warrior->x.fixed,
                                warrior->y.fixed + px(36), warrior->z.fixed);
  prize->motionFlags = 0;
  Actor_StartScript(prize, gColosso_PrizeScript);
  Sound_Play(MUS_TREASURE_FOUND);
  warrior->timer = 0;
  return 0;
}

/// Isaac reaches the finish: he walks up to the first prize, or the second
/// when the opponent is there before him, holds it up, is given and equipped
/// with it, and MsgColossoIsaacGotItem names him and the item he got. Returns
/// nonzero when the opponent arrived first.
static s32 Colosso_TakePrize(s32 firstItem, s32 secondItem) {
  struct ColossoGaugeWork *gauge = (struct ColossoGaugeWork *)gColossoWork;
  struct Actor *leader;
  s32 opponentFirst;
  s32 x;
  s32 z;
  u16 *prizeActorId;
  s16 *raisedPrizeActorId;

  opponentFirst = GameFlag_Get(FLAG_ARENA_REACHED);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (gauge->finishX < leader->x.fixed) {
    x = gauge->finishX + px(12);
  } else {
    x = gauge->finishX - px(12);
  }
  if (opponentFirst != 0) {
    z = gauge->finishZ + px(16);
    prizeActorId = &gauge->finishItemActorIdB;
  } else {
    z = gauge->finishZ - px(16);
    prizeActorId = &gauge->finishItemActorIdA;
  }
  raisedPrizeActorId = &leader->timer;
  *raisedPrizeActorId = *prizeActorId;
  leader->acceleration = px(1) / 4;
  leader->speedLimit = px(1);
  FieldActor_SetMoveTargetFixed(leader, x, 0, z);
  GameFlag_Set(FLAG_ARENA_REACHED);
  Actor_StartScript(leader, gColosso_LeaderFinishScript);
  while (*raisedPrizeActorId != 0) {
    wait(1);
  }
  if (opponentFirst == 0) {
    COLOSSO_GIVE_AND_EQUIP_ITEM(ISAAC, firstItem);
    MessageArgument_Push(firstItem, MESSAGE_ARGUMENT_ITEM);
  } else {
    COLOSSO_GIVE_AND_EQUIP_ITEM(ISAAC, secondItem);
    MessageArgument_Push(secondItem, MESSAGE_ARGUMENT_ITEM);
  }
  MessageArgument_Push(gMainRoutineWork.leaderActorId, MESSAGE_ARGUMENT_ACTOR);
  FieldMessage_Show(MsgColossoIsaacGotItem, 3);
  Actor_WaitForScriptEnd(leader);
  return opponentFirst;
}
