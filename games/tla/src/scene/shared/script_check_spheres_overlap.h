/* Whether two spheres overlap (fdCheckSpheresOverlap), from a script. */
static inline Sint32 scScriptCheckSpheresOverlap(
	Sint32 *a, Sint32 radiusA, Sint32 *b, Sint32 radiusB)
{
	return fdCheckSpheresOverlap(a, radiusA, b, radiusB);
}
