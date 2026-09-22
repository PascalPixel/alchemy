.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_080030f8, 0x080030f8
	.set sub_08015120, 0x08015120
	.set sub_08015140, 0x08015140
	.set sub_08077008, 0x08077008
	.set sub_08077018, 0x08077018
	.set sub_08077028, 0x08077028
	.set sub_08077058, 0x08077058
	.set sub_08077068, 0x08077068
	.set sub_08077228, 0x08077228
	.set sub_08077230, 0x08077230
	.set sub_08077248, 0x08077248
	.set sub_08077298, 0x08077298
	.set sub_080772a0, 0x080772a0
	.set sub_080b04dc, 0x080b04dc
	.set sub_080b0574, 0x080b0574
	.set sub_080b0634, 0x080b0634
	.set sub_080b0a6c, 0x080b0a6c
	.set sub_080b10cc, 0x080b10cc
	.set sub_080b11c4, 0x080b11c4
	.set sub_080b1470, 0x080b1470
	.set sub_080b1868, 0x080b1868
	.set sub_080b196c, 0x080b196c
	.set sub_080b1dec, 0x080b1dec
	.set sub_080b20e8, 0x080b20e8
	.set sub_080f9010, 0x080f9010
	.global Overlay_080b2328
Overlay_080b2328:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #12]
	ldr	r3, [pc, #392]
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	str	r3, [sp, #8]
	bl	sub_08077008
	ldr	r2, [sp, #12]
	lsls	r2, r2, #1
	str	r2, [sp, #4]
	adds	r6, r2, #0
	adds	r6, #216
	ldrh	r3, [r0, r6]
	ldr	r2, [pc, #372]
	ands	r2, r3
	mov	sl, r2
	mov	r8, r0
	mov	r0, sl
	bl	sub_08077018
	adds	r5, r0, #0
	ldrb	r1, [r5, #2]
	adds	r0, r7, #0
	bl	sub_08077228
	str	r0, [sp, #0]
	mov	r3, r8
	ldrh	r0, [r3, r6]
	bl	sub_080b20e8
	ldrb	r1, [r5, #12]
	mov	r9, r0
	cmp	r1, #2
	beq.n	.L_080b238c
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #324]
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b238c:
	mov	r3, r8
	ldrh	r2, [r3, r6]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b23aa
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #300]
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b23aa:
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b23cc
	ldrb	r3, [r5, #3]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080b23cc
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	ldr	r0, [pc, #268]
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b23cc:
	ldr	r3, [pc, #264]
	ldr	r3, [r3, #16]
	cmp	r9, r3
	bls.n	.L_080b23dc
	ldr	r0, [pc, #260]
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b23dc:
	mov	r0, sl
	movs	r1, #2
	bl	sub_08015120
	mov	r0, r9
	movs	r1, #5
	bl	sub_08015120
	ldr	r2, [pc, #240]
	mov	fp, r2
	mov	r0, fp
	bl	sub_080b0574
	movs	r0, #0
	bl	sub_080b0634
	cmp	r0, #0
	beq.n	.L_080b240a
	mov	r0, fp
	adds	r0, #1
	bl	sub_080b0574
	b.n	.L_080b24b0
.L_080b240a:
	ldr	r5, [sp, #4]
	mov	r2, r8
	adds	r5, #216
	mov	r3, r8
	ldrh	r6, [r3, r5]
	strh	r0, [r2, r5]
	ldr	r3, [sp, #8]
	adds	r1, r7, #0
	ldr	r0, [r3, #32]
	bl	sub_080b1dec
	movs	r1, #2
	mov	r0, sl
	bl	sub_08015120
	mov	r0, fp
	adds	r0, #2
	bl	sub_080b0574
	bl	sub_08015140
	movs	r0, #10
	bl	sub_080030f8
	movs	r0, #100
	bl	sub_080f9010
	movs	r0, #110
	bl	sub_080030f8
	movs	r0, #100
	bl	sub_080f9010
	movs	r0, #110
	bl	sub_080030f8
	movs	r0, #100
	bl	sub_080f9010
	movs	r0, #110
	bl	sub_080030f8
	movs	r0, #112
	bl	sub_080f9010
	movs	r0, #20
	bl	sub_080030f8
	mov	r2, r8
	strh	r6, [r2, r5]
	ldr	r1, [sp, #12]
	adds	r0, r7, #0
	bl	sub_08077068
	mov	r3, r9
	negs	r0, r3
	bl	sub_08077230
	bl	sub_080b10cc
	ldr	r2, [sp, #8]
	adds	r1, r7, #0
	ldr	r0, [r2, #32]
	bl	sub_080b1dec
	movs	r1, #2
	mov	r0, sl
	bl	sub_08015120
	mov	r0, fp
	adds	r0, #3
	bl	sub_080b0574
	adds	r0, r7, #0
	ldr	r1, [sp, #12]
	bl	sub_080b1868
	cmp	r0, #0
	beq.n	.L_080b24b0
	adds	r0, r7, #0
	ldr	r1, [sp, #0]
	bl	sub_080b196c
.L_080b24b0:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00000cba
	.4byte 0x00000cbb
	.4byte 0x00000cbc
	.4byte 0x02000240
	.4byte 0x00000cbd
	.2byte 0x0cbe
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r1, [sp, #8]
	ldr	r3, [pc, #416]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #416]
	adds	r3, r3, r6
	ldrh	r4, [r3, #0]
	mov	r9, r0
	movs	r0, #1
	ldr	r1, [pc, #408]
	mov	fp, r0
	movs	r0, #142
	str	r4, [sp, #0]
	lsls	r0, r0, #1
	mov	r8, r3
	adds	r3, r1, r0
	movs	r2, #0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	sl, r2
	ldr	r2, [pc, #388]
	lsls	r3, r3, #1
	ldrsh	r4, [r2, r3]
	movs	r2, #140
	str	r4, [sp, #4]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r7, #0
	cmp	r4, r3
	ble.n	.L_080b2534
	b.n	.L_080b2684
.L_080b2534:
	movs	r3, #228
	mov	r4, r8
	strh	r3, [r4, #0]
	movs	r1, #2
	movs	r0, #228
	bl	sub_08015120
	ldr	r5, [pc, #356]
	adds	r0, r5, #0
	bl	sub_080b0574
	mov	r2, r8
	ldrh	r0, [r2, #0]
	movs	r1, #2
	adds	r5, #1
	bl	sub_08015120
	adds	r0, r5, #0
	bl	sub_080b0574
.L_080b255c:
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080b25b4
	ldr	r0, [pc, #328]
	adds	r3, r6, r0
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	movs	r4, #0
	adds	r0, r7, r1
	mov	fp, r4
	bl	sub_080022fc
	movs	r3, #219
	adds	r7, r0, #0
	lsls	r1, r7, #1
	lsls	r3, r3, #2
	adds	r2, r1, r3
	adds	r3, r6, #2
	adds	r1, r1, r7
	ldrsh	r4, [r3, r2]
	lsls	r1, r1, #3
	subs	r1, #12
	mov	r0, r9
	movs	r2, #0
	mov	sl, r4
	bl	sub_080b0a6c
	movs	r3, #234
	lsls	r3, r3, #2
	adds	r2, r6, r3
	ldr	r4, [pc, #256]
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r5, r6, r4
	ldrh	r2, [r5, #0]
	mov	r0, r9
	adds	r1, r7, #0
	bl	sub_080b11c4
	ldrh	r2, [r5, #0]
	ldr	r0, [sp, #8]
	mov	r1, sl
	bl	sub_080b1470
.L_080b25b4:
	ldr	r1, [pc, #248]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080b262c
	ldr	r0, [pc, #216]
	adds	r5, r6, r0
	ldrh	r1, [r5, #0]
	mov	r0, sl
	bl	sub_08077028
	adds	r1, r0, #0
	cmp	r1, #0
	bge.n	.L_080b2602
	movs	r0, #113
	bl	sub_080f9010
	mov	r0, sl
	movs	r1, #1
	bl	sub_08015120
	ldrh	r0, [r5, #0]
	movs	r1, #2
	bl	sub_08015120
	mov	r0, sl
	bl	sub_08077248
	cmp	r0, #15
	bne.n	.L_080b25fa
	ldr	r0, [pc, #192]
	bl	sub_080b04dc
	b.n	.L_080b255c
.L_080b25fa:
	ldr	r0, [pc, #188]
	bl	sub_080b04dc
	b.n	.L_080b255c
.L_080b2602:
	mov	r0, sl
	bl	sub_08077058
	movs	r0, #101
	bl	sub_080f9010
	ldr	r0, [pc, #172]
	bl	sub_080b0574
	ldrh	r1, [r5, #0]
	mov	r0, sl
	bl	sub_08077028
	ldr	r2, [sp, #4]
	negs	r0, r2
	bl	sub_08077298
	movs	r0, #1
	bl	sub_080772a0
	b.n	.L_080b267a
.L_080b262c:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b2644
	ldr	r0, [pc, #136]
	bl	sub_080b0574
	movs	r0, #113
	bl	sub_080f9010
	b.n	.L_080b267a
.L_080b2644:
	ldr	r5, [pc, #124]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b265c
	movs	r0, #111
	bl	sub_080f9010
	movs	r3, #1
	subs	r7, #1
	mov	fp, r3
.L_080b265c:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b2672
	movs	r0, #111
	bl	sub_080f9010
	movs	r4, #1
	adds	r7, #1
	mov	fp, r4
.L_080b2672:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080b255c
.L_080b267a:
	ldr	r0, [pc, #32]
	mov	r2, sp
	ldrh	r2, [r2, #0]
	adds	r3, r6, r0
	strh	r2, [r3, #0]
.L_080b2684:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x0000039e
	.4byte 0x02000240
	.4byte 0x080b4146
	.4byte 0x00000cc3
	.4byte 0x000003a7
	.4byte 0x03001c94
	.4byte 0x00000c9e
	.4byte 0x00000ca6
	.4byte 0x00000ca1
	.4byte 0x00000cc5
	.4byte 0x03001b04
