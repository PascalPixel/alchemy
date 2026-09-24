#include "TYPES.H"

u8 *Engine_ActorGet();
void Main_08009088();

struct Half {
    u16 v;
};

/* The zero is a one-halfword struct; the child-byte OR goes through a word temporary with 255 held in a variable, so it stays ldrb/orrs. */
void TorebiIzumi_Func02000dd0(s32 id, s32 *pos, s32 dir, s32 palette, s32 value)
{
    u8 *actor = Engine_ActorGet(id);
    u8 *sprite;
    u32 n;

    if (actor != 0) {
        struct Half zero;

        *(s32 *)(actor + 8) = *pos++;
        *(s32 *)(actor + 12) = *pos++;
        *(s32 *)(actor + 16) = *pos;
        *(u16 *)(actor + 6) = dir;
        zero.v = 0;
        actor[85] = zero.v;
        (*(u8 **)(actor + 80))[38] = zero.v;
        Main_08009088(actor, value);
    }
    sprite = *(u8 **)(actor + 80);
    n = sprite[39];
    if (n != 0) {
        u32 mask = 255;
        u8 **list = (u8 **)(sprite + 40);
        u32 left = n;

        do {
            u8 *cell = *list++;

            if (cell[5] != palette) {
                cell[5] = palette;
                {
                    u32 v = cell[22];

                    v |= mask;
                    cell[22] = v;
                }
            }
        } while (--left != 0);
    }
}
