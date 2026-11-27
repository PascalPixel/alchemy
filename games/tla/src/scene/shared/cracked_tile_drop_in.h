// The leader drops in from above through a cracked tile, lands with a burst of dust and a shake.
void scCrackedTileDropIn(void)
{
    ACTOR         *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    Uint32         i;
    EFFECT_OPTIONS options;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdMapGridCenterOnFollowedPoint();
    syWait(1);
    leader->y.fixed            = toFIXED(130);
    leader->gravity            = toFIXED(1) / 2;
    leader->bounce             = 0;
    leader->motionFlags        = 0;
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptPlaySound(SE_DUST_PUFF);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    syFieldScriptWait(24);
    options.palette     = SPRITE_COLOR_WHITE;
    options.update      = fdEffectAdvanceMotion;
    options.startScaleX = toFIXED(16) / 25;
    options.startScaleY = toFIXED(16) / 25;
    // A puff each sixteenth of a turn, the last over the first.
    for (i = 0; i <= TRIG_TURN / FACING_STEP; i++)
    {
        Sint32 angle;
        Sint32 velocity[3];

        angle        = i * FACING_STEP;
        velocity[0]  = syCos(angle);
        velocity[1]  = 0;
        velocity[2]  = syMathSin(angle);
        velocity[0] += velocity[0] >> 1;
        velocity[0] += (syRandom() * (toFIXED(3) / 4) >> 16) - (toFIXED(3) / 8);
        velocity[2] += (syRandom() * (toFIXED(3) / 8) >> 16) - (toFIXED(3) / 16);
        scFieldEffectSpawn(leader->x.fixed,
                           leader->y.fixed,
                           leader->z.fixed,
                           velocity[0],
                           velocity[1],
                           velocity[2],
                           EFFECT_USE_PALETTE | EFFECT_USE_START_SCALE | EFFECT_USE_UPDATE
                               | EFFECT_SCRIPT_MEDIUM,
                           &options);
    }
    fdScriptPlaySound(SE_THUMP);
    fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
                             ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT);
    fdMapGridSetShake(5, 5, 1);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    syFieldMapGridWaitForShakeToSettle();
    fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
                             ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    leader->gravity = toFIXED(1);
    leader->bounce  = toFIXED(1) / 4;
    syFieldScriptWait(10);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
    fdScriptEnd();
}
