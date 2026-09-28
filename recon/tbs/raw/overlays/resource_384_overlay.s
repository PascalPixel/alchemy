.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000308, 0x02000308
	.set sub_02000310, 0x02000310
	.set sub_02000320, 0x02000320
	.set sub_02000342, 0x02000342
	.set sub_02000344, 0x02000344
	.set sub_02000352, 0x02000352
	.set sub_02000354, 0x02000354
	.set sub_0200035e, 0x0200035e
	.set sub_02000362, 0x02000362
	.set sub_0200036e, 0x0200036e
	.set sub_0200037e, 0x0200037e
	.set sub_02000384, 0x02000384
	.set sub_0200038a, 0x0200038a
	.set sub_0200038c, 0x0200038c
	.set sub_0200038e, 0x0200038e
	.set sub_020003ac, 0x020003ac
	.set sub_020003b4, 0x020003b4
	.set sub_020003ba, 0x020003ba
	.set sub_020003d4, 0x020003d4
	.set sub_020003d8, 0x020003d8
	.set sub_020003dc, 0x020003dc
	.set sub_020003e4, 0x020003e4
	.set sub_020003e8, 0x020003e8
	.set sub_020003fe, 0x020003fe
	.set sub_02000404, 0x02000404
	.set sub_02000408, 0x02000408
	.set sub_02000418, 0x02000418
	.set sub_0200041e, 0x0200041e
	.set sub_02000424, 0x02000424
	.set sub_0200042a, 0x0200042a
	.set sub_0200043a, 0x0200043a
	.set sub_0200043e, 0x0200043e
	.set sub_0200044a, 0x0200044a
	.set sub_0200045a, 0x0200045a
	.set sub_02000466, 0x02000466
	.set sub_0200046a, 0x0200046a
	.set sub_0200047a, 0x0200047a
	.set sub_02000492, 0x02000492
	.set sub_0200049c, 0x0200049c
	.set sub_0200049e, 0x0200049e
	.set sub_020004a4, 0x020004a4
	.set sub_020004b4, 0x020004b4
	.set sub_020004c2, 0x020004c2
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020082d0
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
	.4byte 0x02008348
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	ldr r3, [pc, #24]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	bne .L_02000044_0
	ldr r0, [pc, #12]
	b .L_02000044_1
.L_02000044_0:
	ldr r0, [pc, #12]
.L_02000044_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x020083bc
	.4byte 0x0200835c
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {lr}
	ldr r3, [pc, #24]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	bne .L_0200006c_0
	ldr r0, [pc, #12]
	b .L_0200006c_1
.L_0200006c_0:
	ldr r0, [pc, #12]
.L_0200006c_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x020084a0
	.4byte 0x020083ec
	.global Func_02000094
	.thumb_func
Func_02000094:
	push {lr}
	bl sub_02000308
	movs r1, #9
	movs r2, #0
	movs r0, #8
	bl sub_02000342
	movs r0, #40
	bl sub_02000310
	movs r2, #0
	movs r1, #10
	movs r0, #8
	bl sub_02000352
	movs r0, #40
	bl sub_02000320
	ldr r0, [pc, #80]
	bl sub_0200036e
	movs r0, #8
	movs r1, #0
	bl sub_0200037e
	movs r0, #9
	movs r1, #2
	bl sub_0200035e
	movs r1, #2
	movs r0, #10
	bl sub_0200036e
	movs r0, #20
	bl sub_02000344
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl sub_0200038e
	movs r0, #20
	bl sub_02000354
	movs r1, #1
	movs r0, #8
	bl sub_0200038c
	movs r0, #20
	bl sub_02000362
	movs r0, #8
	movs r1, #0
	bl sub_020003ba
	bl sub_0200037e
	pop {r0}
	bx r0
	.4byte 0x0000138a
	.global Func_02000110
	.thumb_func
Func_02000110:
	push {lr}
	bl sub_02000384
	movs r1, #2
	movs r0, #9
	bl sub_020003b4
	movs r0, #20
	bl sub_0200038a
	ldr r0, [pc, #20]
	bl sub_020003d8
	movs r0, #9
	movs r1, #0
	bl sub_020003e8
	bl sub_020003ac
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001388
	.global Func_02000140
	.thumb_func
Func_02000140:
	push {lr}
	bl sub_020003b4
	movs r1, #4
	movs r0, #10
	bl sub_020003d4
	movs r0, #20
	bl sub_020003ba
	ldr r0, [pc, #20]
	bl sub_02000408
	movs r0, #10
	movs r1, #0
	bl sub_02000418
	bl sub_020003dc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001389
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {lr}
	bl sub_020003e4
	ldr r0, [pc, #20]
	bl sub_0200042a
	movs r0, #8
	movs r1, #0
	bl sub_0200043a
	bl sub_020003fe
	pop {r0}
	bx r0
	.4byte 0x0000138e
	.global Func_02000190
	.thumb_func
Func_02000190:
	push {lr}
	bl sub_02000404
	ldr r0, [pc, #20]
	bl sub_0200044a
	movs r0, #9
	movs r1, #0
	bl sub_0200045a
	bl sub_0200041e
	pop {r0}
	bx r0
	.4byte 0x0000138c
	.global Func_020001b0
	.thumb_func
Func_020001b0:
	push {lr}
	bl sub_02000424
	ldr r0, [pc, #20]
	bl sub_0200046a
	movs r0, #10
	movs r1, #0
	bl sub_0200047a
	bl sub_0200043e
	pop {r0}
	bx r0
	.4byte 0x0000138d
	.global Func_020001d0
	.thumb_func
Func_020001d0:
	push {lr}
	movs r0, #123
	bl sub_0200049e
	movs r0, #1
	bl sub_0200049c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	push {r5, lr}
	ldr r3, [pc, #108]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	ldr r3, [pc, #96]
	subs r2, #71
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_020001e4_0
	ldr r0, [pc, #88]
	bl sub_02000466
	b .L_020001e4_1
.L_020001e4_0:
	cmp r3, #10
	bne .L_020001e4_2
	movs r0, #8
	bl sub_02000492
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #20
	orrs r3, r2
	strb r3, [r0]
	b .L_020001e4_1
.L_020001e4_2:
	movs r0, #8
	bl sub_020004a4
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #20
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl sub_020004b4
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl sub_020004c2
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
.L_020001e4_1:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000012f
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/IMPORT.INC"
AlchemyData_020002d0:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000050
	.4byte 0xc00000e8
	.4byte 0x00100000
	.4byte 0x01300030
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x00000220
	.4byte 0xc0000118
	.4byte 0x01b80000
	.4byte 0x02a80030
	.4byte 0x00000138
	.4byte 0xffff000a
	.4byte 0x00000050
	.4byte 0xc00000e8
	.4byte 0x00100000
	.4byte 0x01300030
	.4byte 0x00000100
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0010c014
	.4byte 0x0020c017
	.4byte 0x0030c068
	.4byte 0x000001ff
	.4byte 0x1855003f
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00004000
	.4byte 0x18550040
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00003000
	.4byte 0x18550040
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x020081d1
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x18550008
	.4byte 0x02008095
	.4byte 0x00000000
	.4byte 0x18550009
	.4byte 0x02008111
	.4byte 0x00000000
	.4byte 0x1855000a
	.4byte 0x02008141
	.4byte 0x00008d15
	.4byte 0x18550008
	.4byte 0x02008171
	.4byte 0x00008d15
	.4byte 0x18550009
	.4byte 0x02008191
	.4byte 0x00008d15
	.4byte 0x1855000a
	.4byte 0x020081b1
	.4byte 0x00000013
	.4byte 0x0f8c0064
	.4byte 0x001000bd
	.4byte 0x00000013
	.4byte 0x0f8d0065
	.4byte 0x001000e2
	.4byte 0x00000013
	.4byte 0x0f8e0066
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0f8f0067
	.4byte 0x001000bb
	.4byte 0x00000013
	.4byte 0x0f900068
	.4byte 0x0020002c
	.4byte 0x00000013
	.4byte 0x0f910069
	.4byte 0x001000bc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001cf6
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001cf7
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
