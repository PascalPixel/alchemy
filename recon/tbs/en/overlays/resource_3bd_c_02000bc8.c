/* 2026-09-27 babi-land final audit: complete extent and all four pool
 * words rescored; candidate 96/reference 100, 49 differing halfwords /
 * 18 normalized edits. The actor call sequence is exact. No new source
 * fact changes the independent unsigned/signed-read residual; the recorded
 * snapshot, volatile-read and publication axes remain closed. DONE +0,
 * alignment +0. Keep this one canonical draft as C not yet written.
 * NONMATCHING: candidate 96 of 100 bytes, 49 differing halfwords,
 * 18 aligned edits (2026-09-27). Complete extent [02000bc8,02000c2c)
 * includes its four-word pool. The reference loads the scene twice:
 * unsigned into r1 for the second test, then signed for the first. This
 * draft shares one signed load and loses the later sign-extension pair.
 * A u16 counts[] view still folds into the first compare's ldrsh; an older
 * volatile-read trial kept two loads but folded base+offset into one pool
 * word and sign-extended too early. Do not repeat those axes.
 * The shared halfword-record pattern was tested as a scene snapshot:
 * struct { u16 value; } map; map.value = rows.halves[224][0]. Its complete
 * candidate is byte-identical to this baseline (cmp checked); it does not
 * preserve the independent unsigned read. That type axis is now stopped.
 * Bindings and scene-row ownership agree with exact ARUTAMIRA_DOU/
 * ENTRY_STATE.C. Stable unit arutamira-room-visuals records the complete
 * owner without awarding credit. A next model must explain the two reads
 * before the blend-register store, not merely change the snapshot's type.
 * 2026-09-27: transferred Suhalla's one-pass publication boundary around
 * the complete first comparison/blend-store phase. Prediction: keep the
 * independent unsigned snapshot before the signed scene read. Full diff
 * and binary comparison show identical 96-byte output (49 halfwords,
 * 18 edits); this boundary does not prevent the earlier load merging.
 * Retain the canonical source, stop the publication-block axis. DONE +0.
 * Astra 2026-09-27: transfer the ship counter's word-based signed 16-bit
 * bitfield as records[112].scene for the comparison, keeping the unsigned
 * snapshot. The complete result remains 96/100, 49 halfwords / 18 edits:
 * the two reads still merge. Making only the snapshot a volatile u16 read
 * keeps both loads, but folds the scene offset into the pool address and
 * sign-extends the snapshot before the first comparison (same score).
 * Neither recovers the reference's address and delayed conversion; retain
 * the original body. No compiler, binding or credit change. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows gGameStateRows;
extern u8 Data_00000092[], Data_00000097[];

void ArutamiraDou_ApplyRoomVisuals(void)
{
    u16 map = gGameStateRows.halves[224][0];

    if (gGameStateRows.halves[224][0] == (s32)Data_00000092)
    {
        s32 alpha = 0x1000;

        *(volatile u16 *)0x04000052 = alpha;
    }
    if ((s16)map == (s32)Data_00000097) {
        Engine_ActorSetChildValue(16, 1);
        Engine_ActorSetChildValue(17, 4);
        Engine_ActorSetChildValue(18, 11);
        Engine_ActorSetChildValue(19, 2);
        Engine_ActorSetChildValue(20, 3);
    }
}
