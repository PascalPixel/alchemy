typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

s32 Func_08004358(u32 value, s32 setting) {
    s32 result = -1;
    u8 *entry = (u8 *)0x03001A20;

    {
        volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
        u32 savedInterruptMaster = *interruptMaster;

        *interruptMaster = (u16)(u32)interruptMaster;
        {
            s32 index = 0;

            if (*(u32 *)entry == value) {
                entry[6] = setting;
                result = 0;
            } else {
                do {
                    index++;
                    entry += 8;
                    if (index > 19) {
                        break;
                    }
                } while (*(u32 *)entry != value);
                if (index <= 19) {
                    entry[6] = setting;
                    result = index;
                }
            }
        }
        {
            volatile u16 *restoreInterruptMaster = (volatile u16 *)0x04000208;
            *restoreInterruptMaster = savedInterruptMaster;
        }
    }
    return result;
}
