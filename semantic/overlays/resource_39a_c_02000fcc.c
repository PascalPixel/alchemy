typedef signed int s32;
typedef unsigned short u16;

/* Prepare service zero, set its +6 halfword, and run the follow-up sequence. */
struct SceneService_02000fcc {
    u16 unknown00[3];
    u16 value06;
};

extern void Func_0808a018(void);
extern struct SceneService_02000fcc *Func_0808a080(s32 index);
extern void Func_080f9010(s32 value);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_0808a248(s32 value);

void Func_02000fcc(void)
{
    struct SceneService_02000fcc *service;

    Func_0808a018();
    service = Func_0808a080(0);
    service->value06 = 0x4000;
    Func_080f9010(123);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(1);
}
