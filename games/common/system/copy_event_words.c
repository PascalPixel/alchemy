/* syCopyEventWords, the same in both games. Both games'
   system/system_event.c include it where the function stands. */

/* Unused: copies `byteCnt` bytes a word at a time. */
UNUSED void syCopyEventWords(Uint32 *dst, Uint32 *src, Uint32 byteCnt)
{
	Uint32 idx;
	byteCnt >>= 2;
	for(idx = 0; idx < byteCnt; idx++)
		*dst++ = *src++;
}
