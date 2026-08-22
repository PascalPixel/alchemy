@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08018cac
	.thumb_func
Func_08018cac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #144
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	adds	r7, r1, #0
	ldr	r1, [pc, #352]
	ldr	r2, [pc, #352]
	ldr	r6, [r1, #0]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #348]
	str	r3, [sp, #4]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [sp, #176]
	mov	r8, r0
	cmp	r3, #1
	beq.n	.L0
	ldrh	r2, [r0, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [r1, #88]
	ldr	r3, [r3, #0]
	cmp	r3, r8
	bne.n	.L1
	ldr	r0, [pc, #316]
	bl	Func_08002f40
	ldr	r0, [pc, #316]
	bl	Func_08002f40
	movs	r1, #3
	mov	r9, r1
	cmp	r7, #32
	bne.n	.L1
	b.n	.L2
.L1:
	ldr	r0, [pc, #300]
	bl	Func_08002f40
	movs	r2, #4
	mov	sl, r0
	mov	r9, r2
	cmp	r7, #32
	bne.n	.L3
	b.n	.L2
.L3:
	ldr	r5, [pc, #284]
	adds	r0, r5, #0
	bl	Func_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #272]
	ldr	r0, [pc, #276]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	str	r3, [sp, #0]
	adds	r1, r7, #0
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r0, r8
	bl	Func_080072fc
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Func_08002df0
	adds	r0, r5, #0
	b.n	.L4
.L0:
	movs	r1, #5
	mov	r9, r1
	cmp	r7, #32
	bne.n	.L5
	b.n	.L2
.L5:
	bl	Func_08015e8c
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L6
	b.n	.L4
.L6:
	ldr	r2, [pc, #216]
	subs	r3, r5, r6
	adds	r3, r3, r2
	ldr	r2, [pc, #216]
	adds	r1, r3, #0
	muls	r1, r2
	movs	r3, #1
	movs	r2, #0
	strb	r3, [r5, #5]
	strb	r2, [r5, #4]
	ldr	r3, [sp, #176]
	mov	sl, r1
	cmp	r3, #1
	bne.n	.L7
	movs	r1, #1
	movs	r3, #2
	mov	r9, r1
	strb	r3, [r5, #5]
	b.n	.L8
.L7:
	ldr	r1, [pc, #188]
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	beq.n	.L9
	cmp	r3, #3
	bgt.n	.L10
	cmp	r3, #2
	beq.n	.L11
	b.n	.L12
.L10:
	cmp	r3, #4
	beq.n	.L13
	cmp	r3, #5
	beq.n	.L14
	b.n	.L12
.L9:
	movs	r3, #5
	strb	r3, [r5, #5]
	b.n	.L12
.L13:
	movs	r3, #6
	strb	r3, [r5, #5]
	movs	r3, #8
	strh	r3, [r5, #12]
	b.n	.L12
.L14:
	movs	r3, #7
	b.n	.L15
.L11:
	movs	r3, #4
.L15:
	strb	r3, [r5, #5]
	strh	r2, [r5, #12]
.L12:
	add	r1, sp, #16
	adds	r0, r7, #0
	bl	Func_080178b0
	cmp	r0, #0
	bne.n	.L16
	movs	r0, #1
.L16:
	mov	r9, r0
.L8:
	ldrb	r3, [r5, #5]
	cmp	r3, #2
	bne.n	.L17
	ldr	r2, [pc, #112]
	adds	r6, r6, r2
	ldrh	r3, [r6, #0]
	adds	r7, r5, #0
	adds	r7, #16
	cmp	r3, #99
	bne.n	.L18
	bl	Func_08004080
	strh	r0, [r6, #0]
.L18:
	mov	r3, r8
	ldrh	r2, [r3, #12]
	ldr	r1, [pc, #92]
	ldrh	r3, [r3, #8]
	adds	r3, r3, r1
	adds	r2, r2, r3
	lsls	r2, r2, #3
	ldr	r3, [pc, #32]
	adds	r2, #4
	ands	r2, r3
	ldrh	r1, [r7, #6]
	ldr	r3, [pc, #80]
	ands	r3, r1
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r7, #6]
	ldrb	r3, [r2, #14]
	ldrb	r2, [r2, #10]
	adds	r2, #254
	adds	r3, r3, r2
	lsls	r3, r3, #3
	subs	r3, #1
	strb	r3, [r7, #4]
	b.n	.L19
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0x000012b0
	.4byte 0x00000ea8
	.4byte 0x00000014
	.4byte 0x00000013
	.4byte 0x00000318
	.4byte 0x040000d4
	.4byte 0x080155d0
	.4byte 0xfffff968
	.4byte 0xb6db6db7
	.4byte 0x00000eac
	.4byte 0x000012b6
	.4byte 0x0000fffe
	.4byte 0xfffffe00
.L17:
	ldr	r3, [pc, #116]
	adds	r4, r6, r3
	ldrh	r1, [r4, #0]
	ldr	r2, [pc, #116]
	add	r1, sl
	lsls	r1, r1, #5
	adds	r1, r1, r2
	ldr	r3, [pc, #112]
	add	r0, sp, #16
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [sp, #8]
	mov	r1, fp
	lsrs	r3, r1, #1
	mov	r1, r8
	adds	r3, r2, r3
	ldrh	r2, [r1, #14]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	ldr	r2, [pc, #92]
	adds	r3, r3, r2
	strh	r3, [r5, #20]
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #12]
	lsrs	r2, r3, #1
	adds	r2, r1, r2
	mov	r1, r8
	ldrh	r3, [r1, #12]
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #52]
	adds	r2, #2
	orrs	r2, r3
	strh	r2, [r5, #22]
	ldrh	r3, [r4, #0]
	add	r3, sl
	adds	r7, r5, #0
	strh	r3, [r5, #24]
	adds	r7, #16
.L19:
	movs	r3, #254
	strb	r3, [r5, #15]
	ldrh	r3, [r7, #6]
	lsls	r3, r3, #23
	lsrs	r3, r3, #23
	strh	r3, [r5, #6]
	ldrb	r3, [r7, #4]
	movs	r2, #0
	strh	r3, [r5, #8]
	mov	r3, sl
	strb	r3, [r5, #14]
	str	r2, [r5, #0]
	mov	r0, r8
	adds	r1, r5, #0
	bl	Func_08016584
.L2:
	mov	r0, r9
	b.n	.L4
	.4byte 0x00004000
	.4byte 0x000012b8
	.4byte 0x06010000
	.4byte 0x040000d4
	.4byte 0x84000020
	.4byte 0x0000fffe
.L4:
	add	sp, #144
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
