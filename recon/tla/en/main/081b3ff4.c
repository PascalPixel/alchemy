// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 50 of
// 1072; not worked this pass.
// Draft, differ 118 of 1072: The Broken Seal's LzPack_PackStream
// (games/tbs/src/object/colorfx.c), the small LZ packer, as ☀️ writes it;
// the same 1072 instructions with every register the listing's. Left:
// instruction order only, all of it the native issuing a load earlier: the
// first pool load before sub sp, pool and stack loads between the halves of
// split constants (movs #136; lsls #7 for the work offsets, movs #192; lsls
// #2 for 0x3ff), and stack spills a slot earlier. No empty asm or one-pass
// block can put a load between a split constant's halves.
#include "lzpack.h"
#include "ram_buffer.h"

#define PACK_WINDOW_MASK 0x3ff
#define PACK_MATCH_MAX 272
#define PACK_DISTANCE_MAX 63
#define PACK_LOOKAHEAD 672

static __inline__ void LzPack_FindWindowMatch(s32 start) {
  struct LzPackNode *node;
  s32 len;

  gFlash_Handler3->matchLen = 1;
  if (gFlash_Handler3->bucketBySlot[start] == -1) {
    return;
  }
  node = gFlash_Handler3->bucket[gFlash_Handler3->bucketBySlot[start]];
  if (start + PACK_MATCH_MAX > PACK_WINDOW_MASK) {
    while (node != NULL) {
      s32 dist = (start - node->slot) & PACK_WINDOW_MASK;

      if (dist > 0 && dist <= PACK_DISTANCE_MAX) {
        for (len = 1; len < PACK_MATCH_MAX; len++) {
          if (gFlash_Handler3->bucketBySlot[(start + len) & PACK_WINDOW_MASK] !=
              gFlash_Handler3
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gFlash_Handler3->matchLen < len) {
          gFlash_Handler3->matchOffset = dist;
          gFlash_Handler3->matchLen = len;
          if (len == PACK_MATCH_MAX) {
            return;
          }
        }
      }
      node = node->next;
    }
  } else {
    while (node != NULL) {
      s32 dist = (start - node->slot) & PACK_WINDOW_MASK;

      if (dist > 0 && dist <= PACK_DISTANCE_MAX) {
        for (len = 1; len < PACK_MATCH_MAX; len++) {
          if (gFlash_Handler3->bucketBySlot[start + len] !=
              gFlash_Handler3
                  ->bucketBySlot[(node->slot + len) & PACK_WINDOW_MASK]) {
            break;
          }
        }
        if (gFlash_Handler3->matchLen < len) {
          gFlash_Handler3->matchOffset = dist;
          gFlash_Handler3->matchLen = len;
          if (len == PACK_MATCH_MAX) {
            return;
          }
        }
      }
      node = node->next;
    }
  }
}

s32 Func_081b3ff4(const u8 *input, u8 *dst, s32 size) {
  s32 defer;
  s32 len;
  s32 saveOffset;
  s32 saveLen;
  s32 reach[2];
  s16 code;

  defer = 0;
  gFlash_Handler3 = (struct LzPackWork *)Ram_MapCellBuffer;
  LzPack_ResetSlotBuckets();
  gFlash_Handler3->inputLimit = size;
  gFlash_Handler3->pos = 0;
  gFlash_Handler3->inputCursor = 0;
  gFlash_Handler3->outTotal = 0;
  gFlash_Handler3->flagMask = 0x80;
  gFlash_Handler3->outBuf[0] = 0;
  gFlash_Handler3->outCnt = 1;
  LzPack_ConsumeSlotBytes(0, PACK_LOOKAHEAD, input);

  while (gFlash_Handler3->bucketBySlot[gFlash_Handler3->pos] != -1) {
    LzPack_FindWindowMatch(gFlash_Handler3->pos);
    if (defer == 0) {
      len = gFlash_Handler3->matchLen;
      if (gFlash_Handler3->matchLen > 1) {
        saveOffset = gFlash_Handler3->matchOffset;
        saveLen = len;
        LzPack_FindWindowMatch((gFlash_Handler3->pos + 1) &
                                   PACK_WINDOW_MASK);
        if (gFlash_Handler3->matchLen > 2) {
          reach[0] = gFlash_Handler3->matchLen + 1;
          LzPack_FindWindowMatch((gFlash_Handler3->pos + len) &
                                     PACK_WINDOW_MASK);
          reach[1] = gFlash_Handler3->matchLen + len;
          if (reach[0] >= reach[1]) {
            saveLen = 1;
            defer = 1;
          }
        }
        gFlash_Handler3->matchOffset = saveOffset;
        gFlash_Handler3->matchLen = saveLen;
      }
    }

    if (gFlash_Handler3->matchLen > 1) {
      defer = 0;
      gFlash_Handler3->outBuf[0] |= gFlash_Handler3->flagMask;
      if (gFlash_Handler3->matchLen <= 16) {
        code = ((gFlash_Handler3->matchOffset << 4) & ~0xfff) |
               (gFlash_Handler3->matchOffset & 0xff) |
               (((gFlash_Handler3->matchLen - 1) << 8) & 0xf00);
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = (u16)code >> 8;
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = code;
      } else {
        code = ((gFlash_Handler3->matchOffset << 4) & ~0xfff) |
               (gFlash_Handler3->matchOffset & 0xff);
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = (u16)code >> 8;
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = code;
        gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] =
            gFlash_Handler3->matchLen - 17;
      }
    } else {
      gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] =
          gFlash_Handler3->bucketBySlot[gFlash_Handler3->pos];
      gFlash_Handler3->matchLen = 1;
    }

    LzPack_ConsumeSlotBytes(gFlash_Handler3->pos + PACK_LOOKAHEAD,
                                gFlash_Handler3->matchLen, input);
    gFlash_Handler3->pos =
        (gFlash_Handler3->pos + gFlash_Handler3->matchLen) & PACK_WINDOW_MASK;
    gFlash_Handler3->flagMask >>= 1;
    if (gFlash_Handler3->flagMask == 0) {
      LzPack_CopyBufferedBytes(dst);
      gFlash_Handler3->flagMask = 0x80;
      gFlash_Handler3->outBuf[0] = 0;
      gFlash_Handler3->outCnt = 1;
    }
  }

  gFlash_Handler3->outBuf[0] |= gFlash_Handler3->flagMask;
  gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = 0;
  gFlash_Handler3->outBuf[gFlash_Handler3->outCnt++] = 0;
  LzPack_CopyBufferedBytes(dst);
  return gFlash_Handler3->outTotal;
}
