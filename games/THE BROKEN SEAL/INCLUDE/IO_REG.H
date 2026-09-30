#ifndef GUARD_IO_REG_H
#define GUARD_IO_REG_H

#include "TYPES.H"

/* Game Boy Advance I/O registers at their hardware addresses, named as the
   public hardware documentation names them. */

#define REG_DISPCNT (*(volatile u16 *)0x04000000)
#define REG_VCOUNT (*(volatile u16 *)0x04000006)
#define REG_BG2CNT (*(volatile u16 *)0x0400000c)

#define REG_BG2PA (*(volatile u16 *)0x04000020)
#define REG_BG2PB (*(volatile u16 *)0x04000022)
#define REG_BG2PC (*(volatile u16 *)0x04000024)
#define REG_BG2PD (*(volatile u16 *)0x04000026)
#define REG_BG2X (*(volatile s32 *)0x04000028)
#define REG_BG2Y (*(volatile s32 *)0x0400002c)

#define REG_WIN0H (*(volatile u16 *)0x04000040)
#define REG_WIN1H (*(volatile u16 *)0x04000042)
#define REG_WIN0V (*(volatile u16 *)0x04000044)
#define REG_WIN1V (*(volatile u16 *)0x04000046)
#define REG_WININ (*(volatile u16 *)0x04000048)
#define REG_WINOUT (*(volatile u16 *)0x0400004a)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

/* BG2's screen block in the canvas layout. */
#define BG_CANVAS_MAP ((volatile s16 *)0x06003800)

#define REG_DMA3 ((volatile u32 *)0x040000d4)

/* The first backdrop colour of the palette RAM. */
#define PLTT_BACKDROP (*(volatile u16 *)0x05000000)

#define REG_TM3CNT (*(volatile u32 *)0x0400010c)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e)

#define REG_SIODATA32 ((volatile u32 *)0x04000120)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)
#define REG_SIOCNT16 (*(volatile u16 *)0x04000128)
#define REG_SIODATA8 (*(volatile u16 *)0x0400012a)
#define REG_KEYCNT (*(volatile u16 *)0x04000132)
#define REG_RCNT (*(volatile u16 *)0x04000134)

#define REG_IE (*(volatile u16 *)0x04000200)
#define REG_IF (*(volatile u16 *)0x04000202)
#define REG_WAITCNT (*(volatile u16 *)0x04000204)
#define REG_IME (*(volatile u16 *)0x04000208)

/* KEYCNT: interrupt when A, B, Select and Start are all held (soft reset). */
#define KEYCNT_SOFT_RESET 0xc00f

/* WAITCNT: game pak wait states 3,1 with the prefetch buffer on. */
#define WAITCNT_GAME 0x4014

#endif
