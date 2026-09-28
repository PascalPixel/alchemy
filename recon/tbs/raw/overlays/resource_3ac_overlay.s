.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020084f8
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
	.4byte 0x02008630
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200865c
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, [pc, #40]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	bne .L_0200004c_0
	ldr r0, [pc, #28]
	b .L_0200004c_1
.L_0200004c_0:
	ldr r0, [pc, #28]
	bl 0x02008498
	cmp r0, #0
	beq .L_0200004c_2
	ldr r0, [pc, #20]
	b .L_0200004c_1
.L_0200004c_2:
	ldr r0, [pc, #20]
.L_0200004c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008c98
	.4byte 0x00000941
	.4byte 0x02008a64
	.4byte 0x02008824
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {lr}
	movs r0, #0
	bl 0x020084c0
	ldr r2, [pc, #80]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #80]
	cmp r3, r2
	bhi .L_0200008c_0
	ldr r0, [pc, #76]
	bl 0x02008498
	cmp r0, #0
	beq .L_0200008c_0
	movs r0, #8
	movs r1, #17
	bl 0x020084f0
	b .L_0200008c_1
.L_0200008c_0:
	bl 0x020084b0
	ldr r0, [pc, #52]
	bl 0x02008498
	cmp r0, #0
	beq .L_0200008c_2
	ldr r0, [pc, #48]
	bl 0x020084c8
	movs r0, #17
	movs r1, #0
	bl 0x020084d8
	b .L_0200008c_3
.L_0200008c_2:
	ldr r0, [pc, #36]
	bl 0x020084c8
	movs r0, #17
	movs r1, #0
	bl 0x020084d8
.L_0200008c_3:
	bl 0x020084b8
.L_0200008c_1:
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000941
	.4byte 0x000024fb
	.4byte 0x00001bd0
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	bl 0x020084b0
	ldr r0, [pc, #28]
	bl 0x020084c8
	movs r1, #0
	movs r0, #20
	bl 0x020084d8
	movs r0, #148
	lsls r0, r0, #4
	bl 0x020084a0
	bl 0x020084b8
	pop {r0}
	bx r0
	.4byte 0x00001bd5
	.global Func_02000124
	.thumb_func
Func_02000124:
	push {lr}
	bl 0x020084b0
	ldr r0, [pc, #28]
	bl 0x020084c8
	movs r1, #0
	movs r0, #20
	bl 0x020084d0
	movs r0, #148
	lsls r0, r0, #4
	bl 0x020084a0
	bl 0x020084b8
	pop {r0}
	bx r0
	.4byte 0x00001bdb
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {lr}
	bl 0x020084b0
	ldr r0, [pc, #20]
	bl 0x020084c8
	movs r1, #0
	movs r0, #18
	bl 0x020084d8
	bl 0x020084b8
	pop {r0}
	bx r0
	.4byte 0x000024fe
	.global Func_0200016c
	.thumb_func
Func_0200016c:
	push {lr}
	movs r0, #0
	bl 0x020084c0
	ldr r2, [pc, #76]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #76]
	cmp r3, r2
	bhi .L_0200016c_0
	movs r0, #21
	bl 0x020084e8
	b .L_0200016c_1
.L_0200016c_0:
	ldr r0, [pc, #64]
	bl 0x02008498
	cmp r0, #0
	beq .L_0200016c_2
	bl 0x020084b0
	ldr r0, [pc, #56]
	bl 0x020084c8
	movs r0, #21
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
	b .L_0200016c_1
.L_0200016c_2:
	bl 0x020084b0
	ldr r0, [pc, #36]
	bl 0x020084c8
	movs r0, #21
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
.L_0200016c_1:
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000941
	.4byte 0x00002507
	.4byte 0x00001bdc
	.global Func_020001d8
	.thumb_func
Func_020001d8:
	push {lr}
	ldr r0, [pc, #60]
	bl 0x02008498
	cmp r0, #0
	beq .L_020001d8_0
	bl 0x020084b0
	ldr r0, [pc, #48]
	bl 0x020084c8
	movs r0, #16
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
	b .L_020001d8_1
.L_020001d8_0:
	bl 0x020084b0
	ldr r0, [pc, #28]
	bl 0x020084c8
	movs r0, #16
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
.L_020001d8_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x000024fa
	.4byte 0x00001be0
	.global Func_02000224
	.thumb_func
Func_02000224:
	push {lr}
	movs r0, #0
	bl 0x020084c0
	ldr r2, [pc, #80]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #80]
	cmp r3, r2
	bhi .L_02000224_0
	movs r0, #25
	movs r1, #16
	bl 0x020084e0
	b .L_02000224_1
.L_02000224_0:
	ldr r0, [pc, #68]
	bl 0x02008498
	cmp r0, #0
	beq .L_02000224_2
	bl 0x020084b0
	ldr r0, [pc, #56]
	bl 0x020084c8
	movs r0, #16
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
	b .L_02000224_1
.L_02000224_2:
	bl 0x020084b0
	ldr r0, [pc, #36]
	bl 0x020084c8
	movs r0, #16
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
.L_02000224_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000941
	.4byte 0x000024f9
	.4byte 0x00001bcf
	.global Func_02000294
	.thumb_func
Func_02000294:
	push {lr}
	ldr r0, [pc, #44]
	bl 0x02008498
	cmp r0, #0
	beq .L_02000294_0
	ldr r0, [pc, #36]
	bl 0x020084c8
	movs r0, #14
	movs r1, #0
	bl 0x020084d0
	b .L_02000294_1
.L_02000294_0:
	ldr r0, [pc, #24]
	bl 0x020084c8
	movs r0, #14
	movs r1, #0
	bl 0x020084d0
.L_02000294_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x000024f6
	.4byte 0x00001bde
	.global Func_020002d0
	.thumb_func
Func_020002d0:
	push {r5, lr}
	movs r0, #0
	bl 0x020084c0
	ldrh r5, [r0, #6]
	ldr r0, [pc, #72]
	bl 0x02008498
	cmp r0, #0
	beq .L_020002d0_0
	ldr r2, [pc, #64]
	adds r3, r5, r2
	ldr r2, [pc, #64]
	cmp r3, r2
	bhi .L_020002d0_1
	movs r0, #29
	movs r1, #14
	bl 0x020084e0
	b .L_020002d0_2
.L_020002d0_1:
	bl 0x020084b0
	ldr r0, [pc, #48]
	bl 0x020084c8
	movs r0, #14
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
	b .L_020002d0_2
.L_020002d0_0:
	ldr r0, [pc, #32]
	bl 0x020084c8
	movs r0, #14
	movs r1, #0
	bl 0x020084d0
.L_020002d0_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000941
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x000024f5
	.4byte 0x00001bcd
	.global Func_02000338
	.thumb_func
Func_02000338:
	push {lr}
	ldr r0, [pc, #44]
	bl 0x02008498
	cmp r0, #0
	beq .L_02000338_0
	ldr r0, [pc, #36]
	bl 0x020084c8
	movs r0, #15
	movs r1, #0
	bl 0x020084d0
	b .L_02000338_1
.L_02000338_0:
	ldr r0, [pc, #24]
	bl 0x020084c8
	movs r0, #15
	movs r1, #0
	bl 0x020084d0
.L_02000338_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000941
	.4byte 0x000024f8
	.4byte 0x00001bdf
	.global Func_02000374
	.thumb_func
Func_02000374:
	push {r5, lr}
	movs r0, #0
	bl 0x020084c0
	ldrh r5, [r0, #6]
	ldr r0, [pc, #72]
	bl 0x02008498
	cmp r0, #0
	beq .L_02000374_0
	ldr r2, [pc, #64]
	adds r3, r5, r2
	ldr r2, [pc, #64]
	cmp r3, r2
	bhi .L_02000374_1
	movs r0, #30
	movs r1, #15
	bl 0x020084e0
	b .L_02000374_2
.L_02000374_1:
	bl 0x020084b0
	ldr r0, [pc, #48]
	bl 0x020084c8
	movs r0, #15
	movs r1, #0
	bl 0x020084d0
	bl 0x020084b8
	b .L_02000374_2
.L_02000374_0:
	ldr r0, [pc, #32]
	bl 0x020084c8
	movs r0, #15
	movs r1, #0
	bl 0x020084d0
.L_02000374_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000941
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x000024f7
	.4byte 0x00001bce
	.global Func_020003dc
	.thumb_func
Func_020003dc:
	push {r5, r6, lr}
	ldr r3, [pc, #96]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r2, [pc, #92]
	adds r3, r3, r1
	ldr r5, [pc, #92]
	str r2, [r3]
	subs r2, #71
	adds r3, r5, r2
	movs r1, #0
	ldrsh r6, [r3, r1]
	cmp r6, #10
	bne .L_020003dc_0
	ldr r0, [pc, #80]
	bl 0x020084a8
	movs r1, #226
	ldr r2, [pc, #76]
	lsls r1, r1, #1
	adds r3, r5, r1
	strh r2, [r3]
	movs r2, #227
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r6, [r3]
.L_020003dc_0:
	movs r0, #23
	bl 0x020084c0
	movs r1, #0
	bl 0x02008490
	movs r0, #24
	bl 0x020084c0
	movs r1, #0
	bl 0x02008490
	movs r0, #25
	bl 0x020084c0
	movs r1, #0
	bl 0x02008490
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x00000069
	.global Func_02000454
	.thumb_func
Func_02000454:
	push {lr}
	movs r0, #0
	bl 0x020084c0
	ldr r2, [pc, #36]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #36]
	cmp r3, r2
	bhi .L_02000454_0
	movs r0, #21
	bl 0x020084e8
	b .L_02000454_1
.L_02000454_0:
	ldr r0, [pc, #24]
	bl 0x020084c8
	movs r0, #22
	movs r1, #0
	bl 0x020084d0
.L_02000454_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x0000266b
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/IMPORT.INC"
AlchemyData_020004f8:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c0
	.4byte 0xc00000b8
	.4byte 0x00100000
	.4byte 0x01000000
	.4byte 0x000000e0
	.4byte 0xffff0002
	.4byte 0x000001b0
	.4byte 0xc0000108
	.4byte 0x01280000
	.4byte 0x02180030
	.4byte 0x00000120
	.4byte 0xffff0003
	.4byte 0x000000a0
	.4byte 0xc0000248
	.4byte 0x00100000
	.4byte 0x01000190
	.4byte 0x00000270
	.4byte 0xffff0004
	.4byte 0x000001d0
	.4byte 0xc0000268
	.4byte 0x01280000
	.4byte 0x02180180
	.4byte 0x00000280
	.4byte 0xffff0005
	.4byte 0x000002c0
	.4byte 0xc0000248
	.4byte 0x02400000
	.4byte 0x03300160
	.4byte 0x00000260
	.4byte 0xffff0006
	.4byte 0x00000120
	.4byte 0xc0000348
	.4byte 0x00280000
	.4byte 0x01700290
	.4byte 0x000003b0
	.4byte 0xffff0007
	.4byte 0x000002b0
	.4byte 0xc0000118
	.4byte 0x02380000
	.4byte 0x03280020
	.4byte 0x00000130
	.4byte 0xffff0008
	.4byte 0x00000230
	.4byte 0xc0000370
	.4byte 0x01900000
	.4byte 0x02800290
	.4byte 0x00000390
	.4byte 0xffff0009
	.4byte 0x000002a0
	.4byte 0xc0000378
	.4byte 0x02080000
	.4byte 0x02f80290
	.4byte 0x00000380
	.4byte 0xffff000a
	.4byte 0x00000340
	.4byte 0xc0000350
	.4byte 0x02a80000
	.4byte 0x03980290
	.4byte 0x00000380
	.4byte 0xffff000b
	.4byte 0x000002b0
	.4byte 0xc00000b8
	.4byte 0x02380000
	.4byte 0x03280020
	.4byte 0x00000120
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000069
	.4byte 0x00104068
	.4byte 0x00205068
	.4byte 0x00306068
	.4byte 0x00407068
	.4byte 0x00508068
	.4byte 0x00609068
	.4byte 0x0070a068
	.4byte 0x0080b068
	.4byte 0x00a030a9
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00008000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00014000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01930000
	.4byte 0x00000000
	.4byte 0x008a0000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01fc0000
	.4byte 0x00003000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x0000d000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02130000
	.4byte 0x00013000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x02130000
	.4byte 0x00015000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x01f30000
	.4byte 0x00014000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00005000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x0000a000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00014000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x02b00000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00003000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00005000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001bc1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001bc2
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001bc5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001bc6
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001bc9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001bca
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020082d1
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008375
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020082d1
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008375
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0200808d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001bd3
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001bd4
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020080fd
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0200816d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008455
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001bc3
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001bc4
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001bc7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001bc8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001bcb
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001bcc
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x02008295
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x02008339
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x02008295
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x02008339
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x020081d9
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x020081d9
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001bd8
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001bd9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001bda
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x02008125
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001bdd
	.4byte 0x000000d3
	.4byte 0x0f8a0064
	.4byte 0x001000e5
	.4byte 0x00000033
	.4byte 0x0f8b0065
	.4byte 0x001000b6
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029ba
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029bb
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000024e9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000024ea
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000024ed
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000024ee
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000024f1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000024f2
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020082d1
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008375
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020082d1
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008375
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0200808d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0200814d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002501
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002502
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0200816d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000024eb
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000024ec
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000024ef
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000024f0
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000024f3
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000024f4
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x02008295
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x02008339
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x02008295
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x02008339
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x020081d9
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x020081d9
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002503
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002504
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002505
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002506
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002508
	.4byte 0x000000d3
	.4byte 0x0f8a0064
	.4byte 0x001000e5
	.4byte 0x00000033
	.4byte 0x0f8b0065
	.4byte 0x001000b6
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029ba
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029bb
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008455
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000266c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
