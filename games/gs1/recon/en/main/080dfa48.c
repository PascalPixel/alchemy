#include "types.h"

#define FIELD(p, type, off) (*(type *)((u8 *)(p) + (off)))

typedef struct Particle {
    s32 x, y, z, vx, vy, vz, life;
} Particle;
typedef void (*DrawFn)(void *, const void *, s32, s32, s32, s32);
typedef s32 (*CopyFn)(void *, const void *, s32);

extern u16 Data_080ede48[];
extern u8 Value_00000073, Value_00000099, Value_000000bd;
extern u8 Value_000000c2, Value_000000b9, Value_000000bb, Value_000000c0;
void Func_080cd594(s32);
s32 Func_080ed408(s32,s32,s32,s32,s32);
void Func_080e0524(s32,void *,s32,s32);
void Func_080df9d0(void *,void *,s32,s32);
void *Func_08002f40(s32);
void Func_080041d8(void *,s32);
void Func_080df90c(s32,s16,s32);
void **Func_080b5098(s16);
s32 Func_08004458(void);
void Func_080e3980(s16,s32 *);
void Func_080d6888(s16,s32,s32,s32,s32);
void Func_080b5088(s16,s32);
void Func_080b50e8(s32);
void Func_080049ac(void);
void Func_080051d8(s32,s32);
void Func_080e3944(void *,s32 *);
void Func_080e38b8(void *,s32,s32);
void Func_080e155c(s32,s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(void *);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080dfa48(void *object, s32 variant)
{
    void **cache = (void **)0x03001eec;
    void **cursor = cache;
    void *work = *cursor++;
    void *canvas = *cursor;
    void *source = cache[2];
    DrawFn rectangle[2];
    Particle *particle;
    s32 screen[3], origin[3];
    void *actor_sprite;
    s32 frame, i, size, offset;
    s32 facing = *(s32 *)0x03001e80;
    s32 palette_id;

    FIELD(work, void *, 0x7828) = object;
    Func_080cd594(0);
    if (FIELD(object, s32, 4) == 0) {
        Func_080ed408(46,7,7,3,2);
        Func_080ed408(47,7,7,11,2);
    } else {
        Func_080ed408(46,7,7,7,2);
        Func_080ed408(47,7,7,15,2);
    }
    rectangle[0] = (DrawFn)((void **)0x03001e50)[46];
    rectangle[1] = (DrawFn)((void **)0x03001e50)[47];
    Func_080e0524((s32)&Value_00000073, source, 0, 0);
    Func_080e0524((s32)&Value_00000099, work, 1, 0);
    Func_080df9d0(work, (void *)0x02010000, 40, 288);
    Func_080e0524((s32)&Value_000000bd, work, 1, 1);
    switch (variant) {
    case 0: palette_id=(s32)&Value_000000c2; break;
    case 1: palette_id=(s32)&Value_000000b9; break;
    case 2: palette_id=(s32)&Value_000000bb; break;
    default: palette_id=(s32)&Value_000000c0; break;
    }
    ((CopyFn)0x03001388)((void *)0x05000000, Func_08002f40(palette_id), 128);
    FIELD(work,s32,0x7780)=2;
    FIELD(work,s32,0x7784)=75;
    Func_080041d8((void *)0x080cd261,0x480);
    Func_080df90c(FIELD(object,s32,8),FIELD(object,s16,36),10);
    actor_sprite=*Func_080b5098(FIELD(object,s16,36));

    particle=(Particle *)((u8 *)work+0x7080);
    i=0;
    do {
        particle->x=FIELD(actor_sprite,s32,8);
        particle->y=FIELD(actor_sprite,s32,12)+0xa0000;
        particle->z=FIELD(actor_sprite,s32,16);
        particle->vx=(Func_08004458()&0x1ff)<<11;
        particle->vy=(Func_08004458()-64)<<11;
        particle->vz=(Func_08004458()-128)<<11;
        if(particle->x>0) particle->vx=-particle->vx;
        particle->life=((i+((u32)i>>31))>>1)+16;
        i++; particle++;
    } while(i!=64);

    Func_080e3980(FIELD(object,s16,36),origin);
    frame=0;
    do {
        if(frame<=14) {
            Func_080e3980((s16)FIELD(object,s32,8),screen);
            rectangle[0](canvas,work,
                ((screen[0]+((u32)screen[0]>>31))>>1)-16,
                screen[1]-48,40,32);
            rectangle[1](canvas,work,
                ((screen[0]+((u32)screen[0]>>31))>>1)-16,
                screen[1]-16,40,32);
        }
        if(frame==10) {
            Func_080d6888(FIELD(object,s16,36),7,5,0,8);
            Func_080b5088(FIELD(object,s16,36),4);
            Func_080b50e8(134);
            FIELD(work,s32,0x77a8)=8;
        }
        offset=frame-8;
        if((u32)offset<=11) {
            size=(offset+((u32)offset>>31))>>1;
            rectangle[0](canvas,(u8 *)0x02010000+size*0x3c0,
                ((origin[0]+((u32)origin[0]>>31))>>1)-16,
                screen[1]-40,20,48);
        }
        if((u32)offset<=55) {
            Func_080049ac();
            Func_080051d8(facing,facing+12);
            particle=(Particle *)((u8 *)work+0x7080);
            i=0;
            do {
                if(particle->life>0) {
                    Func_080e3944(particle,screen);
                    size=(particle->life>>4)+2;
                    screen[0]>>=1;
                    rectangle[0](canvas,(u8 *)source+Data_080ede48[size-1],
                        screen[0]-((size+((u32)size>>31))>>1),
                        screen[1]-size,size,size*2);
                    Func_080e38b8(particle,60,-0x200);
                    particle->life--;
                }
                i++; particle++;
            } while(i!=64);
        }
        Func_080e155c(8,8);
        Func_080cd52c();
        FIELD(work,s32,0x7824)=1;
        Func_080030f8(1);
        frame++;
    } while(frame!=60);
    Func_08004278((void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
