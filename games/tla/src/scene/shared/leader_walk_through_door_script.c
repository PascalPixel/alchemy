/* The leader's actor script as he walks north through a doorway, growing */
/* smaller as he goes: in Madra's catacombs, the Lemurian ship's passage, and */
/* the doors of the Islets, Kibombo, Prox and the Shaman Village. */
s32 Leader_WalkThroughDoorScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Z, -toFIXED(1) / 2),
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 64),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
	SC_WAIT(1),
	SC_LOOP(24, SC_TOP),
	SC_END,
};
