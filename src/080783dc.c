typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
struct State { u8 padding[0x36]; s16 max; s16 pad2; s16 pos; };
struct State *Func_08077394(s32);
void Func_0807822c(s32);
s16 Func_080783dc(s32 o, s32 d){ struct State *s=Func_08077394(o);
 s32 cur=s->pos; s32 mx=s->max; s32 p=cur+d;
 s32 r; if(p>mx) r=mx; else { r=0; if(p>=0) r=p; }
 s->pos=r; Func_0807822c(o); return s->pos; }
