.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020081a8
	.global Func_02000038
	.thumb_func
Func_02000038:
	movs r0, #0
	bx lr
	.global Func_0200003c
	.thumb_func
Func_0200003c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020081d8
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020081ec
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x020080c0
	movs r0, #48
	bl 0x020080b8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {lr}
	movs r0, #9
	movs r1, #1
	movs r2, #0
	bl 0x020080c0
	movs r0, #68
	bl 0x020080b8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	movs r0, #10
	movs r1, #2
	movs r2, #0
	bl 0x020080c0
	movs r0, #88
	bl 0x020080b8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000094
	.thumb_func
Func_02000094:
	push {lr}
	movs r0, #11
	movs r1, #3
	movs r2, #0
	bl 0x020080c0
	movs r0, #108
	bl 0x020080b8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008264
	.global Func_020000b4
	.thumb_func
Func_020000b4:
	movs r0, #0
	bx lr
	.include "games/THE BROKEN SEAL/SRC/DEBUG/TEST_ROOMS/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x00500050
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00500000
	.4byte 0x00010003
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00780078
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00780000
	.4byte 0x00010003
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x0028003c
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00010002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0028003c
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00010002
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00800080
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00020003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00800080
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00020003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00200020
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00800080
	.4byte 0x00000000
	.4byte 0x00000080
	.4byte 0x00800000
	.4byte 0x00020003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000bf
	.4byte 0x000000c0
	.4byte 0x000000c1
	.4byte 0x000000c2
	.4byte 0x000001ff
	.4byte 0x0030005a
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x0044005b
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x0058005c
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00008000
	.4byte 0x006c005d
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200804d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008065
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200807d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008095
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
