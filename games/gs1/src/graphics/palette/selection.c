#include "types.h"

extern u8 Data_02000240[];

void GraphicsPalette_DecrementSelectionWrap(void *base)
{
    s32 v;
    u16 t;
    s32 cur;

    base = (u8 *)base + 0x574;
    v = *(u16 *)base;
    t = v;
    cur = t;

    if (cur == 0) {
        cur = 2;
    } else {
        cur = v + 0xFFFF;
    }
    *(u16 *)base = cur;
}

void Menu_AdvanceWorkspaceIndexModulo3(void *arg0)
{
  unsigned int zero;
  unsigned long cnt;
  cnt = 1 + (*((u16 *)(0x574 + ((u8 *)arg0))));
  zero = 0U;
  *((u16 *)(((u8 *)arg0) + 0x574)) = cnt;
  if (((u32)(cnt << 0x10)) >= (((unsigned long) 0x20000U) + 1))
  {
    *((u16 *)(((u8 *)arg0) + 0x574)) = zero;
  }
}

void GraphicsPalette_DecrementSelectedCounter(s32 work)
{
    u8 *p;
    u16 sel;
    s32 off;

    sel = *(u16 *)((u8 *)work + 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        p = &Data_02000240[off];
        break;
    case 1:
        off = 0x205;
        p = &Data_02000240[off];
        break;
    case 2:
        off = 0x206;
        p = &Data_02000240[off];
        break;
    default:
        return;
    }
    if (*p) {
        (*p)--;
    }
}

void GraphicsPalette_AdjustSelectionCounter(s32 arg0)
{
    u8 *sp;
    u16 sel;
    s32 off;

    sel = *(u16 *)((u8 *)arg0 + 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        sp = &Data_02000240[off];
        if (*sp <= 1) {
            break;
        }
        return;
    case 1:
        off = 0x205;
        sp = &Data_02000240[off];
        if (*sp <= 23) {
            break;
        }
        return;
    case 2:
        off = 0x206;
        sp = &Data_02000240[off];
        if (*sp <= 14) {
            break;
        }
        return;
    default:
        return;
    }
    (*sp)++;
}

void GraphicsPalette_SelectionNoOp(void)
{
}
