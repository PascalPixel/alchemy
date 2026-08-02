typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_0808a080(s32 actorId);
extern s32 Func_080091d8(u8 *actor, const s32 *position);
extern s32 Func_080091b0(s32 layer, s32 x, s32 z);
extern void Func_080f9010(s32 soundId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_02000d78(u8 *actor);
extern void Func_02000da8(u8 *actor, s32 position);
extern void Func_02000fcc(s32 layer, s32 x, s32 z, void *destination);
extern void Func_0200101c(s32 layer, s32 x, s32 z, u8 *tile);
extern s32 Func_02001074(s32 actorId, s32 mode);
extern void Func_02001174(s32 actorId);
extern void Func_02001be8(void);

/*
 * Complete 592-byte four-actor terrain/state refresh.  The owner begins at
 * 0x02001d84, returns at 0x02001fbc, and owns the alignment and five-word
 * pool through 0x02001fd3.
 *
 * Each actor in slots 8..11 is reconciled with its cached whole-cell
 * position.  A moved or active actor is collision-probed, its two terrain
 * layers are refreshed, special markers 50/51 trigger their distinct sound
 * and reset paths, and the cache is updated.  The final nested scan transfers
 * the slot flag when two actors occupy the same X/Z cell and one is higher.
 * After all four records are current, Func_02001be8 restores depth order.
 * The 30 static call sites resolve to 14 targets and are kept in machine
 * order, including the intentionally repeated marker queries.
 */

void Func_02001d84(void)
{
    s32 *records = (s32 *)0x0200b6d0;
    s32 *probe = (s32 *)0x0200b720;
    s32 actorId;

    for (actorId = 8; actorId <= 11; actorId++) {
        s32 recordIndex = actorId - 8;
        s32 *record = records + recordIndex * 5;
        u8 *actor = Func_0808a080(actorId);
        u8 tile[4];
        u8 *state;

        actor[0x22] = 2;
        if ((*(s32 *)(actor + 8) >> 20) == record[0] &&
            (*(s32 *)(actor + 16) >> 20) == record[2] &&
            *(s32 *)(actor + 40) == 0) {
            continue;
        }

        probe[0] = *(s32 *)(actor + 8);
        probe[1] = *(s32 *)(actor + 12);
        probe[2] = *(s32 *)(actor + 16);
        state = actor + 0x55;
        if (Func_080091d8(actor, probe) == -1)
            *state = 3;

        Func_0200101c(0, record[0], record[2], (u8 *)(record + 3));
        Func_0200101c(2, record[0], record[2], (u8 *)(record + 3));

        if ((*state & 1) != 0) {
            if (Func_080091b0(2, *(s32 *)(actor + 8),
                              *(s32 *)(actor + 16)) == 50) {
                u8 *flags = actor + 0x23;
                Func_080f9010(189);
                *flags &= 0xfe;
                Func_02001074(actorId, 1);
                *flags |= 1;
            } else if (Func_080091b0(2, *(s32 *)(actor + 8),
                                     *(s32 *)(actor + 16)) == 51) {
                u8 *flags = actor + 0x23;
                Func_02000da8(actor, 0);
                Func_080f9010(189);
                *(s32 *)(actor + 12) = 0;
                *flags &= 0xfe;
                Func_02001174(actorId);
                *(s32 *)(actor + 8) = 0;
                *(s32 *)(actor + 12) = 0;
                *(s32 *)(actor + 16) = 0;
                *flags |= 1;
            } else {
                Func_02000d78(actor);
            }
            *state = 0;
        }

        Func_02000fcc(0, *(s32 *)(actor + 8) >> 20,
                      *(s32 *)(actor + 16) >> 20, record + 3);

        if (*(s32 *)(actor + 12) >= 0) {
            Func_02000fcc(0, 27, (*(s32 *)(actor + 12) >> 20) + 6, tile);
            Func_0200101c(0, *(s32 *)(actor + 8) >> 20,
                          *(s32 *)(actor + 16) >> 20, tile);
            tile[1] = (u8)((tile[1] & 0x3f) |
                           (((u8 *)(record + 3))[1] & 0xc0));
            Func_0200101c(2, *(s32 *)(actor + 8) >> 20,
                          *(s32 *)(actor + 16) >> 20, tile);
        }

        record[0] = *(s32 *)(actor + 8) >> 20;
        record[1] = *(s32 *)(actor + 12) >> 20;
        record[2] = *(s32 *)(actor + 16) >> 20;

        {
            s32 otherIndex;
            for (otherIndex = 0; otherIndex < 4; otherIndex++) {
                s32 *otherRecord = records + otherIndex * 5;
                u8 *other;

                if (otherIndex == recordIndex)
                    continue;
                Func_080770d0(otherRecord[4]);
                other = Func_0808a080(otherIndex + 8);
                if ((*(s32 *)(actor + 8) >> 20) ==
                        (*(s32 *)(other + 8) >> 20) &&
                    (*(s32 *)(actor + 16) >> 20) ==
                        (*(s32 *)(other + 16) >> 20) &&
                    *(s32 *)(actor + 12) > *(s32 *)(other + 12)) {
                    Func_080770c8(otherRecord[4]);
                }
            }
        }
    }

    Func_02001be8();
}
