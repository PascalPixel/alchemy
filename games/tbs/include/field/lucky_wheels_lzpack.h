#ifndef ALCHEMY_LUCKY_WHEELS_LZPACK_H
#define ALCHEMY_LUCKY_WHEELS_LZPACK_H

/* @file */
/* The LZ packer's state: hash chains over a 1024-byte window, one chain per */
/* byte value, and the output it is building. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"

/* A position of the LZ packer's window in the chain of the byte value it */
/* holds. */
typedef struct LZ_PACK_NODE {
	struct LZ_PACK_NODE *next; /* Next node with the same value. */
	struct LZ_PACK_NODE
		*prev; /* Previous node, or the bucket field at the head. */
	s32 slot;  /* its window position, 0 to 1023 */
} LZ_PACK_NODE;

/* The LZ packer's state: a node per window position, a chain head per byte */
/* value, the byte at each position, the best match and the output. */
typedef struct LZ_PACK_STATE {
	LZ_PACK_NODE nodes[1024];  /* one per window position */
	LZ_PACK_NODE *bucket[256]; /* each byte value's chain, or null */
	s32 flgMask;			   /* the flag bit of the next item, 0x80 first */
	s32 bucketBySlot[1024];	   /* each position's byte; -1 past the input */
	s32 outCnt;				   /* bytes of outBuf in use */
	/* The group being built: its flag byte, then up to eight items. */
	u8 outBuf[36];
	s32 matchOfs;	 /* the best match's distance back (fdLzPackFindWindowMatch)
					  */
	s32 matchLen;	 /* its length; 1 for none */
	s32 pos;		 /* the window position being packed */
	u32 inputCursor; /* input bytes read into the window */
	s32 outTotal;	 /* bytes written to the output */
	u32 inputLimit;	 /* the input's size */
} LZ_PACK_STATE;

STATIC_ASSERT(sizeof(LZ_PACK_NODE) == 12);
STATIC_ASSERT(sizeof(LZ_PACK_STATE) == 17476);
STATIC_ASSERT(offsetof(LZ_PACK_STATE, bucket) == 12288);
STATIC_ASSERT(offsetof(LZ_PACK_STATE, inputCursor) == 17464);

/* The packer has no cell of its own for its state pointer: it borrows the */
/* one of the SDK's flash library, which IdentifyFlash sets again before the */
/* flash code reads it. */
extern LZ_PACK_STATE *WaitForFlashWriteWork;
#define LzPackStateWork WaitForFlashWriteWork

#endif
