/* NONMATCHING: candidate 96 of 100 bytes, 49 differing halfwords,
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
 * before the blend-register store, not merely change the snapshot's type. */
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
