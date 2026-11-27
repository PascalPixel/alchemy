/* mnMessageDecodeRawUnbounded and mnMessageCopyExpanded, the same in both
   games. Both games' window/window_messages.c include it where the functions
   stand. */

/* Decodes message into text (a Uint16 string) with no length limit: text must
   have room for the whole message and its terminator. */
void mnMessageDecodeRawUnbounded(Sint32 msg, Uint16 *text)
{
	mnMessageDecodeRaw(msg, text, MESSAGE_CAPACITY_UNLIMITED);
}

/* Decodes message into destination as a zero-terminated string of at most
   capacity entries, and returns its length. capacity counts halfwords including
   the terminator and must be at least 1. */
Sint32 mnMessageCopyExpanded(Sint32 msg, Uint16 *dst, Uint32 capacity)
{
	WIN_STATE *state;
	Uint32	   cnt;

	state = WINDOW_STATE;
	state->textBufEnd = 0;
	mnMessageDecode(msg, 1);
	for(cnt = 0; cnt < capacity - 1 && (dst[cnt] = state->textBuf[cnt]) != 0;
		cnt++) {
	}
	dst[cnt] = 0;
	return cnt;
}
