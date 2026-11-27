// A spark of the lesson at the heart of Air's Rock, Aqua Rock and Gaia Rock: it circles and homes
// in on the Adept.
Sint32 LessonSeekingSparkScript[] = {
	SC_CALL(syOrbitUpdate),
	SC_CALL(scActorAimAtWatched),
	SC_CALL(scActorStepToWatched),
	SC_RELEASE,
	SC_END,
};
