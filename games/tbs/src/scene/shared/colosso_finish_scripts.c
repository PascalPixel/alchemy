/* The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
   the warriors' scripts as each reaches his prize at the finish: he holds it up
   (scColossoRaisePrize) and holds a pose for two and a half seconds, both arms
   raised, then turns south. */

/* The opponent's, from scColossoUpdatePathOpponent. */
Sint32 Colosso_OpponentFinishScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(6),
	SC_CALL(scColossoRaisePrize),
	SC_WAIT(3),
	SC_ANIMATION(WARRIOR_ANIMATION_ARMS_RAISED),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Isaac's, from scColossoTakePrize. */
Sint32 Colosso_LeaderFinishScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(6),
	SC_CALL(scColossoRaisePrize),
	SC_WAIT(3),
	SC_ANIMATION(ANIM_LEADER_ARMS_RAISED),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};
