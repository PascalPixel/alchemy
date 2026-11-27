/* Whether two spheres overlap (fdCheckSpheresOverlap), from an event. */
static inline s32 scEventCheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b,
											 s32 radiusB) {
	return fdCheckSpheresOverlap(a, radiusA, b, radiusB);
}
