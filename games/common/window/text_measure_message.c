/* mnTextMeasureMessage, the same in both games. Both games'
   window/window_measure.c include it where the function stands. */

/* Decodes message and returns its text's width and height in pixels. */
void mnTextMeasureMessage(Sint32 msg, Sint32 *x, Sint32 *y)
{
	mnTextMeasureDecoded(mnMessageDecode(msg, 0), (Uint32 *)x, (Uint32 *)y, 0);
}
