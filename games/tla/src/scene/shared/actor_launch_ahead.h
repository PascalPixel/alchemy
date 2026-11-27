// Launches the actor this one carries (scActorAttach) 32 pixels ahead of it, the way `userValue`
// faces, rolling at up to 8 pixels a tick (scActorRollAndCrumble), with the launch sound.
void fdActorLaunchAhead(ACTOR *self)
{
    ACTOR     *actor  = fdActorGet(self->launchedActorId);
    Sint32     angle  = self->userValue;
    FIELD_SPR *sprite = actor->sprite;
    FIXED      pos[3];

    pos[0] = self->x.fixed;
    pos[1] = self->y.fixed - toFIXED(12);
    pos[2] = self->z.fixed;
    syFieldVectorAddPolarOffset(32, angle, pos);
    actor->facing      = angle;
    actor->motionFlags = 0;
    scFieldActorSetPosAndResetMotion(actor, pos[0], pos[1], pos[2]);
    actor->update        = (OBJECT_UPDATE)scActorRollAndCrumble;
    actor->steeringFlags = 0;
    actor->speedLimit    = toFIXED(8);
    actor->acceleration  = toFIXED(1) / 8;
    sprite->rotation     = angle - DEGtoANG(90);
    actor->scaleX        = 0;
    actor->scaleY        = 0;
    actor->timer         = 0;
    actor->watchedActor  = self->watchedActor;
    fdScriptPlaySound(SE_LAUNCH);
}
