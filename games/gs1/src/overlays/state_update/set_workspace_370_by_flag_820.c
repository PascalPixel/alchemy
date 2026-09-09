#include "types.h"

/* 0x03001ebc is the overlay's workspace pointer. */
extern u8 *Data_03001ebc;

/*
 * Imports. Each alias names the call word its site encodes, not a runtime
 * address. Only those used for their return value are typed, and the
 * declarations are old-style because one name is reached with different
 * argument counts.
 */

extern void Func_02004294();
extern s32 Func_0200427c();
extern void Func_02004270();
extern void Func_0200427a();
extern s32 Func_02004290();
extern void Func_020042da();
/*
 * Branch on flag 0x820 -- resource_39a. One arm sets a record flag; the other
 * sets a different flag and writes workspace halfword 370. Nothing is
 * returned, and the owner extends through the three pool words that follow
 * the epilogue.
 */
void SceneState_SetWorkspace370ByFlag820(void)
{
    Func_02004294();
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    if (Func_0200427c((s32)0x820) != 0) {
        Func_02004270((s32)0x17e5, 1);
    } else {
        Func_0200427a((s32)0x17e4, 1);
        if (Func_02004290((s32)0xe6) != -1) {
            u8 *workspace = Data_03001ebc;

            /* movs r1,#0xb9 / lsls r1,#1 gives the byte offset 370. */
            /*
             * The store goes through a pointer local and an s32 value local,
             * in that order. Storing the literal directly builds the constant
             * in HImode and loads it from the literal pool, costing a pool
             * word; splitting the address out first also fixes which register
             * holds it.
             */
            {
                u16 *slot = (u16 *)(workspace + 370);
                s32 one = 1;

                *slot = (u16)one;
            }
        }
    }
    Func_020042da();
}
