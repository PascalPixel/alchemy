#include "types.h"

#define SceneData_GetTableA638 Func_02000088
#define SceneData_GetTableA920 Func_02000094
#define SceneData_SelectRecordByScene21 Func_0200009c
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

/* Scene selector and table getters for resource_38d. The two records live in
 * the overlay image, which is writable, so the stores below land in it. */
extern s16 Data_02000240[];     /* Shared scene workspace, not overlay data. */
extern u8 Value_00000021;
extern u8 Data_0200a9b4[];      /* In-image record. */
extern u8 Data_0200a99c[];      /* In-image record. */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_02002536();           /* Record hand-off. */

s32 Func_0200250c();            /* Story-flag test. */

/*
 * Returns the in-image table at 0x0200a638. The eight-byte owner at
 * 0x02000088 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTableA638(void)
{
    return (u8 *)0x0200a638;
}

/*
 * Returns the in-image table at 0x0200a920. The eight-byte owner at
 * 0x02000094 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTableA920(void)
{
    return (u8 *)0x0200a920;
}

/*
 * Picks one of two scene records by the current scene selector. For scene
 * 0x21 the record is handed to Func_02002536 and, when story flag 0x84e is
 * set, four of its bytes are stamped; otherwise the other record is returned
 * untouched. The four offsets are one field at a 24-byte stride over four
 * entries, left literal because the entry layout is not established.
 */
s32 SceneData_SelectRecordByScene21(void)
{
    u8 *p;

    /* A signed halfword read. */
    if (Data_02000240[224] == (s32)&Value_00000021) {
        p = Data_0200a9b4;
        Func_02002536(p);

        if (Func_0200250c(0x84e) != 0) {
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
