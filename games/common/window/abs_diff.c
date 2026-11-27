/* mnAbsDiff, the same in both games. Both games' window/screens_icon_menu.c
   include it where the function stands. */

/* Returns the distance between lhs and rhs, reusing their supplied
   difference. */
static inline Sint32 mnAbsDiff(Sint32 diff, Sint32 lhs, Sint32 rhs)
{
	if(diff >= 0)
		return diff;
	return rhs - lhs;
}
