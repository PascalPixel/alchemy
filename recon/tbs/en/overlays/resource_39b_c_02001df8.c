/* NONMATCHING: 208 of 208 bytes, 15 differing halfwords (2026-09-24); twin of
 * 39c:02005388. Unit symbols: Data_02000240_t 0x02000240 and Rumble_Script
 * 0x0200a7c4 (data). Remaining: the reference dumps its first literal pool two
 * instructions later (after the second object's p/zero setup); the HImode
 * zero constant forces the mid-function pool here as well. */
/* Instanced unit: 39b:02001df8 and its twin 39c:02005388; the dust script is a
 * per-overlay absolute symbol. */
#include "TYPES.H"

u8 *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Engine_ObjectSetScript(u8 *obj, s32 script);
void Main_08009020(u8 *sprite, s32 anim);
void Engine_AudioPlayCue(s32 cue);

extern u8 Data_00000000[];
extern s32 Data_02000240_t[];
extern u8 Rumble_Script[];

struct Work {
    u8 pad[20];
    u8 *actors[1];
};

struct Sprite39b {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

/* Two clouds of dust rise around the party leader as the pillar moves. */
void MakyuriIriguchi_Func02001df8(void)
{
    u8 *leader;
    u8 *obj;
    struct Sprite39b *spr;

    leader = (*(struct Work **)0x03001ebc)->actors[Data_02000240_t[125]];
    obj = Engine_ObjectCreate(26, *(s32 *)(leader + 8), *(s32 *)(leader + 12), *(s32 *)(leader + 16));
    if (obj != 0) {
        *(s32 *)(obj + 20) = *(s32 *)(leader + 20);
        spr = *(struct Sprite39b **)(obj + 80);
        Engine_ObjectSetScript(obj, (s32)Rumble_Script);
        {
            u8 *p = obj + 85;
            s32 zero = 0;

            *p = zero;
            *(u16 *)(p + 15) = zero;
        }
        *(u8 **)(obj + 104) = leader;
        if (spr != 0) {
            Main_08009020((u8 *)spr, 2);
            {
                s32 z = (u16)(u32)Data_00000000;

                ((u8 *)spr)[38] = z;
            }
            spr->layer = 1;
        }
    }
    obj = Engine_ObjectCreate(26, *(s32 *)(leader + 8), *(s32 *)(leader + 12), *(s32 *)(leader + 16));
    if (obj != 0) {
        *(s32 *)(obj + 20) = *(s32 *)(leader + 20);
        spr = *(struct Sprite39b **)(obj + 80);
        Engine_ObjectSetScript(obj, (s32)Rumble_Script);
        {
            u8 *p = obj + 85;
            s32 zero = 0;

            *p = zero;
            *(u16 *)(p + 15) = zero;
        }
        *(u8 **)(obj + 104) = leader;
        obj[35] = 2;
        if (spr != 0) {
            Main_08009020((u8 *)spr, 1);
            ((u8 *)spr)[38] = (s32)Data_00000000;
        }
    }
    Engine_AudioPlayCue(130);
}
