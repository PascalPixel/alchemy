/* Stone head `id` bursts out of the wall of handholds under Felix in a shower
   of rocks as the screen shakes. He flails, drops onto whatever is under him,
   the view settles on him, and he is left walking rather than climbing; `flag`
   then keeps the head out. */
void scStoneHeadBurstOut(Sint32 id, Sint32 flag)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 gravity = leader->gravity;
	ACTOR *head;
	Sint32 i;
	Sint32 frame;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	head = fdActorGet(id);
	scStoneHeadPlace(id, leader->x.fixed, leader->y.fixed, leader->z.fixed);
	fdActorStartScript(head, StoneHead_RiseScript);
	for(i = 0; i < STATUE_ROCKS; i++) {
		ACTOR *rock = scFieldActorCreate(SPRITE_ROCK, leader->x.fixed,
			leader->y.fixed + toFIXED(16), leader->z.fixed);

		if(rock != 0) {
			Sint32 angle;
			Sint32 speed;
			Uint32 size;
			Sint32 scale;

			fdActorStartScript(rock, Debris_Script);
			angle = syRandom();
			speed = syRandom();
			size = syRandom();
			scale = (size >> 1) + toFIXED(1) / 8;
			rock->velY = syMulFX(speed * 8, syCos(angle));
			rock->velX = syMulFX(speed * 8, syMathSin(angle));
			rock->acceleration = 0;
			rock->scaleX = scale;
			rock->scaleY = scale;
			rock->gravity = toFIXED(33) / 100;
			rock->bounce = toFIXED(1) / 2;
		}
	}
	fdScriptPlaySnd(SE_STATUE_RISE);
	fdScriptPlaySnd(SE_FLOOR_SWITCH);
	fdMapGridSetShake(2, 2, 0.9);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSelectAnimation(leader, ANIM_LEADER_FLAIL);
	syFieldScriptWait(40);
	fdScriptPlaySnd(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	leader->gravity = toFIXED(33) / 100;
	for(frame = 0; frame < 180; frame++) {
		syWait(1);
		if(leader->y.fixed <= leader->groundHeight)
			break;
	}
	fdScriptPlaySnd(SE_THUMP);
	leader->gravity = gravity;
	fdActorSetAttachedEffect(
		MainRoutineStateWork.leaderActorId, ATTACHED_EFFECT_REMOVE);
	fdActorSelectAnimation(leader, ANIM_LEADER_STILL);
	fdMapGridSetShake(1, 4, 1);
	syWait(10);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syWait(20);
	fdCameraMoveToFX(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorSetSprFlags(leader, SPRITE_FLAG_SHADOW);
	fdActorSelectAnimation(leader, ANIM_STAND);
	leader->velY = toFIXED(4);
	syWait(20);
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
	fdScriptEnd();
	fdScriptSetFlag(flag);
}
