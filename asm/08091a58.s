@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091a58
	.thumb_func
Func_08091a58:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #412]
	ldr	r3, [r3, #0]
	movs	r7, #236
	mov	fp, r3
	lsls	r7, r7, #1
	add	r7, fp
	movs	r3, #0
	ldrsh	r2, [r7, r3]
	sub	sp, #12
	str	r2, [sp, #0]
	adds	r6, r0, #0
	bl	Func_08077030
	movs	r2, #1
	mov	r8, r0
	negs	r2, r2
	cmp	r8, r2
	beq.n	.L0
	b.n	.L1
.L0:
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08015120
	ldr	r0, [pc, #368]
	movs	r1, #1
	bl	Func_08015040
	ldr	r0, [pc, #364]
	movs	r1, #1
	bl	Func_08015040
	movs	r3, #8
	movs	r2, #4
	add	r3, sp
	add	r2, sp
	mov	r9, r3
	mov	sl, r2
.L4:
	ldr	r7, [pc, #348]
	movs	r1, #1
	adds	r0, r7, #0
	bl	Func_08015040
	mov	r0, r9
	mov	r1, sl
	bl	Func_080b0058
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L2
	adds	r0, r6, #0
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08015120
	adds	r0, r7, #4
	movs	r1, #1
	bl	Func_08015040
	b.n	.L4
.L3:
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08015120
	adds	r0, r7, #1
	movs	r1, #5
	bl	Func_08015040
	movs	r0, #1
	bl	Func_08091d84
	adds	r5, r0, #0
	bl	Func_08015140
	cmp	r5, #0
	bne.n	.L4
	movs	r1, #1
	adds	r0, r6, #0
	bl	Func_08077240
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08015120
	adds	r0, r7, #2
	movs	r1, #1
	bl	Func_08015040
	movs	r3, #236
	mov	r2, sp
	lsls	r3, r3, #1
	ldrh	r2, [r2, #0]
	add	r3, fp
	strh	r2, [r3, #0]
	b.n	.L5
.L2:
	ldr	r0, [sp, #8]
	bl	Func_08077008
	ldr	r1, [sp, #4]
	ldr	r0, [sp, #8]
	bl	Func_08077020
	cmp	r0, #0
	ble.n	.L6
	adds	r5, r0, #0
.L7:
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #4]
	subs	r5, #1
	bl	Func_080772b0
	cmp	r5, #0
	bne.n	.L7
.L6:
	adds	r0, r6, #0
	bl	Func_08077030
	mov	r8, r0
	movs	r0, #83
	bl	Func_080f9010
	ldr	r3, [pc, #172]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bne.n	.L8
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08015120
	ldr	r0, [pc, #140]
	movs	r1, #3
	bl	Func_08015040
	b.n	.L9
.L8:
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08015120
	mov	r0, r8
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #128]
	movs	r1, #3
	bl	Func_08015040
.L9:
	movs	r3, #236
	mov	r2, sp
	lsls	r3, r3, #1
	ldrh	r2, [r2, #0]
	add	r3, fp
	strh	r2, [r3, #0]
	b.n	.L5
.L1:
	movs	r0, #83
	bl	Func_080f9010
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08015120
	ldr	r5, [pc, #76]
	movs	r1, #3
	adds	r0, r5, #0
	bl	Func_08015040
	ldr	r3, [pc, #80]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r8, r3
	beq.n	.L10
	adds	r0, r6, #0
	movs	r1, #2
	bl	Func_08015120
	mov	r0, r8
	movs	r1, #1
	bl	Func_08015120
	adds	r0, r5, #1
	movs	r1, #3
	bl	Func_08015040
.L10:
	mov	r3, sp
	ldrh	r3, [r3, #0]
	strh	r3, [r7, #0]
.L5:
	mov	r0, r8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x0000096a
	.4byte 0x00000977
	.4byte 0x00000978
	.4byte 0x02000240
	.4byte 0x0000096b
