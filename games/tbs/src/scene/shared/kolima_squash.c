/* @file */
/* Kolima and Kolima Forest: the party quivers under Tret's curse as it falls,
 */
/* and is set back to its own size as each collapses. */

/* Each party member squashes and wobbles until another script takes over. */
s32 Kolima_PartySquashScript[] = {
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(2),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Back to full size as the party member collapses. */
s32 Kolima_PartyRestoreScaleScript[] = {
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_WAIT(1),
	SC_END,
};
