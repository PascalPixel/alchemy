#include "types.h"

u32 Func_08077300(void);
u8 *Func_08077008(s32);
s32 Func_0808a5d0(s16, s16);
s8 Func_08077290(s32);
void Func_08077158(u16 *);
s32 Func_080770c0(s32);

/*
 * Compose the compact game-state snapshot at 0x02000000. It captures the
 * current character, world state, party summary, settings, event counts, and
 * a checksum over the associated 968-word payload.
 */
void Func_0801f818(void)
{
    u8 *work = (u8 *)0x02000240;
    u8 *snapshot = (u8 *)0x02000000;
    u8 *character;
    u16 identifiers[14];
    u32 checksum = 0;
    u32 *payload;
    s32 count;
    s32 index;

    *(u32 *)(work + 0) = Func_08077300();
    *(u32 *)(work + 4) = *(u32 *)0x03001C9C;
    *(u32 *)0x02001100 = *(u32 *)(work + 4);
    work[0x22A] = *(u8 *)0x03001D08;

    character = Func_08077008(*(s32 *)(work + 0x1F4));
    for (index = 0; index < 12; index++) {
        snapshot[index] = character[index];
    }

    snapshot[12] = character[0xF];
    snapshot[13] = character[0x129];
    *(u16 *)(snapshot + 14) = (u16)Func_0808a5d0(
        *(s16 *)(work + 0x1C0), *(s16 *)(work + 0x1C2));
    *(u32 *)(snapshot + 16) = *(u32 *)(work + 4);
    *(u32 *)(snapshot + 20) = *(u32 *)(work + 0x10);

    snapshot[24] = (u8)Func_08077290(0);
    snapshot[25] = (u8)Func_08077290(1);
    snapshot[26] = (u8)Func_08077290(2);
    snapshot[27] = (u8)Func_08077290(3);

    count = 0;
    Func_08077158(identifiers);
    if (identifiers[0] != 0xFF) {
        do {
            snapshot[28 + count] = (u8)identifiers[count];
            count++;
        } while (count <= 3 && identifiers[count] != 0xFF);
    }
    snapshot[28 + count] = 0xFF;

    snapshot[36] = *(u8 *)0x02000445;
    snapshot[37] = *(u8 *)0x02000446;
    snapshot[33] = *(u8 *)0x0200044F;
    snapshot[34] = 0;
    for (index = 0x30; index <= 0x7F; index++) {
        if (Func_080770c0(index) != 0) {
            snapshot[34]++;
        }
    }
    snapshot[35] = Func_080770c0(0x20) != 0;
    *(u16 *)(snapshot + 38) = (u16)*(u32 *)(work + 0);

    payload = (u32 *)0x02000040;
    for (index = 0; index < 0x3C8; index++) {
        checksum += *payload++;
    }
    *(u32 *)(snapshot + 44) = checksum;
}
