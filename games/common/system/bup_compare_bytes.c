/* syBupCompareBytes, the same in both games. Both games' system/system_bup.c
   include it where the function stands. */

/* Compares `cnt` bytes, as memcmp does. */
Sint32 syBupCompareBytes(Uint8 *left, Uint8 *right, Sint32 cnt)
{
	Sint32 difference = 0;

	while(cnt != 0) {
		difference = *left - *right;
		if(difference != 0)
			break;
		cnt--;
		left++;
		right++;
	}
	return difference;
}
