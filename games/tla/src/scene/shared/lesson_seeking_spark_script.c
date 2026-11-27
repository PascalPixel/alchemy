/* A spark of the lesson at the heart of Air's Rock, Aqua Rock and Gaia Rock: */
/* it circles and homes in on the Adept. */
s32 Lesson_SeekingSparkScript[] = {
	SC_CALL(syOrbitUpdate),
	SC_CALL(fdActorAimAtWatched),
	SC_CALL(fdActorStepToWatched),
	SC_RELEASE,
	SC_END,
};
