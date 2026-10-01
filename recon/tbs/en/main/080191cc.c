/* DRAFT: whole Thumb owner [080191cc,0801964c), 1152 bytes with its
 * switch table and all literal pools. Split and byte-verified on main.
 * Baseline: transfer the proven 8-byte Effect bitfields and sprite attributes
 * from AffineMatrix_BuildForEffect/Ui_ApplyTableScaleToObject, and recover
 * the 36-byte slot, linked 28-byte item and embedded +16 sprite ownership.
 * Correct the old lift's case mapping, random unsignedness, frame updates,
 * paired/signed table indexing, trig calls, and stores to item versus sprite.
 * Prediction: all 17 switch entries, two loops and call ordering agree;
 * the reference has a 24-byte frame including an 8-byte effect at sp+16.
 * Gate: whole owner exact plus compare-all/test/coverage/verify. Diagnose
 * the complete normalized diff. One model + at most two structural follow-ups;
 * stop within 25 minutes, record rejected hypotheses here, never sweep RA.
 * Layout admission rejected a byte/halfword Y union (word aligned here) and
 * an extra sprite tail halfword (the tile union already occupies four bytes).
 * Use the proven halfword Y/byte alias and the 12-byte embedded sprite.
 * Baseline result: 1136/1152 bytes, 398 aligned halfword edits, frame 8/24.
 * Complete diff: switch entries are semantically mapped but bodies are in
 * numeric rather than ROM order; mode 2 reloads attributes after __umodsi3
 * instead of retaining their pre-call values. No exact bytes are adopted.
 * H1: recover the ROM's case-body order (2,5,6,7,4,17,14-16,18,8,9-12).
 * Prediction: switch destinations and shared reset/push tails align before
 * interpreting allocation differences. Do not change calculations yet.
 * H1 result: 1136/1152 bytes, 258 aligned edits (baseline 398). Body order
 * is repaired; frame remains 8/24 and mode 2's live attribute bytes are not
 * retained. This is a useful topology repair, not a match or RA solution.
 * H2: expose one union byte/attribute view and retain the mode-2 flags,
 * X-high byte and original Y across unsigned remainder. The pure arithmetic
 * callee does not own these sprite bytes. Prediction: the missing r8 value
 * and attribute spills appear, shifting the Effect towards reference sp+16.
 * H2 result: 1168/1152 bytes, 216 aligned edits, 498 differing halfwords.
 * Full diff read. Work=r9, slot=sl, no=fp, sprite=r7 and the complete saved
 * register sequence now agree; pre-call Y lives in r8 and table spills at
 * sp+4. Rejected for adoption: frame 16/24, Effect sp+8/sp+16, item r4/r6
 * with extra per-call spills, X-high r6/r4, and merged push/store tails.
 * Other remaining evidence: resource argument reload, signed coordinate
 * subtraction after unsigned random scaling, paired-table pointer lifetime,
 * and mode-7/mode-4 Y tails differ. No register permutation was attempted.
 * Budget exhausted: one complete model plus two structural follow-ups.
 * Keep this typed lifetime evidence; do not count any part as DONE. A later
 * attempt needs a new source-ownership hypothesis, not a spelling sweep.
 * 2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 27,992
 * candidates; the best scored 1737 against 5242 (14 register-only, 7
 * stack-only, 8 operand, 8 reordered, 4 inserted, 6 deleted) after 125
 * rewrites (reorder independent statements, introduce a temporary, swap
 * commutative operands, reorder local declarations), none of them kept. The
 * rewrites (forty-two temporaries, register keywords, statement moves
 * through the item loop) are search artefacts, so the draft keeps its
 * spelling; the size of the drop says the item loop's statement order and
 * temporaries carry most of the difference.
 */
#include "RENDER_INPUT.H"
#include "FIXED_MATH.H"

struct UiEffect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

union UiTileAttribute {
    u16 value;
    struct {
        u16 tile : 10;
        u16 rest : 6;
    } bits;
};

struct UiSpriteAttributes {
    s32 next;
    u8 y;
    u8 affine : 2;
    u8 mode : 2;
    u8 mosaic : 1;
    u8 color : 1;
    u8 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    union UiTileAttribute attribute;
};

union UiSprite {
    struct UiSpriteAttributes fields;
    struct {
        s32 next;
        u8 y;
        u8 flags;
        u8 x_low;
        u8 x_high;
        u16 attribute;
        u16 unused;
    } bytes;
};

struct UiAnimatedItem {
    struct UiAnimatedItem *next;
    u8 unknown_04;
    u8 mode;
    u16 x;
    u16 y;
    u16 unknown_0a;
    u16 frame;
    u8 tile;
    u8 priority;
    union UiSprite sprite;
};

struct UiAnimationSlot {
    struct UiAnimatedItem *head;
    u8 unknown_04[14];
    u16 mode;
    u16 unknown_14;
    u16 flags;
    u8 unknown_18[12];
};

struct UiAnimationWork {
    u8 unknown_000[0x500];
    struct UiAnimationSlot slots[8];
    u8 unknown_620[0x12b6 - 0x620];
    u16 resource;
};

typedef char UiAnimationSlot_size[sizeof(struct UiAnimationSlot) == 36 ? 1 : -1];
typedef char UiAnimatedItem_size[
    sizeof(struct UiAnimatedItem) == sizeof(struct RenderOutput) ? 1 : -1];
typedef char UiAnimatedItem_sprite[
    (u32)&((struct UiAnimatedItem *)0)->sprite == 16 ? 1 : -1];
typedef char UiEffect_size[sizeof(struct UiEffect) == 8 ? 1 : -1];

extern struct UiAnimationWork *gWindowWork;
extern u32 gFrameTick;
extern const u8 Data_080368d4[];
extern const u8 Data_08033e60[];
extern const u8 Data_08033eb0[];
extern const u8 Data_08033ee8[];

u32 __umodsi3(u32 numerator, u32 denominator);
u32 Random16(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
s32 AffineMatrix_BuildForEffect(struct UiEffect *effect);
void RenderOutput_UpdateScaleAnimation(struct UiAnimatedItem *item);
void Runtime_PushSlotEntry(s32 *entry, s32 slot);

void UiWork_AnimateSpriteSlots(void)
{
    struct UiAnimationWork *work = gWindowWork;
    struct UiAnimationSlot *slot = work->slots;
    s32 no;
    struct UiEffect effect;

    for (no = 0; no != 8; no++, slot++) {
        struct UiAnimatedItem *item;
        u32 phase;

        if (!(slot->flags & 1))
            continue;
        item = slot->head;
        phase = (gFrameTick >> 2) & 7;
        while (item != NULL) {
            union UiSprite *sprite = &item->sprite;
            if (slot->mode == 4) {
                item->frame = 2;
                item->mode = 8;
            }
            switch (item->mode) {
            case 2:
                if (work->resource != 0x60) {
                    u32 flags, x_high, y;
                    const u8 *table;
                    u32 step;
                    sprite->fields.attribute.bits.tile = VramBlock_LoadCached(
                        work->resource, 128, &Data_080368d4[phase * 128]);
                    item->tile = sprite->fields.attribute.value;
                    /* FAKEMATCH: retain the two attribute bytes across the
                     * arithmetic call instead of reloading their bitfields. */
                    flags = sprite->bytes.flags;
                    flags &= ~12;
                    flags &= ~16;
                    flags |= 32;
                    flags = (flags & 63) | 128;
                    x_high = sprite->bytes.x_high & 63;
                    sprite->bytes.x_high = x_high;
                    sprite->bytes.flags = flags;
                    y = *(u8 *)&item->y;
                    table = Data_08033e60;
                    step = __umodsi3(gFrameTick, 80);
                    sprite->fields.y = y + table[step] + 2;
                    sprite->bytes.flags = flags & ~3;
                    sprite->bytes.x_high = x_high & ~62;
                }
                break;
            case 5:
                if (gFrameTick & 1) {
                    u32 a, b;
                    a = Random16();
                    b = Random16();
                    sprite->fields.x = item->x + (((a * 3 >> 16) +
                        (b * 3 >> 16)) >> 1) - 1;
                    a = Random16();
                    b = Random16();
                    sprite->fields.y = *(u8 *)&item->y + (((a * 3 >> 16) +
                        (b * 3 >> 16)) >> 1) - 1;
                }
                break;
            case 6:
                if (item->frame == 0)
                    goto reset;
                effect.x = 512;
                effect.y = 512;
                effect.angle = 0;
                sprite->fields.affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->fields.affine = 3;
                /* FAKEMATCH: unsigned wrapped offsets retain literal sharing. */
                sprite->fields.x = item->x + 0xfffb;
                sprite->fields.y = *(u8 *)&item->y + 251;
                item->frame += 0xffff;
                break;
            case 7:
                effect.x = 256;
                effect.y = 256;
                item->frame += 768;
                effect.angle = item->frame;
                sprite->fields.affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->fields.affine = 1;
                sprite->fields.x = item->x - (Trig_Sin(effect.angle + 0xe800) >> 14) - 2;
                sprite->fields.y = *(u8 *)&item->y - (Trig_Cos(effect.angle + 0x6800) >> 14) - 2;
                break;
            case 4:
                if (gFrameTick & 1)
                    item->frame++;
                sprite->fields.x = item->x +
                    (s8)Data_08033eb0[(u16)__umodsi3(item->frame, 20) * 2];
                sprite->fields.y = *(u8 *)&item->y +
                    Data_08033eb0[(u16)__umodsi3(item->frame, 20) * 2 + 1] - 2;
                break;
            case 17:
                item->frame++;
                sprite->fields.y = *(u8 *)&item->y - Data_08033ee8[item->frame & 15];
                break;
            case 14:
            case 15:
            case 16:
                item->frame++;
                sprite->fields.y = *(u8 *)&item->y + Data_08033ee8[item->frame & 15];
                break;
            case 18:
                item->frame++;
                sprite->fields.x = item->x - (s8)Data_08033ee8[item->frame & 15];
                sprite->fields.y = *(u8 *)&item->y + Data_08033ee8[item->frame & 15];
                break;
            case 8:
                if (item->frame == 0)
                    goto reset;
                effect.x = 320;
                effect.y = 320;
                effect.angle = 0;
                sprite->fields.affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->fields.affine = 3;
                /* FAKEMATCH: unsigned wrapped offsets retain literal sharing. */
                sprite->fields.x = item->x + 0xfff8;
                sprite->fields.y = *(u8 *)&item->y + 248;
                item->frame += 0xffff;
                break;
            reset:
                sprite->fields.affine_index = 0;
                sprite->fields.affine = 0;
                sprite->fields.x = item->x;
                sprite->fields.y = item->y;
                break;
            case 9:
            case 10:
            case 11:
            case 12:
                RenderOutput_UpdateScaleAnimation(item);
                break;
            }
            if (item->mode == 2) {
                if (work->resource != 0x60)
                    Runtime_PushSlotEntry(&sprite->fields.next, item->priority);
            } else if (item->mode != 13) {
                Runtime_PushSlotEntry(&sprite->fields.next, item->priority);
            }
            item = item->next;
            phase = (gFrameTick >> 2) & 7;
        }
    }
}
