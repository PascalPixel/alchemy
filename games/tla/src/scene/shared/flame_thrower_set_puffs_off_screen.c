/* Sets whether flame thrower `index` puffs even off screen. */
void scFlameThrowerSetPuffsOffScreen(Sint32 index, Sint16 value)
{
	FLAME_THROWERS_STATE->entries[index].offScreen = value;
}
