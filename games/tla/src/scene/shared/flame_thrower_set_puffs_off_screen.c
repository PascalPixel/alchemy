/* Sets whether flame thrower `index` puffs even off screen. */
void scFlameThrowerSetPuffsOffScr(s32 index, s16 value) {
	FLAME_THROWERS_STATE->entries[index].offScr = value;
}
