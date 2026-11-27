#ifndef ALCHEMY_BUP_H
#define ALCHEMY_BUP_H

// The save state: sector headers and file summaries.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "sdk/flash.h"
#include "system.h"
#include "window/screens.h"
#include "window/window.h"

// The save files; file n is parts n and n + BUP_FILE_COUNT.
#define BUP_FILE_COUNT 3

// The header of a save sector in flash: its signature, the part it holds, its checksum and its
// counter.
typedef struct BUP_SECTOR_HEADER
{
    Uint8  signature[7]; // `CAMELOT`, unterminated
    // File n's parts are n and n + BUP_FILE_COUNT; BUP_PART_NONE (system/system_bup.c) marks a free
    // sector.
    Uint8  partId;
    Uint16 checksum; // low half of the payload's checksum
    Uint16 count;    // the highest is the current copy; restarts after 65000
    Uint8  reserved[4];
} BUP_SECTOR_HEADER;

// A save file's summary for the file list: the leader's name, level and class, the area, play time,
// coins, Djinn and party.
typedef struct BUP_FILE_SUMMARY
{
    Uint8  slotHeader[16]; // room for the part's BUP_SECTOR_HEADER
    Uint8  name[12];       // the leader's
    Uint8  level;          // the leader's; 0 for an empty file
    Uint8  classId;        // the leader's
    Uint16 area;           // area name number (fdAreaGetNameIndex)
    Uint32 playTime;       // ticks
    Uint32 coins;
    Sint8  djinn[4];       // the party's Djinn of each element
    Sint8  party[5];       // up to four party members, ended by -1
    // Copied from MAIN_ROUTINE_STATE.sendFlag, which saving at the end of the game sets; the file
    // list for cleared data lists a file without it as not cleared.
    Sint8  sendFlag;
    Sint8  djinnCount; // how many Djinn flags (FLAG_DJINN_FIRST on) are set
    Sint8  childhood;  // FLAG_CHILDHOOD is set; remaps the party's sprites
    // bits: 15..8 windowBrightness | 7..0 windowHue
    Uint16 windowHue        : 8; // MAIN_ROUTINE_STATE.windowHue
    Uint16 windowBrightness : 8; // MAIN_ROUTINE_STATE.windowBrightness
    // Low half of MAIN_ROUTINE_STATE.buildStamp; a file below the current stamp is listed as from
    // an older build.
    Uint16 buildStamp;
    // Filled on reading: the play time from the file's second part. A file whose playTime differs
    // is listed as corrupted.
    Uint32 secondPartPlayTime;
    Uint32 checksum; // the sum of the 968 words from GameFlagBytes
} BUP_FILE_SUMMARY;

// The save state: which of the 16 flash sectors hold a part, their part ids and counters, one
// sector's buffer and the three files' summaries.
typedef struct BUP_STATE
{
    Uint8  occupied[16]; // 1 for a sector holding the current copy of a part
    Uint8  partId[16];   // each sector's part; 16 for none
    Uint16 count[16];    // each sector's counter

    union
    {
        struct
        {
            BUP_SECTOR_HEADER header;
            Uint8             payload[4080];
        } part;

        Uint8 bytes[4096];
    } slot; // one sector, read from or written to flash

            // Each file's summary, read from its first part.
    BUP_FILE_SUMMARY summary[BUP_FILE_COUNT];
} BUP_STATE;

STATIC_ASSERT(sizeof(BUP_FILE_SUMMARY) == 64);
STATIC_ASSERT(offsetof(BUP_FILE_SUMMARY, level) == 28);
STATIC_ASSERT(offsetof(BUP_FILE_SUMMARY, sendFlag) == 49);
STATIC_ASSERT(offsetof(BUP_FILE_SUMMARY, checksum) == 60);
STATIC_ASSERT(offsetof(BUP_STATE, slot) == 64);
STATIC_ASSERT(offsetof(BUP_STATE, summary) == 4160);

STATIC_ASSERT(sizeof(BUP_SECTOR_HEADER) == 16);
STATIC_ASSERT(sizeof(BUP_STATE) == 4352);
STATIC_ASSERT(offsetof(BUP_STATE, partId) == 16);
STATIC_ASSERT(offsetof(BUP_STATE, count) == 32);
STATIC_ASSERT(offsetof(BUP_FILE_SUMMARY, djinnCount) == 50);
STATIC_ASSERT(offsetof(BUP_FILE_SUMMARY, secondPartPlayTime) == 56);

// The save state, heap slot HEAP_SLOT_BUP under its own name; null between save operations.
#define BupStateWork               (*(BUP_STATE **)&HeapSlots[HEAP_SLOT_BUP])

// The word, counted from SceneLocalState (system/ewram.s), which opens a file's second part, that
// holds the second copy of the play time (mnSaveBuildFileSummary): InventorySnapshotWork's last,
// past SceneLocalState.
#define SECOND_PART_PLAY_TIME_WORD 64

// Starts a DMA 3 transfer; control_ is the unit count with the DMA_ flags << 16.
#define START_DMA(source_, destination_, control_)                                                 \
    syDMAXCopy((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

// Waits until a START_DMA transfer is done.
#define WAIT_DMA() syDMAWait(DMA3)

// Nonzero offers Send on the File Screen: mnSaveScanFileSummaries sets it when a file has its send
// flag set and R and Left are held.
extern Sint16 TitleSendOptionEnabled;

#endif
