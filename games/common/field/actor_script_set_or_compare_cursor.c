/* fdActorScriptSetOrCompareCursor, the same in both games. Both games'
   field/field_script_commands.c include it where the function stands. */

/* Work value SW_CURSOR: sets, adds to or compares the script cursor. */
void fdActorScriptSetOrCompareCursor(
	SCRIPT_OPERANDS *operands, Sint32 operation, Sint32 value)
{
	if(operation == SCRIPT_OPERAND_SET) {
		operands->cursor = value;
	} else if(operation == SCRIPT_OPERAND_ADD) {
		operands->cursor = ((Sint16)operands->cursor + value);
	} else {
		operands->conditionResult = (Sint16)operands->cursor == (Sint16)value;
	}
}
