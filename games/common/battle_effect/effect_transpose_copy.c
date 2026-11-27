/* btEffectTransposeCopy, the same in both games. Both games'
   battle_effect/battle_effect_djinn_attacks.c include it where the function
   stands. */

/* Copies a byte picture turned 90 degrees clockwise. */
void btEffectTransposeCopy(const Uint8 *src, Uint8 *dst, Sint32 w, Sint32 h)
{
	Sint32 x;
	Sint32 y;

	for(y = 0; y != h; y++) {
		for(x = 0; x != w; x++)
			dst[x * h + (h - 1 - y)] = src[y * w + x];
	}
}
