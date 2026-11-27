/* Sets whether flame thrower `idx` puffs even off screen. */
void scFlameThrowerSetPuffsOffScreen(Sint32 idx, Sint16 value)
{
	FLAME_THROWERS_STATE->entries[idx].offScreen = value;
}
