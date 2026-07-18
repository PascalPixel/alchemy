@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a414c
	.thumb_func
Func_080a414c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #0
	sub	sp, #16
	mov	sl, r1
	movs	r1, #8
	ldr	r3, [pc, #772]
	add	r1, sp
	mov	fp, r1
	movs	r2, #0
	ldr	r6, [r3, #0]
	mov	r0, fp
	movs	r3, #1
	mov	r8, r2
	mov	r9, r3
	bl	Func_080a448c
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r2, r6, r2
	str	r2, [sp, #4]
	ldrh	r3, [r2, #0]
	movs	r7, #0
	cmp	r3, #1
	beq.n	.L0
	bl	Func_080a345c
	ldr	r0, [r6, #52]
	bl	Func_08015270
	movs	r1, #134
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldr	r5, [r3, #0]
	bl	Func_080a4eb8
	adds	r0, r5, #0
	bl	Func_08015270
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r2, #3
	movs	r3, #16
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08015070
	bl	Func_080a51d0
	adds	r1, r5, #0
	mov	r0, fp
	bl	Func_080a45cc
	ldr	r0, [r6, #44]
	bl	Func_08015270
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #668]
	ands	r0, r3
	ldr	r3, [pc, #668]
	ldr	r1, [r6, #44]
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
.L0:
	ldr	r1, [sp, #4]
	mov	r3, sl
	ldr	r2, [pc, #652]
	strh	r3, [r1, #0]
	adds	r3, r6, r2
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L1
	mov	r1, fp
	movs	r3, #2
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L2
	movs	r2, #0
	movs	r7, #2
	mov	r8, r2
.L2:
	mov	r1, fp
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L3
	movs	r2, #1
	movs	r7, #0
	mov	r8, r2
.L3:
	mov	r1, fp
	movs	r3, #1
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L4
	movs	r2, #0
	movs	r7, #1
	mov	r8, r2
.L4:
	mov	r1, fp
	movs	r3, #4
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L5
	movs	r2, #1
	movs	r7, #1
	mov	r8, r2
.L5:
	mov	r1, fp
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L6
	movs	r2, #0
	movs	r7, #0
	mov	r8, r2
	b.n	.L6
.L18:
	movs	r0, #113
	bl	Func_080f9010
	movs	r3, #1
	negs	r3, r3
	ldr	r1, [pc, #544]
	mov	sl, r3
	b.n	.L7
.L1:
	movs	r1, #3
	adds	r0, r5, #0
	bl	Func_080022fc
	lsls	r0, r0, #24
	asrs	r7, r0, #24
	movs	r1, #3
	adds	r0, r5, #0
	bl	Func_080022ec
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	mov	r8, r0
	lsls	r3, r0, #1
	add	r3, r8
	adds	r3, r3, r7
	mov	sl, r3
.L6:
	mov	r1, r8
	adds	r0, r7, #0
	bl	Func_080a4110
	mov	r1, r8
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_080a413c
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080a1ac0
	b.n	.L8
.L22:
	mov	r3, r9
	cmp	r3, #0
	beq.n	.L9
	movs	r1, #0
	adds	r0, r7, #3
	mov	r9, r1
	movs	r1, #3
	bl	Func_080022fc
	mov	r2, r8
	adds	r2, #2
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	subs	r2, r2, r3
	mov	r8, r2
	lsls	r3, r2, #1
	adds	r7, r0, #0
	add	r3, r8
	adds	r3, r3, r7
	mov	sl, r3
	bl	Func_080a3c98
	mov	r2, sl
	cmp	r2, #2
	ble.n	.L10
	movs	r3, #151
	lsls	r3, r3, #2
	adds	r2, r6, r3
	ldr	r1, [pc, #420]
	movs	r3, #1
	strb	r3, [r2, #0]
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	subs	r1, #166
	adds	r2, r6, r1
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	Func_080a3ef0
	mov	r2, sl
	cmp	r2, #3
	bne.n	.L9
	movs	r1, #200
	ldr	r0, [pc, #392]
	lsls	r1, r1, #4
	bl	Func_080041d8
	b.n	.L9
.L10:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L11
	movs	r1, #151
	lsls	r1, r1, #2
	adds	r3, r6, r1
	ldr	r2, [pc, #364]
	strb	r5, [r3, #0]
	adds	r3, r6, r2
	subs	r1, #232
	ldrb	r3, [r3, #0]
	adds	r2, r6, r1
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	Func_080a3ef0
	b.n	.L9
.L11:
	ldr	r2, [pc, #340]
	adds	r3, r6, r2
	ldrb	r1, [r3, #0]
	ldr	r0, [r6, #36]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080a112c
.L9:
	mov	r1, r8
	adds	r0, r7, #0
	bl	Func_080a4110
	mov	r1, r8
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_080a413c
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r5, [pc, #300]
	ldr	r2, [r5, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L12
	mov	r1, fp
	mov	r2, sl
	ldrsb	r3, [r1, r2]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L13
	movs	r0, #114
	bl	Func_080f9010
	b.n	.L12
.L13:
	mov	r2, sl
	cmp	r2, #5
	bhi.n	.L14
	lsls	r3, r2, #2
	ldr	r2, [pc, #264]
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080a4398
	.4byte 0x080a43a0
	.4byte 0x080a43a8
	.4byte 0x080a43a8
	.4byte 0x080a43b0
	.4byte 0x080a43a8
	movs	r0, #174
	bl	Func_080f9010
	b.n	.L15
	movs	r0, #175
	bl	Func_080f9010
	b.n	.L15
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L15
	movs	r0, #117
	bl	Func_080f9010
	b.n	.L15
.L14:
	movs	r0, #112
	bl	Func_080f9010
.L15:
	ldr	r1, [pc, #180]
.L7:
	mov	r2, sl
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	b.n	.L16
.L12:
	ldr	r2, [r5, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L17
	b.n	.L18
.L17:
	ldr	r1, [pc, #176]
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L19
	subs	r3, #65
	movs	r1, #1
	movs	r0, #111
	add	r8, r3
	mov	r9, r1
	bl	Func_080f9010
	b.n	.L8
.L19:
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L20
	movs	r2, #1
	movs	r0, #111
	add	r8, r2
	mov	r9, r2
	bl	Func_080f9010
	b.n	.L8
.L20:
	ldr	r2, [r1, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L21
	movs	r3, #1
	movs	r0, #111
	adds	r7, #1
	mov	r9, r3
	bl	Func_080f9010
	b.n	.L8
.L21:
	ldr	r3, [r1, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	movs	r1, #1
	movs	r0, #111
	subs	r7, #1
	mov	r9, r1
	bl	Func_080f9010
.L8:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L16
	b.n	.L22
.L16:
	movs	r3, #151
	lsls	r3, r3, #2
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	bl	Func_080a3c98
	mov	r0, sl
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00000075
	.4byte 0x0000025d
	.4byte 0x0000021a
	.4byte 0x080a3c09
	.4byte 0x03001c94
	.4byte 0x080a4380
	.4byte 0x03001b04
