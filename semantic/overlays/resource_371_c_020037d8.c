typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

struct StoryWork_020037d8 {
    u8 reserved000[386];
    s16 scene_value;
};

extern struct StoryWork_020037d8 *Data_03001ebc;
extern u8 Data_02000240[];
extern void *Func_0808a080(s32 actor);
extern void Func_0808a250(s32 actor, s32 mode);
extern void Func_080770c8(s32 flag);
extern void Func_080770d0(s32 flag);
extern void Func_080770e8(s32 flag, s32 value);

/* Publish the actor-98 scene state and restore its selected actor. */
void Func_020037d8(void)
{
    u8 *actor;

    if (Data_03001ebc->scene_value == 99) {
        Data_03001ebc->scene_value = 0;
    }
    Func_080770d0(0x2f0);
    Func_080770c8(0x2f1);
    Func_080770e8(0x2f8, 0);
    Func_0808a250(98, 5);
    Data_02000240[0x22b] = 3;
    Func_0808a250(98, 7);
    actor = Func_0808a080(*(s32 *)(Data_02000240 + 500));
    actor[85] = 2;
}
