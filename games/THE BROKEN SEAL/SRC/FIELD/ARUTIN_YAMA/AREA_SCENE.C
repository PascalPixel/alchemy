#include "TYPES.H"
extern s16 Data_02000240_t[][1];

void ArutinYama_ApplyEntryState();
void FieldScene_RunScene3a4_02002310();
void FieldScene_RunScene3a4_02002428();
void FieldScene_RunScene3a4_02002490();
void FieldScene_RunScene3a4_020025c0();
void FieldScene_RunScene3a4_020026c0();
void Local_02002804();
void FieldScene_RunScene3a4_02002934();
void FieldScene_RunScene3a4_020029dc();


/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
extern u8 Data_0000004d[];
extern u8 Data_0000004f[];
extern u8 Data_00000050[];
extern u8 Data_00000051[];
extern u8 Data_00000052[];
extern u8 Data_00000053[];
extern u8 Data_00000055[];
extern u8 Data_00000056[];
extern u8 Data_00000057[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Altin Peak: mark the event state and run the entry scene of the area
 * variant stored at +0x1c0 of the game state. */
s32 ArutinYama_RunAreaScene(void)
{

    *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x204;
    if (Data_02000240_t[224][0] == (s32)Data_0000004d) {
        ArutinYama_ApplyEntryState();
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_0000004f) {
            FieldScene_RunScene3a4_02002310();
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_00000050) {
                FieldScene_RunScene3a4_02002428();
            } else {
                if (Data_02000240_t[224][0] == (s32)Data_00000051) {
                    FieldScene_RunScene3a4_02002490();
                } else {
                    if (Data_02000240_t[224][0] == (s32)Data_00000052) {
                        FieldScene_RunScene3a4_020025c0();
                    } else {
                        if (Data_02000240_t[224][0] == (s32)Data_00000053) {
                            FieldScene_RunScene3a4_020026c0();
                        } else {
                            if (Data_02000240_t[224][0] == (s32)Data_00000055) {
                                Local_02002804();
                            } else {
                                if (Data_02000240_t[224][0] == (s32)Data_00000056) {
                                    FieldScene_RunScene3a4_02002934();
                                } else {
                                    if (Data_02000240_t[224][0] == (s32)Data_00000057) {
                                        FieldScene_RunScene3a4_020029dc();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
