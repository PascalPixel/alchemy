#include "types.h"
#include "battle.h"
#include "graphics.h"
#include "mem.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "palbuf.h"
#include "fxblend.h"
#include "dma.h"
#include "io_reg.h"

void Func_080d67e8(void);

void BattleFx_SetBlock30ValuesMaxZero(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
    state->timer = 0x7fff;
    state->enabled = 0;
}

void BattleFx_SetBlock30Values12Zero(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
    state->timer = 12;
    state->enabled = 0;
}

void BattleFx_SetBlock30Values128One(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
    state->timer = 128;
    state->enabled = 1;
}
