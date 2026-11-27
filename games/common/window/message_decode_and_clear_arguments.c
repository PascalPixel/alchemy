/* mnMessageDecodeAndClearArguments, the same in both games. Both games'
   window/window_messages.c include it where the function stands. */

/* Decodes message and empties the argument table; returns where its decoded
   text starts. The Djinn and party join messages (screens_joined.c) use it. */
Sint32 mnMessageDecodeAndClearArguments(Sint32 msg)
{
	return mnMessageDecode(msg, 1);
}
