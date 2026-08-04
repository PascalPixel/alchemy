typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3c8 owner at 0x020023d4, 168 bytes: among scene slots 8-13,
 * locate candidates sharing the selected slot's x/z tile, retain the highest
 * candidate at least one 16.16 unit above its y value, store that candidate id
 * at selected+100, then move/release the selected slot and run its local effect.
 *
 * Complete owner: high-register prologue and four-byte frame at 0x020023d4
 * through the sole interworking return at 0x02002468-0x02002474, followed by
 * alignment and one referenced pool word through 0x0200247b.  Eight static
 * calls across seven targets match independently; the two scene-accessor call
 * sites sit inside the bounded six-iteration loop.
 */

u8 *Func_0808a080();
void Func_0808a090();
void Func_08009150();
void Func_0808a0e8();
void Func_080f9010();
void Func_0808a010();
void Func_02000b08();

void Func_020023d4(s32 selector)
{
    u8 *candidateRecord;
    u8 *selectedRecord = 0;
    s32 highest = (s32)0xffb00000;
    s32 index;

    for (index = 0; index <= 5; index++) {
        s32 candidate = index + 8;
        s32 candidateY;

        if (candidate == selector) {
            continue;
        }

        candidateRecord = Func_0808a080(candidate);
        selectedRecord = Func_0808a080(selector);

        if ((*(s32 *)(candidateRecord + 8) >> 20)
                != (*(s32 *)(selectedRecord + 8) >> 20)
            || (*(s32 *)(candidateRecord + 16) >> 20)
                != (*(s32 *)(selectedRecord + 16) >> 20)) {
            continue;
        }

        candidateY = *(s32 *)(candidateRecord + 12) + 0x100000;
        if (candidateY >= highest) {
            *(u16 *)(selectedRecord + 100) = (u16)candidate;
            highest = candidateY;
        }
    }

    Func_0808a090(selector, 0x40000, 0x20000);
    Func_08009150(selectedRecord,
                  *(s32 *)(selectedRecord + 8),
                  highest,
                  *(s32 *)(selectedRecord + 16));
    Func_0808a0e8(selector);
    Func_080f9010(188);
    Func_02000b08(selector);
    Func_0808a010(30);
}
