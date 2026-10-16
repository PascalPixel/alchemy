#ifndef ALCHEMY_TRACKBUF_H
#define ALCHEMY_TRACKBUF_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"
#include "audio_engine_symbols.h"

struct AudioSlotNode {
  struct AudioSlotNode *next; // 同じ値の次のノード
  struct AudioSlotNode *prev; // 前のノード（先頭ならバケツの欄）
  s32 slot;
};

struct AudioTrackSlotWork {
  struct AudioSlotNode nodes[0x400];
  struct AudioSlotNode *bucket[0x100];
  s32 flagMask;
  s32 bucketBySlot[0x400];
  s32 outCnt;
  u8 outBuf[0x24];
  s32 matchOffset;
  s32 matchLen;
  s32 pos;
  u32 inputCursor;
  s32 outTotal;
  u32 inputLimit;
};

LAYOUT_SIZE_GUARD(AudioSlotNode_Size, struct AudioSlotNode, 12);
LAYOUT_SIZE_GUARD(AudioTrackSlotWork_Size, struct AudioTrackSlotWork, 0x4444);
LAYOUT_OFFSET_GUARD(AudioTrackSlotWork_Buckets, struct AudioTrackSlotWork,
                    bucket, 0x3000);
LAYOUT_OFFSET_GUARD(AudioTrackSlotWork_InputCursor, struct AudioTrackSlotWork,
                    inputCursor, 0x4438);

#if GAME_TLA
void AudioTrack_ResetSlotBuckets(void);
void AudioTrack_InsertSlotNode(s32 slot);
void AudioTrack_RemoveSlotNode(s32 slot);
void AudioTrack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input);
void AudioTrack_CopyBufferedBytes(u8 *destination);
s32 AudioTrack_PackStream(const u8 *input, u8 *destination, s32 size);
#endif

#endif
