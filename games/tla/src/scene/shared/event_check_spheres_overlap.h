/// Whether two spheres overlap (Field_CheckSpheresOverlap), from an event.
static inline s32 Event_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b,
                                            s32 radiusB) {
  return Field_CheckSpheresOverlap(a, radiusA, b, radiusB);
}
