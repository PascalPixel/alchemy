/* mnTitlePaletteFadeClampChannel, the same in both games. Both games'
   menu/title_intro.c include it where the function stands. */

/* Holds a colour channel's level to 0 to 31. */
Sint32 mnTitlePaletteFadeClampChannel(Sint32 value)
{
	if(value > RGB_LEVEL_MAX)
		return RGB_LEVEL_MAX;
	if(value < 0)
		value = 0;
	return value;
}
