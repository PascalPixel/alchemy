/* fdActorScriptApplyOperandSet, fdActorScriptApplyOperandAdd and
   fdActorScriptApplyOperandCompare, the same in both games. Both games'
   field/field_script_commands.c include it where the functions stand. */

/* SC_SET(w, v): sets work value w to v. */
Sint32 fdActorScriptApplyOperandSet(ACTOR *interpreter)
{
	SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

	return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_SET);
}

/* SC_ADD(w, v): adds v to work value w. */
Sint32 fdActorScriptApplyOperandAdd(ACTOR *interpreter)
{
	SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

	return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_ADD);
}

/* SC_CMP(w, v): tests whether work value w equals v. */
Sint32 fdActorScriptApplyOperandCompare(ACTOR *interpreter)
{
	SCRIPT_OPERANDS *operands = (SCRIPT_OPERANDS *)interpreter;

	return fdActorScriptApplyOperandCommand(operands, SCRIPT_OPERAND_COMPARE);
}
