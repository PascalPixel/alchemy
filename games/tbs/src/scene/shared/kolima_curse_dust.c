/// @file
/// Kolima and Kolima Forest (SceneId_Kolima and SceneId_KolimaForestEntrance):
/// the sparkling dust that falls over the party when Tret's curse strikes
/// ("Watch out! It's happening again!").

#include "macros.h"
#include "field/map_grid_scroll.h"

/// Where the dust falls, east of the point the map view follows: the band's
/// west edge, 16.16 pixels from that point, and its width, in 256ths of a
/// pixel; the event closes the band in (enum KolimaCurseDustState).
s32 gKolima_CurseDustOffset BSS_DATA;
s32 gKolima_CurseDustWidth BSS_DATA;
u32 gKolima_CurseDustState BSS_DATA;

/// A task: every eighth frame a speck (its script gKolima_CurseDustScript)
/// starts 160 pixels up at a random point of the band, or anywhere within 128
/// pixels of the point the map view follows once the dust is centred, with
/// SE_GLITTER every 64 frames.
void Kolima_UpdateCurseDust(void) {
  struct Actor *actor = NULL;
  s32 *origin;
  struct Sprite *sprite;
  s32 x;
  FIXED z;

  switch (gKolima_CurseDustState) {
  case KOLIMA_DUST_CLOSING:
    if (gKolima_CurseDustWidth < 15000)
      gKolima_CurseDustWidth += 50;
    if (gKolima_CurseDustOffset > toFIXED(60))
      gKolima_CurseDustOffset -= toFIXED(1) / 4;
    break;
  case KOLIMA_DUST_CLOSING_WIDE:
    if (gKolima_CurseDustWidth < 30000)
      gKolima_CurseDustWidth += 50;
    if (gKolima_CurseDustOffset > toFIXED(24))
      gKolima_CurseDustOffset -= toFIXED(1) / 4;
    break;
  case KOLIMA_DUST_DRIFTING:
    if (gKolima_CurseDustOffset < -toFIXED(128)) {
      gKolima_CurseDustState = KOLIMA_DUST_CENTRED;
    } else {
      gKolima_CurseDustWidth += 50;
      gKolima_CurseDustOffset -= toFIXED(1) / 4;
    }
    break;
  }
  if ((gFrameCount & 7) == 0 &&
      (actor = FieldActor_Create(SPRITE_SPARKLE, 0, 0, 0)) != 0) {
    origin = ((struct MapGridScrollState *)gMapGridState[0])->origin;
    if ((gFrameCount & 63) == 0)
      Sound_Play(SE_GLITTER);
    if (gKolima_CurseDustState != KOLIMA_DUST_CENTRED)
      x = origin[0] + ((Random_Below(gKolima_CurseDustWidth)) << 8) +
          gKolima_CurseDustOffset;
    else
      x = origin[0] + (Random_Next() << 8) - toFIXED(128);
    z = origin[2] + (Random_Next() << 8) - toFIXED(128);
    actor->motionFlags = 0;
    actor->y.fixed = toFIXED(160);
    sprite = actor->sprite;
    actor->scaleX = 9 * FIXED_ONE / 10;
    actor->scaleY = 9 * FIXED_ONE / 10;
    actor->x.fixed = x;
    actor->z.fixed = z;
    sprite->flags = 0;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority = 1;
    Actor_SelectAnimation(actor, 1);
    Actor_StartScript(actor, gKolima_CurseDustScript);
  }
}
