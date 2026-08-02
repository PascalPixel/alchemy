typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3bd owner at 0x02000bc8 (100 bytes).  This is a short scene
 * selector hook: it watches the active scene id at Data_02000240 + 0x1c0,
 * enables the blend-alpha register for scene 0x97, and selects the five
 * presentation modes used by that scene.  The owner is a leaf (`push {lr}` /
 * `pop {r0}; bx r0`) and takes no arguments.
 *
 * The literal pool is at 0x02000c1c..0x02000c2b.  Its words are the shared
 * scene-state base, 0x92, the GBA blend-alpha register, and 0x97.  The store
 * is expressed as the ordinary volatile hardware write used by the resident
 * semantic sources; it is not an inline-assembly substitute.
 */

extern s16 Data_02000240[];
extern void Func_0808a158(s32 id, s32 mode);

void Func_02000bc8(void)
{
    u16 scene = (u16)Data_02000240[224];

    if ((s16)scene == 0x92)
        *(volatile u16 *)0x04000052 = 0x1000;

    if ((s16)scene != 0x97)
        return;

    Func_0808a158(16, 1);
    Func_0808a158(17, 4);
    Func_0808a158(18, 11);
    Func_0808a158(19, 2);
    Func_0808a158(20, 3);
}
