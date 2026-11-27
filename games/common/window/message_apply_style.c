/* mnMessageApplyStyle, the same in both games. Both games'
   window/window_windows.c include it where the function stands. */

/* Makes the message's text colour, style and line spacing the ones glyphs are
   drawn with, before it draws its next character. */
void mnMessageApplyStyle(MESSAGE *msg)
{
	WIN_STATE *state = WINDOW_STATE;

	state->colour = msg->colour;
	state->outline = msg->outline;
	state->lineSpacing = msg->lineSpacing;
}
