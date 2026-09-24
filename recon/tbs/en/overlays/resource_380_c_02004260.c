/* NONMATCHING: 200 bytes, candidate 200, 2 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * the zero for the sprite flag bytes is built in r2 (reload) and moved to r8;
 * the reference builds it in r0 after the item copy to sl. Calls now carry
 * their Engine_ and Main_ names (the two text calls share one legacy veneer
 * address, so Func_ names were ambiguous). Plain, declaration, s32/u8
 * zero-variable and param-copy spellings give 4 or more. */
#include "TYPES.H"

u8 *Engine_ObjectCreate(s32);
s32 Main_08077040(s32);
s32 Main_08077038(s32, s32);
void Engine_ObjectSetScript(u8 *, void *);
u8 *Engine_HeapAllocate(s32, s32);
void Engine_ItemLoadIcon(s32);
void Engine_VramLoad(s32, s32, u8 *);
void Engine_HeapRelease(s32);
void Engine_AudioPlayCue(s32);
void Engine_RunRisingObjectSequence(u8 *, s32);
s32 Main_080772b0(s32, s32);
void Main_08077028(s32, s32);
void Engine_ObjectDispatchRelease(u8 *);
void Engine_ActorSetAnimation(s32, s32);
extern u8 Data_0200cbe4[];

s32 Local_02004260(s32 item)
{
    u8 *buf;
    u8 *obj;
    s32 text;
    s32 size;
    u8 *sprite;
    u8 *p;
    s32 mask;

    /* FAKEMATCH candidate: the do/while places the zero after the item copy.
     * Score 2026-09-23: 200 of 200 bytes, 2 halfword edits; the reference
     * builds the zero in r0 (mov r8, r0), this draft in r2. Zero, id and
     * item-derived spellings of buf leave it in r2. */
    do {
        buf = 0;
    } while (0);
    obj = Engine_ObjectCreate(22);
    text = Main_08077040(224);
    size = Main_08077038(text, 224);

    if (obj == 0) {
        return text;
    }
    {
        Engine_ObjectSetScript(obj, Data_0200cbe4);
        sprite = *(u8 **)(obj + 80);
        p = sprite + 38;
        *p = (u32)buf;
        p++;
        *p = (u32)buf;
        mask = 33;
        mask = -mask;
        sprite[5] &= mask;
        sprite[9] &= 0xf;
        *(s32 *)(obj + 40) = 163840;
        *(s32 *)(obj + 72) = 16384;
        buf = Engine_HeapAllocate(17, 1544);
        Engine_ItemLoadIcon(item);
        Engine_VramLoad(sprite[28], 128, buf + 1024);
        Engine_HeapRelease(17);
        Engine_AudioPlayCue(83);
        Engine_RunRisingObjectSequence(obj, 3);
        Main_080772b0(text, size);
        Main_08077028(text, item);
        Engine_ObjectDispatchRelease(obj);
        Engine_ActorSetAnimation(0, 1);
    }
    return text;
}
