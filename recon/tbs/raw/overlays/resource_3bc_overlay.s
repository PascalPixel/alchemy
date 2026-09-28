.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200d000
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
	.4byte 0x0200d090
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200d0a8
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #276]
	movs	r2, #250
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #272]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200ca18
	ldr	r3, [r0, #16]
	asrs	r4, r3, #20
	ldr	r3, [pc, #260]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020000fc
	ldr	r3, [pc, #256]
	ldr	r2, [r3, #0]
	movs	r1, #3
	adds	r2, #1
	ands	r2, r1
	str	r2, [r3, #0]
	ldr	r3, [pc, #248]
	movs	r2, #11
	movs	r6, #18
	mov	r9, r3
	mov	sl, r2
	movs	r7, #33
.L_0200008e:
	ldr	r3, [pc, #232]
	ldr	r2, [r3, #0]
	mov	r8, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, r6, r3
	subs	r3, #18
	mov	r2, r9
	ldrsb	r5, [r2, r3]
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200ca70
	adds	r1, r5, #0
	adds	r0, r6, #5
	adds	r1, #8
	bl 0x0200ca70
	mov	r3, sl
	str	r3, [sp, #4]
	movs	r0, #32
	movs	r1, #11
	movs	r2, #1
	movs	r3, #2
	str	r7, [sp, #0]
	bl 0x0200c940
	cmp	r5, #7
	beq.n	.L_020000dc
	mov	r2, sl
	str	r2, [sp, #4]
	movs	r0, #74
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	bl 0x0200c940
.L_020000dc:
	adds	r6, #1
	adds	r7, #2
	cmp	r6, #22
	ble.n	.L_0200008e
	mov	r3, r8
	ldr	r2, [r3, #0]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #140]
	lsls	r3, r3, #1
	adds	r3, #5
	ldrsb	r1, [r1, r3]
	movs	r0, #28
	bl 0x0200ca70
	b.n	.L_0200014c
.L_020000fc:
	ldr	r3, [pc, #120]
	ldr	r2, [r3, #0]
	movs	r3, #193
	lsls	r3, r3, #1
	adds	r3, r3, r5
	mov	ip, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #108]
	ldr	r2, [pc, #112]
	lsls	r3, r3, #1
	ldr	r7, [pc, #112]
	movs	r6, #18
	adds	r1, r3, r1
	mov	lr, r2
.L_0200011a:
	ldrb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r5, r3, #24
	ldr	r3, [r0, #8]
	lsls	r2, r6, #21
	subs	r3, r3, r2
	add	r3, lr
	adds	r1, #1
	cmp	r3, r7
	bhi.n	.L_02000146
	cmp	r4, #11
	bne.n	.L_0200013a
	cmp	r5, #4
	bne.n	.L_0200013a
	mov	r3, ip
	strh	r5, [r3, #0]
.L_0200013a:
	cmp	r4, #12
	bne.n	.L_02000146
	cmp	r5, #5
	bne.n	.L_02000146
	mov	r2, ip
	strh	r5, [r2, #0]
.L_02000146:
	adds	r6, #1
	cmp	r6, #22
	ble.n	.L_0200011a
.L_0200014c:
	ldr	r2, [pc, #36]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r3, #17
	bls.n	.L_0200015c
	movs	r3, #0
	str	r3, [r2, #0]
.L_0200015c:
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0200d484
	.4byte 0x0200d480
	.4byte 0x0200cc20
	.4byte 0x0031ffff
	.2byte 0xfffe
	.2byte 0x0013
	.global Func_02000188
	.thumb_func
Func_02000188:
	push {r5, lr}
	ldr r3, [pc, #28]
	movs r2, #0
	str r2, [r3]
	ldr r5, [pc, #24]
	ldr r3, [pc, #28]
	adds r0, r5, #0
	str r2, [r3]
	bl 0x0200c850
	bl 0x0200cbc4
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d484
	.4byte 0x0200804d
	.4byte 0x0200d480
	.global Func_020001b4
	.thumb_func
Func_020001b4:
	push {lr}
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #8]
	bl 0x0200c848
	pop {r0}
	bx r0
	.4byte 0x0200804d
	.global Func_020001c8
	.thumb_func
Func_020001c8:
	push {r5, lr}
	movs r0, #10
	bl 0x0200c840
	ldr r3, [pc, #40]
	ldr r3, [r3]
	movs r5, #0
	b .L_020001c8_0
.L_020001c8_2:
	movs r0, #1
	adds r5, #1
	bl 0x0200c840
	cmp r5, #119
	bgt .L_020001c8_1
	ldr r3, [pc, #20]
	ldr r3, [r3]
.L_020001c8_0:
	cmp r3, #3
	bne .L_020001c8_2
	ldr r3, [pc, #16]
	ldr r3, [r3]
	cmp r3, #1
	bne .L_020001c8_2
.L_020001c8_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d480
	.4byte 0x0200d484
	.global Func_02000204
	.thumb_func
Func_02000204:
	push {r5, lr}
	ldr r3, [pc, #56]
	movs r1, #250
	ldr r2, [r3]
	ldr r3, [pc, #52]
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r0, [r3]
	movs r3, #240
	lsls r3, r3, #1
	adds r2, r2, r3
	ldr r5, [r2]
	bl 0x0200ca18
	movs r1, #18
	ldrsh r3, [r0, r1]
	subs r3, #183
	cmp r3, #3
	bhi .L_02000204_0
	ldr r3, [r5, #8]
	ldr r2, [pc, #24]
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r3, [r0, #8]
	adds r3, r3, r2
	str r3, [r0, #8]
.L_02000204_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0xffff3334
	.global Func_0200024c
	.thumb_func
Func_0200024c:
	push {lr}
	movs r0, #216
	lsls r0, r0, #2
	sub sp, #8
	bl 0x0200c9b8
	movs r3, #49
	movs r2, #61
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #61
	movs r2, #1
	movs r3, #4
	bl 0x0200c940
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000274
	.thumb_func
Func_02000274:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl 0x0200ca18
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r0, #11
	bl 0x0200ca18
	ldr r6, [pc, #92]
	ldr r5, [pc, #96]
	movs r2, #128
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	str r6, [r0, #52]
	str r5, [r0, #48]
	lsls r2, r2, #14
	bl 0x0200c910
	movs r0, #10
	bl 0x0200ca18
	movs r2, #128
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	str r6, [r0, #52]
	str r5, [r0, #48]
	lsls r2, r2, #11
	bl 0x0200c910
	ldr r0, [pc, #60]
	bl 0x0200c9b8
	movs r3, #13
	str r3, [sp, #0]
	movs r5, #12
	movs r0, #15
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c940
	movs r3, #9
	str r3, [sp, #0]
	movs r0, #14
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c940
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x00000362
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #236]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [pc, #232]
	sub sp, #8
	ldr r6, [r3]
	bl 0x0200c9b0
	adds r7, r0, #0
	cmp r7, #0
	bne .L_020002f8_0
	movs r0, #10
	bl 0x0200ca18
	cmp r0, #0
	beq .L_020002f8_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r6, #0
	bl 0x0200ca40
.L_020002f8_1:
	adds r0, r6, #0
	bl 0x0200ca60
	movs r0, #11
	bl 0x0200ca18
	adds r3, r0, #0
	ldr r2, [pc, #180]
	adds r3, #85
	strb r7, [r3]
	ldr r3, [pc, #180]
	mov r8, r2
	str r2, [r0, #52]
	movs r2, #128
	ldr r1, [r0, #8]
	mov r10, r3
	str r3, [r0, #48]
	lsls r2, r2, #14
	ldr r3, [r0, #16]
	bl 0x0200c910
	movs r0, #10
	bl 0x0200ca18
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	mov r2, r8
	str r2, [r0, #52]
	mov r3, r10
	movs r2, #128
	ldr r1, [r0, #8]
	str r3, [r0, #48]
	lsls r2, r2, #11
	ldr r3, [r0, #16]
	bl 0x0200c910
	adds r0, r6, #0
	bl 0x0200ca18
	movs r2, #85
	adds r5, r0, #0
	adds r2, r2, r5
	strb r7, [r2]
	mov r9, r2
	mov r2, r10
	mov r3, r8
	str r2, [r5, #48]
	movs r2, #128
	lsls r2, r2, #11
	str r3, [r5, #52]
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	bl 0x0200c910
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200c958
	adds r0, r6, #0
	bl 0x0200ca60
	movs r3, #9
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r1, #24
	movs r0, #0
	bl 0x0200c940
	movs r0, #2
	bl 0x0200c840
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c958
	movs r3, #3
	mov r2, r9
	strb r3, [r2]
	ldr r3, [r5, #12]
	ldr r0, [pc, #36]
	str r3, [r5, #20]
	bl 0x0200c9b8
.L_020002f8_0:
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00000362
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x00000367
	.global Func_02000404
	.thumb_func
Func_02000404:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, [pc, #136]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	sub sp, #8
	bl 0x0200ca70
	movs r0, #11
	bl 0x0200ca18
	movs r2, #0
	adds r3, r0, #0
	mov r8, r2
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	ldr r6, [pc, #104]
	ldr r5, [pc, #104]
	movs r2, #128
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	str r6, [r0, #52]
	str r5, [r0, #48]
	lsls r2, r2, #11
	bl 0x0200c910
	movs r0, #10
	bl 0x0200ca18
	adds r3, r0, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r2, #128
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	str r6, [r0, #52]
	str r5, [r0, #48]
	lsls r2, r2, #14
	bl 0x0200c910
	movs r0, #10
	bl 0x0200ca60
	movs r3, #9
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #25
	movs r2, #1
	movs r3, #1
	movs r0, #0
	bl 0x0200c940
	movs r0, #2
	bl 0x0200c840
	ldr r0, [pc, #28]
	bl 0x0200c9c0
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x00000367
	.global Func_020004a4
	.thumb_func
Func_020004a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #12
	sub sp, #8
	bl 0x0200ca18
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	mov r9, r3
	cmp r3, #9
	bne .L_020004a4_0
	ldr r3, [r0, #16]
	asrs r7, r3, #20
	cmp r7, #12
	bne .L_020004a4_0
	movs r0, #12
	bl 0x0200ca18
	movs r1, #0
	adds r5, r0, #0
	bl 0x0200c958
	movs r1, #2
	mov r10, r1
	adds r2, r5, #0
	adds r2, #35
	mov r1, r10
	strb r1, [r2]
	movs r3, #0
	adds r2, #50
	strb r3, [r2]
	ldr r2, [pc, #136]
	ldr r6, [pc, #140]
	mov r8, r2
	str r2, [r5, #52]
	movs r2, #128
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	lsls r2, r2, #11
	adds r0, r5, #0
	str r6, [r5, #48]
	bl 0x0200c910
	movs r0, #11
	bl 0x0200ca18
	adds r3, r0, #0
	adds r3, #35
	mov r1, r10
	strb r1, [r3]
	mov r2, r8
	str r2, [r0, #52]
	movs r2, #128
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	lsls r2, r2, #14
	str r6, [r0, #48]
	bl 0x0200c910
	movs r0, #10
	bl 0x0200ca18
	mov r3, r8
	movs r2, #128
	ldr r1, [r0, #8]
	str r3, [r0, #52]
	lsls r2, r2, #11
	ldr r3, [r0, #16]
	str r6, [r0, #48]
	bl 0x0200c910
	movs r0, #218
	lsls r0, r0, #2
	bl 0x0200c9b8
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #15
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl 0x0200c940
	mov r1, r9
	str r1, [sp, #0]
	movs r0, #1
	movs r1, #25
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl 0x0200c940
.L_020004a4_0:
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00006666
	.4byte 0x0000cccc
	.global Func_0200057c
	.thumb_func
Func_0200057c:
	push {lr}
	bl 0x0200c628
	bl 0x020084a4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200058c
	.thumb_func
Func_0200058c:
	push {r5, r6, lr}
	movs r0, #13
	sub sp, #8
	bl 0x0200ca18
	ldr r5, [r0, #8]
	movs r0, #220
	asrs r5, r5, #20
	adds r1, r5, #0
	lsls r0, r0, #2
	bl 0x0200c9d0
	movs r3, #18
	str r3, [sp, #0]
	movs r6, #11
	movs r0, #18
	movs r1, #10
	movs r2, #3
	movs r3, #1
	str r6, [sp, #4]
	bl 0x0200c940
	movs r0, #17
	movs r1, #11
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c940
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020005d0
	.thumb_func
Func_020005d0:
	bx lr
	.2byte 0x0000
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	push {lr}
	bl 0x0200c628
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020005e0
	.thumb_func
Func_020005e0:
	push {r5, r6, lr}
	sub sp, #8
	movs r6, #15
.L_020005e0_1:
	adds r0, r6, #0
	bl 0x0200ca18
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl 0x0200c930
	cmp r0, #0
	bne .L_020005e0_0
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	ldr r2, [r5, #8]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #83
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200c940
	ldr r3, [r5, #16]
	ldr r2, [r5, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	adds r3, #52
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #83
	movs r3, #1
	movs r1, #13
	movs r2, #1
	bl 0x0200c940
	ldr r3, [pc, #20]
	adds r0, r6, r3
	bl 0x0200c9b8
.L_020005e0_0:
	adds r6, #1
	cmp r6, #17
	ble .L_020005e0_1
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000205
	.global Func_02000658
	.thumb_func
Func_02000658:
	push {r5, lr}
	ldr r3, [pc, #68]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl 0x0200ca18
	ldr r3, [r0, #8]
	asrs r1, r3, #20
	ldr r3, [r0, #16]
	movs r5, #23
	asrs r4, r3, #20
	cmp r1, #81
	bne .L_02000658_0
	cmp r4, #12
	bne .L_02000658_0
	ldrh r2, [r0, #6]
	movs r3, #224
	lsls r3, r3, #8
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #7
	cmp r3, r2
	bne .L_02000658_1
	movs r5, #253
.L_02000658_1:
	lsls r1, r1, #20
	lsls r2, r4, #20
	movs r0, #0
	adds r3, r5, #0
	bl 0x0200c968
.L_02000658_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.global Func_020006a4
	.thumb_func
Func_020006a4:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	movs r0, #0
	adds r1, r5, #0
	adds r2, r6, #0
	bl 0x0200c938
	cmp r0, #255
	bne .L_020006a4_0
	movs r0, #2
	negs r0, r0
	b .L_020006a4_1
.L_020006a4_0:
	movs r0, #15
	bl 0x0200ca18
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	asrs r3, r3, #20
	asrs r6, r6, #20
	cmp r3, r5
	bne .L_020006a4_2
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, r6
	beq .L_020006a4_3
.L_020006a4_2:
	movs r0, #16
	bl 0x0200ca18
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, r5
	bne .L_020006a4_4
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, r6
	beq .L_020006a4_3
.L_020006a4_4:
	movs r0, #17
	bl 0x0200ca18
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, r5
	bne .L_020006a4_5
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, r6
	bne .L_020006a4_5
.L_020006a4_3:
	movs r0, #1
	negs r0, r0
	b .L_020006a4_1
.L_020006a4_5:
	movs r0, #0
.L_020006a4_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000714
	.thumb_func
Func_02000714:
	push {r5, r6, lr}
	ldr r3, [pc, #76]
	adds r5, r1, #0
	adds r1, r5, r3
	adds r6, r0, #0
	bl 0x020086a4
	cmp r0, #0
	bne .L_02000714_0
	ldr r3, [pc, #64]
	adds r0, r6, #0
	adds r1, r5, r3
	bl 0x020086a4
	cmp r0, #0
	bne .L_02000714_0
	movs r3, #128
	lsls r3, r3, #12
	adds r1, r5, r3
	adds r0, r6, #0
	bl 0x020086a4
	cmp r0, #0
	bne .L_02000714_0
	movs r3, #192
	lsls r3, r3, #13
	adds r1, r5, r3
	adds r0, r6, #0
	bl 0x020086a4
	cmp r0, #0
	beq .L_02000714_1
.L_02000714_0:
	movs r0, #1
	negs r0, r0
	b .L_02000714_2
.L_02000714_1:
	movs r0, #0
.L_02000714_2:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xffe80000
	.4byte 0xfff80000
	.global Func_0200076c
	.thumb_func
Func_0200076c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, [pc, #648]
	movs r0, #250
	lsls r0, r0, #1
	adds r5, r5, r0
	ldr r0, [r5]
	sub sp, #32
	bl 0x0200ca18
	mov r10, r0
	movs r0, #31
	bl 0x0200ca18
	mov r8, r0
	mov r0, r10
	ldrh r3, [r0, #6]
	ldr r2, [pc, #620]
	lsrs r3, r3, #13
	movs r1, #0
	lsls r3, r3, #1
	mov r9, r1
	ldrh r1, [r2, r3]
	ldr r3, [r0, #8]
	ldr r2, [pc, #612]
	ldr r5, [r5]
	movs r4, #128
	lsls r4, r4, #12
	ands r3, r2
	adds r3, r3, r4
	mov r11, r5
	add r5, sp, #20
	str r3, [r5]
	ldr r3, [r0, #12]
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	ands r3, r2
	adds r3, r3, r4
	movs r0, #128
	str r3, [r5, #8]
	adds r2, r5, #0
	lsls r0, r0, #13
	str r4, [sp, #8]
	bl 0x0200c868
	mov r1, r8
	ldr r7, [r1, #8]
	ldr r2, [r5]
	subs r3, r2, r7
	ldr r6, [r1, #16]
	ldr r4, [sp, #8]
	cmp r3, #0
	bge .L_0200076c_0
	subs r3, r7, r2
.L_0200076c_0:
	cmp r3, r4
	ble .L_0200076c_1
	b .L_0200076c_2
.L_0200076c_1:
	ldr r2, [r5, #8]
	subs r3, r2, r6
	cmp r3, #0
	blt .L_0200076c_3
	movs r2, #128
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_0200076c_4
	b .L_0200076c_2
.L_0200076c_4:
	b .L_0200076c_5
.L_0200076c_3:
	movs r0, #128
	subs r3, r6, r2
	lsls r0, r0, #14
	cmp r3, r0
	ble .L_0200076c_5
	b .L_0200076c_2
.L_0200076c_5:
	ldr r1, [pc, #516]
	ldr r3, [r1]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0200076c_6
	movs r1, #2
	subs r2, #40
	str r1, [sp, #16]
	str r2, [sp, #12]
.L_0200076c_8:
	ldr r3, [pc, #492]
	adds r5, r7, r3
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x02008714
	cmp r0, #0
	bne .L_0200076c_7
	movs r0, #1
	add r9, r0
	adds r7, r5, #0
	b .L_0200076c_8
.L_0200076c_6:
	ldr r3, [r1]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	bne .L_0200076c_9
	b .L_0200076c_10
.L_0200076c_9:
	movs r1, #3
	movs r2, #8
	str r1, [sp, #16]
	str r2, [sp, #12]
.L_0200076c_11:
	movs r3, #128
	lsls r3, r3, #13
	adds r5, r7, r3
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x02008714
	cmp r0, #0
	bne .L_0200076c_7
	movs r0, #1
	add r9, r0
	adds r7, r5, #0
	b .L_0200076c_11
.L_0200076c_7:
	mov r1, r9
	cmp r1, #0
	bne .L_0200076c_12
	b .L_0200076c_10
.L_0200076c_12:
	mov r2, r8
	ldr r3, [r2, #8]
	asrs r3, r3, #20
	str r3, [sp, #0]
	movs r3, #9
	str r3, [sp, #4]
	movs r0, #74
	movs r1, #8
	movs r2, #1
	movs r3, #4
	bl 0x0200c940
	movs r3, #74
	movs r2, #60
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #5
	movs r2, #8
	movs r0, #120
	movs r1, #60
	bl 0x0200c940
	bl 0x0200ca00
	movs r1, #8
	mov r0, r11
	bl 0x0200ca70
	movs r0, #6
	bl 0x0200c9f8
	ldr r5, [pc, #360]
	movs r0, #128
	mov r1, r8
	lsls r0, r0, #8
	str r0, [r1, #48]
	str r5, [r1, #52]
	ldr r1, [sp, #16]
	mov r0, r8
	bl 0x0200c8e0
	adds r3, r6, #0
	movs r2, #0
	adds r1, r7, #0
	mov r0, r8
	bl 0x0200c910
	movs r0, #6
	bl 0x0200c9f8
	mov r0, r11
	movs r1, #2
	bl 0x0200ca70
	ldr r1, [pc, #320]
	movs r0, #27
	bl 0x0200c878
	movs r2, #240
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r0, [r0]
	mov r1, r8
	bl 0x0200c8f8
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #8
	mov r0, r11
	bl 0x0200ca28
	movs r0, #239
	bl 0x0200cba0
	mov r0, r10
	movs r1, #2
	bl 0x0200c8e0
	ldr r3, [sp, #12]
	mov r1, r9
	muls r1, r3
	mov r0, r10
	ldr r3, [r0, #8]
	lsls r1, r1, #16
	adds r1, r1, r3
	movs r2, #0
	ldr r3, [r0, #16]
	bl 0x0200c910
	mov r0, r10
	bl 0x0200c918
	movs r1, #1
	mov r0, r10
	bl 0x0200c8e0
	mov r0, r8
	bl 0x0200c918
	movs r1, #166
	lsls r1, r1, #19
	cmp r7, r1
	blt .L_0200076c_13
	ldr r0, [pc, #224]
	bl 0x0200c9b8
	movs r0, #31
	movs r1, #3
	bl 0x0200ca70
	movs r2, #6
	movs r1, #18
	movs r0, #31
	bl 0x0200ca58
	movs r0, #30
	bl 0x0200c9f8
	movs r1, #8
	mov r0, r8
	bl 0x0200c8e0
	mov r0, r8
	bl 0x0200c918
	mov r2, r8
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r3, #10
	str r3, [sp, #4]
	movs r5, #84
	movs r0, #86
	movs r1, #10
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x0200c940
	movs r3, #12
	str r3, [sp, #4]
	movs r0, #86
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200c940
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200cba0
	movs r0, #240
	bl 0x0200cba0
	b .L_0200076c_14
.L_0200076c_13:
	movs r1, #1
	mov r0, r8
	bl 0x0200c8e0
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200cba0
	movs r0, #213
	bl 0x0200cba0
	movs r2, #9
	asrs r5, r7, #20
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #9
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	bl 0x0200c940
	movs r3, #61
	str r3, [sp, #4]
	movs r0, #85
	movs r1, #9
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	bl 0x0200c940
.L_0200076c_14:
	movs r0, #15
	bl 0x0200c9f8
	bl 0x0200ca08
	b .L_0200076c_10
.L_0200076c_2:
	bl 0x0200c628
	bl 0x020085e0
.L_0200076c_10:
	sub sp, #-32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0200cc38
	.4byte 0xfff00000
	.4byte 0x03001ae8
	.4byte 0x00003333
	.4byte 0x00000ccc
	.4byte 0x00000369
	.global Func_02000a20
	.thumb_func
Func_02000a20:
	push {lr}
	movs r0, #1
	bl 0x0200c928
.L_02000a28:
	movs r0, #2
	bl 0x0200c920
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200cba0
	movs r0, #217
	bl 0x0200cba0
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000a44
	.thumb_func
Func_02000a44:
	push {r5, lr}
	movs r0, #30
	bl 0x0200ca18
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #32]
	movs r1, #2
	str r3, [r5, #52]
	str r3, [r5, #48]
	bl 0x0200c8e0
	adds r0, r5, #0
	ldr r1, [pc, #20]
	bl 0x0200c8e8
	ldr r0, [pc, #20]
	bl 0x0200c9b8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x0200cc48
	.4byte 0x00000363
	.global Func_02000a84
	.thumb_func
Func_02000a84:
	push {r5, lr}
	movs r0, #28
	bl 0x0200cb90
	ldr r0, [pc, #56]
	bl 0x0200c9b8
	movs r0, #10
	bl 0x0200c840
	ldr r2, [pc, #48]
	ldr r3, [r2]
	cmp r3, #1
	beq .L_02000a84_0
	cmp r3, #3
	beq .L_02000a84_0
	adds r5, r2, #0
.L_02000a84_1:
	movs r0, #1
	bl 0x0200c840
	ldr r3, [r5]
	cmp r3, #1
	beq .L_02000a84_0
	cmp r3, #3
	bne .L_02000a84_1
.L_02000a84_0:
	movs r0, #1
	bl 0x0200c840
	ldr r0, [pc, #16]
	bl 0x0200c850
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000361
	.4byte 0x0200d480
	.4byte 0x0200804d
	.global Func_02000ad4
	.thumb_func
Func_02000ad4:
	push {r5, r6, lr}
	ldr r6, [pc, #80]
	movs r0, #250
	lsls r0, r0, #1
	adds r6, r6, r0
	ldr r0, [r6]
	bl 0x0200ca18
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #9
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl 0x0200cae8
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200c8e0
	ldr r3, [r5, #16]
	ldr r2, [pc, #36]
	movs r0, #192
	ands r3, r2
	lsls r0, r0, #13
	adds r3, r3, r0
	ldr r1, [r5, #8]
	adds r0, r5, #0
	ldr r2, [r5, #12]
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff00000
	.global Func_02000b30
	.thumb_func
Func_02000b30:
	push {r5, r6, lr}
	ldr r3, [pc, #100]
	movs r0, #250
	lsls r0, r0, #1
	adds r6, r3, r0
	ldr r0, [r6]
	bl 0x0200ca18
	adds r5, r0, #0
	movs r2, #166
	ldr r3, [r5, #8]
	lsls r2, r2, #18
	cmp r3, r2
	ble .L_02000b30_0
	str r2, [r5, #8]
.L_02000b30_0:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200c8e0
	ldr r3, [r5, #16]
	ldr r2, [pc, #52]
	movs r0, #192
	ands r3, r2
	lsls r0, r0, #12
	ldr r2, [r5, #12]
	adds r3, r3, r0
	ldr r1, [r5, #8]
	adds r0, r5, #0
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl 0x0200cae8
	ldr r0, [r6]
	movs r1, #6
	movs r2, #0
	bl 0x0200ca90
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0xfff00000
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
	bx lr
	.2byte 0x0000
	.global Func_02000ba4
	.thumb_func
Func_02000ba4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #160]
	ldr r2, [pc, #164]
	ldr r3, [r3]
	movs r1, #250
	lsls r1, r1, #1
	mov r10, r3
	adds r3, r2, r1
	ldr r3, [r3]
	subs r1, #50
	mov r8, r3
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r6, r0, #0
	cmp r3, #2
	bne .L_02000ba4_0
	bl 0x0200ca00
	lsls r3, r6, #1
	ldr r7, [pc, #132]
	adds r5, r3, r6
	adds r0, r5, r7
	bl 0x0200cab0
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200cab8
	mov r0, r8
	movs r1, #0
	bl 0x0200ca10
	cmp r0, #0
	bne .L_02000ba4_1
	adds r0, r7, #1
	adds r0, r5, r0
	bl 0x0200cab0
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200cac0
	movs r2, #224
	lsls r2, r2, #1
	movs r3, #128
	add r2, r10
	lsls r3, r3, #2
	str r3, [r2]
	movs r2, #228
	lsls r2, r2, #1
	add r2, r10
	movs r3, #15
	str r3, [r2]
	bl 0x0200cb58
	bl 0x0200cb60
	adds r0, r6, #0
	bl 0x0200a640
	bl 0x0200cb50
	bl 0x0200cb60
	b .L_02000ba4_2
.L_02000ba4_1:
	adds r0, r7, #2
	adds r0, r5, r0
	bl 0x0200cab0
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200cac0
.L_02000ba4_2:
	bl 0x0200ca08
.L_02000ba4_0:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00002073
	.global Func_02000c5c
	.thumb_func
Func_02000c5c:
	push {r5, r6, lr}
	bl 0x0200bcc0
	bl 0x0200ca00
	movs r1, #17
	movs r0, #3
	bl 0x0200bddc
	adds r6, r0, #0
	bl 0x0200bcd0
	movs r5, #9
.L_02000c5c_0:
	movs r0, #8
	subs r5, #1
	bl 0x0200ca38
	cmp r5, #0
	bge .L_02000c5c_0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #191
	movs r0, #8
	lsls r1, r1, #3
	movs r2, #192
	bl 0x0200ca48
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #187
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #3
	bl 0x0200ca50
	movs r0, #8
	movs r1, #1
	bl 0x0200ca70
	movs r2, #0
	movs r1, #8
	movs r0, #0
	bl 0x0200caa0
	movs r0, #10
	bl 0x0200c9f8
	movs r0, #8
	movs r1, #3
	bl 0x0200ca70
	movs r1, #3
	movs r0, #0
	bl 0x0200ca78
	movs r0, #20
	bl 0x0200c9f8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200ca28
	movs r1, #188
	movs r0, #0
	lsls r1, r1, #3
	movs r2, #192
	bl 0x0200ca48
	movs r1, #190
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #3
	bl 0x0200ca50
	movs r0, #0
	movs r1, #16
	bl 0x0200ca70
	movs r1, #9
	movs r0, #8
	bl 0x0200ca70
	movs r0, #10
	bl 0x0200c9f8
	movs r1, #4
	subs r1, r1, r6
	adds r1, #1
	movs r0, #72
	bl 0x0200cb18
	ldr r3, [pc, #40]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r5, [pc, #36]
	movs r1, #4
	adds r0, r5, #0
	bl 0x0200cb20
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200cb28
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200c9b8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x00000091
	.global Func_02000d70
	.thumb_func
Func_02000d70:
	push {lr}
	bl 0x0200c790
	cmp r0, #0
	bne .L_02000d70_0
	bl 0x0200cb30
.L_02000d70_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000d84
	.thumb_func
Func_02000d84:
	push {lr}
	bl 0x0200c790
	cmp r0, #0
	bne .L_02000d84_0
	bl 0x0200cb30
	b .L_02000d84_1
.L_02000d84_0:
	bl 0x0200857c
.L_02000d84_1:
	pop {r0}
	bx r0
	.global Func_02000d9c
	.thumb_func
Func_02000d9c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200d488
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #916]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #162
	subs r2, #192
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #12
	bl 0x0200c9b8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #892]
	bl 0x0200c848
	movs r3, #120
.L_02000dd2:
	movs r2, #60
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #6
	movs r2, #8
	movs r1, #60
	movs r0, #74
	bl 0x0200c940
	movs r0, #9
	bl 0x0200ca18
	movs r1, #0
	bl 0x0200c958
	movs r0, #10
.L_02000df2:
	bl 0x0200ca18
	movs r1, #0
	adds r7, r0, #0
	movs r6, #128
	bl 0x0200c958
	adds r3, r7, #0
	adds r3, #85
	movs r5, #0
	lsls r6, r6, #14
	strb r5, [r3]
	movs r0, #11
	str r6, [r7, #12]
	bl 0x0200ca18
	movs r1, #0
	adds r7, r0, #0
	bl 0x0200c958
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r5, #128
	lsls r5, r5, #11
	str r5, [r7, #12]
	ldr r0, [pc, #804]
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02000df2_0
	movs r1, #5
	movs r0, #9
	bl 0x0200ca70
	movs r0, #10
	bl 0x0200ca18
	adds r7, r0, #0
	str r5, [r7, #12]
	movs r0, #11
	bl 0x0200ca18
	movs r3, #13
	adds r7, r0, #0
	movs r2, #12
	str r6, [r7, #12]
	movs r0, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #12
	b .L_02000df2_1
.L_02000df2_0:
	movs r0, #9
	bl 0x0200ca18
	movs r3, #192
	adds r7, r0, #0
	lsls r3, r3, #9
	str r3, [r7, #24]
	str r3, [r7, #28]
	ldr r0, [pc, #740]
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02000df2_2
	movs r3, #9
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #24
.L_02000df2_1:
	movs r2, #1
	movs r3, #1
	bl 0x0200c940
	b .L_02000df2_3
.L_02000df2_2:
	movs r3, #9
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #25
	movs r2, #1
	movs r3, #1
	bl 0x0200c940
.L_02000df2_3:
	movs r0, #218
	lsls r0, r0, #2
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02000df2_4
	movs r3, #13
	str r3, [sp, #0]
	movs r5, #12
	movs r0, #15
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c940
	movs r3, #9
	str r3, [sp, #0]
	movs r2, #1
	movs r3, #1
	movs r1, #25
	movs r0, #1
	str r5, [sp, #4]
	bl 0x0200c940
	movs r0, #12
	bl 0x0200ca18
	movs r1, #0
	adds r7, r0, #0
	bl 0x0200c958
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #12]
	adds r3, r7, #0
	adds r3, #35
	movs r5, #2
	strb r5, [r3]
	movs r0, #10
	bl 0x0200ca18
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #11
	str r3, [r7, #12]
	adds r3, r7, #0
	adds r3, #35
	strb r5, [r3]
	movs r0, #11
	bl 0x0200ca18
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #14
	str r3, [r7, #12]
.L_02000df2_4:
	movs r0, #220
	lsls r0, r0, #2
	bl 0x0200c9c8
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000df2_5
	movs r6, #19
.L_02000df2_5:
	movs r0, #13
	bl 0x0200ca18
	movs r2, #128
	adds r7, r0, #0
	lsls r2, r2, #12
	lsls r3, r6, #20
	adds r3, r3, r2
	adds r2, r7, #0
	str r3, [r7, #8]
.L_02000f3a:
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	subs r2, #50
	movs r3, #2
	strb r3, [r2]
	movs r3, #18
	movs r5, #11
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #10
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200c940
	movs r0, #17
	movs r1, #11
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl 0x0200c940
	movs r5, #15
.L_02000f3a_1:
	adds r0, r5, #0
	bl 0x0200ca18
	adds r7, r0, #0
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #0
	bl 0x0200c930
	ldr r3, [r7, #12]
	cmp r3, #0
	bne .L_02000f3a_0
	cmp r0, #0
	bne .L_02000f3a_0
	adds r2, r7, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	adds r3, r7, #0
	adds r3, #85
	strb r0, [r3]
	ldr r2, [r7, #8]
	ldr r3, [r7, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #83
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200c940
	ldr r3, [r7, #16]
	ldr r2, [r7, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	adds r3, #52
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #83
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200c940
.L_02000f3a_0:
	adds r5, #1
	cmp r5, #17
	ble .L_02000f3a_1
	ldr r0, [pc, #388]
.L_02000fd0:
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02000fd0_0
	movs r3, #0
	mov r9, r3
	movs r2, #2
	movs r3, #11
	movs r5, #18
	mov r8, r2
	movs r6, #33
	mov r10, r3
.L_02000fd0_1:
	adds r0, r5, #0
	bl 0x0200ca18
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #35
	mov r2, r8
	strb r2, [r3]
	movs r1, #2
	bl 0x0200c8e0
	adds r0, r5, #5
	bl 0x0200ca18
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #35
	mov r2, r8
	strb r2, [r3]
	mov r2, r9
	adds r3, #50
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r7, #12]
	movs r1, #10
	bl 0x0200c8e0
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #74
	movs r1, #12
	movs r2, #1
	movs r3, #1
	adds r5, #1
	str r6, [sp, #0]
	bl 0x0200c940
	adds r6, #2
	cmp r5, #22
	ble .L_02000fd0_1
	movs r0, #28
	movs r1, #10
	bl 0x0200ca70
	movs r0, #28
	bl 0x0200cb90
	b .L_02000fd0_2
.L_02000fd0_0:
	movs r2, #0
	movs r5, #18
	mov r8, r2
	movs r6, #2
.L_02000fd0_3:
	adds r0, r5, #0
	bl 0x0200ca18
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #35
	strb r6, [r3]
	adds r0, r5, #5
	bl 0x0200ca18
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #35
	strb r6, [r3]
	mov r2, r8
	adds r3, #50
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #14
	adds r5, #1
	str r3, [r7, #12]
	cmp r5, #22
	ble .L_02000fd0_3
	movs r1, #200
	ldr r0, [pc, #212]
	lsls r1, r1, #4
	bl 0x0200c848
.L_02000fd0_2:
	movs r0, #216
	lsls r0, r0, #2
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02000fd0_4
	movs r0, #29
	movs r1, #4
	bl 0x0200ca70
	movs r3, #49
	movs r2, #61
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #61
	movs r2, #1
	movs r3, #4
	bl 0x0200c940
.L_02000fd0_4:
	ldr r0, [pc, #168]
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02000fd0_5
	movs r0, #1
	bl 0x0200c928
	movs r0, #30
	bl 0x0200ca18
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #140]
	str r3, [r7, #8]
	movs r3, #184
	lsls r3, r3, #16
	str r3, [r7, #16]
	movs r1, #0
	bl 0x0200c958
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200c8e0
	ldr r1, [pc, #120]
	adds r0, r7, #0
	bl 0x0200c8e8
	b .L_02000fd0_6
.L_02000fd0_5:
	movs r0, #2
	bl 0x0200c928
.L_02000fd0_6:
	ldr r0, [pc, #108]
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02000fd0_7
	movs r0, #31
	bl 0x0200ca18
	movs r1, #8
	adds r7, r0, #0
	bl 0x0200c8e0
	adds r2, r7, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r3, #10
	movs r5, #84
	str r3, [sp, #4]
	movs r0, #86
	movs r1, #10
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl 0x0200c940
	movs r3, #12
	str r3, [sp, #4]
	movs r0, #86
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200c940
	b .L_02000fd0_8
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x8659
	.2byte 0x0200
	.2byte 0x0362
	.2byte 0x0000
	.2byte 0x0367
	.2byte 0x0000
	.2byte 0x0361
	.2byte 0x0000
	.4byte 0x0200804d
	.4byte 0x00000363
	.4byte 0x046a0000
	.4byte 0x0200cbec
	.4byte 0x00000369
.L_02000fd0_7:
	movs r0, #31
	bl 0x0200ca18
	adds r7, r0, #0
	ldr r3, [r7, #8]
	movs r2, #9
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #9
	movs r2, #1
	movs r3, #4
	bl 0x0200c940
	ldr r3, [r7, #8]
	movs r2, #61
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #9
	movs r2, #1
	movs r3, #4
	bl 0x0200c940
.L_02000fd0_8:
	movs r0, #9
	bl 0x0200ca18
	adds r7, r0, #0
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	movs r5, #2
	strb r6, [r3]
	subs r3, #50
	strb r5, [r3]
	movs r0, #10
	bl 0x0200ca18
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	subs r3, #50
	strb r5, [r3]
	movs r0, #11
	bl 0x0200ca18
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	subs r3, #50
	strb r5, [r3]
	movs r0, #8
	movs r1, #9
	bl 0x0200ca70
	ldr r5, [pc, #384]
	movs r2, #249
	lsls r2, r2, #1
	adds r3, r5, r2
	strb r6, [r3]
	movs r1, #3
	movs r0, #39
	bl 0x0200bbd0
	movs r1, #17
	movs r0, #40
	bl 0x0200bbd0
	movs r0, #8
	movs r1, #2
	bl 0x0200caa8
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #4
	bls .L_02000fd0_9
	b .L_02000fd0_10
.L_02000fd0_9:
	ldr r2, [pc, #336]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	str r2, [sp, #208]
	lsls r0, r0, #8
	str r2, [sp, #856]
	lsls r0, r0, #8
	str r3, [sp, #32]
	lsls r0, r0, #8
	str r3, [sp, #120]
	lsls r0, r0, #8
	str r3, [sp, #232]
	lsls r0, r0, #8
	movs r2, #192
	lsls r2, r2, #16
	str r2, [sp, #0]
	movs r2, #39
	str r2, [sp, #4]
	movs r3, #189
	movs r2, #40
	str r2, [sp, #8]
	lsls r3, r3, #19
	movs r1, #8
	movs r2, #6
	movs r0, #0
	bl 0x0200c494
	movs r3, #5
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #2
	movs r0, #127
	bl 0x0200c948
	movs r0, #32
	bl 0x0200ca20
	movs r0, #33
	bl 0x0200ca20
	movs r0, #34
	bl 0x0200ca20
	movs r0, #35
	bl 0x0200ca20
	movs r0, #36
	bl 0x0200ca20
	movs r0, #37
	bl 0x0200ca20
	movs r0, #38
	bl 0x0200ca20
	ldr r0, [pc, #220]
	bl 0x0200c9b0
	cmp r0, #0
	bne .L_02000fd0_11
	movs r0, #17
	bl 0x0200cba0
	movs r0, #0
	bl 0x0200a640
	bl 0x0200a5c8
	movs r0, #1
	movs r1, #0
	bl 0x0200cb88
	movs r0, #3
	bl 0x0200b468
.L_02000fd0_11:
	movs r0, #1
	movs r1, #0
	bl 0x0200cb88
	movs r0, #2
	movs r1, #0
	bl 0x0200cb88
	movs r0, #3
	movs r1, #0
	bl 0x0200cb88
	ldr r0, [pc, #160]
	bl 0x0200c57c
	b .L_02000fd0_10
	.2byte 0x21c8
	.2byte 0x0109
	.2byte 0x4826
	.2byte 0xf003
	.2byte 0xfab4
	.2byte 0x2027
	.2byte 0xf003
	.2byte 0xfb9d
	.2byte 0x2028
	.2byte 0xf003
	.2byte 0xfb9a
	.2byte 0x481f
	.2byte 0xf003
	.2byte 0xfb5f
	.2byte 0x2800
	.2byte 0xd12c
	.2byte 0xf001
	.2byte 0xf967
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xf9a0
	.2byte 0x2000
	.2byte 0xf002
	.2byte 0xf8b1
	.2byte 0xe023
	.2byte 0x4818
	.2byte 0xf003
	.2byte 0xfb51
	.2byte 0x2800
	.2byte 0xd11e
	.2byte 0x2020
	.2byte 0xf000
	.2byte 0xfb7a
	.2byte 0xf001
	.2byte 0xfb9a
	.2byte 0xe018
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xf8a8
	.2byte 0x2004
	.2byte 0xf003
	.2byte 0xfbf3
	.2byte 0x2095
	.2byte 0x0100
	.2byte 0xf003
	.2byte 0xfb43
	.2byte 0x4811
	.2byte 0xf003
	.2byte 0xfb40
	.2byte 0xe00a
	.2byte 0x2001
	.2byte 0x4240
	.2byte 0xf000
	.2byte 0xf899
	.2byte 0x2005
	.2byte 0xf003
	.2byte 0xfbe4
	.2byte 0x2095
	.2byte 0x0100
	.2byte 0xf003
	.2byte 0xfb34
.L_02000fd0_10:
	movs r0, #0
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02009220
	.4byte 0x00000109
	.4byte 0x000000e6
	.2byte 0xa711
	.2byte 0x0200
	.2byte 0x0951
	.2byte 0x0000
	.global Func_0200137c
	.thumb_func
Func_0200137c:
	adds r1, r0, #0
	adds r1, #100
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, [r0, #8]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #8]
	movs r4, #102
	adds r4, r4, r0
	movs r3, #0
	ldrsh r2, [r4, r3]
	ldr r3, [r0, #12]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [pc, #28]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldrh r3, [r1]
	adds r3, #5
	strh r3, [r1]
	ldrh r3, [r4]
	subs r3, #1
	strh r3, [r4]
	movs r0, #0
	bx lr
	.2byte 0x0000
	.4byte 0x00000666
	.global Func_020013c0
	.thumb_func
Func_020013c0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	bl 0x0200ca18
	ldr r3, [pc, #152]
	adds r5, r0, #0
	ldr r0, [r3]
	ldr r7, [r5, #8]
	adds r0, #1
	ldr r5, [r5, #12]
	movs r1, #180
	str r0, [r3]
	movs r6, #41
	mov r8, r5
	bl 0x0200c838
	cmp r0, #20
	beq .L_020013c0_0
	cmp r0, #20
	bgt .L_020013c0_1
	cmp r0, #10
	beq .L_020013c0_2
	b .L_020013c0_3
.L_020013c0_1:
	cmp r0, #30
	beq .L_020013c0_4
	b .L_020013c0_3
.L_020013c0_0:
	movs r6, #42
	b .L_020013c0_2
.L_020013c0_4:
	movs r6, #43
.L_020013c0_2:
	adds r0, r6, #0
	bl 0x0200ca18
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020013c0_3
	movs r0, #0
	bl 0x0200ca18
	cmp r0, #0
	beq .L_020013c0_5
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	adds r0, r6, #0
	bl 0x0200ca68
.L_020013c0_5:
	adds r0, r6, #0
	bl 0x0200ca18
	movs r1, #0
	bl 0x0200c958
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [pc, #56]
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r7, r3
	str r2, [r5, #8]
	add r3, r8
	adds r2, r5, #0
	str r3, [r5, #12]
	str r3, [r5, #60]
	adds r2, #100
	movs r3, #25
	strh r3, [r2]
	adds r2, #2
	movs r3, #128
	strh r3, [r2]
	ldr r1, [pc, #24]
	adds r0, r6, #0
	bl 0x0200ca30
.L_020013c0_3:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200db80
	.4byte 0x00006666
	.4byte 0x0200d96c
	.global Func_02001474
	.thumb_func
Func_02001474:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #39
	bl 0x0200ca20
	movs r0, #40
	bl 0x0200ca20
	movs r0, #1
	bl 0x0200c9f0
	movs r0, #17
	bl 0x0200cba0
	bl 0x0200ca00
	movs r1, #193
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #19
	lsls r2, r2, #16
	bl 0x0200ca68
	cmp r5, #0
	bge .L_02001474_0
	movs r0, #8
	movs r1, #10
	bl 0x0200ca70
	b .L_02001474_1
.L_02001474_0:
	movs r0, #8
	movs r1, #8
	bl 0x0200ca70
.L_02001474_1:
	ldr r1, [pc, #1020]
	movs r0, #8
	bl 0x0200ca30
	movs r1, #188
	movs r2, #192
	lsls r2, r2, #16
	lsls r1, r1, #19
	movs r0, #0
	bl 0x0200ca68
	movs r0, #0
	bl 0x0200ca18
	movs r5, #0
	strh r5, [r0, #6]
	ldr r1, [pc, #992]
	movs r0, #0
	bl 0x0200ca30
	movs r0, #0
	movs r1, #35
	bl 0x0200ca70
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #183
	movs r2, #184
	movs r0, #1
	lsls r1, r1, #19
	lsls r2, r2, #16
	bl 0x0200ca68
	movs r1, #183
	movs r2, #200
	movs r0, #2
	lsls r1, r1, #19
	lsls r2, r2, #16
	bl 0x0200ca68
	movs r1, #181
	movs r2, #192
	lsls r2, r2, #16
	lsls r1, r1, #19
	movs r0, #3
	bl 0x0200ca68
	movs r0, #1
	bl 0x0200ca18
	strh r5, [r0, #6]
	movs r0, #2
	bl 0x0200ca18
	strh r5, [r0, #6]
	movs r0, #3
	bl 0x0200ca18
	strh r5, [r0, #6]
	movs r0, #1
	bl 0x0200c840
	movs r0, #0
	movs r1, #0
	bl 0x0200caf0
	ldr r3, [pc, #856]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #1
	str r5, [r3]
	movs r1, #1
	ldr r0, [pc, #844]
	bl 0x0200cb40
	bl 0x0200cb50
	bl 0x0200cb60
	ldr r0, [pc, #832]
	bl 0x0200cab0
	movs r0, #60
	bl 0x0200c9f8
	ldr r6, [pc, #824]
	movs r0, #0
	adds r1, r6, #0
	bl 0x0200ca30
	movs r0, #0
	bl 0x0200ca18
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	movs r0, #0
	mov r10, r3
	bl 0x0200ca18
	mov r2, r10
	movs r1, #36
	str r2, [r0, #28]
	movs r0, #0
	bl 0x0200ca78
	movs r0, #0
	bl 0x0200ca18
	movs r2, #192
	ldr r3, [r0, #8]
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r0, #8]
	movs r0, #10
	bl 0x0200c9f8
	movs r0, #0
	bl 0x0200ca18
	movs r1, #0
	bl 0x0200c958
	movs r0, #20
	bl 0x0200c9f8
	ldr r1, [pc, #748]
	movs r0, #0
	bl 0x0200ca30
	movs r1, #0
	movs r0, #1
	bl 0x0200cac0
	movs r0, #20
	bl 0x0200c9f8
	movs r1, #188
	movs r0, #1
	lsls r1, r1, #3
	movs r2, #176
	bl 0x0200ca50
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200cad0
	movs r2, #20
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200cae0
	movs r0, #2
	movs r1, #0
	bl 0x0200cac0
	movs r1, #2
	movs r0, #1
	bl 0x0200cb88
	movs r0, #30
	bl 0x0200c9f8
	movs r1, #186
	movs r0, #2
	lsls r1, r1, #3
	movs r2, #176
	bl 0x0200ca50
	movs r1, #190
	movs r0, #1
	lsls r1, r1, #3
	movs r2, #184
	bl 0x0200ca48
	movs r1, #188
	movs r2, #176
	movs r0, #2
	lsls r1, r1, #3
	bl 0x0200ca50
	movs r0, #1
	movs r1, #1
	bl 0x0200ca70
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200cad0
	movs r1, #128
	movs r2, #10
	movs r0, #2
	lsls r1, r1, #7
	bl 0x0200cad0
	movs r1, #2
	movs r0, #2
	bl 0x0200ca88
	movs r0, #10
	bl 0x0200c9f8
	movs r1, #1
	movs r0, #2
	bl 0x0200cb88
	movs r0, #30
	bl 0x0200c9f8
	movs r1, #4
	movs r0, #1
	bl 0x0200ca78
	movs r0, #30
	bl 0x0200c9f8
	movs r0, #3
	movs r1, #0
	bl 0x0200cac0
	movs r0, #1
	movs r1, #3
	bl 0x0200cb88
	movs r0, #2
	movs r1, #3
	bl 0x0200cb88
	movs r1, #186
	movs r2, #184
	movs r0, #3
	lsls r1, r1, #3
	bl 0x0200ca50
	movs r0, #2
	movs r1, #0
	bl 0x0200cb88
	movs r0, #1
	movs r1, #0
	bl 0x0200cb88
	movs r1, #129
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200cae0
	movs r0, #2
	movs r1, #1
	bl 0x0200cb88
	movs r1, #2
	movs r0, #1
	bl 0x0200cb88
	movs r0, #40
	bl 0x0200c9f8
	movs r1, #2
	movs r0, #3
	bl 0x0200ca88
	movs r0, #10
	bl 0x0200c9f8
	movs r0, #2
	movs r1, #3
	bl 0x0200cb88
	movs r1, #3
	movs r0, #1
	bl 0x0200cb88
	movs r0, #20
	bl 0x0200c9f8
	movs r0, #3
	movs r1, #0
	bl 0x0200cac0
	ldr r1, [pc, #432]
	movs r0, #0
	bl 0x0200ca30
	movs r0, #1
	movs r1, #2
	bl 0x0200ca88
	movs r1, #0
	movs r0, #1
	bl 0x0200cb88
	movs r0, #20
	bl 0x0200c9f8
	movs r0, #1
	movs r1, #0
	bl 0x0200cac0
	movs r0, #2
	movs r1, #2
	bl 0x0200ca88
	movs r1, #0
	movs r0, #2
	bl 0x0200cb88
	movs r0, #20
	bl 0x0200c9f8
	movs r0, #2
	movs r1, #0
	bl 0x0200cac0
	movs r0, #3
	movs r1, #2
	bl 0x0200ca88
	movs r1, #0
	movs r0, #3
	bl 0x0200cb88
	movs r0, #20
	bl 0x0200c9f8
	movs r0, #3
	movs r1, #0
	bl 0x0200cac0
	adds r1, r6, #0
	movs r0, #0
	bl 0x0200ca30
	movs r0, #60
	bl 0x0200c9f8
	ldr r3, [pc, #324]
	ldr r5, [pc, #328]
	movs r6, #9
	movs r1, #200
	str r6, [r3]
	lsls r1, r1, #4
	adds r0, r5, #0
	mov r8, r3
	bl 0x0200c848
	movs r0, #5
	bl 0x0200c9f8
	adds r0, r5, #0
	bl 0x0200c850
	movs r0, #55
	bl 0x0200c9f8
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #292]
	bl 0x0200cae0
	movs r1, #200
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200c848
	movs r0, #20
	bl 0x0200c9f8
	adds r0, r5, #0
	bl 0x0200c850
	movs r0, #40
	bl 0x0200c9f8
	movs r0, #2
	ldr r1, [pc, #256]
	movs r2, #60
	bl 0x0200cae0
	mov r2, r8
	movs r1, #200
	str r6, [r2]
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200c848
	movs r0, #35
	bl 0x0200c9f8
	adds r0, r5, #0
	bl 0x0200c850
	movs r0, #25
	bl 0x0200c9f8
	movs r1, #129
	movs r2, #60
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200cae0
	mov r3, r8
	movs r1, #200
	str r6, [r3]
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200c848
	movs r0, #35
	bl 0x0200c9f8
	adds r0, r5, #0
	bl 0x0200c850
	movs r0, #25
	bl 0x0200c9f8
	movs r1, #129
	movs r2, #60
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200cae0
	movs r0, #3
	movs r1, #2
	bl 0x0200cb88
	movs r1, #3
	movs r0, #2
	bl 0x0200cb88
	movs r0, #60
	bl 0x0200c9f8
	movs r0, #3
	movs r1, #0
	bl 0x0200cb88
	movs r0, #2
	movs r1, #0
	bl 0x0200cb88
	mov r2, r8
	movs r1, #200
	str r6, [r2]
	lsls r1, r1, #4
	adds r0, r5, #0
	bl 0x0200c848
	movs r0, #35
	bl 0x0200c9f8
	adds r0, r5, #0
	bl 0x0200c850
	movs r0, #25
	bl 0x0200c9f8
	movs r1, #132
	movs r2, #60
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200cae0
	movs r0, #1
	movs r1, #3
	bl 0x0200ca80
	movs r0, #2
	movs r1, #3
	bl 0x0200ca80
	movs r0, #3
	movs r1, #3
	bl 0x0200ca88
	movs r0, #3
	movs r1, #2
	bl 0x0200cb88
	movs r0, #1
	movs r1, #2
	b .L_02001474_2
	.2byte 0x0000
	.4byte 0x0200d668
	.4byte 0x0200d738
	.4byte 0x03001ebc
	.4byte 0x00010001
	.4byte 0x000020f1
	.4byte 0x0200d950
	.4byte 0x0200d808
	.4byte 0x0200d8ac
	.4byte 0x0200db80
	.4byte 0x020093c1
	.4byte 0x00000101
.L_02001474_2:
	bl 0x0200cb88
	mov r3, r8
	movs r1, #200
	str r6, [r3]
	adds r0, r5, #0
	lsls r1, r1, #4
	bl 0x0200c848
	movs r0, #1
	movs r1, #3
	bl 0x0200ca70
	movs r0, #2
	movs r1, #3
	bl 0x0200ca70
	movs r1, #3
	movs r0, #3
	bl 0x0200ca70
	movs r0, #60
	bl 0x0200c9f8
	movs r1, #183
	lsls r1, r1, #3
	movs r2, #200
	movs r0, #3
	bl 0x0200ca48
	movs r0, #5
	bl 0x0200c9f8
	movs r1, #171
	lsls r1, r1, #3
	movs r2, #184
	movs r0, #2
	bl 0x0200ca48
	movs r0, #3
	bl 0x0200c9f8
	movs r1, #189
	movs r0, #1
	lsls r1, r1, #3
	movs r2, #184
	bl 0x0200ca50
	movs r1, #171
	movs r2, #184
	lsls r1, r1, #3
	movs r0, #1
	bl 0x0200ca48
	movs r0, #3
	bl 0x0200ca60
	movs r0, #3
	movs r1, #1
	bl 0x0200ca70
	movs r1, #0
	movs r0, #3
	bl 0x0200cb88
	movs r0, #60
	bl 0x0200c9f8
	movs r1, #179
	movs r0, #3
	lsls r1, r1, #3
	movs r2, #200
	bl 0x0200ca50
	movs r1, #171
	lsls r1, r1, #3
	movs r2, #184
	movs r0, #3
	bl 0x0200ca48
	bl 0x0200cb58
	bl 0x0200cb60
	movs r0, #30
	bl 0x0200c9f8
	movs r1, #189
	movs r2, #176
	movs r0, #1
	lsls r1, r1, #19
	lsls r2, r2, #16
	bl 0x0200ca68
	movs r1, #183
	movs r2, #192
	movs r0, #2
	lsls r1, r1, #19
	lsls r2, r2, #16
	bl 0x0200ca68
	movs r1, #195
	movs r2, #200
	lsls r2, r2, #16
	movs r0, #3
	lsls r1, r1, #19
	bl 0x0200ca68
	bl 0x0200c960
	movs r1, #2
	mov r0, r10
	bl 0x0200cb40
	movs r0, #1
	bl 0x0200cb48
	ldr r0, [pc, #52]
	bl 0x0200cab0
	movs r0, #1
	movs r1, #0
	bl 0x0200cac0
	movs r0, #2
	movs r1, #0
	bl 0x0200cac0
	movs r1, #0
	movs r0, #3
	bl 0x0200cac0
	movs r0, #60
	bl 0x0200c9f8
	bl 0x0200ca08
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000214c
	.global Func_02001a0c
	.thumb_func
Func_02001a0c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	bl 0x0200ca18
	movs r3, #10
	ldrsh r2, [r0, r3]
	mov r9, r2
	movs r3, #18
	ldrsh r2, [r0, r3]
	mov r10, r2
	bl 0x0200ca00
	movs r1, #128
	movs r2, #128
	adds r0, r7, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	mov r3, r10
	lsls r5, r3, #16
	mov r2, r9
	ldr r3, [pc, #396]
	lsls r6, r2, #16
	adds r1, r6, #0
	adds r2, r5, r3
	movs r0, #0
	bl 0x0200ca68
	ldr r3, [pc, #388]
	ldr r2, [pc, #388]
	adds r3, r3, r5
	mov r8, r3
	adds r1, r6, r2
	movs r0, #1
	mov r2, r8
	bl 0x0200ca68
	movs r2, #128
	lsls r2, r2, #13
	adds r1, r6, r2
	movs r0, #2
	mov r2, r8
	bl 0x0200ca68
	ldr r3, [pc, #364]
	adds r1, r6, #0
	adds r2, r5, r3
	movs r0, #3
	bl 0x0200ca68
	ldr r2, [pc, #356]
	adds r5, r5, r2
	adds r2, r5, #0
	adds r1, r6, #0
	adds r0, r7, #0
	bl 0x0200ca68
	movs r0, #0
	bl 0x0200ca18
	movs r6, #192
	lsls r6, r6, #8
	movs r1, #0
	strh r6, [r0, #6]
	movs r0, #0
	bl 0x0200caf0
	bl 0x0200cb50
	bl 0x0200cb60
	ldr r0, [pc, #316]
	bl 0x0200cab0
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200ca78
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200ca88
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200ca88
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200ca88
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	movs r0, #3
	movs r1, #3
	bl 0x0200ca70
	movs r0, #1
	movs r1, #3
	bl 0x0200ca70
	movs r0, #2
	movs r1, #3
	bl 0x0200ca70
	movs r1, #3
	movs r0, #0
	bl 0x0200ca78
	movs r0, #6
	bl 0x0200c9f8
	movs r0, #1
	movs r1, #2
	bl 0x0200ca70
	movs r0, #0
	bl 0x0200ca18
	cmp r0, #0
	beq .L_02001a0c_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200ca40
.L_02001a0c_0:
	movs r0, #2
	movs r1, #2
	bl 0x0200ca70
	movs r0, #0
	bl 0x0200ca18
	cmp r0, #0
	beq .L_02001a0c_1
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200ca40
.L_02001a0c_1:
	movs r0, #3
	movs r1, #2
	bl 0x0200ca70
	movs r0, #0
	bl 0x0200ca18
	cmp r0, #0
	beq .L_02001a0c_2
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200ca40
.L_02001a0c_2:
	mov r5, r9
	subs r5, #16
	mov r2, r10
	adds r0, r7, #0
	adds r1, r5, #0
	subs r2, #64
	bl 0x0200ca50
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200ca68
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200ca68
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200ca68
	mov r2, r10
	adds r0, r7, #0
	adds r1, r5, #0
	subs r2, #16
	bl 0x0200ca50
	adds r0, r7, #0
	mov r1, r9
	mov r2, r10
	bl 0x0200ca50
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #10
	bl 0x0200cad0
	bl 0x0200ca08
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0xffd00000
	.4byte 0xffd80000
	.4byte 0xfff00000
	.4byte 0xffe00000
	.4byte 0xffb00000
	.4byte 0x000020ed
	.global Func_02001c20
	.thumb_func
Func_02001c20:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #440]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #2
	bne .L_02001c20_0
	bl 0x0200a88c
	b .L_02001c20_1
.L_02001c20_0:
	bl 0x0200ca00
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200aa94
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02001c20_2
	b .L_02001c20_3
.L_02001c20_2:
	ldr r0, [pc, #400]
	bl 0x0200cab0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200caf8
	movs r0, #152
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #16
	bl 0x0200cb00
	bl 0x0200cb08
	movs r0, #30
	bl 0x0200c9f8
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	movs r1, #68
	movs r2, #0
	movs r0, #104
	bl 0x0200ba58
	movs r0, #60
	bl 0x0200c9f8
	movs r2, #10
	movs r1, #96
	movs r0, #168
	bl 0x0200babc
	movs r0, #70
	bl 0x0200c9f8
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200cac0
	bl 0x0200bb18
	movs r0, #2
	bl 0x0200c840
	movs r0, #10
	bl 0x0200ca18
	adds r3, r0, #0
	ldr r2, [pc, #292]
	adds r3, #85
	strb r6, [r3]
	ldr r3, [pc, #292]
	mov r10, r2
	str r2, [r0, #48]
	movs r2, #128
	mov r8, r3
	ldr r1, [r0, #8]
	str r3, [r0, #52]
	lsls r2, r2, #11
	ldr r3, [r0, #16]
	bl 0x0200c910
	movs r0, #11
	bl 0x0200ca18
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	mov r2, r10
	str r2, [r5, #48]
	mov r3, r8
	movs r2, #128
	ldr r1, [r5, #8]
	lsls r2, r2, #14
	str r3, [r5, #52]
	ldr r3, [r5, #16]
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	movs r0, #45
	bl 0x0200c9f8
	movs r0, #10
	bl 0x0200ca18
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	mov r2, r10
	str r2, [r0, #48]
	mov r3, r8
	movs r2, #128
	ldr r1, [r0, #8]
	str r3, [r0, #52]
	lsls r2, r2, #14
	ldr r3, [r0, #16]
	bl 0x0200c910
	movs r0, #11
	bl 0x0200ca18
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	mov r2, r10
	mov r3, r8
	str r2, [r5, #48]
	movs r2, #128
	lsls r2, r2, #11
	ldr r1, [r5, #8]
	str r3, [r5, #52]
	ldr r3, [r5, #16]
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	movs r0, #15
	bl 0x0200c9f8
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	movs r1, #68
	movs r2, #0
	movs r0, #104
	bl 0x0200ba58
	movs r0, #30
	bl 0x0200c9f8
	movs r1, #96
	movs r2, #10
	movs r0, #168
	bl 0x0200babc
	movs r0, #40
	bl 0x0200c9f8
	movs r2, #10
	movs r1, #68
	movs r0, #104
	bl 0x0200babc
	movs r0, #70
	bl 0x0200c9f8
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200cac0
	bl 0x0200bb18
	movs r0, #2
	bl 0x0200c840
	movs r0, #0
	movs r1, #0
	bl 0x0200caf0
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200ab50
	b .L_02001c20_4
.L_02001c20_3:
	cmp r6, #1
	bne .L_02001c20_4
	ldr r0, [pc, #52]
	bl 0x0200cab0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
.L_02001c20_4:
	adds r1, r7, #0
	movs r2, #1
	adds r0, r6, #0
	bl 0x0200abac
	bl 0x0200ca08
.L_02001c20_1:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x000020b2
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x000020b1
	.global Func_02001df8
	.thumb_func
Func_02001df8:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #392]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r6, r0, #0
	cmp r3, #2
	bne .L_02001df8_0
	bl 0x0200a88c
	b .L_02001df8_1
.L_02001df8_0:
	bl 0x0200ca00
	adds r0, r6, #0
	movs r1, #2
	bl 0x0200aa94
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02001df8_2
	b .L_02001df8_3
.L_02001df8_2:
	ldr r0, [pc, #352]
	bl 0x0200cab0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200caf8
	movs r0, #188
	movs r1, #1
	movs r2, #152
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200cb00
	movs r5, #140
	bl 0x0200cb08
	movs r0, #30
	bl 0x0200c9f8
	lsls r5, r5, #1
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200cac0
	adds r1, r5, #0
	movs r2, #200
	movs r0, #0
	bl 0x0200b640
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	adds r1, r5, #0
	movs r2, #152
	movs r0, #0
	bl 0x0200bb80
	movs r1, #148
	lsls r1, r1, #1
	movs r2, #152
	movs r0, #0
	bl 0x0200bb80
	movs r0, #10
	bl 0x0200c9f8
	bl 0x0200cb30
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl 0x0200cb00
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #15
	movs r0, #0
	bl 0x0200cad0
	bl 0x0200cb30
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl 0x0200cb00
	movs r1, #0
	movs r2, #15
	movs r0, #0
	bl 0x0200cad0
	bl 0x0200cb30
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl 0x0200cb00
	movs r1, #128
	movs r2, #15
	movs r0, #0
	lsls r1, r1, #7
	bl 0x0200cad0
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200cac0
	movs r1, #40
	movs r2, #0
	movs r0, #96
	bl 0x0200ba58
	movs r1, #40
	movs r2, #10
	movs r0, #128
	bl 0x0200babc
	movs r0, #30
	bl 0x0200c9f8
	movs r1, #40
	movs r2, #10
	movs r0, #160
	bl 0x0200babc
	movs r0, #30
	bl 0x0200c9f8
	movs r2, #10
	movs r1, #72
	movs r0, #160
	bl 0x0200babc
	movs r0, #30
	bl 0x0200c9f8
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200cac0
	bl 0x0200bb18
	movs r0, #0
	bl 0x0200b81c
	movs r0, #0
	movs r1, #0
	bl 0x0200caf0
	adds r0, r6, #0
	movs r1, #2
	bl 0x0200ab50
	b .L_02001df8_4
.L_02001df8_3:
	cmp r7, #1
	bne .L_02001df8_4
	ldr r0, [pc, #40]
	bl 0x0200cab0
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200cac0
.L_02001df8_4:
	adds r1, r6, #0
	movs r2, #2
	adds r0, r7, #0
	bl 0x0200abac
	bl 0x0200ca08
.L_02001df8_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x000020b7
	.4byte 0x000020b6
	.global Func_02001f90
	.thumb_func
Func_02001f90:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #324]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #2
	bne .L_02001f90_0
	bl 0x0200a88c
	b .L_02001f90_1
.L_02001f90_0:
	bl 0x0200ca00
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200aa94
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02001f90_2
	ldr r3, [pc, #284]
	ldr r3, [r3]
	ldr r0, [pc, #284]
	mov r8, r3
	bl 0x0200cab0
	bl 0x02008188
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200caf8
	movs r0, #154
	movs r1, #1
	movs r2, #184
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #18
	bl 0x0200cb00
	bl 0x0200cb08
	movs r0, #30
	bl 0x0200c9f8
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200cac0
	bl 0x020081b4
	movs r0, #60
	bl 0x0200c9f8
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	movs r1, #252
	lsls r1, r1, #1
	movs r2, #200
	movs r0, #0
	bl 0x0200b640
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200cad0
	bl 0x020081c8
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #170
	movs r2, #200
	lsls r1, r1, #2
	movs r0, #0
	bl 0x0200bb40
	movs r3, #193
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	beq .L_02001f90_3
	movs r5, #193
	lsls r5, r5, #1
	add r5, r8
.L_02001f90_4:
	movs r0, #1
	adds r6, #1
	bl 0x0200c840
	cmp r6, #239
	bgt .L_02001f90_3
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #5
	bne .L_02001f90_4
.L_02001f90_3:
	bl 0x02008b30
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200cad0
	movs r2, #60
	movs r0, #0
	ldr r1, [pc, #100]
	bl 0x0200cae0
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200cac0
	movs r0, #0
	bl 0x0200b81c
	movs r0, #0
	movs r1, #0
	bl 0x0200caf0
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200ab50
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #0
	strh r3, [r2]
	b .L_02001f90_5
.L_02001f90_2:
	cmp r6, #1
	bne .L_02001f90_5
	ldr r0, [pc, #52]
	bl 0x0200cab0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
.L_02001f90_5:
	adds r1, r7, #0
	movs r2, #3
	adds r0, r6, #0
	bl 0x0200abac
	bl 0x0200ca08
.L_02001f90_1:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0x000020bb
	.4byte 0x00000103
	.4byte 0x000020ba
	.global Func_020020f0
	.thumb_func
Func_020020f0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #440]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #2
	bne .L_020020f0_0
	bl 0x0200a88c
	b 0x0200a2a6
.L_020020f0_0:
	bl 0x0200ca00
	adds r0, r7, #0
	movs r1, #4
	bl 0x0200aa94
	mov r11, r0
	cmp r0, #0
	beq .L_020020f0_1
	b 0x0200a284
.L_020020f0_1:
	ldr r0, [pc, #400]
	bl 0x0200cab0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200caf8
	movs r0, #214
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #18
	bl 0x0200cb00
	bl 0x0200cb08
	movs r0, #30
	bl 0x0200c9f8
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	movs r1, #204
	lsls r1, r1, #2
	movs r2, #200
	movs r0, #0
	bl 0x0200b640
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #210
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #200
	bl 0x0200ca50
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #0
	bl 0x0200cad0
	bl 0x0200cb38
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl 0x0200cb00
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
.L_020021b6:
	lsls r2, r2, #7
	movs r0, #0
	bl 0x0200ca28
	movs r0, #0
	bl 0x0200ca18
	adds r5, r0, #0
	ldr r3, [r5, #12]
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #0
	lsls r1, r1, #8
	mov r8, r3
	ldr r6, [r5, #8]
	bl 0x0200ca28
	movs r0, #0
	movs r1, #10
	bl 0x0200ca70
	movs r2, #192
	lsls r2, r2, #11
	add r2, r8
	ldr r3, [r5, #16]
	adds r1, r6, #0
	adds r0, r5, #0
	mov r9, r2
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	movs r0, #0
	movs r1, #14
	bl 0x0200ca70
	movs r3, #128
	lsls r3, r3, #15
	adds r3, r3, r6
	mov r10, r3
	mov r2, r9
	ldr r3, [r5, #16]
	mov r1, r10
	adds r0, r5, #0
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	movs r0, #0
	movs r1, #10
	bl 0x0200ca70
	movs r2, #216
	lsls r2, r2, #14
	add r8, r2
	ldr r3, [r5, #16]
	mov r2, r8
	mov r1, r10
	adds r0, r5, #0
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	movs r0, #0
	movs r1, #15
	bl 0x0200ca70
	movs r3, #192
	lsls r3, r3, #14
	adds r6, r6, r3
	mov r2, r8
	ldr r3, [r5, #16]
	adds r1, r6, #0
	adds r0, r5, #0
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	movs r0, #0
	movs r1, #12
	bl 0x0200ca70
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200cac0
	movs r0, #0
	bl 0x0200b81c
	movs r0, #0
	movs r1, #0
	bl 0x0200caf0
	adds r0, r7, #0
	movs r1, #4
	bl 0x0200ab50
	b .L_020021b6_0
	.2byte 0x465a
	.2byte 0x2a01
	.2byte 0xd106
	.2byte 0x480d
	.2byte 0xf002
	.2byte 0xfc10
	.2byte 0x1c38
	.2byte 0x2100
	.2byte 0xf002
	.2byte 0xfc14
.L_020021b6_0:
	adds r1, r7, #0
	movs r2, #4
	mov r0, r11
	bl 0x0200abac
	bl 0x0200ca08
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x20bf
	.2byte 0x0000
	.2byte 0x20be
	.2byte 0x0000
	.global Func_020022c4
	.thumb_func
Func_020022c4:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r2, #0
	bl 0x0200cb68
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020022c4_0
	bl 0x0200c900
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200c8e0
	lsls r1, r6, #16
	ldr r2, [r5, #12]
	lsls r3, r7, #16
	adds r0, r5, #0
	bl 0x0200c910
.L_020022c4_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r2, #0
	bl 0x0200cb68
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020022f4_0
	bl 0x0200c900
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200c8e0
	lsls r1, r6, #16
	ldr r2, [r5, #12]
	lsls r3, r7, #16
	adds r0, r5, #0
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c8e0
.L_020022f4_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002330
	.thumb_func
Func_02002330:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #372]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #2
	bne .L_02002330_0
	bl 0x0200a88c
	b .L_02002330_1
.L_02002330_0:
	bl 0x0200ca00
	adds r0, r7, #0
	movs r1, #5
	bl 0x0200aa94
	mov r8, r0
	cmp r0, #0
	beq .L_02002330_2
	b .L_02002330_3
.L_02002330_2:
	ldr r0, [pc, #332]
	bl 0x0200cab0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200caf8
	movs r0, #135
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #19
	bl 0x0200cb00
	bl 0x0200cb08
	movs r0, #30
	bl 0x0200c9f8
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
	movs r1, #246
	lsls r1, r1, #2
	movs r2, #184
	movs r0, #0
	bl 0x0200b640
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #248
	lsls r1, r1, #2
	movs r0, #0
	movs r2, #184
	bl 0x0200a2f4
	movs r0, #0
	ldr r1, [pc, #232]
	ldr r2, [pc, #236]
	bl 0x0200ca28
	movs r1, #140
	movs r2, #184
	lsls r1, r1, #3
	movs r0, #0
	bl 0x0200a2c4
	movs r0, #120
	bl 0x0200c9f8
	ldr r1, [pc, #216]
	movs r0, #0
	bl 0x0200cae8
	movs r0, #120
	bl 0x0200c9f8
	movs r0, #0
	bl 0x0200b8dc
	movs r0, #0
	movs r1, #1
	bl 0x0200ca70
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200cae8
	movs r0, #0
	ldr r1, [pc, #180]
	movs r2, #0
	bl 0x0200cae0
	movs r0, #0
	bl 0x0200ca18
	movs r6, #119
	adds r5, r0, #0
.L_02002330_5:
	movs r1, #248
	ldr r3, [r5, #8]
	lsls r1, r1, #18
	cmp r3, r1
	ble .L_02002330_4
	ldr r2, [pc, #156]
	adds r3, r3, r2
	str r3, [r5, #8]
.L_02002330_4:
	movs r0, #1
	subs r6, #1
	bl 0x0200c840
	cmp r6, #0
	bge .L_02002330_5
	movs r0, #0
	ldr r1, [pc, #140]
	movs r2, #60
	bl 0x0200cae0
	movs r1, #140
	movs r2, #184
	lsls r1, r1, #3
	movs r0, #0
	bl 0x0200a2c4
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200cac0
	movs r0, #0
	bl 0x0200b8dc
	ldr r3, [pc, #76]
	movs r1, #249
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strb r3, [r2]
	movs r0, #0
	bl 0x0200b81c
	movs r0, #0
	movs r1, #0
	bl 0x0200caf0
	adds r0, r7, #0
	movs r1, #5
	bl 0x0200ab50
	b .L_02002330_6
.L_02002330_3:
	mov r2, r8
	cmp r2, #1
	bne .L_02002330_6
	ldr r0, [pc, #68]
	bl 0x0200cab0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
.L_02002330_6:
	adds r1, r7, #0
	movs r2, #5
	mov r0, r8
	bl 0x0200abac
	bl 0x0200ca08
.L_02002330_1:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x000020c3
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x00000101
	.4byte 0x00000105
	.4byte 0xfffecccd
	.4byte 0x00000103
	.4byte 0x000020c2
	.global Func_020024d0
	.thumb_func
Func_020024d0:
	push {r5, r6, lr}
	ldr r3, [pc, #232]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r5, r0, #0
	cmp r3, #2
	bne .L_020024d0_0
	bl 0x0200a88c
	b .L_020024d0_1
.L_020024d0_0:
	bl 0x0200ca00
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200aa94
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020024d0_2
	ldr r0, [pc, #192]
	bl 0x0200cab0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200caf8
	movs r0, #161
	movs r1, #1
	movs r2, #152
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #19
	bl 0x0200cb00
	bl 0x0200cb08
	movs r0, #30
	bl 0x0200c9f8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200cac0
	movs r2, #0
	movs r1, #88
	movs r0, #180
	bl 0x0200ba58
	movs r0, #60
	bl 0x0200c9f8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200cac0
	movs r2, #10
	movs r1, #84
	movs r0, #32
	bl 0x0200babc
	movs r0, #30
	bl 0x0200c9f8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200cac0
	movs r2, #30
	movs r1, #84
	movs r0, #96
	bl 0x0200babc
	movs r0, #60
	bl 0x0200c9f8
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200cac0
	bl 0x0200bb18
	movs r0, #2
	bl 0x0200c9f8
	movs r0, #0
	movs r1, #0
	bl 0x0200caf0
	adds r0, r5, #0
	movs r1, #6
	bl 0x0200ab50
	b .L_020024d0_3
.L_020024d0_2:
	cmp r6, #1
	bne .L_020024d0_3
	ldr r0, [pc, #40]
	bl 0x0200cab0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200cac0
.L_020024d0_3:
	adds r1, r5, #0
	movs r2, #6
	adds r0, r6, #0
	bl 0x0200abac
	bl 0x0200ca08
.L_020024d0_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x000020c7
	.4byte 0x000020c6
	.global Func_020025c8
	.thumb_func
Func_020025c8:
	push {r5, r6, lr}
	movs r0, #224
	lsls r0, r0, #2
	bl 0x0200c9c8
	adds r5, r0, #0
	movs r0, #226
	lsls r0, r0, #2
	bl 0x0200c9c8
	movs r6, #128
	lsls r6, r6, #12
	adds r2, r0, #0
	lsls r5, r5, #20
	adds r5, r5, r6
	lsls r2, r2, #20
	adds r2, r2, r6
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200ca68
	movs r0, #228
	lsls r0, r0, #2
	bl 0x0200c9c8
	adds r5, r0, #0
	movs r0, #230
	lsls r0, r0, #2
	bl 0x0200c9c8
	lsls r5, r5, #20
	adds r2, r0, #0
	adds r5, r5, r6
	lsls r2, r2, #20
	adds r2, r2, r6
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200ca68
	movs r0, #232
	lsls r0, r0, #2
	bl 0x0200c9c8
	adds r5, r0, #0
	movs r0, #234
	lsls r0, r0, #2
	bl 0x0200c9c8
	lsls r5, r5, #20
	adds r2, r0, #0
	adds r5, r5, r6
	lsls r2, r2, #20
	adds r2, r2, r6
	movs r0, #3
	adds r1, r5, #0
	bl 0x0200ca68
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02002640
	.thumb_func
Func_02002640:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200c9e8
	movs r0, #1
	bl 0x0200c9e8
	movs r0, #2
	bl 0x0200c9e8
	movs r0, #3
	bl 0x0200c9e8
	movs r0, #5
	bl 0x0200c9e8
	adds r0, r5, #0
	bl 0x0200c9e0
	ldr r3, [pc, #76]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	str r5, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200caf0
	adds r0, r5, #0
	bl 0x0200c998
	adds r5, r0, #0
	ldrh r3, [r5, #52]
	ldr r1, [pc, #52]
	strh r3, [r5, #56]
	ldrh r3, [r5, #54]
	ldr r2, [pc, #40]
	strh r3, [r5, #58]
	adds r3, r5, r1
	strb r2, [r3]
	movs r2, #56
	ldrsh r0, [r5, r2]
	movs r3, #52
	ldrsh r1, [r5, r3]
	lsls r0, r0, #14
	bl 0x0200c830
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_02002640_0
	movs r3, #0
	cmp r0, #0
	blt .L_02002640_0
	adds r3, r0, #0
	b .L_02002640_0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x00000131
.L_02002640_0:
	strh r3, [r5, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02002640_1
	movs r1, #56
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_02002640_1
	movs r3, #1
	strh r3, [r5, #20]
.L_02002640_1:
	movs r2, #58
	ldrsh r0, [r5, r2]
	movs r3, #54
	ldrsh r1, [r5, r3]
	lsls r0, r0, #14
	bl 0x0200c830
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_02002640_2
	movs r3, #0
	cmp r0, #0
	blt .L_02002640_2
	adds r3, r0, #0
.L_02002640_2:
	strh r3, [r5, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02002640_3
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_02002640_3
	movs r3, #1
	strh r3, [r5, #22]
.L_02002640_3:
	bl 0x0200cb80
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02002710
	.thumb_func
Func_02002710:
	push {r5, lr}
	ldr r3, [pc, #56]
	movs r1, #250
	ldr r5, [r3]
	ldr r3, [pc, #52]
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r2, [r3]
	cmp r2, #0
	beq .L_02002710_0
	subs r1, #118
	adds r3, r5, r1
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r3, r3, #26
	cmp r3, r2
	bne .L_02002710_0
	ldr r0, [pc, #32]
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02002710_0
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #99
	strh r3, [r2]
.L_02002710_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000141
	.global Func_02002758
	.thumb_func
Func_02002758:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #280]
	ldr r3, [r3]
	mov r9, r3
	movs r3, #128
	movs r2, #8
	lsls r3, r3, #13
	mov r10, r2
	mov r8, r3
	movs r2, #250
	ldr r3, [pc, #264]
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r7, [r3]
	adds r0, r7, #0
	bl 0x0200ca18
	adds r6, r0, #0
	bl 0x0200ca00
	movs r5, #8
.L_02002758_5:
	adds r0, r5, #0
	bl 0x0200ca18
	cmp r0, #0
	beq .L_02002758_0
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_02002758_0
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #165
	bne .L_02002758_0
	ldr r2, [r6, #8]
	ldr r3, [r0, #8]
	subs r2, r2, r3
	cmp r2, #0
	bge .L_02002758_1
	ldr r3, [pc, #204]
	adds r2, r2, r3
.L_02002758_1:
	asrs r1, r2, #16
	ldr r3, [r0, #16]
	ldr r2, [r6, #16]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_02002758_2
	ldr r2, [pc, #188]
	adds r0, r0, r2
.L_02002758_2:
	asrs r0, r0, #16
	cmp r0, #0
	bgt .L_02002758_0
	adds r2, r1, #0
	cmp r2, #0
	bge .L_02002758_3
	negs r2, r2
.L_02002758_3:
	cmp r0, #0
	bge .L_02002758_4
	negs r0, r0
.L_02002758_4:
	adds r0, r2, r0
	cmp r0, r8
	bge .L_02002758_0
	mov r10, r5
	mov r8, r0
.L_02002758_0:
	adds r5, #1
	cmp r5, #66
	ble .L_02002758_5
	ldr r0, [pc, #152]
	bl 0x0200cab0
	movs r1, #0
	mov r0, r10
	bl 0x0200cac0
	movs r3, #224
	lsls r3, r3, #1
	add r3, r9
	mov r8, r3
	movs r3, #128
	lsls r3, r3, #2
	mov r2, r8
	str r3, [r2]
	movs r2, #228
	lsls r2, r2, #1
	add r2, r9
	movs r3, #15
	str r3, [r2]
	movs r0, #20
	bl 0x0200c9f8
	bl 0x0200cb58
	bl 0x0200cb60
	ldr r1, [r6, #8]
	movs r3, #220
	lsls r5, r7, #4
	lsls r3, r3, #2
	adds r0, r5, r3
	asrs r1, r1, #20
	bl 0x0200c9d0
	ldr r1, [r6, #16]
	movs r2, #222
	lsls r2, r2, #2
	asrs r1, r1, #20
	adds r0, r5, r2
	adds r7, #1
	bl 0x0200c9d0
	cmp r7, #3
	ble .L_02002758_6
	movs r0, #10
	bl 0x0200cb10
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200c9b8
	b .L_02002758_7
.L_02002758_6:
	adds r0, r7, #0
	bl 0x0200a640
	bl 0x0200cb50
	bl 0x0200cb60
	movs r3, #0
	mov r2, r8
	str r3, [r2]
.L_02002758_7:
	bl 0x0200ca08
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000ffff
	.4byte 0x00002085
	.global Func_0200288c
	.thumb_func
Func_0200288c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, [pc, #248]
	adds r7, r0, #0
	ldr r0, [r5]
	mov r9, r0
	adds r0, r7, #0
	bl 0x0200ca18
	adds r0, r7, #0
	bl 0x0200ca18
	ldr r3, [pc, #232]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r6, [r3]
	adds r0, r6, #0
	bl 0x0200ca18
	mov r11, r0
	bl 0x0200ca00
	ldr r3, [pc, #212]
	mov r8, r3
	mov r0, r8
	bl 0x0200cab0
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200cab8
	ldr r2, [r5]
	ldr r0, [pc, #196]
	ldr r1, [pc, #200]
	adds r3, r2, r0
	strh r1, [r3]
	ldr r3, [pc, #196]
	adds r2, r2, r3
	movs r3, #4
	strh r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200ca10
	mov r10, r0
	cmp r0, #0
	bne .L_0200288c_0
	mov r0, r8
	adds r0, #1
	bl 0x0200cab0
	adds r0, r7, #0
	movs r1, #0
	movs r7, #224
	bl 0x0200cac0
	lsls r7, r7, #1
	movs r3, #128
	movs r2, #228
	lsls r3, r3, #2
	add r7, r9
	lsls r2, r2, #1
	add r2, r9
	str r3, [r7]
	movs r3, #15
	str r3, [r2]
	bl 0x0200cb58
	bl 0x0200cb60
	mov r0, r11
	ldr r1, [r0, #8]
	movs r2, #220
	lsls r5, r6, #4
	lsls r2, r2, #2
	adds r0, r5, r2
	asrs r1, r1, #20
	bl 0x0200c9d0
	mov r3, r11
	ldr r1, [r3, #16]
	movs r2, #222
	lsls r2, r2, #2
	asrs r1, r1, #20
	adds r0, r5, r2
	adds r6, #1
	bl 0x0200c9d0
	cmp r6, #3
	ble .L_0200288c_1
	movs r0, #10
	bl 0x0200cb10
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200c9b8
	b .L_0200288c_2
.L_0200288c_1:
	adds r0, r6, #0
	bl 0x0200a640
	bl 0x0200cb50
	bl 0x0200cb60
	mov r3, r10
	str r3, [r7]
	b .L_0200288c_2
.L_0200288c_0:
	mov r0, r8
	adds r0, #2
	bl 0x0200cab0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200cac0
.L_0200288c_2:
	bl 0x0200ca08
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00002086
	.4byte 0x00000cc2
	.4byte 0x00002089
	.4byte 0x00000cc4
	.global Func_020029ac
	.thumb_func
Func_020029ac:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	bl 0x0200ca18
	movs r3, #10
	ldrsh r7, [r0, r3]
	movs r3, #18
	ldrsh r6, [r0, r3]
	bl 0x0200ca00
	bl 0x0200c9d8
	cmp r0, #1
	bgt .L_020029ac_0
	ldr r0, [pc, #124]
	bl 0x0200cab0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200cac8
	cmp r0, #0
	bne .L_020029ac_1
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	adds r2, r6, #0
	adds r2, #64
	adds r1, r7, #0
	adds r0, r5, #0
	bl 0x0200ca48
	movs r0, #15
	bl 0x0200c9f8
	movs r0, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl 0x0200ca50
	adds r2, r6, #0
	movs r0, #0
	adds r2, #32
	adds r1, r7, #0
	bl 0x0200ca50
	bl 0x0200cb58
	bl 0x0200cb60
	movs r0, #11
	bl 0x0200cb10
	b .L_020029ac_1
.L_020029ac_0:
	ldr r0, [pc, #28]
	bl 0x0200cab0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200cac0
.L_020029ac_1:
	bl 0x0200ca08
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000020e5
	.4byte 0x000020e8
	.global Func_02002a50
	.thumb_func
Func_02002a50:
	push {lr}
	movs r0, #224
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200c9d0
	movs r0, #226
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200c9d0
	movs r0, #228
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200c9d0
	movs r0, #230
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200c9d0
	movs r0, #232
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200c9d0
	movs r0, #234
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200c9d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002a94
	.thumb_func
Func_02002a94:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	bl 0x0200cb70
	movs r1, #5
	adds r0, r5, #0
	bl 0x0200c978
	ldr r3, [pc, #140]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #132]
	cmp r2, r3
	bne .L_02002a94_0
	ldr r0, [pc, #128]
	b .L_02002a94_1
.L_02002a94_0:
	ldr r3, [pc, #128]
	cmp r2, r3
	bne .L_02002a94_2
	ldr r0, [pc, #128]
	b .L_02002a94_1
.L_02002a94_2:
	ldr r0, [pc, #128]
.L_02002a94_1:
	bl 0x0200cab0
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200cac0
	movs r2, #128
	lsls r2, r2, #2
	adds r0, r5, r2
	bl 0x0200c9b0
	cmp r0, #0
	bne .L_02002a94_3
	movs r3, #130
	lsls r3, r3, #2
	adds r5, r5, r3
	adds r0, r5, #0
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02002a94_4
	movs r0, #0
	bl 0x0200c988
	cmp r0, #1
	bne .L_02002a94_5
.L_02002a94_3:
	movs r0, #2
	b .L_02002a94_6
.L_02002a94_5:
	cmp r0, #2
	beq .L_02002a94_7
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_02002a94_6
.L_02002a94_7:
	movs r0, #3
	b .L_02002a94_6
.L_02002a94_4:
	adds r0, r5, #0
	bl 0x0200c9b8
	ldr r0, [pc, #52]
	bl 0x0200cab0
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200cab8
	movs r0, #0
	movs r1, #0
	bl 0x0200ca10
.L_02002a94_6:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008f
	.4byte 0x00002076
	.4byte 0x00000090
	.4byte 0x00002078
	.4byte 0x0000207a
	.4byte 0x0000207c
	.global Func_02002b50
	.thumb_func
Func_02002b50:
	push {r5, lr}
	adds r5, r0, #0
	adds r0, r1, #0
	movs r1, #5
	bl 0x0200c978
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02002b50_0
	ldr r0, [pc, #44]
	b .L_02002b50_1
.L_02002b50_0:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02002b50_2
	ldr r0, [pc, #40]
	b .L_02002b50_1
.L_02002b50_2:
	ldr r0, [pc, #40]
.L_02002b50_1:
	adds r0, #1
	bl 0x0200cab0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200cac0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008f
	.4byte 0x00002076
	.4byte 0x00000090
	.4byte 0x00002078
	.4byte 0x0000207a
	.global Func_02002bac
	.thumb_func
Func_02002bac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	sub sp, #12
	adds r6, r0, #0
	mov r0, r10
	str r2, [sp, #0]
	bl 0x0200ca18
	adds r5, r0, #0
	movs r2, #10
	ldrsh r0, [r5, r2]
	mov r9, r0
	movs r0, #18
	ldrsh r3, [r5, r0]
	mov r11, r3
	cmp r6, #3
	beq .L_02002bac_0
	bl 0x0200c9d8
	adds r7, r0, #0
	cmp r7, #0
	ble .L_02002bac_1
	ldr r3, [pc, #536]
	movs r0, #252
	lsls r0, r0, #1
	add r2, sp, #4
	adds r1, r3, r0
	adds r5, r7, #0
.L_02002bac_2:
	ldrb r3, [r1]
	subs r5, #1
	strb r3, [r2]
	adds r1, #1
	adds r2, #1
	cmp r5, #0
	bne .L_02002bac_2
.L_02002bac_1:
	cmp r7, #1
	bgt .L_02002bac_3
	ldr r0, [pc, #512]
	b .L_02002bac_4
.L_02002bac_3:
	ldr r2, [sp, #0]
	movs r3, #128
	lsls r3, r3, #2
	adds r0, r2, r3
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02002bac_5
	ldr r0, [pc, #496]
	b .L_02002bac_4
.L_02002bac_5:
	cmp r6, #2
	bne .L_02002bac_6
	movs r0, #6
	movs r5, #0
	bl 0x0200c840
	b .L_02002bac_7
.L_02002bac_6:
	ldr r0, [pc, #480]
	bl 0x0200cab0
	movs r1, #0
	mov r0, r10
	bl 0x0200cab8
	movs r0, #0
	movs r1, #0
	bl 0x0200ca10
	adds r5, r0, #0
.L_02002bac_7:
	cmp r5, #0
	bne .L_02002bac_0
	cmp r5, r7
	bge .L_02002bac_8
	add r6, sp, #4
	adds r5, r7, #0
.L_02002bac_9:
	ldrb r0, [r6]
	lsls r0, r0, #24
	asrs r0, r0, #24
	subs r5, #1
	adds r6, #1
	bl 0x0200c9e8
	cmp r5, #0
	bne .L_02002bac_9
.L_02002bac_8:
	cmp r7, #0
	ble .L_02002bac_10
	add r6, sp, #4
	adds r5, r7, #0
.L_02002bac_12:
	ldrb r3, [r6]
	lsls r3, r3, #24
	asrs r0, r3, #24
	adds r6, #1
	cmp r0, #0
	beq .L_02002bac_11
	bl 0x0200c9e0
.L_02002bac_11:
	subs r5, #1
	cmp r5, #0
	bne .L_02002bac_12
.L_02002bac_10:
	bl 0x0200cb98
	mov r8, r0
	cmp r7, #0
	ble .L_02002bac_13
	add r6, sp, #4
	adds r5, r7, #0
.L_02002bac_14:
	ldrb r0, [r6]
	lsls r0, r0, #24
	asrs r0, r0, #24
	subs r5, #1
	adds r6, #1
	bl 0x0200c9e8
	cmp r5, #0
	bne .L_02002bac_14
.L_02002bac_13:
	cmp r7, #0
	ble .L_02002bac_15
	add r6, sp, #4
	adds r5, r7, #0
.L_02002bac_16:
	ldrb r0, [r6]
	lsls r0, r0, #24
	asrs r0, r0, #24
	subs r5, #1
	adds r6, #1
	bl 0x0200c9e0
	cmp r5, #0
	bne .L_02002bac_16
.L_02002bac_15:
	movs r0, #1
	negs r0, r0
	cmp r8, r0
	bne .L_02002bac_17
.L_02002bac_0:
	ldr r0, [pc, #336]
.L_02002bac_4:
	bl 0x0200cab0
	mov r0, r10
	movs r1, #0
	bl 0x0200cac0
	b .L_02002bac_18
.L_02002bac_17:
	movs r1, #1
	mov r0, r8
	bl 0x0200c978
	ldr r0, [pc, #316]
	bl 0x0200cab0
	mov r0, r10
	movs r1, #0
	bl 0x0200cac0
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	mov r0, r8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r1, #128
	movs r2, #128
	mov r0, r10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ca28
	movs r0, #0
	bl 0x0200ca18
	cmp r0, #0
	beq .L_02002bac_19
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	mov r0, r8
	bl 0x0200ca68
.L_02002bac_19:
	mov r5, r11
	adds r5, #16
	mov r6, r9
	adds r2, r5, #0
	adds r6, #16
	mov r0, r8
	mov r1, r9
	bl 0x0200ca50
	adds r1, r6, #0
	adds r2, r5, #0
	movs r0, #0
	bl 0x0200ca50
	movs r2, #30
	mov r0, r8
	movs r1, #0
	bl 0x0200caa0
	mov r0, r8
	movs r1, #3
	bl 0x0200ca70
	subs r5, #32
	movs r0, #0
	movs r1, #3
	bl 0x0200ca78
	adds r2, r5, #0
	mov r0, r10
	mov r1, r9
	bl 0x0200ca50
	adds r2, r5, #0
	adds r1, r6, #0
	mov r0, r10
	bl 0x0200ca48
	movs r0, #0
	mov r1, r8
	bl 0x0200cb88
	adds r2, r5, #0
	mov r0, r8
	mov r1, r9
	bl 0x0200ca50
	mov r0, r10
	movs r1, #1
	bl 0x0200ca70
	movs r1, #128
	mov r0, r10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200cad0
	mov r2, r11
	subs r2, #48
	mov r0, r8
	mov r1, r9
	bl 0x0200ca50
	adds r2, r5, #0
	mov r0, r10
	mov r1, r9
	bl 0x0200ca50
	mov r1, r9
	mov r2, r11
	mov r0, r10
	bl 0x0200ca50
	mov r0, r8
	bl 0x0200c9e8
	movs r3, #128
	ldr r2, [sp, #0]
	lsls r3, r3, #2
	adds r0, r2, r3
	bl 0x0200c9b8
	mov r0, r8
	bl 0x0200ca18
	adds r5, r0, #0
	ldr r1, [r5, #8]
	mov r0, r8
	movs r2, #220
	lsls r6, r0, #4
	lsls r2, r2, #2
	asrs r1, r1, #20
	adds r0, r6, r2
	bl 0x0200c9d0
	ldr r1, [r5, #16]
	movs r3, #222
	lsls r3, r3, #2
	asrs r1, r1, #20
	adds r0, r6, r3
	bl 0x0200c9d0
.L_02002bac_18:
	sub sp, #-12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00002083
	.4byte 0x00002084
	.4byte 0x0000207d
	.4byte 0x0000207e
	.4byte 0x0000207f
	.global Func_02002e18
	.thumb_func
Func_02002e18:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	mov r8, r1
	bl 0x0200c998
	mov r1, r8
	adds r5, r0, #0
	adds r0, r7, #0
	bl 0x0200c9a0
	movs r6, #0
	adds r5, #216
.L_02002e18_1:
	ldrh r3, [r5]
	adds r5, #2
	cmp r3, r8
	bne .L_02002e18_0
	adds r0, r7, #0
	adds r1, r6, #0
	bl 0x0200c9a8
.L_02002e18_0:
	adds r6, #1
	cmp r6, #14
	ble .L_02002e18_1
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002e54
	.thumb_func
Func_02002e54:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #229
	lsls r0, r0, #5
	bl 0x0200c888
	ldr r7, [pc, #104]
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #1
	negs r2, r2
	adds r6, r0, #0
	cmp r3, r2
	bne .L_02002e54_0
	bl 0x0200c8b8
	strh r0, [r7]
.L_02002e54_0:
	ldr r3, [pc, #88]
	ldrb r3, [r3, r5]
	mov r8, r3
	cmp r5, #8
	bne .L_02002e54_1
	movs r5, #4
.L_02002e54_1:
	ldr r0, [pc, #80]
	bl 0x0200c8d0
	adds r1, r6, #0
	bl 0x0200c898
	mov r2, r8
	adds r0, r6, r2
	ldr r3, [pc, #68]
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	lsls r2, r5, #10
	adds r2, r2, r6
	movs r1, #128
	adds r2, #160
	lsls r1, r1, #3
	movs r3, #0
	ldrsh r0, [r7, r3]
	bl 0x0200c8b0
	movs r2, #128
	ldr r1, [pc, #36]
	lsls r2, r2, #24
.L_02002e54_2:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_02002e54_2
	adds r0, r6, #0
	bl 0x0200c890
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200d9a4
	.4byte 0x0200ccb0
	.4byte 0x000000e7
	.4byte 0x040000d4
	.4byte 0x050003e0
	.4byte 0x84000008
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #340]
	sub	sp, #20
	str	r0, [sp, #8]
	ldr	r3, [pc, #336]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #336]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r0
	mov	r8, r3
.L_02002f10:
	ldr	r1, [pc, #324]
	movs	r2, #0
	ldrsh	r4, [r1, r2]
	ldrh	r3, [r1, #0]
	cmp	r4, #0
	bne.n	.L_02002fe6
	ldr	r5, [pc, #316]
	ldr	r0, [r5, #0]
	ldrh	r3, [r0, #0]
	movs	r2, #128
	lsls	r3, r3, #16
	adds	r0, #2
	asrs	r3, r3, #16
	lsls	r2, r2, #6
	str	r0, [r5, #0]
	cmp	r3, r2
	beq.n	.L_02002fac
	cmp	r3, r2
	bgt.n	.L_02002f48
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_02002fd4
	movs	r2, #128
	lsls	r2, r2, #5
	cmp	r3, r2
	beq.n	.L_02002f94
	b.n	.L_02002f10
.L_02002f48:
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r3, r2
	beq.n	.L_02002f66
	cmp	r3, r2
	bgt.n	.L_02002f5e
	movs	r1, #192
	lsls	r1, r1, #6
	cmp	r3, r1
	beq.n	.L_02002f7c
	b.n	.L_02002f10
.L_02002f5e:
	ldr	r2, [pc, #256]
	cmp	r3, r2
	beq.n	.L_02002fca
	b.n	.L_02002f10
.L_02002f66:
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	ldr	r2, [pc, #248]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #240]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #240]
	b.n	.L_02002fc2
.L_02002f7c:
	ldr	r2, [pc, #232]
	ldr	r1, [pc, #240]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #220]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #224]
	b.n	.L_02002fc2
.L_02002f94:
	ldr	r2, [pc, #224]
	ldr	r1, [pc, #228]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #216]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #216]
	b.n	.L_02002fc2
.L_02002fac:
	ldr	r2, [pc, #216]
	ldr	r1, [pc, #220]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #208]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #208]
.L_02002fc2:
	adds	r2, #2
	str	r2, [r5, #0]
	strh	r4, [r3, #0]
	b.n	.L_02002f10
.L_02002fca:
	ldrh	r3, [r0, #0]
	strh	r3, [r1, #0]
	adds	r3, r0, #2
	str	r3, [r5, #0]
	b.n	.L_02002f10
.L_02002fd4:
	ldr	r0, [pc, #192]
	bl 0x0200c850
	ldr	r3, [pc, #116]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200c8a0
	b.n	.L_0200339c
.L_02002fe6:
	subs	r3, #1
	strh	r3, [r1, #0]
	ldr	r3, [pc, #148]
	movs	r5, #0
	ldrsh	r7, [r3, r5]
	mov	r9, r3
	cmp	r7, #0
	bne.n	.L_02003000
	ldr	r3, [pc, #128]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	mov	fp, r0
	b.n	.L_02003032
.L_02003000:
	ldr	r3, [pc, #120]
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r2, [pc, #124]
	ldr	r3, [pc, #108]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	ldrh	r5, [r2, #0]
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200c830
	adds	r6, r6, r0
	mov	fp, r6
	cmp	r5, r7
	blt.n	.L_02003032
	ldr	r3, [pc, #24]
	mov	r0, r9
	strh	r3, [r0, #0]
.L_02003032:
	ldr	r1, [pc, #92]
	movs	r2, #0
	ldrsh	r7, [r1, r2]
	mov	r9, r1
	cmp	r7, #0
	bne.n	.L_0200309c
	ldr	r3, [pc, #72]
	movs	r0, #0
	ldrsh	r5, [r3, r0]
	str	r5, [sp, #4]
	b.n	.L_020030ce
	.4byte 0x00000000
	.4byte 0x0200dc00
	.4byte 0x0200d9a4
	.4byte 0x03001b10
	.4byte 0x0200dbdc
	.4byte 0x0200dbe0
	.4byte 0x00007fff
	.4byte 0x0200dbb0
	.4byte 0x0200dc38
	.4byte 0x0200dbac
	.4byte 0x0200dc30
	.4byte 0x0200dbbc
	.4byte 0x0200dbb8
	.4byte 0x0200dba8
	.4byte 0x0200db94
	.4byte 0x0200dc3c
	.4byte 0x0200dbd4
	.4byte 0x0200dbd8
	.4byte 0x0200dbe8
	.4byte 0x0200dbc4
	.2byte 0xaee9
	.2byte 0x0200
.L_0200309c:
	ldr	r3, [pc, #72]
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	ldr	r3, [pc, #72]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	ldr	r2, [pc, #68]
	ldrh	r5, [r2, #0]
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200c830
	adds	r6, r6, r0
	str	r6, [sp, #4]
	cmp	r5, r7
	blt.n	.L_020030ce
	ldr	r3, [pc, #24]
	mov	r5, r9
	strh	r3, [r5, #0]
.L_020030ce:
	ldr	r0, [pc, #36]
	movs	r1, #0
	ldrsh	r7, [r0, r1]
	mov	r9, r0
	cmp	r7, #0
	bne.n	.L_020030fc
	ldr	r3, [pc, #28]
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	b.n	.L_0200312c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200dbd8
	.4byte 0x0200dbd4
	.4byte 0x0200dbc4
	.4byte 0x0200dbac
	.2byte 0xdc38
	.2byte 0x0200
.L_020030fc:
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #104]
	movs	r5, #0
	ldrsh	r6, [r3, r5]
	ldrh	r5, [r2, #0]
	ldr	r3, [pc, #100]
	adds	r5, #1
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200c830
	adds	r6, r6, r0
	cmp	r5, r7
	blt.n	.L_0200312c
	ldr	r3, [pc, #56]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200312c:
	add	r0, sp, #12
	ldr	r3, [r0, #4]
	ldr	r2, [pc, #60]
	ands	r3, r2
	str	r3, [r0, #4]
	mov	r3, fp
	lsls	r1, r3, #16
	ldr	r3, [sp, #12]
	lsrs	r1, r1, #16
	ands	r3, r2
	ldr	r2, [pc, #48]
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #12]
	bl 0x0200c8c0
	ldr	r2, [pc, #36]
	ldr	r3, [r2, #0]
	lsls	r0, r0, #16
	adds	r3, r3, r6
	asrs	r0, r0, #16
	str	r3, [r2, #0]
	b.n	.L_0200317c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200dbbc
	.4byte 0x0200dc30
	.4byte 0x0200dc38
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.2byte 0xdbb0
	.2byte 0x0200
.L_0200317c:
	cmp	r3, #0
	bge.n	.L_02003182
	adds	r3, #255
.L_02003182:
	asrs	r6, r3, #8
	ldr	r3, [pc, #552]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #2
	bne.n	.L_02003190
	b.n	.L_020032e8
.L_02003190:
	cmp	r3, #2
	bgt.n	.L_0200319a
	cmp	r3, #1
	beq.n	.L_020031a4
	b.n	.L_0200333a
.L_0200319a:
	cmp	r3, #3
	beq.n	.L_0200321a
	cmp	r3, #4
	beq.n	.L_02003296
	b.n	.L_0200333a
.L_020031a4:
	lsls	r0, r0, #25
	ldr	r4, [pc, #524]
	movs	r5, #0
	movs	r7, #56
	mov	r9, r0
.L_020031ae:
	lsls	r3, r5, #5
	subs	r3, #48
	mov	r0, fp
	muls	r0, r3
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L_020031be
	adds	r3, #255
.L_020031be:
	asrs	r3, r3, #8
	adds	r3, r6, r3
	ldr	r1, [pc, #500]
	adds	r2, r3, #0
	adds	r3, #152
	adds	r2, #88
	cmp	r3, r1
	bhi.n	.L_0200320e
	ldr	r3, [pc, #492]
	ldr	r1, [sp, #8]
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	orrs	r3, r4
	mov	r2, r9
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r0, r1, #0
	orrs	r3, r2
	str	r0, [sp, #8]
	stmia	r1!, {r3}
	movs	r3, #244
	adds	r0, r1, #0
	lsls	r3, r3, #8
	mov	r1, r8
	orrs	r3, r1
	stmia	r0!, {r3}
	movs	r1, #12
	adds	r2, r0, #0
	mov	r0, sl
	add	sl, r1
	movs	r1, #236
	str	r4, [sp, #0]
	str	r2, [sp, #8]
	bl 0x0200c8c8
	ldr	r4, [sp, #0]
.L_0200320e:
	movs	r2, #8
	adds	r5, #1
	add	r8, r2
	cmp	r5, #3
	ble.n	.L_020031ae
	b.n	.L_0200333a
.L_0200321a:
	lsls	r0, r0, #25
	ldr	r4, [pc, #404]
	movs	r5, #0
	movs	r7, #48
	mov	r9, r0
.L_02003224:
	lsls	r3, r5, #5
	subs	r3, #16
	mov	r0, fp
	muls	r0, r3
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L_02003234
	adds	r3, #255
.L_02003234:
	asrs	r3, r3, #8
	adds	r3, r6, r3
	ldr	r1, [pc, #380]
	adds	r2, r3, #0
	adds	r3, #152
	adds	r2, #88
	cmp	r3, r1
	bhi.n	.L_0200328a
	ldr	r3, [pc, #372]
	ldr	r1, [sp, #8]
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	orrs	r3, r4
	mov	r2, r9
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r0, r1, #0
	orrs	r3, r2
	str	r0, [sp, #8]
.L_02003262:
	stmia	r1!, {r3}
	ldr	r3, [pc, #344]
	adds	r0, r1, #0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #244
	lsls	r2, r2, #8
	add	r3, r8
	orrs	r3, r2
	stmia	r0!, {r3}
	movs	r1, #12
	adds	r2, r0, #0
	mov	r0, sl
	add	sl, r1
	movs	r1, #236
	str	r4, [sp, #0]
	str	r2, [sp, #8]
	bl 0x0200c8c8
	ldr	r4, [sp, #0]
.L_0200328a:
	movs	r2, #8
	adds	r5, #1
	add	r8, r2
	cmp	r5, #1
	ble.n	.L_02003224
	b.n	.L_0200333a
.L_02003296:
	adds	r3, r6, #0
	movs	r5, #152
	adds	r2, r6, #0
	adds	r3, #120
	lsls	r5, r5, #1
	movs	r7, #48
	ldr	r4, [pc, #288]
	adds	r2, #56
	cmp	r3, r5
	bcs.n	.L_0200333a
	ldr	r3, [pc, #272]
	mov	r1, sl
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	lsls	r2, r0, #25
	orrs	r3, r4
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r5, r1, #0
	orrs	r3, r2
	str	r5, [sp, #8]
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #240]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	movs	r2, #244
	add	r3, r8
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [r1, #0]
	mov	r0, sl
	movs	r1, #236
	bl 0x0200c8c8
	b.n	.L_0200333a
.L_020032e8:
	adds	r3, r6, #0
	movs	r1, #152
	movs	r4, #128
	adds	r2, r6, #0
	adds	r3, #152
	lsls	r1, r1, #1
	movs	r7, #48
	lsls	r4, r4, #24
	adds	r2, #88
	cmp	r3, r1
	bcs.n	.L_0200333a
	ldr	r3, [pc, #188]
	mov	r5, sl
	ands	r2, r3
	movs	r3, #0
	stmia	r5!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	lsls	r2, r0, #25
	orrs	r3, r4
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r5, #0
	orrs	r3, r2
	str	r1, [sp, #8]
	stmia	r5!, {r3}
	adds	r0, r5, #0
	str	r0, [sp, #8]
	ldr	r3, [pc, #156]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #244
	add	r3, r8
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [r5, #0]
	mov	r0, sl
	movs	r1, #236
	bl 0x0200c8c8
.L_0200333a:
	ldr	r0, [pc, #140]
	ldr	r1, [pc, #140]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02003368
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, #4
	lsls	r2, r2, #6
	stmia	r3!, {r2}
	ldr	r2, [pc, #112]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02003368:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_0200339a
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r0, #0]
	ldr	r5, [sp, #4]
	movs	r3, #16
	lsls	r2, r2, #2
	subs	r3, r3, r5
	adds	r2, r2, r0
	lsls	r3, r3, #8
	adds	r2, #4
	orrs	r3, r5
	stmia	r2!, {r3}
	ldr	r3, [pc, #64]
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_0200339a:
	strh	r4, [r1, #0]
.L_0200339c:
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
	.4byte 0x0200dbd0
	.4byte 0x80004000
	.4byte 0x0000012f
	.4byte 0x000001ff
	.4byte 0x0200dba4
	.4byte 0xc0004000
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000050
	.2byte 0x0052
	.2byte 0x0400
	.global Func_020033d8
	.thumb_func
Func_020033d8:
	push {r5, r6, lr}
	ldr r3, [pc, #48]
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r2, [pc, #44]
	strh r5, [r3]
	movs r1, #200
	lsls r3, r6, #4
	lsls r1, r1, #4
	strh r3, [r2]
	ldr r0, [pc, #36]
	bl 0x0200c848
	ldr r1, [pc, #36]
	cmp r5, #2
	bne .L_020033d8_0
	ldr r1, [pc, #32]
.L_020033d8_0:
	cmp r5, #4
	bne .L_020033d8_1
	ldr r1, [pc, #32]
.L_020033d8_1:
	cmp r5, #3
	bne .L_020033d8_2
	cmp r6, #0
	beq .L_020033d8_3
	ldr r1, [pc, #24]
	b .L_020033d8_2
	.4byte 0x0200dbd0
	.4byte 0x0200dba4
	.4byte 0x0200aee9
	.4byte 0x0200d9a6
	.4byte 0x0200ccba
	.4byte 0x0200d9d2
	.4byte 0x0200cce2
.L_020033d8_3:
	ldr r1, [pc, #28]
.L_020033d8_2:
	ldr r2, [pc, #24]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r3, [pc, #28]
	str r1, [r3]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r2, [pc, #28]
	movs r3, #0
	str r3, [r2]
	b .L_020033d8_4
	.4byte 0x00000000
	.4byte 0x0200da50
	.4byte 0x0200dbdc
	.4byte 0x0200dbe0
	.4byte 0x0200dc38
	.4byte 0x0200dbac
	.4byte 0x0200dbb0
.L_020033d8_4:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003468
	.thumb_func
Func_02003468:
	push {r5, lr}
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02003468_0
	bl 0x0200ca00
	bl 0x0200cb50
	bl 0x0200cb60
	movs r0, #30
	bl 0x0200c9f8
	movs r0, #89
	bl 0x0200cba0
	movs r0, #0
	bl 0x0200ae54
	movs r0, #1
	movs r1, #0
	bl 0x0200b3d8
	movs r0, #120
	bl 0x0200c9f8
	bl 0x0200ca08
	b .L_02003468_1
.L_02003468_0:
	movs r0, #247
	bl 0x0200cba0
	bl 0x0200ca00
	bl 0x0200cb50
	bl 0x0200cb60
	lsls r2, r5, #4
	ldr r3, [pc, #176]
	subs r2, r2, r5
	lsls r2, r2, #2
	strh r2, [r3, #30]
	movs r0, #30
	bl 0x0200c9f8
	adds r0, r5, #0
	adds r0, #90
	bl 0x0200cba0
	adds r0, r5, #0
	bl 0x0200ae54
	movs r0, #1
	movs r1, #0
	bl 0x0200b3d8
	movs r0, #120
	bl 0x0200c9f8
	b .L_02003468_2
.L_02003468_3:
	movs r0, #1
	bl 0x0200c840
.L_02003468_2:
	bl 0x0200cba8
	cmp r0, #0
	bne .L_02003468_3
	ldr r0, [pc, #120]
	bl 0x0200cba0
	movs r0, #5
	bl 0x0200ae54
	movs r1, #0
	movs r0, #2
	bl 0x0200b3d8
	movs r0, #236
	bl 0x0200cba0
	movs r0, #60
	bl 0x0200c9f8
	movs r1, #1
	movs r0, #2
	bl 0x0200b3d8
	movs r0, #236
	bl 0x0200cba0
	movs r0, #60
	bl 0x0200c9f8
	movs r0, #6
	bl 0x0200ae54
	movs r1, #0
	movs r0, #2
	bl 0x0200b3d8
	movs r0, #236
	bl 0x0200cba0
	movs r0, #60
	bl 0x0200c9f8
	movs r0, #7
	bl 0x0200ae54
	movs r1, #0
	movs r0, #4
	bl 0x0200b3d8
	movs r0, #237
	bl 0x0200cba0
	bl 0x0200cb78
	bl 0x0200ca08
	ldr r0, [pc, #20]
	bl 0x0200c9b8
.L_02003468_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200d9a6
	.4byte 0x00000121
	.4byte 0x00000123
	.global Func_02003574
	.thumb_func
Func_02003574:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r0, #247
	bl 0x0200cba0
	bl 0x0200cb50
	bl 0x0200cb60
	lsls r3, r5, #4
	ldr r2, [pc, #164]
	subs r3, r3, r5
	lsls r6, r3, #2
	strh r6, [r2, #26]
	ldr r1, [pc, #160]
	adds r2, r5, #0
	cmp r5, #0
	bge .L_02003574_0
	negs r2, r5
.L_02003574_0:
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	strh r3, [r1, #26]
	cmp r5, #0
	bge .L_02003574_1
	movs r0, #30
	bl 0x0200c9f8
	movs r0, #86
	bl 0x0200cba0
	movs r0, #8
	bl 0x0200ae54
	movs r1, #1
	movs r0, #3
	bl 0x0200b3d8
	lsls r0, r5, #4
	subs r0, r5, r0
	lsls r0, r0, #2
	adds r0, #60
	bl 0x0200c9f8
	movs r0, #0
	b .L_02003574_2
.L_02003574_1:
	movs r0, #30
	bl 0x0200c9f8
	adds r0, r5, #0
	adds r0, #90
	bl 0x0200cba0
	movs r0, #4
	bl 0x0200ae54
	movs r1, #0
	movs r0, #3
	bl 0x0200b3d8
	adds r0, r6, #0
	adds r0, #60
	bl 0x0200c9f8
	movs r0, #8
.L_02003574_2:
	ldr r1, [pc, #64]
	movs r2, #0
	bl 0x0200cae0
	b .L_02003574_3
.L_02003574_4:
	movs r0, #1
	bl 0x0200c840
.L_02003574_3:
	bl 0x0200cba8
	cmp r0, #0
	bne .L_02003574_4
	movs r0, #19
	bl 0x0200cba0
	movs r0, #30
	bl 0x0200c9f8
	ldr r0, [pc, #32]
	bl 0x0200cba0
	bl 0x0200cb58
	bl 0x0200cb60
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200da50
	.4byte 0x0200cce2
	.4byte 0x00000105
	.4byte 0x00000121
	.global Func_02003640
	.thumb_func
Func_02003640:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #428]
	sub sp, #4
	ldr r6, [r3]
	mov r11, r0
	mov r8, r1
	mov r10, r2
	bl 0x0200ca18
	movs r3, #1
	strb r3, [r6, #6]
	movs r3, #4
	adds r7, r0, #0
	strb r3, [r6, #7]
	ldr r3, [r7, #8]
	ldr r2, [pc, #404]
	str r3, [r2]
	ldr r3, [r7, #16]
	ldr r2, [pc, #400]
	str r3, [r2]
	ldr r0, [r7, #80]
	ldrh r3, [r7, #6]
	ldr r2, [pc, #396]
	mov r9, r0
	str r3, [r2]
	mov r0, r11
	movs r1, #2
	bl 0x0200cad8
	adds r2, r7, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r5, #1
	orrs r5, r3
	strb r5, [r2]
	movs r5, #128
	lsls r5, r5, #7
	adds r0, r7, #0
	strh r5, [r7, #6]
	movs r1, #3
	bl 0x0200c958
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200c8e0
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200c8e0
	mov r3, r10
	lsls r3, r3, #16
	mov r10, r3
	mov r1, r8
	lsls r1, r1, #16
	mov r0, r11
	mov r2, r10
	mov r8, r1
	bl 0x0200ca68
	movs r0, #0
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200ca98
	ldr r4, [pc, #316]
	ldr r6, [pc, #316]
	ldrh r3, [r6]
	adds r1, r3, #0
	strh r6, [r6]
	ldrh r2, [r4]
	cmp r2, #31
	bgt .L_02003640_0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r4
	strh r2, [r4]
	movs r2, #240
	adds r3, #4
	lsls r2, r2, #4
	stmia r3!, {r2}
	ldr r2, [pc, #288]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02003640_0:
	strh r1, [r6]
	mov r0, r9
	movs r2, #13
	ldrb r1, [r0, #5]
	negs r2, r2
	adds r3, r2, #0
	ands r3, r1
	movs r1, #4
	orrs r3, r1
	strb r3, [r0, #5]
	ldrb r3, [r0, #17]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0, #17]
	movs r0, #252
	str r4, [sp, #0]
	bl 0x0200cba0
	ldr r4, [sp, #0]
	movs r5, #0
.L_02003640_2:
	movs r1, #128
	lsls r2, r5, #12
	lsls r1, r1, #5
	adds r3, r2, r1
	str r3, [r7, #24]
	movs r3, #248
	lsls r3, r3, #9
	subs r3, r3, r2
	str r3, [r7, #28]
	ldrh r3, [r6]
	adds r0, r3, #0
	strh r6, [r6]
	ldrh r3, [r4]
	cmp r3, #31
	bgt .L_02003640_1
	lsls r1, r3, #1
	adds r1, r1, r3
	adds r3, #1
	strh r3, [r4]
	movs r3, #15
	lsls r1, r1, #2
	subs r3, r3, r5
	adds r1, r4, r1
	lsls r3, r3, #8
	adds r2, r5, #1
	adds r1, #4
	orrs r3, r2
	stmia r1!, {r3}
	ldr r3, [pc, #184]
	stmia r1!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r1]
.L_02003640_1:
	strh r0, [r6]
	movs r0, #1
	str r4, [sp, #0]
	bl 0x0200c840
	adds r5, #2
	ldr r4, [sp, #0]
	cmp r5, #15
	ble .L_02003640_2
	ldr r1, [pc, #144]
	ldr r0, [pc, #148]
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02003640_3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	movs r2, #16
	stmia r3!, {r2}
	ldr r2, [pc, #124]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02003640_3:
	strh r4, [r0]
	movs r3, #136
	lsls r3, r3, #9
	str r3, [r7, #24]
	movs r3, #240
	lsls r3, r3, #8
	str r3, [r7, #28]
	movs r0, #1
	bl 0x0200c9f8
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r0, #13
	bl 0x0200c9f8
	mov r3, r9
	movs r2, #13
	ldrb r1, [r3, #5]
	negs r2, r2
	adds r3, r2, #0
	mov r0, r9
	ands r3, r1
	strb r3, [r0, #5]
	ldrb r3, [r0, #17]
	ands r2, r3
	strb r2, [r0, #17]
	movs r1, #3
	mov r0, r11
	bl 0x0200ca78
	movs r0, #20
	bl 0x0200c9f8
	sub sp, #-4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e68
	.4byte 0x0200dc44
	.4byte 0x0200db9c
	.4byte 0x0200dbc8
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000050
	.4byte 0x04000052
	.global Func_0200381c
	.thumb_func
Func_0200381c:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #40]
	adds r6, r0, #0
	ldr r7, [r3]
	bl 0x0200ca18
	ldr r3, [pc, #32]
	movs r1, #249
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrb r3, [r2]
	adds r5, r0, #0
	cmp r3, #1
	bne .L_0200381c_0
	movs r3, #0
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200ca70
	b .L_0200381c_1
	.2byte 0x0000
	.4byte 0x03001e68
	.4byte 0x02000240
.L_0200381c_0:
	movs r1, #128
	adds r0, r6, #0
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200cad0
	adds r0, r6, #0
	movs r1, #3
	bl 0x0200ca70
	movs r0, #30
	bl 0x0200c9f8
.L_0200381c_1:
	movs r2, #0
	movs r3, #15
	strb r2, [r7, #7]
	strb r3, [r7, #6]
	ldr r3, [pc, #84]
	ldr r3, [r3]
	str r3, [r5, #8]
	ldr r3, [pc, #80]
	ldr r3, [r3]
	str r3, [r5, #16]
	ldr r3, [pc, #80]
	ldr r3, [r3]
	strh r3, [r5, #6]
	movs r3, #128
	lsls r3, r3, #24
	adds r0, r5, #0
	str r3, [r5, #56]
	str r3, [r5, #64]
	adds r0, #85
	movs r3, #3
	str r2, [r5, #36]
	str r2, [r5, #44]
	ldr r1, [pc, #44]
	strb r3, [r0]
	adds r3, r5, #0
	adds r3, #34
	strb r1, [r3]
	adds r0, r5, #0
	str r2, [r5, #12]
	str r2, [r5, #20]
	movs r1, #1
	bl 0x0200c958
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c8e0
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c8e0
	movs r0, #1
	bl 0x0200c840
	b .L_0200381c_2
	.4byte 0x00000000
	.4byte 0x0200dc44
	.4byte 0x0200db9c
	.4byte 0x0200dbc8
.L_0200381c_2:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020038dc
	.thumb_func
Func_020038dc:
	push {r5, lr}
	bl 0x0200ca18
	adds r5, r0, #0
	bl 0x0200c900
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	pop {r5}
	pop {r0}
	bx r0
	.global Func_020038fc
	.thumb_func
Func_020038fc:
	push {r5, lr}
	ldr r5, [pc, #24]
	movs r2, #0
	ldrsh r3, [r5, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_020038fc_0
	bl 0x0200c990
	strh r0, [r5]
.L_020038fc_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200dace
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #180]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #176]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	ldr	r2, [pc, #172]
	lsrs	r3, r3, #5
	ldr	r0, [pc, #172]
	mov	sl, r3
	movs	r3, #0
	ldrsh	r7, [r2, r3]
	mov	fp, r0
	mov	r9, r2
	cmp	r7, #0
	beq.n	.L_020039b0
	ldr	r3, [pc, #160]
	ldrh	r5, [r3, #0]
	adds	r5, #1
	strh	r5, [r3, #0]
	ldr	r0, [pc, #156]
	ldr	r1, [pc, #160]
	ldr	r3, [pc, #160]
	mov	r8, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	lsls	r5, r5, #16
	subs	r2, r2, r3
	asrs	r5, r5, #16
	ldrh	r6, [r1, #0]
	adds	r0, r5, #0
	muls	r0, r2
	adds	r1, r7, #0
	bl 0x0200c830
	ldr	r2, [pc, #136]
	adds	r6, r6, r0
	mov	r1, r8
	strh	r6, [r1, #0]
	mov	r8, r2
	ldr	r3, [pc, #128]
	ldr	r2, [pc, #132]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	ldrh	r6, [r2, #0]
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	subs	r3, r3, r2
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200c830
	mov	r2, r8
	adds	r6, r6, r0
	strh	r6, [r2, #0]
	cmp	r5, r7
	blt.n	.L_020039aa
	ldr	r3, [pc, #52]
	mov	r0, r9
	strh	r3, [r0, #0]
.L_020039aa:
	ldr	r2, [pc, #96]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
.L_020039b0:
	ldr	r2, [pc, #88]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #13
	bgt.n	.L_02003a3c
	ldr	r3, [pc, #48]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #56]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	mov	r0, fp
	movs	r3, #0
	stmia	r0!, {r3}
	subs	r1, #8
	ldr	r3, [pc, #56]
	lsls	r1, r1, #16
	b.n	.L_02003a14
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200dace
	.4byte 0x03001b10
	.4byte 0x0200dbcc
	.4byte 0x0200dbf0
	.4byte 0x0200db90
	.4byte 0x0200dc34
	.4byte 0x0200dbe4
	.4byte 0x0200dba0
	.4byte 0x0200dbc0
	.4byte 0x0200dc40
	.4byte 0x0200dbfc
	.4byte 0x0200dbb4
	.2byte 0xdb98
	.2byte 0x0200
.L_02003a14:
	subs	r2, #8
	orrs	r2, r1
	movs	r4, #128
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r4, r4, #23
	lsls	r3, r3, #28
	orrs	r2, r4
	orrs	r2, r3
	movs	r3, #128
	stmia	r0!, {r2}
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r0, #0]
	movs	r1, #255
	mov	r0, fp
	bl 0x0200c8c8
	b.n	.L_02003a44
.L_02003a3c:
	cmp	r3, #19
	ble.n	.L_02003a44
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
.L_02003a44:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0x0000
	.global Func_02003a58
	.thumb_func
Func_02003a58:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	mov r8, r1
	adds r5, r2, #0
	bl 0x0200b8fc
	ldr r3, [pc, #44]
	strh r6, [r3]
	ldr r3, [pc, #44]
	mov r2, r8
	strh r2, [r3]
	ldr r3, [pc, #28]
	ldr r2, [pc, #40]
	ands r5, r3
	strh r5, [r2]
	ldr r3, [pc, #40]
	ldr r2, [pc, #20]
	strh r2, [r3]
	ldr r3, [pc, #36]
	movs r1, #200
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #32]
	bl 0x0200c848
	b .L_02003a58_0
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0200dc34
	.4byte 0x0200dbc0
	.4byte 0x0200db98
	.4byte 0x0200dbb4
	.4byte 0x0200dbcc
	.4byte 0x0200b91d
.L_02003a58_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003abc
	.thumb_func
Func_02003abc:
	push {lr}
	ldr r3, [pc, #52]
	strh r0, [r3]
	ldr r3, [pc, #52]
	strh r1, [r3]
	ldr r3, [pc, #52]
	ldr r1, [pc, #52]
	ldrh r3, [r3]
	strh r3, [r1]
	ldr r3, [pc, #52]
	ldr r1, [pc, #52]
	ldrh r3, [r3]
	strh r3, [r1]
	ldr r3, [pc, #52]
	strh r2, [r3]
	ldr r2, [pc, #52]
	ldr r3, [pc, #16]
	movs r1, #200
	strh r3, [r2]
	lsls r1, r1, #4
	ldr r0, [pc, #44]
	bl 0x0200c848
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200dba0
	.4byte 0x0200dc40
	.4byte 0x0200dc34
	.4byte 0x0200dbe4
	.4byte 0x0200dbc0
	.4byte 0x0200dbfc
	.4byte 0x0200dbcc
	.4byte 0x0200db90
	.4byte 0x0200b91d
	.global Func_02003b18
	.thumb_func
Func_02003b18:
	push {r5, lr}
	ldr r0, [pc, #28]
	bl 0x0200c850
	ldr r5, [pc, #24]
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl 0x0200c8a0
	ldr r3, [pc, #8]
	strh r3, [r5]
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffffffff
	.4byte 0x0200b91d
	.4byte 0x0200dace
	.global Func_02003b40
	.thumb_func
Func_02003b40:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r2, #0
	bl 0x0200cb68
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02003b40_0
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	asrs r3, r3, #1
	str r3, [r5, #52]
	adds r3, r5, #0
	movs r2, #0
	adds r3, #91
	strb r2, [r3]
	bl 0x0200c900
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200c8e0
	lsls r1, r6, #16
	ldr r2, [r5, #12]
	lsls r3, r7, #16
	adds r0, r5, #0
	bl 0x0200c910
.L_02003b40_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02003b80
	.thumb_func
Func_02003b80:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r2, #0
	bl 0x0200cb68
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02003b80_0
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #48]
	asrs r3, r3, #1
	str r3, [r5, #52]
	adds r3, r5, #0
	movs r2, #0
	adds r3, #91
	strb r2, [r3]
	bl 0x0200c900
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200c8e0
	lsls r1, r6, #16
	ldr r2, [r5, #12]
	lsls r3, r7, #16
	adds r0, r5, #0
	bl 0x0200c910
	adds r0, r5, #0
	bl 0x0200c918
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c8e0
.L_02003b80_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02003bd0
	.thumb_func
Func_02003bd0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r9, r1
	bl 0x0200ca18
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02003bd0_0
	adds r3, r6, #0
	adds r3, #84
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #1
	bne .L_02003bd0_0
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #17
	ldr r7, [r6, #80]
	bl 0x0200c870
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #3
	movs r2, #0
	adds r5, r5, r3
	mov r0, sp
	mov r8, r2
	str r2, [r0]
	ldr r3, [pc, #100]
	adds r1, r5, #0
	ldr r2, [pc, #100]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r9
	bl 0x0200c980
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r7, #28]
	bl 0x0200c8b0
	adds r5, r0, #0
	movs r0, #17
	bl 0x0200c880
	adds r3, r6, #0
	adds r3, #92
	mov r2, r10
	strb r2, [r3]
	ldr r0, [r7, #40]
	bl 0x0200c8d8
	mov r3, r8
	str r3, [r7, #40]
	adds r3, r7, #0
	adds r3, #39
	mov r2, r8
	strb r2, [r3]
	ldrb r2, [r7, #5]
	movs r3, #33
	negs r3, r3
	ands r3, r2
	strb r3, [r7, #5]
	ldr r3, [pc, #28]
	ldrh r2, [r7, #8]
	ands r5, r3
	ldr r3, [pc, #32]
	ands r3, r2
	orrs r3, r5
	strh r3, [r7, #8]
	adds r3, r7, #0
	adds r3, #37
	mov r2, r8
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	b .L_02003bd0_0
	.2byte 0x0000
	.4byte 0x000003ff
	.4byte 0x040000d4
	.4byte 0x85000020
	.4byte 0xfffffc00
.L_02003bd0_0:
	sub sp, #-4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02003c94
	.thumb_func
Func_02003c94:
	push {r5, lr}
	adds r0, #8
	movs r3, #0
	ldr r5, [pc, #32]
	strb r3, [r0]
	movs r2, #7
	subs r0, #1
	movs r4, #15
.L_02003c94_0:
	adds r3, r1, #0
	ands r3, r4
	ldrb r3, [r5, r3]
	subs r2, #1
	strb r3, [r0]
	lsrs r1, r1, #4
	subs r0, #1
	cmp r2, #0
	bge .L_02003c94_0
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200ce3c
	.global Func_02003cc0
	.thumb_func
Func_02003cc0:
	bx lr
	.2byte 0x0000
	.global Func_02003cc4
	.thumb_func
Func_02003cc4:
	ldr r2, [pc, #4]
	movs r3, #9
	strh r3, [r2]
	bx lr
	.4byte 0x02001000
	.global Func_02003cd0
	.thumb_func
Func_02003cd0:
	push {r5, lr}
	ldr r5, [pc, #28]
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #9
	beq .L_02003cd0_0
.L_02003cd0_1:
	movs r0, #1
	bl 0x0200c840
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #9
	bne .L_02003cd0_1
.L_02003cd0_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02001000
	.global Func_02003cf4
	.thumb_func
Func_02003cf4:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r3, [r5, #40]
	movs r2, #255
	adds r3, #255
	lsls r2, r2, #1
	sub sp, #12
	cmp r3, r2
	bhi .L_02003cf4_0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02003cf4_0:
	bl 0x0200c858
	movs r3, #100
	muls r3, r0
	lsrs r3, r3, #16
	cmp r3, #9
	bhi .L_02003cf4_1
	ldr r3, [r5, #8]
	mov r6, sp
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl 0x0200c858
	adds r5, r0, #0
	bl 0x0200c858
	lsls r5, r5, #4
	adds r1, r0, #0
	adds r2, r6, #0
	adds r0, r5, #0
	bl 0x0200c868
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	ldr r0, [pc, #56]
	bl 0x0200c8f0
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02003cf4_1
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #0
	bl 0x0200c958
	ldr r1, [pc, #32]
	adds r0, r5, #0
	bl 0x0200c8e8
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c8e0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c8e0
.L_02003cf4_1:
	sub sp, #-12
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0000011d
	.4byte 0x0200ce50
	.global Func_02003d88
	.thumb_func
Func_02003d88:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	movs r2, #100
	adds r2, r2, r5
	movs r3, #0
	ldrsh r0, [r2, r3]
	mov r8, r2
	bl 0x0200ca18
	ldr r2, [r5, #12]
	movs r3, #144
	lsls r3, r3, #14
	adds r2, r2, r3
	adds r6, r0, #0
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	bl 0x0200c910
	adds r3, r6, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	ldr r1, [pc, #28]
	adds r0, r6, #0
	bl 0x0200c8e8
	movs r0, #83
	bl 0x0200cba0
	mov r2, r8
	strh r5, [r2]
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0200ce74
	.global Func_02003ddc
	.thumb_func
Func_02003ddc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #236]
	mov r10, r0
	ldr r0, [pc, #236]
	mov r8, r1
	ldr r5, [r3]
	bl 0x0200c9b0
	ldr r3, [pc, #232]
	adds r7, r0, #0
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	bl 0x0200ca18
	adds r3, r5, #0
	adds r6, r0, #0
	adds r3, #232
	ldr r2, [r3]
	movs r0, #192
	ldr r3, [r6, #8]
	lsls r0, r0, #12
	adds r1, r2, r0
	cmp r2, r3
	blt .L_02003ddc_0
	ldr r3, [pc, #200]
	adds r1, r2, r3
.L_02003ddc_0:
	cmp r7, #0
	beq .L_02003ddc_1
	adds r3, r5, #0
	adds r3, #236
	ldr r3, [r3]
	movs r0, #128
	lsls r0, r0, #13
	adds r4, r3, r0
	adds r3, r5, #0
	adds r3, #228
	b .L_02003ddc_2
.L_02003ddc_1:
	adds r3, r5, #0
	adds r3, #236
	ldr r3, [r3]
	ldr r2, [pc, #172]
	adds r4, r3, r2
	adds r3, r5, #0
	adds r3, #226
.L_02003ddc_2:
	ldrh r3, [r3]
	adds r5, r6, #0
	adds r5, #100
	strh r3, [r5]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #9
	movs r2, #0
	str r3, [r6, #48]
	adds r0, r6, #0
	adds r3, r4, #0
	bl 0x0200c910
	ldr r0, [pc, #120]
	bl 0x0200c9b8
	adds r0, r6, #0
	ldr r1, [pc, #128]
	bl 0x0200c8e8
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #0
	beq .L_02003ddc_3
.L_02003ddc_4:
	movs r0, #1
	bl 0x0200c840
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_02003ddc_4
.L_02003ddc_3:
	cmp r7, #0
	bne .L_02003ddc_5
	mov r1, r10
	movs r0, #0
	bl 0x0200ae18
	mov r0, r10
	movs r1, #2
	bl 0x0200c978
	b .L_02003ddc_6
.L_02003ddc_5:
	mov r1, r8
	movs r0, #0
	bl 0x0200ae18
	mov r0, r8
	movs r1, #2
	bl 0x0200c978
.L_02003ddc_6:
	ldr r3, [pc, #52]
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl 0x0200c978
	movs r1, #3
	ldr r0, [pc, #48]
	bl 0x0200c970
	adds r0, r6, #0
	bl 0x0200c908
	adds r0, r7, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001f3c
	.4byte 0x00000211
	.4byte 0x02000240
	.4byte 0xfff40000
	.4byte 0xfff00000
	.4byte 0x0200db24
	.4byte 0x0000096a
	.global Func_02003ef0
	.thumb_func
Func_02003ef0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r1, [pc, #332]
	ldr r3, [pc, #332]
	movs r2, #4
	ldrsh r0, [r1, r2]
	sub sp, #4
	ldr r6, [r3]
	mov r8, r1
	bl 0x0200ca18
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02003ef0_0
	b .L_02003ef0_1
.L_02003ef0_0:
	mov r2, r8
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #1
	beq .L_02003ef0_2
	b .L_02003ef0_3
.L_02003ef0_2:
	ldrh r3, [r2, #6]
	adds r2, r3, #1
	lsls r3, r3, #16
	asrs r3, r3, #15
	adds r3, #240
	ldrsh r7, [r6, r3]
	adds r3, r2, #1
	lsls r2, r2, #16
	asrs r2, r2, #15
	mov r1, r8
	adds r2, #240
	strh r3, [r1, #6]
	ldrsh r4, [r6, r2]
	cmp r7, #0
	bne .L_02003ef0_4
	cmp r4, #0
	bne .L_02003ef0_4
	movs r3, #9
	strh r3, [r1]
	movs r1, #1
	bl 0x0200c8e0
	adds r3, r6, #0
	adds r3, #232
	ldr r2, [r3]
	movs r1, #192
	ldr r3, [r5, #8]
	lsls r1, r1, #12
	adds r7, r2, r1
	cmp r2, r3
	blt .L_02003ef0_5
	ldr r3, [pc, #240]
	adds r7, r2, r3
.L_02003ef0_5:
	ldr r0, [pc, #240]
	bl 0x0200c9b0
	cmp r0, #0
	beq .L_02003ef0_6
	adds r3, r6, #0
	adds r3, #236
	ldr r3, [r3]
	movs r1, #128
	lsls r1, r1, #13
	adds r4, r3, r1
	adds r3, r6, #0
	adds r3, #228
	b .L_02003ef0_7
.L_02003ef0_6:
	adds r3, r6, #0
	adds r3, #236
	ldr r3, [r3]
	ldr r2, [pc, #208]
	adds r4, r3, r2
	adds r3, r6, #0
	adds r3, #226
.L_02003ef0_7:
	ldrh r2, [r3]
	adds r3, r5, #0
	adds r3, #100
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #9
	adds r1, r7, #0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r2, #0
	adds r3, r4, #0
	bl 0x0200c910
	ldr r0, [pc, #164]
	bl 0x0200c9b8
	ldr r1, [pc, #168]
	adds r0, r5, #0
	bl 0x0200c8e8
	b .L_02003ef0_1
.L_02003ef0_4:
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	lsls r7, r7, #16
	lsls r4, r4, #16
	cmp r3, #0
	beq .L_02003ef0_8
	adds r3, r6, #0
	adds r3, #232
	ldr r3, [r3]
	lsls r3, r3, #1
	subs r7, r3, r7
.L_02003ef0_8:
	ldr r2, [r5, #8]
	cmp r2, r7
	bne .L_02003ef0_9
	ldr r3, [r5, #16]
	cmp r3, r4
	beq .L_02003ef0_10
	b .L_02003ef0_11
.L_02003ef0_9:
	ldr r3, [r5, #16]
.L_02003ef0_11:
	subs r0, r4, r3
	subs r1, r7, r2
	str r4, [sp, #0]
	bl 0x0200c860
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	movs r2, #128
	asrs r0, r0, #16
	lsls r2, r2, #5
	ldr r4, [sp, #0]
	cmp r0, r2
	ble .L_02003ef0_12
	adds r0, r2, #0
.L_02003ef0_12:
	ldr r2, [pc, #88]
	cmp r0, r2
	bge .L_02003ef0_13
	adds r0, r2, #0
.L_02003ef0_13:
	adds r3, r3, r0
	movs r2, #0
	strh r3, [r5, #6]
	mov r3, r8
	str r7, [r5, #8]
	str r4, [r5, #16]
	strh r2, [r3, #8]
	b .L_02003ef0_14
.L_02003ef0_10:
	mov r1, r8
	ldrh r3, [r1, #8]
	mov r2, r8
	adds r3, #1
	strh r3, [r2, #8]
.L_02003ef0_14:
	mov r2, r8
	movs r1, #8
	ldrsh r3, [r2, r1]
	cmp r3, #2
	ble .L_02003ef0_15
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c8e0
	b .L_02003ef0_1
.L_02003ef0_15:
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200c8e0
	b .L_02003ef0_1
	.2byte 0x0000
	.4byte 0x02001000
	.4byte 0x03001f3c
	.4byte 0xfff40000
	.4byte 0x00000211
	.4byte 0xfff00000
	.4byte 0x0200dad0
	.4byte 0xfffff000
.L_02003ef0_3:
	cmp r3, #2
	bne .L_02003ef0_1
	mov r2, r8
	movs r3, #10
	ldrsh r7, [r5, r3]
	ldrh r3, [r2, #6]
	adds r2, r3, #1
	lsls r3, r3, #16
	asrs r3, r3, #15
	movs r1, #18
	ldrsh r4, [r5, r1]
	adds r3, #240
	adds r1, r2, #1
	lsls r2, r2, #16
	strh r7, [r6, r3]
	asrs r2, r2, #15
	mov r3, r8
	adds r2, #240
	strh r1, [r3, #6]
	lsls r3, r1, #16
	strh r4, [r6, r2]
	asrs r2, r3, #16
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_02003ef0_1
	adds r3, r1, #1
	lsls r3, r3, #16
	ldr r1, [pc, #36]
	lsls r2, r2, #1
	asrs r3, r3, #15
	adds r2, #240
	adds r3, #240
	strh r1, [r6, r2]
	strh r1, [r6, r3]
	adds r3, r6, #0
	adds r3, #224
	ldrh r3, [r3]
	mov r1, r8
	strh r3, [r1, #4]
	movs r2, #0
	mov r3, r8
	strh r2, [r3, #6]
	movs r3, #1
	strh r3, [r1]
	b .L_02003ef0_1
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0000383e
.L_02003ef0_1:
	sub sp, #-4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #916]
	ldr	r3, [r3, #0]
	adds	r1, r3, #0
	sub	sp, #20
	mov	r8, r1
	str	r3, [sp, #12]
	str	r3, [sp, #16]
	mov	r7, r8
	adds	r7, #216
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	ldr	r2, [pc, #896]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	str	r3, [sp, #8]
	mov	r3, r8
	adds	r3, #230
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	subs	r3, #10
	mov	fp, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0200411e
	mov	r6, r8
	adds	r6, #218
	movs	r3, #2
	strh	r3, [r6, #0]
	b.n	.L_0200418c
.L_0200411e:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200c9b0
	cmp	r0, #0
	beq.n	.L_0200413e
	mov	r6, r8
	adds	r6, #218
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #0]
	cmp	r3, #0
	ble.n	.L_0200418c
	subs	r3, r2, #1
	strh	r3, [r6, #0]
	b.n	.L_0200418c
.L_0200413e:
	mov	r6, r8
	adds	r6, #218
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #0]
	cmp	r3, #1
	bgt.n	.L_0200418c
	adds	r3, r2, #1
	movs	r2, #128
	strh	r3, [r6, #0]
	lsls	r2, r2, #9
	lsls	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_0200418c
	ldr	r3, [pc, #800]
	ldr	r0, [pc, #800]
	ldr	r1, [pc, #804]
	ldr	r2, [pc, #804]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200c888
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #792]
	bl 0x0200c898
	movs	r1, #128
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	lsls	r1, r1, #2
	adds	r2, r5, #0
	bl 0x0200c8b0
	adds	r0, r5, #0
	bl 0x0200c890
.L_0200418c:
	movs	r1, #0
	ldrsh	r2, [r6, r1]
	cmp	r2, #0
	bne.n	.L_020041a2
	ldr	r3, [sp, #12]
	adds	r3, #216
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200c8a8
	b.n	.L_02004462
.L_020041a2:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r7, r3, #0
	subs	r7, #8
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	lsls	r3, r3, #4
	str	r3, [sp, #4]
	movs	r2, #128
	ldr	r1, [sp, #4]
	lsls	r2, r2, #8
	movs	r3, #104
	mov	r9, r2
	ldr	r2, [sp, #12]
	subs	r4, r3, r1
	movs	r3, #0
	stmia	r2!, {r3}
	lsls	r3, r4, #16
	adds	r1, r2, #0
	str	r1, [sp, #12]
	orrs	r3, r7
	mov	r1, r9
	orrs	r3, r1
	stmia	r2!, {r3}
	ldr	r3, [sp, #8]
	movs	r5, #228
	lsls	r5, r5, #8
	adds	r1, r2, #0
	orrs	r3, r5
	str	r1, [sp, #12]
	stmia	r2!, {r3}
	adds	r1, r2, #0
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	movs	r6, #0
	add	r8, r2
	bl 0x0200c8c8
	cmp	r6, fp
	bcs.n	.L_0200423c
	ldr	r3, [sp, #8]
	movs	r1, #128
	adds	r3, #2
	orrs	r3, r5
	lsls	r1, r1, #23
	ldr	r5, [sp, #12]
	mov	r9, r1
	mov	sl, r3
.L_0200420a:
	lsls	r2, r6, #4
	movs	r3, #96
	subs	r4, r3, r2
	movs	r3, #0
	str	r3, [r5, #0]
	lsls	r3, r4, #16
	mov	r2, r9
	orrs	r3, r7
	orrs	r3, r2
	str	r3, [r5, #4]
	mov	r3, sl
	str	r3, [r5, #8]
	ldr	r1, [sp, #12]
	adds	r1, #12
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	adds	r6, #1
	add	r8, r2
	adds	r5, #12
	bl 0x0200c8c8
	cmp	r6, fp
	bcc.n	.L_0200420a
.L_0200423c:
	ldr	r2, [sp, #12]
	movs	r6, #0
	movs	r3, #128
	stmia	r2!, {r6}
	lsls	r3, r3, #8
	mov	r9, r3
	movs	r3, #224
	adds	r1, r2, #0
	lsls	r3, r3, #15
	str	r1, [sp, #12]
	orrs	r3, r7
	mov	r1, r9
	orrs	r3, r1
	stmia	r2!, {r3}
	ldr	r5, [sp, #8]
	adds	r1, r2, #0
	movs	r2, #228
	lsls	r2, r2, #8
	adds	r5, #6
	orrs	r5, r2
	stmia	r1!, {r5}
	mov	r0, r8
	adds	r3, r1, #0
	mov	sl, r2
	movs	r1, #255
	movs	r2, #12
	add	r8, r2
	str	r3, [sp, #12]
	bl 0x0200c8c8
	ldr	r1, [sp, #12]
	stmia	r1!, {r6}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	movs	r3, #240
	lsls	r3, r3, #15
	mov	r2, r9
	orrs	r3, r7
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #21
	orrs	r3, r2
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #12]
	stmia	r1!, {r5}
	adds	r3, r1, #0
	movs	r1, #12
	mov	r0, r8
	add	r8, r1
	movs	r1, #255
	str	r3, [sp, #12]
	bl 0x0200c8c8
	cmp	r6, fp
	bcs.n	.L_020042fe
	ldr	r4, [sp, #8]
	movs	r2, #128
	movs	r1, #128
	mov	r3, sl
	adds	r4, #2
	lsls	r2, r2, #23
	lsls	r1, r1, #16
	ldr	r5, [sp, #12]
	mov	r9, r2
	orrs	r4, r3
	mov	sl, r1
.L_020042c2:
	movs	r3, #0
	str	r3, [r5, #0]
	mov	r2, sl
	adds	r3, r7, #0
	orrs	r3, r2
	mov	r1, r9
	movs	r2, #128
	orrs	r3, r1
	lsls	r2, r2, #21
	orrs	r3, r2
	str	r3, [r5, #4]
	str	r4, [r5, #8]
	ldr	r2, [sp, #12]
	mov	r0, r8
	adds	r2, #12
	movs	r3, #12
	movs	r1, #255
	str	r4, [sp, #0]
	str	r2, [sp, #12]
	add	r8, r3
	bl 0x0200c8c8
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r6, #1
	adds	r5, #12
	add	sl, r1
	ldr	r4, [sp, #0]
	cmp	r6, fp
	bcc.n	.L_020042c2
.L_020042fe:
	movs	r2, #128
	ldr	r4, [sp, #4]
	lsls	r2, r2, #8
	mov	r9, r2
	ldr	r2, [sp, #12]
	movs	r3, #0
	adds	r4, #128
	stmia	r2!, {r3}
	mov	fp, r3
	lsls	r3, r4, #16
	orrs	r7, r3
	mov	r3, r9
	orrs	r7, r3
	movs	r3, #128
	lsls	r3, r3, #21
	adds	r1, r2, #0
	orrs	r7, r3
	str	r1, [sp, #12]
	stmia	r2!, {r7}
	ldr	r3, [sp, #8]
	adds	r1, r2, #0
	movs	r2, #228
	lsls	r2, r2, #8
	orrs	r3, r2
	mov	sl, r2
	adds	r2, r1, #0
	stmia	r2!, {r3}
	adds	r1, r2, #0
	movs	r3, #12
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r1, #255
	add	r8, r3
	bl 0x0200c8c8
	ldr	r3, [pc, #328]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #4
	bhi.n	.L_02004352
	b.n	.L_02004462
.L_02004352:
	ldr	r3, [sp, #16]
	movs	r1, #128
	adds	r3, #224
	lsls	r1, r1, #23
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	mov	r9, r1
	bl 0x0200cb68
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020043e0
	ldr	r3, [sp, #16]
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r6, #8]
	movs	r5, #224
	lsls	r5, r5, #12
	subs	r0, r0, r3
	adds	r1, r5, #0
	bl 0x0200c830
	ldr	r3, [sp, #16]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r4, r0, #0
	ldr	r0, [r6, #16]
	adds	r4, #112
	adds	r1, r5, #0
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl 0x0200c830
	ldr	r3, [sp, #16]
	adds	r3, #218
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r1, [sp, #12]
	subs	r7, r0, #4
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	stmia	r1!, {r3}
	ldr	r4, [sp, #0]
	adds	r2, r1, #0
	lsls	r3, r4, #16
	str	r2, [sp, #12]
	orrs	r7, r3
	mov	r2, r9
	orrs	r7, r2
	stmia	r1!, {r7}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, sl
	adds	r3, #12
	orrs	r3, r1
	ldr	r1, [sp, #12]
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	add	r8, r2
	bl 0x0200c8c8
.L_020043e0:
	ldr	r3, [sp, #16]
	adds	r3, #222
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200cb68
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02004462
	ldr	r3, [sp, #16]
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r6, #8]
	movs	r5, #224
	lsls	r5, r5, #12
	subs	r0, r0, r3
	adds	r1, r5, #0
	bl 0x0200c830
	ldr	r3, [sp, #16]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r4, r0, #0
	ldr	r0, [r6, #16]
	adds	r4, #112
	adds	r1, r5, #0
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl 0x0200c830
	ldr	r3, [sp, #16]
	adds	r3, #218
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r1, [sp, #12]
	subs	r7, r0, #4
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	stmia	r1!, {r3}
	ldr	r4, [sp, #0]
	adds	r2, r1, #0
	lsls	r3, r4, #16
	str	r2, [sp, #12]
	orrs	r7, r3
	mov	r2, r9
	orrs	r7, r2
	stmia	r1!, {r7}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, sl
	adds	r3, #8
	ldr	r2, [sp, #12]
	orrs	r3, r1
	str	r3, [r2, #0]
	mov	r3, r8
	adds	r0, r3, #0
	movs	r1, #255
	bl 0x0200c8c8
.L_02004462:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f3c
	.4byte 0x03001b10
	.4byte 0x040000d4
	.4byte 0x0200cd60
	.4byte 0x050003c0
	.4byte 0x80000010
	.4byte 0x0200cd80
	.2byte 0x1e40
	.2byte 0x0300
	.global Func_02004494
	.thumb_func
Func_02004494:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	mov r8, r0
	mov r10, r1
	movs r0, #59
	ldr r1, [pc, #172]
	ldr r5, [sp, #40]
	ldr r6, [sp, #44]
	str r3, [sp, #0]
	mov r9, r2
	bl 0x0200c878
	adds r7, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200c888
	adds r3, r7, #0
	adds r3, #222
	mov r2, r8
.L_020044c8:
	strh r2, [r3]
	adds r3, #2
	mov r2, r10
	strh r2, [r3]
	adds r3, #2
	strh r5, [r3]
	adds r3, #2
	strh r6, [r3]
	mov r2, r9
	adds r3, #2
	strh r2, [r3]
	ldr r2, [sp, #0]
	adds r3, #2
	str r2, [r3]
	ldr r2, [sp, #36]
	adds r3, #4
	str r2, [r3]
	mov r11, r0
	mov r0, r8
	bl 0x0200ca18
	adds r6, r0, #0
	mov r0, r10
	bl 0x0200ca18
	adds r5, r0, #0
	ldr r0, [pc, #92]
	bl 0x0200c9b0
	cmp r0, #0
	bne .L_020044c8_0
	ldr r2, [sp, #0]
	lsls r3, r2, #1
	ldr r2, [r6, #8]
	subs r3, r3, r2
	str r3, [r5, #8]
	ldr r3, [r6, #16]
	str r3, [r5, #16]
.L_020044c8_0:
	ldr r2, [pc, #60]
	adds r3, r7, #0
	adds r3, #218
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	mov r1, r11
	ldr r0, [pc, #60]
	bl 0x0200c898
	bl 0x0200c8b8
	adds r3, r7, #0
	adds r3, #216
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	mov r2, r11
	lsls r1, r1, #2
	asrs r0, r0, #16
	bl 0x0200c8b0
	ldr r1, [pc, #32]
	ldr r0, [pc, #36]
	bl 0x0200c848
	mov r0, r11
	bl 0x0200c890
	sub sp, #-4
	b .L_020044c8_1
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x7170
	.2byte 0x0000
	.4byte 0x00000109
	.4byte 0x0200cd80
	.4byte 0x00000c76
	.4byte 0x0200c0d1
.L_020044c8_1:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_0200457c
	.thumb_func
Func_0200457c:
	push {r5, r6, lr}
	ldr r3, [pc, #60]
	ldr r6, [r3]
	ldr r5, [pc, #60]
	bl 0x0200c8d0
	adds r1, r6, #0
	adds r1, #240
	bl 0x0200c898
	ldr r0, [pc, #48]
	bl 0x0200c9b0
	cmp r0, #0
	bne .L_0200457c_0
	movs r3, #1
	strh r3, [r5]
	strh r3, [r5, #2]
	adds r3, r6, #0
	adds r3, #224
	ldrh r3, [r3]
	strh r0, [r5, #8]
	strh r3, [r5, #4]
	strh r0, [r5, #6]
.L_0200457c_0:
	ldr r1, [pc, #24]
	ldr r0, [pc, #28]
	bl 0x0200c848
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f3c
	.4byte 0x02001000
	.4byte 0x00000109
	.4byte 0x00000c85
	.4byte 0x0200bef1
	.global Func_020045d0
	.thumb_func
Func_020045d0:
	ldr r3, [pc, #8]
	ldr r3, [r3]
	adds r3, #220
	strh r0, [r3]
	bx lr
	.2byte 0x0000
	.4byte 0x03001f3c
	.global Func_020045e0
	.thumb_func
Func_020045e0:
	push {r5, r6, lr}
	ldr r3, [pc, #64]
	adds r4, r0, #0
	ldr r2, [r3]
	ldr r3, [r4]
	adds r1, r2, #0
	movs r5, #8
	asrs r6, r3, #20
	adds r1, #52
.L_020045e0_2:
	ldmia r1!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, r3
	bne .L_020045e0_0
	ldr r2, [r4, #4]
	ldr r3, [r0, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020045e0_0
	ldr r2, [r4, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_020045e0_1
.L_020045e0_0:
	adds r5, #1
	cmp r5, #65
	bls .L_020045e0_2
	movs r0, #0
.L_020045e0_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02004628
	.thumb_func
Func_02004628:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #328]
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #12
	bl 0x0200ca18
	ldrh r3, [r0, #6]
	ldr r2, [pc, #312]
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r1, [r2, r5]
	ldr r3, [pc, #308]
	mov r9, r2
	adds r2, r1, #0
	ands r2, r3
	mov r10, r3
	ldr r3, [r0, #8]
	mov r7, sp
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r0, #12]
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	mov r8, r0
	str r3, [r7, #8]
	adds r0, r7, #0
	mov r1, r8
	bl 0x0200c5e0
	adds r6, r0, #0
	cmp r6, #0
	beq 0x0200c76e
	mov r0, r9
	ldr r1, [r0, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r7, #8]
.L_0200469a:
	adds r0, r7, #0
	adds r1, r6, #0
.L_0200469e:
	bl 0x0200c5e0
	cmp r0, #0
	beq .L_0200469e_0
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne 0x0200c76e
.L_0200469e_0:
	ldr r3, [r6, #8]
	str r3, [r7]
.L_020046b8:
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r0, r7, #0
	str r3, [r7, #8]
	adds r1, r6, #0
	bl 0x0200c5e0
	cmp r0, #0
	beq 0x0200c6e0
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
.L_020046de:
	bne 0x0200c76e
	movs r3, #0
	adds r2, r6, #0
	adds r2, #34
	mov r11, r3
	movs r3, #2
	strb r3, [r2]
	mov r0, r9
	ldr r1, [r0, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r0, r6, #0
	adds r1, r7, #0
	bl 0x0200c950
.L_02004710:
	cmp r0, #0
	bgt .L_02004710_0
	ldr r5, [pc, #116]
	movs r1, #8
	mov r0, r8
	bl 0x0200c8e0
	movs r0, #15
	bl 0x0200c840
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200c910
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x0200c910
	movs r0, #238
	bl 0x0200cba0
	adds r0, r6, #0
	bl 0x0200c918
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200cba0
	ldr r3, [r7]
	str r3, [r6, #8]
	ldr r3, [r7, #8]
	mov r2, r11
	str r3, [r6, #16]
	str r2, [r6, #36]
	str r2, [r6, #44]
	mov r0, r8
	movs r1, #1
	bl 0x0200c8e0
.L_02004710_0:
	sub sp, #-12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0xcfc0
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xffff
	.4byte 0x00003333
	.global Func_02004790
	.thumb_func
Func_02004790:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #144]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #12
	bl 0x0200ca18
	adds r6, r0, #0
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r7, r3, r2
	movs r3, #192
	lsls r3, r3, #8
	ldr r1, [pc, #116]
	ands r7, r3
	ldr r3, [r6, #8]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	mov r5, sp
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	movs r0, #128
	ands r3, r1
	adds r3, r3, r2
	lsls r0, r0, #13
	mov r8, r1
	adds r2, r5, #0
	adds r1, r7, #0
	str r3, [r5, #8]
	bl 0x0200c868
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200c5e0
	cmp r0, #0
	bne .L_02004790_0
	ldr r3, [r6, #8]
	mov r2, r8
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	adds r3, r3, r1
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	movs r0, #128
	ands r3, r2
	adds r3, r3, r1
	lsls r0, r0, #14
	adds r1, r7, #0
	adds r2, r5, #0
	str r3, [r5, #8]
	bl 0x0200c868
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200c5e0
.L_02004790_0:
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0xfff00000
	.include "games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_MARUTA/IMPORT.INC"
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x05070507
	.4byte 0x06040b07
	.4byte 0x0a040604
	.4byte 0x07050705
	.4byte 0x04060c05
	.4byte 0x0a060406
	.4byte 0x00000000
	.4byte 0x80004000
	.4byte 0x80008000
	.4byte 0x0000c000
	.4byte 0x00000003
	.4byte 0x04400000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x046a0000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008a21
	.4byte 0x80010000
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x20202000
	.4byte 0x40404060
	.4byte 0x10000080
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x2000001e
	.4byte 0x001e0000
	.4byte 0x001e7fff
	.4byte 0x1000ffff
	.4byte 0x00010080
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x1000003c
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x00f01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060170
	.4byte 0x00067fff
	.4byte 0x00e01000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01601000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600d0
	.4byte 0x00067fff
	.4byte 0x01501000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600c0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.4byte 0x82fc0100
	.4byte 0x70462310
	.4byte 0x201abddc
	.4byte 0x648ad0cc
	.4byte 0xa8a89c76
	.4byte 0x81984754
	.4byte 0x6138edda
	.4byte 0x5f28474a
	.4byte 0xa01027d3
	.4byte 0xa0abb823
	.4byte 0xf0214faa
	.4byte 0x5557fc29
	.4byte 0x1c29f456
	.4byte 0xc0a8882e
	.4byte 0xf029560c
	.4byte 0x39f9b811
	.4byte 0xa3e78fa8
	.4byte 0xd88e8df8
	.4byte 0x60101ddd
	.4byte 0x2101d56c
	.4byte 0xa68033e0
	.4byte 0x1ca188ea
	.4byte 0xb037924e
	.4byte 0x8e40d46f
	.4byte 0x56a22701
	.4byte 0x9957005d
	.4byte 0x6e51113b
	.4byte 0x9f8022e5
	.4byte 0x3a3ae095
	.4byte 0x39edc9ab
	.4byte 0x9ddeeaec
	.4byte 0x378733aa
	.4byte 0xbb23a0e8
	.4byte 0x7b4bb0f7
	.4byte 0x0596cc80
	.4byte 0xc075eccf
	.4byte 0x3fd9b0ef
	.4byte 0xcc032ec8
	.4byte 0x8d5aec81
	.4byte 0xee419f03
	.4byte 0x7c20f81f
	.4byte 0x38be6e09
	.4byte 0xbe2a7c41
	.4byte 0x07316774
	.4byte 0xf7104b82
	.4byte 0xf17c90f8
	.4byte 0x00000001
	.4byte 0x33323130
	.4byte 0x37363534
	.4byte 0x42413938
	.4byte 0x46454443
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001b
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x0200bcf5
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000000a
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
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.4byte 0xffff0000
	.4byte 0x00000058
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000058
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0xc0000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000058
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000058
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000091
	.4byte 0x00a01091
	.4byte 0x00b4208c
	.4byte 0x0046308a
	.4byte 0x0056308a
	.4byte 0x000001ff
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x04d80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x04f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0123
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00028000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x04380000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x04b80000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x04380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x05e80000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x05e80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0xffff0118
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0118
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0xffff0118
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000202
	.4byte 0xffff005a
	.4byte 0x02008d71
	.4byte 0x00000202
	.4byte 0xffff005b
	.4byte 0x02008d85
	.4byte 0x00000002
	.4byte 0x0363000b
	.4byte 0x02008205
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x0200857d
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x020084a5
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x0200858d
	.4byte 0x00000202
	.4byte 0xffff0016
	.4byte 0x020085d5
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x020085d1
	.4byte 0x00000202
	.4byte 0xffff0017
	.4byte 0x0200876d
	.4byte 0x00008c15
	.4byte 0x0214000f
	.4byte 0x020085e1
	.4byte 0x00008c15
	.4byte 0x02150010
	.4byte 0x020085e1
	.4byte 0x00008c15
	.4byte 0x02160011
	.4byte 0x020085e1
	.4byte 0x00000002
	.4byte 0x0367000f
	.4byte 0x020082f9
	.4byte 0x00000002
	.4byte 0x13670010
	.4byte 0x02008405
	.4byte 0x00000002
	.4byte 0x02100032
	.4byte 0x02008c5d
	.4byte 0x00000013
	.4byte 0x036a0065
	.4byte 0x001000e2
	.4byte 0x00000013
	.4byte 0x036b0066
	.4byte 0x001000b5
	.4byte 0x00008413
	.4byte 0x036c0067
	.4byte 0x001000e3
	.4byte 0x50001815
	.4byte 0x03620009
	.4byte 0x02008275
	.4byte 0x00000c15
	.4byte 0x0360001d
	.4byte 0x0200824d
	.4byte 0x00009115
	.4byte 0x0361001c
	.4byte 0x02008a85
	.4byte 0x50008e15
	.4byte 0x0363001e
	.4byte 0x02008a45
	.4byte 0x00000006
	.4byte 0xffff0004
	.4byte 0x02008ad5
	.4byte 0x00000006
	.4byte 0xffff0005
	.4byte 0x02008b31
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte 0x0200a9ad
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte 0x02009c21
	.4byte 0x00000000
	.4byte 0xffff0022
	.4byte 0x02009df9
	.4byte 0x00000000
	.4byte 0xffff0023
	.4byte 0x02009f91
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x0200a0f1
	.4byte 0x00000000
	.4byte 0xffff0025
	.4byte 0x0200a331
	.4byte 0x00000000
	.4byte 0xffff0026
	.4byte 0x0200a4d1
	.4byte 0x00008d15
	.4byte 0xffff0020
	.4byte 0x0000213c
	.4byte 0x00008d15
	.4byte 0xffff0021
	.4byte 0x00002146
	.4byte 0x00008d15
	.4byte 0xffff0022
	.4byte 0x00002147
	.4byte 0x00008d15
	.4byte 0xffff0023
	.4byte 0x00002148
	.4byte 0x00008d15
	.4byte 0xffff0024
	.4byte 0x00002149
	.4byte 0x00008d15
	.4byte 0xffff0025
	.4byte 0x0000214a
	.4byte 0x00008d15
	.4byte 0xffff0026
	.4byte 0x0000214b
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte 0x0200a759
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000200
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000200
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000200
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000200
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000200
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000200
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffff00
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000c00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000c00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000c00
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff400
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff400
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff400
	.4byte 0x00000000
	.4byte 0x00000020
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000c00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000c00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000c00
	.4byte 0x00000000
	.4byte 0x0000001a
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff400
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff400
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff400
	.4byte 0x00000000
	.4byte 0x00000034
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x0200937d
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
	.4byte 0x4000ffff
	.4byte 0x0800ff44
	.4byte 0x01801000
	.4byte 0x20000001
	.4byte 0x00010010
	.4byte 0x000e7fff
	.4byte 0x00003000
	.4byte 0x7fff0014
	.4byte 0x3000003c
	.4byte 0x00140800
	.4byte 0x003c7fff
	.4byte 0x1000ffff
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060110
	.4byte 0x00067fff
	.4byte 0x01901000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060120
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x000601a0
	.4byte 0x00067fff
	.4byte 0x01301000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601b0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601c0
	.4byte 0x00067fff
	.4byte 0xffffffff
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x0200bd89
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x0200bd89
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
