typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct ActorRecord_02001300 {
    u8 padding00[0x30];
    s32 field30;
    s32 field34;
};

/*
 * Flag-0x302 actor-state copier at 0x02001300, complete 120-byte span through
 * its sole return and five-word pool before 0x02001378.  The two actor-field
 * copies and independently linked coordinate-table lookup remain explicit.
 */

extern const u8 Data_02000240[];
extern const s32 Data_02001f00[][2];
extern s32 Func_080770c0(s32 flag);
extern struct ActorRecord_02001300 *Func_0808a080(s32 actor);
extern void Func_0808a0c8(s32 actor, s32 x, s32 z);

void Func_02001300(void)
{
    u8 *state = *(u8 **)0x03001ebc;
    struct ActorRecord_02001300 *actor8;
    struct ActorRecord_02001300 *actor0;
    s32 index;

    if (Func_080770c0(0x302) == 0)
        return;
    if (*(const s16 *)(Data_02000240 + 0x24a) == 8)
        return;

    index = *(s16 *)(state + 0x16c) - 45;
    actor8 = Func_0808a080(8);
    actor0 = Func_0808a080(0);
    actor8->field30 = actor0->field30;
    actor8 = Func_0808a080(8);
    actor0 = Func_0808a080(0);
    actor8->field34 = actor0->field34;
    Func_0808a0c8(8, Data_02001f00[index][0], Data_02001f00[index][1]);
}
