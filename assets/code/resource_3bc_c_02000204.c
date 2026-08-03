typedef signed int s32;
typedef signed short s16;
typedef unsigned int u32;
typedef unsigned char u8;

typedef struct Object {
    u8 filler00[8];
    s32 x; /* 16.16 position */
    u8 filler0c[4];
    s32 z;
} Object;

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

Object *Func_02004c36();

void Func_02000204(void)
{
    u8 *workspace = Data_03001ebc;
    s16 *table = Data_02000240;
    s32 id = *(s32 *)&table[250];
    Object *subject = *(Object **)(workspace + 480);
    Object *actor = Func_02004c36(id);
    s32 z = *(s16 *)((u8 *)actor + 0x12);

    /* Nudge both records left while the actor occupies rows 183 through 186. */
    if ((u32)(z - 183) <= 3) {
        subject->x += -0xcccc;
        actor->x += -0xcccc;
    }
}
