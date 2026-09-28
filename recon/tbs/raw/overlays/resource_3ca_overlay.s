.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020096c8
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
	.4byte 0x02009710
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200971c
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, [pc, #28]
	movs r2, #182
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl 0x02009464
	movs r2, #0
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {lr}
	bl 0x02009188
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	movs r0, #128
	sub sp, #12
	lsls r0, r0, #8
	str r0, [sp, #4]
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #128
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #9
	str r0, [sp, #8]
	lsls r1, r1, #11
	lsls r3, r3, #6
	movs r0, #0
	str r2, [sp, #0]
	bl 0x020090d4
	sub sp, #-12
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020000a8
	.thumb_func
Func_020000a8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020097ac
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	push {r5, lr}
	ldr r5, [pc, #56]
	movs r1, #6
	ldrh r0, [r5]
	bl 0x02009314
	ldr r2, [pc, #48]
	lsls r0, r0, #16
	lsrs r0, r0, #15
	adds r0, r0, r2
	ldr r3, [pc, #44]
	ldr r1, [pc, #48]
	ldr r2, [pc, #48]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldrh r3, [r5]
	movs r2, #140
	adds r3, #1
	strh r3, [r5]
	lsls r2, r2, #14
	lsls r3, r3, #16
	cmp r3, r2
	bls .L_020000b0_0
	ldr r3, [pc, #8]
	strh r3, [r5]
.L_020000b0_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000000
	.4byte 0x0200981c
	.4byte 0x020094ac
	.4byte 0x040000d4
	.4byte 0x050000e8
	.4byte 0x80000006
	.global Func_02000100
	.thumb_func
Func_02000100:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_02000100_0
	subs r3, #1
	strh r3, [r2]
	bl 0x02009334
	adds r5, r0, #0
	bl 0x02009334
	ldr r3, [r6, #8]
	subs r5, r5, r0
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r2, [pc, #52]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	b .L_02000100_1
.L_02000100_0:
	adds r2, r6, #0
	adds r2, #102
	movs r0, #0
	ldrsh r3, [r2, r0]
	cmp r3, #0
	beq .L_02000100_1
	strh r1, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200936c
	adds r2, r6, #0
	adds r2, #94
	movs r3, #20
	strh r3, [r2]
	ldr r1, [pc, #16]
	adds r0, r6, #0
	bl 0x02009374
.L_02000100_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x020094c4
	.global Func_02000168
	.thumb_func
Func_02000168:
	push {lr}
	ldr r2, [pc, #32]
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	cmp r3, #40
	bne .L_02000168_0
	ldr r1, [pc, #24]
	ldr r3, [r1]
	cmp r3, #4
	ble .L_02000168_0
	subs r3, #1
	str r3, [r1]
	movs r3, #0
	str r3, [r2]
.L_02000168_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x020097f4
	.4byte 0x020097f0
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #444]
	ldr	r3, [r3, #0]
	mov	r9, r3
	ldr	r3, [pc, #440]
	ldr	r3, [r3, #0]
	sub	sp, #20
	cmp	r3, #0
	beq.n	.L_020001e0
	ldr	r5, [pc, #436]
	ldr	r0, [r5, #0]
	lsls	r0, r0, #9
	bl 0x0200933c
	ldr	r3, [pc, #428]
	movs	r1, #3
	mov	ip, pc
	bx	r3
	.4byte 0x30084b6a
	.4byte 0x466a681b
	.4byte 0x32120200
	.4byte 0x8013181b
	.4byte 0x4b678812
	.4byte 0x682b801a
	.2byte 0x3301
	.2byte 0x602b
.L_020001e0:
	ldr	r3, [pc, #404]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020002bc
	ldr	r2, [pc, #400]
	ldr	r0, [r2, #0]
	lsls	r0, r0, #9
	mov	fp, r2
	bl 0x0200933c
	ldr	r3, [pc, #372]
	movs	r1, #2
	mov	ip, pc
	bx	r3
	.4byte 0x22a04b60
	.4byte 0x0406681b
	.4byte 0x444a0052
	.4byte 0x6013199b
	.4byte 0x22b84b5d
	.4byte 0x0052681b
	.4byte 0x199b444a
	.4byte 0x4b5b6013
	.4byte 0x46984a5b
	.4byte 0x4692681b
	.4byte 0xd00c4553
	.4byte 0xf0012000
	.4byte 0x4642f8d3
	.4byte 0x1c056813
	.4byte 0x1c2a199b
	.4byte 0x616b60eb
	.4byte 0x23003255
	.4byte 0x4f537013
	.4byte 0x4553683b
	.4byte 0x2001d00e
	.4byte 0xf8c2f001
	.4byte 0x1c05683b
	.4byte 0x60eb199b
	.4byte 0x68134642
	.4byte 0x199b1c2a
	.4byte 0x3255616b
	.4byte 0x70132300
	.4byte 0x683b4f4a
	.4byte 0xd00e4553
	.4byte 0xf0012003
	.4byte 0x683bf8af
	.4byte 0x199b1c05
	.4byte 0x464260eb
	.4byte 0x1c2a6813
	.4byte 0x616b199b
	.4byte 0x23003255
	.4byte 0x4f427013
	.4byte 0x4553683b
	.4byte 0x2002d00e
	.4byte 0xf89cf001
	.4byte 0x1c05683b
	.4byte 0x60eb199b
	.4byte 0x68134642
	.4byte 0x199b1c2a
	.4byte 0x3255616b
	.4byte 0x70132300
	.4byte 0x6813465a
	.2byte 0x3301
	.2byte 0x6013
.L_020002bc:
	ldr	r3, [pc, #220]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020003ac
	ldr	r3, [pc, #216]
	ldr	r3, [r3, #0]
	movs	r7, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_020003ac
	mov	r3, r9
	adds	r3, #228
	ldr	r6, [r3, #0]
	adds	r3, #4
	ldr	r2, [pc, #176]
	ldr	r5, [r3, #0]
	ands	r6, r2
	ands	r5, r2
	bl 0x02009334
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r3, r3, #4
	add	r2, sp, #4
	adds	r6, r6, r3
	movs	r3, #0
	str	r3, [r2, #4]
	str	r6, [r2, #0]
	str	r2, [sp, #0]
	bl 0x02009334
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #5
	adds	r5, r5, r3
	movs	r3, #240
	ldr	r2, [sp, #0]
	lsls	r3, r3, #13
	adds	r5, r5, r3
	str	r5, [r2, #8]
	ldr	r1, [r2, #0]
	adds	r3, r5, #0
	ldr	r2, [r2, #4]
	ldr	r0, [pc, #144]
	bl 0x0200937c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020003ac
	ldr	r3, [pc, #136]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #100
	movs	r3, #60
	strh	r3, [r2, #0]
	adds	r3, r5, #0
	ldr	r1, [pc, #44]
	adds	r3, #102
	strh	r7, [r3, #0]
	subs	r3, #17
	strb	r1, [r3, #0]
	subs	r2, #65
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r1, [r5, #80]
	ldrb	r2, [r1, #9]
	subs	r3, #15
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r1, #0
	bl 0x02009394
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200936c
	b.n	.L_020003ac
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001e70
	.4byte 0x020097e8
	.4byte 0x020097ec
	.4byte 0x03000118
	.4byte 0x020097f0
	.4byte 0x04000052
	.4byte 0x020097fc
	.4byte 0x02009800
	.4byte 0x02009804
	.4byte 0x02009808
	.4byte 0x0200980c
	.4byte 0xffff0000
	.4byte 0x02009810
	.4byte 0x02009814
	.4byte 0x02009818
	.4byte 0x020097f8
	.4byte 0x03001e40
	.4byte 0x000001f7
	.2byte 0x8101
	.2byte 0x0200
.L_020003ac:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.global Func_020003c0
	.thumb_func
Func_020003c0:
	push {r5, lr}
	movs r0, #0
	ldr r5, [pc, #16]
	bl 0x020093d4
	ldr r3, [r0, #12]
	movs r0, #0
	str r3, [r5]
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0200980c
	.global Func_020003dc
	.thumb_func
Func_020003dc:
	push {r5, lr}
	movs r0, #1
	ldr r5, [pc, #16]
	bl 0x020093d4
	ldr r3, [r0, #12]
	movs r0, #0
	str r3, [r5]
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02009810
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	push {r5, lr}
	movs r0, #3
	ldr r5, [pc, #16]
	bl 0x020093d4
	ldr r3, [r0, #12]
	movs r0, #0
	str r3, [r5]
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02009814
	.global Func_02000414
	.thumb_func
Func_02000414:
	push {r5, lr}
	movs r0, #2
	ldr r5, [pc, #16]
	bl 0x020093d4
	ldr r3, [r0, #12]
	movs r0, #0
	str r3, [r5]
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02009818
	.global Func_02000430
	.thumb_func
Func_02000430:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x020093bc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl 0x0200944c
	movs r3, #18
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #3
	movs r3, #1
	movs r0, #18
	bl 0x0200938c
	ldr r0, [pc, #936]
	bl 0x0200941c
	movs r1, #10
	movs r3, #192
	movs r0, #1
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #8
	bl 0x02009484
	movs r3, #192
	movs r0, #3
	movs r1, #0
	movs r2, #24
	lsls r3, r3, #8
	bl 0x02009484
	movs r3, #192
	lsls r3, r3, #8
	movs r2, #16
	movs r1, #10
	movs r0, #2
	bl 0x02009484
	movs r0, #1
	bl 0x020093fc
	movs r0, #50
	bl 0x020093b4
	movs r1, #2
	movs r0, #1
	bl 0x02009414
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #1
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #3
	bl 0x02009434
	movs r0, #40
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #2
	bl 0x02009434
	movs r0, #60
	bl 0x020093b4
	movs r0, #0
	movs r1, #3
	bl 0x02009404
	movs r0, #1
	movs r1, #3
	bl 0x02009404
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #60
	bl 0x020093b4
	movs r1, #2
	movs r0, #1
	bl 0x02009414
	movs r0, #30
	bl 0x020093b4
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #1
	bl 0x02009424
	movs r0, #0
	movs r1, #0
	bl 0x020093cc
	cmp r0, #0
	bne .L_02000430_0
	movs r0, #30
	bl 0x020093b4
	movs r1, #129
	movs r2, #50
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200943c
	movs r0, #1
	movs r1, #0
	bl 0x0200942c
	ldr r3, [pc, #640]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000430_1
.L_02000430_0:
	movs r0, #30
	bl 0x020093b4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #50
	bl 0x0200943c
	ldr r3, [pc, #604]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #1
	movs r1, #0
	bl 0x0200942c
.L_02000430_1:
	movs r0, #10
	bl 0x020093b4
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #2
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r2, #50
	movs r0, #2
	ldr r1, [pc, #556]
	bl 0x0200943c
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #516]
	bl 0x0200943c
	movs r1, #2
	movs r0, #3
	bl 0x02009414
	movs r0, #30
	bl 0x020093b4
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #128
	movs r2, #50
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200943c
	movs r1, #0
	movs r0, #1
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #2
	movs r0, #2
	bl 0x02009414
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r1, #4
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x02009424
	movs r0, #0
	movs r1, #0
	bl 0x020093cc
	cmp r0, #0
	bne .L_02000430_2
	movs r0, #20
	bl 0x020093b4
	movs r1, #2
	movs r0, #1
	bl 0x02009414
	movs r0, #20
	bl 0x020093b4
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r0, #1
	movs r1, #0
	bl 0x0200942c
	ldr r3, [pc, #308]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000430_3
.L_02000430_2:
	movs r0, #20
	bl 0x020093b4
	movs r1, #2
	movs r0, #1
	bl 0x02009414
	movs r0, #20
	bl 0x020093b4
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	ldr r3, [pc, #252]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #1
	movs r1, #0
	bl 0x0200942c
.L_02000430_3:
	movs r0, #10
	bl 0x020093b4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #0
	bl 0x0200943c
	movs r0, #10
	bl 0x020093b4
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #1
	bl 0x0200942c
	movs r0, #20
	bl 0x020093b4
	movs r1, #3
	movs r0, #0
	bl 0x0200940c
	movs r0, #30
	bl 0x020093b4
	movs r0, #10
	bl 0x020093b4
	movs r1, #3
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x02009424
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #2
	movs r2, #0
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r0, #0
	movs r1, #0
	bl 0x020093cc
	cmp r0, #0
	bne .L_02000430_4
	movs r0, #30
	bl 0x020093b4
	movs r1, #3
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r0, #3
	movs r1, #0
	bl 0x0200942c
	ldr r3, [pc, #20]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02000430_5
	.4byte 0x000028fe
	.4byte 0x03001ebc
	.4byte 0x00000101
.L_02000430_4:
	movs r0, #30
	bl 0x020093b4
	movs r1, #3
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	ldr r3, [pc, #724]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #3
	movs r1, #0
	bl 0x0200942c
.L_02000430_5:
	movs r0, #10
	bl 0x020093b4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r1, #0
	movs r0, #1
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r0, #10
	bl 0x020093b4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #2
	bl 0x02009434
	movs r0, #30
	bl 0x020093b4
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #2
	bl 0x0200942c
	movs r0, #10
	bl 0x020093b4
	movs r1, #3
	movs r0, #3
	bl 0x0200940c
	movs r0, #20
	bl 0x020093b4
	movs r1, #0
	movs r0, #3
	bl 0x0200942c
	movs r0, #17
	bl 0x0200949c
	movs r0, #10
	bl 0x020093b4
	movs r0, #0
	movs r1, #3
	bl 0x02009404
	movs r0, #1
	movs r1, #3
	bl 0x02009404
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #40
	bl 0x020093b4
	movs r0, #0
	ldr r1, [pc, #532]
	ldr r2, [pc, #532]
	bl 0x020093dc
	movs r0, #1
	ldr r1, [pc, #520]
	ldr r2, [pc, #524]
	bl 0x020093dc
	movs r0, #2
	ldr r1, [pc, #512]
	ldr r2, [pc, #512]
	bl 0x020093dc
	ldr r2, [pc, #508]
	movs r0, #3
	ldr r1, [pc, #500]
	bl 0x020093dc
	ldr r1, [pc, #500]
	movs r0, #0
	bl 0x020093e4
	movs r0, #50
	bl 0x020093b4
	movs r0, #132
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200944c
	ldr r1, [pc, #472]
	movs r0, #1
	bl 0x020093e4
	movs r0, #50
	bl 0x020093b4
	ldr r1, [pc, #464]
	movs r0, #2
	bl 0x020093e4
	movs r0, #2
	bl 0x020093ec
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009434
	movs r2, #32
	movs r1, #0
	negs r2, r2
	movs r0, #3
	bl 0x0200948c
	movs r0, #30
	bl 0x020093b4
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #3
	bl 0x02009434
	movs r0, #60
	bl 0x020093b4
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	ldr r1, [pc, #364]
	movs r0, #3
	bl 0x020093e4
	movs r0, #3
	bl 0x020093ec
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #3
	bl 0x02009434
	movs r0, #20
	bl 0x020093b4
	movs r0, #216
	movs r1, #1
	movs r2, #168
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl 0x0200944c
	bl 0x02009454
	movs r0, #20
	bl 0x020093b4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009434
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009434
	movs r0, #40
	bl 0x020093b4
	movs r0, #0
	movs r1, #3
	bl 0x02009404
	movs r0, #1
	movs r1, #3
	bl 0x02009404
	movs r0, #3
	movs r1, #3
	bl 0x02009404
	movs r1, #3
	movs r0, #2
	bl 0x0200940c
	movs r0, #30
	bl 0x020093b4
	movs r0, #67
	bl 0x0200949c
	movs r0, #240
	bl 0x020094a4
	bl 0x020092ac
	movs r0, #80
	bl 0x020093b4
	ldr r6, [pc, #192]
	movs r0, #8
	ldr r5, [r6]
	bl 0x020093d4
	movs r3, #131
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #48]
	movs r3, #142
	lsls r3, r3, #1
	adds r5, r5, r3
	ldr r3, [pc, #172]
	movs r0, #8
	str r3, [r5]
	movs r1, #60
	movs r2, #0
	bl 0x0200948c
	ldr r3, [pc, #160]
	movs r2, #0
	str r3, [r5]
	movs r1, #60
	movs r0, #8
	bl 0x0200948c
	movs r0, #80
	bl 0x020093b4
	movs r0, #100
	bl 0x020093b4
	ldr r0, [pc, #140]
	ldr r1, [pc, #140]
	bl 0x02009444
	movs r0, #202
	movs r1, #1
	movs r2, #168
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	bl 0x0200944c
	movs r0, #150
	lsls r0, r0, #1
	bl 0x020093b4
	ldr r1, [r6, #76]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	movs r2, #0
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	movs r3, #228
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #96
	str r3, [r2]
	bl 0x0200946c
	bl 0x02009474
	movs r0, #30
	bl 0x020093b4
	movs r0, #141
	lsls r0, r0, #1
	bl 0x020093ac
	ldr r0, [pc, #60]
	movs r1, #9
	bl 0x0200945c
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x02009820
	.4byte 0x020098e0
	.4byte 0x0200998c
	.4byte 0x02009a4c
	.4byte 0x03001e70
	.4byte 0xffffd000
	.4byte 0xffffa000
	.4byte 0x00023333
	.4byte 0x0000028f
	.4byte 0x00000000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #372]
	ldr	r3, [r3, #0]
	sub	sp, #8
	mov	sl, r3
	bl 0x020093bc
	bl 0x0200947c
	movs	r1, #156
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #232
	bl 0x020093f4
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #0
	bl 0x02009434
	movs	r0, #40
	bl 0x020093b4
	movs	r0, #140
	bl 0x0200949c
	movs	r6, #160
	movs	r5, #0
	lsls	r6, r6, #19
.L_02000b76:
	lsls	r3, r5, #11
	lsls	r2, r5, #5
	orrs	r3, r2
	strh	r3, [r6, #0]
	movs	r0, #10
	adds	r5, #1
	bl 0x020093b4
	cmp	r5, #15
	ble.n	.L_02000b76
	movs	r2, #252
	movs	r3, #160
	lsls	r2, r2, #7
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	ldr	r2, [pc, #288]
	movs	r7, #129
	ldr	r6, [pc, #288]
	mov	r8, r2
	lsls	r7, r7, #4
	movs	r5, #2
.L_02000ba0:
	movs	r0, #212
	bl 0x0200949c
	mov	r3, r8
	strh	r3, [r6, #0]
	movs	r0, #3
	bl 0x020093b4
	strh	r7, [r6, #0]
	movs	r0, #65
	subs	r5, #1
	bl 0x020093b4
	cmp	r5, #0
	bge.n	.L_02000ba0
	ldr	r3, [pc, #256]
	movs	r5, #1
	str	r5, [r3, #0]
	ldr	r3, [pc, #252]
	movs	r2, #0
	movs	r1, #200
	str	r2, [r3, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #248]
	mov	r8, r2
	bl 0x02009324
	ldr	r6, [pc, #244]
	movs	r0, #20
	str	r5, [r6, #0]
	bl 0x020093b4
	movs	r0, #163
	bl 0x0200949c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x0200939c
	movs	r0, #60
	bl 0x020093b4
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	str	r5, [r6, #0]
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	lsls	r0, r0, #10
	bl 0x0200939c
	movs	r0, #60
	bl 0x020093b4
	movs	r0, #192
	movs	r1, #192
	movs	r2, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl 0x0200939c
	ldr	r3, [pc, #168]
	mov	r2, r8
	movs	r1, #200
	movs	r6, #160
	movs	r5, #184
	str	r2, [r3, #0]
	ldr	r0, [pc, #160]
	lsls	r1, r1, #4
	lsls	r6, r6, #1
	lsls	r5, r5, #1
	ldr	r7, [pc, #156]
	bl 0x02009324
	add	r6, sl
	movs	r2, #0
	add	r5, sl
.L_02000c44:
	ldr	r3, [r6, #0]
	adds	r3, r3, r7
	str	r3, [r6, #0]
	ldr	r3, [r5, #0]
	adds	r3, r3, r7
	adds	r2, r2, r7
	str	r3, [r5, #0]
	movs	r0, #1
	str	r2, [sp, #0]
	bl 0x0200931c
	ldr	r3, [pc, #128]
	ldr	r2, [sp, #0]
	cmp	r2, r3
	ble.n	.L_02000c44
	ldr	r0, [pc, #112]
	bl 0x0200932c
	ldr	r0, [pc, #116]
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #116]
	ldrh	r1, [r0, #0]
	movs	r6, #0
	ldr	r4, [pc, #56]
	str	r6, [r3, #0]
	adds	r3, r2, #0
	mov	r5, sp
	ands	r3, r1
	orrs	r3, r4
	adds	r5, #6
	strh	r3, [r5, #0]
	strh	r3, [r0, #0]
	subs	r0, #2
	ldrh	r1, [r0, #0]
	adds	r3, r2, #0
	ands	r3, r1
	orrs	r3, r4
	strh	r3, [r5, #0]
	ldr	r1, [pc, #84]
	strh	r3, [r0, #0]
	ldrh	r3, [r1, #0]
	ands	r2, r3
	ldr	r3, [pc, #20]
	orrs	r2, r3
	ldr	r3, [pc, #32]
	movs	r0, #144
	strh	r2, [r5, #0]
	str	r6, [r3, #0]
	strh	r2, [r1, #0]
	lsls	r0, r0, #1
	b.n	.L_02000cec
	.2byte 0x0000
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x03001e70
	.4byte 0x00001010
	.4byte 0x04000052
	.4byte 0x020097e8
	.4byte 0x020097ec
	.4byte 0x02008195
	.4byte 0x020097f8
	.4byte 0x020097f4
	.4byte 0x02008169
	.4byte 0x00003333
	.4byte 0x0059ffff
	.4byte 0x0400000e
	.4byte 0x0000fffc
	.2byte 0x000a
	.2byte 0x0400
.L_02000cec:
	bl 0x0200949c
	movs	r0, #1
	bl 0x0200931c
	movs	r0, #145
	bl 0x0200949c
	movs	r2, #191
	ldr	r3, [pc, #136]
	strh	r2, [r3, #0]
	movs	r5, #0
	ldr	r6, [pc, #132]
.L_02000d06:
	strh	r5, [r6, #0]
	movs	r0, #1
	adds	r5, #1
	bl 0x020093b4
	cmp	r5, #16
	ble.n	.L_02000d06
	movs	r0, #40
	bl 0x020093b4
	movs	r0, #1
	movs	r1, #1
	ldr	r2, [pc, #112]
	negs	r0, r0
	negs	r1, r1
	bl 0x0200939c
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #96]
	str	r3, [r2, #0]
	movs	r3, #184
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #88]
	str	r3, [r2, #0]
	ldr	r2, [pc, #88]
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r6, [pc, #68]
	movs	r5, #16
.L_02000d4a:
	strh	r5, [r6, #0]
	movs	r0, #8
	subs	r5, #1
	bl 0x020093b4
	cmp	r5, #0
	bge.n	.L_02000d4a
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #64]
	bl 0x02009324
	movs	r0, #80
	bl 0x0200949c
	bl 0x02009494
	movs	r0, #20
	bl 0x020093b4
	bl 0x020093c4
	bl 0x02008430
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x04000050
	.4byte 0x04000054
	.4byte 0x0000e666
	.4byte 0x02009804
	.4byte 0x02009808
	.4byte 0x020097fc
	.2byte 0x80b1
	.2byte 0x0200
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #316]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, [pc, #312]
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #4
	ldr r7, [r5]
	cmp r3, #99
	bne .L_02000da4_0
	movs r0, #0
	movs r1, #242
	bl 0x020093a4
.L_02000da4_0:
	ldr r3, [r5, #76]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	movs r0, #8
	bl 0x020093d4
	movs r6, #0
	adds r0, #89
	strb r6, [r0]
	movs r0, #8
	bl 0x020093d4
	movs r5, #2
	adds r0, #35
	strb r5, [r0]
	movs r0, #9
	bl 0x020093d4
	adds r0, #89
	strb r6, [r0]
	movs r0, #9
	bl 0x020093d4
	adds r0, #35
	strb r5, [r0]
	movs r0, #8
	bl 0x020093d4
	ldr r1, [r0, #80]
	subs r5, #15
	ldrb r2, [r1, #9]
	adds r3, r5, #0
	movs r6, #4
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #9]
	movs r0, #9
	bl 0x020093d4
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #9]
	movs r0, #0
	bl 0x020093d4
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #9]
	ldr r1, [r0, #80]
	ldrb r2, [r1, #21]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #21]
	movs r0, #1
	bl 0x020093d4
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #9]
	ldr r1, [r0, #80]
	ldrb r2, [r1, #21]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #21]
	movs r0, #2
	bl 0x020093d4
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #9]
	ldr r1, [r0, #80]
	ldrb r2, [r1, #21]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #21]
	movs r0, #3
	bl 0x020093d4
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r6
	strb r3, [r1, #9]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #21]
	ands r5, r3
	orrs r5, r6
	strb r5, [r2, #21]
	ldr r0, [pc, #80]
	ldr r1, [pc, #80]
	ldrh r2, [r0]
	adds r3, r1, #0
	ands r3, r2
	ldr r2, [pc, #52]
	mov r5, sp
	adds r5, #2
	orrs r3, r2
	strh r3, [r5]
	strh r3, [r0]
	subs r0, #2
	ldrh r2, [r0]
	ldr r4, [pc, #40]
	adds r3, r1, #0
	ands r3, r2
	orrs r3, r4
	strh r3, [r5]
	ldr r2, [pc, #52]
	strh r3, [r0]
	ldrh r3, [r2]
	ands r1, r3
	orrs r1, r4
	strh r1, [r5]
	strh r1, [r2]
	ldr r2, [pc, #40]
	ldr r3, [pc, #44]
	strh r2, [r3]
	movs r2, #129
	lsls r2, r2, #4
	adds r3, #2
	strh r2, [r3]
	b .L_02000da4_1
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x02000240
	.4byte 0x03001e70
	.4byte 0x0400000e
	.4byte 0x0000fffc
	.4byte 0x0400000a
	.4byte 0x00002648
	.4byte 0x04000050
.L_02000da4_1:
	movs r3, #154
	lsls r3, r3, #1
	adds r1, r7, r3
	ldr r3, [r1, #12]
	ldr r2, [pc, #32]
	adds r3, r3, r2
	str r3, [r1, #12]
	movs r3, #178
	lsls r3, r3, #1
	adds r1, r7, r3
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
	bl 0x02009384
	bl 0x0200807c
	movs r0, #0
	sub sp, #-4
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0xffa60000
	.global Func_02000f30
	.thumb_func
Func_02000f30:
	ldr r3, [pc, #52]
	movs r2, #240
	ldr r0, [r3]
	lsls r2, r2, #4
	adds r3, r0, r2
	ldrb r2, [r3]
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #7
	adds r0, r0, r3
	ldr r3, [pc, #36]
	ldr r2, [pc, #40]
	ldrh r4, [r3, #10]
	ands r2, r4
	strh r2, [r3, #10]
	ldr r2, [pc, #36]
	ldrh r4, [r3, #10]
	ands r2, r4
	strh r2, [r3, #10]
	ldr r1, [pc, #32]
	ldrh r2, [r3, #10]
	ldmia r0!, {r2}
	str r2, [r1]
	ldr r2, [pc, #28]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
	.2byte 0x0000
	.4byte 0x03001ed8
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x0400001c
	.4byte 0xa6600001
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #288]
	ldr	r0, [pc, #288]
	ldr	r6, [r3, #0]
	movs	r2, #14
	ldrsh	r1, [r0, r2]
	movs	r2, #240
	lsls	r2, r2, #4
.L_02000f9c:
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r2, #241
	lsls	r3, r3, #7
	lsls	r2, r2, #4
	adds	r5, r6, r3
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	subs	r2, #14
	lsls	r1, r1, #16
	sub	sp, #4
	mov	sl, r3
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	str	r1, [sp, #0]
	lsrs	r3, r1, #16
	ldr	r1, [pc, #240]
	adds	r2, r2, r3
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	adds	r4, r3, #0
	muls	r4, r2
	ldr	r2, [pc, #232]
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	ldrh	r0, [r0, #12]
	mov	lr, r3
	ldr	r3, [pc, #228]
	mov	r9, r0
	movs	r0, #255
	movs	r7, #0
	mov	r8, r3
	mov	fp, r0
	mov	r1, fp
	asrs	r3, r4, #16
	ands	r3, r1
	ldr	r2, [pc, #212]
	lsls	r3, r3, #1
	ldrsh	r0, [r2, r3]
	mov	r1, lr
	mov	ip, pc
	bx	r8
	.4byte 0xda002800
	.4byte 0x020330ff
	.4byte 0x444b0c1b
	.4byte 0x802b3701
	.4byte 0x35044454
	.4byte 0xd1ea2fa0
	.4byte 0x011222f0
	.4byte 0x781b18b3
	.4byte 0x405a2201
	.4byte 0x1a9b0113
	.4byte 0x01db4829
	.4byte 0x1c9d18f3
	.4byte 0x681b1833
	.4byte 0x469a4927
	.4byte 0x881a1873
	.4byte 0x38089b00
	.4byte 0x18330c19
	.4byte 0x1852681b
	.4byte 0x43541c1c
	.4byte 0x18b34a22
	.4byte 0x469e681b
	.4byte 0x20ff4b1c
	.4byte 0x46982700
	.4byte 0x46834689
	.4byte 0x14234659
	.4byte 0x4a19400b
	.4byte 0x5ed0005b
	.4byte 0x00004671
	.4byte 0x474046fc
	.4byte 0xda002800
	.4byte 0x020330ff
	.4byte 0x444b0c1b
	.4byte 0x802b3701
	.4byte 0x35044454
	.4byte 0xd1e92fa0
	.4byte 0x18f24b11
	.4byte 0x20f08813
	.4byte 0x80133301
	.4byte 0x18310100
	.4byte 0x2201780b
	.4byte 0x700b4053
	.4byte 0xbce8b001
	.4byte 0x46a94698
	.4byte 0x46bb46b2
	.4byte 0xbc01bce0
	.4byte 0x00004700
	.4byte 0x03001ed8
	.4byte 0x03001ad0
	.4byte 0x00000f08
	.4byte 0x00000f18
	.4byte 0x03000118
	.4byte 0x020094c8
	.4byte 0x00000f14
	.4byte 0x00000f02
	.2byte 0x0f1c
	.2byte 0x0000
	.global Func_020010d4
	.thumb_func
Func_020010d4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r1, #242
	adds r5, r0, #0
	lsls r1, r1, #4
	movs r0, #34
	sub sp, #4
	mov r8, r2
	adds r7, r3, #0
	bl 0x02009344
	movs r3, #0
	adds r4, r0, #0
	mov r0, sp
	str r3, [r0]
	adds r1, r4, #0
	ldr r3, [pc, #104]
	ldr r2, [pc, #108]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	adds r1, r3, #0
	lsls r2, r2, #24
.L_020010d4_0:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_020010d4_0
	ldr r2, [pc, #92]
	adds r3, r4, r2
	adds r2, #7
	strb r5, [r3]
	adds r3, r4, r2
	str r6, [r3]
	ldr r3, [pc, #84]
	adds r2, r4, r3
	ldr r3, [sp, #24]
	str r3, [r2]
	ldr r2, [pc, #80]
	adds r3, r4, r2
	str r7, [r3]
	ldr r3, [pc, #76]
	adds r2, r4, r3
	ldr r3, [sp, #32]
	str r3, [r2]
	movs r2, #241
	lsls r2, r2, #4
	adds r3, r4, r2
	mov r2, r8
	str r2, [r3]
	ldr r3, [pc, #64]
	adds r2, r4, r3
	ldr r3, [sp, #28]
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, [pc, #56]
	bl 0x02009324
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, [pc, #48]
	bl 0x02009324
	sub sp, #-4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x040000d4
	.4byte 0x850003c8
	.4byte 0x00000f01
	.4byte 0x00000f0c
	.4byte 0x00000f18
	.4byte 0x00000f1c
	.4byte 0x00000f14
	.4byte 0x02008f81
	.4byte 0x02008f31
	.global Func_02001188
	.thumb_func
Func_02001188:
	push {lr}
	ldr r0, [pc, #36]
	bl 0x0200932c
	ldr r0, [pc, #32]
	bl 0x0200932c
	ldr r2, [pc, #32]
	ldr r3, [pc, #32]
	ldrh r1, [r2, #10]
	ands r3, r1
	strh r3, [r2, #10]
	ldr r3, [pc, #28]
	ldrh r1, [r2, #10]
	ands r3, r1
	strh r3, [r2, #10]
.L_020011a8:
	ldrh r3, [r2, #10]
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x8f31
	.2byte 0x0200
	.2byte 0x8f81
	.2byte 0x0200
	.2byte 0x00b0
	.2byte 0x0400
	.2byte 0xc5ff
	.2byte 0x0000
	.2byte 0x7fff
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #208]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #204]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #204]
	ldrh	r3, [r3, #2]
	mov	lr, r2
	lsrs	r3, r3, #5
	mov	r1, lr
	mov	ip, r3
	movs	r4, #0
	ldrsh	r3, [r1, r4]
	ldr	r0, [pc, #192]
	ldrh	r2, [r2, #0]
	cmp	r3, #0
	beq.n	.L_020011f0
	subs	r3, r2, #1
	mov	r2, lr
	strh	r3, [r2, #0]
.L_020011f0:
	movs	r5, #0
.L_020011f2:
	mov	r4, lr
	ldrh	r3, [r4, #0]
	lsls	r4, r3, #16
	asrs	r1, r4, #16
	negs	r3, r1
.L_020011fc:
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r2, #0
	movs	r7, #255
	stmia	r0!, {r2}
	ands	r3, r7
	lsls	r2, r5, #21
	ldr	r6, [pc, #152]
	orrs	r3, r2
	orrs	r3, r6
	stmia	r0!, {r3}
	mov	r2, ip
	adds	r5, #1
	stmia	r0!, {r2}
	cmp	r5, #7
	bls.n	.L_020011f2
	lsrs	r3, r4, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	adds	r2, r3, #0
	adds	r2, #136
	ands	r2, r7
	movs	r5, #0
	movs	r7, #0
	adds	r4, r6, #0
	adds	r1, r0, #0
.L_02001232:
	lsls	r3, r5, #21
	orrs	r3, r2
	orrs	r3, r4
	str	r3, [r1, #4]
	adds	r5, #1
	mov	r3, ip
	str	r7, [r1, #0]
	str	r3, [r1, #8]
	adds	r0, #12
	adds	r1, #12
	cmp	r5, #7
	bls.n	.L_02001232
	ldr	r3, [pc, #84]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r2, #152
	movs	r3, #255
	ldr	r4, [pc, #72]
	movs	r5, #0
	ands	r2, r3
	movs	r6, #0
	adds	r1, r0, #0
.L_02001266:
	lsls	r3, r5, #21
	orrs	r3, r2
	orrs	r3, r4
	str	r3, [r1, #4]
	adds	r5, #1
	mov	r3, ip
	str	r6, [r1, #0]
	str	r3, [r1, #8]
	adds	r1, #12
	cmp	r5, #7
	bls.n	.L_02001266
	ldr	r6, [pc, #36]
	movs	r5, #0
.L_02001280:
	adds	r0, r6, #0
	movs	r1, #255
	adds	r5, #1
	bl 0x02009364
	adds	r6, #12
	cmp	r5, #23
	bls.n	.L_02001280
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x02009c1a
	.4byte 0x03001b10
	.4byte 0x02009c18
	.4byte 0x02009af8
	.2byte 0x4000
	.2byte 0x8000
	.global Func_020012ac
	.thumb_func
Func_020012ac:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #1
	sub sp, #4
	bl 0x0200934c
	ldr r5, [pc, #64]
	adds r6, r0, #0
	bl 0x0200935c
	ldr r3, [pc, #60]
	strh r0, [r5]
	mov r0, sp
	str r3, [r0]
	adds r1, r6, #0
	ldr r3, [pc, #56]
	ldr r2, [pc, #56]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	movs r3, #0
	ldrsh r0, [r5, r3]
	adds r2, r6, #0
	lsls r1, r1, #1
	bl 0x02009354
	ldr r2, [pc, #40]
	ldr r3, [pc, #20]
	movs r1, #200
	strh r3, [r2]
	lsls r1, r1, #4
	ldr r0, [pc, #36]
	bl 0x02009324
	sub sp, #-4
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000030
	.4byte 0x02009c1a
	.4byte 0x11111111
	.4byte 0x040000d4
	.4byte 0x85000040
	.4byte 0x02009c18
	.4byte 0x020091c5
	.include "games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x69c05860
	.4byte 0x69c07f20
	.4byte 0x48005860
	.4byte 0x69c05860
	.4byte 0x69c07f20
	.4byte 0x00005860
	.4byte 0x0000001b
	.4byte 0x00640000
	.4byte 0x012d00c8
	.4byte 0x01f50191
	.4byte 0x02bc0259
	.4byte 0x0381031f
	.4byte 0x044403e3
	.4byte 0x050404a5
	.4byte 0x05c20563
	.4byte 0x067b061f
	.4byte 0x073106d7
	.4byte 0x07e2078a
	.4byte 0x088f0839
	.4byte 0x093608e3
	.4byte 0x09d70987
	.4byte 0x0a730a26
	.4byte 0x0b080abe
	.4byte 0x0b960b50
	.4byte 0x0c1d0bda
	.4byte 0x0c9d0c5e
	.4byte 0x0d140cd9
	.4byte 0x0d840d4d
	.4byte 0x0deb0db9
	.4byte 0x0e4a0e1c
	.4byte 0x0ea00e76
	.4byte 0x0eed0ec8
	.4byte 0x0f310f10
	.4byte 0x0f6b0f4f
	.4byte 0x0f9c0f85
	.4byte 0x0fc30fb1
	.4byte 0x0fe10fd3
	.4byte 0x0ff40fec
	.4byte 0x0ffe0ffb
	.4byte 0x0ffe1000
	.4byte 0x0ff40ffb
	.4byte 0x0fe10fec
	.4byte 0x0fc30fd3
	.4byte 0x0f9c0fb1
	.4byte 0x0f6b0f85
	.4byte 0x0f310f4f
	.4byte 0x0eed0f10
	.4byte 0x0ea00ec8
	.4byte 0x0e4a0e76
	.4byte 0x0deb0e1c
	.4byte 0x0d840db9
	.4byte 0x0d140d4d
	.4byte 0x0c9d0cd9
	.4byte 0x0c1d0c5e
	.4byte 0x0b960bda
	.4byte 0x0b080b50
	.4byte 0x0a730abe
	.4byte 0x09d70a26
	.4byte 0x09360987
	.4byte 0x088f08e3
	.4byte 0x07e20839
	.4byte 0x0731078a
	.4byte 0x067b06d7
	.4byte 0x05c2061f
	.4byte 0x05040563
	.4byte 0x044404a5
	.4byte 0x038103e3
	.4byte 0x02bc031f
	.4byte 0x01f50259
	.4byte 0x012d0191
	.4byte 0x006400c8
	.4byte 0xff9c0000
	.4byte 0xfed3ff38
	.4byte 0xfe0bfe6f
	.4byte 0xfd44fda7
	.4byte 0xfc7ffce1
	.4byte 0xfbbcfc1d
	.4byte 0xfafcfb5b
	.4byte 0xfa3efa9d
	.4byte 0xf985f9e1
	.4byte 0xf8cff929
	.4byte 0xf81ef876
	.4byte 0xf771f7c7
	.4byte 0xf6caf71d
	.4byte 0xf629f679
	.4byte 0xf58df5da
	.4byte 0xf4f8f542
	.4byte 0xf46af4b0
	.4byte 0xf3e3f426
	.4byte 0xf363f3a2
	.4byte 0xf2ecf327
	.4byte 0xf27cf2b3
	.4byte 0xf215f247
	.4byte 0xf1b6f1e4
	.4byte 0xf160f18a
	.4byte 0xf113f138
	.4byte 0xf0cff0f0
	.4byte 0xf095f0b1
	.4byte 0xf064f07b
	.4byte 0xf03df04f
	.4byte 0xf01ff02d
	.4byte 0xf00cf014
	.4byte 0xf002f005
	.4byte 0xf002f000
	.4byte 0xf00cf005
	.4byte 0xf01ff014
	.4byte 0xf03df02d
	.4byte 0xf064f04f
	.4byte 0xf095f07b
	.4byte 0xf0cff0b1
	.4byte 0xf113f0f0
	.4byte 0xf160f138
	.4byte 0xf1b6f18a
	.4byte 0xf215f1e4
	.4byte 0xf27cf247
	.4byte 0xf2ecf2b3
	.4byte 0xf363f327
	.4byte 0xf3e3f3a2
	.4byte 0xf46af426
	.4byte 0xf4f8f4b0
	.4byte 0xf58df542
	.4byte 0xf629f5da
	.4byte 0xf6caf679
	.4byte 0xf771f71d
	.4byte 0xf81ef7c7
	.4byte 0xf8cff876
	.4byte 0xf985f929
	.4byte 0xfa3ef9e1
	.4byte 0xfafcfa9d
	.4byte 0xfbbcfb5b
	.4byte 0xfc7ffc1d
	.4byte 0xfd44fce1
	.4byte 0xfe0bfda7
	.4byte 0xfed3fe6f
	.4byte 0xff9cff38
	.4byte 0xffff0000
	.4byte 0x00000198
	.4byte 0xc00000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc0000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x00000148
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000bc
	.4byte 0x0011f0b4
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01f6
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff01f6
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200804d
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008071
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200807d
	.4byte 0x0000f204
	.4byte 0xffff000a
	.4byte 0x02008b35
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0xffff0000
	.4byte 0xffff0000
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x020083c1
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffd80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x020083dd
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xffec0000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008415
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x020083f9
	.4byte 0x00000010
