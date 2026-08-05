#include "types.h"

/*
 * resource_3bc state-machine slot 3, complete 352-byte owner and pool.  The
 * workspace halfword at +0x182 is the cue polled during the bounded wait and
 * reset after the successful sequence.
 */

extern s16 Data_02000240[];


























extern void Func_02004836(void);
extern void Func_020069b0(void);
extern s32 Func_02004a4c(s32 actor, s32 slot);
extern void Func_02006a7a(s32 message);
extern void Func_02002156(void);
extern void Func_02006ad2(s32 x, s32 z);
extern void Func_02006aec(s32 x, s32 y, s32 z, s32 mode);
extern void Func_02006af8(void);
extern void Func_020069ee(s32 frames);
extern void Func_02006abe(s32 actor, s32 mode);
extern void Func_020021b6(void);
extern void Func_02006a00(s32 frames);
extern void Func_02006ad0(s32 actor, s32 mode);
extern void Func_0200565c(s32 actor, s32 x, s32 z);
extern void Func_02006af6(s32 actor, s32 speed, s32 frames);
extern void Func_020021f2(void);
extern void Func_02006a60(s32 actor, s32 x, s32 z);
extern void Func_02005b84(s32 actor, s32 x, s32 z);
extern void Func_020068a0(s32 frames);
extern void Func_02002ba0(void);
extern void Func_02006b4c(s32 actor, s32 speed, s32 frames);
extern void Func_02006b66(s32 actor, s32 cue, s32 frames);
extern void Func_02006b4e(s32 actor, s32 mode);
extern void Func_020058b0(s32 actor);
extern void Func_02006b8c(s32 actor, s32 mode);
extern void Func_02004bf4(s32 actor, s32 slot);
extern void Func_02006b6a(s32 message);
extern void Func_02006b82(s32 actor, s32 mode);
extern void Func_02004c78(s32 state, s32 actor, s32 slot);
extern void Func_02006ad8(void);
void Func_02001f90(s32 actor)
{
    s32 state;
    u8 *workspace;

    if (Data_02000240[225] == 2) {
        Func_02004836();
        return;
    }

    Func_020069b0();
    state = Func_02004a4c(actor, 3);

    if (state == 0) {
        workspace = *(u8 **)0x03001ebc;
        Func_02006a7a(0x20bb);
        Func_02002156();
        Func_02006ad2(0x30000, 0x6000);
        Func_02006aec(0x2680000, -1, 0xb80000, 1);
        Func_02006af8();
        Func_020069ee(30);
        Func_02006abe(actor, 0);
        Func_020021b6();
        Func_02006a00(60);
        Func_02006ad0(actor, 0);
        Func_0200565c(0, 0x1f8, 0xc8);
        Func_02006af6(0, 0, 0);
        Func_020021f2();
        Func_02006a60(0, 0x18000, 0xc000);
        Func_02005b84(0, 0x2a8, 0xc8);

        while (*(s16 *)(workspace + 0x182) != 5) {
                break;
            Func_020068a0(1);
            if (state > 239)
            state++;
        }

        Func_02002ba0();
        Func_02006b4c(0, 0xc000, 20);
        Func_02006b66(0, 0x103, 60);
        Func_02006b4e(actor, 0);
        Func_020058b0(0);
        Func_02006b8c(0, 0);
        Func_02004bf4(actor, 3);
        *(s16 *)(workspace + 0x182) = 0;
    } else if (state == 1) {
        Func_02006b6a(0x20ba);
        Func_02006b82(actor, 0);
    }

    Func_02004c78(state, actor, 3);
    Func_02006ad8();
}
