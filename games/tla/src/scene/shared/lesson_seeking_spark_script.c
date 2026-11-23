/// A spark of the lesson at the heart of Air's Rock, Aqua Rock and Gaia Rock:
/// it circles and homes in on the Adept.
s32 gLesson_SeekingSparkScript[] = {
    SC_CALL(Orbit_Update),
    SC_CALL(Actor_AimAtWatched),
    SC_CALL(Actor_StepToWatched),
    SC_RELEASE,
    SC_END,
};
