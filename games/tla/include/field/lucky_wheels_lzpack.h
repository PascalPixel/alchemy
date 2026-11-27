#ifndef ALCHEMY_LUCKY_WHEELS_LZPACK_H
#define ALCHEMY_LUCKY_WHEELS_LZPACK_H

// The LZ packer's state: hash chains over a 1024-byte window, one chain per byte value, and the
// output it is building.

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "sdk/flash_data.h"

// A position of the LZ packer's window in the chain of the byte value it holds.
typedef struct LZ_PACK_NODE
{
	struct LZ_PACK_NODE *next; // Next node with the same value.
	// Previous node, or the bucket field at the head.
	struct LZ_PACK_NODE *prev;
	Sint32               slot; // its window position, 0 to 1023
} LZ_PACK_NODE;

// The LZ packer's state: a node per window position, a chain head per byte value, the byte at each
// position, the best match and the output.
typedef struct LZ_PACK_STATE
{
	LZ_PACK_NODE  nodes[1024];        // one per window position
	LZ_PACK_NODE *bucket[256];        // each byte value's chain, or null
	Sint32        flagMask;           // the flag bit of the next item, 0x80 first
	Sint32        bucketBySlot[1024]; // each position's byte; -1 past the input
	Sint32        outCount;           // bytes of outBuf in use
	// The group being built: its flag byte, then up to eight items.
	Uint8         outBuf[36];
	// the best match's distance back (fdLZPackFindWindowMatch)
	Sint32        matchOffset;
	Sint32        matchLen;    // its length; 1 for none
	Sint32        pos;         // the window position being packed
	Uint32        inputCursor; // input bytes read into the window
	Sint32        outTotal;    // bytes written to the output
	Uint32        inputLimit;  // the input's size
} LZ_PACK_STATE;

STATIC_ASSERT(sizeof(LZ_PACK_NODE) == 12);
STATIC_ASSERT(sizeof(LZ_PACK_STATE) == 17476);
STATIC_ASSERT(offsetof(LZ_PACK_STATE, bucket) == 12288);
STATIC_ASSERT(offsetof(LZ_PACK_STATE, inputCursor) == 17464);

// The packer has no metatile of its own for its state pointer: it borrows the SDK's flash library's
// WaitForFlashWriteWork (sdk/flash_data.h), which IdentifyFlash sets again before the flash code
// reads it.
#define LzPackStateWork (*(LZ_PACK_STATE **)&WaitForFlashWriteWork)

void   fdLZPackResetSlotBuckets(void);
void   fdLZPackInsertSlotNode(Sint32 slot);
void   fdLZPackRemoveSlotNode(Sint32 slot);
void   fdLZPackConsumeSlotBytes(Sint32 start, Sint32 count, const Uint8 *input);
void   fdLZPackCopyBufferedBytes(Uint8 *dest);
Sint32 fdLZPackPackStream(const Uint8 *input, Uint8 *dest, Sint32 size);

#endif
