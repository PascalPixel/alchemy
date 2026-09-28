/* Draft of resource_38d 0x0200809c (SceneData_SelectRecordByScene21), built
 * with games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/KYUDEN.H. Remaining
 * difference: the ROM loads the scene number 0x21 from the literal pool to
 * compare it with the game state's scene, as a link-time value would; a C
 * constant compares against an immediate. The two actor tables and the
 * record hand-off also need names. The listing keeps these rows. */
#include "KYUDEN.H"

extern u8 Value_00000021;
extern u8 Data_0200a9b4[];
extern u8 Data_0200a99c[];
void Func_02002536();

struct SceneRecord {
    u8 unk_000[166];
    u8 field_166;
    u8 unk_167[23];
    u8 field_190;
    u8 unk_191[23];
    u8 field_214;
    u8 unk_215[23];
    u8 field_238;
};

/* Taking a reward changes four entries in this palace placement table. */
s32 SceneData_SelectRecordByScene21(void)
{
    u8 *p;

    /* A signed halfword read. */
    if (gGameState.scene == (s32)&Value_00000021) {
        p = Data_0200a9b4;
        Func_02002536(p);

        if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
            struct SceneRecord *rec = (struct SceneRecord *)p;

            rec->field_166 = 2;
            rec->field_190 = 0;
            rec->field_214 = 3;
            rec->field_238 = 1;
        }

        return (s32)p;
    }
    return (s32)Data_0200a99c;
}
