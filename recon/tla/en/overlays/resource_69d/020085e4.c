#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "PARTY_STATE.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "IO_REG.H"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 Data_020026f0;
extern s32 Data_020026f4;
extern u16 Data_0200271c[];
extern u16 Data_0200273c[];
extern u16 Data_02002740[];
extern u32 gFrameCount;

s32 __modsi3();
s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02001e28();
void Func_02001ef8();
void Func_02001e48();
void Func_02001ed0();
void ObjectDispatch_SetSingleChildFlags();
void WaitFrames();
void Motion_CamBounds();
struct FieldActor *Func_02001ea8();
void Func_02001dd8();
void Func_02001f08();
void Func_02001de0();
void Object_AttachWorkTargetToObject();
void Func_02001e50();

void Func_020005e4(void)
{
    u16 colors[16];
    s32 phase;
    s32 i;
    s32 offset;

    phase = (Data_020026f0 / 4) % 5;
    if (Data_020026f4 != 0) {
        u8 *work = Ram_HeapSlots->event_work;

        if (*(s16 *)(work + 358) != 0 && (HALF_AT(work, 358) & 0x3ff) != 153)
            return;
        if (*(s8 *)(work + 0xca4) != 0)
            return;
        if (*(s16 *)(work + 350) != 0)
            return;
    }
    i = 0;
    offset = 4;
next:
    {
        s32 slot = (phase + i) % 5;
        u16 *dst = colors;

        asm volatile("" : "+l"(dst)); /* FAKEMATCH: ⚓️ rebuilds the buffer address each pass */
        dst[slot] = HALF_AT(Data_0200271c, offset);
    }
    i++;
    offset += 2;
    if (i <= 4)
        goto next;
    Dma_Set(colors, (void *)0x05000184, 0x84000002, REG_DMA3);
    Data_020026f0++;
}

void Func_020006a0(s32 mode)
{
    Data_020026f4 = 1;
    if (mode != 2) {
        Iwram_CopyWords(Data_0200271c, (void *)0x05000180, 32);
        Iwram_CopyWords(Data_0200273c, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1) {
        Data_020026f0 = 0;
        Scheduler_AddOrUpdateCallback(Func_020005e4, 1152);
    } else {
        Dma_Set(Data_02002740, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}

void Func_02000724(s32 id)
{
    struct FieldActor *object;
    s32 height;

    object = Object_GetById(id);
    object->motion_flags = 4;
    height = object->terrain_height;
    asm volatile("" : "+l"(height)); /* FAKEMATCH: ⚓️ loads the ground height before it clears the word */
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
    {
        u8 *flags = &object->priority_flags;

        *flags |= 128;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02001e28(object->x, object->z, 0, (height >> 19) + 6);
    Func_02001ef8(object->x, object->z, 0, 128);
}

extern u8 Data_020026f8[];
extern s32 Data_02002718;
void ObjectDispatch_ApplyValueToChildren();
s32 Func_02001ee8();
void Func_02001ed8();
void Func_02001db8();
s32 Math_Cosine();
s32 Math_Sine();

s32 Func_02000964(s32 *a, s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return Iwram_Sqrt(dx * dx + dz * dz);
}

struct EventHold {
    u8 unknown_000[362];
    u16 hold;
};

static inline s32 MapAttr(s32 x, s32 z, s32 layer)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep the argument order */
    return Func_02001ee8(x, z, layer);
}

void Func_02000990(struct FieldActor *ball)
{
    u8 *work;
    struct FieldActor *leader;
    s32 pos[3];
    s32 d;
    s32 near;
    s32 t;

    work = Ram_HeapSlots->event_work;
    leader = Object_GetById(gPartyState.current_owner);
    if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 || *(s16 *)(work + 350) != 0
        || *(s16 *)(work + 360) != 0) {
        ObjectDispatch_ApplyValueToChildren(ball, 0);
        return;
    }
    ObjectDispatch_ApplyValueToChildren(ball, 16);
    t = ball->timer + 1;
    ball->timer = t;
    if ((t & 31) == 31)
        Func_02001f08(231);
    ball->sprite->priority = leader->sprite->priority;
    if (MapAttr(ball->x, ball->z, 2) == 255)
        goto stop;
    pos[0] = ball->x;
    pos[1] = ball->y;
    pos[2] = ball->z;
    Func_02001ed8(pos);
    d = pos[0];
    if (d > 0x1100000)
        goto stop;
    if (d < -0x200000)
        goto stop;
    if (BYTE_AT(ball, 98) == 0) {
        d = leader->y - ball->y;
        if (d < 0)
            d = ball->y - leader->y;
        near = 0;
        if (Func_02000964(&leader->x, &ball->x) <= 12 && d < 0xc0000)
            near = 1;
        if (near && !GameFlag_Test(260)) {
            WORD_AT(leader, 104) = (s32)ball;
            leader->facing = ball->facing;
            leader->priority_flags &= 254;
            ((struct EventHold *)work)->hold = 200;
            gPartyState.render_mode = 0;
            BYTE_AT(ball, 98) = 1;
            BYTE_AT(ball, 99) = 1;
        }
    }
    ball->x += Iwram_MulQ16(Math_Cosine(ball->facing), ball->speed);
    ball->z += Iwram_MulQ16(Math_Sine(ball->facing), ball->speed);
    return;
stop:
    BYTE_AT(ball, 99) = 0;
    WORD_AT(ball, 108) = 0;
    Func_02001db8(ball, Data_020026f8);
    Func_02001f08(228);
    Data_02002718 = 0;
}
