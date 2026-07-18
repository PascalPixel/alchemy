@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e6d3c
	.thumb_func
Func_080e6d3c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #308]
	mov	sl, r2
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #304]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	sub	sp, #32
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	ldr	r3, [pc, #296]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r9, r1
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	add	r1, sp, #16
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	cmp	r0, #1
	beq.n	.L0
	cmp	r0, #1
	bgt.n	.L1
	cmp	r0, #0
	beq.n	.L2
	b.n	.L3
.L1:
	cmp	r0, #2
	beq.n	.L4
	cmp	r0, #3
	beq.n	.L5
	b.n	.L3
.L2:
	add	r3, sp, #8
	mov	r8, r3
	ldr	r3, [pc, #248]
	movs	r5, #0
	adds	r7, r1, #0
	adds	r6, r2, r3
.L6:
	adds	r0, r5, #0
	movs	r1, #3
	bl	Func_080022fc
	lsls	r0, r0, #21
	add	r0, r9
	str	r0, [r7, #0]
	movs	r1, #3
	adds	r0, r5, #0
	bl	Func_080022ec
	lsls	r0, r0, #21
	add	r0, sl
	str	r0, [r7, #8]
	adds	r1, r7, #0
	ldmia	r6!, {r0}
	mov	r2, r8
	movs	r3, #0
	adds	r5, #1
	bl	Func_08009008
	cmp	r5, #9
	bne.n	.L6
	b.n	.L3
.L0:
	add	r3, sp, #8
	mov	r8, r3
	ldr	r3, [pc, #188]
	movs	r5, #0
	adds	r6, r1, #0
	adds	r7, r2, r3
.L7:
	ldr	r3, [pc, #184]
	ldrb	r3, [r3, r5]
	ldr	r2, [pc, #184]
	lsls	r3, r3, #16
	add	r3, r9
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #180]
	ldrb	r3, [r3, r5]
	ldr	r2, [pc, #180]
	lsls	r3, r3, #16
	add	r3, sl
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldmia	r7!, {r0}
	adds	r1, r6, #0
	mov	r2, r8
	movs	r3, #0
	adds	r5, #1
	bl	Func_08009008
	cmp	r5, #12
	bne.n	.L7
	b.n	.L3
.L4:
	add	r3, sp, #8
	mov	r8, r3
	ldr	r3, [pc, #128]
	movs	r5, #0
	adds	r6, r1, #0
	adds	r7, r2, r3
.L8:
	ldr	r3, [pc, #140]
	ldrb	r3, [r3, r5]
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #13
	add	r3, r9
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #128]
	ldrb	r3, [r3, r5]
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #8]
	ldmia	r7!, {r0}
	adds	r1, r6, #0
	mov	r2, r8
	movs	r3, #0
	adds	r5, #1
	bl	Func_08009008
	cmp	r5, #8
	bne.n	.L8
	b.n	.L3
.L5:
	ldr	r3, [pc, #76]
	movs	r5, #0
	mov	r8, sp
	adds	r6, r1, #0
	adds	r7, r2, r3
.L9:
	ldr	r3, [pc, #92]
	ldrb	r3, [r3, r5]
	lsls	r3, r3, #16
	add	r3, r9
	str	r3, [r6, #0]
	ldr	r3, [pc, #88]
	ldrb	r3, [r3, r5]
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #8]
	ldmia	r7!, {r0}
	adds	r1, r6, #0
	mov	r2, r8
	movs	r3, #0
	adds	r5, #1
	bl	Func_08009008
	cmp	r5, #8
	bne.n	.L9
.L3:
	add	sp, #32
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x080edab8
	.4byte 0x080edac0
	.4byte 0x000077d8
	.4byte 0x080eee1e
	.4byte 0xfff00000
	.4byte 0x080eee2a
	.4byte 0xffe00000
	.4byte 0x080eee36
	.4byte 0x080eee3e
	.4byte 0x080eee46
	.4byte 0x080eee4e
