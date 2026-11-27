/* fdFixedPointReciprocal, the same in both games. Both games'
   field/lucky_dice.c include it where the function stands. */

/* Unused: the reciprocal of an 8.8 fixed-point value. */
UNUSED Sint16 fdFixedPointReciprocal(Sint16 value)
{
	return toFIXED(1) / value;
}
