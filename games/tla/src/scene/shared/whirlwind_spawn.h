/* A whirlwind blows out of the stone face at `source`. Mars Lighthouse keeps a
   copy nothing calls, its whirlwinds starting from a gust. */
ACTOR *scWhirlwindSpawn(ACTOR *source)
{
	VECTOR3		   position;
	EFFECT_OPTIONS options;
	ACTOR		  *actor;

	fdScriptPlaySnd(SE_WHIRLWIND);
	position.x =
		source->x.fixed + syMathMulFX(toFIXED(8), syCos(source->facing));
	position.z =
		source->z.fixed + syMathMulFX(toFIXED(8), syMathSin(source->facing));
	actor = scFieldActorCreate(
		SPRITE_WHIRLWIND, position.x, source->y.fixed, position.z);
	fdActorSelectAnimation(actor, 2);
	fdActorSetSprFlags(actor, 0);
	actor->motionFlg = 0;
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	actor->facing = source->facing;
	actor->tmr = 0;
	actor->sceneCnt = 0;
	actor->sceneByte = 0;
	actor->update = scWhirlwindUpdate;
	actor->speedLimit = toFIXED(3) / 2;
	options.pri = 1;
	options.pal = SPRITE_COLOR_WHITE;
	scFieldEffectSpawn(position.x, source->y.fixed, position.z - toFIXED(6), 0,
		0, 0, EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY, &options);
	return actor;
}
