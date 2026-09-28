.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
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
	bl 0x02008270
	movs r1, #9
	movs r2, #0
	movs r0, #8
	bl 0x020082a0
	movs r0, #40
	bl 0x02008268
	movs r2, #0
	movs r1, #10
	movs r0, #8
	bl 0x020082a0
	movs r0, #40
	bl 0x02008268
	ldr r0, [pc, #80]
	bl 0x020082b0
	movs r0, #8
	movs r1, #0
	bl 0x020082b8
	movs r0, #9
	movs r1, #2
	bl 0x02008290
	movs r1, #2
	movs r0, #10
	bl 0x02008298
	movs r0, #20
	bl 0x02008268
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x020082a8
	movs r0, #20
	bl 0x02008268
	movs r1, #1
	movs r0, #8
	bl 0x02008298
	movs r0, #20
	bl 0x02008268
	movs r0, #8
	movs r1, #0
	bl 0x020082b8
	bl 0x02008278
	pop {r0}
	bx r0
	.4byte 0x0000138a
	.global Func_02000110
	.thumb_func
Func_02000110:
	push {lr}
	bl 0x02008270
	movs r1, #2
	movs r0, #9
	bl 0x02008298
	movs r0, #20
	bl 0x02008268
	ldr r0, [pc, #20]
	bl 0x020082b0
	movs r0, #9
	movs r1, #0
	bl 0x020082b8
	bl 0x02008278
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001388
	.global Func_02000140
	.thumb_func
Func_02000140:
	push {lr}
	bl 0x02008270
	movs r1, #4
	movs r0, #10
	bl 0x02008288
	movs r0, #20
	bl 0x02008268
	ldr r0, [pc, #20]
	bl 0x020082b0
	movs r0, #10
	movs r1, #0
	bl 0x020082b8
	bl 0x02008278
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001389
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {lr}
	bl 0x02008270
	ldr r0, [pc, #20]
	bl 0x020082b0
	movs r0, #8
	movs r1, #0
	bl 0x020082b8
	bl 0x02008278
	pop {r0}
	bx r0
	.4byte 0x0000138e
	.global Func_02000190
	.thumb_func
Func_02000190:
	push {lr}
	bl 0x02008270
	ldr r0, [pc, #20]
	bl 0x020082b0
	movs r0, #9
	movs r1, #0
	bl 0x020082b8
	bl 0x02008278
	pop {r0}
	bx r0
	.4byte 0x0000138c
	.global Func_020001b0
	.thumb_func
Func_020001b0:
	push {lr}
	bl 0x02008270
	ldr r0, [pc, #20]
	bl 0x020082b0
	movs r0, #10
	movs r1, #0
	bl 0x020082b8
	bl 0x02008278
	pop {r0}
	bx r0
	.4byte 0x0000138d
	.global Func_020001d0
	.thumb_func
Func_020001d0:
	push {lr}
	movs r0, #123
	bl 0x020082c8
	movs r0, #1
	bl 0x020082c0
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
	bl 0x02008260
	b .L_020001e4_1
.L_020001e4_0:
	cmp r3, #10
	bne .L_020001e4_2
	movs r0, #8
	bl 0x02008280
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #20
	orrs r3, r2
	strb r3, [r0]
	b .L_020001e4_1
.L_020001e4_2:
	movs r0, #8
	bl 0x02008280
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #20
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl 0x02008280
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl 0x02008280
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
