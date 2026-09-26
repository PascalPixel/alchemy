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
 * numeric rather than ROM order; mode 2 reloads attributes after Math_ModU
 * instead of retaining their pre-call values. No exact bytes are adopted.
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

struct UiSprite {
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
    struct UiSprite sprite;
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

extern struct UiAnimationWork *Data_03001e8c;
extern u32 Data_03001800;
extern const u8 Data_080368d4[];
extern const u8 Data_08033e60[];
extern const u8 Data_08033eb0[];
extern const u8 Data_08033ee8[];

u32 Math_ModU(u32 numerator, u32 denominator);
u32 Random16(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
s32 AffineMatrix_BuildForEffect(struct UiEffect *effect);
void RenderOutput_UpdateScaleAnimation(struct UiAnimatedItem *item);
void Runtime_PushSlotEntry(s32 *entry, s32 slot);

void UiWork_AnimateSpriteSlots(void)
{
    struct UiAnimationWork *work = Data_03001e8c;
    struct UiAnimationSlot *slot = work->slots;
    s32 no;
    struct UiEffect effect;

    for (no = 0; no != 8; no++, slot++) {
        struct UiAnimatedItem *item;
        u32 phase;

        if (!(slot->flags & 1))
            continue;
        item = slot->head;
        phase = (Data_03001800 >> 2) & 7;
        while (item != NULL) {
            struct UiSprite *sprite = &item->sprite;
            if (slot->mode == 4) {
                item->frame = 2;
                item->mode = 8;
            }
            switch (item->mode) {
            case 2:
                if (work->resource != 0x60) {
                    sprite->attribute.bits.tile = VramBlock_LoadCached(
                        work->resource, 128, &Data_080368d4[phase * 128]);
                    item->tile = sprite->attribute.value;
                    sprite->mode = 0;
                    sprite->mosaic = 0;
                    sprite->color = 1;
                    sprite->shape = 2;
                    sprite->size = 0;
                    sprite->y = *(u8 *)&item->y +
                        Data_08033e60[Math_ModU(Data_03001800, 80)] + 2;
                    sprite->affine = 0;
                    sprite->affine_index = 0;
                }
                break;
            case 4:
                if (Data_03001800 & 1)
                    item->frame++;
                sprite->x = item->x +
                    (s8)Data_08033eb0[(u16)Math_ModU(item->frame, 20) * 2];
                sprite->y = *(u8 *)&item->y +
                    Data_08033eb0[(u16)Math_ModU(item->frame, 20) * 2 + 1] - 2;
                break;
            case 5:
                if (Data_03001800 & 1) {
                    u32 a, b;
                    a = Random16();
                    b = Random16();
                    sprite->x = item->x + (((a * 3 >> 16) +
                        (b * 3 >> 16)) >> 1) - 1;
                    a = Random16();
                    b = Random16();
                    sprite->y = *(u8 *)&item->y + (((a * 3 >> 16) +
                        (b * 3 >> 16)) >> 1) - 1;
                }
                break;
            case 6:
                if (item->frame == 0)
                    goto reset;
                effect.x = 512;
                effect.y = 512;
                effect.angle = 0;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 3;
                /* FAKEMATCH: unsigned wrapped offsets retain literal sharing. */
                sprite->x = item->x + 0xfffb;
                sprite->y = *(u8 *)&item->y + 251;
                item->frame += 0xffff;
                break;
            case 7:
                effect.x = 256;
                effect.y = 256;
                item->frame += 768;
                effect.angle = item->frame;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 1;
                sprite->x = item->x - (Trig_Sin(effect.angle + 0xe800) >> 14) - 2;
                sprite->y = *(u8 *)&item->y - (Trig_Cos(effect.angle + 0x6800) >> 14) - 2;
                break;
            case 8:
                if (item->frame == 0)
                    goto reset;
                effect.x = 320;
                effect.y = 320;
                effect.angle = 0;
                sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
                sprite->affine = 3;
                /* FAKEMATCH: unsigned wrapped offsets retain literal sharing. */
                sprite->x = item->x + 0xfff8;
                sprite->y = *(u8 *)&item->y + 248;
                item->frame += 0xffff;
                break;
            reset:
                sprite->affine_index = 0;
                sprite->affine = 0;
                sprite->x = item->x;
                sprite->y = item->y;
                break;
            case 9:
            case 10:
            case 11:
            case 12:
                RenderOutput_UpdateScaleAnimation(item);
                break;
            case 14:
            case 15:
            case 16:
                item->frame++;
                sprite->y = *(u8 *)&item->y + Data_08033ee8[item->frame & 15];
                break;
            case 17:
                item->frame++;
                sprite->y = *(u8 *)&item->y - Data_08033ee8[item->frame & 15];
                break;
            case 18:
                item->frame++;
                sprite->x = item->x - (s8)Data_08033ee8[item->frame & 15];
                sprite->y = *(u8 *)&item->y + Data_08033ee8[item->frame & 15];
                break;
            }
            if (item->mode == 2) {
                if (work->resource != 0x60)
                    Runtime_PushSlotEntry(&sprite->next, item->priority);
            } else if (item->mode != 13) {
                Runtime_PushSlotEntry(&sprite->next, item->priority);
            }
            item = item->next;
            phase = (Data_03001800 >> 2) & 7;
        }
    }
}
