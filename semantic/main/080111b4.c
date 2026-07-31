#include "types.h"

/*
 * The per-frame camera step for the walking map: decay the two camera
 * velocities, notice when the camera crosses a whole-tile boundary on either
 * axis, publish the view matrix, and hand the finished view to the renderer
 * selected by the pointer at Data_03001e50 + 184.
 *
 * Three globals are addressed off one pooled base: 0x03001e80 is loaded at
 * 0x080111c2 and the same register is walked back to 0x03001e6c and
 * 0x03001e70 by the subs at 0x080111cc and 0x080111d2.
 *
 * The mov ip,pc ; bx r3 sequences at 0x08011228, 0x08011234, 0x08011254 and
 * 0x08011260 are ordinary indirect calls to the pooled 0x03000118, the
 * IWRAM-relocated ARM fixed-point multiply, each landing site consuming r0 in
 * the same frame, as recorded for 0x0800ebec.  The movs r0,r0 at 0x08011232
 * and 0x0801125e are inside the instruction stream, aligning the return
 * address that mov ip,pc captures so it lands past the bx; the first call of
 * each pair already has that alignment and carries no nop.
 *
 * The bl Func_080072f0 at 0x08011344 and 0x08011372 is the runtime
 * _call_via_r3 thunk inside 0x080072e4..0x08007320 - index 3 - an indirect
 * call through r3, holding the pooled 0x03000250 and 0x0300013c
 * respectively; 0x0300013c is the ratio helper, so cosine first and sine
 * second yields the tangent.  The bl Func_080072f4 at 0x080113a6 is
 * _call_via_r4, index 4, an indirect call through the renderer pointer.
 *
 * if (v < 0) v += 0xfffff; v >>= 20 at 0x08011266 and 0x08011272 is the
 * truncating fixed-point-to-whole conversion at 20 fractional bits, and the
 * xor against 0x100000 is the "did bit 20 change" boundary test that gates
 * the two edge notifications.
 *
 * The str r1,[sp,#12] at 0x08011300 writes back the advanced walker pointer,
 * which no later instruction reads, so it is a dead store.
 *
 * The epilogue at 0x080113aa unwinds the frame and ends pop {r0}; bx r0, so
 * the owner is void.  Published as a callback at 0x0800916c and 0x08010d3c,
 * with no direct call site.
 */

struct Position_080111b4 {
    s32 x;
    s32 y;
    s32 z;
};

struct State_080111b4 {
    u8 unknown_00[12];
    struct Position_080111b4 position;
};

struct Map_080111b4 {
    struct Position_080111b4 *walker;
    s32 velocity_x;
    s32 velocity_z;
    s32 damping;
    u8 unknown_10[212];
    s32 previous_x;
    s32 previous_z;
    u8 unknown_ec[44];
    u16 heading;
    u16 pitch;
    u8 unknown_11c[556];
    s32 near;
    s32 far;
};

struct Transfer_080111b4 {
    u8 unknown_00[12];
    s32 field0c;
    s32 anchor;
};

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080111b4)(s32 a, s32 b);
/* 0x0300013c, the ratio helper: second argument over first. */
typedef s32 (*Ratio_080111b4)(s32 first, s32 second);
/* 0x03000250, the view publisher. */
typedef void (*Publish_080111b4)(struct Position_080111b4 *vector,
                                 struct State_080111b4 *state);
typedef void (*Render_080111b4)(struct State_080111b4 *state,
                                struct Position_080111b4 *position,
                                void *context,
                                void *surface);

extern struct State_080111b4 *Data_03001e80;
extern void *Data_03001e6c;
extern struct Map_080111b4 *Data_03001e70;
extern struct Transfer_080111b4 Data_03001ce0;
extern s32 Data_03001af4;
extern s32 Data_03001f60;
extern u32 Data_03001e40;
extern u8 Data_03001e50[];

void Func_080114a0(void);
s32 Func_08004458(void);
void Func_08011164(s32 tile);
void Func_080110e0(s32 tile);
void Func_08005258(s32 near, s32 half, s32 twice);
void Func_080049ac(void);
void Func_08004cb4(struct Position_080111b4 *position);
void Func_08004c1c(s32 angle);
void Func_08004bd4(s32 angle);
void Func_080051d8(struct State_080111b4 *state,
                   struct Position_080111b4 *position);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_080123f4(s32 tangent, struct Position_080111b4 *position,
                   void *context);

void Func_080111b4(void)
{
    struct State_080111b4 *state = Data_03001e80;
    void *context = Data_03001e6c;
    struct Map_080111b4 *map = Data_03001e70;
    struct Position_080111b4 *position = &state->position;
    struct Position_080111b4 *walker = map->walker;
    void *surface = (u8 *)context + 3200;
    s32 near = map->near;
    s32 far = map->far;
    Multiply_080111b4 multiply = (Multiply_080111b4)0x03000118;
    struct Position_080111b4 view;
    s32 x;
    s32 z;
    s32 tile_x;
    s32 tile_z;
    s32 previous;

    Func_080114a0();

    if (walker != 0) {
        z = walker->z;
        x = walker->x;

        if (map->velocity_x != 0) {
            s32 first = Func_08004458();
            s32 second = Func_08004458();
            s32 velocity = map->velocity_x;

            x += multiply(velocity, first - second);
            map->velocity_x = multiply(velocity, map->damping);
        }
        if (map->velocity_z != 0) {
            s32 first = Func_08004458();
            s32 second = Func_08004458();
            s32 velocity = map->velocity_z;

            z += multiply(velocity, first - second);
            map->velocity_z = multiply(velocity, map->damping);
        }

        tile_x = (x < 0 ? x + 0xfffff : x) >> 20;
        tile_z = (z < 0 ? z + 0xfffff : z) >> 20;

        previous = map->previous_x;
        if (((previous ^ x) & 0x100000) != 0) {
            if (previous < x)
                Func_08011164(tile_x + 16);
            else
                Func_08011164(tile_x - 16);
        }

        previous = map->previous_z;
        if (((previous ^ z) & 0x100000) != 0) {
            if (previous < z)
                Func_080110e0(tile_z + 12);
            else
                Func_080110e0(tile_z - 18);
        }

        map->previous_x = x;
        map->previous_z = z;
    }

    Data_03001ce0.field0c = 120;
    Data_03001ce0.anchor = 96;
    Func_08005258(near, (far + (s32)((u32)far >> 31)) >> 1, far << 1);

    position->x = walker->x;
    position->y = 0;
    position->z = walker->z;

    Func_080049ac();
    Func_08004cb4(position);
    Func_08004c1c(map->pitch);
    Func_08004bd4(map->heading);

    view.x = 0;
    view.y = 0;
    view.z = far + 0x10000;
    ((Publish_080111b4)0x03000250)(&view, state);

    Func_080049ac();
    Func_080051d8(state, position);

    if (Data_03001af4 != map->heading) {
        s32 cosine = Func_0800231c(map->heading);
        s32 sine = Func_08002322(map->heading);

        Func_080123f4(((Ratio_080111b4)0x0300013c)(cosine, sine),
                      position, context);
        Data_03001f60 = 0;
        Data_03001af4 = map->heading;
    }

    (*(Render_080111b4 *)(Data_03001e50 + 184))(
        state,
        position,
        context,
        (u8 *)surface + (Data_03001e40 & 1) * 5 * 1024);
}
