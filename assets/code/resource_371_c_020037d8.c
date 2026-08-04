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
void *Func_02007b28(s32 actor);
void Func_02007c22(s32 actor, s32 mode);
void Func_02007c34(s32 actor, s32 mode);
void Func_02007a90(s32 flag);
void Func_02007a92(s32 flag);
void Func_02007ab2(s32 flag, s32 value);

/* Publish the actor-98 scene state and restore its selected actor. */
void Func_020037d8(void)
{
    u8 *actor;

    if (Data_03001ebc->scene_value == 99) {
        Data_03001ebc->scene_value = 0;
    }
    Func_02007a92(0x2f0);
    Func_02007a90(0x2f1);
    Func_02007ab2(0x2f8, 0);
    Func_02007c22(98, 5);
    Data_02000240[0x22b] = 3;
    Func_02007c34(98, 7);
    actor = Func_02007b28(*(s32 *)(Data_02000240 + 500));
    actor[85] = 2;
}
