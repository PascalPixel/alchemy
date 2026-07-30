typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void *Func_080048b0(s32, s32);
s32 Func_08002dd8(s32);
u32 Func_08007304(void *);
void Func_08019bac(void *, s32);

/*
 * Decode a text stream into the engine's halfword token buffer. Most tokens
 * occupy one slot; the extended control forms consume an additional one or
 * two payload bytes. A temporary text workspace is installed when the shared
 * workspace is not already active.
 */
s32 Func_080196c4(s32 text_id, u16 *output, s32 slots_left) {
    u8 parser[12];
    volatile u32 *dma3;
    void *workspace;
    u32 token;
    s32 owns_workspace;

    owns_workspace = (*(s32 *)0x03001F18 == 0);
    if (owns_workspace) {
        workspace = Func_080048b0(0x32, 0x140);
        dma3 = (volatile u32 *)0x040000D4;
        dma3[0] = 0x08015430;
        dma3[1] = (u32)workspace;
        dma3[2] = 0x84000050;
    }

    Func_08019bac(parser, text_id);
    for (;;) {
        token = Func_08007304(parser);
        if (token == 0) {
            break;
        }

        if (token == 0xE) {
            slots_left -= 3;
            if (slots_left <= 0) {
                break;
            }
            *output++ = (u16)token;
            *output++ = (u16)(Func_08007304(parser) - 1);
            *output++ = (u16)(Func_08007304(parser) - 1);
        } else if (token == 0xF || (token >= 8 && token <= 0xC)) {
            slots_left -= 1;
            if (slots_left <= 0) {
                break;
            }
            *output++ = (u16)token;
            *output++ = (u16)(Func_08007304(parser) - 1);
        } else {
            slots_left -= 1;
            if (slots_left <= 0) {
                break;
            }
            *output++ = (u16)token;
        }
    }

    *output = 0;
    if (owns_workspace) {
        return Func_08002dd8(0x32);
    }
    return (s32)token;
}
