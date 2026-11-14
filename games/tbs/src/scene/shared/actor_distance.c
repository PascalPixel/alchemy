/// @file
/// The distance between two actors, for the scenes where someone reacts to
/// who is near: the Mercury and Venus Lighthouse aeries, Tolbi, Vault,
/// Venus Lighthouse, Xian and every scene with staged_actor.c.

/// The distance in whole pixels between two positions, each given by its x
/// and read on through the y and z that follow it, as an actor holds them.
// UB: an actor's x, y and z are separate members, read here as an array.
UNUSED static s32 Position_Distance(const s32 *first, const s32 *second) {
  s32 deltaX = (*first++ - *second++) >> 16;
  s32 deltaY = (*first++ - *second++) >> 16;
  s32 deltaZ = (*first - *second) >> 16;

  return Iwram_Sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

/// The distance in whole pixels between two actors, in all three axes.
#define Actor_Distance(first, second)                                          \
  Position_Distance(&(first)->x.fixed, &(second)->x.fixed)
