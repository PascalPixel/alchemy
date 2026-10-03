#include "RESOURCE.H"
/* 2026-09-29: five minutes of permutation reached 9291 from 10306 through
 * 70 rewrites, mostly operand swaps, casts and temporaries; not kept, since
 * the owner is far from exact. */
/* Complete owner [08020bd8, 0802106c): 1172 bytes including both final
 * pool words. The default unregistered score incorrectly included the
 * following menu and Djinn functions (1928 bytes); score this extent only.
 * Named globals plus shared MenuCursor unions: 1140 bytes, 526 differing
 * halfwords / 347 aligned edits before correcting the owner lookup argument.
 * The union only repairs one store-order slot here. Named globals leave a
 * 92-byte frame instead of 96; the saved text length remains in a high
 * register instead of its stack slot. The accept-key block rotates before
 * the loop, and several literal pools differ. Replacing the loop with
 * explicit next-frame/finished gotos grows to 1192 bytes and 385 edits.
 * Three bounded structural hypotheses stopped; do not repeat those axes.
 * FAKEMATCH: shared cursor unions preserve object-pointer store ordering. */
#include "TYPES.H"
#include "IO_REG.H"
#include "DMA.H"
#include "RENDER_INPUT.H"
#include "SHOP.H"
#include "WORKSPACE_OPTIONS.H"

extern u8 *gWindowWork;
extern u32 gFrameTick;
extern u32 gKeysRepeat;
extern u32 gKeyState;
extern s8 Data_080371f6[];

u8 *Owner_GetStateFar(s32 owner);
void Ui_LoadWindowGraphics(void);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
s32 Localization_LookupEntryId(s32);
s32 UiWindow_CreateWithSideObject(s32,s32,s32,s32);
void UiWindow_DrawDividerLine(struct RenderInput *,s32,s32,s32,s32);
void UiText_DrawPaddedLabel(struct RenderInput *,u8 *);
void ShopCursor_SetPositionImmediateFar(union MenuCursor *,s32,s32);
s32 UiText_SetRenderString(u8 *);
void RenderOutput_PrepareForRedraw(struct RenderInput *);
void WaitFrames(s32);
void Shop_SetCursorFar(union MenuCursor *,s32,s32,s32);
void ShopCursor_AdvanceFar(union MenuCursor *);
void ShopCursor_MoveTowardTargetFar(union MenuCursor *);
void Audio_PlayCue(s32);
void UiWork_Finalize(struct RenderInput *,s32);
void UiWork_FinalizeEntityMatchingLocalizedId(s32);

/* The five-character name editor uses the same cursor state as shop menus. */
s32 NameEntry_EditOwnerName(s32 entry)
{
    s32 result = 0;
    s32 original_length = 0;
    s32 length = 0;
    u8 text[16];
    u8 *input = text + 1;
    u8 *saved = Owner_GetStateFar(entry);
    u8 *work = gWindowWork;
    s32 text_dirty = 1;
    s32 cursor_dirty = 1;
    struct RenderInput *window;
    struct RenderInput *label;
    union MenuCursor cursor;
    union MenuCursor caret;
    s32 column, row, slot;
    u8 *end;
    struct RenderOutput *output;

    Ui_LoadWindowGraphics();
    window = UiWindow_Create(3,6,24,9,2);
    label = UiWindow_Create(8,3,8,3,2);
    UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry),0,3,1);
    UiWindow_CopyTilemapRegion(window,(void *)0x08073864);
    UiWindow_DrawDividerLine(window,18,0,18,7);
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
    UiText_DrawPaddedLabel(label,saved);
    slot = Resource_FindFreeEntry();
    column = 18;
    row = 5;
    if(slot <= 95) {
        VramBlock_LoadCached(slot,128,(void *)0x080310a4);
        output = RenderOutput_Create(slot,0x40000000,window,0,0);
        cursor.output = output;
        ShopCursor_SetPositionImmediateFar(&cursor,window->x * 8 + 140,window->y * 8 + 52);
    }
    slot = Resource_FindFreeEntry();
    if(slot <= 95) {
        VramBlock_LoadCached(slot,128,(void *)0x080317e4);
        output = RenderOutput_Create(slot,0x40000000,window,0,0);
        caret.output = output;
        output->sentinel = 255;
        ((u8 *)output)[25] &= -13;
        ShopCursor_SetPositionImmediateFar(&caret,UiText_SetRenderString(input)+70,22);
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
        UiWindow_SetTileAttributeRect(window,column,row,width,1,14);
        WaitFrames(1);
        UiWindow_SetTileAttributeRect(window,column,row,width,1,15);
        if(cursor_dirty) {
            cursor_dirty = 0;
            Shop_SetCursorFar(&cursor,(window->x+column)*8-7,(window->y+row)*8+15,3);
        }
        if(text_dirty) {
            text_dirty = 0;
            Shop_SetCursorFar(&caret,UiText_SetRenderString(input)+70,22,3);
        }
        ShopCursor_AdvanceFar(&cursor);
        ShopCursor_MoveTowardTargetFar(&caret);
        {
            u32 frame = (gFrameTick >> 1) & 7;
            u8 *sprite = (u8 *)caret.output;
            s8 *wave = Data_080371f6;
            u32 x = (*(u16 *)(sprite+6) + wave[frame]) & 511;
            *(u16 *)(sprite+22) = (*(u16 *)(sprite+22) & 0xfffffe00) | x;
            frame = (frame+5)&7;
            sprite[20] = sprite[8] + ((u8 *)wave)[frame];
        }
        if(gKeysRepeat & KEY_UP) {
            Audio_PlayCue(111); cursor_dirty = 1; row--;
            if(column != 18) { if(row == -1) row = 5; }
            else row = 5 - (row != 3);
        }
        if(gKeysRepeat & KEY_DOWN) {
            Audio_PlayCue(111); cursor_dirty = 1; row++;
            if(column != 18) { if(row == 6) row = 0; }
            else row = 4 + (row != 6);
        }
        if(gKeysRepeat & KEY_LEFT) {
            Audio_PlayCue(111); cursor_dirty = 1; column--;
            if(column == -1) { column = 18; if((u32)(row-4)>1) column = 16; }
            else if(column==5 || column==11 || column==17) column--;
        }
        if(gKeysRepeat & KEY_RIGHT) {
            Audio_PlayCue(111); cursor_dirty = 1; column++;
            if(column == 19) column=0;
            else if(column==5 || column==11 || column==17) column++;
            if(column==18 && (u32)(row-4)>1) column=0;
        }
        if(gKeyState & KEY_START) {
            Audio_PlayCue(111); cursor_dirty=1; column=18; row=5;
        }
        if(gKeysRepeat & KEY_B) {
            Audio_PlayCue(113);
remove_character:
            if(length != 0) {
                length--; *--end = 0;
                RenderOutput_PrepareForRedraw(label);
                UiText_DrawPaddedLabel(label,input);
                text_dirty=1;
                continue;
            }
            result=-1;
            break;
        }
        if(!(gKeysRepeat & KEY_A)) continue;
        Audio_PlayCue(112);
        if(column==18) {
            if(row==5) {
                if(length==0) {
                    caret.output->active = 13;
                    RenderOutput_PrepareForRedraw(label);
                    UiText_DrawPaddedLabel(label,saved);
                    WaitFrames(10);
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
            RenderOutput_PrepareForRedraw(label);
            UiText_DrawPaddedLabel(label,input);
            text_dirty=1;
        }
    }
    UiWork_Finalize(window,2);
    UiWork_Finalize(label,2);
    UiWork_FinalizeEntityMatchingLocalizedId(entry);
    WaitFrames(1);
    return result;
}
