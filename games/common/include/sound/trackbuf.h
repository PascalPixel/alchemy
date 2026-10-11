#ifndef ALCHEMY_TRACKBUF_H
#define ALCHEMY_TRACKBUF_H

#include "edition.h"
#include "types.h"
#include "audio_engine_symbols.h"

struct AudioSlotNode
{
	struct AudioSlotNode *next; // 同じ値の次のノード
	struct AudioSlotNode *prev; // 前のノード（先頭ならバケツの欄）
	s32					  slot;
};

struct AudioTrackSlotWork
{
	struct AudioSlotNode  nodes[0x400];
	struct AudioSlotNode *bucket[0x100];
	s32					  flag_mask;
	s32					  bucket_by_slot[0x400];
	s32					  out_cnt;
	u8					  out_buf[0x24];
	s32					  match_ofs;
	s32					  match_len;
	s32					  pos;
	u32					  input_cursor;
	s32					  out_total;
	u32					  input_limit;
};

typedef char AudioSlotNode_Size[sizeof(struct AudioSlotNode) == 12 ? 1 : -1];
typedef char AudioTrackSlotWork_Size[sizeof(struct AudioTrackSlotWork) == 0x4444 ? 1 : -1];
typedef char
	AudioTrackSlotWork_Buckets[(u32) & ((struct AudioTrackSlotWork *)0)->bucket == 0x3000 ? 1 : -1];
typedef char AudioTrackSlotWork_InputCursor
	[(u32) & ((struct AudioTrackSlotWork *)0)->input_cursor == 0x4438 ? 1 : -1];

#if GAME_TLA
void AudioTrack_ResetSlotBuckets(void);
void AudioTrack_InsertSlotNode(s32 slot);
void AudioTrack_RemoveSlotNode(s32 slot);
void AudioTrack_ConsumeSlotBytes(s32 start, s32 count, const u8 *input);
void AudioTrack_CopyBufferedBytes(u8 *destination);
s32	 AudioTrack_PackStream(const u8 *input, u8 *destination, s32 size);
#endif

#endif
