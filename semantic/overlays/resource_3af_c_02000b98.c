typedef int s32;

/*
 * resource_3af owner at 0x02000b98, 32 bytes: a one-line dialogue
 * callback -- open the scripted-scene bracket, speak line 0x1f00, hand
 * actor 21 back, close the bracket.
 *
 * PUBLISHED, NOT CALLED. Like 0x02003c88, no `bl` anywhere in the image
 * resolves here; the function is installed by data. Its Thumb pointer
 * (0x02000b99 + 0x8000 = 0x02008b99) appears at 0x02005a14, an entry of
 * the script-record table near the image end. It is in no inventory
 * row, not in assets/code, and `tools/overlay_unindexed.ts` reports
 * zero for this overlay.
 *
 * That last point is the finding worth carrying: after the queue's
 * fourteen rows were drafted, BOTH the whole-image BL sweep and
 * `overlay_unindexed.ts` read zero unowned functions, and the overlay
 * still had eleven owners left in it. A call-graph walk cannot see a
 * data-installed callback. The sweep that does see them is: for every
 * 4-aligned word in the image, take `(word & ~1) - 0x8000`, and if the
 * word had its Thumb bit set and the halfword at that offset matches
 * the prologue shape `0xb4xx`/`0xb5xx`, it is a published function.
 *
 * Complete owner: `push {lr}` at 0x02000b98 through `pop {r0} / bx r0`
 * at 0x02000bb0-0x02000bb2; one trailing pool word (0x00001f00) ends at
 * 0x02000bb7, and the next owner prologue (the drafted 0x02000bb8
 * reward twin) begins at 0x02000bb8.
 *
 * Uncertainty: the actor id and the dialogue id are transcribed; which
 * script slot installs this callback is known (0x02005a14) but what
 * that record represents is not.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a190(s32 id, s32 arg1);

void Func_02000b98(void)
{
    Func_0808a018();
    Func_0808a170(0x1f00);
    Func_0808a190(21, 0);
    Func_0808a020();
}
