/* Draft: the 244-byte Japanese/English prefix routine lacks the localized
 * eight-character limit and separating space. All four localized editions
 * use the 266-byte production branch; this attempt remains compilable. */
#include "TYPES.H"
#include "OWNER_STATE.H"

s32 Party_Check(void);
void SerialRuntime_WaitForTransferB(void);
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void Sys_Free(void *);
void *Runtime_BumpAllocateAlternatePool(s32);
void WaitFrames(s32);
void *Resource_FarCall005(s32);

extern char MsgEnemyLabel;

s32 UpdateNameEntries(void)
{
    u16 name_text[16];
    void *buffer;
    u8 *name_entry;
    s32 named_count;
    s32 index;
    s32 len;
    s32 i;

    buffer = Runtime_BumpAllocateAlternatePool(340);
    named_count = 0;
    index = 0;
    while (index <= 2) {
        name_entry = Owner_GetState(index + 128);
        if (Party_Check() == -1) {
            break;
        }
        SerialRuntime_WaitForTransferB();
        if (name_entry[298] != 0) {
            named_count += 1;
        }
        WaitFrames(2);
        Ui_AdjustValueWithoutLimitFar((s32)&MsgEnemyLabel, name_text);
        i = 0;
        if (name_text[i] != 0) {
            do {
                i += 1;
                if (i > 4) {
                    break;
                }
            } while (name_text[i] != 0);
        }
        len = i;
        for (i = 14; i >= len; i--) {
            name_entry[i] = name_entry[i - len];
        }
        for (i = 0; i < len; i++) {
            name_entry[i] = (u8)name_text[i];
        }
        name_entry[14] = 0;
        index += 1;
    }
    Sys_Free(buffer);
    buffer = Runtime_BumpAllocateAlternatePool(340);
    Resource_FarCall005(1);
    if (Party_Check() != -1) {
        SerialRuntime_WaitForTransferB();
        WaitFrames(2);
    }
    Sys_Free(buffer);
    return named_count;
}
