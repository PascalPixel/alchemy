#include "types.h"

extern s32 Data_0200b69c;
extern s32 Data_0200b698;
extern u8 Data_0200b5ec[];

void Func_02005326(s32);
void Func_0200512e(u8 *, u8 *);

s32 Func_020026cc(u8 *record)
{
    u8 *data;
    if (Data_0200b69c != 0) {
        if ((u32)(*(s32 *)(record + 8) + 0xffc4ffff) <= 0x0051fffe) {
            if (*(s32 *)(record + 16) > 0x00d30000) {
                if (*(s32 *)(record + 16) <= 0x0100ffff) {
                    goto inside;
                }
            }
        }
        if ((u32)(*(s32 *)(record + 8) + 0xffbaffff) <= 0x0034fffe) {
            if (*(s32 *)(record + 16) > 0x00c20000) {
                if (*(s32 *)(record + 16) <= 0x0114ffff) {
                    goto inside;
                }
            }
        }
    } else {
        if ((u32)(*(s32 *)(record + 8) + 0xffc4ffff) <= 0x0033fffe) {
            if (*(s32 *)(record + 16) > 0x00c20000) {
                if (*(s32 *)(record + 16) < 0x00e60000) {
                    goto inside;
                }
            }
        }
        if ((u32)(*(s32 *)(record + 8) + 0xff90ffff) <= 0x001dfffe) {
            if (*(s32 *)(record + 16) > 0x00d80000) {
                if (*(s32 *)(record + 16) < 0x00fa0000) {
                    goto inside;
                }
            }
        }
        if ((u32)(*(s32 *)(record + 8) + 0xffb1ffff) <= 0x002bfffe) {
            if (*(s32 *)(record + 16) > 0x00f10000) {
                if (*(s32 *)(record + 16) <= 0x0114ffff) {
                    goto inside;
                }
            }
        }
    }
    return 0;

inside:
    Func_02005326(106);
    data = Data_0200b5ec;
    Func_0200512e(record, data);
    Data_0200b698 = 1;
    return 0;
}
#include "resource_38f.h"
