/* fdBlendScriptSuspend and fdBlendScriptResume, the same in both games. Both
   games' field/field_tile_animation.c include it where the functions stand. */

/* Suspends the blend script; scenes reach it through fdBlendScriptSuspendStub,
   in The Broken Seal the lighthouse scenes and Vale's Babi script. */
void fdBlendScriptSuspend(void)
{
	sySuspendEvent(fdBlendScriptRun);
}

/* Resumes the blend script, through fdBlendScriptResumeStub. */
void fdBlendScriptResume(void)
{
	syResumeEvent(fdBlendScriptRun);
}
