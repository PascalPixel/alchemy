/* The clear screen's two button codes. Each new press must match the next
 * entry of its sequence, a wrong press starts it over, and reaching the
 * zero entry unlocks the code with a chime. A held press counts once. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* FAKEMATCH: volatile keys make each test read them again, as the game does. */
extern volatile u32 gKeysHeld;
extern s16 Clear_CodeUnlocked;
extern s16 Clear_ExtraCodeUnlocked;
extern s16 Clear_CodeProgress;
extern s16 Clear_ExtraCodeProgress;
extern s16 Clear_CodeHeld;
extern s16 Clear_ExtraCodeHeld;
extern u16 Clear_CodeSequence[];
extern u16 Clear_ExtraCodeSequence[];

void Clear_CheckButtonCodes(void)
{
    if (Clear_CodeUnlocked == 0) {
        if (Clear_CodeHeld != 0) {
            if (gKeysHeld == 0)
                Clear_CodeHeld = 0;
        } else if (gKeysHeld != 0) {
            if (gKeysHeld == Clear_CodeSequence[Clear_CodeProgress]) {
                Clear_CodeProgress++;
                Clear_CodeHeld = 1;
                if (Clear_CodeSequence[Clear_CodeProgress] == 0) {
                    Clear_CodeUnlocked = 1;
                    Engine_AudioPlayCue(110);
                }
            } else {
                Clear_CodeProgress = 0;
            }
        }
    }
    if (Clear_ExtraCodeUnlocked == 0) {
        if (Clear_ExtraCodeHeld != 0) {
            if (gKeysHeld == 0)
                Clear_ExtraCodeHeld = 0;
        } else if (gKeysHeld != 0) {
            if (gKeysHeld == Clear_ExtraCodeSequence[Clear_ExtraCodeProgress]) {
                Clear_ExtraCodeProgress++;
                Clear_ExtraCodeHeld = 1;
                if (Clear_ExtraCodeSequence[Clear_ExtraCodeProgress] == 0) {
                    Clear_ExtraCodeUnlocked = 1;
                    Engine_AudioPlayCue(110);
                }
            } else {
                Clear_ExtraCodeProgress = 0;
            }
        }
    }
}
