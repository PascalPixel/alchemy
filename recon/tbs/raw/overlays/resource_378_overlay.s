.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl 0x0200b610
	adds r2, r0, #0
	ldr r3, [r5, #16]
	ldr r0, [r2, #16]
	ldr r1, [r2, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x0200b538
	strh r0, [r5, #6]
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200baa8
	.global Func_02000064
	.thumb_func
Func_02000064:
	movs r0, #0
	bx lr
	.global Func_02000068
	.thumb_func
Func_02000068:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200bbc8
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {lr}
	ldr r3, [pc, #188]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #34
	bhi .L_02000070_0
	ldr r2, [pc, #172]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r0, [r3, #8]
	lsls r0, r0, #8
	strh r0, [r3, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r4, [r3, #8]
	lsls r0, r0, #8
	strh r4, [r3, #8]
	lsls r0, r0, #8
	strh r4, [r3, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r4, #8]
	lsls r0, r0, #8
	strh r0, [r4, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r4, [r4, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r4, [r4, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r0, [r5, #8]
	lsls r0, r0, #8
	strh r4, [r3, #8]
	lsls r0, r0, #8
	ldr r0, [pc, #28]
	b .L_02000070_1
	.2byte 0x4807
	.2byte 0xe004
	.2byte 0x4807
	.2byte 0xe002
	.2byte 0x4807
	.2byte 0xe000
.L_02000070_0:
	ldr r0, [pc, #28]
.L_02000070_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200808c
	.4byte 0x0200bc0c
	.2byte 0xbccc
	.2byte 0x0200
	.2byte 0xbd2c
	.2byte 0x0200
	.2byte 0xbe04
	.2byte 0x0200
	.4byte 0x0200bbf4
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {lr}
	ldr r3, [pc, #248]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #10
	cmp r3, #40
	bhi .L_0200014c_0
	ldr r2, [pc, #232]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r4, [r1, #16]
	lsls r0, r0, #8
	strh r0, [r2, #16]
	lsls r0, r0, #8
	strh r4, [r1, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r2, #16]
	lsls r0, r0, #8
	strh r4, [r2, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r3, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r0, [r3, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r0, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r4, #16]
	lsls r0, r0, #8
	strh r4, [r2, #16]
	lsls r0, r0, #8
	ldr r0, [pc, #64]
	b .L_0200014c_1
	.2byte 0x4810
	.2byte 0xe016
	.2byte 0x4810
	.2byte 0xe014
	.2byte 0x4810
	.2byte 0xe012
	.2byte 0x4810
	.2byte 0xe010
	.2byte 0x4810
	.2byte 0xe00e
.L_0200014c_0:
	ldr r0, [pc, #64]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_0200014c_2
	ldr r0, [pc, #60]
	b .L_0200014c_1
.L_0200014c_2:
	ldr r0, [pc, #60]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_0200014c_3
	ldr r0, [pc, #52]
	b .L_0200014c_1
.L_0200014c_3:
	ldr r0, [pc, #52]
.L_0200014c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008168
	.4byte 0x0200be70
	.2byte 0xbec4
	.2byte 0x0200
	.2byte 0xbf0c
	.2byte 0x0200
	.2byte 0xc0ec
	.2byte 0x0200
	.2byte 0xc038
	.2byte 0x0200
	.2byte 0xc080
	.2byte 0x0200
	.4byte 0x0000087a
	.4byte 0x0200bfd8
	.4byte 0x00000815
	.4byte 0x0200bf78
	.4byte 0x0200be34
	.global Func_0200027c
	.thumb_func
Func_0200027c:
	push {lr}
	bl 0x0200b5f0
	ldr r0, [pc, #84]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_0200027c_0
	ldr r0, [pc, #76]
	bl 0x0200b6a8
	b .L_0200027c_1
.L_0200027c_0:
	ldr r0, [pc, #72]
	bl 0x0200b6a8
.L_0200027c_1:
	ldr r3, [pc, #72]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	bne .L_0200027c_2
	ldr r0, [pc, #60]
	bl 0x0200b6a8
.L_0200027c_2:
	movs r0, #9
	movs r1, #1
	bl 0x0200b660
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200b698
	movs r0, #2
	bl 0x0200b5e8
	movs r0, #9
	movs r1, #0
	bl 0x0200b6b8
	bl 0x0200b5f8
	pop {r0}
	bx r0
	.4byte 0x00000855
	.4byte 0x00001377
	.4byte 0x00001289
	.4byte 0x02000240
	.4byte 0x00001ce9
	.global Func_020002ec
	.thumb_func
Func_020002ec:
	push {lr}
	bl 0x0200b5f0
	ldr r0, [pc, #88]
	bl 0x0200b5d0
	cmp r0, #0
	bne .L_020002ec_0
	ldr r0, [pc, #80]
	bl 0x0200b6a8
	b .L_020002ec_1
.L_020002ec_0:
	ldr r0, [pc, #76]
	bl 0x0200b6a8
.L_020002ec_1:
	ldr r3, [pc, #76]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	bne .L_020002ec_2
	ldr r0, [pc, #64]
	bl 0x0200b6a8
.L_020002ec_2:
	movs r0, #9
	bl 0x0200b630
	movs r1, #1
	movs r0, #9
	bl 0x0200b660
	movs r0, #2
	bl 0x0200b5e8
	movs r0, #9
	movs r1, #0
	bl 0x0200b6b8
	movs r0, #9
	movs r1, #2
	bl 0x0200b620
	bl 0x0200b5f8
	pop {r0}
	bx r0
	.4byte 0x00000855
	.4byte 0x0000128b
	.4byte 0x00001379
	.4byte 0x02000240
	.4byte 0x00001ceb
	.global Func_02000360
	.thumb_func
Func_02000360:
	push {lr}
	bl 0x0200b5f0
	ldr r0, [pc, #380]
	bl 0x0200b6a8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b0
	movs r0, #0
	movs r1, #0
	bl 0x0200b608
	cmp r0, #0
	bne .L_02000360_0
	movs r0, #8
	movs r1, #3
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	b .L_02000360_1
.L_02000360_0:
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b0
	movs r0, #0
	movs r1, #0
	bl 0x0200b608
	cmp r0, #0
	beq .L_02000360_2
	b .L_02000360_1
.L_02000360_2:
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b0
	movs r0, #0
	movs r1, #0
	bl 0x0200b608
	cmp r0, #0
	beq .L_02000360_3
	b .L_02000360_1
.L_02000360_3:
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #8
	bl 0x0200b610
	movs r2, #160
	ldrh r3, [r0, #6]
	lsls r2, r2, #8
	cmp r3, r2
	bcc .L_02000360_4
	movs r0, #8
	bl 0x0200b610
	movs r2, #224
	ldrh r3, [r0, #6]
	lsls r2, r2, #8
	cmp r3, r2
	bhi .L_02000360_4
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200b618
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #8
	bl 0x0200b610
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #152
	movs r2, #120
	movs r0, #8
	bl 0x0200b648
	movs r0, #1
	bl 0x0200b5e8
	movs r0, #8
	bl 0x0200b610
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #168
	movs r2, #120
	bl 0x0200b648
	movs r1, #192
	movs r2, #168
	movs r0, #0
	bl 0x0200b640
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #168
	movs r2, #120
	bl 0x0200b648
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b6c0
	movs r0, #0
	bl 0x0200b650
	b .L_02000360_5
.L_02000360_4:
	movs r1, #192
	movs r2, #168
	movs r0, #0
	bl 0x0200b640
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b6c0
	movs r0, #0
	bl 0x0200b650
.L_02000360_5:
	bl 0x0200987c
	movs r1, #0
	movs r0, #0
	bl 0x0200b708
	movs r0, #120
	bl 0x0200b710
	movs r0, #120
	bl 0x0200b5e8
	movs r0, #86
	bl 0x0200b730
	bl 0x0200b738
	movs r0, #159
	lsls r0, r0, #4
	bl 0x0200b5d8
	movs r0, #30
	bl 0x0200b700
.L_02000360_1:
	bl 0x0200b5f8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001164
	.global Func_020004e8
	.thumb_func
Func_020004e8:
	push {r5, lr}
	sub sp, #28
	bl 0x0200b5f0
	ldr r0, [pc, #116]
	bl 0x0200b600
	movs r0, #1
	bl 0x0200b528
	ldr r0, [pc, #108]
	bl 0x0200b6a8
	movs r1, #0
	movs r0, #9
	bl 0x0200b6b0
	movs r0, #0
	movs r1, #0
	bl 0x0200b608
	cmp r0, #0
	bne .L_020004e8_0
	movs r0, #9
	movs r1, #0
	bl 0x0200b6b8
	b .L_020004e8_1
.L_020004e8_0:
	ldr r3, [pc, #76]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #1
	movs r3, #3
	movs r2, #7
	movs r1, #16
	movs r4, #14
	str r0, [sp, #0]
	str r3, [sp, #4]
	str r2, [sp, #8]
	str r0, [sp, #16]
	movs r5, #0
	movs r0, #2
	movs r2, #1
	movs r3, #24
	str r1, [sp, #12]
	str r4, [sp, #20]
	str r5, [sp, #24]
	bl 0x0200b6c8
	movs r0, #9
	movs r1, #0
	bl 0x0200b6b8
.L_020004e8_1:
	bl 0x0200b5f8
	sub sp, #-28
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200bc9c
	.4byte 0x00001bfd
	.4byte 0x03001ebc
	.global Func_02000574
	.thumb_func
Func_02000574:
	push {lr}
	bl 0x0200b5f0
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	ldr r0, [pc, #56]
	bl 0x0200b6a8
	movs r1, #0
	movs r0, #10
	bl 0x0200b6b0
	movs r0, #0
	movs r1, #0
	bl 0x0200b608
	cmp r0, #1
	bne .L_02000574_0
	ldr r3, [pc, #32]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000574_0:
	movs r0, #10
	movs r1, #0
	bl 0x0200b6b8
	bl 0x0200b5f8
	pop {r0}
	bx r0
	.4byte 0x0000119f
	.4byte 0x03001ebc
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	push {lr}
	ldr r3, [pc, #252]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #128
	adds r2, #73
	str r2, [r3]
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b708
	movs r0, #1
	bl 0x0200b710
	movs r0, #1
	bl 0x0200b5e8
	ldr r3, [pc, #220]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #10
	cmp r3, #25
	bhi .L_020005cc_0
	ldr r2, [pc, #204]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r4, [r6, #50]
	lsls r0, r0, #8
	strh r4, [r6, #50]
	lsls r0, r0, #8
	strh r4, [r6, #50]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r1, #52]
	lsls r0, r0, #8
	strh r0, [r5, #52]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r0, [r4, #52]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r0, [r4, #52]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r6, [r0, #54]
	lsls r0, r0, #8
	strh r0, [r4, #52]
	lsls r0, r0, #8
	ldr r0, [pc, #96]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_020005cc_1
	movs r1, #200
	movs r2, #160
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x0200b658
	b .L_020005cc_1
	.2byte 0xf002
	.2byte 0xfdef
	.2byte 0x4812
	.2byte 0xf002
	.2byte 0xff9c
	.2byte 0x2800
	.2byte 0xd101
	.2byte 0xf000
	.2byte 0xf8f6
.L_020005cc_1:
	ldr r0, [pc, #60]
	bl 0x0200b5e0
	b .L_020005cc_0
	.2byte 0xf002
	.2byte 0xfde2
	.2byte 0x480d
	.2byte 0xf002
	.2byte 0xff93
	.2byte 0x480a
	.2byte 0xf002
	.2byte 0xff8c
	.2byte 0x2800
	.2byte 0xd101
	.2byte 0xf000
	.2byte 0xf8e6
	.2byte 0x4807
	.2byte 0xf002
	.2byte 0xff8d
.L_020005cc_0:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0200860c
	.4byte 0x00000855
	.2byte 0x0109
	.2byte 0x0000
	.4byte 0x0000012f
	.2byte 0x0201
	.2byte 0x0000
	.global Func_020006e8
	.thumb_func
Func_020006e8:
	push {lr}
	bl 0x0200b5f0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200b6e0
	movs r1, #1
	movs r0, #1
	bl 0x0200b6f8
	bl 0x0200b6f0
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	ldr r0, [pc, #28]
	bl 0x0200b6a8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200b5d8
	bl 0x0200b5f8
	pop {r0}
	bx r0
	.4byte 0x0000116c
	.global Func_0200074c
	.thumb_func
Func_0200074c:
	push {lr}
	bl 0x0200b5f0
	movs r0, #0
	movs r1, #0
	bl 0x0200b660
	movs r0, #1
	movs r1, #0
	bl 0x0200b660
	movs r0, #11
	movs r1, #0
	bl 0x0200b660
	movs r0, #12
	movs r1, #0
	bl 0x0200b660
	movs r0, #8
	movs r1, #0
	bl 0x0200b660
	movs r0, #9
	movs r1, #0
	bl 0x0200b660
	movs r0, #10
	movs r1, #0
	bl 0x0200b660
	movs r1, #0
	ldr r0, [pc, #108]
	bl 0x0200b708
	movs r0, #120
	bl 0x0200b710
	movs r0, #180
	bl 0x0200b5e8
	ldr r3, [pc, #96]
	ldr r0, [pc, #96]
	ldr r2, [r3]
	ldr r4, [pc, #96]
	movs r1, #248
	adds r3, r2, r0
	lsls r1, r1, #7
	strh r1, [r3]
	adds r3, r2, r4
	adds r4, #2
	strh r1, [r3]
	adds r3, r2, r4
	strh r1, [r3]
	movs r1, #168
	ldr r0, [pc, #60]
	lsls r1, r1, #6
	ldr r4, [pc, #76]
	adds r3, r2, r1
	strb r0, [r3]
	ldr r0, [pc, #72]
	adds r3, r2, r4
	movs r1, #1
	strb r1, [r3]
	adds r3, r2, r0
	strb r1, [r3]
	ldr r3, [pc, #64]
	adds r2, r2, r3
	strb r1, [r2]
	movs r0, #1
	bl 0x0200b5e8
	movs r2, #0
	ldr r0, [pc, #56]
	movs r1, #1
	bl 0x0200b5a0
	movs r1, #0
	movs r0, #0
	bl 0x0200b708
	movs r0, #120
	bl 0x0200b710
	movs r0, #120
	b .L_0200074c_0
	.4byte 0x00000000
	.4byte 0x00010002
	.4byte 0x03001ed0
	.4byte 0x00000e5a
	.4byte 0x00000e5c
	.4byte 0x00002a01
	.4byte 0x00002a02
	.4byte 0x00002a03
	.4byte 0x0000116d
.L_0200074c_0:
	bl 0x0200b5e8
	movs r0, #60
	bl 0x0200b5e8
	bl 0x0200a7d4
	cmp r0, #0
	bne .L_0200074c_1
	bl 0x0200b5f8
	movs r0, #20
	bl 0x0200b700
	b .L_0200074c_2
.L_0200074c_1:
	bl 0x0200b5f8
	movs r0, #50
	bl 0x0200b700
.L_0200074c_2:
	pop {r0}
	bx r0
	.global Func_02000848
	.thumb_func
Func_02000848:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #0
	bl 0x0200b610
	ldr r3, [r0, #80]
	ldr r4, [r6, #80]
	ldrb r3, [r3, #9]
	movs r5, #13
	ldrb r1, [r4, #9]
	negs r5, r5
	movs r2, #12
	ands r2, r3
	adds r3, r5, #0
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	movs r0, #0
	bl 0x0200b610
	ldr r3, [r0, #80]
	ldr r1, [r6, #80]
	ldrb r2, [r3, #9]
	movs r3, #12
	ands r3, r2
	ldrb r2, [r1, #21]
	ands r5, r2
	orrs r5, r3
	strb r5, [r1, #21]
	movs r0, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_0200088c
	.thumb_func
Func_0200088c:
	push {r5, lr}
	bl 0x0200b5f0
	ldr r0, [pc, #692]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_0200088c_0
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b698
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r0, #192
	movs r1, #1
	movs r2, #160
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200b6e8
	bl 0x0200b6f0
	ldr r3, [pc, #604]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #64
	str r3, [r2]
	bl 0x0200b718
	bl 0x0200b720
	movs r0, #120
	bl 0x0200b5e8
	bl 0x020097ca
.L_0200088c_0:
	movs r1, #0
	ldr r0, [pc, #564]
	bl 0x0200b708
	movs r0, #1
	bl 0x0200b710
	movs r0, #1
	bl 0x0200b5e8
	movs r0, #192
	movs r1, #1
	movs r2, #160
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200b6e8
	bl 0x0200b6f0
	ldr r5, [pc, #520]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	bl 0x0200b718
	bl 0x0200b720
	bl 0x0200a90c
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b708
	movs r0, #60
	bl 0x0200b710
	movs r0, #100
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	bl 0x0200b660
	movs r1, #1
	movs r0, #1
	bl 0x0200b660
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #1
	bl 0x0200b620
	movs r0, #12
	movs r1, #1
	bl 0x0200b620
	movs r0, #0
	ldr r1, [pc, #416]
	ldr r2, [pc, #416]
	bl 0x0200b618
	movs r0, #1
	ldr r1, [pc, #404]
	ldr r2, [pc, #408]
	bl 0x0200b618
	movs r0, #11
	ldr r1, [pc, #396]
	ldr r2, [pc, #396]
	bl 0x0200b618
	movs r0, #12
	ldr r1, [pc, #384]
	ldr r2, [pc, #388]
	bl 0x0200b618
	movs r0, #9
	ldr r1, [pc, #376]
	ldr r2, [pc, #376]
	bl 0x0200b618
	movs r0, #10
	ldr r1, [pc, #364]
	ldr r2, [pc, #368]
	bl 0x0200b618
	ldr r2, [pc, #360]
	ldr r1, [pc, #356]
	movs r0, #8
	bl 0x0200b618
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #12
	bl 0x0200b688
	ldr r0, [pc, #340]
	bl 0x0200b6a8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #12
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #11
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #11
	movs r2, #0
	bl 0x0200b690
	movs r1, #11
	movs r2, #0
	movs r0, #1
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b0
	movs r0, #0
	movs r1, #0
	bl 0x0200b608
	cmp r0, #0
	bne .L_0200088c_1
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #11
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #11
	movs r1, #0
	bl 0x0200b6b8
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200088c_2
.L_0200088c_1:
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #11
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #11
	movs r1, #0
	bl 0x0200b6b8
.L_0200088c_2:
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #9
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #9
	movs r2, #0
	bl 0x0200b690
	movs r1, #9
	movs r2, #0
	movs r0, #11
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #9
	bl 0x0200b6b0
	movs r0, #0
	movs r1, #0
	bl 0x0200b608
	cmp r0, #0
	bne .L_0200088c_3
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #9
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #9
	movs r1, #0
	bl 0x0200b6b8
	ldr r3, [pc, #20]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200088c_4
	.4byte 0x00000201
	.4byte 0x03001ebc
	.4byte 0x00010002
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00001138
.L_0200088c_3:
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #9
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	ldr r3, [pc, #964]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #9
	movs r1, #0
	bl 0x0200b6b8
.L_0200088c_4:
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r0, #1
	movs r1, #3
	bl 0x0200b660
	movs r0, #11
	movs r1, #3
	bl 0x0200b660
	movs r0, #12
	movs r1, #3
	bl 0x0200b660
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b668
	movs r0, #30
	bl 0x0200b5e8
	ldr r1, [pc, #884]
	movs r2, #0
	movs r0, #12
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #8
	movs r2, #0
	movs r0, #12
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #120
	movs r1, #224
	movs r0, #12
	bl 0x0200b648
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #11
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #3
	bl 0x0200b688
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #12
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #12
	bl 0x0200b6c0
	movs r0, #60
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #8
	movs r0, #12
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #12
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #8
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #12
	bl 0x0200b6c0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #8
	movs r2, #0
	movs r0, #12
	bl 0x0200b690
	movs r0, #50
	bl 0x0200b5e8
	ldr r1, [pc, #528]
	movs r2, #0
	movs r0, #12
	bl 0x0200b6d0
	movs r0, #40
	bl 0x0200b5e8
	movs r0, #0
	ldr r1, [pc, #508]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #1
	ldr r1, [pc, #500]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #11
	ldr r1, [pc, #488]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #9
	ldr r1, [pc, #480]
	movs r2, #0
	bl 0x0200b6d0
	movs r2, #0
	ldr r1, [pc, #468]
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #8
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #1
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #1
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #129
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200b6d0
	movs r0, #0
	movs r1, #1
	bl 0x0200b680
	movs r0, #1
	movs r1, #1
	bl 0x0200b680
	movs r0, #11
	movs r1, #1
	bl 0x0200b680
	movs r0, #9
	movs r1, #1
	bl 0x0200b680
	movs r1, #1
	movs r0, #10
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #8
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #0
	bl 0x0200b688
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #12
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #8
	movs r0, #12
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #11
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #40
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #2
	bl 0x0200b688
	movs r0, #8
	movs r1, #0
	bl 0x0200b660
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #8
	movs r5, #144
	bl 0x0200b6a0
	lsls r5, r5, #5
	bl 0x0200a5c4
	movs r0, #196
	bl 0x0200b730
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200a750
	movs r0, #32
	bl 0x0200b5e8
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200a750
	movs r0, #16
	bl 0x0200b5e8
	movs r5, #0
	b .L_0200088c_5
	.4byte 0x03001ebc
	.4byte 0x00000101
.L_0200088c_5:
	movs r1, #144
	movs r0, #8
	lsls r1, r1, #5
	bl 0x0200a750
	adds r5, #1
	movs r0, #8
	bl 0x0200b5e8
	cmp r5, #5
	bls .L_0200088c_5
	movs r5, #144
	lsls r5, r5, #5
	movs r0, #8
	bl 0x0200b5e8
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200a750
	movs r0, #32
	bl 0x0200b5e8
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200a750
	movs r0, #96
	bl 0x0200b5e8
	movs r0, #32
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6a0
	movs r0, #30
	bl 0x0200b5e8
	bl 0x0200a660
	movs r0, #8
	movs r1, #1
	bl 0x0200b660
	movs r1, #2
	movs r0, #8
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200b6d8
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #10
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #10
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #1
	movs r0, #0
	bl 0x0200b698
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #1
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #1
	movs r0, #0
	bl 0x0200b698
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r0, #1
	movs r1, #3
	bl 0x0200b660
	movs r2, #0
	movs r1, #10
	movs r0, #9
	bl 0x0200b698
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b660
	movs r0, #10
	bl 0x0200b670
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #11
	movs r2, #0
	bl 0x0200b698
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b698
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r0, #11
	movs r1, #3
	bl 0x0200b660
	movs r0, #1
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #12
	bl 0x0200b660
	movs r0, #12
	bl 0x0200b670
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #2
	bl 0x0200b688
	movs r0, #8
	movs r1, #0
	bl 0x0200b660
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200b6a0
	bl 0x0200a5c4
	movs r0, #196
	bl 0x0200b730
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200a750
	movs r0, #32
	bl 0x0200b5e8
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200a750
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl 0x0200b690
	movs r0, #16
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	bl 0x0200b680
	movs r0, #1
	movs r1, #1
	bl 0x0200b680
	movs r0, #11
	movs r1, #1
	bl 0x0200b680
	movs r0, #12
	movs r1, #1
	bl 0x0200b680
	movs r0, #9
	movs r1, #1
	bl 0x0200b680
	movs r0, #10
	movs r1, #1
	bl 0x0200b680
	movs r5, #0
.L_0200088c_6:
	movs r1, #144
	movs r0, #8
	lsls r1, r1, #5
	bl 0x0200a750
	adds r5, #1
	movs r0, #8
	bl 0x0200b5e8
	cmp r5, #5
	bls .L_0200088c_6
	movs r5, #144
	movs r0, #8
	lsls r5, r5, #5
	bl 0x0200b5e8
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200a750
	movs r0, #32
	bl 0x0200b5e8
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200a750
	movs r0, #128
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6a0
	movs r0, #30
	bl 0x0200b5e8
	bl 0x0200a660
	movs r0, #8
	movs r1, #1
	bl 0x0200b660
	movs r1, #2
	movs r0, #8
	bl 0x0200b688
	movs r0, #30
	bl 0x0200b5e8
	ldr r1, [pc, #1016]
	movs r2, #0
	movs r0, #12
	bl 0x0200b6d0
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #10
	movs r2, #0
	movs r0, #9
	bl 0x0200b698
	movs r0, #40
	bl 0x0200b5e8
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r0, #10
	movs r1, #8
	bl 0x0200b690
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #11
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	bl 0x0200b680
	movs r0, #1
	movs r1, #1
	bl 0x0200b680
	movs r2, #0
	movs r0, #0
	movs r1, #1
	bl 0x0200b698
	movs r0, #9
	movs r1, #1
	bl 0x0200b680
	movs r0, #10
	movs r1, #1
	bl 0x0200b680
	movs r2, #0
	movs r0, #9
	movs r1, #10
	bl 0x0200b698
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200b6d8
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #1
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	ldr r1, [pc, #756]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #1
	ldr r1, [pc, #744]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #11
	ldr r1, [pc, #736]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #12
	ldr r1, [pc, #724]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #9
	ldr r1, [pc, #716]
	movs r2, #0
	bl 0x0200b6d0
	movs r2, #0
	ldr r1, [pc, #704]
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r0, #1
	movs r1, #3
	bl 0x0200b660
	movs r0, #11
	movs r1, #3
	bl 0x0200b660
	movs r0, #12
	movs r1, #3
	bl 0x0200b660
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b660
	movs r0, #10
	bl 0x0200b670
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #12
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #12
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #50
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #80
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #11
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	ldr r1, [pc, #380]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #1
	ldr r1, [pc, #368]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #11
	ldr r1, [pc, #360]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #12
	ldr r1, [pc, #348]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #9
	ldr r1, [pc, #340]
	movs r2, #0
	bl 0x0200b6d0
	movs r2, #0
	ldr r1, [pc, #328]
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #80
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #8
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200b6d8
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #12
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #8
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #40
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	movs r2, #0
	bl 0x0200b698
	movs r0, #9
	movs r1, #10
	movs r2, #0
	bl 0x0200b698
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl 0x0200b690
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #11
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #200
	movs r2, #136
	bl 0x0200b648
	movs r0, #0
	movs r1, #8
	movs r2, #0
	b .L_0200088c_7
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x00000101
.L_0200088c_7:
	bl 0x0200b690
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #1
	movs r0, #8
	bl 0x0200b698
	movs r0, #40
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #50
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x0200b690
	movs r0, #40
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #50
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #8
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r0, #11
	movs r1, #1
	bl 0x0200b688
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #129
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200b6d0
	movs r1, #1
	movs r0, #12
	bl 0x0200b680
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #168
	movs r2, #120
	bl 0x0200b648
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200b6c0
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #10
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl 0x0200b690
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r0, #1
	movs r1, #2
	bl 0x0200b688
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #1
	movs r1, #0
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	ldr r0, [pc, #168]
	bl 0x0200b6a8
	movs r1, #1
	movs r0, #8
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b0
	movs r0, #0
	movs r1, #0
	bl 0x0200b608
	cmp r0, #0
	bne .L_0200088c_8
	bl 0x0200987c
	movs r1, #0
	movs r0, #0
	bl 0x0200b708
	movs r0, #120
	bl 0x0200b710
	movs r0, #120
	bl 0x0200b5e8
	movs r0, #86
	bl 0x0200b730
	bl 0x0200b738
	movs r0, #159
	lsls r0, r0, #4
	bl 0x0200b5d8
	movs r0, #30
	bl 0x0200b700
	b .L_0200088c_9
.L_0200088c_8:
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #12
	movs r1, #1
	bl 0x0200b688
	movs r1, #4
	movs r0, #12
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #10
	bl 0x0200b610
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r0, #10
	bl 0x0200b610
	ldr r3, [pc, #16]
	str r3, [r0, #108]
.L_0200088c_9:
	bl 0x0200b5f8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001162
	.4byte 0x02008849
	.global Func_0200187c
	.thumb_func
Func_0200187c:
	push {r5, r6, lr}
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200b6d8
	movs r1, #2
	movs r0, #1
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	ldr r1, [pc, #788]
	ldr r2, [pc, #792]
	bl 0x0200b618
	movs r0, #1
	ldr r1, [pc, #780]
	ldr r2, [pc, #780]
	bl 0x0200b618
	movs r0, #11
	ldr r1, [pc, #768]
	ldr r2, [pc, #772]
	bl 0x0200b618
	movs r0, #12
	ldr r1, [pc, #760]
	ldr r2, [pc, #760]
	bl 0x0200b618
	movs r0, #9
	ldr r1, [pc, #748]
	ldr r2, [pc, #752]
	bl 0x0200b618
	movs r0, #10
	ldr r1, [pc, #740]
	ldr r2, [pc, #740]
	bl 0x0200b618
	movs r0, #8
	ldr r1, [pc, #728]
	ldr r2, [pc, #732]
	bl 0x0200b618
	movs r0, #192
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200b6e8
	bl 0x0200b6f0
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #11
	movs r0, #0
	bl 0x0200b698
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #11
	bl 0x0200b668
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #3
	bl 0x0200b660
	movs r0, #12
	movs r1, #3
	bl 0x0200b660
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b668
	movs r0, #50
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl 0x0200b690
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #1
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	ldr r0, [pc, #520]
	bl 0x0200b6a8
	movs r1, #0
	movs r0, #1
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r0, #12
	movs r1, #1
	bl 0x0200b690
	movs r0, #12
	movs r1, #2
	bl 0x0200b680
	movs r2, #0
	ldr r1, [pc, #484]
	movs r0, #12
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #1
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #40
	bl 0x0200b5e8
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #12
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200b6c0
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #80
	bl 0x0200b5e8
	movs r0, #17
	bl 0x0200b730
	movs r1, #1
	ldr r0, [pc, #324]
	bl 0x0200b708
	movs r0, #60
	bl 0x0200b710
	movs r0, #40
	bl 0x0200b5e8
	ldr r0, [pc, #292]
	ldr r1, [pc, #308]
	bl 0x0200b6e0
	movs r0, #192
	movs r1, #1
	movs r2, #208
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #15
	lsls r0, r0, #16
	bl 0x0200b6e8
	movs r0, #120
	bl 0x0200b5e8
	movs r0, #21
	bl 0x0200b730
	movs r0, #154
	lsls r0, r0, #1
	bl 0x0200b730
	movs r1, #200
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #12
	bl 0x0200b658
	movs r0, #13
	ldr r1, [pc, #232]
	ldr r2, [pc, #248]
	bl 0x0200b618
	movs r2, #72
	movs r1, #200
	movs r0, #13
	bl 0x0200b638
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200b730
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #2
	bl 0x0200b688
	movs r0, #8
	movs r1, #0
	bl 0x0200b660
	bl 0x0200a5c4
	movs r0, #0
	movs r1, #13
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #13
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #13
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #13
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #13
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r0, #10
	movs r1, #13
	bl 0x0200b690
	movs r0, #0
	movs r1, #2
	bl 0x0200b680
	movs r0, #1
	movs r1, #2
	bl 0x0200b680
	movs r0, #11
	movs r1, #2
	bl 0x0200b680
	movs r0, #12
	movs r1, #2
	bl 0x0200b680
	movs r0, #9
	movs r1, #2
	bl 0x0200b680
	movs r1, #2
	movs r0, #10
	bl 0x0200b680
	movs r0, #40
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #13
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #40
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #13
	bl 0x0200b6b8
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #13
	bl 0x0200b6a0
	movs r0, #17
	bl 0x0200b730
	movs r0, #154
	lsls r0, r0, #1
	bl 0x0200b730
	ldr r6, [pc, #32]
	movs r5, #0
	b .L_0200187c_0
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00001171
	.4byte 0x00000103
	.4byte 0x00010005
	.4byte 0x00000ccc
	.4byte 0x00003333
	.4byte 0xfffffd71
.L_0200187c_0:
	movs r0, #13
	bl 0x0200b1b8
	movs r0, #4
	bl 0x0200b5e8
	movs r0, #13
	bl 0x0200b610
	ldr r3, [r0, #24]
	adds r3, r3, r6
	str r3, [r0, #24]
	movs r0, #13
	bl 0x0200b610
	ldr r3, [r0, #28]
	adds r5, #1
	adds r3, r3, r6
	str r3, [r0, #28]
	cmp r5, #31
	bls .L_0200187c_0
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200b730
	movs r0, #13
	movs r1, #0
	bl 0x0200b6a0
	movs r1, #0
	movs r2, #0
	movs r0, #13
	bl 0x0200b658
	movs r0, #40
	bl 0x0200b5e8
	movs r0, #192
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	negs r1, r1
	bl 0x0200b6e8
	bl 0x0200b6f0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200b708
	movs r0, #60
	bl 0x0200b710
	movs r0, #120
	bl 0x0200b5e8
	bl 0x0200a660
	movs r1, #1
	movs r0, #8
	bl 0x0200b660
	movs r0, #2
	bl 0x0200b730
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #8
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	bl 0x0200b680
	movs r0, #1
	movs r1, #1
	bl 0x0200b680
	movs r0, #11
	movs r1, #1
	bl 0x0200b680
	movs r0, #12
	movs r1, #1
	bl 0x0200b680
	movs r0, #9
	movs r1, #1
	bl 0x0200b680
	movs r1, #1
	movs r0, #10
	bl 0x0200b688
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200b690
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #8
	movs r2, #0
	movs r0, #12
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #12
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
.L_02001d28:
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200b6d0
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r0, #1
	movs r1, #3
	bl 0x0200b660
	movs r0, #11
	movs r1, #3
	bl 0x0200b660
	movs r0, #12
	movs r1, #3
	bl 0x0200b660
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #180
	bl 0x0200b5e8
	ldr r1, [pc, #1016]
	movs r2, #0
	movs r0, #12
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	movs r2, #0
	bl 0x0200b698
	movs r1, #10
	movs r2, #0
	movs r0, #9
	bl 0x0200b698
	movs r0, #40
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #8
	movs r0, #10
	bl 0x0200b690
	movs r0, #40
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #1
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #1
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r2, #0
	ldr r1, [pc, #904]
	movs r0, #8
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	bl 0x0200b680
	movs r0, #1
	movs r1, #1
	bl 0x0200b680
	movs r0, #11
	movs r1, #1
	bl 0x0200b680
	movs r0, #12
	movs r1, #1
	bl 0x0200b680
	movs r0, #9
	movs r1, #1
	bl 0x0200b680
	movs r1, #1
	movs r0, #10
	bl 0x0200b688
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	ldr r1, [pc, #820]
	movs r2, #0
	movs r0, #8
	bl 0x0200b6d0
	movs r0, #80
	bl 0x0200b5e8
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r2, #0
	ldr r1, [pc, #768]
	movs r0, #8
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #1
	bl 0x0200b688
	movs r1, #131
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200b6d0
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	bl 0x0200b680
	movs r0, #1
	movs r1, #1
	bl 0x0200b680
	movs r0, #11
	movs r1, #1
	bl 0x0200b680
	movs r0, #12
	movs r1, #1
	bl 0x0200b680
	movs r0, #9
	movs r1, #1
	bl 0x0200b680
	movs r1, #1
	movs r0, #10
	bl 0x0200b688
	movs r0, #40
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r0, #1
	movs r1, #3
	bl 0x0200b660
	movs r0, #11
	movs r1, #3
	bl 0x0200b660
	movs r0, #12
	movs r1, #3
	bl 0x0200b660
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b668
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r0, #1
	movs r1, #3
	bl 0x0200b660
	movs r0, #11
	movs r1, #3
	bl 0x0200b660
	movs r0, #12
	movs r1, #3
	bl 0x0200b660
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b668
	movs r0, #120
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #1
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #1
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	ldr r1, [pc, #480]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #1
	ldr r1, [pc, #468]
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #11
	ldr r1, [pc, #460]
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r0, #9
	ldr r1, [pc, #436]
	movs r2, #0
	bl 0x0200b6d0
	movs r2, #0
	ldr r1, [pc, #428]
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #12
	movs r1, #1
	bl 0x0200b688
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #12
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #8
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
.L_020020de:
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #3
	bl 0x0200b668
	movs r1, #0
	movs r0, #8
	bl 0x0200b6b8
	movs r0, #40
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #168
	movs r2, #176
	bl 0x0200b648
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #9
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #10
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #8
	movs r1, #200
	movs r2, #200
	bl 0x0200b648
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #12
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b6c0
	movs r2, #136
	movs r1, #200
	lsls r2, r2, #1
	movs r0, #8
	bl 0x0200b640
	movs r0, #40
	bl 0x0200b5e8
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl 0x0200b6c0
	movs r0, #8
	bl 0x0200b650
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200b658
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	b .L_020020de_0
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
.L_020020de_0:
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #120
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200b690
	movs r0, #1
	movs r1, #10
	movs r2, #0
	bl 0x0200b690
	movs r0, #11
	movs r1, #9
	movs r2, #0
	bl 0x0200b690
	movs r1, #9
	movs r2, #0
	movs r0, #12
	bl 0x0200b690
	movs r0, #120
	bl 0x0200b5e8
	ldr r1, [pc, #792]
	movs r2, #0
	movs r0, #9
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #9
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl 0x0200b6c0
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl 0x0200b6d0
	movs r0, #80
	bl 0x0200b5e8
	movs r2, #0
	movs r0, #9
	movs r1, #10
	bl 0x0200b690
	movs r1, #1
	movs r0, #9
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #9
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl 0x0200b6c0
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	ldr r1, [pc, #656]
	movs r0, #10
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #9
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #10
	bl 0x0200b690
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #10
	bl 0x0200b688
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl 0x0200b6c0
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #10
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #136
	movs r0, #9
	movs r1, #200
	lsls r2, r2, #1
	bl 0x0200b640
	movs r2, #136
	movs r0, #10
	movs r1, #200
	lsls r2, r2, #1
	bl 0x0200b648
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200b658
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200b658
	ldr r1, [pc, #496]
	movs r2, #0
	movs r0, #12
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #12
	movs r1, #200
	movs r2, #136
	bl 0x0200b648
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x0200b698
	movs r0, #1
	movs r1, #12
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #12
	movs r0, #11
	bl 0x0200b690
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #12
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #11
	movs r1, #168
	movs r2, #168
	bl 0x0200b648
	movs r1, #12
	movs r2, #0
	movs r0, #11
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #11
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #11
	movs r0, #1
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #11
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #4
	movs r0, #12
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #11
	bl 0x0200b688
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #12
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #2
	bl 0x0200b680
	movs r0, #1
	movs r1, #2
	bl 0x0200b680
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #11
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #0
	movs r0, #11
	bl 0x0200b6b8
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #11
	movs r0, #0
	bl 0x0200b690
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #11
	bl 0x0200b660
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #12
	movs r1, #3
	bl 0x0200b660
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000105
	.4byte 0x00000101
	.global Func_020025c4
	.thumb_func
Func_020025c4:
	push {r5, lr}
	movs r0, #8
	bl 0x0200b610
	cmp r0, #0
	beq .L_020025c4_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #14
	bl 0x0200b658
.L_020025c4_0:
	movs r1, #0
	movs r0, #14
	bl 0x0200b660
	movs r0, #14
	bl 0x0200b610
	adds r5, r0, #0
	movs r0, #8
	bl 0x0200b610
	ldrh r3, [r0, #6]
	movs r0, #14
	strh r3, [r5, #6]
	bl 0x0200b610
	ldr r3, [pc, #96]
	str r3, [r0, #108]
	movs r0, #14
	bl 0x0200b610
	ldr r5, [r0, #80]
	adds r3, r5, #0
	adds r3, #39
	ldrb r3, [r3]
	movs r0, #0
	cmp r0, r3
	bcs .L_020025c4_1
	adds r1, r5, #0
	movs r4, #10
	mov r12, r3
	adds r1, #40
.L_020025c4_3:
	ldmia r1!, {r2}
	cmp r2, #0
	beq .L_020025c4_2
	ldr r3, [r2, #16]
	cmp r3, #0
	beq .L_020025c4_2
	strb r4, [r2, #5]
.L_020025c4_2:
	adds r0, #1
	cmp r0, r12
	bcc .L_020025c4_3
.L_020025c4_1:
	adds r2, r5, #0
	adds r2, #37
	movs r3, #1
	strb r3, [r2]
	movs r0, #14
	bl 0x0200b610
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldrb r2, [r5, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r5, #9]
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200a67d
	.global Func_02002660
	.thumb_func
Func_02002660:
	push {lr}
	movs r0, #14
	bl 0x0200b610
	movs r3, #0
	str r3, [r0, #108]
	movs r1, #0
	movs r0, #14
	movs r2, #0
	bl 0x0200b658
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200267c
	.thumb_func
Func_0200267c:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #8
	bl 0x0200b610
	ldr r3, [r0, #8]
	str r3, [r5, #8]
	str r3, [r5, #56]
	ldr r3, [r0, #12]
	str r3, [r5, #12]
	str r3, [r5, #60]
	ldr r2, [pc, #80]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r5, #16]
	str r3, [r5, #64]
	ldr r3, [pc, #72]
	ldr r2, [r3]
	movs r3, #3
	ands r2, r3
	cmp r2, #1
	beq .L_0200267c_0
	cmp r2, #1
	bcc .L_0200267c_1
	cmp r2, #2
	beq .L_0200267c_2
	cmp r2, #3
	beq .L_0200267c_3
	b .L_0200267c_4
.L_0200267c_1:
	ldr r3, [r0, #8]
	ldr r2, [pc, #48]
	b .L_0200267c_5
.L_0200267c_0:
	ldr r3, [r0, #8]
	movs r2, #192
	lsls r2, r2, #10
.L_0200267c_5:
	adds r3, r3, r2
	str r3, [r5, #8]
	str r3, [r5, #56]
	b .L_0200267c_4
.L_0200267c_2:
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #12]
	str r3, [r5, #60]
	b .L_0200267c_4
.L_0200267c_3:
	ldr r3, [r0, #16]
	str r3, [r5, #16]
	str r3, [r5, #64]
.L_0200267c_4:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xfffe0000
	.4byte 0x03001e40
	.4byte 0xfffc8000
	.global Func_020026f0
	.thumb_func
Func_020026f0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r2, #100
	adds r2, r2, r5
	ldrh r6, [r2]
	ldr r1, [r5, #104]
	adds r0, r6, #0
	mov r8, r2
	mov r10, r1
	bl 0x0200b548
	mov r1, r10
	lsls r2, r0, #3
	ldr r3, [r1, #8]
	subs r2, r2, r0
	lsls r2, r2, #1
	adds r3, r3, r2
	str r3, [r5, #8]
	adds r0, r6, #0
	bl 0x0200b540
	mov r1, r10
	lsls r3, r0, #2
	ldr r2, [r1, #16]
	adds r3, r3, r0
	lsls r3, r3, #1
	adds r2, r2, r3
	ldr r3, [r5, #8]
	str r2, [r5, #16]
	str r2, [r5, #64]
	str r3, [r5, #56]
	mov r2, r8
	adds r5, #102
	ldrh r3, [r2]
	ldrh r2, [r5]
	mov r1, r8
	adds r3, r3, r2
	strh r3, [r1]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002750
	.thumb_func
Func_02002750:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r1
	bl 0x0200b610
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02002750_0
	ldr r2, [r7, #12]
	movs r3, #180
	lsls r3, r3, #14
	adds r2, r2, r3
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	ldr r0, [pc, #76]
	bl 0x0200b570
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02002750_0
	ldr r1, [pc, #68]
	ldr r5, [r6, #80]
	bl 0x0200b568
	adds r3, r6, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	adds r3, #2
	mov r2, r8
	strh r2, [r3]
	ldr r3, [pc, #44]
	ldr r1, [pc, #32]
	str r3, [r6, #108]
	adds r3, r5, #0
	adds r3, #38
	strb r1, [r3]
	ldr r3, [r7, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	ldrb r1, [r5, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	str r7, [r6, #104]
	strb r3, [r5, #9]
	b .L_02002750_0
	.4byte 0x00000000
	.4byte 0x0000011d
	.4byte 0x0200c15c
	.4byte 0x0200a6f1
.L_02002750_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #8
	movs	r1, #0
	movs	r2, #0
	sub	sp, #20
	bl 0x0200b658
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b658
	movs	r0, #10
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b658
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b658
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b658
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x0200b658
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200b658
	movs	r0, #128
	movs	r1, #2
	lsls	r0, r0, #9
	bl 0x0200b708
	movs	r0, #1
	bl 0x0200b710
	movs	r0, #1
	bl 0x0200b5e8
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #25
	movs	r3, #5
	movs	r0, #2
	bl 0x0200b588
	ldr	r5, [pc, #172]
	adds	r7, r0, #0
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #16
	movs	r3, #0
	bl 0x0200b598
	movs	r0, #1
	bl 0x0200b5c8
	cmp	r0, #0
	bne.n	.L_02002870
	adds	r0, r5, #2
	adds	r1, r7, #0
	movs	r2, #16
	movs	r3, #16
	bl 0x0200b598
	b.n	.L_0200287c
.L_02002870:
	adds	r0, r5, #1
	adds	r1, r7, #0
	movs	r2, #16
	movs	r3, #16
	bl 0x0200b598
.L_0200287c:
	add	r1, sp, #4
	add	r0, sp, #8
	bl 0x0200b5a8
	movs	r2, #60
	add	r0, sp, #8
	movs	r1, #72
	bl 0x0200b5b0
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_020028dc
	ldr	r2, [pc, #96]
	movs	r6, #1
	mov	r8, r2
.L_020028a2:
	ldr	r3, [pc, #96]
	ldr	r3, [r3, #0]
	movs	r2, #192
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020028b0
	eors	r5, r6
.L_020028b0:
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #1
	ands	r3, r2
	lsls	r3, r3, #2
	mov	r2, r8
	ldr	r1, [r2, r3]
	lsls	r2, r5, #4
	add	r0, sp, #8
	adds	r1, #24
	adds	r2, #60
	bl 0x0200b5b0
	movs	r0, #1
	bl 0x0200b5e8
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_020028a2
.L_020028dc:
	ldr	r0, [sp, #4]
	bl 0x0200b5b8
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200b590
	adds	r0, r5, #0
	add	sp, #20
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x0000116e
	.4byte 0x03001c94
	.4byte 0x0200c11c
	.4byte 0x03001b04
	.2byte 0x1800
	.2byte 0x0300
	.global Func_0200290c
	.thumb_func
Func_0200290c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b618
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #8
	movs r0, #1
	lsls r1, r1, #9
	bl 0x0200b618
	movs r1, #2
	movs r0, #12
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #12
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #0
	bl 0x0200b668
	movs r0, #15
	bl 0x0200b5e8
	movs r2, #0
	movs r0, #0
	movs r1, #1
	bl 0x0200b690
	movs r1, #1
	movs r0, #0
	bl 0x0200b680
	movs r0, #0
	bl 0x0200b610
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #0
	mov r8, r2
	strb r3, [r0]
	movs r1, #184
	movs r2, #168
	movs r0, #0
	bl 0x0200b640
	movs r0, #1
	bl 0x0200b610
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	movs r2, #168
	movs r1, #200
	strb r5, [r0]
	movs r0, #1
	bl 0x0200b648
	movs r0, #1
	bl 0x0200b5e8
	movs r0, #1
	bl 0x0200b610
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #0
	bl 0x0200b650
	movs r1, #1
	movs r0, #0
	bl 0x0200b660
	movs r0, #0
	bl 0x0200b610
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl 0x0200b610
	adds r0, #90
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x0200b678
	movs r0, #15
	bl 0x0200b5e8
	movs r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b690
	movs r0, #5
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #2
	movs r0, #1
	bl 0x0200b678
	movs r0, #25
	bl 0x0200b5e8
	movs r0, #1
	movs r1, #2
	bl 0x0200b688
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #5
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #0
	bl 0x0200b668
	movs r0, #5
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #0
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #15
	bl 0x0200b5e8
	movs r0, #11
	movs r1, #3
	bl 0x0200b660
	movs r0, #12
	movs r1, #3
	bl 0x0200b660
	movs r0, #8
	movs r1, #3
	bl 0x0200b660
	movs r0, #9
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #10
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #11
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #11
	movs r0, #1
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #20
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #0
	bl 0x0200b6c0
	movs r0, #15
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #0
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #184
	movs r2, #216
	movs r3, #168
	lsls r3, r3, #16
	lsls r1, r1, #16
	lsls r2, r2, #13
	movs r0, #222
	bl 0x0200b098
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #1
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200b678
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #15
	bl 0x0200b5e8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #30
	bl 0x0200b5e8
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200b678
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #15
	bl 0x0200b5e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200b6d0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #1
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #8
	bl 0x0200b610
	movs r6, #1
	adds r0, #100
	strh r6, [r0]
	movs r0, #8
	bl 0x0200b610
	ldr r5, [pc, #1020]
	str r5, [r0, #108]
	movs r0, #12
	bl 0x0200b610
	adds r0, #100
	strh r6, [r0]
	movs r0, #12
	bl 0x0200b610
	movs r1, #196
	str r5, [r0, #108]
	movs r2, #180
	movs r0, #1
	bl 0x0200b648
	movs r0, #1
	movs r1, #184
	movs r2, #184
	bl 0x0200b648
	movs r0, #1
	movs r1, #180
	movs r2, #180
	bl 0x0200b648
	movs r0, #1
	movs r1, #168
	movs r2, #168
	bl 0x0200b648
	movs r0, #1
	movs r1, #180
	movs r2, #156
	bl 0x0200b648
	movs r0, #1
	movs r1, #200
	movs r2, #104
	bl 0x0200b640
	movs r0, #0
	movs r1, #192
	movs r2, #168
	bl 0x0200b648
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200b6c0
	movs r0, #1
	bl 0x0200b650
	movs r0, #30
	bl 0x0200b5e8
	movs r1, #1
	movs r0, #1
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #15
	bl 0x0200b5e8
	movs r0, #12
	bl 0x0200b610
	mov r3, r8
	str r3, [r0, #108]
	movs r0, #8
	bl 0x0200b610
	mov r2, r8
	str r2, [r0, #108]
	movs r1, #2
	movs r0, #8
	bl 0x0200b680
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #8
	movs r1, #0
	bl 0x0200b660
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x0200b6d0
	movs r0, #60
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #11
	movs r0, #0
	bl 0x0200b698
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #0
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #11
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #0
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #2
	movs r0, #0
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #2
	movs r2, #0
	movs r0, #0
	bl 0x0200b678
	movs r0, #20
	bl 0x0200b5e8
	movs r1, #2
	movs r2, #0
	movs r0, #0
	bl 0x0200b678
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #15
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #12
	movs r0, #0
	bl 0x0200b698
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #0
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #12
	bl 0x0200b668
	movs r0, #60
	bl 0x0200b5e8
	movs r0, #1
	movs r1, #208
	movs r2, #168
	bl 0x0200b648
	movs r0, #0
	movs r1, #11
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #4
	bl 0x0200b660
	movs r1, #4
	movs r0, #1
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #1
	bl 0x0200b680
	movs r1, #1
	movs r0, #1
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #3
	movs r0, #12
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200b6c0
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #2
	bl 0x0200b680
	movs r1, #2
	movs r0, #1
	bl 0x0200b688
	movs r0, #10
	bl 0x0200b5e8
	ldr r1, [pc, #456]
	movs r0, #0
	bl 0x0200b620
	ldr r1, [pc, #452]
	movs r0, #1
	bl 0x0200b620
	movs r0, #0
	bl 0x0200b628
	movs r0, #1
	bl 0x0200b628
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b618
	movs r1, #192
	movs r2, #192
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200b618
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200b678
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200b678
	movs r0, #0
	movs r1, #9
	movs r2, #0
	bl 0x0200b690
	movs r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b690
	movs r0, #1
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x0200b690
	movs r1, #11
	movs r2, #0
	movs r0, #1
	bl 0x0200b690
	movs r0, #1
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #8
	movs r2, #0
	bl 0x0200b690
	movs r1, #9
	movs r2, #0
	movs r0, #1
	bl 0x0200b690
	movs r0, #1
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #192
	movs r2, #168
	bl 0x0200b640
	movs r1, #208
	movs r2, #168
	movs r0, #1
	bl 0x0200b648
	movs r0, #0
	bl 0x0200b650
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200b6c0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #11
	movs r2, #0
	bl 0x0200b690
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x0200b690
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #11
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #12
	bl 0x0200b668
	movs r0, #30
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #1
	movs r0, #0
	bl 0x0200b698
	movs r0, #20
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #3
	bl 0x0200b660
	movs r1, #3
	movs r0, #1
	bl 0x0200b668
	movs r0, #10
	bl 0x0200b5e8
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x0200b6c0
	movs r0, #10
	bl 0x0200b5e8
	movs r0, #0
	movs r1, #2
	bl 0x0200b660
	movs r0, #1
.L_02003070:
	movs r1, #2
	bl 0x0200b660
	movs r0, #60
	bl 0x0200b5e8
	b .L_02003070_0
	.2byte 0x0000
	.2byte 0x8031
	.2byte 0x0200
	.2byte 0xb740
	.2byte 0x0200
	.2byte 0xb81c
	.2byte 0x0200
.L_02003070_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003098
	.thumb_func
Func_02003098:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r0, #22
	bl 0x0200b570
	adds r7, r0, #0
	movs r5, #0
	cmp r7, #0
	beq .L_02003098_0
	ldr r1, [pc, #140]
	bl 0x0200b568
	ldr r6, [r7, #80]
	adds r3, r6, #0
	adds r3, #38
	strb r5, [r3]
	adds r3, #1
	strb r5, [r3]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #40]
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #193
	str r3, [r7, #72]
	lsls r1, r1, #3
	movs r0, #17
	bl 0x0200b550
	adds r5, r0, #0
	mov r0, r8
	bl 0x0200b5c0
	movs r2, #128
	lsls r2, r2, #3
	adds r5, r5, r2
	adds r2, r5, #0
	ldrb r0, [r6, #28]
	movs r1, #128
	bl 0x0200b560
	movs r0, #17
	bl 0x0200b558
	movs r5, #0
	adds r6, r7, #0
	adds r6, #85
	mov r8, r5
.L_02003098_2:
	ldr r3, [r7, #40]
	movs r2, #255
	adds r3, #255
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_02003098_1
	mov r3, r8
	strb r3, [r6]
.L_02003098_1:
	movs r0, #1
	adds r5, #1
	bl 0x0200b528
	cmp r5, #59
	bls .L_02003098_2
	ldr r1, [pc, #20]
	adds r0, r7, #0
	bl 0x0200b568
.L_02003098_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200b8f8
	.4byte 0x0200ba9c
	.global Func_02003144
	.thumb_func
Func_02003144:
	push {lr}
	adds r3, r0, #0
	adds r3, #100
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [r0, #12]
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r0, #12]
	str r3, [r0, #60]
	adds r1, r0, #0
	adds r1, #102
	ldrh r3, [r1]
	lsls r3, r3, #16
	asrs r2, r3, #18
	ldr r3, [pc, #28]
	ands r2, r3
	movs r4, #0
	cmp r2, #1
	beq .L_02003144_0
	cmp r2, #1
	bgt .L_02003144_1
	cmp r2, #0
	beq .L_02003144_2
	b .L_02003144_3
.L_02003144_1:
	cmp r2, #2
	beq .L_02003144_4
	cmp r2, #3
	beq .L_02003144_0
	b .L_02003144_3
	.4byte 0x00000003
.L_02003144_2:
	movs r4, #128
	lsls r4, r4, #9
	b .L_02003144_3
.L_02003144_0:
	ldr r4, [pc, #32]
	b .L_02003144_3
.L_02003144_4:
	ldr r4, [pc, #32]
.L_02003144_3:
	str r4, [r0, #24]
	str r4, [r0, #28]
	ldrh r3, [r1]
	subs r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_02003144_5
	ldr r1, [pc, #16]
	bl 0x0200b568
.L_02003144_5:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x0200c18c
	.global Func_020031b8
	.thumb_func
Func_020031b8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	bl 0x0200b610
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020031b8_0
	bl 0x0200b530
	movs r1, #20
	bl 0x0200b520
	ldr r5, [r6, #8]
	ldr r2, [pc, #128]
	lsls r0, r0, #16
	adds r5, r5, r0
	adds r5, r5, r2
	bl 0x0200b530
	movs r3, #15
	ands r3, r0
	ldr r2, [r6, #12]
	lsls r3, r3, #16
	adds r2, r2, r3
	ldr r3, [pc, #112]
	movs r0, #143
	adds r2, r2, r3
	lsls r0, r0, #1
	ldr r3, [r6, #16]
	adds r1, r5, #0
	bl 0x0200b570
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020031b8_0
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	ldr r5, [r7, #80]
	strb r3, [r2]
	bl 0x0200b530
	movs r1, #10
	bl 0x0200b520
	adds r3, r7, #0
	adds r3, #100
	ldr r2, [pc, #56]
	adds r0, #5
	strh r0, [r3]
	mov r8, r2
	bl 0x0200b530
	movs r1, #60
	bl 0x0200b520
	adds r3, r7, #0
	adds r3, #102
	adds r0, #30
	strh r0, [r3]
	ldr r3, [pc, #44]
	str r3, [r7, #108]
	adds r3, r5, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	ldr r3, [r6, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	ldrb r1, [r5, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
	b .L_020031b8_0
	.4byte 0x00000000
	.4byte 0xfff60000
	.4byte 0xfff80000
	.4byte 0x0200b145
.L_020031b8_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003270
	.thumb_func
Func_02003270:
	push {lr}
	sub sp, #8
	movs r3, #3
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #64
	movs r2, #11
	movs r3, #68
	bl 0x0200b578
	movs r3, #11
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #10
	movs r2, #3
	movs r3, #2
	movs r0, #11
	bl 0x0200b580
	movs r0, #1
	bl 0x0200b528
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_020032a8
	.thumb_func
Func_020032a8:
	push {lr}
	movs r0, #0
	bl 0x0200b610
	ldr r2, [pc, #20]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #20]
	lsls r3, r3, #16
	movs r0, #1
	cmp r3, r2
	bls .L_020032a8_0
	movs r0, #0
.L_020032a8_0:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00005fff
	.4byte 0x3ffe0000
	.global Func_020032d0
	.thumb_func
Func_020032d0:
	push {lr}
	bl 0x0200b2a8
	cmp r0, #0
	beq .L_020032d0_0
	movs r0, #8
	bl 0x0200b728
	b .L_020032d0_1
.L_020032d0_0:
	bl 0x0200b5f0
	ldr r0, [pc, #56]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_020032d0_2
	ldr r0, [pc, #48]
	bl 0x0200b6a8
	b .L_020032d0_3
.L_020032d0_2:
	ldr r0, [pc, #44]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_020032d0_4
	ldr r0, [pc, #40]
	bl 0x0200b6a8
	b .L_020032d0_3
.L_020032d0_4:
	ldr r0, [pc, #36]
	bl 0x0200b6a8
.L_020032d0_3:
	movs r0, #8
	movs r1, #0
	bl 0x0200b6b8
	bl 0x0200b5f8
.L_020032d0_1:
	pop {r0}
	bx r0
	.4byte 0x0000087a
	.4byte 0x00001bfc
	.4byte 0x00000815
	.4byte 0x0000119d
	.4byte 0x00001035
	.global Func_02003334
	.thumb_func
Func_02003334:
	push {lr}
	bl 0x0200b2a8
	cmp r0, #0
	beq .L_02003334_0
	movs r0, #8
	bl 0x0200b728
	b .L_02003334_1
.L_02003334_0:
	bl 0x0200b5f0
	ldr r3, [pc, #248]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #10
	cmp r3, #40
	bhi .L_02003334_2
	ldr r2, [pc, #232]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	push {r3}
	lsls r0, r0, #8
	push {r1, r5}
	lsls r0, r0, #8
	push {r3}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r1, r3, r5}
	lsls r0, r0, #8
	push {r1, r3, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r2, r4, r5}
	lsls r0, r0, #8
	push {r1, r3, r5}
	lsls r0, r0, #8
	ldr r0, [pc, #64]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_02003334_3
	ldr r0, [pc, #60]
	bl 0x0200b6a8
	b .L_02003334_2
.L_02003334_3:
	ldr r0, [pc, #56]
	bl 0x0200b6a8
	b .L_02003334_2
	.2byte 0x480d
	.2byte 0xf000
	.2byte 0xf940
	.2byte 0xe004
	.2byte 0xf000
	.2byte 0xf8e5
	.2byte 0xf7fc
	.2byte 0xff97
	.2byte 0xe005
.L_02003334_2:
	movs r0, #8
	movs r1, #0
	bl 0x0200b6b8
	bl 0x0200b5f8
.L_02003334_1:
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0200b364
	.4byte 0x00000855
	.4byte 0x00001376
	.4byte 0x00001288
	.2byte 0x1ce8
	.2byte 0x0000
	.global Func_0200345c
	.thumb_func
Func_0200345c:
	push {lr}
	bl 0x0200b2a8
	cmp r0, #0
	beq .L_0200345c_0
	movs r0, #8
	bl 0x0200b728
	b .L_0200345c_1
.L_0200345c_0:
	bl 0x0200b5f0
	ldr r0, [pc, #40]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_0200345c_2
	ldr r0, [pc, #32]
	bl 0x0200b6a8
	b .L_0200345c_3
.L_0200345c_2:
	ldr r0, [pc, #28]
	bl 0x0200b6a8
.L_0200345c_3:
	movs r0, #8
	movs r1, #0
	bl 0x0200b6b8
	bl 0x0200b5f8
.L_0200345c_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000845
	.4byte 0x0000171c
	.4byte 0x00001408
	.global Func_020034a8
	.thumb_func
Func_020034a8:
	push {lr}
	bl 0x0200b2a8
	cmp r0, #0
	beq .L_020034a8_0
	movs r0, #8
	bl 0x0200b728
	b 0x0200b4e0
.L_020034a8_0:
	bl 0x0200b5f0
	ldr r0, [pc, #36]
	bl 0x0200b6a8
	ldr r0, [pc, #32]
	bl 0x0200b5d0
	cmp r0, #0
	beq .L_020034a8_1
	ldr r0, [pc, #28]
	bl 0x0200b6a8
.L_020034a8_1:
	movs r0, #8
	movs r1, #0
.L_020034d8:
	bl 0x0200b6b8
	bl 0x0200b5f8
	pop {r0}
	bx r0
	.2byte 0x190a
	.2byte 0x0000
	.2byte 0x0909
	.2byte 0x0000
	.2byte 0x1951
	.2byte 0x0000
	.global Func_020034f0
	.thumb_func
Func_020034f0:
	push {lr}
	bl 0x0200b2a8
	cmp r0, #0
	beq .L_020034f0_0
	movs r0, #8
	bl 0x0200b728
	b .L_020034f0_1
.L_020034f0_0:
	bl 0x0200b5f0
	ldr r0, [pc, #20]
	bl 0x0200b6a8
	movs r0, #8
	movs r1, #0
	bl 0x0200b6b8
	bl 0x0200b5f8
.L_020034f0_1:
	pop {r0}
	bx r0
	.4byte 0x00001823
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHINDEN_HEYA/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00028000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x80010000
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ec0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ec0000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00028000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x80010000
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ac0000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ac0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000f000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000f000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001b
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c7
	.4byte 0xc00000f6
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff0002
	.4byte 0x000000c7
	.4byte 0x400000a4
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff000a
	.4byte 0x000000c7
	.4byte 0xc00000f6
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff000b
	.4byte 0x000000c7
	.4byte 0xc00000f6
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff000c
	.4byte 0x000000c7
	.4byte 0x400000a4
	.4byte 0x00400000
	.4byte 0x01500008
	.4byte 0x00000110
	.4byte 0xffff0014
	.4byte 0x000000c0
	.4byte 0xc00000a8
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0xffff0015
	.4byte 0x000000c0
	.4byte 0xc00000a8
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0xffff001d
	.4byte 0x000000c8
	.4byte 0xc00000f0
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0xffff0020
	.4byte 0x000000c8
	.4byte 0xc00000f0
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0xffff0023
	.4byte 0x000000c8
	.4byte 0xc00000f0
	.4byte 0x00800000
	.4byte 0x01400000
	.4byte 0x00000110
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00101004
	.4byte 0x00a0b014
	.4byte 0x00b0b017
	.4byte 0x01415009
	.4byte 0x01e0c005
	.4byte 0x0280304b
	.4byte 0x0290901e
	.4byte 0x02a0b048
	.4byte 0x03201001
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00004000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00003000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00820000
	.4byte 0x00013000
	.4byte 0xffff007a
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00005000
	.4byte 0xffff00dd
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0003d000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00003000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0000b000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x0000b000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x0003d000
	.4byte 0xffff0075
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00035000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b2d1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001036
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001037
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b335
	.4byte 0x00000000
	.4byte 0x08550009
	.4byte 0x0200827d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200827d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001378
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x020082ed
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000000a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b335
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200827d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001cea
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x020082ed
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008361
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00001168
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001169
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001167
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000116a
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000116b
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200874d
	.4byte 0x00000002
	.4byte 0x02000002
	.4byte 0x020086e9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b2d1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000119e
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008575
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000011d8
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000011d9
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000011da
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b2d1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020084e9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001c02
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c03
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001c04
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001c05
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b4a9
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b4a9
	.4byte 0x00008d15
	.4byte 0x09090008
	.4byte 0x0000190b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001952
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b45d
	.4byte 0x00000000
	.4byte 0x08450009
	.4byte 0x00001409
	.4byte 0x00008d15
	.4byte 0x08450008
	.4byte 0x0000140a
	.4byte 0x00008d15
	.4byte 0x08450009
	.4byte 0x0000140b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000171d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000171e
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000171f
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000029
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000002a
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200b4f1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001824
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff8000
	.4byte 0x00000016
	.4byte 0x00000012
	.4byte 0xffff8000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000060
	.4byte 0x00000000
	.4byte 0x0000001b
	.4byte 0x0000001b
