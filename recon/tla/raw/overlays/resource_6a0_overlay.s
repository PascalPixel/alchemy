.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008e31, 0x02008045, 0x02008051, 0x02008059, 0x02008e29, 0x0200804d, 0x02008fc1
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	movs	r0, #21
	movs	r1, #74
	bl 0x0200abe4
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb1d4
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb204
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb23c
	.2byte 0x0200
	push	{r5, lr}
	sub	sp, #8
	cmp	r0, #1
	bne.n	.L_02000088
	movs	r0, #133
	bl 0x0200ac5c
	movs	r3, #76
	movs	r2, #35
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #76
	movs	r1, #42
	movs	r2, #4
	movs	r3, #3
	bl 0x0200ab2c
	movs	r0, #6
	bl 0x0200aa4c
.L_02000088:
	movs	r3, #76
	str	r3, [sp, #0]
	movs	r5, #35
	movs	r0, #81
	movs	r1, #42
	movs	r2, #4
	movs	r3, #3
	str	r5, [sp, #4]
	bl 0x0200ab2c
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r0, #12
	movs	r1, #41
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ab24
	add	sp, #8
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #134
	sub	sp, #8
	bl 0x0200ac5c
	movs	r5, #35
	movs	r6, #76
	movs	r1, #42
	movs	r2, #4
	movs	r3, #3
	movs	r0, #76
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ab2c
	movs	r0, #6
	bl 0x0200aa4c
	movs	r0, #71
	movs	r1, #42
	movs	r2, #4
	movs	r3, #3
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ab2c
	movs	r3, #12
	str	r3, [sp, #0]
	movs	r0, #12
	movs	r1, #36
	movs	r2, #4
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ab24
	add	sp, #8
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #16
	bl 0x0200ab7c
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000138
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r3, #8
	bne.n	.L_02000138
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #37
	bne.n	.L_02000138
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200aab4
	movs	r0, #1
	bl 0x02008060
.L_02000138:
	pop	{r5, pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_0200015c
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200aab4
	movs	r0, #1
	bl 0x02008060
.L_0200015c:
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200aaac
	cmp	r0, #1
	bne.n	.L_0200017e
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200aabc
	bl 0x020080b4
.L_0200017e:
	bl 0x02008100
	pop	{pc}
	push	{lr}
	movs	r0, #10
	bl 0x0200aa4c
	bl 0x02008100
	pop	{pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
.L_02000196:
	mov	r7, r8
	push	{r7}
	movs	r0, #18
	bl 0x0200ab7c
	adds	r5, r0, #0
	movs	r0, #19
	bl 0x0200ab7c
	ldr	r3, [r5, #8]
	movs	r7, #1
	asrs	r6, r3, #20
	ldr	r3, [r5, #16]
	mov	r8, r0
	negs	r7, r7
	asrs	r5, r3, #20
	cmp	r6, #35
	bne.n	.L_020001d6
.L_020001ba:
	cmp	r5, #15
	bne.n	.L_020001d6
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020001d6
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aab4
	movs	r7, #0
.L_020001d6:
	cmp	r6, #31
	bne.n	.L_020001f8
	cmp	r5, #16
	bne.n	.L_020001f8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020001f8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aab4
	movs	r7, #2
.L_020001f8:
	cmp	r6, #33
	bne.n	.L_0200021a
	cmp	r5, #10
	bne.n	.L_0200021a
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_0200021a
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aab4
	movs	r7, #4
.L_0200021a:
	mov	r2, r8
	ldr	r3, [r2, #8]
	asrs	r6, r3, #20
	ldr	r3, [r2, #16]
	asrs	r5, r3, #20
	cmp	r6, #35
	bne.n	.L_02000244
	cmp	r5, #15
	bne.n	.L_02000244
	movs	r0, #161
	lsls	r0, r0, #4
.L_02000230:
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000244
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aab4
	movs	r7, #1
.L_02000244:
	cmp	r6, #31
	bne.n	.L_02000266
	cmp	r5, #16
	bne.n	.L_02000266
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000266
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aab4
	movs	r7, #3
.L_02000266:
	cmp	r6, #33
	bne.n	.L_02000288
	cmp	r5, #10
	bne.n	.L_02000288
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000288
	movs	r0, #160
	lsls	r0, r0, #4
.L_02000280:
	adds	r0, #21
	bl 0x0200aab4
	movs	r7, #5
.L_02000288:
	adds	r0, r7, #0
	pop	{r3}
	mov	r8, r3
.L_0200028e:
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r0, #161
	lsls	r0, r0, #4
	movs	r5, #0
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020002bc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020002bc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020002be
.L_020002bc:
	movs	r5, #1
.L_020002be:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020002e8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020002e8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020002ea
.L_020002e8:
	adds	r5, #1
.L_020002ea:
	adds	r0, r5, #0
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	sub	sp, #8
	movs	r3, #94
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #94
	movs	r1, #60
	movs	r2, #9
	movs	r3, #8
	bl 0x0200ab2c
	movs	r0, #140
	movs	r1, #240
	movs	r3, #2
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	negs	r3, r3
	bl 0x0200ab54
	movs	r0, #248
	movs	r1, #128
	movs	r3, #2
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	negs	r3, r3
	bl 0x0200ab54
	movs	r0, #132
	movs	r1, #160
	movs	r3, #2
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	negs	r3, r3
	bl 0x0200ab54
	movs	r0, #152
	movs	r1, #160
	lsls	r1, r1, #16
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #18
	bl 0x0200ac3c
	movs	r0, #248
	movs	r1, #160
	lsls	r1, r1, #16
	movs	r2, #2
	movs	r3, #0
	lsls	r0, r0, #17
	bl 0x0200ac3c
	movs	r0, #248
	movs	r1, #224
	lsls	r0, r0, #17
	lsls	r1, r1, #16
	movs	r2, #2
	movs	r3, #0
	bl 0x0200ac3c
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000388
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000398
.L_02000388:
	movs	r0, #140
	movs	r1, #240
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r3, #0
	bl 0x0200ab54
.L_02000398:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020003b4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020003c4
.L_020003b4:
	movs	r0, #248
	movs	r1, #128
	lsls	r0, r0, #17
	lsls	r1, r1, #17
	movs	r2, #0
	movs	r3, #0
	bl 0x0200ab54
.L_020003c4:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020003e0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020003f0
.L_020003e0:
	movs	r0, #132
	movs	r1, #160
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r3, #0
	bl 0x0200ab54
.L_020003f0:
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_0200047a
	movs	r3, #96
	str	r3, [sp, #0]
	movs	r5, #15
	movs	r0, #96
	movs	r1, #56
	movs	r2, #7
	movs	r3, #2
	str	r5, [sp, #4]
	bl 0x0200ab2c
	movs	r3, #99
	str	r3, [sp, #0]
	movs	r0, #111
	movs	r1, #51
	movs	r2, #1
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ab2c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000454
	movs	r3, #95
	movs	r2, #14
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #105
	movs	r1, #51
	movs	r2, #1
	movs	r3, #3
	bl 0x0200ab2c
	movs	r0, #248
	movs	r1, #224
	lsls	r0, r0, #17
	lsls	r1, r1, #16
	movs	r2, #2
	movs	r3, #236
	bl 0x0200ac3c
.L_02000454:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000464
	b.n	.L_0200059c
.L_02000464:
	movs	r3, #97
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #111
	movs	r1, #53
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ab2c
	b.n	.L_0200059c
.L_0200047a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000514
	movs	r3, #98
	str	r3, [sp, #0]
	movs	r0, #98
	movs	r1, #51
	movs	r2, #5
	movs	r3, #1
	movs	r5, #10
	str	r5, [sp, #4]
	bl 0x0200ab2c
	movs	r3, #99
	movs	r2, #11
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #99
	movs	r1, #52
	movs	r2, #4
	movs	r3, #5
	bl 0x0200ab2c
	movs	r0, #152
	movs	r1, #160
	lsls	r0, r0, #18
	lsls	r1, r1, #16
	movs	r2, #2
	movs	r3, #236
	bl 0x0200ac3c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020004e2
	movs	r3, #95
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #106
	movs	r1, #53
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ab2c
.L_020004e2:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_0200059c
	movs	r3, #95
	str	r3, [sp, #0]
	movs	r0, #109
	movs	r1, #51
	movs	r2, #3
	movs	r3, #1
	str	r5, [sp, #4]
	bl 0x0200ab2c
	movs	r0, #248
	movs	r1, #160
	lsls	r0, r0, #17
	lsls	r1, r1, #16
	movs	r2, #2
	movs	r3, #236
	bl 0x0200ac3c
	b.n	.L_0200059c
.L_02000514:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000536
	movs	r3, #95
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #106
	movs	r1, #53
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ab2c
.L_02000536:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000558
	movs	r3, #95
	movs	r2, #16
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #107
	movs	r1, #53
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ab2c
.L_02000558:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_0200057a
	movs	r3, #97
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #111
	movs	r1, #52
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ab2c
.L_0200057a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_0200059c
	movs	r3, #97
	movs	r2, #10
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #111
	movs	r1, #53
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ab2c
.L_0200059c:
	add	sp, #8
	pop	{r5, pc}
	push	{r5, lr}
	ldr	r3, [pc, #64]
	adds	r5, r0, #0
	adds	r4, r3, #0
	movs	r2, #0
	ldrsh	r3, [r4, r2]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_020005de
	movs	r0, #2
	movs	r1, #0
.L_020005b8:
	ldrsh	r2, [r4, r1]
	ldr	r3, [r5, #8]
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020005d0
	ldrsh	r2, [r0, r4]
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_020005d0
	movs	r0, #1
	b.n	.L_020005e0
.L_020005d0:
	adds	r1, #4
	ldrsh	r3, [r4, r1]
	movs	r2, #1
	negs	r2, r2
	adds	r0, #4
	cmp	r3, r2
	bne.n	.L_020005b8
.L_020005de:
	movs	r0, #0
.L_020005e0:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0xac64
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #26
	sub	sp, #8
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020006a2
	movs	r3, #9
	str	r3, [sp, #4]
	movs	r5, #72
	movs	r0, #80
	movs	r1, #46
	movs	r2, #1
	movs	r3, #7
	str	r5, [sp, #0]
	bl 0x0200ab2c
	movs	r3, #16
	str	r3, [sp, #4]
	movs	r0, #80
	movs	r1, #53
	movs	r2, #8
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200ab2c
	movs	r3, #8
	movs	r2, #74
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #78
	movs	r1, #47
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ab2c
	movs	r0, #128
	movs	r1, #192
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	movs	r3, #4
	bl 0x0200ab54
	cmp	r6, #1
	bne.n	.L_02000682
	movs	r7, #0
.L_0200064c:
	movs	r6, #0
.L_0200064e:
	adds	r0, r6, #0
	adds	r0, #10
	bl 0x0200ab7c
	adds	r5, r0, #0
	bl 0x020085a0
	cmp	r0, #0
	beq.n	.L_02000670
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [pc, #260]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
.L_02000670:
	adds	r6, #1
	cmp	r6, #5
	ble.n	.L_0200064e
	movs	r0, #1
	adds	r7, #1
	bl 0x0200aa4c
	cmp	r7, #15
	ble.n	.L_0200064c
.L_02000682:
	movs	r6, #0
.L_02000684:
	adds	r0, r6, #0
	adds	r0, #10
	bl 0x0200ab7c
	adds	r5, r0, #0
	adds	r2, r5, #0
	movs	r3, #3
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r6, #1
	ldr	r3, [r5, #20]
	str	r3, [r5, #12]
	cmp	r6, #5
	ble.n	.L_02000684
	b.n	.L_0200076a
.L_020006a2:
	movs	r3, #9
	str	r3, [sp, #4]
	movs	r5, #72
	movs	r0, #72
	movs	r1, #46
	movs	r2, #1
	movs	r3, #7
	str	r5, [sp, #0]
	bl 0x0200ab2c
	movs	r3, #16
	str	r3, [sp, #4]
	movs	r0, #72
	movs	r1, #53
	movs	r2, #8
	movs	r3, #5
	str	r5, [sp, #0]
	bl 0x0200ab2c
	movs	r3, #8
	movs	r2, #74
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #70
	movs	r1, #47
	movs	r2, #1
	movs	r3, #1
	bl 0x0200ab2c
	movs	r0, #128
	movs	r1, #192
	lsls	r1, r1, #16
	lsls	r0, r0, #16
	movs	r2, #0
	movs	r3, #6
	bl 0x0200ab54
	ldr	r3, [pc, #132]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ab8c
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aba4
	cmp	r6, #1
	bne.n	.L_02000742
	movs	r7, #0
.L_0200070a:
	movs	r6, #0
.L_0200070c:
	adds	r0, r6, #0
	adds	r0, #10
	bl 0x0200ab7c
	adds	r5, r0, #0
	bl 0x020085a0
	cmp	r0, #0
	beq.n	.L_02000730
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r5, #12]
.L_02000730:
	adds	r6, #1
	cmp	r6, #5
	ble.n	.L_0200070c
	movs	r0, #1
	adds	r7, #1
	bl 0x0200aa4c
	cmp	r7, #15
	ble.n	.L_0200070a
.L_02000742:
	movs	r6, #0
.L_02000744:
	adds	r0, r6, #0
	adds	r0, #10
	bl 0x0200ab7c
	adds	r5, r0, #0
	bl 0x020085a0
	cmp	r0, #0
	beq.n	.L_02000764
	adds	r2, r5, #0
	movs	r3, #4
	adds	r2, #85
	strb	r3, [r2, #0]
	movs	r3, #130
	lsls	r3, r3, #14
	str	r3, [r5, #12]
.L_02000764:
	adds	r6, #1
	cmp	r6, #5
	ble.n	.L_02000744
.L_0200076a:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0xfffe0000
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r0, r5, #0
	bl 0x0200ab7c
	adds	r6, r0, #0
	cmp	r5, #20
	bne.n	.L_020007a0
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	movs	r2, #0
	movs	r3, #6
	bl 0x0200ab54
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200ac54
.L_020007a0:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	adds	r0, r7, #0
	bl 0x0200ab7c
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	bl 0x0200ab14
	ldr	r3, [r5, #20]
	cmp	r3, r0
	beq.n	.L_02000806
	movs	r2, #85
	adds	r2, r2, r5
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r0, #2
	mov	r8, r2
	bl 0x0200aa4c
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	movs	r6, #0
	b.n	.L_020007f0
.L_020007e0:
	movs	r0, #1
	adds	r6, #1
	bl 0x0200aa4c
	cmp	r6, #29
	bgt.n	.L_020007fa
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
.L_020007f0:
	cmp	r2, r3
	bgt.n	.L_020007e0
	ldr	r3, [r5, #40]
	cmp	r3, #0
	bne.n	.L_020007e0
.L_020007fa:
	movs	r0, #188
	bl 0x0200ac5c
	movs	r3, #0
	mov	r2, r8
	strb	r3, [r2, #0]
.L_02000806:
	adds	r3, r7, #0
	subs	r3, #18
	cmp	r3, #1
	bls.n	.L_02000810
	b.n	.L_0200092a
.L_02000810:
	bl 0x02008194
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_02000820
	b.n	.L_020009ce
.L_02000820:
	adds	r0, r7, #0
	bl 0x0200ab7c
	movs	r1, #0
	bl 0x0200ab34
	movs	r0, #30
	bl 0x0200ab64
	bl 0x020082f0
	cmp	r5, #0
	bne.n	.L_02000840
	movs	r0, #167
	bl 0x0200ac5c
.L_02000840:
	cmp	r5, #1
	bne.n	.L_0200084a
	movs	r0, #167
	bl 0x0200ac5c
.L_0200084a:
	cmp	r5, #3
	bne.n	.L_02000860
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000860
	movs	r0, #167
	bl 0x0200ac5c
.L_02000860:
	cmp	r5, #4
	bne.n	.L_02000878
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000878
	movs	r0, #167
	bl 0x0200ac5c
.L_02000878:
	ldr	r3, [pc, #344]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	bl 0x0200ab8c
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aba4
	bl 0x02008290
	cmp	r0, #2
	beq.n	.L_0200089a
	b.n	.L_020009ce
.L_0200089a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #22
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_020008aa
	b.n	.L_020009ce
.L_020008aa:
	movs	r0, #17
	bl 0x0200ab7c
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #22
	bl 0x0200aabc
	movs	r0, #10
	bl 0x0200ab64
	bl 0x0200ab6c
	movs	r0, #0
	bl 0x0200ac0c
	movs	r0, #166
	movs	r1, #1
	movs	r2, #136
	lsls	r2, r2, #16
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200abcc
	bl 0x0200abd4
	movs	r0, #20
	bl 0x0200ab64
	movs	r0, #151
	bl 0x0200ac5c
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200aad4
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r2, [r3, #0]
	ldr	r1, [r5, #16]
	movs	r3, #255
	ldr	r0, [r5, #8]
	bl 0x0200ac3c
	movs	r0, #30
	bl 0x0200ab64
	ldr	r0, [r6, #0]
	movs	r1, #1
	bl 0x0200abc4
	bl 0x0200abd4
	bl 0x0200ab74
	b.n	.L_020009ce
.L_0200092a:
	cmp	r7, #20
	bne.n	.L_020009ce
	ldr	r0, [r5, #8]
	asrs	r3, r0, #20
	cmp	r3, #8
	bne.n	.L_020009b8
	ldr	r3, [r5, #16]
	asrs	r3, r3, #20
	cmp	r3, #12
	bne.n	.L_020009b8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #26
	bl 0x0200aab4
	movs	r0, #20
	bl 0x0200ab7c
	movs	r1, #0
	bl 0x0200ab34
	movs	r0, #30
	bl 0x0200ab64
	movs	r0, #167
	bl 0x0200ac5c
	movs	r0, #1
	bl 0x020085e8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #23
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020009ce
	movs	r0, #9
	bl 0x0200ab7c
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #23
	bl 0x0200aabc
	movs	r0, #20
	bl 0x0200ab64
	movs	r0, #151
	bl 0x0200ac5c
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x0200aad4
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #253
	ands	r3, r2
	strb	r3, [r1, #0]
.L_020009a6:
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r2, [r3, #0]
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r3, #255
	bl 0x0200ac3c
	b.n	.L_020009ce
.L_020009b8:
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #8
	bl 0x0200ab54
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200ac4c
.L_020009ce:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	bl 0x0200ac0c
	pop	{pc}
	.2byte 0x0000
	push	{lr}
	adds	r3, r0, #0
	adds	r2, r1, #0
	ldr	r0, [pc, #8]
	adds	r1, r3, #0
	.2byte 0xf001
	.2byte 0xf82b
	pop	{pc}
	.2byte 0xb1c4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #4
	ldr	r7, [sp, #32]
	mov	sl, r0
	mov	r9, r3
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl 0x0200ab7c
	movs	r3, #128
	lsls	r3, r3, #12
	mov	r8, r3
	lsls	r5, r5, #20
	lsls	r6, r6, #20
	add	r5, r8
	add	r6, r8
	adds	r4, r0, #0
	adds	r1, r5, #0
	mov	r0, sl
	adds	r2, r6, #0
	str	r4, [sp, #0]
	bl 0x0200aba4
	ldr	r4, [sp, #0]
	movs	r3, #3
	adds	r2, r4, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	ldr	r3, [pc, #48]
	movs	r2, #204
	str	r3, [r4, #20]
	str	r3, [r4, #12]
	lsls	r2, r2, #8
	mov	r3, r8
	str	r3, [r4, #40]
	mov	r0, sl
	ldr	r1, [pc, #36]
	adds	r2, #204
	bl 0x0200ab84
	mov	r0, sl
	mov	r1, r9
	adds	r2, r7, #0
	bl 0x0200ab9c
	movs	r0, #222
	bl 0x0200ac5c
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0xfff00000
	.2byte 0x9999
	.2byte 0x0001
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	movs	r0, #161
	lsls	r0, r0, #4
	sub	sp, #4
	bl 0x0200aaac
	adds	r5, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	mov	r9, r0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	mov	sl, r0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
	mov	r8, r0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	adds	r6, r0, #0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	add	r5, r9
	add	r5, sl
	add	r5, r8
	adds	r5, r5, r6
	cmn	r5, r0
	bne.n	.L_02000ad4
	b.n	.L_02000c6c
.L_02000ad4:
	movs	r0, #148
	movs	r1, #1
	movs	r2, #248
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200abcc
	bl 0x0200abd4
	movs	r0, #10
	bl 0x0200ab64
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000b0e
	movs	r2, #0
	movs	r3, #16
	str	r2, [sp, #0]
	negs	r3, r3
	movs	r0, #18
	movs	r1, #35
	movs	r2, #15
	bl 0x020089f8
.L_02000b0e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000b2e
	movs	r2, #0
	movs	r3, #16
	str	r2, [sp, #0]
.L_02000b22:
	negs	r3, r3
.L_02000b24:
	movs	r0, #19
	movs	r1, #35
	movs	r2, #15
	bl 0x020089f8
.L_02000b2e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000b4c
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #18
	movs	r1, #31
	movs	r2, #16
	movs	r3, #16
	bl 0x020089f8
.L_02000b4c:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000b6a
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #19
	movs	r1, #31
	movs	r2, #16
	movs	r3, #16
	bl 0x020089f8
.L_02000b6a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000b8a
	movs	r2, #0
	movs	r3, #16
	str	r2, [sp, #0]
	negs	r3, r3
	movs	r0, #18
	movs	r1, #33
	movs	r2, #10
	bl 0x020089f8
.L_02000b8a:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000baa
	movs	r2, #0
	movs	r3, #16
	str	r2, [sp, #0]
	negs	r3, r3
	movs	r0, #19
	movs	r1, #33
	movs	r2, #10
	bl 0x020089f8
.L_02000baa:
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aabc
	bl 0x020082f0
	movs	r0, #2
	bl 0x0200aa4c
	movs	r0, #18
	bl 0x0200ab7c
	adds	r5, r0, #0
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
.L_02000bfa:
	movs	r6, #0
	b.n	.L_02000c0e
.L_02000bfe:
	movs	r0, #1
	adds	r6, #1
	bl 0x0200aa4c
	cmp	r6, #29
	bgt.n	.L_02000c18
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
.L_02000c0e:
	cmp	r2, r3
	bgt.n	.L_02000bfe
	ldr	r3, [r5, #40]
	cmp	r3, #0
	bne.n	.L_02000bfe
.L_02000c18:
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #19
	bl 0x0200ab7c
	adds	r5, r0, #0
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	movs	r6, #0
	b.n	.L_02000c40
.L_02000c30:
	movs	r0, #1
	adds	r6, #1
	bl 0x0200aa4c
	cmp	r6, #29
	bgt.n	.L_02000c4a
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
.L_02000c40:
	cmp	r2, r3
	bgt.n	.L_02000c30
	ldr	r3, [r5, #40]
	cmp	r3, #0
	bne.n	.L_02000c30
.L_02000c4a:
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r0, #30
	bl 0x0200ab64
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl 0x0200abc4
	bl 0x0200abd4
.L_02000c6c:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #26
	sub	sp, #4
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000c9e
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #20
	movs	r1, #8
	movs	r2, #12
	movs	r3, #16
	bl 0x020089f8
.L_02000c9e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #26
	bl 0x0200aabc
	movs	r0, #1
	bl 0x020085e8
	movs	r0, #2
	bl 0x0200aa4c
	movs	r0, #20
	bl 0x0200ab7c
	adds	r5, r0, #0
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
	movs	r6, #0
	b.n	.L_02000cd4
.L_02000cc4:
	movs	r0, #1
	adds	r6, #1
	bl 0x0200aa4c
	cmp	r6, #29
	bgt.n	.L_02000cde
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #20]
.L_02000cd4:
	cmp	r2, r3
	bgt.n	.L_02000cc4
	ldr	r3, [r5, #40]
	cmp	r3, #0
	bne.n	.L_02000cc4
.L_02000cde:
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	add	sp, #4
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
.L_02000cf0:
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #292]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200ab7c
	adds	r6, r0, #0
	movs	r0, #18
	bl 0x0200ab7c
	adds	r7, r0, #0
	movs	r0, #19
	bl 0x0200ab7c
	ldrh	r1, [r6, #6]
	movs	r3, #0
	movs	r2, #128
	lsls	r2, r2, #6
	mov	sl, r3
	movs	r3, #192
	adds	r1, r1, r2
	lsls	r3, r3, #8
	ands	r1, r3
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	mov	r8, r0
	ldr	r3, [r6, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	adds	r2, r5, #0
	ldr	r3, [r6, #16]
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	bl 0x0200aa74
	ldr	r3, [r5, #0]
	asrs	r6, r3, #20
	ldr	r3, [r5, #8]
	asrs	r5, r3, #20
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bne.n	.L_02000d62
	ldr	r3, [r7, #8]
	asrs	r3, r3, #20
	cmp	r6, r3
	bne.n	.L_02000d62
	ldr	r3, [r7, #16]
	asrs	r3, r3, #20
	cmp	r5, r3
	bne.n	.L_02000d62
	movs	r3, #1
	mov	sl, r3
.L_02000d62:
	mov	r2, r8
	ldr	r3, [r2, #12]
	cmp	r3, #0
	bne.n	.L_02000d7e
	ldr	r3, [r2, #8]
	asrs	r3, r3, #20
	cmp	r6, r3
	bne.n	.L_02000d7e
	ldr	r3, [r2, #16]
	asrs	r3, r3, #20
	cmp	r5, r3
	bne.n	.L_02000d7e
	movs	r3, #1
	mov	sl, r3
.L_02000d7e:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_02000e06
	cmp	r6, #35
	bne.n	.L_02000daa
	cmp	r5, #15
	bne.n	.L_02000daa
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000da6
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000daa
.L_02000da6:
	movs	r3, #2
	mov	sl, r3
.L_02000daa:
	cmp	r6, #31
	bne.n	.L_02000dd2
	cmp	r5, #16
	bne.n	.L_02000dd2
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000dce
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000dd2
.L_02000dce:
	movs	r2, #2
	mov	sl, r2
.L_02000dd2:
	cmp	r6, #33
	bne.n	.L_02000dfa
	cmp	r5, #10
	bne.n	.L_02000dfa
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000df6
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000dfa
.L_02000df6:
	movs	r3, #2
	mov	sl, r3
.L_02000dfa:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_02000e06
	bl 0x0200abec
	b.n	.L_02000e10
.L_02000e06:
	mov	r3, sl
	cmp	r3, #1
	bne.n	.L_02000e10
	bl 0x0200ac24
.L_02000e10:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x0200abec
	pop	{pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0xb3a4
	.2byte 0x0200
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	movs	r2, #129
	adds	r3, r3, r1
	lsls	r2, r2, #2
	str	r2, [r3, #0]
	ldr	r2, [pc, #368]
	adds	r1, #54
	adds	r3, r2, r1
	ldrh	r3, [r3, #0]
	movs	r1, #128
	subs	r3, #3
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	cmp	r3, r1
	bhi.n	.L_02000e6e
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
.L_02000e6e:
	movs	r0, #8
	bl 0x0200ab7c
	movs	r5, #192
	lsls	r5, r5, #8
	str	r5, [r0, #24]
	movs	r0, #8
	bl 0x0200ab7c
	str	r5, [r0, #28]
	ldr	r0, [pc, #312]
	.2byte 0xf000
	.2byte 0xfdaa
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000eaa
	movs	r1, #136
	movs	r2, #150
	movs	r0, #16
	lsls	r1, r1, #16
	lsls	r2, r2, #18
	bl 0x0200aba4
	movs	r0, #0
	bl 0x02008060
.L_02000eaa:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #2
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000ebe
	movs	r0, #0
	bl 0x02008060
.L_02000ebe:
	bl 0x02009db0
	movs	r0, #2
	bl 0x02009ee8
	movs	r0, #18
	movs	r1, #2
	bl 0x02009eb8
	movs	r0, #19
	movs	r1, #1
	bl 0x02009eb8
	movs	r0, #20
	movs	r1, #0
	bl 0x02009eb8
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000f08
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000f08
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000f12
.L_02000f08:
	movs	r0, #18
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aba4
.L_02000f12:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aaac
.L_02000f1c:
	cmp	r0, #0
	bne.n	.L_02000f3c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02000f3c
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02000f46
.L_02000f3c:
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200aba4
.L_02000f46:
	ldr	r3, [pc, #112]
	movs	r2, #133
	lsls	r2, r2, #2
.L_02000f4c:
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #64
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #128
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #11
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #12
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #13
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #14
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #15
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x02000240
	.2byte 0xb1c4
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #0
	.2byte 0xf000
	.2byte 0xf8b4
	movs	r0, #1
	.2byte 0xf000
	.2byte 0xf92b
	ldr	r3, [pc, #348]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #12
	bne.n	.L_02001032
	movs	r1, #168
	movs	r2, #132
	movs	r0, #10
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200aba4
	movs	r1, #200
	movs	r2, #132
	movs	r0, #11
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200aba4
	movs	r1, #232
	movs	r2, #132
	movs	r0, #12
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200aba4
	movs	r1, #168
	movs	r2, #148
	movs	r0, #13
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200aba4
	movs	r1, #136
	movs	r2, #164
	movs	r0, #14
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200aba4
	movs	r1, #168
	movs	r2, #164
	movs	r0, #15
	lsls	r1, r1, #16
	lsls	r2, r2, #17
	bl 0x0200aba4
.L_02001032:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02001080
	bl 0x02008290
	cmp	r0, #2
	beq.n	.L_02001080
	movs	r0, #161
	lsls	r0, r0, #4
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #18
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #20
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #17
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #19
	bl 0x0200aabc
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #21
	bl 0x0200aabc
.L_02001080:
	bl 0x02008290
	cmp	r0, #2
	bne.n	.L_02001094
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #22
	bl 0x0200aabc
	b.n	.L_0200109e
.L_02001094:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #22
	bl 0x0200aab4
.L_0200109e:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #23
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020010b8
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #26
	bl 0x0200aabc
	b.n	.L_020010c2
.L_020010b8:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #26
	bl 0x0200aab4
.L_020010c2:
	movs	r0, #18
	bl 0x0200ab7c
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #19
	bl 0x0200ab7c
	adds	r0, #85
	strb	r5, [r0, #0]
	movs	r0, #20
	bl 0x0200ab7c
	adds	r0, #85
	strb	r5, [r0, #0]
	bl 0x020082f0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #26
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02001120
	movs	r0, #20
	bl 0x0200ab7c
	adds	r5, r0, #0
	ldr	r1, [r5, #16]
	movs	r2, #0
	ldr	r0, [r5, #8]
	bl 0x0200ab5c
	adds	r3, r0, #0
	ldr	r1, [r5, #16]
	ldr	r0, [r5, #8]
	adds	r3, #2
	movs	r2, #0
	bl 0x0200ab54
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200ac4c
.L_02001120:
	movs	r0, #0
	bl 0x020085e8
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #20
	adds	r0, #255
	bl 0x0200aab4
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #37
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02001158
	movs	r0, #98
	adds	r0, #255
	bl 0x0200aab4
	movs	r0, #162
	lsls	r0, r0, #1
	bl 0x0200aab4
.L_02001158:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x60184b01
	.2byte 0x4770
	.2byte 0x0000
	push	{r2, r4, r7, lr}
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #164]
	sub	sp, #32
	ldr	r0, [r3, #0]
	cmp	r0, #0
	bge.n	.L_0200117a
	adds	r0, #3
.L_0200117a:
	asrs	r0, r0, #2
	movs	r1, #5
	bl 0x0200aa44
	ldr	r3, [pc, #148]
	mov	r8, r0
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_020011ce
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r0, #179
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrh	r1, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_020011ae
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r1
	cmp	r3, #153
	bne.n	.L_0200120a
.L_020011ae:
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #164
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0200120a
	movs	r0, #175
	lsls	r0, r0, #1
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_0200120a
.L_020011ce:
	movs	r5, #0
	movs	r6, #4
.L_020011d2:
	mov	r2, r8
	adds	r0, r2, r5
	movs	r1, #5
	mov	r7, sp
	bl 0x0200aa44
	ldr	r3, [pc, #60]
	lsls	r0, r0, #1
	ldrh	r3, [r3, r6]
	adds	r5, #1
	strh	r3, [r7, r0]
	adds	r6, #2
	cmp	r5, #4
	ble.n	.L_020011d2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r7, #0
	ldr	r1, [pc, #36]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L_0200120a:
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r4, r7, lr}
	lsls	r0, r0, #8
	push	{r2, r4, r7, lr}
	lsls	r0, r0, #8
	push	{r2, r3, r4, r5, r7, lr}
	lsls	r0, r0, #8
	lsls	r4, r0, #6
	lsls	r0, r0, #20
	push	{r5, r6, lr}
	ldr	r2, [pc, #88]
	movs	r3, #1
	adds	r6, r0, #0
	str	r3, [r2, #0]
	cmp	r6, #2
	beq.n	0x02009248
	ldr	r1, [pc, #80]
	movs	r2, #32
	ldr	r0, [pc, #80]
	ldr	r5, [pc, #80]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4814
	ldr	r1, [pc, #80]
	movs	r2, #32
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02001258
	cmp	r6, #1
	bne.n	.L_0200126a
.L_02001258:
	ldr	r3, [pc, #60]
	movs	r2, #0
	movs	r1, #144
	str	r2, [r3, #0]
	ldr	r0, [pc, #56]
	lsls	r1, r1, #3
	bl 0x0200aa54
	b.n	.L_0200127e
.L_0200126a:
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #44]
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_0200127e:
	pop	{r5, r6, pc}
	push	{r2, r4, r7, lr}
	lsls	r0, r0, #8
	lsls	r0, r0, #6
	lsls	r0, r0, #20
	push	{r2, r3, r4, r5, r7, lr}
	lsls	r0, r0, #8
	lsls	r0, r6, #28
	lsls	r0, r0, #12
	push	{r2, r3, r4, r6, r7, lr}
	lsls	r0, r0, #8
.L_02001294:
	lsls	r0, r4, #6
	lsls	r0, r0, #20
	push	{r4, r7, lr}
	lsls	r0, r0, #8
	str	r1, [sp, #420]
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	lsls	r0, r0, #8
	lsls	r4, r0, #6
	lsls	r0, r0, #20
	push	{r5, lr}
	bl 0x0200ab7c
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #85
	movs	r3, #4
	strb	r3, [r1, #0]
	movs	r2, #0
	ldr	r3, [r5, #20]
	str	r2, [r5, #68]
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r5, #12]
	subs	r1, #50
	ldrb	r2, [r1, #0]
	movs	r3, #128
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_020012d0:
	adds	r3, r5, #0
	adds	r3, #34
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	ldrb	r0, [r3, #0]
	bl 0x0200ab14
	adds	r3, r0, #0
	asrs	r3, r3, #19
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	adds	r3, #6
	movs	r2, #0
	bl 0x0200ab54
	ldr	r0, [r5, #8]
	ldr	r1, [r5, #16]
	movs	r2, #0
	movs	r3, #128
	bl 0x0200ac44
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #244]
.L_02001312:
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ab7c
	adds	r6, r0, #0
	ldr	r7, [r6, #104]
	bl 0x0200ab6c
	movs	r0, #0
	bl 0x0200ac0c
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	movs	r5, #0
	strh	r5, [r3, #0]
	movs	r3, #85
	adds	r3, r3, r6
	mov	r9, r3
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200ab34
	movs	r3, #99
	adds	r3, r3, r7
	mov	r8, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0200139c
.L_02001356:
	ldr	r3, [r7, #8]
	ldr	r2, [pc, #176]
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	adds	r3, r3, r5
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	cmp	r5, r2
	bgt.n	.L_02001372
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	adds	r5, r5, r3
.L_02001372:
	ldr	r3, [pc, #156]
	adds	r1, r6, #0
	ldr	r2, [r3, #0]
	ldrb	r3, [r3, #0]
	adds	r1, #35
	lsls	r3, r3, #12
	strh	r3, [r6, #6]
	movs	r3, #1
	ands	r2, r3
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	movs	r0, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	bl 0x0200aa4c
	mov	r2, r8
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_02001356
.L_0200139c:
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #68
	movs	r2, #1
	add	r3, sl
	strh	r2, [r3, #0]
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #70
	add	r3, sl
	strh	r2, [r3, #0]
	ldr	r3, [r7, #8]
	ldrh	r1, [r7, #6]
	subs	r2, #3
	asrs	r3, r3, #19
	ands	r3, r2
	asrs	r1, r1, #13
	adds	r3, r3, r1
	subs	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #8]
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #56]
	asrs	r3, r3, #19
	ands	r3, r2
	movs	r2, #2
	ands	r1, r2
	subs	r3, r3, r1
.L_020013d4:
	adds	r3, #1
	lsls	r3, r3, #19
	str	r3, [r6, #16]
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	adds	r3, r6, #0
	adds	r3, #35
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200ab34
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	b.n	.L_02001414
	.4byte 0x00000001
	.4byte 0x02000240
	.4byte 0x0003ffff
	.2byte 0x122c
	.2byte 0x0300
.L_02001414:
	bl 0x0200abcc
	bl 0x0200abdc
	movs	r3, #128
	adds	r7, r0, #0
	lsls	r3, r3, #12
	str	r3, [r7, #48]
	movs	r3, #128
	ldr	r5, [pc, #52]
	lsls	r3, r3, #9
	str	r3, [r7, #52]
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	movs	r0, #0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	bl 0x0200ab14
	ldr	r3, [r6, #16]
	adds	r2, r0, #0
	ldr	r1, [r6, #8]
	adds	r0, r7, #0
	bl 0x0200ab04
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L_0200147a
	b.n	.L_02001460
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02001460:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200aa4c
	cmp	r5, #59
	bgt.n	.L_0200147a
	ldr	r3, [r6, #20]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	ldr	r2, [r6, #12]
	cmp	r2, r3
	bgt.n	.L_02001460
.L_0200147a:
	movs	r0, #127
	bl 0x0200ac5c
	ldr	r3, [r6, #40]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0200149a
.L_02001488:
	movs	r0, #1
	adds	r5, #1
	bl 0x0200aa4c
	cmp	r5, #59
	bgt.n	.L_0200149a
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02001488
.L_0200149a:
	adds	r0, r7, #0
	bl 0x0200ab0c
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	ldr	r0, [r5, #0]
	bl 0x0200ab7c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #1
	ldr	r0, [r5, #0]
	bl 0x0200abc4
	movs	r2, #208
	lsls	r2, r2, #4
	adds	r2, #70
	add	r2, sl
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #170
	lsls	r3, r3, #1
	movs	r6, #0
	add	r3, sl
	strh	r6, [r3, #0]
	bl 0x0200ab74
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [r1, #0]
	ldr	r4, [r0, #0]
	ldr	r2, [r1, #8]
	subs	r4, r4, r3
	ldr	r3, [r0, #8]
	asrs	r4, r4, #16
	subs	r3, r3, r2
	asrs	r3, r3, #16
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r4, #0
	muls	r0, r4
	adds	r3, r2, #0
.L_02001504:
	adds	r0, r0, r3
	ldr	r3, [pc, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.2byte 0x0000
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
.L_02001524:
	movs	r2, #133
	mov	sl, r3
	ldr	r3, [pc, #80]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl 0x0200ab7c
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
.L_02001542:
	adds	r7, r0, #0
	cmp	r3, #0
	bne.n	.L_02001572
	movs	r3, #173
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_02001572
	movs	r3, #175
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
.L_02001560:
	cmp	r3, #0
	bne.n	.L_02001572
	movs	r3, #180
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02001580
.L_02001572:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200aadc
	b.n	.L_020016be
	.2byte 0x0240
	.2byte 0x0200
.L_02001580:
	adds	r0, r6, #0
	movs	r1, #16
	bl 0x0200aadc
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #31
	ands	r3, r2
	cmp	r3, #31
	bne.n	.L_020015a0
	movs	r0, #231
	bl 0x0200ac5c
.L_020015a0:
	ldr	r3, [r7, #80]
	ldr	r0, [r6, #80]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r0, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	movs	r2, #2
	ldr	r0, [r6, #8]
	ldr	r1, [r6, #16]
	bl 0x0200ac34
	cmp	r0, #255
	beq.n	.L_020016a2
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl 0x0200ac14
	ldr	r5, [r5, #0]
	movs	r3, #136
	lsls	r3, r3, #17
	cmp	r5, r3
	bgt.n	.L_020016a2
	ldr	r2, [pc, #172]
	cmp	r5, r2
	blt.n	.L_020016a2
	movs	r3, #98
	adds	r3, r3, r6
	mov	r9, r3
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_02001668
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #12]
	subs	r5, r2, r3
	cmp	r5, #0
	bge.n	.L_02001600
	subs	r5, r3, r2
.L_02001600:
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	adds	r0, #8
	adds	r1, #8
	mov	r8, r2
	bl 0x020094e8
	cmp	r0, #12
	bgt.n	.L_02001620
	movs	r3, #192
	lsls	r3, r3, #12
	cmp	r5, r3
	bge.n	.L_02001620
	movs	r2, #1
	mov	r8, r2
.L_02001620:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_02001668
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02001668
	ldrh	r3, [r6, #6]
	str	r6, [r7, #104]
	strh	r3, [r7, #6]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #181
	lsls	r2, r2, #1
	strb	r3, [r1, #0]
	add	r2, sl
	movs	r3, #200
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r2, #128
	ldr	r0, [pc, #56]
	lsls	r2, r2, #2
	adds	r2, #18
	adds	r3, r3, r2
	strb	r0, [r3, #0]
	mov	r2, r9
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r2, r6, #0
	adds	r2, #99
	strb	r3, [r2, #0]
.L_02001668:
	ldrh	r0, [r6, #6]
	bl 0x0200aa6c
	ldr	r1, [r6, #48]
	ldr	r5, [pc, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68b3
	adds	r3, r3, r0
	ldrh	r0, [r6, #6]
	str	r3, [r6, #8]
	bl 0x0200aa64
	ldr	r1, [r6, #48]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6933
	b.n	.L_0200169c
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_0200169c:
	adds	r3, r3, r0
	str	r3, [r6, #16]
	b.n	.L_020016be
.L_020016a2:
	adds	r3, r6, #0
	adds	r3, #99
	movs	r5, #0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #32]
	adds	r0, r6, #0
	str	r5, [r6, #108]
	bl 0x0200aae4
	movs	r0, #228
	bl 0x0200ac5c
	ldr	r3, [pc, #20]
	str	r5, [r3, #0]
.L_020016be:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r3, r4, r7, lr}
	lsls	r0, r0, #8
	push	{r3, r4, r5, r7, lr}
	lsls	r0, r0, #8
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #222
	sub	sp, #68
	bl 0x0200ac5c
	ldrh	r0, [r5, #6]
	bl 0x0200aa6c
	adds	r1, r0, #0
	movs	r0, #128
	ldr	r6, [pc, #152]
	lsls	r0, r0, #12
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x68ab
	add	r2, sp, #56
	adds	r3, r3, r0
	str	r3, [r2, #0]
	mov	r8, r2
	ldrh	r0, [r5, #6]
	bl 0x0200aa64
	adds	r1, r0, #0
	movs	r0, #128
	lsls	r0, r0, #12
.L_0200170c:
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x692b
	mov	r2, r8
	adds	r3, r3, r0
	str	r3, [r2, #8]
	movs	r0, #140
	ldr	r1, [r2, #0]
	lsls	r0, r0, #1
	ldr	r2, [r5, #12]
	bl 0x0200aaec
	movs	r1, #2
	adds	r7, r0, #0
	bl 0x0200aad4
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200ab34
	adds	r3, r7, #0
	movs	r6, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	ldr	r2, [pc, #56]
	ldrh	r3, [r5, #6]
	add	r4, sp, #16
	strh	r3, [r7, #6]
	adds	r3, r7, #0
	adds	r3, #100
	strh	r6, [r3, #0]
	subs	r3, #2
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #44]
	str	r3, [r7, #108]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r7, #48]
	movs	r3, #1
	str	r3, [r4, #0]
	movs	r3, #7
	str	r3, [r4, #4]
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldr	r2, [r3, #8]
	ldr	r3, [pc, #24]
	ldr	r1, [r5, #12]
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [sp, #8]
	b.n	.L_02001794
	.4byte 0x00000000
	.4byte 0x0300021c
	.4byte 0x02009515
	.2byte 0x0000
	.2byte 0xfffa
.L_02001794:
	.2byte 0x2300
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	bl 0x02009f74
	adds	r0, r7, #0
	add	sp, #68
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	sub	sp, #56
	ldr	r7, [r3, #0]
	movs	r3, #7
	ands	r7, r3
	mov	sl, r0
	cmp	r7, #0
	bne.n	.L_0200183a
	add	r6, sp, #16
	movs	r3, #3
	str	r3, [r6, #0]
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	movs	r3, #14
	str	r3, [r6, #4]
	bl 0x0200aa5c
	mov	r2, sl
	lsls	r3, r0, #3
	ldr	r2, [r2, #8]
	adds	r3, r3, r0
	lsrs	r3, r3, #16
	subs	r3, #4
	lsls	r3, r3, #16
	mov	r8, r2
	add	r8, r3
	bl 0x0200aa5c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	lsrs	r3, r3, #16
	movs	r2, #32
	subs	r2, r2, r3
	mov	r3, sl
	ldr	r5, [r3, #12]
	lsls	r2, r2, #16
	adds	r5, r5, r2
	bl 0x0200aa5c
	adds	r3, r0, #0
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsrs	r0, r0, #16
	movs	r2, #160
	lsls	r2, r2, #11
	lsls	r0, r0, #16
	adds	r0, r0, r2
	movs	r1, #10
	bl 0x0200aa3c
	mov	r3, sl
	ldr	r2, [r3, #16]
	movs	r3, #176
	lsls	r3, r3, #12
	str	r0, [sp, #0]
	str	r3, [sp, #8]
	mov	r0, r8
	adds	r1, r5, #0
	movs	r3, #0
	str	r7, [sp, #4]
	str	r6, [sp, #12]
	bl 0x02009f74
.L_0200183a:
	movs	r0, #0
	add	sp, #56
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #9
	bl 0x0200ab7c
	adds	r6, r0, #0
	movs	r0, #10
	bl 0x0200ab7c
	adds	r7, r0, #0
	movs	r0, #23
	bl 0x0200ab7c
	adds	r5, r0, #0
	ldr	r2, [r5, #80]
	movs	r1, #128
	mov	r8, r2
	movs	r2, #248
	movs	r0, #24
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200aba4
	movs	r1, #128
	movs	r2, #248
	movs	r0, #23
	lsls	r1, r1, #18
	lsls	r2, r2, #16
	bl 0x0200aba4
	movs	r1, #236
	movs	r2, #128
	movs	r0, #9
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200aba4
	movs	r1, #138
	movs	r2, #128
	lsls	r2, r2, #17
	movs	r0, #10
	lsls	r1, r1, #18
	bl 0x0200aba4
	movs	r0, #24
	bl 0x0200ab7c
	movs	r3, #85
	movs	r2, #0
	adds	r3, r3, r5
	str	r2, [r0, #24]
	strb	r2, [r3, #0]
	mov	fp, r3
	ldr	r3, [r5, #20]
	movs	r0, #160
	str	r3, [r5, #12]
	movs	r3, #85
	adds	r3, r3, r6
	strb	r2, [r3, #0]
	mov	r9, r3
	ldr	r3, [r6, #20]
	lsls	r0, r0, #4
	str	r3, [r6, #12]
	movs	r3, #85
	adds	r3, r3, r7
	strb	r2, [r3, #0]
	mov	sl, r3
	ldr	r3, [r7, #20]
	adds	r0, #10
	str	r3, [r7, #12]
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02001928
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #208]
	movs	r0, #9
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r2, [pc, #204]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #200]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl 0x0200ab7c
	movs	r1, #4
	bl 0x0200ab4c
	movs	r0, #11
	bl 0x0200ab7c
	movs	r1, #4
	bl 0x0200ab4c
.L_02001928:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02001972
	ldr	r3, [r7, #12]
	ldr	r2, [pc, #132]
	movs	r0, #10
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r2, [pc, #128]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	mov	r2, r8
	ldrh	r3, [r2, #18]
	ldr	r2, [pc, #120]
	adds	r3, r3, r2
	mov	r2, r8
	strh	r3, [r2, #18]
	bl 0x0200ab7c
	movs	r1, #4
	bl 0x0200ab4c
	movs	r0, #12
	bl 0x0200ab7c
	movs	r1, #4
	bl 0x0200ab4c
.L_02001972:
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #10
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020019b2
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #11
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020019b2
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r2, [pc, #56]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #12]
	mov	r2, r9
	movs	r3, #4
	strb	r3, [r2, #0]
	mov	r2, sl
	strb	r3, [r2, #0]
	mov	r2, fp
	strb	r3, [r2, #0]
.L_020019b2:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00066640
	.4byte 0x0001eb80
	.4byte 0xfffd70c0
	.4byte 0x00028f40
	.4byte 0xfffff800
	.4byte 0x00199900
	.2byte 0x8480
	.2byte 0x001b
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #255
	ldrh	r3, [r0, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	sub	sp, #4
	cmp	r3, r2
	beq.n	.L_02001a40
	adds	r7, r0, #0
.L_020019f2:
	ldrh	r3, [r7, #0]
	mov	r8, r3
	mov	r0, r8
	bl 0x0200ab7c
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #89
	movs	r2, #2
	ldrsh	r6, [r7, r2]
	ldrb	r2, [r1, #0]
	movs	r3, #4
	ldrsh	r4, [r7, r3]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	movs	r1, #0
	str	r4, [sp, #0]
	bl 0x0200ab34
	ldr	r4, [sp, #0]
	lsls	r6, r6, #16
	lsls	r4, r4, #16
	lsrs	r4, r4, #16
	lsrs	r6, r6, #16
	adds	r5, #34
	ldrb	r3, [r5, #0]
	adds	r2, r4, #0
	mov	r0, r8
	adds	r1, r6, #0
	adds	r7, #6
	bl 0x02009acc
	movs	r2, #255
	ldrh	r3, [r7, #0]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_020019f2
.L_02001a40:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	adds	r5, r0, #0
	mov	r9, r1
	mov	sl, r2
	movs	r1, #255
	ldr	r2, [r3, #0]
	b.n	.L_02001ab4
.L_02001a64:
	ldrh	r3, [r5, #0]
	movs	r1, #26
	ldrsh	r7, [r2, r1]
	cmp	r7, r3
	bne.n	.L_02001ab0
	adds	r0, r7, #0
	bl 0x0200ab7c
	adds	r5, #2
	ldrh	r2, [r5, #0]
	mov	r3, sl
	adds	r6, r0, #0
	mov	r8, r2
	ldrh	r5, [r5, #2]
	cmp	r3, #7
	bgt.n	.L_02001a8c
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #64]
	adds	r3, r3, r1
	str	r3, [r6, #28]
.L_02001a8c:
	mov	r2, r9
	cmp	r2, #1
	bne.n	.L_02001abe
	adds	r0, r5, #0
	bl 0x0200aab4
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r3, [r3, #0]
	adds	r0, r7, #0
	mov	r1, r8
	adds	r2, r5, #0
	bl 0x02009acc
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #28]
	b.n	.L_02001abe
.L_02001ab0:
	adds	r5, #6
	movs	r1, #255
.L_02001ab4:
	ldrh	r3, [r5, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_02001a64
.L_02001abe:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0xe100
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	adds	r5, r3, #0
	mov	r8, r2
	adds	r6, r1, #0
	bl 0x0200ab7c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	lsls	r3, r5, #3
	subs	r3, r3, r5
	movs	r1, #156
	lsls	r1, r1, #1
	lsls	r3, r3, #3
	adds	r3, r3, r1
	ldr	r5, [r2, r3]
	adds	r7, r0, #0
	bl 0x0200ac1c
	lsls	r0, r0, #2
	adds	r5, r5, r0
	mov	r0, r8
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02001b40
	adds	r1, r7, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #0
	strb	r3, [r5, #2]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	cmp	r6, #1
	beq.n	.L_02001b2c
	cmp	r6, #1
	bcc.n	.L_02001b22
	cmp	r6, #2
	beq.n	.L_02001b36
	b.n	.L_02001b6e
.L_02001b22:
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200aad4
	b.n	.L_02001b6e
.L_02001b2c:
	adds	r0, r7, #0
	movs	r1, #4
	bl 0x0200aad4
	b.n	.L_02001b6e
.L_02001b36:
	adds	r0, r7, #0
	movs	r1, #6
	bl 0x0200aad4
	b.n	.L_02001b6e
.L_02001b40:
	movs	r3, #255
	strb	r3, [r5, #2]
	cmp	r6, #1
	beq.n	.L_02001b5c
	cmp	r6, #1
	bcc.n	.L_02001b52
	cmp	r6, #2
	beq.n	.L_02001b66
	b.n	.L_02001b6e
.L_02001b52:
	adds	r0, r7, #0
	movs	r1, #1
	bl 0x0200aad4
	b.n	.L_02001b6e
.L_02001b5c:
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200aad4
	b.n	.L_02001b6e
.L_02001b66:
	adds	r0, r7, #0
	movs	r1, #5
	bl 0x0200aad4
.L_02001b6e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r3, [pc, #380]
	adds	r2, r1, #0
	adds	r2, #228
	ldr	r0, [r2, #0]
	ldr	r2, [r2, #4]
	ands	r0, r3
	ands	r2, r3
	ldr	r3, [pc, #372]
	mov	sl, r0
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	mov	r8, r2
	ldr	r2, [pc, #364]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	sub	sp, #8
	lsrs	r3, r3, #5
	str	r3, [sp, #4]
	ldr	r6, [pc, #356]
	ldr	r3, [r1, #0]
	movs	r1, #0
	ldr	r3, [r3, #4]
	mov	r9, r1
	str	r3, [sp, #0]
	ldr	r3, [pc, #348]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r9, r3
	blt.n	.L_02001bc6
	b.n	.L_02001cfa
.L_02001bc6:
	ldr	r2, [pc, #340]
	mov	r0, r9
	lsls	r3, r0, #2
	ldr	r5, [r2, r3]
	cmp	r5, #0
	bne.n	.L_02001bd4
	b.n	.L_02001cea
.L_02001bd4:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	bne.n	.L_02001bdc
	b.n	.L_02001cea
.L_02001bdc:
	mov	r1, sl
	subs	r0, r3, r1
	ldr	r2, [sp, #0]
	ldr	r3, [r5, #12]
	movs	r1, #128
	subs	r3, r3, r2
	ldr	r2, [r5, #16]
	lsls	r1, r1, #12
	adds	r3, r3, r1
	mov	r1, r8
	subs	r2, r2, r1
	ldr	r1, [sp, #0]
	subs	r2, r2, r1
	subs	r4, r2, r3
	adds	r3, r3, r2
	asrs	r3, r3, #16
	adds	r3, #58
	mov	fp, r3
	ldr	r3, [pc, #284]
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	adds	r3, r5, #0
	mov	ip, r2
	asrs	r1, r0, #16
	mov	r0, ip
	adds	r3, #100
	asrs	r2, r4, #16
	cmp	r0, #0
	bne.n	.L_02001c52
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #7
	movs	r1, #167
	adds	r4, r2, #0
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #16
	cmp	r3, r1
	bhi.n	.L_02001cea
	movs	r2, #16
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_02001cea
	cmp	r4, #239
	bgt.n	.L_02001cea
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	mov	r3, ip
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #212]
	b.n	.L_02001c8e
.L_02001c52:
	movs	r0, #0
	ldrsh	r7, [r3, r0]
	adds	r0, r1, #0
	adds	r3, r1, #0
	movs	r1, #175
	adds	r4, r2, #0
	adds	r3, #23
	lsls	r1, r1, #1
	subs	r0, #8
	subs	r4, #64
	cmp	r3, r1
	bhi.n	.L_02001cea
	movs	r2, #64
	negs	r2, r2
	cmp	r4, r2
	ble.n	.L_02001cea
	cmp	r4, #175
	bgt.n	.L_02001cea
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r0, r3
	movs	r3, #255
	adds	r1, r6, #0
	ands	r4, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r0, #16
	orrs	r4, r3
	ldr	r3, [pc, #152]
.L_02001c8e:
	movs	r2, #128
	orrs	r4, r3
	stmia	r1!, {r4}
	ldr	r0, [sp, #4]
	lsls	r3, r7, #3
	adds	r3, r0, r3
	lsls	r2, r2, #4
	orrs	r3, r2
	str	r3, [r1, #0]
	ldr	r3, [pc, #136]
	movs	r0, #1
	ldrh	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_02001ccc
	adds	r0, r5, #0
	bl 0x0200ac2c
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r6, #9]
	negs	r1, r1
	adds	r2, r1, #0
	lsls	r0, r0, #2
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r6, #9]
	b.n	.L_02001ce0
.L_02001ccc:
	movs	r3, #3
	ands	r3, r2
	movs	r0, #13
	ldrb	r2, [r6, #9]
	negs	r0, r0
	adds	r1, r0, #0
	lsls	r3, r3, #2
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r6, #9]
.L_02001ce0:
	adds	r0, r6, #0
	mov	r1, fp
	bl 0x0200aaa4
	adds	r6, #12
.L_02001cea:
	ldr	r3, [pc, #44]
	movs	r1, #1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	add	r9, r1
	cmp	r9, r3
	bge.n	.L_02001cfa
	b.n	.L_02001bc6
.L_02001cfa:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5fc
	lsls	r0, r0, #8
	adds	r6, #224
	lsls	r0, r0, #8
	.2byte 0xb640
	lsls	r0, r0, #8
	push	{r1, r2, r3, r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	.2byte 0xb600
	lsls	r0, r0, #8
	.2byte 0xb700
	lsls	r0, r0, #8
	movs	r0, #0
	ands	r0, r0
	add	r0, pc, #0
	.2byte 0xc000
	.2byte 0xb702
	lsls	r0, r0, #8
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200aa7c
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200aa8c
	ldr	r5, [pc, #76]
	bl 0x0200aa9c
	movs	r1, #192
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200aa94
	adds	r0, r6, #0
	bl 0x0200aa84
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200aa54
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b600
	.2byte 0xac9c
	.2byte 0x0200
	push	{r2, r3, r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	ldr	r3, [sp, #468]
	lsls	r0, r0, #8
	push	{r1, r2, r3, r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	.2byte 0xb700
	lsls	r0, r0, #8
	.2byte 0xb702
	lsls	r0, r0, #8
	push	{r5, r6, lr}
	movs	r0, #192
	lsls	r0, r0, #4
	bl 0x0200aa7c
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #76]
	bl 0x0200aa8c
	ldr	r5, [pc, #76]
	bl 0x0200aa9c
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200aa94
	adds	r0, r6, #0
	bl 0x0200aa84
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl 0x0200aa54
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #16]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #8]
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b600
	.2byte 0xadff
	.2byte 0x0200
	push	{r2, r3, r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	ldr	r3, [sp, #468]
	lsls	r0, r0, #8
	push	{r1, r2, r3, r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	.2byte 0xb700
	lsls	r0, r0, #8
	.2byte 0xb702
	lsls	r0, r0, #8
	push	{r5, r6, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	bl 0x0200aa7c
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	movs	r1, #64
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c31
	ldr	r0, [pc, #80]
	bl 0x0200aa8c
	ldr	r5, [pc, #80]
	bl 0x0200aa9c
	movs	r1, #128
	strh	r0, [r5, #0]
	lsls	r0, r0, #16
	adds	r2, r6, #0
	lsls	r1, r1, #4
	asrs	r0, r0, #16
	bl 0x0200aa94
	adds	r0, r6, #0
	bl 0x0200aa84
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	bl 0x0200aa54
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	.2byte 0xe015
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x03000258
	.4byte 0x0200b600
	.2byte 0xb02e
	.2byte 0x0200
	push	{r2, r3, r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	ldr	r3, [sp, #468]
	lsls	r0, r0, #8
	push	{r1, r2, r3, r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	.2byte 0xb700
	lsls	r0, r0, #8
	.2byte 0xb702
	lsls	r0, r0, #8
.L_02001eb4:
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r1, #0
	bl 0x0200ab7c
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_02001ede
	adds	r3, r4, #0
	adds	r3, #100
	strh	r5, [r3, #0]
	ldr	r1, [pc, #16]
	ldr	r0, [pc, #20]
	ldrh	r2, [r1, #0]
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	adds	r2, #1
	lsls	r3, r3, #2
	str	r4, [r0, r3]
	strh	r2, [r1, #0]
.L_02001ede:
	pop	{r5, pc}
	push	{r1, r2, r3, r4, r5, r6, r7, lr}
	lsls	r0, r0, #8
	.2byte 0xb600
	lsls	r0, r0, #8
	ldr	r3, [pc, #4]
	strh	r0, [r3, #0]
	bx	lr
	.2byte 0x0000
	.2byte 0xb702
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_02001f38
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001f38
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_02001f38:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x0200ab7c
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02001fbc
	cmp	r7, #0
	beq.n	.L_02001fbc
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02001fc4
.L_02001fbc:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02001fc4:
	mov	r3, sl
	bl 0x0200aaec
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02001fd2
	b.n	.L_0200211e
.L_02001fd2:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200aad4
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x0200aae4
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200ab34
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02009ef4
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200211e
	cmp	r7, #0
	beq.n	.L_0200211e
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02002054
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x0200abb4
.L_02002054:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02002074
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02009ef4
.L_02002074:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_02002088
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_02002088:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020020ce
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020020b6
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200aa3c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020020c8
.L_020020b6:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200aa3c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020020c8:
	bl 0x0200aa3c
	str	r0, [r6, #52]
.L_020020ce:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020020ea
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200aad4
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x0200aae4
.L_020020ea:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020020fc
.L_020020f6:
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_020020fc:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200210e
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_0200210e:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200211e
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_0200211e:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r2, r3, r5, r7, lr}
	lsls	r0, r0, #8
	ldr	r7, [sp, #244]
	lsls	r0, r0, #8
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r4, [pc, #268]
	movs	r1, #1
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	negs	r1, r1
	sub	sp, #4
	cmp	r3, r1
	beq.n	.L_02002248
	lsls	r3, r3, #3
	adds	r3, r3, r4
	adds	r3, #32
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	str	r4, [sp, #0]
	bl 0x0200ab7c
	mov	r1, r8
	ldr	r3, [r0, #8]
	movs	r5, #0
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	ldr	r4, [sp, #0]
	cmp	r3, r2
	bne.n	.L_02002188
	ldr	r3, [r0, #16]
	movs	r5, #2
	ldrsh	r2, [r1, r5]
	asrs	r3, r3, #20
	cmp	r3, r2
	beq.n	.L_02002190
.L_02002188:
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r4, #12]
.L_02002190:
	movs	r0, #12
	ldrsh	r3, [r4, r0]
	movs	r2, #1
	negs	r2, r2
	ldr	r1, [pc, #192]
	cmp	r3, r2
	beq.n	.L_02002248
	movs	r5, #14
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_02002248
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	str	r4, [sp, #0]
	adds	r3, r2, #0
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r5, [r3, #4]
	ldr	r3, [r2, #0]
	ands	r0, r1
	ands	r5, r1
	ldr	r6, [r3, #4]
	movs	r1, #16
	ldrsh	r3, [r4, r1]
	ldr	r2, [pc, #156]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r3
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	lsls	r1, r1, #20
	subs	r7, r1, r0
	movs	r0, #2
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	lsls	r2, r2, #20
	bl 0x0200ab14
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	subs	r0, r0, r6
	lsls	r3, r3, #20
	subs	r3, r3, r5
.L_020021f0:
	subs	r3, r3, r6
	subs	r2, r3, r0
	asrs	r7, r7, #16
	adds	r0, r0, r3
	asrs	r0, r0, #16
	adds	r3, r7, #0
	movs	r5, #167
	asrs	r2, r2, #16
	adds	r1, r0, #0
	adds	r3, #15
	lsls	r5, r5, #1
	adds	r2, #14
	adds	r1, #58
	ldr	r4, [sp, #0]
	cmp	r3, r5
	bhi.n	.L_02002248
	movs	r0, #15
	negs	r0, r0
	cmp	r2, r0
	blt.n	.L_02002248
	cmp	r2, #239
	bgt.n	.L_02002248
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r7, r3
	movs	r3, #255
	ands	r2, r3
	movs	r3, #0
	str	r3, [r4, #20]
	lsls	r3, r7, #16
	orrs	r2, r3
	ldr	r3, [pc, #48]
	adds	r0, r4, #0
	orrs	r2, r3
	movs	r3, #128
	str	r2, [r4, #24]
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r4, #28]
	adds	r0, #20
	bl 0x0200aaa4
.L_02002248:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b704
	.4byte 0x02000240
	.4byte 0xffff0000
	.4byte 0x020036e0
	.2byte 0x8800
	.2byte 0x8000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r0, [sp, #44]
	ldr	r0, [pc, #540]
	str	r1, [sp, #40]
	mov	r8, r0
	movs	r1, #32
	add	r1, r8
	mov	r9, r1
	mov	ip, r9
	adds	r5, r2, #0
	mov	r2, ip
	adds	r6, r3, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #520]
	movs	r1, #4
	ldr	r7, [sp, #80]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xa80b
	ldrh	r0, [r0, #0]
	mov	r1, r8
	strh	r0, [r1, #4]
	add	r1, sp, #40
	ldrh	r1, [r1, #0]
	mov	r3, r8
	strh	r1, [r3, #0]
	strh	r5, [r3, #2]
	movs	r3, #255
	lsls	r3, r3, #8
	mov	r5, r8
	mov	r0, r8
	adds	r3, #255
	mov	r1, r8
	strh	r6, [r5, #6]
	movs	r2, #0
	strh	r7, [r0, #8]
	strh	r3, [r1, #12]
	mov	r3, r8
	strh	r2, [r3, #10]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r2, #132
	mov	ip, r3
	lsls	r2, r2, #1
	mov	r1, ip
	add	r2, ip
	adds	r1, #236
	ldr	r0, [r1, #0]
	ldr	r3, [r2, #8]
	ldr	r5, [r2, #48]
	adds	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #32]
	adds	r1, #4
	ldr	r3, [r2, #12]
	ldr	r2, [r1, #0]
	adds	r3, r3, r2
	asrs	r3, r3, #20
	str	r3, [sp, #28]
	mov	r3, ip
	adds	r3, #244
	ldr	r3, [r3, #0]
	subs	r3, r3, r0
	asrs	r3, r3, #20
	str	r3, [sp, #24]
	mov	r3, ip
	adds	r3, #248
	ldr	r3, [r3, #0]
	asrs	r0, r0, #20
	subs	r3, r3, r2
	asrs	r2, r2, #20
	lsls	r2, r2, #7
	adds	r2, r2, r0
	lsls	r2, r2, #2
	asrs	r3, r3, #20
	adds	r5, r5, r2
	movs	r0, #0
	str	r3, [sp, #20]
	str	r5, [sp, #36]
	str	r0, [sp, #12]
	cmp	r0, r3
	bge.n	.L_020023ec
.L_0200231c:
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #36]
	ldr	r5, [sp, #24]
	lsls	r3, r1, #9
	adds	r2, r2, r3
	movs	r3, #0
	mov	fp, r2
	str	r3, [sp, #16]
	cmp	r3, r5
	bge.n	.L_020023e0
.L_02002330:
	mov	r0, fp
	ldrb	r5, [r0, #2]
	cmp	r5, #0
	beq.n	.L_020023d0
	ldr	r1, [sp, #44]
	cmp	r5, r1
	bcc.n	.L_020023d0
	adds	r1, #1
	mov	sl, r1
	cmp	r5, sl
	bhi.n	.L_020023d0
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #32]
	mov	r0, r9
	adds	r7, r2, r3
	strh	r7, [r0, #0]
	ldr	r1, [sp, #12]
	ldr	r2, [sp, #28]
	add	r0, sp, #40
	ldrh	r0, [r0, #0]
	adds	r6, r1, r2
	mov	r3, r9
	mov	r1, r9
	strh	r6, [r3, #2]
	strh	r0, [r1, #4]
	ldr	r1, [sp, #40]
	movs	r0, #10
	adds	r1, #1
	adds	r0, #255
	str	r1, [sp, #40]
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02002384
	cmp	r5, sl
	bne.n	.L_020023c2
	mov	r3, r9
	movs	r2, #4
	ldrsh	r0, [r3, r2]
	bl 0x0200aab4
	b.n	.L_020023c2
.L_02002384:
	mov	r1, r9
	movs	r5, #4
	ldrsh	r0, [r1, r5]
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_020023c2
	mov	r2, r8
	ldrh	r4, [r2, #6]
	ldrh	r5, [r2, #8]
	movs	r3, #8
	ldrsh	r1, [r2, r3]
	movs	r3, #6
	ldrsh	r0, [r2, r3]
	movs	r2, #64
	adds	r3, r2, #0
	ands	r3, r4
	ands	r2, r5
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	asrs	r3, r3, #16
	asrs	r2, r2, #16
	orrs	r7, r3
	orrs	r6, r2
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200ab1c
.L_020023c2:
	mov	r0, r8
	ldrh	r3, [r0, #10]
	mov	r1, r8
	adds	r3, #1
	strh	r3, [r1, #10]
	movs	r5, #8
	add	r9, r5
.L_020023d0:
	ldr	r2, [sp, #16]
	ldr	r5, [sp, #24]
	adds	r2, #1
	movs	r3, #4
	str	r2, [sp, #16]
	add	fp, r3
	cmp	r2, r5
	blt.n	.L_02002330
.L_020023e0:
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #12]
	cmp	r0, r1
	blt.n	.L_0200231c
.L_020023ec:
	movs	r0, #10
	adds	r0, #255
	bl 0x0200aaac
	cmp	r0, #0
	beq.n	.L_02002444
	ldr	r3, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ab7c
	ldr	r3, [r0, #8]
	movs	r2, #0
	asrs	r4, r3, #20
	ldr	r3, [r0, #16]
	mov	r0, r8
	asrs	r1, r3, #20
	ldr	r3, [sp, #8]
	mov	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	cmp	r2, r3
	bge.n	.L_02002444
.L_0200241e:
	mov	r0, r9
	movs	r5, #0
	ldrsh	r3, [r0, r5]
	cmp	r3, r4
	bne.n	.L_02002434
	movs	r5, #2
	ldrsh	r3, [r0, r5]
	cmp	r3, r1
	bne.n	.L_02002434
	mov	r0, r8
	strh	r2, [r0, #12]
.L_02002434:
	movs	r3, #8
	mov	r0, r8
.L_02002438:
	add	r9, r3
	movs	r5, #10
	ldrsh	r3, [r0, r5]
	adds	r2, #1
	cmp	r2, r3
	blt.n	.L_0200241e
.L_02002444:
	movs	r0, #128
	lsls	r0, r0, #1
	bl 0x0200aa7c
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #63
.L_02002452:
	ldr	r3, [pc, #80]
	subs	r2, #1
	stmia	r1!, {r3}
	cmp	r2, #0
	bge.n	.L_02002452
	bl 0x0200aa9c
	mov	r1, r8
	strh	r0, [r1, #16]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r2, r5, #0
	lsls	r1, r1, #1
	asrs	r0, r0, #16
	bl 0x0200aa94
	adds	r0, r5, #0
	bl 0x0200aa84
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl 0x0200aa54
	mov	r3, r8
	movs	r2, #10
	ldrsh	r0, [r3, r2]
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0200b704
	.4byte 0x03000258
	.4byte 0x02000240
	.4byte 0x11111111
	.2byte 0xa13d
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	ldr	r1, [pc, #116]
	movs	r2, #133
	mov	r8, r1
	lsls	r2, r2, #2
	add	r8, r2
	mov	r3, r8
	ldr	r0, [r3, #0]
	bl 0x0200ab7c
	mov	r1, r8
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #16]
	mov	sl, r0
	movs	r2, #128
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #10
	bl 0x0200ab84
	asrs	r5, r5, #20
	mov	r2, r8
	asrs	r6, r6, #20
	ldr	r0, [r2, #0]
	lsls	r1, r5, #4
	lsls	r2, r6, #4
	adds	r1, #8
	adds	r2, #8
	bl 0x0200ab94
	movs	r0, #1
	bl 0x0200aa4c
	movs	r3, #128
	lsls	r3, r3, #12
	lsls	r5, r5, #20
	lsls	r6, r6, #20
	adds	r5, r5, r3
	mov	r1, sl
	adds	r6, r6, r3
	ldr	r2, [r1, #12]
	adds	r3, r6, #0
	adds	r1, r5, #0
	mov	r0, sl
	bl 0x0200aaf4
	movs	r0, #4
	bl 0x0200ab64
	bl 0x0200abdc
	ldr	r2, [pc, #20]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #100]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	mov	r8, r0
	ldr	r0, [r3, #0]
	sub	sp, #8
	bl 0x0200ab7c
	mov	r2, r8
	ldrh	r1, [r2, #6]
	movs	r2, #64
	ldr	r6, [r0, #8]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	asrs	r6, r6, #20
	orrs	r6, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r0, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200a4ac
	movs	r0, #161
	bl 0x0200ac5c
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #1
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ab1c
	movs	r0, #12
	bl 0x0200ab64
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r1, [pc, #164]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r1, r1, r2
	mov	r8, r0
	ldr	r0, [r1, #0]
	sub	sp, #8
	mov	sl, r1
	bl 0x0200ab7c
	adds	r6, r0, #0
	ldr	r3, [r6, #8]
	movs	r2, #64
	asrs	r7, r3, #20
	mov	r3, r8
	ldrh	r1, [r3, #6]
	adds	r3, r2, #0
	ands	r3, r1
	lsls	r3, r3, #16
	mov	r1, r8
	asrs	r3, r3, #16
	orrs	r7, r3
	ldrh	r3, [r1, #8]
	ldr	r5, [r6, #16]
	ands	r2, r3
	lsls	r2, r2, #16
	asrs	r2, r2, #16
	asrs	r5, r5, #20
	orrs	r5, r2
	bl 0x0200a4ac
	movs	r0, #229
	bl 0x0200ac5c
	mov	r3, r8
	movs	r2, #8
	ldrsh	r1, [r3, r2]
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	adds	r1, #2
	movs	r2, #1
	movs	r3, #1
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200ab1c
	movs	r0, #12
	bl 0x0200ab64
	movs	r3, #128
	ldr	r2, [pc, #56]
	lsls	r3, r3, #7
	strh	r3, [r6, #6]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	mov	r3, sl
	ldr	r0, [r3, #0]
	bl 0x0200ab7c
	movs	r1, #0
	bl 0x0200ab34
	movs	r2, #226
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r3, r3, #19
	adds	r2, #255
	adds	r3, #74
	strh	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #16]
	movs	r7, #0
	orrs	r3, r2
	strh	r3, [r1, #0]
	mov	r2, sl
	b.n	.L_02002658
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00008000
	.2byte 0x0240
	.2byte 0x0200
.L_02002658:
	movs	r3, #1
	mov	r1, r8
	strh	r3, [r1, #14]
	ldr	r0, [r2, #0]
	movs	r1, #28
	bl 0x0200abac
	movs	r0, #16
	bl 0x0200ab64
.L_0200266c:
	cmp	r7, #5
	bne.n	.L_02002676
	movs	r0, #204
	bl 0x0200ac5c
.L_02002676:
	ldr	r3, [r6, #24]
	ldr	r1, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r3, r3, r1
	str	r3, [r6, #24]
	ldr	r3, [r6, #28]
	ldr	r1, [pc, #88]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	adds	r7, #1
	bl 0x0200aa4c
	cmp	r7, #39
	ble.n	.L_0200266c
	ldr	r3, [pc, #68]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x0200ab7c
	movs	r3, #0
	adds	r0, #84
	strb	r3, [r0, #0]
	mov	r1, r8
	strh	r3, [r1, #14]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #93
	str	r2, [r3, #0]
	bl 0x0200abfc
	bl 0x0200ac04
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0xfffffd00
	.4byte 0xffff6667
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200aa3c
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02002714
	adds	r3, #15
.L_02002714:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #376]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	sub	sp, #68
	bl 0x0200ab7c
	adds	r7, r0, #0
	bl 0x0200ab6c
	movs	r0, #0
	bl 0x0200ac0c
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	negs	r1, r1
	negs	r2, r2
	movs	r3, #0
	negs	r0, r0
	bl 0x0200abcc
	bl 0x0200aafc
	movs	r0, #1
	bl 0x0200aa4c
	movs	r3, #130
	lsls	r3, r3, #16
	str	r3, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r5, r7, #0
	str	r3, [r7, #72]
	adds	r5, #85
	movs	r3, #0
	str	r3, [r7, #68]
	strb	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r4, #214
	lsls	r4, r4, #1
	movs	r2, #128
	adds	r3, r3, r4
	lsls	r2, r2, #1
	str	r2, [r3, #0]
	bl 0x0200abf4
	bl 0x0200ac04
	movs	r0, #204
	bl 0x0200ac5c
	movs	r3, #3
	strb	r3, [r5, #0]
	movs	r0, #24
	bl 0x0200ab64
	add	r2, sp, #28
	movs	r3, #7
	str	r3, [r2, #4]
	ldr	r3, [pc, #256]
	mov	r8, r2
	str	r3, [r2, #36]
	movs	r3, #163
	lsls	r3, r3, #8
	adds	r3, #215
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	movs	r3, #0
	mov	sl, r3
.L_020027d6:
	mov	r4, sl
	lsls	r5, r4, #12
	adds	r0, r5, #0
	bl 0x0200aa6c
	add	r6, sp, #16
	movs	r3, #0
	str	r0, [r6, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	bl 0x0200aa64
	ldr	r3, [r6, #0]
	str	r0, [r6, #8]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200aa5c
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200aa5c
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r4, [pc, #176]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r4
	ldr	r4, [r6, #4]
	str	r5, [r6, #8]
	ldr	r2, [r7, #16]
	ldr	r3, [r6, #0]
	ldr	r0, [r7, #8]
	ldr	r1, [r7, #12]
	str	r4, [sp, #0]
	ldr	r4, [pc, #156]
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	mov	r4, r8
	str	r4, [sp, #12]
	bl 0x02009f74
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #16
	bls.n	.L_020027d6
	movs	r0, #188
	bl 0x0200ac5c
	ldr	r5, [pc, #112]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r5, r5, r4
	movs	r1, #2
	ldr	r0, [r5, #0]
	adds	r1, #255
	bl 0x0200abbc
	ldr	r0, [r5, #0]
.L_02002862:
	movs	r1, #49
	bl 0x0200abac
	movs	r0, #160
	movs	r1, #160
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200ab3c
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r0, r0
	negs	r1, r1
	bl 0x0200ab3c
	bl 0x0200ab44
	movs	r1, #128
	ldr	r0, [r5, #0]
	lsls	r1, r1, #1
	bl 0x0200abbc
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #72]
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r7, #68]
	movs	r0, #10
	bl 0x0200ab64
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl 0x0200abac
	bl 0x0200ab74
	add	sp, #68
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200a6e5
	.4byte 0xffffa000
	.4byte 0xffffd000
	.2byte 0x0001
	.2byte 0x0109
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #156]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x0200ab7c
	ldr	r3, [r0, #8]
	ldr	r6, [pc, #144]
	asrs	r3, r3, #20
	mov	r8, r3
	ldr	r3, [r0, #16]
	adds	r5, r6, #0
	asrs	r3, r3, #20
	mov	sl, r3
	movs	r1, #10
	ldrsh	r3, [r6, r1]
	movs	r7, #0
	adds	r5, #32
	ldrh	r2, [r6, #10]
	cmp	r7, r3
	bge.n	.L_02002970
.L_02002908:
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, r8
	bne.n	.L_02002964
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	cmp	r3, sl
	bne.n	.L_02002964
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	bl 0x0200aaac
	cmp	r0, #0
	bne.n	.L_02002938
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl 0x0200a534
	movs	r3, #4
	ldrsh	r0, [r5, r3]
	bl 0x0200aab4
	strh	r7, [r6, #12]
	b.n	.L_02002970
.L_02002938:
	movs	r1, #12
	ldrsh	r3, [r6, r1]
	cmp	r7, r3
	beq.n	.L_02002970
	adds	r0, r6, #0
	adds	r1, r5, #0
	strh	r7, [r6, #12]
	bl 0x0200a5a4
	movs	r2, #2
	ldrsh	r0, [r6, r2]
	mov	r1, r8
	bl 0x0200aacc
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r1, sl
	adds	r0, #8
	bl 0x0200aacc
	movs	r0, #1
	b.n	.L_02002972
.L_02002964:
	lsls	r3, r2, #16
	adds	r7, #1
	asrs	r3, r3, #16
	adds	r5, #8
	cmp	r7, r3
	blt.n	.L_02002908
.L_02002970:
	movs	r0, #0
.L_02002972:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0xb704
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	ldr	r3, [pc, #156]
	str	r2, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	fp, r1
	bl 0x0200ab7c
	movs	r3, #192
	ldr	r5, [pc, #140]
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r6, r0, #0
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	mov	sl, r3
	bl 0x0200aac4
	adds	r7, r0, #0
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	adds	r0, #8
	bl 0x0200aac4
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_020029d2
	cmp	r0, #0
	beq.n	.L_02002a26
.L_020029d2:
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #0
	bl 0x0200aacc
	movs	r3, #2
	ldrsh	r0, [r5, r3]
	movs	r1, #0
	adds	r0, #8
	bl 0x0200aacc
	mov	r3, r9
	adds	r2, r7, r3
	mov	r3, r8
	movs	r1, #128
	add	r3, fp
	lsls	r1, r1, #12
	lsls	r3, r3, #20
	adds	r3, r3, r1
	str	r3, [r6, #16]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, sl
	lsls	r2, r2, #20
	adds	r2, r2, r1
	ldr	r1, [r3, #0]
	str	r2, [r6, #8]
	str	r2, [r1, #8]
	ldr	r3, [r6, #16]
	str	r3, [r1, #16]
	bl 0x0200aafc
	bl 0x0200a73c
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #0]
	str	r2, [r3, #0]
.L_02002a26:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x0200b704
	.irp EntryTarget, 0x03000528, 0x03000508, 0x080000c1, 0x080000d1, 0x080000f9, 0x08000119, 0x08000121, 0x08000129, 0x08000169, 0x08000179, 0x080001a9, 0x080001c9, 0x080001d1, 0x080001e9, 0x080003c9, 0x080003d1, 0x080003d9, 0x080003e9, 0x080003f1, 0x08020091, 0x08020099, 0x080200a9, 0x080200c1, 0x080200e9, 0x08020121, 0x08020149, 0x08020151, 0x080201c1, 0x080201e1, 0x080201e9, 0x080201f1, 0x08020219, 0x08020229, 0x08020231, 0x08020279, 0x08020361, 0x08020369, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8089, 0x080c8099, 0x080c80b1, 0x080c80c9, 0x080c80e9, 0x080c80f9, 0x080c8119, 0x080c8171, 0x080c8219, 0x080c8229, 0x080c8239, 0x080c8241, 0x080c8259, 0x080c8289, 0x080c82e1, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c85c1, 0x080c8711, 0x080c8749, 0x080c87e9, 0x080c8831, 0x080c8841, 0x080c8849, 0x080c8851, 0x080c8859, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.section .rodata,"a",%progbits
	.4byte 0x00100008
	.4byte 0x0010000a
	.4byte 0x0010000b
	.4byte 0x0010000c
	.4byte 0x0010000e
	.4byte 0x0010000f
	.4byte 0x0012000a
	.4byte 0x0012000c
	.4byte 0x0012000d
	.4byte 0x00140008
	.4byte 0x0014000a
	.4byte 0x0014000d
	.4byte 0x0014000f
	.4byte 0x0000ffff
	.4byte 0x06345d01
	.4byte 0x08003b01
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte 0x08071768
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte 0x020010df
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte 0x02007800
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte 0x0800200e
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte 0x08076918
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.4byte 0x00000002
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte 0x08e7ee5a
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte 0x087d19d7
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.4byte 0x01000000
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00010011
	.4byte 0x00090a16
	.4byte 0x0a170001
	.4byte 0x0000ffff
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000104
	.4byte 0x00101106
	.4byte 0x00203104
	.4byte 0x00302104
	.4byte 0x00405104
	.4byte 0x00504104
	.4byte 0x00607104
	.4byte 0x00706104
	.4byte 0x00808107
	.4byte 0x0090a104
	.4byte 0x00a09104
	.4byte 0x00b0c104
	.4byte 0x00c0b104
	.4byte 0x000001ff
	.4byte 0xffff0154
	.4byte 0x00000001
	.4byte 0x02340000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00026000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00026000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01020000
	.4byte 0xffff013c
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00026000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0x007a00f6
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
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
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000031
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000202
	.4byte 0x1a1a001e
	.4byte 0x02008e21
	.4byte 0x00000002
	.4byte 0x0a1b0014
	.4byte 0x0200813d
	.4byte 0x00000002
	.4byte 0x0a1b0015
	.4byte 0x02008161
	.4byte 0x00008602
	.4byte 0xffff0028
	.4byte 0x02008ced
	.4byte 0x0000c602
	.4byte 0xffff0029
	.4byte 0x02008ced
	.4byte 0x00000602
	.4byte 0xffff002a
	.4byte 0x02008ced
	.4byte 0x00004602
	.4byte 0xffff002b
	.4byte 0x02008ced
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte 0x02008039
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte 0x02008779
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte 0x020087a5
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte 0x02008779
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x020087a5
	.4byte 0x00000008
	.4byte 0xffff0023
	.4byte 0x02008779
	.4byte 0x00000009
	.4byte 0xffff0023
	.4byte 0x020087a5
	.4byte 0x10008c15
	.4byte 0xffff0014
	.4byte 0x02008779
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x020087a5
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0x0a1b0010
	.4byte 0x02008185
	.4byte 0x80008615
	.4byte 0x0a160011
	.4byte 0x020089d9
	.4byte 0x50008615
	.4byte 0x0a160011
	.4byte 0x020089e5
	.4byte 0x00008615
	.4byte 0x0a160011
	.4byte 0x02008a75
	.4byte 0x50008615
	.4byte 0x0a170009
	.4byte 0x020089e5
	.4byte 0x00008615
	.4byte 0x0a170009
	.4byte 0x02008c7d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x0200b110
	.4byte 0x0200b14c
	.4byte 0x0200b188
