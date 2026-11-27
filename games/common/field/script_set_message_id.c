/* fdScriptSetMessageId, the same in both games. Both games'
   field/script_commands_actor_commands.c include it where the function
   stands. */

/* The script command "set message": the number of the script's next message,
   written to the field state's message. */
void fdScriptSetMessageId(Sint32 msg)
{
	FieldStateWork->msg = msg;
}
