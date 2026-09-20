#include "TYPES.H"
#include "DMA.H"
#include "RENDER_INPUT.H"
#include "SHOP.H"

u8 *Func_08077008(void);
void Func_0800479c(void);
struct RenderInput *Func_080162d4(s32, s32, s32, s32, s32);
s32 Func_08019d2c(s32);
s32 Func_08019da8(s32,s32,s32,s32);
void Func_080209d0(struct RenderInput *, const void *);
void Func_0801e41c(struct RenderInput *,s32,s32,s32,s32);
void Func_08020b64(struct RenderInput *,u8 *);
s32 Func_08004080(void);
s32 Func_08003fa4(u32,u32,const void *);
struct RenderOutput *Func_0801eadc(s32,s32,struct RenderInput *,s32,s32);
void Func_080b0038(struct ShopCursor *,s32,s32);
s32 Func_08020b14(u8 *);
void Func_08016478(struct RenderInput *);
void Func_080030f8(s32);
void Func_08020a60(struct RenderInput *,s32,s32,s32,s32,u32);
void Func_080b0030(struct ShopCursor *,s32,s32,s32);
void Func_080b0020(struct ShopCursor *);
void Func_080b0028(struct ShopCursor *);
void Func_080f9010(s32);
void Func_08016418(struct RenderInput *,s32);
void Func_08019e48(s32);

/* The five-character name editor uses the same cursor state as shop menus. */
s32 Func_08020bd8(s32 entry)
{
    s32 result = 0;
    s32 original_length = 0;
    s32 length = 0;
    u8 text[16];
    u8 *input = text + 1;
    u8 *saved = Func_08077008();
    u8 *work = *(u8 **)0x03001e8c;
    s32 text_dirty = 1;
    s32 cursor_dirty = 1;
    struct RenderInput *window;
    struct RenderInput *label;
    struct ShopCursor cursor;
    struct ShopCursor caret;
    s32 column, row, slot;
    u8 *end;
    struct RenderOutput *output;

    Func_0800479c();
    window = Func_080162d4(3,6,24,9,2);
    label = Func_080162d4(8,3,8,3,2);
    Func_08019da8(Func_08019d2c(entry),0,3,1);
    Func_080209d0(window,(void *)0x08073864);
    Func_0801e41c(window,18,0,18,7);
    work[0xea3] = text_dirty;
    text[0] = result;
    {
        u8 *dst = input;
        u8 *src = saved;
        do {
            u32 c = *src++;
            *dst++ = c;
            if(c != 0) { original_length++; length++; }
        } while(dst <= text + 14);
    }
    input[14] = 0;
    Func_08020b64(label,saved);
    slot = Func_08004080();
    column = 18;
    row = 5;
    if(slot <= 95) {
        Func_08003fa4(slot,128,(void *)0x080310a4);
        output = Func_0801eadc(slot,0x40000000,window,0,0);
        cursor.anchor = (struct ShopCursorAnchor *)output;
        Func_080b0038(&cursor,window->x * 8 + 140,window->y * 8 + 52);
    }
    slot = Func_08004080();
    if(slot <= 95) {
        Func_08003fa4(slot,128,(void *)0x080317e4);
        output = Func_0801eadc(slot,0x40000000,window,0,0);
        caret.anchor = (struct ShopCursorAnchor *)output;
        output->sentinel = 255;
        ((u8 *)output)[25] &= -13;
        Func_080b0038(&caret,Func_08020b14(input)+70,22);
    }
    Dma_Set((void *)0x050001e0,(void *)0x050001c0,0x84000008,(volatile u32 *)0x040000d4);
    *(volatile u16 *)0x050001c8 = 0x6318;
    end = input + original_length;
    for(;;) {
        s32 width = 1;
        if(column == 18) {
            if(row == 4) width = 3;
            if(row == 5) width = 3;
        }
        Func_08020a60(window,column,row,width,1,14);
        Func_080030f8(1);
        Func_08020a60(window,column,row,width,1,15);
        if(cursor_dirty) {
            cursor_dirty = 0;
            Func_080b0030(&cursor,(window->x+column)*8-7,(window->y+row)*8+15,3);
        }
        if(text_dirty) {
            text_dirty = 0;
            Func_080b0030(&caret,Func_08020b14(input)+70,22,3);
        }
        Func_080b0020(&cursor);
        Func_080b0028(&caret);
        {
            u32 frame = (*(u32 *)0x03001800 >> 1) & 7;
            u8 *sprite = (u8 *)caret.anchor;
            s8 *wave = (s8 *)0x080371f6;
            u32 x = (*(u16 *)(sprite+6) + wave[frame]) & 511;
            *(u16 *)(sprite+22) = (*(u16 *)(sprite+22) & 0xfffffe00) | x;
            frame = (frame+5)&7;
            sprite[20] = sprite[8] + ((u8 *)wave)[frame];
        }
        if(*(u32 *)0x03001b04 & 64) {
            Func_080f9010(111); cursor_dirty = 1; row--;
            if(column != 18) { if(row == -1) row = 5; }
            else row = 5 - (row != 3);
        }
        if(*(u32 *)0x03001b04 & 128) {
            Func_080f9010(111); cursor_dirty = 1; row++;
            if(column != 18) { if(row == 6) row = 0; }
            else row = 4 + (row != 6);
        }
        if(*(u32 *)0x03001b04 & 32) {
            Func_080f9010(111); cursor_dirty = 1; column--;
            if(column == -1) { column = 18; if((u32)(row-4)>1) column = 16; }
            else if(column==5 || column==11 || column==17) column--;
        }
        if(*(u32 *)0x03001b04 & 16) {
            Func_080f9010(111); cursor_dirty = 1; column++;
            if(column == 19) column=0;
            else if(column==5 || column==11 || column==17) column++;
            if(column==18 && (u32)(row-4)>1) column=0;
        }
        if(*(u32 *)0x03001c94 & 8) {
            Func_080f9010(111); cursor_dirty=1; column=18; row=5;
        }
        if(*(u32 *)0x03001b04 & 2) {
            Func_080f9010(113);
remove_character:
            if(length != 0) {
                length--; *--end = 0;
                Func_08016478(label);
                Func_08020b64(label,input);
                text_dirty=1;
                continue;
            }
            result=-1;
            break;
        }
        if(!(*(u32 *)0x03001b04 & 1)) continue;
        Func_080f9010(112);
        if(column==18) {
            if(row==5) {
                if(length==0) {
                    caret.anchor->kind=13;
                    Func_08016478(label);
                    Func_08020b64(label,saved);
                    Func_080030f8(10);
                } else {
                    s32 i=0; u8 *src=input; u8 *dst=saved;
                    do { *dst++ = *src++; i++; } while(i<=14);
                }
                break;
            }
            if(row==4) goto remove_character;
            continue;
        }
        {
            u32 cell=(window->y+row+1)*32+window->x+column+1;
            u32 character=work[cell*2];
            if(length==5) continue;
            *end++=character; *end=0; length++;
            if(length==5) { cursor_dirty=1; column=18; row=5; }
            Func_08016478(label);
            Func_08020b64(label,input);
            text_dirty=1;
        }
    }
    Func_08016418(window,2);
    Func_08016418(label,2);
    Func_08019e48(entry);
    Func_080030f8(1);
    return result;
}
