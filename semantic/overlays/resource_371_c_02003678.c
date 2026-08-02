typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct Selection_02003678 {
    u8 reserved000[500];
    s32 actor_id;
};

struct Actor_02003678 {
    u8 reserved00[6];
    u16 presentation;
};

struct SceneWork_02003678 {
    u8 reserved000[386];
    u16 state_one_marker;
};

extern struct Selection_02003678 Data_02000240;
extern struct SceneWork_02003678 *Data_03001ebc;
extern volatile s32 Data_03001e40;
extern struct Actor_02003678 *Func_0808a080(s32 actor);
extern s32 Func_080770e0(s32 state);
extern s32 Func_080770c0(s32 flag);
extern void Func_080770e8(s32 state, s32 value);

void Func_02003678(void)
{
    struct Actor_02003678 *actor;
    s32 progress;

    actor = Func_0808a080(Data_02000240.actor_id);
    actor->presentation = (u16)(Data_03001e40 << 12);

    progress = Func_080770e0(0x2f8);
    if (progress != 0) {
        if (progress == 1) {
            Data_03001ebc->state_one_marker = 99;
        } else if (Func_080770c0(0x106) == 0) {
            progress -= 1;
        }
    }
    Func_080770e8(0x2f8, progress);
}
