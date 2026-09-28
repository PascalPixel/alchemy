.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	movs r0, #14
	movs r1, #23
	bl 0x0200bca0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_02000040_0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r1, #16]
	ldr r3, [r5, #16]
	ldr r1, [r1, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x0200ba98
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_02000040_0
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_02000040_1
	adds r0, r2, #0
.L_02000040_1:
	ldr r2, [pc, #20]
	cmp r0, r2
	bge .L_02000040_2
	adds r0, r2, #0
.L_02000040_2:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_02000040_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff000
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #8
	bl 0x0200bb98
	ldr r3, [r0, #8]
	str r3, [r5, #8]
	ldr r3, [pc, #16]
	str r3, [r5, #12]
	ldr r3, [r0, #16]
	movs r0, #0
	str r3, [r5, #16]
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfff40000
	.global Func_020000bc
	.thumb_func
Func_020000bc:
	ldr r3, [r0, #8]
	ldr r2, [r0, #36]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #40]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	ldr r1, [r0, #44]
	str r3, [r0, #12]
	ldr r3, [r0, #24]
	adds r3, r3, r1
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r1
	str r3, [r0, #28]
	ldr r3, [r0, #72]
	subs r2, r2, r3
	str r2, [r0, #40]
	movs r0, #0
	bx lr
	.global Func_020000e4
	.thumb_func
Func_020000e4:
	push {lr}
	ldr r3, [pc, #128]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #120]
	cmp r2, r3
	bne .L_020000e4_0
	ldr r0, [pc, #116]
	b .L_020000e4_1
.L_020000e4_0:
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_020000e4_2
	ldr r0, [pc, #116]
	b .L_020000e4_1
.L_020000e4_2:
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_020000e4_3
	ldr r0, [pc, #112]
	b .L_020000e4_1
.L_020000e4_3:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_020000e4_4
	ldr r0, [pc, #112]
	b .L_020000e4_1
.L_020000e4_4:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_020000e4_5
	ldr r0, [pc, #108]
	b .L_020000e4_1
.L_020000e4_5:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_020000e4_6
	ldr r0, [pc, #108]
	b .L_020000e4_1
.L_020000e4_6:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_020000e4_7
	ldr r0, [pc, #104]
	b .L_020000e4_1
.L_020000e4_7:
	ldr r3, [pc, #104]
	cmp r2, r3
	bne .L_020000e4_8
	ldr r0, [pc, #104]
	b .L_020000e4_1
.L_020000e4_8:
	ldr r3, [pc, #104]
	cmp r2, r3
	bne .L_020000e4_9
	ldr r0, [pc, #100]
	b .L_020000e4_1
.L_020000e4_9:
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_020000e4_10
	ldr r0, [pc, #100]
	b .L_020000e4_1
.L_020000e4_10:
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_020000e4_11
	ldr r0, [pc, #96]
	b .L_020000e4_1
.L_020000e4_11:
	ldr r0, [pc, #96]
.L_020000e4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x0200c194
	.4byte 0x0000004e
	.4byte 0x0200c20c
	.4byte 0x0000004f
	.4byte 0x0200c26c
	.4byte 0x00000050
	.4byte 0x0200c314
	.4byte 0x00000051
	.4byte 0x0200c3ec
	.4byte 0x00000052
	.4byte 0x0200c464
	.4byte 0x00000053
	.4byte 0x0200c524
	.4byte 0x00000054
	.4byte 0x0200c59c
	.4byte 0x00000055
	.4byte 0x0200c644
	.4byte 0x00000056
	.4byte 0x0200c704
	.4byte 0x00000057
	.4byte 0x0200c77c
	.4byte 0x0200c164
	.global Func_020001c8
	.thumb_func
Func_020001c8:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_020001c8_0
	ldr r0, [pc, #24]
	b .L_020001c8_1
.L_020001c8_0:
	ldr r3, [pc, #24]
	movs r0, #0
	cmp r2, r3
	bne .L_020001c8_1
	ldr r0, [pc, #20]
.L_020001c8_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000055
	.4byte 0x0200c80c
	.4byte 0x00000056
	.4byte 0x0200c83c
	.global Func_02000204
	.thumb_func
Func_02000204:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c85c
	.global Func_0200020c
	.thumb_func
Func_0200020c:
	push {lr}
	ldr r3, [pc, #108]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_0200020c_0
	ldr r0, [pc, #96]
	b .L_0200020c_1
.L_0200020c_0:
	ldr r3, [pc, #96]
	cmp r2, r3
	bne .L_0200020c_2
	ldr r0, [pc, #96]
	b .L_0200020c_1
.L_0200020c_2:
	ldr r3, [pc, #96]
	cmp r2, r3
	bne .L_0200020c_3
	ldr r0, [pc, #92]
	b .L_0200020c_1
.L_0200020c_3:
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_0200020c_4
	ldr r0, [pc, #92]
	b .L_0200020c_1
.L_0200020c_4:
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_0200020c_5
	ldr r0, [pc, #88]
	b .L_0200020c_1
.L_0200020c_5:
	ldr r3, [pc, #88]
	cmp r2, r3
	bne .L_0200020c_6
	ldr r0, [pc, #88]
	b .L_0200020c_1
.L_0200020c_6:
	ldr r3, [pc, #88]
	cmp r2, r3
	bne .L_0200020c_7
	ldr r0, [pc, #84]
	b .L_0200020c_1
.L_0200020c_7:
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_0200020c_8
	ldr r0, [pc, #84]
	b .L_0200020c_1
.L_0200020c_8:
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_0200020c_9
	ldr r0, [pc, #80]
	b .L_0200020c_1
.L_0200020c_9:
	ldr r0, [pc, #80]
.L_0200020c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x0200c940
	.4byte 0x0000004f
	.4byte 0x0200c9a0
	.4byte 0x00000051
	.4byte 0x0200ca00
	.4byte 0x00000052
	.4byte 0x0200ca60
	.4byte 0x00000053
	.4byte 0x0200caa8
	.4byte 0x00000054
	.4byte 0x0200cb68
	.4byte 0x00000055
	.4byte 0x0200cb98
	.4byte 0x00000056
	.4byte 0x0200cc40
	.4byte 0x00000057
	.4byte 0x0200ccd0
	.4byte 0x0200c928
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	movs	r0, #0
	bl 0x0200bb98
	adds	r6, r0, #0
	movs	r0, #8
	bl 0x0200bb98
	adds	r5, r0, #0
	bl 0x0200bd00
	bl 0x0200bb70
	movs	r1, #22
	movs	r0, #0
	bl 0x0200bbf0
	movs	r0, #10
	bl 0x0200bb68
	movs	r0, #152
	bl 0x0200bd20
	ldr	r1, [pc, #128]
	ldr	r2, [pc, #132]
	movs	r0, #0
	bl 0x0200bba0
	ldr	r1, [r5, #12]
	ldr	r2, [r6, #12]
	subs	r3, r1, r2
	cmp	r3, #0
	bge.n	.L_02000312
	subs	r3, r2, r1
.L_02000312:
	asrs	r3, r3, #14
	movs	r2, #128
	lsls	r3, r3, #14
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r6, #40]
	movs	r0, #0
	movs	r1, #7
	bl 0x0200bbf0
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	bl 0x0200baf8
	movs	r0, #10
	bl 0x0200ba78
	ldr	r1, [r6, #80]
	ldrb	r3, [r1, #9]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r0, #0
	bl 0x0200bbe0
	b.n	.L_02000350
.L_0200034a:
	movs	r0, #1
	bl 0x0200ba78
.L_02000350:
	ldr	r2, [r5, #12]
	ldr	r3, [r6, #12]
	asrs	r2, r2, #14
	asrs	r3, r3, #14
	cmp	r2, r3
	blt.n	.L_0200034a
	bl 0x0200bb78
	movs	r0, #159
	bl 0x0200bd20
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200b850
	movs	r0, #20
	bl 0x0200ba78
	bl 0x0200bd18
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x00033333
	.2byte 0x9999
	.2byte 0x0001
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {lr}
	ldr r3, [pc, #28]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #24]
	movs r1, #99
	bl 0x0200bca8
	movs r0, #53
	movs r1, #2
	bl 0x0200bc98
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x0000004d
	.global Func_020003b4
	.thumb_func
Func_020003b4:
	push {lr}
	ldr r3, [pc, #28]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #24]
	movs r1, #99
	bl 0x0200bca8
	movs r0, #53
	movs r1, #2
	bl 0x0200bc98
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x0000004f
	.global Func_020003e0
	.thumb_func
Func_020003e0:
	push {lr}
	ldr r3, [pc, #28]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #24]
	movs r1, #99
	bl 0x0200bca8
	movs r0, #53
	movs r1, #2
	bl 0x0200bc98
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x00000051
	.global Func_0200040c
	.thumb_func
Func_0200040c:
	push {r5, r6, r7, lr}
	movs r0, #10
	sub sp, #8
	bl 0x0200bb98
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200040c_0
	movs r3, #1
	movs r7, #24
	movs r6, #26
	movs r0, #24
	movs r1, #27
	movs r2, #2
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb28
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #25
	bne .L_0200040c_1
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl 0x0200bb28
	b .L_0200040c_2
.L_0200040c_1:
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb28
.L_0200040c_2:
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bb30
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	bl 0x0200baf0
	movs r0, #1
	bl 0x0200ba78
.L_0200040c_0:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_0200047c
	.thumb_func
Func_0200047c:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl 0x0200bb98
	movs r3, #9
	movs r2, #13
	adds r5, r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	cmp r5, #0
	beq .L_0200047c_0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bb30
	ldr r3, [r5, #12]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_0200047c_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb58
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffe00000
	.global Func_020004cc
	.thumb_func
Func_020004cc:
	push {r5, lr}
	movs r0, #9
	sub sp, #8
	bl 0x0200bb98
	movs r3, #17
	movs r2, #13
	adds r5, r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #29
	movs r1, #1
	movs r2, #3
	movs r3, #1
	bl 0x0200bb28
	cmp r5, #0
	beq .L_020004cc_0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
.L_020004cc_0:
	ldr r0, [pc, #12]
	bl 0x0200bb58
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000201
	.global Func_0200050c
	.thumb_func
Func_0200050c:
	push {r5, lr}
	movs r0, #9
	sub sp, #8
	bl 0x0200bb98
	movs r3, #26
	adds r5, r0, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	cmp r5, #0
	beq .L_0200050c_0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bb30
	ldr r3, [r5, #12]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_0200050c_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb58
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffe00000
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	push {r5, lr}
	movs r0, #9
	sub sp, #8
	bl 0x0200bb98
	movs r3, #25
	movs r2, #13
	adds r5, r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	cmp r5, #0
	beq .L_0200055c_0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bb30
	ldr r3, [r5, #12]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_0200055c_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb58
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffe00000
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	push {r5, lr}
	movs r0, #9
	sub sp, #8
	bl 0x0200bb98
	movs r3, #43
	movs r2, #41
	adds r5, r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #45
	movs r1, #41
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	cmp r5, #0
	beq .L_020005ac_0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bb30
	ldr r3, [r5, #12]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_020005ac_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb58
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffe00000
	.global Func_020005fc
	.thumb_func
Func_020005fc:
	push {r5, lr}
	movs r0, #11
	sub sp, #8
	bl 0x0200bb98
	movs r3, #17
	movs r2, #10
	adds r5, r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	cmp r5, #0
	beq .L_020005fc_0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bb30
	ldr r3, [r5, #12]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_020005fc_0:
	ldr r0, [pc, #16]
	bl 0x0200bb58
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffe00000
	.4byte 0x00000201
	.global Func_02000650
	.thumb_func
Func_02000650:
	push {r5, lr}
	movs r0, #12
	sub sp, #8
	bl 0x0200bb98
	movs r3, #26
	movs r2, #15
	adds r5, r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	cmp r5, #0
	beq .L_02000650_0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bb30
	ldr r3, [r5, #12]
	ldr r2, [pc, #28]
	adds r3, r3, r2
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02000650_0:
	movs r0, #129
	lsls r0, r0, #2
	bl 0x0200bb58
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffe00000
	.global Func_020006a0
	.thumb_func
Func_020006a0:
	push {r5, r6, lr}
	movs r6, #128
	lsls r6, r6, #19
	ldrh r2, [r6]
	ldr r3, [pc, #40]
	ands r3, r2
	lsls r3, r3, #16
	asrs r5, r3, #16
	bl 0x0200ba90
	movs r3, #100
	muls r3, r0
	ldr r2, [pc, #28]
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bcc .L_020006a0_0
	movs r3, #128
	lsls r3, r3, #2
	orrs r5, r3
.L_020006a0_0:
	lsls r3, r5, #16
	lsrs r3, r3, #16
	strh r3, [r6]
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0000fdff
	.4byte 0x0200d238
	.global Func_020006dc
	.thumb_func
Func_020006dc:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #176]
	movs r0, #230
	ldr r5, [r3]
	sub sp, #8
	bl 0x0200bd20
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200bb38
	movs r0, #10
	bl 0x0200bb68
	movs r2, #178
	lsls r2, r2, #1
	ldr r7, [pc, #140]
	adds r6, r5, r2
	movs r5, #0
.L_020006dc_1:
	ldr r3, [r6, #12]
	ldr r2, [pc, #136]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	bl 0x0200ba78
	cmp r5, #8
	bne .L_020006dc_0
	movs r0, #8
	bl 0x0200bb98
	str r7, [r0, #24]
	movs r0, #8
	bl 0x0200bb98
	movs r1, #152
	movs r2, #216
	str r7, [r0, #28]
	lsls r1, r1, #16
	movs r0, #8
	lsls r2, r2, #16
	bl 0x0200bbe8
	movs r0, #8
	ldr r1, [pc, #92]
	bl 0x0200bba8
.L_020006dc_0:
	adds r5, #1
	cmp r5, #23
	ble .L_020006dc_1
	ldr r2, [pc, #84]
	movs r0, #1
	movs r1, #0
	bl 0x0200bab8
	ldr r2, [pc, #80]
	ldr r3, [pc, #52]
	strh r3, [r2]
	adds r5, r2, #0
.L_020006dc_2:
	movs r0, #1
	bl 0x0200ba78
	ldrh r3, [r5]
	movs r2, #200
	adds r3, #1
	strh r3, [r5]
	lsls r2, r2, #15
	lsls r3, r3, #16
	cmp r3, r2
	bls .L_020006dc_2
	movs r0, #1
	bl 0x0200ba78
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200bab8
	ldr r0, [pc, #36]
	bl 0x0200bd20
	movs r0, #1
	movs r1, #1
	b .L_020006dc_3
	.4byte 0x00000000
	.4byte 0x03001e70
	.4byte 0x00001999
	.4byte 0xffff0000
	.4byte 0x0200bd48
	.4byte 0x020086a1
	.4byte 0x0200d238
	.4byte 0x00000121
.L_020006dc_3:
	negs r1, r1
	ldr r2, [pc, #48]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #30
	bl 0x0200bb68
	movs r3, #3
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #2
	movs r0, #0
	bl 0x0200bb28
	ldr r0, [pc, #16]
	bl 0x0200bb58
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0x000008fd
	.global Func_020007e8
	.thumb_func
Func_020007e8:
	push {r5, r6, lr}
	ldr r3, [pc, #152]
	sub sp, #8
	ldr r5, [r3]
	movs r2, #28
	movs r3, #77
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #4
	movs r1, #41
	movs r2, #16
	movs r0, #93
	bl 0x0200bb20
	movs r0, #230
	bl 0x0200bd20
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl 0x0200bb38
	movs r0, #10
	bl 0x0200bb68
	movs r2, #178
	lsls r2, r2, #1
	adds r5, r5, r2
	movs r6, #23
.L_020007e8_0:
	ldr r3, [r5, #12]
	ldr r2, [pc, #92]
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #4
	subs r6, #1
	bl 0x0200ba78
	cmp r6, #0
	bge .L_020007e8_0
	ldr r2, [pc, #76]
	movs r0, #1
	movs r1, #0
	bl 0x0200bab8
	ldr r2, [pc, #72]
	ldr r3, [pc, #52]
	strh r3, [r2]
	adds r5, r2, #0
.L_020007e8_1:
	movs r0, #1
	bl 0x0200ba78
	ldrh r3, [r5]
	movs r2, #200
	adds r3, #1
	strh r3, [r5]
	lsls r2, r2, #15
	lsls r3, r3, #16
	cmp r3, r2
	bls .L_020007e8_1
	movs r0, #1
	bl 0x0200ba78
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200bab8
	ldr r0, [pc, #28]
	bl 0x0200bd20
	movs r0, #1
	movs r1, #1
	b .L_020007e8_2
	.4byte 0x00000000
	.4byte 0x03001e70
	.4byte 0xffff0000
	.4byte 0x020086a1
	.4byte 0x0200d238
	.4byte 0x00000121
.L_020007e8_2:
	negs r1, r1
	ldr r2, [pc, #48]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #30
	bl 0x0200bb68
	movs r3, #77
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #41
	movs r2, #16
	movs r3, #4
	movs r0, #77
	bl 0x0200bb20
	ldr r0, [pc, #16]
	bl 0x0200bb58
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0x000008fe
	.global Func_020008d4
	.thumb_func
Func_020008d4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, [pc, #188]
	ldr r3, [r3]
	sub sp, #8
	movs r5, #1
	mov r8, r3
	movs r0, #113
	movs r1, #31
	movs r2, #103
	movs r3, #17
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bb08
	movs r3, #3
	str r3, [sp, #0]
	movs r6, #2
	movs r0, #111
	movs r1, #32
	movs r2, #104
	movs r3, #18
	str r6, [sp, #4]
	bl 0x0200bb08
	movs r3, #18
	movs r1, #32
	movs r2, #103
	movs r0, #64
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200bb08
	movs r0, #230
	bl 0x0200bd20
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bb38
	movs r0, #10
	bl 0x0200bb68
	movs r5, #178
	lsls r5, r5, #1
	add r5, r8
	movs r6, #23
.L_020008d4_0:
	ldr r3, [r5, #12]
	ldr r2, [pc, #92]
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #4
	subs r6, #1
	bl 0x0200ba78
	cmp r6, #0
	bge .L_020008d4_0
	ldr r2, [pc, #76]
	movs r0, #1
	movs r1, #0
	bl 0x0200bab8
	ldr r2, [pc, #72]
	ldr r3, [pc, #52]
	strh r3, [r2]
	adds r5, r2, #0
.L_020008d4_1:
	movs r0, #1
	bl 0x0200ba78
	ldrh r3, [r5]
	movs r2, #200
	adds r3, #1
	strh r3, [r5]
	lsls r2, r2, #15
	lsls r3, r3, #16
	cmp r3, r2
	bls .L_020008d4_1
	movs r0, #1
	bl 0x0200ba78
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200bab8
	ldr r0, [pc, #28]
	bl 0x0200bd20
	movs r0, #1
	movs r1, #1
	b .L_020008d4_2
	.4byte 0x00000000
	.4byte 0x03001e70
	.4byte 0xffff0000
	.4byte 0x020086a1
	.4byte 0x0200d238
	.4byte 0x00000121
.L_020008d4_2:
	negs r1, r1
	ldr r2, [pc, #52]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #30
	bl 0x0200bb68
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #14
	movs r2, #103
	movs r3, #17
	movs r0, #103
	bl 0x0200bb08
	ldr r0, [pc, #20]
	bl 0x0200bb58
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0x00000907
	.global Func_020009ec
	.thumb_func
Func_020009ec:
	push {lr}
	ldr r0, [pc, #112]
	sub sp, #8
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020009ec_0
	movs r3, #24
	movs r2, #80
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #1
	movs r2, #24
	movs r3, #11
	bl 0x0200bb08
	ldr r0, [pc, #60]
	bl 0x0200bb60
	b .L_020009ec_1
.L_020009ec_0:
	movs r3, #24
	movs r2, #80
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #1
	movs r2, #24
	movs r3, #11
	bl 0x0200bb08
	ldr r0, [pc, #12]
	bl 0x0200bb58
.L_020009ec_1:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000323
	.global Func_02000a64
	.thumb_func
Func_02000a64:
	push {lr}
	bl 0x0200bb70
	movs r1, #1
	ldr r0, [pc, #32]
	bl 0x0200bb48
	movs r0, #125
	bl 0x0200bd20
	bl 0x020089ec
	movs r0, #20
	bl 0x0200ba78
	bl 0x0200bd18
	bl 0x0200bb78
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001528
	.global Func_02000a94
	.thumb_func
Func_02000a94:
	push {lr}
	ldr r0, [pc, #112]
	sub sp, #8
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02000a94_0
	movs r3, #11
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #32
	movs r2, #11
	movs r3, #4
	bl 0x0200bb08
	ldr r0, [pc, #60]
	bl 0x0200bb60
	b .L_02000a94_1
.L_02000a94_0:
	movs r3, #11
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #32
	movs r2, #11
	movs r3, #4
	bl 0x0200bb08
	ldr r0, [pc, #12]
	bl 0x0200bb58
.L_02000a94_1:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000325
	.global Func_02000b0c
	.thumb_func
Func_02000b0c:
	push {lr}
	bl 0x0200bb70
	movs r1, #1
	ldr r0, [pc, #32]
	bl 0x0200bb48
	movs r0, #125
	bl 0x0200bd20
	bl 0x02008a94
	movs r0, #20
	bl 0x0200ba78
	bl 0x0200bd18
	bl 0x0200bb78
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001528
	.global Func_02000b3c
	.thumb_func
Func_02000b3c:
	push {lr}
	sub sp, #8
	bl 0x0200bb70
	ldr r0, [pc, #136]
	movs r1, #1
	bl 0x0200bb48
	movs r0, #125
	bl 0x0200bd20
	ldr r0, [pc, #128]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02000b3c_0
	movs r3, #16
	movs r2, #92
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #29
	movs r2, #16
	movs r3, #28
	bl 0x0200bb08
	ldr r0, [pc, #76]
	bl 0x0200bb60
	b .L_02000b3c_1
.L_02000b3c_0:
	movs r3, #16
	movs r2, #92
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #29
	movs r2, #16
	movs r3, #28
	bl 0x0200bb08
	ldr r0, [pc, #28]
	bl 0x0200bb58
.L_02000b3c_1:
	movs r0, #20
	bl 0x0200ba78
	bl 0x0200bd18
	bl 0x0200bb78
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001528
	.4byte 0x00000326
	.global Func_02000bd8
	.thumb_func
Func_02000bd8:
	push {lr}
	sub sp, #8
	bl 0x0200bb70
	ldr r0, [pc, #136]
	movs r1, #1
	bl 0x0200bb48
	movs r0, #125
	bl 0x0200bd20
	ldr r0, [pc, #128]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02000bd8_0
	movs r3, #29
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #28
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #28
	movs r2, #29
	movs r3, #17
	bl 0x0200bb08
	ldr r0, [pc, #76]
	bl 0x0200bb60
	b .L_02000bd8_1
.L_02000bd8_0:
	movs r3, #29
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #28
	movs r2, #29
	movs r3, #17
	bl 0x0200bb08
	ldr r0, [pc, #28]
	bl 0x0200bb58
.L_02000bd8_1:
	movs r0, #20
	bl 0x0200ba78
	bl 0x0200bd18
	bl 0x0200bb78
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001528
	.4byte 0x00000327
	.global Func_02000c74
	.thumb_func
Func_02000c74:
	push {lr}
	movs r0, #10
	bl 0x0200bb98
	movs r3, #3
	adds r0, #35
	strb r3, [r0]
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000c88
	.thumb_func
Func_02000c88:
	push {lr}
	movs r0, #10
	bl 0x0200bb98
	movs r3, #1
	adds r0, #35
	strb r3, [r0]
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000c9c
	.thumb_func
Func_02000c9c:
	push {lr}
	sub sp, #8
	bl 0x0200bb70
	movs r3, #24
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r1, #27
	movs r2, #2
	movs r0, #24
	bl 0x0200bb28
	movs r0, #185
	bl 0x0200bd20
	movs r0, #10
	ldr r1, [pc, #96]
	ldr r2, [pc, #100]
	bl 0x0200bba0
	ldr r1, [pc, #88]
	ldr r2, [pc, #92]
	movs r0, #0
	bl 0x0200bba0
	movs r0, #10
	bl 0x0200bb98
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #8
	movs r0, #0
	bl 0x0200bbf0
	movs r1, #200
	movs r2, #212
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bbc0
	movs r1, #204
	movs r2, #212
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200bbc0
	movs r0, #10
	bl 0x0200bbe0
	movs r0, #0
	movs r1, #1
	bl 0x0200bbf0
	bl 0x0200840c
	bl 0x0200bb78
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00003333
	.4byte 0x00001999
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #10
	sub	sp, #4
	bl 0x0200bb98
	ldr	r3, [r0, #12]
	ldr	r2, [r0, #8]
	ldr	r6, [r0, #80]
	mov	r9, r2
	str	r3, [sp, #0]
	mov	sl, r0
	bl 0x0200bb70
	movs	r0, #141
	bl 0x0200bd20
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200bb38
	movs	r0, #10
	bl 0x0200bb68
	ldr	r0, [pc, #320]
	bl 0x0200bd20
	movs	r0, #1
	movs	r1, #1
	ldr	r2, [pc, #312]
	negs	r0, r0
	negs	r1, r1
	bl 0x0200bb38
	movs	r0, #20
	bl 0x0200bb68
	ldr	r2, [pc, #300]
	movs	r7, #0
	mov	r8, r2
.L_02000d8e:
	movs	r3, #128
	lsls	r3, r3, #12
	ldrh	r2, [r6, #30]
	adds	r7, r7, r3
	lsrs	r3, r7, #16
	adds	r3, r3, r2
	strh	r3, [r6, #30]
	movs	r2, #128
	ldrh	r0, [r6, #30]
	lsls	r2, r2, #7
	adds	r0, r0, r2
	bl 0x0200baa8
	adds	r5, r0, #0
	lsls	r3, r5, #4
	add	r3, r9
	mov	r2, sl
	str	r3, [r2, #8]
	ldrh	r1, [r6, #30]
	cmp	r1, r8
	bhi.n	.L_02000dc0
	movs	r0, #1
	bl 0x0200ba78
	b.n	.L_02000d8e
.L_02000dc0:
	movs	r3, #224
	lsls	r3, r3, #7
	movs	r7, #0
	mov	r8, r3
.L_02000dc8:
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r7, r7, r2
	lsrs	r3, r7, #16
	subs	r3, r1, r3
	strh	r3, [r6, #30]
	movs	r3, #128
	ldrh	r0, [r6, #30]
	lsls	r3, r3, #7
	adds	r0, r0, r3
	bl 0x0200baa8
	adds	r5, r0, #0
	lsls	r3, r5, #4
	add	r3, r9
	mov	r2, sl
	str	r3, [r2, #8]
	ldrh	r1, [r6, #30]
	cmp	r1, r8
	bls.n	.L_02000dfa
	movs	r0, #1
	bl 0x0200ba78
	ldrh	r1, [r6, #30]
	b.n	.L_02000dc8
.L_02000dfa:
	movs	r3, #128
	movs	r7, #128
	lsls	r3, r3, #8
	lsls	r7, r7, #12
	mov	fp, r3
.L_02000e04:
	lsrs	r2, r7, #19
	lsrs	r3, r7, #16
	adds	r3, r3, r2
	lsls	r3, r3, #16
	adds	r7, r3, #0
	lsrs	r2, r7, #16
	adds	r3, r2, r1
	strh	r3, [r6, #30]
	movs	r3, #128
	ldrh	r0, [r6, #30]
	lsls	r3, r3, #7
	adds	r0, r0, r3
	mov	r8, r2
	bl 0x0200baa8
	adds	r5, r0, #0
	ldrh	r0, [r6, #30]
	add	r0, fp
	bl 0x0200baa0
	lsls	r3, r5, #4
	add	r3, r9
	mov	r2, sl
	str	r3, [r2, #8]
	ldrh	r3, [r6, #30]
	cmp	r3, fp
	bls.n	.L_02000e44
	ldr	r2, [sp, #0]
	lsls	r3, r0, #3
	subs	r3, r2, r3
	mov	r2, sl
	str	r3, [r2, #12]
.L_02000e44:
	ldrh	r3, [r6, #30]
	ldr	r2, [pc, #116]
	add	r3, r8
	cmp	r3, r2
	bgt.n	.L_02000e58
	movs	r0, #1
	bl 0x0200ba78
	ldrh	r1, [r6, #30]
	b.n	.L_02000e04
.L_02000e58:
	movs	r0, #1
	bl 0x0200ba78
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r6, #30]
	movs	r0, #183
	bl 0x0200bd20
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200bb38
	movs	r0, #20
	bl 0x0200bb68
	ldr	r0, [pc, #44]
	bl 0x0200bd20
	movs	r0, #1
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #36]
	negs	r0, r0
	bl 0x0200bb38
	movs	r0, #5
	bl 0x02008ec0
	bl 0x0200bb78
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00008fff
	.2byte 0xbfff
	.2byte 0x0000
	.global Func_02000ec0
	.thumb_func
Func_02000ec0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	movs r0, #0
	bl 0x0200bb98
	mov r8, r0
	movs r0, #8
	bl 0x0200bb98
	adds r7, r0, #0
	movs r0, #9
	bl 0x0200bb98
	mov r10, r0
	movs r0, #10
	bl 0x0200bb98
	movs r1, #129
	adds r6, r0, #0
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200bc68
	movs r0, #40
	bl 0x0200bb68
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200bc78
	movs r0, #196
	movs r1, #1
	movs r2, #232
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200bc80
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #0
	lsls r1, r1, #10
	bl 0x0200bba0
	movs r0, #0
	movs r1, #6
	bl 0x0200bbf0
	movs r1, #198
	movs r2, #140
	movs r0, #0
	lsls r1, r1, #2
	bl 0x0200bbc8
	movs r0, #0
	movs r1, #1
	bl 0x0200bbf0
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #100
	bl 0x0200bc50
	ldr r1, [pc, #952]
	movs r2, #60
	movs r0, #0
	bl 0x0200bc60
	movs r0, #183
	bl 0x0200bd20
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200bb38
	movs r0, #20
	bl 0x0200bb68
	ldr r3, [pc, #916]
	mov r0, r10
	str r3, [r0, #24]
	str r3, [r0, #28]
	mov r1, r10
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	ldr r3, [pc, #900]
	mov r2, r10
	movs r0, #0
	str r3, [r2, #108]
	movs r1, #4
	mov r11, r0
	movs r0, #8
	bl 0x0200bbf0
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #68]
	ldr r3, [pc, #880]
	str r3, [r7, #8]
	mov r9, r3
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r7, #12]
	movs r5, #128
	movs r3, #180
	lsls r3, r3, #15
	lsls r5, r5, #10
	str r3, [r7, #16]
	movs r0, #10
	str r5, [r7, #24]
	str r5, [r7, #28]
	bl 0x0200bb68
	movs r0, #183
	bl 0x0200bd20
	movs r0, #128
	movs r2, #128
	adds r1, r5, #0
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200bb38
	movs r0, #20
	bl 0x0200bb68
	ldr r3, [r6, #8]
	movs r0, #224
	lsls r0, r0, #12
	adds r3, r3, r0
	str r3, [r6, #8]
	ldr r2, [pc, #816]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, [r6, #80]
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #107
	bl 0x0200bd20
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200bb38
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #80
	movs r0, #0
	bl 0x0200bc60
	movs r0, #55
	bl 0x0200bd20
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #10
	bl 0x0200bb38
	movs r0, #8
	movs r1, #0
	bl 0x0200bc58
	movs r0, #0
	movs r1, #0
	bl 0x0200bc58
	movs r0, #0
	ldr r1, [pc, #712]
	bl 0x0200bc68
	movs r1, #160
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	mov r6, r8
	bl 0x0200bba0
	adds r6, #100
	mov r3, r11
	movs r0, #0
	strh r3, [r6]
	ldr r1, [pc, #700]
	bl 0x0200bba8
	ldr r0, [pc, #700]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02000ec0_0
	movs r2, #132
	movs r0, #1
	ldr r1, [pc, #688]
	lsls r2, r2, #18
	bl 0x0200bbe8
	movs r0, #1
	bl 0x0200bb98
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
.L_02000ec0_0:
	movs r0, #160
	movs r1, #160
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200bc78
	movs r1, #1
	movs r2, #139
	lsls r2, r2, #18
	movs r3, #1
	negs r1, r1
	mov r0, r9
	bl 0x0200bc80
	ldr r0, [sp, #8]
	bl 0x0200bb68
	movs r0, #8
	movs r1, #1
	bl 0x0200bc58
	adds r5, r7, #0
	movs r0, #8
	ldr r1, [pc, #628]
	ldr r2, [pc, #628]
	bl 0x0200bba0
	adds r5, #100
	mov r0, r11
	strh r0, [r5]
	ldr r1, [pc, #620]
	movs r0, #8
	bl 0x0200bba8
.L_02000ec0_1:
	movs r0, #1
	bl 0x0200ba78
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_02000ec0_1
	movs r0, #0
	movs r1, #0
	bl 0x0200bc68
.L_02000ec0_2:
	movs r0, #1
	bl 0x0200ba78
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #0
	beq .L_02000ec0_2
	movs r1, #2
	movs r0, #0
	bl 0x0200bc58
	movs r0, #0
	bl 0x0200bb98
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #556]
	bl 0x0200bd20
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #548]
	bl 0x0200bb38
	ldr r3, [pc, #504]
.L_02001122:
	mov r2, r10
	str r3, [r2, #8]
	ldr r6, [pc, #540]
	ldr r3, [pc, #540]
	movs r5, #0
	str r5, [r2, #108]
	str r3, [r2, #16]
	str r6, [r2, #12]
	movs r0, #8
	ldr r1, [pc, #532]
	ldr r2, [pc, #536]
	bl 0x0200bba0
	ldr r3, [pc, #532]
	str r3, [r7, #68]
	ldr r3, [pc, #532]
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #11
	movs r2, #151
	str r3, [r7, #40]
	movs r0, #8
	ldr r1, [pc, #524]
	lsls r2, r2, #2
	mov r8, r3
	bl 0x0200bbc8
	movs r0, #8
	ldr r1, [pc, #516]
	ldr r2, [pc, #492]
	bl 0x0200bba0
	movs r2, #161
	ldr r1, [pc, #500]
	lsls r2, r2, #2
	movs r0, #8
	bl 0x0200bbc0
	movs r0, #15
	bl 0x0200bb68
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl 0x0200bb38
	movs r3, #11
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #36
	movs r2, #43
	movs r3, #36
	bl 0x0200bb08
	movs r3, #43
	movs r2, #35
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #5
	movs r0, #25
	movs r1, #35
	movs r2, #10
	bl 0x0200bb28
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl 0x0200bbe8
	ldr r5, [pc, #416]
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200ba80
	movs r0, #80
	bl 0x0200bb68
	adds r0, r5, #0
	bl 0x0200ba88
	movs r0, #60
	bl 0x0200bb68
	movs r0, #17
	bl 0x0200bd20
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #336]
	bl 0x0200bb38
	movs r0, #120
	bl 0x0200bb68
	ldr r0, [pc, #300]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001122_0
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bba0
	movs r1, #206
	movs r0, #1
	lsls r1, r1, #2
	ldr r2, [pc, #336]
	bl 0x0200bbd0
.L_02001122_0:
	movs r0, #0
	ldr r1, [pc, #332]
	ldr r2, [pc, #332]
	bl 0x0200bba0
	movs r2, #146
	movs r0, #0
	ldr r1, [pc, #328]
	lsls r2, r2, #2
	bl 0x0200bbd8
	ldr r0, [pc, #244]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001122_1
	movs r0, #1
	movs r1, #1
	bl 0x0200bbf0
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bc50
.L_02001122_1:
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200bc50
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bc60
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bc60
	movs r0, #197
	adds r1, r6, #0
	ldr r2, [pc, #252]
	movs r3, #1
	lsls r0, r0, #18
	bl 0x0200bc80
	bl 0x0200bc88
	movs r0, #148
	bl 0x0200bd20
	movs r0, #240
	bl 0x0200bb68
	ldr r0, [pc, #148]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001122_2
	movs r1, #128
	mov r0, r8
	lsls r1, r1, #8
	bl 0x0200bc78
	movs r2, #146
	movs r3, #1
	ldr r0, [pc, #208]
	movs r1, #0
	lsls r2, r2, #18
	bl 0x0200bc80
	bl 0x0200bc88
	movs r1, #210
	movs r2, #138
	movs r0, #1
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200bbd8
	movs r0, #1
	ldr r1, [pc, #168]
	ldr r2, [pc, #180]
	bl 0x0200bbd8
	movs r0, #1
	movs r1, #2
	bl 0x0200bbf0
	movs r0, #0
	bl 0x0200bb98
	cmp r0, #0
	beq .L_02001122_3
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200bbc0
.L_02001122_3:
	movs r0, #1
	bl 0x0200bbe0
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
.L_02001122_2:
	bl 0x0200bd08
	ldr r0, [pc, #124]
	bl 0x0200bb58
	sub sp, #-12
	b .L_02001122_4
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0x8099
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x0312
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xbdec
	.2byte 0x0200
	.4byte 0x00000205
	.2byte 0x0000
	.2byte 0x036e
	.2byte 0x95c2
	.2byte 0x0001
	.2byte 0xcae1
	.2byte 0x0000
	.2byte 0xbd78
	.2byte 0x0200
	.2byte 0x0121
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0xffc00000
	.4byte 0x026a0000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x00003333
	.4byte 0x00000312
	.4byte 0x00033333
	.4byte 0x0200abe1
	.4byte 0x0000022e
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00000356
	.4byte 0x02620000
	.4byte 0x03560000
	.4byte 0x00000232
	.4byte 0x00000908
.L_02001122_4:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02001398
	.thumb_func
Func_02001398:
	push {r5, r6, lr}
	bl 0x0200bb70
	ldr r5, [pc, #684]
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200bb48
	ldr r0, [pc, #676]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001398_0
	b 0x02009640
.L_02001398_0:
	ldr r0, [pc, #668]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001398_1
	b 0x02009640
.L_02001398_1:
	ldr r0, [pc, #660]
	bl 0x0200bb58
	movs r0, #0
	ldr r1, [pc, #656]
	ldr r2, [pc, #660]
	bl 0x0200bba0
	movs r0, #0
	ldr r1, [pc, #656]
	movs r2, #140
	bl 0x0200bbd8
	movs r1, #195
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #140
	bl 0x0200bbd8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r0, #0
	bl 0x0200bb98
	cmp r0, #0
	beq .L_02001398_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200bbe8
.L_02001398_2:
	movs r0, #1
	ldr r1, [pc, #592]
	ldr r2, [pc, #596]
	bl 0x0200bba0
	movs r1, #200
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #140
	bl 0x0200bbd8
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #1
	bl 0x0200bc50
	adds r0, r5, #1
	bl 0x0200bc30
	movs r1, #4
	movs r0, #1
	bl 0x0200bbf0
	movs r0, #20
	bl 0x0200bb68
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200bc48
	movs r0, #1
	movs r1, #6
	movs r2, #0
	bl 0x0200bc00
	ldr r1, [pc, #532]
	ldr r2, [pc, #520]
	movs r0, #1
	bl 0x0200bba0
	movs r0, #1
	bl 0x0200bb98
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
.L_02001466:
	adds r3, r5, #0
	ands r3, r2
	movs r1, #198
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #110
	movs r0, #1
	bl 0x0200bbd8
	movs r0, #1
	bl 0x0200bb68
	movs r0, #1
	bl 0x0200bb98
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #161
	bl 0x0200bd20
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200bb38
	movs r0, #1
	bl 0x0200bb98
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #198
	ands r5, r3
	lsls r1, r1, #2
	movs r2, #120
	strb r5, [r0]
	movs r0, #1
	bl 0x0200bbd8
	movs r0, #1
	bl 0x0200bb68
	movs r0, #1
	bl 0x0200bb98
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r1, #1
	movs r0, #1
	negs r1, r1
	ldr r2, [pc, #400]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #80
	bl 0x0200bb68
	movs r0, #141
	bl 0x0200bd20
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200bb38
	movs r0, #40
	bl 0x0200bb68
	movs r0, #0
	ldr r1, [pc, #360]
	movs r2, #0
	bl 0x0200bc60
	movs r0, #1
	ldr r1, [pc, #352]
	movs r2, #60
	bl 0x0200bc60
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200bc50
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200bc50
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bc50
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r0, #1
	movs r1, #0
	movs r2, #40
	bl 0x0200bc50
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bc50
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bc60
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200bc50
	movs r0, #1
	movs r1, #2
	bl 0x0200bc08
	movs r0, #1
	movs r1, #0
	movs r2, #10
	bl 0x0200bc48
	movs r1, #160
	movs r2, #160
	lsls r2, r2, #9
	movs r0, #1
	lsls r1, r1, #10
	bl 0x0200bba0
	movs r0, #1
	movs r1, #5
	bl 0x0200bbf0
	movs r1, #199
	movs r0, #1
	lsls r1, r1, #2
.L_020015b6:
	movs r2, #138
	bl 0x0200bbc8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200bc50
	movs r1, #201
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #140
	bl 0x0200bbc8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bc50
	movs r1, #201
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #166
	bl 0x0200bbc8
	movs r1, #191
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #166
	bl 0x0200bbc8
	movs r1, #191
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #198
	bl 0x0200bbc8
	movs r0, #1
	ldr r1, [pc, #108]
	movs r2, #198
	bl 0x0200bbc8
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bc60
	movs r2, #246
	movs r0, #1
	ldr r1, [pc, #84]
	bl 0x0200bbc8
	movs r0, #1
	movs r1, #1
	bl 0x0200bbf0
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	movs r0, #40
	bl 0x0200bb68
	movs r0, #10
	bl 0x02008ec0
	bl 0x0200bb78
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1953
	.2byte 0x0000
	.2byte 0x0908
	.2byte 0x0000
	.2byte 0x0f14
	.2byte 0x0000
	.2byte 0x0205
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0316
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.4byte 0x00000312
	.global Func_02001678
	.thumb_func
Func_02001678:
	push {lr}
	ldr r3, [pc, #140]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #132]
	cmp r2, r3
	bne .L_02001678_0
	ldr r0, [pc, #128]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02001678_1
	ldr r0, [pc, #124]
	b .L_02001678_2
.L_02001678_1:
	ldr r0, [pc, #124]
	b .L_02001678_2
.L_02001678_0:
	ldr r3, [pc, #124]
	cmp r2, r3
	bne .L_02001678_3
	ldr r0, [pc, #120]
	b .L_02001678_2
.L_02001678_3:
	ldr r3, [pc, #120]
	cmp r2, r3
	bne .L_02001678_4
	ldr r0, [pc, #120]
	b .L_02001678_2
.L_02001678_4:
	ldr r3, [pc, #120]
	cmp r2, r3
	bne .L_02001678_5
	ldr r0, [pc, #116]
	b .L_02001678_2
.L_02001678_5:
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_02001678_6
	ldr r0, [pc, #116]
	b .L_02001678_2
.L_02001678_6:
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_02001678_7
	ldr r0, [pc, #112]
	b .L_02001678_2
.L_02001678_7:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_02001678_8
	ldr r0, [pc, #112]
	b .L_02001678_2
.L_02001678_8:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_02001678_9
	ldr r0, [pc, #108]
	b .L_02001678_2
.L_02001678_9:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_02001678_10
	ldr r0, [pc, #108]
	b .L_02001678_2
.L_02001678_10:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_02001678_11
	ldr r0, [pc, #104]
	b .L_02001678_2
.L_02001678_11:
	ldr r3, [pc, #104]
	cmp r2, r3
	bne .L_02001678_12
	ldr r0, [pc, #104]
	b .L_02001678_2
.L_02001678_12:
	ldr r0, [pc, #104]
.L_02001678_2:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x000008fd
	.4byte 0x0200cd6c
	.4byte 0x0200cd24
	.4byte 0x0000004e
	.4byte 0x0200cd9c
	.4byte 0x0000004f
	.4byte 0x0200cdc0
	.4byte 0x00000050
	.4byte 0x0200ce5c
	.4byte 0x00000051
	.4byte 0x0200cebc
	.4byte 0x00000052
	.4byte 0x0200cf34
	.4byte 0x00000053
	.4byte 0x0200cfb8
	.4byte 0x00000054
	.4byte 0x0200d06c
	.4byte 0x00000055
	.4byte 0x0200d0cc
	.4byte 0x00000056
	.4byte 0x0200d12c
	.4byte 0x00000057
	.4byte 0x0200d150
	.4byte 0x0200cd18
	.global Func_02001770
	.thumb_func
Func_02001770:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #102
	movs r1, #0
	ldrsh r3, [r6, r1]
	ldrh r2, [r6]
	cmp r3, #0
	beq .L_02001770_0
	subs r3, r2, #1
	movs r2, #128
	strh r3, [r6]
	lsls r2, r2, #9
	lsls r3, r3, #16
	cmp r3, r2
	bne .L_02001770_0
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #148]
	bl 0x0200bb38
.L_02001770_0:
	ldr r7, [r5, #40]
	cmp r7, #0
	bne .L_02001770_1
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200bad0
	ldr r3, [r5, #12]
	ldr r1, [pc, #132]
	ldr r2, [r5, #20]
	adds r3, r3, r1
	str r3, [r5, #12]
	cmp r3, r2
	bge .L_02001770_2
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_02001770_3
	movs r0, #229
	bl 0x0200bd20
	movs r3, #4
	movs r0, #128
	movs r2, #128
	str r7, [r5, #104]
	lsls r2, r2, #9
	strh r3, [r6]
	lsls r0, r0, #9
	movs r1, #0
	bl 0x0200bb38
	ldr r2, [r5, #20]
.L_02001770_3:
	str r2, [r5, #12]
.L_02001770_2:
	adds r2, r5, #0
	adds r2, #91
	movs r3, #1
	b .L_02001770_4
.L_02001770_1:
	adds r2, r5, #0
	adds r2, #91
	movs r3, #0
.L_02001770_4:
	strb r3, [r2]
	adds r6, r5, #0
	adds r6, #100
	movs r1, #0
	ldrsh r3, [r6, r1]
	ldrh r2, [r6]
	cmp r3, #0
	bne .L_02001770_5
	movs r0, #152
	bl 0x0200bd20
	movs r3, #1
	adds r0, r5, #0
	movs r1, #2
	str r3, [r5, #104]
	bl 0x0200bad0
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #40]
	ldrh r2, [r6]
.L_02001770_5:
	adds r3, r2, #1
	movs r2, #240
	strh r3, [r6]
	lsls r2, r2, #14
	lsls r3, r3, #16
	cmp r3, r2
	bne .L_02001770_6
	movs r3, #0
	strh r3, [r6]
.L_02001770_6:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0xfffe8000
	.global Func_02001838
	.thumb_func
Func_02001838:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #10
	sub sp, #8
	bl 0x0200bb98
	mov r9, r0
	bl 0x0200bb70
	ldr r0, [pc, #144]
	ldr r1, [pc, #148]
	bl 0x0200bc78
	movs r0, #149
	movs r1, #1
	ldr r2, [pc, #140]
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200bc80
	bl 0x0200bc88
	movs r0, #147
	bl 0x0200bd20
	movs r1, #2
	movs r0, #10
	bl 0x0200bc10
	movs r0, #40
	bl 0x0200bb68
	ldr r0, [pc, #108]
	ldr r1, [pc, #112]
	bl 0x0200bc78
	movs r1, #128
	movs r2, #212
	ldr r0, [pc, #104]
	lsls r1, r1, #14
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200bc80
	movs r5, #0
	mov r2, r9
	movs r3, #100
	str r5, [r2, #104]
	add r3, r9
	ldr r2, [pc, #60]
	strh r5, [r3]
	ldr r6, [pc, #84]
	mov r8, r3
	mov r7, r9
	ldr r3, [pc, #80]
	mov r10, r2
	adds r7, #102
	mov r2, r9
	strh r5, [r7]
	movs r0, #10
	str r3, [r2, #72]
	str r6, [r2, #108]
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	bl 0x0200bba0
	movs r1, #154
	movs r0, #10
	lsls r1, r1, #1
	ldr r2, [pc, #64]
	bl 0x0200bbc8
	ldr r1, [pc, #60]
	movs r0, #10
	movs r2, #215
	bl 0x0200bbc8
	mov r3, r9
	mov r2, r10
	b .L_02001838_0
	.4byte 0x00000000
	.4byte 0x00026666
	.4byte 0x00004ccc
	.4byte 0x01510000
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x01270000
	.4byte 0x02009771
	.4byte 0x00006666
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000123
	.4byte 0x00000137
.L_02001838_0:
	str r5, [r3, #108]
	adds r3, #91
	strb r2, [r3]
	movs r0, #16
	bl 0x0200bb68
	movs r1, #1
	movs r0, #10
	bl 0x0200bbf0
	movs r0, #229
	bl 0x0200bd20
	movs r0, #128
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200bb38
	movs r0, #4
	bl 0x0200bb68
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #924]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #40
	bl 0x0200bb68
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200bc50
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #40
	movs r0, #10
	bl 0x0200bc50
	bl 0x0200ad08
	movs r0, #40
	bl 0x0200bb68
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200bc50
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #40
	bl 0x0200bc50
	movs r0, #167
	movs r1, #1
	movs r2, #244
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200bc80
	mov r3, r9
	mov r2, r8
	movs r1, #160
	str r5, [r3, #104]
	movs r0, #10
	strh r5, [r2]
	lsls r1, r1, #1
	strh r5, [r7]
	movs r2, #232
	str r6, [r3, #108]
	bl 0x0200bbc8
	movs r1, #170
	movs r2, #131
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200bbc8
	movs r1, #187
	movs r2, #131
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200bbc8
	mov r3, r9
	str r5, [r3, #108]
	movs r0, #16
	bl 0x0200bb68
	movs r1, #1
	movs r0, #10
	bl 0x0200bbf0
	movs r0, #229
	bl 0x0200bd20
	movs r0, #128
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200bb38
	movs r0, #4
	bl 0x0200bb68
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #732]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #20
	bl 0x0200bb68
	movs r1, #240
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200bc50
	movs r1, #208
	movs r2, #40
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200bc50
	movs r0, #153
	bl 0x0200bd20
	movs r0, #10
	bl 0x0200bb98
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #2
	movs r0, #10
	bl 0x0200bbf0
	movs r1, #190
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #10
	bl 0x0200bbc8
	movs r0, #10
	bl 0x0200bb68
	movs r0, #229
	bl 0x0200bd20
	movs r0, #128
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200bb38
	movs r0, #4
	bl 0x0200bb68
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #620]
	negs r1, r1
	negs r0, r0
	bl 0x0200bb38
	movs r0, #6
	bl 0x0200bb68
	movs r1, #1
	movs r0, #10
	bl 0x0200bbf0
	movs r0, #40
	bl 0x0200bb68
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200bc50
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bc50
	movs r0, #152
	movs r1, #1
	movs r2, #215
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200bc80
	mov r3, r8
	mov r2, r9
	str r5, [r2, #104]
	movs r0, #10
	strh r5, [r3]
	ldr r1, [pc, #544]
	strh r5, [r7]
	str r6, [r2, #108]
	movs r2, #219
	bl 0x0200bbc8
	mov r2, r9
	str r5, [r2, #108]
	movs r1, #1
	movs r0, #10
	bl 0x0200bbf0
	movs r0, #16
	bl 0x0200bb68
	movs r0, #229
	bl 0x0200bd20
	movs r0, #128
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200bb38
	movs r0, #4
	bl 0x0200bb68
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #476]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #40
	bl 0x0200bb68
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #40
	movs r0, #10
	bl 0x0200bc50
	movs r0, #9
	bl 0x0200bb98
	mov r3, r10
	movs r2, #17
	adds r0, #85
	movs r5, #13
	strb r3, [r0]
	mov r9, r2
	str r2, [sp, #0]
	movs r0, #3
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200bb28
	movs r3, #18
	str r3, [sp, #0]
	mov r10, r3
	movs r0, #3
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200bb28
	movs r2, #19
	str r2, [sp, #0]
	movs r3, #1
	mov r8, r2
	movs r0, #3
	movs r1, #0
	movs r2, #1
	str r5, [sp, #4]
	bl 0x0200bb28
	ldr r2, [pc, #384]
	ldr r1, [pc, #388]
	movs r0, #10
	bl 0x0200bba0
	movs r0, #10
	bl 0x0200bb98
	movs r1, #0
	bl 0x0200bb30
	movs r0, #153
	bl 0x0200bd20
	movs r0, #10
	bl 0x0200bb98
	movs r6, #160
	lsls r6, r6, #11
	str r6, [r0, #40]
	movs r1, #3
	movs r0, #10
	bl 0x0200bbf0
	movs r2, #215
	movs r0, #10
	ldr r1, [pc, #340]
	bl 0x0200bbc8
	movs r1, #1
	movs r0, #10
	bl 0x0200bbf0
	movs r0, #10
	bl 0x0200bb98
	movs r1, #1
	bl 0x0200bb30
	movs r0, #229
	bl 0x0200bd20
	movs r0, #128
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200bb38
	movs r0, #4
	bl 0x0200bb68
	movs r0, #1
	movs r1, #1
	ldr r2, [pc, #268]
	negs r1, r1
	negs r0, r0
	bl 0x0200bb38
	movs r0, #40
	bl 0x0200bb68
	movs r0, #153
	bl 0x0200bd20
	movs r0, #10
	bl 0x0200bb98
	movs r1, #3
	str r6, [r0, #40]
	movs r0, #10
	bl 0x0200bbf0
	movs r1, #130
	movs r2, #215
	movs r0, #10
	lsls r1, r1, #1
	bl 0x0200bbc8
	movs r1, #1
	movs r0, #10
	bl 0x0200bbf0
	movs r0, #229
	bl 0x0200bd20
	movs r0, #128
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200bb38
	movs r0, #4
	bl 0x0200bb68
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #180]
	negs r0, r0
	bl 0x0200bb38
	movs r0, #40
	bl 0x0200bb68
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200bc50
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #6
	movs r0, #10
	bl 0x0200bc50
	movs r0, #147
	bl 0x0200bd20
	movs r1, #2
	movs r0, #10
	bl 0x0200bc10
	movs r0, #40
	bl 0x0200bb68
	mov r3, r9
	str r3, [sp, #0]
	movs r0, #4
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200bb28
	mov r2, r10
	str r2, [sp, #0]
	movs r0, #2
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200bb28
	mov r3, r8
	str r3, [sp, #0]
	movs r2, #1
	movs r3, #1
	movs r1, #0
	movs r0, #4
	str r5, [sp, #4]
	bl 0x0200bb28
	movs r0, #0
	bl 0x0200bb98
	ldr r1, [pc, #80]
	adds r5, r0, #0
	ldr r0, [pc, #80]
	bl 0x0200bc78
	movs r3, #1
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	bl 0x0200bc80
	bl 0x0200bc88
	movs r1, #128
	ldr r2, [pc, #60]
	lsls r1, r1, #9
	movs r0, #10
	bl 0x0200bc28
	ldr r0, [pc, #56]
	bl 0x0200bb58
	bl 0x0200bb78
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0000e666
	.4byte 0x00000149
	.4byte 0x0000b333
	.4byte 0x00016666
	.4byte 0x00000127
	.4byte 0x00009999
	.4byte 0x0004cccc
	.4byte 0x0200bd34
	.4byte 0x00000904
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #10
	bl 0x0200bb98
	adds	r7, r0, #0
	bl 0x0200bb70
	movs	r0, #10
	bl 0x0200bbb0
	ldr	r0, [pc, #172]
	ldr	r1, [pc, #176]
	bl 0x0200bc78
	movs	r1, #128
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r1, r1, #15
	ldr	r0, [pc, #164]
	bl 0x0200bc80
	bl 0x0200bc88
	movs	r0, #147
	bl 0x0200bd20
	movs	r1, #2
	movs	r0, #10
	bl 0x0200bc10
	movs	r0, #40
.L_02001d58:
	bl 0x0200bb68
	movs	r1, #192
	movs	r0, #10
	lsls	r1, r1, #6
	movs	r2, #20
	bl 0x0200bc50
	movs	r1, #160
	movs	r0, #10
	lsls	r1, r1, #7
	movs	r2, #20
	bl 0x0200bc50
	movs	r1, #128
	movs	r2, #40
	movs	r0, #10
	lsls	r1, r1, #8
	bl 0x0200bc50
	ldr	r0, [pc, #100]
	ldr	r1, [pc, #104]
	bl 0x0200bc78
	movs	r0, #128
	movs	r1, #128
	movs	r2, #202
	lsls	r0, r0, #16
	lsls	r1, r1, #15
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bc80
	ldr	r3, [pc, #60]
	mov	sl, r3
	movs	r3, #102
	adds	r3, r3, r7
	movs	r2, #100
	movs	r5, #0
	adds	r2, r2, r7
	mov	fp, r3
	ldr	r6, [pc, #68]
	ldr	r3, [pc, #68]
	str	r5, [r7, #104]
	mov	r8, r2
	strh	r5, [r2, #0]
	mov	r2, fp
	strh	r5, [r2, #0]
	movs	r0, #10
	str	r3, [r7, #72]
	ldr	r1, [pc, #56]
	ldr	r2, [pc, #60]
	str	r6, [r7, #108]
	bl 0x0200bba0
	movs	r0, #10
	movs	r1, #212
	movs	r2, #200
	bl 0x0200bbc8
	movs	r1, #103
	movs	r0, #10
	movs	r2, #200
	b.n	.L_02001e00
	.4byte 0x00000000
	.4byte 0x00026666
	.4byte 0x00004ccc
	.4byte 0x01170000
	.4byte 0x0000cccc
	.4byte 0x00001999
	.4byte 0x02009771
	.4byte 0x00006666
	.4byte 0x00013333
	.2byte 0x9999
	.2byte 0x0000
.L_02001e00:
	bl 0x0200bbc8
	movs	r3, #91
	adds	r3, r3, r7
	mov	r2, sl
	str	r5, [r7, #108]
	movs	r0, #10
	strb	r2, [r3, #0]
	mov	r9, r3
	bl 0x0200bb68
	movs	r1, #1
	movs	r0, #10
	bl 0x0200bbf0
	movs	r0, #229
	bl 0x0200bd20
	movs	r0, #128
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200bb38
	movs	r0, #4
	bl 0x0200bb68
	movs	r0, #1
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #456]
	negs	r0, r0
	bl 0x0200bb38
	movs	r0, #20
	bl 0x0200bb68
	movs	r1, #160
	lsls	r1, r1, #7
	movs	r2, #40
	movs	r0, #10
	bl 0x0200bc50
	movs	r0, #10
	bl 0x0200bb98
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r2, [pc, #416]
	ldr	r1, [pc, #420]
	movs	r0, #10
	bl 0x0200bba0
	movs	r0, #10
	bl 0x0200bb98
	movs	r1, #0
	bl 0x0200bb30
	movs	r0, #153
	bl 0x0200bd20
	movs	r0, #10
	bl 0x0200bb98
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r1, #3
	movs	r0, #10
	bl 0x0200bbf0
	movs	r2, #214
	movs	r0, #10
	movs	r1, #86
	bl 0x0200bbc8
	movs	r1, #1
	movs	r0, #10
	bl 0x0200bbf0
	movs	r0, #10
	bl 0x0200bb98
	movs	r1, #1
	bl 0x0200bb30
	movs	r0, #10
	bl 0x0200bb68
	movs	r0, #229
	bl 0x0200bd20
	movs	r0, #128
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200bb38
	movs	r0, #8
	bl 0x0200bb68
	movs	r0, #1
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #296]
	negs	r0, r0
	bl 0x0200bb38
	movs	r0, #40
	bl 0x0200bb68
	movs	r0, #10
	bl 0x0200bb98
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	movs	r1, #192
	strb	r3, [r0, #0]
	lsls	r1, r1, #6
	movs	r0, #10
	movs	r2, #20
	bl 0x0200bc50
	movs	r0, #10
	movs	r1, #0
	movs	r2, #40
	bl 0x0200bc50
	mov	r3, r8
	mov	r2, fp
	str	r5, [r7, #104]
	movs	r0, #10
	strh	r5, [r3, #0]
	ldr	r1, [pc, #244]
	strh	r5, [r2, #0]
	ldr	r2, [pc, #236]
	str	r6, [r7, #108]
	bl 0x0200bba0
	movs	r0, #10
	movs	r1, #120
	movs	r2, #215
	bl 0x0200bbc8
	mov	r3, sl
	mov	r2, r9
	movs	r1, #1
	str	r5, [r7, #108]
	movs	r0, #10
	strb	r3, [r2, #0]
	bl 0x0200bbf0
	movs	r0, #16
	bl 0x0200bb68
	movs	r0, #229
	bl 0x0200bd20
	movs	r0, #128
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200bb38
	movs	r0, #4
	bl 0x0200bb68
	movs	r0, #1
	movs	r1, #1
	negs	r1, r1
	ldr	r2, [pc, #160]
	negs	r0, r0
	bl 0x0200bb38
	movs	r0, #40
	bl 0x0200bb68
	movs	r2, #10
	movs	r1, #0
	movs	r0, #10
	bl 0x0200bc50
	movs	r0, #147
	bl 0x0200bd20
	movs	r1, #2
	movs	r0, #10
	bl 0x0200bc10
	movs	r0, #80
	bl 0x0200bb68
	movs	r0, #10
	movs	r1, #3
	bl 0x0200bbf0
	movs	r0, #130
	movs	r2, #168
	lsls	r2, r2, #16
	movs	r3, #0
	lsls	r0, r0, #16
	movs	r1, #0
	bl 0x0200abb0
	movs	r0, #60
	bl 0x0200bb68
	ldr	r2, [pc, #96]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_02001fcc
	adds	r6, r2, #0
.L_02001fba:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200bb68
	cmp	r5, #59
	bhi.n	.L_02001fcc
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02001fba
.L_02001fcc:
	movs	r0, #0
	bl 0x0200bb98
	ldr	r1, [pc, #56]
	adds	r7, r0, #0
	ldr	r0, [pc, #64]
	bl 0x0200bc78
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	ldr	r2, [r7, #16]
	movs	r3, #1
	bl 0x0200bc80
	bl 0x0200bc88
	ldr	r0, [pc, #44]
	bl 0x0200bb58
	bl 0x0200bb78
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0x00009999
	.4byte 0x00013333
	.4byte 0x03001c94
	.4byte 0x0004cccc
	.2byte 0x0905
	.2byte 0x0000
	.global Func_02002020
	.thumb_func
Func_02002020:
	push {lr}
	ldr r3, [pc, #128]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	str r3, [r1, r2]
	ldr r3, [pc, #116]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #116]
	cmp r2, r3
	bne .L_02002020_0
	bl 0x0200a0d0
	b .L_02002020_1
.L_02002020_0:
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_02002020_2
	bl 0x0200a310
	b .L_02002020_1
.L_02002020_2:
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_02002020_3
	bl 0x0200a428
	b .L_02002020_1
.L_02002020_3:
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_02002020_4
	bl 0x0200a490
	b .L_02002020_1
.L_02002020_4:
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_02002020_5
	bl 0x0200a5c0
	b .L_02002020_1
.L_02002020_5:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_02002020_6
	bl 0x0200a6c0
	b .L_02002020_1
.L_02002020_6:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02002020_7
	bl 0x0200a804
	b .L_02002020_1
.L_02002020_7:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02002020_8
	bl 0x0200a934
	b .L_02002020_1
.L_02002020_8:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02002020_1
	bl 0x0200a9dc
.L_02002020_1:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000004d
	.4byte 0x0000004f
	.4byte 0x00000050
	.4byte 0x00000051
	.4byte 0x00000052
	.4byte 0x00000053
	.4byte 0x00000055
	.4byte 0x00000056
	.4byte 0x00000057
	.global Func_020020d0
	.thumb_func
Func_020020d0:
	push {r5, r6, lr}
	ldr r0, [pc, #532]
	sub sp, #8
	bl 0x0200bb50
	cmp r0, #0
	bne .L_020020d0_0
	ldr r3, [pc, #524]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_020020d0_0
	bl 0x0200ad58
.L_020020d0_0:
	bl 0x0200ba44
	ldr r0, [pc, #504]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_020020d0_1
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	b .L_020020d0_2
.L_020020d0_1:
	movs r0, #8
	bl 0x0200bb98
	cmp r0, #0
	beq .L_020020d0_2
	adds r2, r0, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	bl 0x0200bb30
.L_020020d0_2:
	movs r0, #9
	bl 0x0200bb98
	cmp r0, #0
	beq .L_020020d0_3
	adds r2, r0, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	bl 0x0200bb30
.L_020020d0_3:
	ldr r0, [pc, #432]
	bl 0x0200bb50
	adds r5, r0, #0
	cmp r5, #0
	bne .L_020020d0_4
	movs r0, #10
	movs r1, #2
	bl 0x0200bc18
	ldr r0, [pc, #416]
	bl 0x0200bb50
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020020d0_5
	movs r1, #0
	movs r0, #9
	bl 0x0200bbf0
	movs r0, #9
	bl 0x0200bb98
	ldr r3, [pc, #396]
	str r3, [r0, #108]
	movs r0, #9
	bl 0x0200bb98
	adds r0, #85
	strb r5, [r0]
	movs r0, #9
	bl 0x0200bb98
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r0, #12]
	movs r2, #13
	movs r3, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #2
	movs r1, #0
	movs r2, #1
	bl 0x0200bb28
	movs r1, #240
	movs r2, #215
	lsls r2, r2, #16
	lsls r1, r1, #15
	movs r0, #10
	bl 0x0200bbe8
	movs r0, #10
	bl 0x0200bb98
	movs r1, #3
	strh r5, [r0, #6]
	movs r0, #10
	bl 0x0200bbf0
	movs r0, #130
	movs r2, #168
	lsls r0, r0, #16
	lsls r2, r2, #16
	movs r1, #0
	movs r3, #0
	bl 0x0200abb0
	b .L_020020d0_6
.L_020020d0_5:
	ldr r0, [pc, #304]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_020020d0_7
	b .L_020020d0_6
.L_020020d0_7:
	movs r1, #0
	movs r0, #9
	bl 0x0200bbf0
	movs r0, #9
	bl 0x0200bb98
	ldr r3, [pc, #276]
	str r3, [r0, #108]
	movs r0, #9
	bl 0x0200bb98
	adds r0, #85
	strb r6, [r0]
	movs r0, #9
	bl 0x0200bb98
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r0, #12]
	movs r2, #13
	movs r3, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r1, #130
	movs r2, #215
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200bbe8
	movs r1, #128
	ldr r2, [pc, #220]
	movs r0, #10
	lsls r1, r1, #9
	bl 0x0200bc28
	b .L_020020d0_6
.L_020020d0_4:
	ldr r3, [pc, #212]
	movs r0, #10
	movs r1, #0
	movs r2, #0
	ldr r5, [r3]
	bl 0x0200bbe8
	movs r3, #3
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #2
	bl 0x0200bb28
	ldrh r2, [r5, #20]
	ldr r3, [pc, #180]
	ands r3, r2
	movs r0, #8
	strh r3, [r5, #20]
	bl 0x0200b460
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020020d0_8
	movs r0, #8
	movs r1, #5
	bl 0x0200bbf0
	movs r3, #9
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #13
	movs r2, #1
	movs r3, #1
	movs r0, #7
	bl 0x0200bb28
	movs r0, #8
	bl 0x0200bb98
	movs r3, #0
	str r3, [r0, #12]
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
.L_020020d0_8:
	movs r0, #9
	bl 0x0200b460
	ldr r0, [pc, #104]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020020d0_6
	movs r0, #9
	movs r1, #5
	bl 0x0200bbf0
	movs r3, #17
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #1
	movs r2, #3
	movs r3, #1
	movs r0, #29
	bl 0x0200bb28
	movs r0, #9
	bl 0x0200bb98
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r0, #12]
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
.L_020020d0_6:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000109
	.4byte 0x02000240
	.4byte 0x000008fd
	.4byte 0x00000905
	.4byte 0x0200ace1
	.4byte 0x00000904
	.4byte 0x0200bd34
	.4byte 0x03001e70
	.4byte 0x0000fdff
	.4byte 0x00000201
	.global Func_02002310
	.thumb_func
Func_02002310:
	push {lr}
	ldr r0, [pc, #252]
	sub sp, #8
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02002310_0
	ldr r3, [pc, #244]
	ldr r1, [r3]
	ldr r3, [pc, #244]
	ldrh r2, [r1, #20]
	ands r3, r2
	strh r3, [r1, #20]
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	b .L_02002310_1
.L_02002310_0:
	bl 0x0200ba44
	ldr r0, [pc, #224]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_02002310_2
	ldr r3, [pc, #216]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_02002310_2
	bl 0x0200ae1c
	b .L_02002310_1
.L_02002310_2:
	movs r3, #37
	movs r2, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #24
	movs r2, #1
	movs r3, #2
	bl 0x0200bb28
	movs r3, #45
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #44
	movs r1, #23
	movs r2, #1
	movs r3, #2
	bl 0x0200bb28
	ldr r0, [pc, #140]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_02002310_1
	movs r0, #9
	movs r1, #2
	bl 0x0200bc18
	movs r0, #9
	movs r1, #3
	bl 0x0200bbf0
	movs r0, #238
	movs r2, #209
	movs r3, #128
	lsls r0, r0, #16
	lsls r2, r2, #17
	lsls r3, r3, #8
	movs r1, #0
	bl 0x0200abb0
.L_02002310_1:
	ldr r0, [pc, #116]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02002310_3
	movs r3, #24
	movs r2, #80
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #1
	movs r2, #24
	movs r3, #11
	bl 0x0200bb08
	b .L_02002310_4
.L_02002310_3:
	movs r3, #24
	movs r2, #80
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #1
	movs r2, #24
	movs r3, #11
	bl 0x0200bb08
.L_02002310_4:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x000008fe
	.4byte 0x03001e70
	.4byte 0x0000fdff
	.4byte 0x00000109
	.4byte 0x02000240
	.4byte 0x00000323
	.global Func_02002428
	.thumb_func
Func_02002428:
	push {lr}
	ldr r0, [pc, #80]
	sub sp, #8
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02002428_0
	ldr r3, [pc, #72]
	ldr r1, [r3]
	ldr r3, [pc, #72]
	ldrh r2, [r1, #20]
	ands r3, r2
	strh r3, [r1, #20]
	b .L_02002428_1
.L_02002428_0:
	movs r3, #53
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #52
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
.L_02002428_1:
	ldr r3, [pc, #44]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #6
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_02002428_2
	ldr r0, [pc, #28]
	bl 0x0200bb60
.L_02002428_2:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000008fe
	.4byte 0x03001e70
	.4byte 0x0000fdff
	.4byte 0x02000240
	.4byte 0x0000012f
	.global Func_02002490
	.thumb_func
Func_02002490:
	push {lr}
	ldr r0, [pc, #276]
	sub sp, #8
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02002490_0
	ldr r3, [pc, #268]
	ldr r1, [r3]
	ldr r3, [pc, #268]
	ldrh r2, [r1, #20]
	ands r3, r2
	strh r3, [r1, #20]
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	b 0x0200a506
.L_02002490_0:
	ldr r0, [pc, #252]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_02002490_1
	ldr r3, [pc, #244]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_02002490_1
	bl 0x0200ae84
.L_02002490_1:
	bl 0x0200ba44
	ldr r0, [pc, #204]
	bl 0x0200bb50
	cmp r0, #0
	bne 0x0200a506
	movs r0, #10
	movs r1, #2
	bl 0x0200bc18
	movs r0, #10
	movs r1, #3
	bl 0x0200bbf0
	movs r0, #187
	movs r1, #128
	movs r2, #140
	movs r3, #128
	lsls r0, r0, #18
	lsls r1, r1, #12
.L_020024fe:
	lsls r2, r2, #17
	lsls r3, r3, #8
	bl 0x0200abb0
	movs r0, #9
	bl 0x0200b460
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020024fe_0
	movs r0, #9
	movs r1, #5
	bl 0x0200bbf0
	movs r3, #25
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #23
	movs r1, #13
	bl 0x0200bb28
	movs r0, #9
	bl 0x0200bb98
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
.L_020024fe_0:
	ldr r0, [pc, #116]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020024fe_1
	movs r3, #11
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #32
	movs r2, #11
	movs r3, #4
	bl 0x0200bb08
	b .L_020024fe_2
.L_020024fe_1:
	movs r3, #11
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #32
	movs r2, #11
	movs r3, #4
	bl 0x0200bb08
.L_020024fe_2:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0907
	.2byte 0x0000
	.2byte 0x1e70
	.2byte 0x0300
	.2byte 0xfdff
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.4byte 0x00000325
	.global Func_020025c0
	.thumb_func
Func_020025c0:
	push {r5, lr}
	ldr r3, [pc, #228]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #2
	bne .L_020025c0_0
	ldr r0, [pc, #212]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_020025c0_0
	movs r1, #179
	movs r2, #208
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200bbe8
.L_020025c0_0:
	movs r0, #9
	bl 0x0200b460
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020025c0_1
	movs r0, #9
	bl 0x0200bb98
	movs r1, #5
	adds r5, r0, #0
	movs r0, #9
	bl 0x0200bbf0
	movs r3, #43
	movs r2, #41
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	adds r5, #35
	movs r0, #45
	movs r1, #41
	bl 0x0200bb28
	ldrb r2, [r5]
	movs r3, #2
	orrs r3, r2
	strb r3, [r5]
.L_020025c0_1:
	ldr r0, [pc, #128]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020025c0_2
	ldr r3, [pc, #124]
	ldr r1, [r3]
	ldr r3, [pc, #124]
	ldrh r2, [r1, #20]
	ands r3, r2
	strh r3, [r1, #20]
.L_020025c0_2:
	ldr r0, [pc, #120]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020025c0_3
	movs r3, #16
	movs r2, #92
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #29
	movs r2, #16
	movs r3, #28
	bl 0x0200bb08
	b .L_020025c0_4
.L_020025c0_3:
	movs r3, #16
	movs r2, #92
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #29
	movs r2, #16
	movs r3, #28
	bl 0x0200bb08
.L_020025c0_4:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000109
	.4byte 0x00000907
	.4byte 0x03001e70
	.4byte 0x0000fdff
	.4byte 0x00000326
	.global Func_020026c0
	.thumb_func
Func_020026c0:
	push {lr}
	movs r0, #9
	sub sp, #8
	bl 0x0200bb98
	movs r1, #0
	bl 0x0200bb30
	bl 0x0200840c
	movs r0, #9
	bl 0x0200b460
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020026c0_0
	movs r0, #9
	movs r1, #5
	bl 0x0200bbf0
	movs r3, #26
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	movs r0, #0
	movs r1, #0
	bl 0x0200bb28
	movs r0, #9
	bl 0x0200bb98
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
.L_020026c0_0:
	movs r0, #11
	bl 0x0200b460
	ldr r0, [pc, #224]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020026c0_1
	movs r0, #11
	movs r1, #5
	bl 0x0200bbf0
	movs r3, #17
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #1
	movs r1, #0
	bl 0x0200bb28
	movs r0, #11
	bl 0x0200bb98
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
.L_020026c0_1:
	movs r0, #12
	bl 0x0200b460
	movs r0, #129
	lsls r0, r0, #2
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020026c0_2
	movs r0, #12
	movs r1, #5
	bl 0x0200bbf0
	movs r3, #26
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r0, #1
	movs r1, #0
	bl 0x0200bb28
	movs r0, #12
	bl 0x0200bb98
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
.L_020026c0_2:
	movs r1, #200
	ldr r0, [pc, #108]
	lsls r1, r1, #4
	bl 0x0200ba80
	ldr r0, [pc, #104]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_020026c0_3
	movs r3, #29
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #28
	movs r2, #29
	movs r3, #17
	bl 0x0200bb08
	b .L_020026c0_4
.L_020026c0_3:
	movs r3, #29
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #28
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl 0x0200bb28
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #28
	movs r2, #29
	movs r3, #17
	bl 0x0200bb08
.L_020026c0_4:
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000201
	.4byte 0x0200b429
	.4byte 0x00000327
	.global Func_02002804
	.thumb_func
Func_02002804:
	push {r5, r6, lr}
	movs r0, #10
	sub sp, #8
	bl 0x0200bb98
	movs r1, #0
	adds r6, r0, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200bbe8
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl 0x0200bbe8
	movs r0, #9
	bl 0x0200bb98
	movs r1, #0
	bl 0x0200bb30
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #80]
	str r3, [r6, #24]
	ldr r3, [pc, #80]
	ldr r2, [r6, #80]
	str r3, [r6, #28]
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #12
	bl 0x0200bb98
	ldr r5, [pc, #56]
	adds r0, #85
	strb r5, [r0]
	movs r0, #12
	bl 0x0200bb98
	ldr r3, [pc, #56]
	str r3, [r0, #12]
	ldr r0, [pc, #56]
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02002804_0
	ldr r3, [r6, #8]
	movs r2, #224
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r2, [pc, #40]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, [r6, #80]
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r2, #30]
.L_02002804_0:
	ldr r0, [pc, #20]
	b .L_02002804_1
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000e666
	.4byte 0x00009999
	.4byte 0xffe40000
	.4byte 0x00000908
	.4byte 0xfff80000
.L_02002804_1:
	bl 0x0200bb50
	cmp r0, #0
	beq .L_02002804_2
	movs r3, #11
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #36
	movs r2, #43
	movs r3, #36
	bl 0x0200bb08
	movs r3, #43
	movs r2, #35
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #35
	movs r2, #10
	movs r3, #5
	bl 0x0200bb28
	bl 0x0200baf0
	movs r0, #1
	bl 0x0200ba78
.L_02002804_2:
	ldr r3, [pc, #72]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	bne .L_02002804_3
	ldr r0, [pc, #60]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_02002804_3
	bl 0x0200bb70
	movs r0, #0
	bl 0x0200bb98
	ldr r1, [pc, #44]
	str r1, [r0, #12]
	movs r0, #198
	lsls r0, r0, #18
	ldr r2, [pc, #40]
	movs r3, #0
	bl 0x0200bc80
	bl 0x0200baf0
	movs r0, #1
	bl 0x0200ba78
	bl 0x0200bb78
.L_02002804_3:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00000109
	.4byte 0xffa80000
	.4byte 0x02410000
	.global Func_02002934
	.thumb_func
Func_02002934:
	push {r5, lr}
	ldr r0, [pc, #152]
	bl 0x0200bb50
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002934_0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	b .L_02002934_1
.L_02002934_0:
	movs r0, #8
	bl 0x0200bb98
	movs r1, #0
	bl 0x0200bb30
	movs r1, #3
	movs r0, #9
	bl 0x0200bc58
	movs r0, #9
	bl 0x0200bb98
	movs r1, #0
	bl 0x0200bb30
	movs r0, #9
	bl 0x0200bb98
	adds r0, #89
	strb r5, [r0]
.L_02002934_1:
	ldr r3, [pc, #80]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_02002934_2
	cmp r3, #98
	bne .L_02002934_3
.L_02002934_2:
	ldr r0, [pc, #64]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_02002934_4
	movs r0, #0
	bl 0x0200bb98
	adds r5, r0, #0
	bl 0x0200bb70
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r5, #12]
	bl 0x0200bb78
	b .L_02002934_4
.L_02002934_3:
	cmp r3, #99
	bne .L_02002934_4
	ldr r0, [pc, #24]
	bl 0x0200bb50
	cmp r0, #0
	bne .L_02002934_4
	bl 0x0200b028
.L_02002934_4:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000909
	.4byte 0x02000240
	.4byte 0x00000109
	.global Func_020029dc
	.thumb_func
Func_020029dc:
	push {lr}
	movs r0, #9
	bl 0x0200bb98
	movs r1, #0
	bl 0x0200bb30
	ldr r3, [pc, #32]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_020029dc_0
	movs r1, #184
	movs r2, #164
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200bbe8
.L_020029dc_0:
	pop {r0}
	bx r0
	.4byte 0x02000240
	.global Func_02002a10
	.thumb_func
Func_02002a10:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r1, [r5, #80]
	ldrb r2, [r1, #9]
	subs r3, #13
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #3
	bl 0x0200bc20
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bb30
	ldr r3, [pc, #8]
	str r3, [r5, #24]
	str r3, [r5, #28]
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00004ccc
	.global Func_02002a48
	.thumb_func
Func_02002a48:
	push {r5, r6, lr}
	ldr r3, [pc, #244]
	ldr r3, [r3]
	adds r5, r0, #0
	cmp r3, #0
	beq .L_02002a48_0
	movs r1, #128
	lsls r1, r1, #8
	cmp r3, r1
	beq .L_02002a48_1
	adds r6, r5, #0
	adds r6, #100
	b .L_02002a48_2
.L_02002a48_0:
	bl 0x0200ba90
	adds r6, r5, #0
	lsls r0, r0, #1
	adds r6, #100
	lsrs r0, r0, #16
	movs r3, #0
	ldrsh r2, [r6, r3]
	subs r0, #1
	lsls r0, r0, #16
	ldr r3, [r5, #8]
	lsls r2, r2, #12
	asrs r0, r0, #1
	adds r2, r2, r0
	adds r3, r3, r2
	b .L_02002a48_3
.L_02002a48_1:
	bl 0x0200ba90
	adds r6, r5, #0
	lsls r0, r0, #1
	adds r6, #100
	lsrs r0, r0, #16
	movs r1, #0
	ldrsh r2, [r6, r1]
	subs r0, #1
	lsls r0, r0, #16
	ldr r3, [r5, #8]
	lsls r2, r2, #12
	asrs r0, r0, #1
	adds r2, r2, r0
	subs r3, r3, r2
.L_02002a48_3:
	str r3, [r5, #8]
.L_02002a48_2:
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #3
	bgt .L_02002a48_4
	ldr r3, [pc, #148]
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02002a48_5
	movs r1, #128
	lsls r1, r1, #8
	cmp r3, r1
	beq .L_02002a48_6
	b .L_02002a48_7
.L_02002a48_5:
	ldr r3, [r5, #8]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	b .L_02002a48_8
.L_02002a48_6:
	ldr r3, [r5, #8]
	ldr r1, [pc, #120]
	adds r3, r3, r1
.L_02002a48_8:
	str r3, [r5, #8]
.L_02002a48_7:
	ldr r3, [r5, #24]
	ldr r2, [pc, #116]
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r1, [pc, #116]
	ldr r3, [r5, #28]
	adds r3, r3, r1
	b .L_02002a48_9
.L_02002a48_4:
	ldr r3, [r5, #16]
	ldr r2, [pc, #108]
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r2, [pc, #108]
	ldr r3, [r5, #24]
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
.L_02002a48_9:
	str r3, [r5, #28]
	bl 0x0200ba90
	movs r1, #0
	ldrsh r3, [r6, r1]
	muls r3, r0
	lsrs r3, r3, #16
	ldrh r2, [r6]
	cmp r3, #0
	bne .L_02002a48_10
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200bc20
	ldrh r2, [r6]
.L_02002a48_10:
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_02002a48_11
	subs r3, r2, #2
	b .L_02002a48_12
.L_02002a48_11:
	bl 0x0200ba90
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #1
	adds r3, #2
.L_02002a48_12:
	strh r3, [r6]
	ldr r3, [r5, #104]
	subs r3, #1
	str r3, [r5, #104]
	cmp r3, #0
	bne .L_02002a48_13
	adds r0, r5, #0
	bl 0x0200bae8
.L_02002a48_13:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200d23c
	.4byte 0xffff8000
	.4byte 0x00001999
	.4byte 0xfffff334
	.4byte 0x00013333
	.4byte 0x000007ae
	.global Func_02002b58
	.thumb_func
Func_02002b58:
	push {r5, lr}
	ldr r3, [pc, #72]
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_02002b58_0
	ldr r3, [pc, #64]
	movs r0, #222
	ldr r1, [r3]
	ldr r2, [r3, #4]
	ldr r3, [r3, #8]
	bl 0x0200bae0
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002b58_0
	adds r2, r5, #0
	adds r2, #100
	movs r3, #30
	strh r3, [r2]
	adds r2, #2
	movs r3, #1
	strh r3, [r2]
	movs r3, #20
	str r3, [r5, #104]
	bl 0x0200aa10
	ldr r3, [pc, #24]
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl 0x0200bad0
.L_02002b58_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x0200d240
	.4byte 0x0200aa49
	.global Func_02002bb0
	.thumb_func
Func_02002bb0:
	push {lr}
	ldr r4, [pc, #32]
	str r2, [r4, #8]
	ldr r2, [pc, #32]
	str r0, [r4]
	str r1, [r4, #4]
	movs r0, #170
	str r3, [r2]
	bl 0x0200bd10
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #16]
	bl 0x0200ba80
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d240
	.4byte 0x0200d23c
	.4byte 0x0200ab59
	.global Func_02002be0
	.thumb_func
Func_02002be0:
	push {r5, r6, lr}
	ldr r6, [pc, #224]
	movs r1, #3
	ldr r0, [r6]
	bl 0x0200ba70
	cmp r0, #0
	bne .L_02002be0_0
	bl 0x0200ba90
	lsls r1, r0, #1
	adds r1, r1, r0
	lsls r1, r1, #4
	ldr r2, [pc, #204]
	lsrs r1, r1, #16
	lsls r1, r1, #16
	movs r3, #152
	adds r1, r1, r2
	movs r0, #200
	ldr r2, [pc, #196]
	lsls r3, r3, #18
	bl 0x0200bae0
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002be0_0
	ldr r0, [r6]
	movs r1, #9
	bl 0x0200ba70
	cmp r0, #0
	bne .L_02002be0_1
	bl 0x0200ba90
	lsls r0, r0, #1
	lsrs r0, r0, #16
	cmp r0, #0
	beq .L_02002be0_2
	movs r0, #145
	bl 0x0200bd20
	b .L_02002be0_1
.L_02002be0_2:
	movs r0, #144
	bl 0x0200bd20
.L_02002be0_1:
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	bl 0x0200ba90
	ldr r3, [pc, #136]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, [pc, #132]
	adds r2, r5, #0
	str r3, [r5, #72]
	adds r2, #97
	movs r3, #1
	str r0, [r5, #28]
	str r0, [r5, #24]
	movs r1, #0
	strb r3, [r2]
	adds r0, r5, #0
	bl 0x0200bb30
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200bad0
	ldr r1, [pc, #76]
	adds r0, r5, #0
	bl 0x0200bad8
	bl 0x0200ba90
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	lsrs r3, r3, #16
	subs r3, #3
	lsls r3, r3, #16
	str r3, [r5, #36]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #40]
	bl 0x0200ba90
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #9
	ldr r2, [pc, #36]
	lsrs r3, r3, #16
	adds r3, r3, r2
	str r3, [r5, #44]
.L_02002be0_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x02fd0000
	.4byte 0xffc00000
	.4byte 0x00004ccc
	.4byte 0x00006666
	.4byte 0x0200c01c
	.4byte 0xfffffd00
	.global Func_02002ce0
	.thumb_func
Func_02002ce0:
	push {lr}
	ldr r3, [pc, #32]
	ldr r2, [r3]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	bne .L_02002ce0_0
	movs r1, #2
	bl 0x0200bc20
	b .L_02002ce0_1
.L_02002ce0_0:
	cmp r2, #2
	bne .L_02002ce0_1
	movs r1, #0
	bl 0x0200bc20
.L_02002ce0_1:
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02002d08
	.thumb_func
Func_02002d08:
	push {lr}
	movs r0, #24
	movs r1, #1
	bl 0x0200bcd8
	movs r0, #10
	movs r1, #9
	bl 0x0200bce0
	bl 0x0200bcf8
	movs r1, #2
	movs r0, #10
	bl 0x0200bc18
	movs r0, #1
	bl 0x0200bcd0
	movs r0, #10
	movs r1, #2
	bl 0x0200bc18
	bl 0x0200bce8
	movs r1, #2
	movs r0, #10
	bl 0x0200bc18
	bl 0x0200bcf0
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200bd20
	movs r0, #10
	movs r1, #2
	bl 0x0200bc18
	pop {r0}
	bx r0
	.global Func_02002d58
	.thumb_func
Func_02002d58:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #0
	bl 0x0200bb98
	adds r5, r0, #0
	bl 0x0200bb70
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	ldr r6, [pc, #136]
	ldr r3, [pc, #136]
	ldr r2, [r6]
	movs r1, #224
	lsls r1, r1, #1
	str r3, [r2, r1]
	mov r8, r1
	bl 0x0200bcb0
	bl 0x0200bcb8
	movs r0, #20
	bl 0x0200bb68
	movs r0, #202
	movs r1, #3
	bl 0x0200bcc0
	movs r1, #0
	movs r0, #202
	bl 0x0200bb88
	bl 0x0200bc90
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r1, [pc, #88]
	ldr r0, [pc, #92]
	bl 0x0200bc78
	movs r0, #200
	movs r2, #249
	movs r1, #0
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #15
	bl 0x0200bc80
	bl 0x0200bc88
	movs r0, #20
	bl 0x0200bb68
	bl 0x020086dc
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	ldr r0, [r5, #8]
	movs r3, #1
	bl 0x0200bc80
	bl 0x0200bc88
	ldr r0, [pc, #44]
	bl 0x0200bb80
	ldr r2, [r6]
	movs r3, #129
	lsls r3, r3, #2
	mov r1, r8
	str r3, [r2, r1]
	bl 0x0200bb78
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00000201
	.4byte 0x00003333
	.4byte 0x00019999
	.4byte 0x0200cd6c
	.global Func_02002e1c
	.thumb_func
Func_02002e1c:
	push {r5, r6, lr}
	bl 0x0200bb70
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	movs r1, #164
	movs r2, #212
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #8
	bl 0x0200bbe8
	ldr r0, [pc, #60]
	bl 0x0200bb58
	bl 0x020089ec
	bl 0x0200baf0
	movs r0, #1
	bl 0x0200ba78
	ldr r5, [pc, #44]
	ldr r3, [pc, #44]
	ldr r2, [r5]
	movs r6, #224
	lsls r6, r6, #1
	str r3, [r2, r6]
	bl 0x0200bcb0
	bl 0x0200bcb8
	bl 0x020087e8
	ldr r2, [r5]
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r2, r6]
	bl 0x0200bb78
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000323
	.4byte 0x03001ebc
	.4byte 0x00000201
	.global Func_02002e84
	.thumb_func
Func_02002e84:
	push {r5, r6, lr}
	bl 0x0200bb70
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200bbe8
	movs r1, #244
	movs r2, #138
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl 0x0200bbe8
	ldr r0, [pc, #60]
	bl 0x0200bb58
	bl 0x02008a94
	bl 0x0200baf0
	movs r0, #1
	bl 0x0200ba78
	ldr r5, [pc, #44]
	ldr r3, [pc, #44]
	ldr r2, [r5]
	movs r6, #224
	lsls r6, r6, #1
	str r3, [r2, r6]
	bl 0x0200bcb0
	bl 0x0200bcb8
	bl 0x020088d4
	ldr r2, [r5]
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r2, r6]
	bl 0x0200bb78
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000325
	.4byte 0x03001ebc
	.4byte 0x00000201
	.global Func_02002eec
	.thumb_func
Func_02002eec:
	push {r5, lr}
	ldr r5, [pc, #28]
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, #60
	bne .L_02002eec_0
	movs r0, #183
	bl 0x0200bd20
	movs r3, #0
	str r3, [r5]
.L_02002eec_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d1b0
	.global Func_02002f10
	.thumb_func
Func_02002f10:
	push {lr}
	bl 0x0200bb70
	ldr r0, [pc, #192]
	ldr r1, [pc, #192]
	bl 0x0200bc78
	movs r0, #164
	movs r1, #1
	movs r2, #174
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200bc80
	movs r0, #0
	ldr r1, [pc, #164]
	ldr r2, [pc, #168]
	bl 0x0200bba0
	movs r1, #164
	movs r2, #116
	lsls r1, r1, #1
	movs r0, #0
	bl 0x0200bbd8
	movs r0, #148
	bl 0x0200bd20
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #144]
	bl 0x0200ba80
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200bb38
	movs r0, #8
	ldr r1, [pc, #124]
	ldr r2, [pc, #128]
	bl 0x0200bba0
	ldr r2, [pc, #120]
	movs r0, #9
	ldr r1, [pc, #112]
	bl 0x0200bba0
	movs r0, #8
	movs r1, #2
	bl 0x0200bbf0
	movs r1, #164
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #104
	bl 0x0200bbc0
	movs r1, #164
	lsls r1, r1, #1
	movs r2, #108
	movs r0, #9
	bl 0x0200bbc0
	movs r0, #60
	bl 0x0200bb68
	movs r1, #128
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200bc60
	movs r1, #2
	movs r0, #0
	bl 0x0200bc08
	movs r0, #8
	bl 0x0200bbe0
	ldr r3, [pc, #52]
	ldr r2, [pc, #52]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r0, [pc, #48]
	movs r1, #99
	bl 0x0200bca8
	movs r0, #53
	movs r1, #3
	bl 0x0200bc98
	pop {r0}
	bx r0
	.4byte 0x00009999
	.4byte 0x00001333
	.4byte 0x00004ccc
	.4byte 0x0200aeed
	.4byte 0x00001999
	.4byte 0x00000ccc
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x00000056
	.global Func_02002ffc
	.thumb_func
Func_02002ffc:
	push {lr}
	ldr r3, [pc, #36]
	ldr r3, [r3]
	movs r2, #1
	lsrs r3, r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02002ffc_0
	movs r0, #8
	movs r1, #7
	bl 0x0200bc18
	b .L_02002ffc_1
.L_02002ffc_0:
	movs r0, #8
	movs r1, #6
	bl 0x0200bc18
.L_02002ffc_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.global Func_02003028
	.thumb_func
Func_02003028:
	push {r5, r6, lr}
	bl 0x0200bb70
	movs r1, #164
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl 0x0200bbe8
	movs r1, #164
	movs r2, #176
	lsls r2, r2, #15
	movs r0, #9
	lsls r1, r1, #17
	bl 0x0200bbe8
	movs r0, #8
	movs r1, #0
	bl 0x0200bbf0
	ldr r5, [pc, #896]
	movs r3, #224
	ldr r1, [r5]
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #40
	str r3, [r2]
	bl 0x0200bcb0
	bl 0x0200bcb8
	movs r0, #20
	bl 0x0200bb68
	movs r0, #0
	bl 0x0200bb98
	cmp r0, #0
	beq .L_02003028_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200bbe8
.L_02003028_0:
	movs r0, #0
	bl 0x0200bb98
	cmp r0, #0
	beq .L_02003028_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200bbe8
.L_02003028_1:
	movs r0, #0
	bl 0x0200bb98
	cmp r0, #0
	beq .L_02003028_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200bbe8
.L_02003028_2:
	movs r0, #1
	ldr r1, [pc, #800]
	ldr r2, [pc, #804]
	bl 0x0200bba0
	movs r0, #2
	ldr r1, [pc, #792]
	ldr r2, [pc, #792]
	bl 0x0200bba0
	ldr r2, [pc, #788]
	movs r0, #3
	ldr r1, [pc, #780]
	bl 0x0200bba0
	ldr r1, [pc, #780]
	movs r0, #1
	bl 0x0200bba8
	ldr r1, [pc, #776]
	movs r0, #2
	bl 0x0200bba8
	ldr r1, [pc, #772]
	movs r0, #3
	bl 0x0200bbb8
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #192
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200bc50
	movs r0, #1
	movs r1, #1
	bl 0x0200bc10
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #10
	movs r0, #1
	bl 0x0200bc50
	ldr r0, [pc, #712]
	bl 0x0200bc30
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x0200bc48
	movs r0, #2
	movs r1, #1
	bl 0x0200bc10
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bc50
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200bc50
	movs r1, #0
	movs r0, #2
	bl 0x0200bc38
	movs r0, #0
	movs r1, #0
	bl 0x0200bb90
	cmp r0, #0
	bne .L_02003028_3
	movs r0, #2
	movs r1, #3
	bl 0x0200bbf8
	b .L_02003028_4
.L_02003028_3:
	movs r0, #2
	movs r1, #4
	bl 0x0200bbf8
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02003028_4:
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x0200bc48
	movs r0, #0
	movs r1, #3
	bl 0x0200bbf0
	movs r0, #1
	movs r1, #3
	bl 0x0200bbf0
	movs r0, #0
	movs r1, #3
	bl 0x0200bbf0
	movs r1, #3
	movs r0, #0
	bl 0x0200bbf8
	movs r0, #20
	bl 0x0200bb68
	movs r1, #2
	movs r0, #3
	bl 0x0200bc10
	ldr r0, [pc, #560]
	bl 0x0200bc30
	movs r0, #3
	movs r1, #0
	bl 0x0200bc40
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200bc50
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bc60
	movs r1, #128
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #6
	bl 0x0200bc50
	movs r0, #1
	movs r1, #0
	bl 0x0200bc40
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200bc50
	movs r0, #0
	ldr r1, [pc, #492]
	movs r2, #0
	bl 0x0200bc60
	movs r0, #1
	ldr r1, [pc, #484]
	movs r2, #0
	bl 0x0200bc60
	movs r0, #2
	ldr r1, [pc, #472]
	movs r2, #0
	bl 0x0200bc60
	movs r2, #40
	ldr r1, [pc, #464]
	movs r0, #3
	bl 0x0200bc60
	movs r0, #190
	bl 0x0200bd20
	movs r1, #7
	movs r0, #8
	bl 0x0200bc18
	movs r0, #10
	bl 0x0200bb68
	ldr r0, [pc, #440]
	bl 0x0200bd20
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bc60
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bc60
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bc60
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #3
	bl 0x0200bc60
	movs r0, #103
	bl 0x0200bd20
	ldr r6, [pc, #336]
	movs r1, #200
	adds r0, r6, #0
	lsls r1, r1, #4
	bl 0x0200ba80
	ldr r5, [pc, #328]
	movs r0, #9
	adds r1, r5, #0
	bl 0x0200bba8
	adds r1, r5, #0
	movs r0, #8
	bl 0x0200bbb8
	adds r0, r6, #0
	bl 0x0200ba88
	movs r0, #60
	bl 0x0200bb68
	movs r1, #2
	movs r0, #2
	bl 0x0200bc10
	movs r0, #20
	bl 0x0200bb68
	movs r0, #2
	movs r1, #0
	bl 0x0200bc40
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #160
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200bc50
	movs r0, #1
	movs r1, #3
	bl 0x0200bbf0
	movs r0, #2
	movs r1, #3
	bl 0x0200bbf0
	movs r1, #3
	movs r0, #3
	bl 0x0200bbf8
	movs r0, #20
	bl 0x0200bb68
	movs r0, #3
	movs r1, #1
	bl 0x0200bc10
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x0200bc48
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bc50
	movs r1, #192
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #7
	bl 0x0200bc50
	movs r0, #1
	movs r1, #3
	bl 0x0200bbf8
	movs r2, #10
	movs r0, #1
	movs r1, #0
	bl 0x0200bc48
	movs r0, #0
	movs r1, #3
	bl 0x0200bbf8
	movs r0, #1
	movs r1, #3
	bl 0x0200bbf0
	movs r0, #2
	movs r1, #3
	bl 0x0200bbf0
	movs r1, #3
	movs r0, #3
	bl 0x0200bbf8
	movs r0, #20
	bl 0x0200bb68
	ldr r5, [pc, #128]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200bba8
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200bba8
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200bbb8
	movs r0, #20
	bl 0x0200bb68
	ldr r5, [pc, #48]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	ldr r0, [pc, #84]
	bl 0x0200bb60
	ldr r3, [r5]
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	ldr r0, [pc, #72]
	bl 0x0200bb58
	bl 0x0200bb78
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x0200c054
	.4byte 0x0200c084
	.4byte 0x0200c0b4
	.4byte 0x0000190c
	.4byte 0x00001910
	.4byte 0x00000101
	.4byte 0x00000121
	.4byte 0x0200affd
	.4byte 0x0200c0e4
	.4byte 0x0200c12c
	.4byte 0x0000012f
	.4byte 0x00000909
	.global Func_02003410
	.thumb_func
Func_02003410:
	ldr r3, [pc, #12]
	movs r1, #191
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	ldr r3, [pc, #8]
	strh r3, [r2]
	bx lr
	.4byte 0x03001ebc
	.4byte 0x00001018
	.global Func_02003428
	.thumb_func
Func_02003428:
	push {lr}
	movs r0, #0
	bl 0x0200bb98
	movs r2, #192
	ldr r3, [r0, #12]
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_02003428_0
	movs r0, #11
	bl 0x0200bb98
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #12
	bl 0x0200bc58
	b .L_02003428_1
.L_02003428_0:
	movs r0, #12
	movs r1, #2
	bl 0x0200bc58
.L_02003428_1:
	pop {r0}
	bx r0
	.global Func_02003460
	.thumb_func
Func_02003460:
	push {lr}
	bl 0x0200bb98
	adds r1, r0, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #255
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #0
	bl 0x0200bb40
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003484
	.thumb_func
Func_02003484:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200bd20
	movs r0, #232
	bl 0x0200bd20
	ldr r2, [r5, #8]
	ldr r3, [pc, #248]
	ands r2, r3
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	mov r9, r2
	ldr r2, [r5, #16]
	ands r2, r3
	mov r11, r2
	movs r3, #128
	add r10, r9
	lsls r3, r3, #10
	add r9, r11
	ldr r2, [r5, #12]
	mov r1, r10
	str r3, [r5, #52]
	adds r0, r5, #0
	mov r3, r9
	bl 0x0200baf8
	adds r0, r5, #0
	bl 0x0200bb00
	adds r3, r5, #0
	movs r6, #0
	adds r3, #34
	strb r6, [r3]
	mov r2, r9
	mov r3, r10
	str r3, [r5, #8]
	str r2, [r5, #16]
	str r6, [r5, #36]
	str r6, [r5, #44]
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200bad0
	movs r0, #15
	bl 0x0200ba78
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200bad0
	movs r0, #30
	bl 0x0200ba78
	ldr r5, [r5, #80]
	movs r2, #1
	mov r8, r2
	adds r3, r5, #0
	mov r2, r8
	adds r3, #39
	strb r2, [r3]
	ldr r0, [r5, #44]
	bl 0x0200bac8
	str r6, [r5, #44]
	mov r3, r8
	adds r5, #37
	strb r3, [r5]
	movs r2, #250
	ldr r3, [pc, #128]
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl 0x0200bcc8
	adds r5, r0, #0
	movs r0, #152
	bl 0x0200bd20
	mov r3, r10
	str r3, [r5, #8]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #128
	mov r2, r9
	lsls r3, r3, #9
	str r3, [r5, #72]
	ldr r1, [r5, #80]
	str r2, [r5, #16]
	subs r6, #13
	ldrb r2, [r1, #9]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r1, #9]
	adds r0, r5, #0
	movs r1, #7
	bl 0x0200bad0
	movs r3, #192
	lsls r3, r3, #13
	add r11, r3
	ldr r2, [r5, #12]
	adds r0, r5, #0
	mov r1, r10
	mov r3, r11
	bl 0x0200baf8
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r0, #20
	bl 0x0200ba78
	ldr r2, [r5, #80]
	ldrb r3, [r2, #9]
	ands r6, r3
	movs r3, #8
	orrs r6, r3
	strb r6, [r2, #9]
	movs r0, #159
	bl 0x0200bd20
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xfff00000
	.4byte 0x02000240
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r6, r3, r2
	movs	r3, #192
	lsls	r3, r3, #8
	sub	sp, #12
	ands	r6, r3
	ldr	r3, [r7, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #192
	adds	r1, r6, #0
	lsls	r0, r0, #13
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl 0x0200bab0
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #12
	ldr	r2, [pc, #120]
	adds	r3, r3, r1
	ands	r3, r2
	mov	r9, r3
	ldr	r3, [r5, #8]
	adds	r3, r3, r1
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r7, #0
	movs	r1, #5
	mov	sl, r3
	adds	r6, r6, r2
	bl 0x0200bad0
	movs	r0, #184
	bl 0x0200bd20
	movs	r3, #15
	mov	r8, r3
.L_02003610:
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r6, r6, r2
	movs	r0, #192
	mov	r2, sl
	mov	r3, r9
	str	r2, [r5, #8]
	lsls	r0, r0, #13
	adds	r1, r6, #0
	adds	r2, r5, #0
	str	r3, [r5, #0]
	bl 0x0200bab0
	ldr	r3, [r5, #0]
	str	r3, [r7, #8]
	movs	r2, #128
	ldr	r3, [r5, #8]
	lsls	r2, r2, #7
	str	r3, [r7, #16]
	adds	r3, r6, r2
	strh	r3, [r7, #6]
	movs	r0, #1
	bl 0x0200ba78
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_02003610
.L_0200364c:
	movs	r0, #233
	bl 0x0200bd20
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r0, #0
	ldrh	r3, [r7, #6]
	ldr	r2, [pc, #160]
	adds	r6, r3, r2
	movs	r3, #192
	lsls	r3, r3, #8
	sub	sp, #12
	ands	r6, r3
	ldr	r3, [r7, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	movs	r0, #192
	adds	r1, r6, #0
	lsls	r0, r0, #13
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl 0x0200bab0
	ldr	r3, [r5, #0]
	movs	r1, #128
	lsls	r1, r1, #12
	ldr	r2, [pc, #120]
	adds	r3, r3, r1
	ands	r3, r2
	mov	r9, r3
	ldr	r3, [r5, #8]
	adds	r3, r3, r1
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r7, #0
	movs	r1, #6
	mov	sl, r3
	adds	r6, r6, r2
	bl 0x0200bad0
	movs	r0, #184
	bl 0x0200bd20
	movs	r3, #15
	mov	r8, r3
.L_020036ca:
	ldr	r2, [pc, #84]
	movs	r0, #192
	adds	r6, r6, r2
	mov	r2, sl
	mov	r3, r9
	str	r2, [r5, #8]
	lsls	r0, r0, #13
	adds	r1, r6, #0
	adds	r2, r5, #0
	str	r3, [r5, #0]
	bl 0x0200bab0
	ldr	r3, [r5, #0]
	str	r3, [r7, #8]
	ldr	r2, [pc, #48]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	adds	r3, r6, r2
	strh	r3, [r7, #6]
	movs	r0, #1
	bl 0x0200ba78
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_020036ca
	movs	r0, #233
	bl 0x0200bd20
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xffffc000
	.4byte 0xfff00000
	.2byte 0xfc00
	.2byte 0xffff
	.global Func_02003724
	.thumb_func
Func_02003724:
	push {lr}
	adds r0, #102
	movs r3, #33
	strh r3, [r0]
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200bd20
	pop {r0}
	bx r0
	.global Func_02003738
	.thumb_func
Func_02003738:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	movs r0, #192
	lsls r0, r0, #8
	mov r9, r0
	ldrh r3, [r7, #6]
	mov r1, r9
	ldr r0, [r7, #12]
	ands r1, r3
	sub sp, #12
	mov r9, r1
	cmp r0, #0
	bge .L_02003738_0
	ldr r2, [pc, #232]
	adds r0, r0, r2
.L_02003738_0:
	adds r3, r7, #0
	asrs r0, r0, #16
	adds r3, #100
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r1, r10
	subs r3, r3, r1
	ldr r2, [pc, #216]
	lsls r3, r3, #2
	adds r3, #64
	ldr r2, [r2, r3]
	mov r10, r2
	movs r2, #102
	adds r2, r2, r7
	mov r8, r2
	mov r1, r8
	movs r0, #0
	ldrsh r3, [r1, r0]
	ldrh r2, [r2]
	cmp r3, #0
	beq .L_02003738_1
	subs r3, r2, #1
	movs r0, #160
	mov r2, r8
	strh r3, [r2]
	lsls r0, r0, #13
	lsls r3, r3, #16
	cmp r3, r0
	bne .L_02003738_2
	movs r0, #184
	bl 0x0200bd20
.L_02003738_2:
	mov r2, r8
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #0
	bne .L_02003738_1
	movs r0, #233
	bl 0x0200bd20
.L_02003738_1:
	ldr r3, [r7, #8]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	movs r1, #192
	str r3, [r5, #8]
	mov r0, r10
	lsls r1, r1, #8
	ldr r3, [pc, #132]
	movs r0, r0
	mov r12, pc
	bx r3
	.2byte 0x4649
	.2byte 0x1c2a
	.2byte 0xf000
	.2byte 0xf96e
	.2byte 0x6829
	.2byte 0x60b9
	.2byte 0x68aa
	.2byte 0x2002
	.2byte 0x613a
	.2byte 0xf000
	.2byte 0xf997
	.2byte 0x21c0
	.2byte 0x1c06
	.2byte 0x0249
	.2byte 0x4650
	.2byte 0x4b18
	.2byte 0x46fc
	.2byte 0x4718
	.2byte 0x4240
	.2byte 0x4649
	.2byte 0x1c2a
	.2byte 0xf000
	.2byte 0xf95b
	.2byte 0x6829
	.2byte 0x68aa
	.2byte 0x2002
	.2byte 0xf000
	.2byte 0xf986
	.2byte 0x4642
	.2byte 0x2100
	.2byte 0x5e53
	.2byte 0x2b14
	.2byte 0xdc11
	.2byte 0x4286
	.2byte 0xd104
	.2byte 0x1c38
	.2byte 0x2102
	.2byte 0xf000
	.2byte 0xf95b
	.2byte 0xe00a
	.2byte 0x4286
	.2byte 0xdd04
	.2byte 0x1c38
	.2byte 0x2103
	.2byte 0xf000
	.2byte 0xf954
	.2byte 0xe003
	.2byte 0x1c38
	.2byte 0x2104
	.2byte 0xf000
	.2byte 0xf94f
	.2byte 0xb003
	.2byte 0xbc68
	.2byte 0x4698
	.2byte 0x46a9
	.2byte 0x46b2
	.2byte 0xbce0
	.2byte 0xbc01
	.2byte 0x4700
	.2byte 0x0000
	.4byte 0x0000ffff
	.4byte 0x0200d1b4
	.4byte 0x03000118
	.global Func_02003850
	.thumb_func
Func_02003850:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	mov r8, r1
	mov r9, r0
	bl 0x0200bb98
	ldr r3, [pc, #36]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl 0x0200bcc8
	mov r10, r0
	bl 0x0200bb70
	movs r3, #1
	negs r3, r3
	cmp r8, r3
	bne .L_02003850_0
	ldrh r2, [r6, #6]
	mov r8, r2
.L_02003850_0:
	movs r7, #0
	mov r5, sp
	b .L_02003850_1
	.4byte 0x02000240
.L_02003850_3:
	movs r3, #128
	lsls r3, r3, #7
	add r8, r3
	adds r7, #1
.L_02003850_1:
	cmp r7, #3
	bgt .L_02003850_2
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	movs r0, #128
	str r3, [r5, #8]
	lsls r0, r0, #13
	mov r1, r8
	adds r2, r5, #0
	bl 0x0200bab0
	ldr r1, [r5]
	ldr r2, [r5, #8]
	movs r0, #2
	bl 0x0200bb10
	ldr r3, [r6, #12]
	cmp r0, r3
	bne .L_02003850_3
.L_02003850_2:
	cmp r7, #4
	beq .L_02003850_4
	adds r2, r6, #0
	movs r3, #2
	adds r2, #34
	strb r3, [r2]
	movs r5, #0
	mov r2, r10
	str r5, [r2, #8]
	str r5, [r2, #16]
	movs r1, #16
	ldr r0, [r6, #80]
	bl 0x0200bac0
	mov r0, r9
	movs r1, #1
	bl 0x0200bc70
	bl 0x0200bc88
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #13
	lsls r1, r1, #10
	bl 0x0200bc78
	mov r3, r8
	strh r3, [r6, #6]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #48]
	ldr r3, [pc, #28]
	ldr r2, [pc, #20]
	str r3, [r6, #52]
	adds r3, r6, #0
	adds r3, #91
	strb r2, [r3]
	ldr r2, [r6, #12]
	cmp r2, #0
	bge .L_02003850_5
	ldr r3, [pc, #12]
	adds r2, r2, r3
	b .L_02003850_5
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00000ccc
	.4byte 0x0000ffff
.L_02003850_5:
	adds r3, r6, #0
	asrs r2, r2, #16
	adds r3, #100
	strh r2, [r3]
	adds r3, #2
	strh r5, [r3]
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	movs r0, #192
	str r3, [r5, #8]
	lsls r0, r0, #13
	mov r1, r8
	adds r2, r5, #0
	bl 0x0200bab0
	ldr r1, [r5]
	ldr r2, [r6, #12]
	ldr r3, [r5, #8]
	adds r0, r6, #0
	bl 0x0200baf8
	adds r0, r6, #0
	bl 0x0200bb00
	movs r0, #233
	bl 0x0200bd20
.L_02003850_12:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #2
	bl 0x0200bb18
	cmp r0, #98
	beq .L_02003850_6
	cmp r0, #98
	bgt .L_02003850_7
	cmp r0, #96
	beq .L_02003850_8
	cmp r0, #97
	beq .L_02003850_9
	b .L_02003850_10
.L_02003850_7:
	cmp r0, #99
	beq .L_02003850_11
	b .L_02003850_10
.L_02003850_6:
	adds r0, r6, #0
	bl 0x0200b5ac
	b .L_02003850_10
.L_02003850_9:
	adds r0, r6, #0
	bl 0x0200b668
	b .L_02003850_10
.L_02003850_8:
	adds r0, r6, #0
	bl 0x0200b724
.L_02003850_10:
	adds r0, r6, #0
	bl 0x0200b738
	movs r0, #1
	bl 0x0200ba78
	b .L_02003850_12
.L_02003850_11:
	adds r0, r6, #0
	bl 0x0200b484
	bl 0x0200bb78
.L_02003850_4:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020039c8
	.thumb_func
Func_020039c8:
	push {r5, r6, lr}
	ldr r3, [pc, #48]
	ldr r4, [pc, #48]
	ldr r6, [r3]
	movs r2, #0
	ldrsh r3, [r4, r2]
	cmp r3, #0
	bgt .L_020039c8_0
.L_020039c8_2:
	ldr r1, [pc, #40]
	ldrh r3, [r1]
	ldr r5, [pc, #40]
	adds r2, r3, #1
	lsls r3, r3, #16
	asrs r3, r3, #16
	ldrsb r0, [r5, r3]
	movs r3, #1
	negs r3, r3
	strh r2, [r1]
	cmp r0, r3
	bne .L_020039c8_1
	ldr r3, [pc, #4]
	strh r3, [r1]
	b .L_020039c8_2
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001ed0
	.4byte 0x0200d25c
	.4byte 0x0200d260
	.4byte 0x0200bd28
.L_020039c8_1:
	adds r3, r2, #1
	strh r3, [r1]
	lsls r3, r2, #16
	asrs r3, r3, #16
	ldrsb r3, [r5, r3]
	ldr r4, [pc, #28]
	lsls r0, r0, #1
	strh r3, [r4]
	adds r0, r6, r0
	ldr r3, [pc, #24]
	ldr r1, [pc, #24]
	ldr r2, [pc, #28]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_020039c8_0:
	ldrh r3, [r4]
	subs r3, #1
	strh r3, [r4]
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200d25c
	.4byte 0x040000d4
	.4byte 0x05000006
	.4byte 0x80000009
	.global Func_02003a44
	.thumb_func
Func_02003a44:
	push {lr}
	ldr r2, [pc, #20]
	ldr r3, [pc, #20]
	strh r2, [r3]
	ldr r3, [pc, #20]
	movs r1, #200
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #16]
	bl 0x0200ba80
	b .L_02003a44_0
	.4byte 0x00000000
	.4byte 0x0200d260
	.4byte 0x0200d25c
	.4byte 0x0200b9c9
.L_02003a44_0:
	pop {r0}
	bx r0
	.include "games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x20021003
	.4byte 0x20024001
	.4byte 0x000000ff
	.4byte 0x00000022
	.4byte 0x02008041
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000222
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000222
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000006c
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fc0000
	.4byte 0x00000000
	.4byte 0x011a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01460000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01660000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01660000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03120000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01b60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03240000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x031a0000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03560000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000400
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x020080bd
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x013a0000
	.4byte 0x00000000
	.4byte 0x007e0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x015a0000
	.4byte 0x00000000
	.4byte 0x007e0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x008a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000080
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00760000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001c8
	.4byte 0xc0000208
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0xffff0062
	.4byte 0x000001c8
	.4byte 0xc00001e8
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0xffff0063
	.4byte 0x00000068
	.4byte 0x400000e8
	.4byte 0x00000000
	.4byte 0x01fa0038
	.4byte 0x00000212
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc00001d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000038
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001e8
	.4byte 0xc0000230
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000298
	.4byte 0x4000017c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000388
	.4byte 0xc00000d6
	.4byte 0x034a0000
	.4byte 0x03f80028
	.4byte 0x000000f8
	.4byte 0xffff0062
	.4byte 0x000001e8
	.4byte 0xc0000230
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000140
	.4byte 0x400001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc00001f2
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0002
	.4byte 0x00000077
	.4byte 0x4000006c
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0003
	.4byte 0x00000237
	.4byte 0x40000069
	.4byte 0x00180000
	.4byte 0x02880000
	.4byte 0x00000230
	.4byte 0xffff0004
	.4byte 0x00000377
	.4byte 0xc000031e
	.4byte 0x02c80000
	.4byte 0x03c00238
	.4byte 0x0000035c
	.4byte 0xffff0005
	.4byte 0x00000317
	.4byte 0x400002a7
	.4byte 0x02c80000
	.4byte 0x03c00230
	.4byte 0x0000035c
	.4byte 0xffff0006
	.4byte 0x00000058
	.4byte 0xc0000348
	.4byte 0x00200000
	.4byte 0x011002c0
	.4byte 0x00000360
	.4byte 0xffff0007
	.4byte 0x000000d8
	.4byte 0xc0000348
	.4byte 0x00200000
	.4byte 0x011002c0
	.4byte 0x00000360
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000249
	.4byte 0xc00001f5
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0062
	.4byte 0x00000249
	.4byte 0xc00001f5
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0063
	.4byte 0x000002eb
	.4byte 0x40000102
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000238
	.4byte 0xc000015c
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0002
	.4byte 0x00000127
	.4byte 0x4000005f
	.4byte 0x00000000
	.4byte 0x031e0000
	.4byte 0x00000212
	.4byte 0xffff0003
	.4byte 0x000001ba
	.4byte 0xc0000306
	.4byte 0x00000000
	.4byte 0x02000212
	.4byte 0x000003b4
	.4byte 0xffff0004
	.4byte 0x00000048
	.4byte 0xc0000366
	.4byte 0x00000000
	.4byte 0x02000212
	.4byte 0x000003b4
	.4byte 0xffff0005
	.4byte 0x00000396
	.4byte 0x400002c8
	.4byte 0x02080000
	.4byte 0x03e0023a
	.4byte 0x00000312
	.4byte 0xffff0006
	.4byte 0x00000267
	.4byte 0x400002ac
	.4byte 0x02080000
	.4byte 0x03e0023a
	.4byte 0x00000312
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc00001f6
	.4byte 0x00000000
	.4byte 0x03340000
	.4byte 0x00000238
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0x400000ae
	.4byte 0x00000000
	.4byte 0x03340000
	.4byte 0x00000238
	.4byte 0xffff0003
	.4byte 0x00000108
	.4byte 0xc000037a
	.4byte 0x00900000
	.4byte 0x018002d0
	.4byte 0x0000038c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000188
	.4byte 0x40000074
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00000234
	.4byte 0xffff0002
	.4byte 0x00000028
	.4byte 0xc0000074
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00000234
	.4byte 0xffff0003
	.4byte 0x00000208
	.4byte 0x40000268
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0xc00003ca
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0xffff0005
	.4byte 0x000003b7
	.4byte 0x40000317
	.4byte 0x00f40000
	.4byte 0x04000234
	.4byte 0x00000400
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000169
	.4byte 0xc0000135
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x00000330
	.4byte 0xffff0002
	.4byte 0x00000059
	.4byte 0xc00002ed
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x00000330
	.4byte 0xffff0003
	.4byte 0x000002d8
	.4byte 0xc00002cd
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0004
	.4byte 0x000003a7
	.4byte 0x400001ed
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0005
	.4byte 0x00000317
	.4byte 0x40000236
	.4byte 0x026a0000
	.4byte 0x03de0000
	.4byte 0x00000356
	.4byte 0xffff0006
	.4byte 0x00100318
	.4byte 0x40000241
	.4byte 0x026a0000
	.4byte 0x03e60000
	.4byte 0x00000356
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00040058
	.4byte 0x400001a8
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0xffff0062
	.4byte 0x00040058
	.4byte 0x400001a8
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0xffff0063
	.4byte 0x00000148
	.4byte 0xc0000076
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x0000020c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a0
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0xc0000205
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0xffff0002
	.4byte 0x000000c6
	.4byte 0x40000150
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0xffff0003
	.4byte 0x00000238
	.4byte 0xc0000088
	.4byte 0x01e00000
	.4byte 0x02ac000f
	.4byte 0x000000b4
	.4byte 0xffff0004
	.4byte 0x00000049
	.4byte 0xc00000be
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000024e
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff960316
	.4byte 0x031a023f
	.4byte 0x0243ff9a
	.4byte 0x0006ffff
	.4byte 0xff940314
	.4byte 0x031c0243
	.4byte 0x024bff9c
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00260054
	.4byte 0x005c01a4
	.4byte 0x01ac002e
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000004d
	.4byte 0x0010a04b
	.4byte 0x0000004e
	.4byte 0x00103050
	.4byte 0x0020104f
	.4byte 0x0000004f
	.4byte 0x0010204e
	.4byte 0x0020304f
	.4byte 0x0030204f
	.4byte 0x00000050
	.4byte 0x0010b04b
	.4byte 0x00204050
	.4byte 0x0030104e
	.4byte 0x00402050
	.4byte 0x00501052
	.4byte 0x0060404a
	.4byte 0x0070504a
	.4byte 0x00000051
	.4byte 0x00106052
	.4byte 0x00000052
	.4byte 0x00105050
	.4byte 0x00203052
	.4byte 0x00302052
	.4byte 0x00405052
	.4byte 0x00504052
	.4byte 0x00601051
	.4byte 0x00000053
	.4byte 0x00101054
	.4byte 0x00201055
	.4byte 0x0030d04b
	.4byte 0x00000054
	.4byte 0x00101053
	.4byte 0x00205055
	.4byte 0x00301057
	.4byte 0x0040c04b
	.4byte 0x00503055
	.4byte 0x00000055
	.4byte 0x00102053
	.4byte 0x00204055
	.4byte 0x00305054
	.4byte 0x00402055
	.4byte 0x00502054
	.4byte 0x00601056
	.4byte 0x00000056
	.4byte 0x00106055
	.4byte 0x00000057
	.4byte 0x00103054
	.4byte 0x00203057
	.4byte 0x00302057
	.4byte 0x00434002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00020000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00028000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x03780000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x02f70000
	.4byte 0x00000000
	.4byte 0x011d0000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00020000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0x0047005b
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000007
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x03060000
	.4byte 0x00000000
	.4byte 0x004c0000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x03060000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0127
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x0904000a
	.4byte 0x02009839
	.4byte 0x00000002
	.4byte 0x0905000b
	.4byte 0x02009d0d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008389
	.4byte 0x00008d15
	.4byte 0xffff040a
	.4byte 0x02008389
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00001815
	.4byte 0x02000008
	.4byte 0x0200847d
	.4byte 0x00001815
	.4byte 0x02010009
	.4byte 0x020084cd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020083b5
	.4byte 0x00008d15
	.4byte 0xffff0409
	.4byte 0x020083b5
	.4byte 0x00008f15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0xffff0053
	.4byte 0x02008a65
	.4byte 0x00000013
	.4byte 0x0f760064
	.4byte 0x001000c0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000013
	.4byte 0x0f050064
	.4byte 0x001000b4
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte 0x0200855d
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020083e1
	.4byte 0x00008d15
	.4byte 0xffff040a
	.4byte 0x020083e1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte 0x02008b0d
	.4byte 0x00002413
	.4byte 0x0f770064
	.4byte 0x0010007b
	.4byte 0x00000413
	.4byte 0x0f770064
	.4byte 0x0010007b
	.4byte 0x0000e413
	.4byte 0x0f770064
	.4byte 0x0010007b
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
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte 0x020085ad
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte 0x02008b3d
	.4byte 0x00000013
	.4byte 0x0ef20064
	.4byte 0x00500002
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
	.4byte 0x00000013
	.4byte 0x0f070065
	.4byte 0x001000ba
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008031
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082cd
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte 0x02008bd9
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008c75
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008c89
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x0200840d
	.4byte 0x00000602
	.4byte 0xffff000c
	.4byte 0x02008c9d
	.4byte 0x00001815
	.4byte 0x02000009
	.4byte 0x0200850d
	.4byte 0x00001815
	.4byte 0x0201000b
	.4byte 0x020085fd
	.4byte 0x00001815
	.4byte 0x0204000c
	.4byte 0x02008651
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
	.4byte 0x00000013
	.4byte 0x0f780064
	.4byte 0x001000e5
	.4byte 0x00008f15
	.4byte 0xffff0008
	.4byte 0x00000000
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
	.2byte 0x0001
.L_020050f2:
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0005
	movs	r0, r0
	movs	r3, r0
	movs	r0, r0
	movs	r4, r1
	.2byte 0xffff
	.2byte 0x9399
	lsls	r0, r0, #8
	ldrh	r5, [r2, #48]
	movs	r0, r0
	movs	r3, r1
	lsrs	r0, r1, #4
	ldrh	r5, [r5, #40]
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r2, r0
	movs	r0, r0
	movs	r2, r1
	lsrs	r1, r1, #4
	add	r7, sp, #68
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
	lsls	r4, r4, #1
	lsrs	r0, r1, #28
	lsls	r3, r1, #3
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r1, r0
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
.L_0200515e:
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r1, r0
.L_0200516a:
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
.L_02005172:
	movs	r0, r0
.L_02005174:
	movs	r1, r0
	movs	r0, r0
	movs	r4, r0
.L_0200517a:
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	ldrh	r5, [r2, #56]
.L_02005182:
	movs	r0, r0
.L_02005184:
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0000
	movs	r0, r0
	ldrh	r5, [r2, #32]
	movs	r0, r0
.L_02005190:
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x0000
.L_02005196:
	movs	r0, r0
.L_02005198:
	movs	r3, r2
	movs	r0, r0
.L_0200519c:
	lsls	r4, r4, #1
.L_0200519e:
	lsrs	r1, r7, #29
.L_020051a0:
	lsls	r6, r6, #2
	movs	r0, r2
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020051b0:
	.2byte 0x0000
.L_020051b2:
	.2byte 0x0000
	movs	r0, r0
	movs	r1, r0
	lsrs	r5, r2, #13
.L_020051ba:
	movs	r1, r0
.L_020051bc:
	asrs	r3, r5, #28
.L_020051be:
	movs	r1, r0
	movs	r3, #135
	movs	r1, r0
	adds	r0, #111
	movs	r1, r0
.L_020051c8:
	subs	r5, #234
	movs	r1, r0
	ldr	r3, [pc, #1012]
	movs	r1, r0
	ldrh	r0, [r6, r2]
.L_020051d2:
	movs	r1, r0
	ldr	r1, [r1, #32]
.L_020051d6:
	movs	r1, r0
	ldrb	r1, [r2, #8]
.L_020051da:
	movs	r1, r0
	ldrh	r6, [r1, #22]
	movs	r1, r0
.L_020051e0:
	ldr	r4, [sp, #292]
	movs	r1, r0
	add	r6, sp, #548
	movs	r1, r0
.L_020051e8:
	stmia	r1!, {r0, r3, r4, r7}
	movs	r1, r0
	bpl.n	.L_020050f2
	movs	r1, r0
	.2byte 0xea4b
	.2byte 0x0001
	movs	r0, r0
	movs	r2, r0
	asrs	r3, r5, #26
	movs	r2, r0
	cmp	r6, #86
	movs	r2, r0
	.2byte 0x470f
	movs	r2, r0
	str	r7, [r3, #12]
	movs	r2, r0
	ldrb	r4, [r2, #15]
.L_0200520a:
	movs	r2, r0
	str	r7, [sp, #1004]
	movs	r2, r0
	push	{r5, r6, lr}
	movs	r2, r0
.L_02005214:
	bmi.n	0x0200d23e
	movs	r2, r0
.L_02005218:
	.2byte 0xf422
	.2byte 0x0002
	asrs	r4, r3, #22
	movs	r3, r0
	subs	r0, #146
	movs	r3, r0
	ldrb	r3, [r2, r4]
	movs	r3, r0
	strh	r3, [r6, #24]
	movs	r3, r0
.L_0200522c:
	add	r3, sp, #12
	movs	r3, r0
	bmi.n	.L_0200515e
	movs	r3, r0
	movs	r0, r0
	movs	r4, r0
