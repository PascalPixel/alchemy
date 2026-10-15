#ifndef ALCHEMY_OAMBUF_H
#define ALCHEMY_OAMBUF_H

#include "types.h"

// OAM buffer: sprite entries a routine builds by hand and DMA copies to OAM.
// 手で組む OAM の写し 1 体分。DMA でそのまま OAM へ送る
// （LuckyDice_Run、ReelGame_RunFrame、Title_PlaceIntroSprites）
/* FAKEMATCH: the OAM shadow words are volatile, the form the bytes prove in all
 * three lists: loop.c forms no DEST_ADDR givs for volatile stores, so every
 * loop over a list keeps its offset givs. Without the qualifier LuckyDice_Run
 * (camera.c) differs 1702; ReelGame_RunFrame (colorfx.c) differs 119 without
 * attr01's (1730 instructions for 1729) and 1558 without attr2's (1701 for
 * 1729); Title_PlaceIntroSprites (palette.c) differs 205 (471 for 496). */
struct OamShadow {
  volatile u32 attr01; // attr0 | attr1 << 16
  volatile u32 attr2;  // attr2 | アフィン値 << 16
};

#endif
