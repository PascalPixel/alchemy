@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ed104
	.thumb_func
Func_080ed104:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #96]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r6, [pc, #92]
	ldr	r3, [r3, #0]
	sub	sp, #44
	mov	sl, r1
	str	r3, [sp, #20]
	add	r6, sl
	str	r0, [r6, #0]
	movs	r0, #0
	ldr	r5, [r2, #8]
	bl	Func_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #60]
	adds	r1, r5, #0
	strh	r3, [r2, #0]
	ldr	r0, [pc, #72]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #64]
	mov	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r1, #140
	lsls	r1, r1, #3
	ldr	r0, [pc, #52]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r3, [r6, #0]
	mov	r2, sp
	adds	r2, #24
	ldr	r0, [r3, #4]
	adds	r1, r2, #0
	str	r2, [sp, #16]
	bl	Func_080cef64
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x00000073
	.4byte 0x00000051
	.4byte 0x000000c0
.L0:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #576]
	movs	r3, #75
	add	r2, sl
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #568]
	bl	Func_080041d8
	ldr	r3, [r6, #0]
	add	r5, sp, #32
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	adds	r1, r5, #0
	bl	Func_080e3980
	ldr	r2, [r5, #0]
	movs	r3, #64
	ldr	r1, [pc, #548]
	subs	r3, r3, r2
	lsls	r3, r3, #8
	movs	r7, #225
	str	r3, [r1, #0]
	lsls	r7, r7, #7
	movs	r1, #0
	mov	r8, r1
	add	r7, sl
.L1:
	bl	Func_08004458
	ldr	r6, [pc, #528]
	ands	r6, r0
	bl	Func_08004458
	ldr	r3, [pc, #524]
	adds	r5, r0, #0
	ands	r5, r3
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	movs	r3, #224
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #7
	movs	r2, #1
	ands	r3, r0
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L1
	ldr	r2, [pc, #460]
	ldr	r1, [pc, #460]
	movs	r4, #0
	add	r2, sl
	movs	r3, #8
	add	r1, sl
	str	r3, [r2, #0]
	str	r1, [sp, #12]
	str	r4, [sp, #8]
	mov	r9, r4
.L17:
	mov	r2, r9
	cmp	r2, #10
	bne.n	.L2
	ldr	r3, [pc, #432]
	movs	r4, #8
	add	r3, sl
	str	r4, [r3, #0]
	movs	r0, #212
	bl	Func_080b50e8
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #0]
	movs	r1, #0
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	Func_080b5088
	ldr	r4, [sp, #12]
	ldr	r3, [r4, #0]
	movs	r2, #8
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	Func_080d6888
.L2:
	mov	r3, r9
	cmp	r3, #7
	ble.n	.L3
	cmp	r3, #31
	bgt.n	.L4
	lsls	r3, r3, #1
	add	r3, r9
	lsls	r3, r3, #2
	adds	r5, r3, #0
	subs	r5, #96
	b.n	.L5
.L4:
	movs	r3, #136
	ldr	r4, [sp, #8]
	lsls	r3, r3, #1
	subs	r5, r3, r4
.L5:
	cmp	r5, #0
	ble.n	.L3
	cmp	r5, #80
	ble.n	.L6
	movs	r5, #80
	movs	r7, #2
	b.n	.L7
.L6:
	movs	r7, #3
.L7:
	movs	r1, #0
	movs	r2, #112
	mov	r8, r1
	movs	r6, #50
	mov	fp, r2
.L10:
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L8
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	str	r7, [sp, #0]
	bl	Func_080ed408
	b.n	.L9
.L8:
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	str	r7, [sp, #0]
	bl	Func_080ed408
.L9:
	ldr	r4, [pc, #300]
	ldr	r4, [r4, #0]
	movs	r3, #14
	mov	ip, r4
	str	r4, [sp, #24]
	mov	r4, fp
	adds	r2, r6, #0
	str	r3, [sp, #0]
	mov	r1, sl
	str	r5, [sp, #4]
	ldr	r0, [sp, #20]
	subs	r3, r4, r5
	bl	Func_08007314
	movs	r0, #46
	bl	Func_08002dd8
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #14
	cmp	r2, #2
	bne.n	.L10
.L3:
	ldr	r4, [sp, #12]
	ldr	r3, [r4, #0]
	ldr	r1, [sp, #16]
	ldr	r0, [r3, #4]
	movs	r5, #225
	bl	Func_080cef64
	lsls	r5, r5, #7
	movs	r1, #0
	mov	r8, r1
	add	r5, sl
.L13:
	mov	r2, r8
	lsrs	r3, r2, #31
	add	r3, r8
	asrs	r3, r3, #1
	adds	r3, #8
	cmp	r9, r3
	blt.n	.L11
	ldr	r0, [r5, #24]
	cmp	r0, #28
	bgt.n	.L11
	movs	r1, #3
	bl	Func_080022ec
	movs	r3, #2
	ldrsh	r4, [r5, r3]
	movs	r1, #6
	ldrsh	r6, [r5, r1]
	cmp	r0, #6
	ble.n	.L12
	movs	r0, #6
.L12:
	ldr	r3, [pc, #196]
	lsls	r2, r0, #1
	ldrh	r1, [r3, r2]
	movs	r3, #140
	lsls	r3, r3, #3
	add	r1, sl
	adds	r1, r1, r3
	ldr	r3, [pc, #184]
	ldrh	r0, [r3, r2]
	lsrs	r3, r0, #1
	subs	r2, r4, r3
	str	r0, [sp, #0]
	subs	r3, r6, r3
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	bl	Func_080072f4
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	adds	r0, r5, #0
	movs	r1, #62
	ldr	r2, [pc, #156]
	bl	Func_080e3908
.L11:
	movs	r4, #1
	add	r8, r4
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L13
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	mov	r2, r9
	cmp	r2, #7
	bgt.n	.L14
	movs	r0, #2
	movs	r1, #2
	bl	Func_080e155c
	b.n	.L15
.L14:
	movs	r0, #16
	movs	r1, #16
	bl	Func_080e155c
.L15:
	bl	Func_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r4, #1
	ldr	r3, [sp, #8]
	add	r9, r4
	adds	r3, #6
	mov	r1, r9
	str	r3, [sp, #8]
	cmp	r1, #54
	beq.n	.L16
	b.n	.L17
.L16:
	ldr	r0, [pc, #32]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x04000028
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x03001f08
	.4byte 0x080eef88
	.4byte 0x080eef96
	.4byte 0xffffe000
	.4byte 0x00007824
