#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_08077148(void);

extern u8 Data_02000240[];

s32 LinkLobby_Func02000f30(s32 id)
{
    s32 count;
    s32 max;
    s32 i;

    count = Main_08077148();
    max = 3;
    if (Engine_GameFlagIsSet(0x172) == 0) {
        max = 4;
    }
    if (count > max) {
        count = max;
    }
    for (i = 0; i < count; i++) {
        if (Data_02000240[0x1f8 + i] == 0xff) {
            return 0;
        }
        if (Data_02000240[0x1f8 + i] == id) {
            return 1;
        }
    }
    return 0;
}
