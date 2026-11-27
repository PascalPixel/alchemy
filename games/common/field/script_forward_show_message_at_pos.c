/* fdScriptForwardShowMessageAtPos, the same in both games. Both games'
   field/script_commands_messages.c include it where the function stands. */

/* Unused: nothing calls it; forwards its arguments to
   fdScriptShowMessageAtPos. */
void fdScriptForwardShowMessageAtPos(
	Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y)
{
	fdScriptShowMessageAtPos(unusedA, unusedB, x, y);
}
