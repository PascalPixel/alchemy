@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080e90a8
	.thumb_func
Func_080e90a8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #428]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #88
	str	r3, [sp, #36]
	ldr	r3, [r2, #8]
	str	r3, [sp, #32]
	subs	r2, #108
	ldr	r5, [pc, #412]
	mov	fp, r1
	ldr	r2, [r2, #0]
	add	r5, fp
	str	r2, [sp, #28]
	ldr	r7, [pc, #408]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r0, [pc, #400]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	adds	r1, r7, #0
	ldr	r0, [pc, #392]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r2, #0
	ldr	r1, [sp, #32]
	ldr	r0, [pc, #380]
	movs	r3, #0
	bl	Func_080e0524
	ldr	r3, [r5, #0]
	movs	r2, #36
	ldrsh	r1, [r3, r2]
	ldr	r0, [r3, #8]
	movs	r2, #4
	movs	r3, #0
	bl	Func_080b5078
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r5, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	movs	r5, #225
	movs	r2, #0
	lsls	r5, r5, #7
	ldr	r6, [r0, #0]
	mov	r8, r2
	movs	r7, #255
	add	r5, fp
.L2:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	Func_08004458
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	Func_08004458
	ands	r0, r7
	subs	r0, #127
	ldr	r3, [r5, #0]
	lsls	r0, r0, #12
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L0
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L0:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L1
	adds	r3, #3
.L1:
	asrs	r3, r3, #2
	lsls	r3, r3, #1
	adds	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L2
	ldr	r3, [pc, #224]
	add	r3, fp
	ldr	r3, [r3, #0]
	mov	r2, sp
	adds	r2, #64
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r2, #0
	str	r2, [sp, #24]
	bl	Func_080e396c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #220]
	bl	Func_080041d8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #208]
	movs	r3, #75
	add	r2, fp
	str	r3, [r2, #0]
	movs	r3, #0
	mov	r9, r3
	ldr	r1, [sp, #28]
	ldr	r2, [pc, #168]
	ldr	r3, [pc, #196]
	mov	r0, sp
	adds	r0, #76
	adds	r1, #12
	add	r2, fp
	add	r3, fp
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #20]
	str	r3, [sp, #8]
.L21:
	mov	r0, r9
	cmp	r0, #8
	bne.n	.L3
	movs	r0, #134
	bl	Func_080b50e8
.L3:
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	beq.n	.L4
	mov	r2, r9
	cmp	r2, #8
	bne.n	.L4
	movs	r0, #212
	bl	Func_080f9010
.L4:
	ldr	r5, [pc, #112]
	add	r5, fp
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #16]
	ldr	r0, [r3, #8]
	bl	Func_080e396c
	mov	r3, r9
	subs	r3, #6
	cmp	r3, #5
	bhi.n	.L5
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L6
	movs	r3, #3
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
	b.n	.L7
.L6:
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Func_080ed408
.L7:
	ldr	r3, [pc, #84]
	ldr	r0, [sp, #20]
	adds	r3, #184
	ldr	r4, [r3, #0]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	str	r4, [sp, #40]
	cmp	r3, #0
	bne.n	.L8
	ldr	r2, [sp, #76]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	movs	r1, #48
	ldr	r3, [sp, #80]
	asrs	r2, r2, #1
	str	r1, [sp, #0]
	movs	r1, #72
	str	r1, [sp, #4]
	subs	r2, #24
	subs	r3, #24
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #8]
	bl	Func_080072f4
	b.n	.L9
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x00000096
	.4byte 0x00000063
	.4byte 0x00000073
	.4byte 0x080cd261
	.4byte 0x00007784
	.4byte 0xffffaf00
	.4byte 0x03001e50
.L8:
	ldr	r2, [sp, #76]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	movs	r1, #48
	ldr	r3, [sp, #80]
	str	r1, [sp, #0]
	movs	r1, #72
	str	r1, [sp, #4]
	asrs	r2, r2, #1
	subs	r3, #24
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #8]
	bl	Func_080072f4
.L9:
	movs	r0, #46
	bl	Func_08002dd8
.L5:
	mov	r2, r9
	subs	r2, #16
	cmp	r2, #31
	bhi.n	.L10
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r5, r3, #1
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	bl	Func_080ed408
	ldr	r3, [pc, #444]
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	cmp	r5, #2
	ble.n	.L11
	movs	r5, #2
.L11:
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L12
	movs	r2, #150
	lsls	r2, r2, #6
.L12:
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #3
	adds	r1, r1, r5
	lsls	r1, r1, #7
	ldr	r3, [sp, #24]
	adds	r1, r2, r1
	ldr	r2, [pc, #404]
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r0, [sp, #24]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r0, #40
	str	r0, [sp, #0]
	asrs	r2, r2, #1
	movs	r0, #80
	str	r0, [sp, #4]
	subs	r2, #20
	subs	r3, #48
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	bl	Func_080072f4
	ldr	r0, [pc, #372]
	bl	Func_080d655c
	movs	r0, #46
	bl	Func_08002dd8
.L10:
	mov	r1, r9
	cmp	r1, #8
	bne.n	.L13
	ldr	r3, [pc, #360]
	add	r0, sp, #48
	str	r3, [r0, #0]
	ldr	r1, [sp, #36]
	ldr	r3, [pc, #356]
	ldr	r2, [pc, #356]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L13:
	bl	Func_080049ac
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #12]
	bl	Func_080051d8
	mov	r2, r9
	cmp	r2, #3
	ble.n	.L14
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	add	r1, sp, #40
	ldr	r0, [r3, #4]
	mov	sl, r1
	bl	Func_080cef64
	movs	r7, #225
	movs	r2, #0
	lsls	r7, r7, #7
	mov	r8, r2
	add	r6, sp, #52
	add	r7, fp
.L16:
	ldr	r5, [r7, #24]
	cmp	r5, #0
	ble.n	.L15
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	Func_080e3944
	ldr	r2, [r6, #0]
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldr	r0, [sp, #24]
	ldr	r3, [r6, #4]
	ldr	r1, [r0, #4]
	adds	r3, r3, r1
	mov	r1, r8
	lsrs	r4, r1, #31
	asrs	r5, r5, #3
	add	r4, r8
	adds	r5, #2
	movs	r1, #1
	asrs	r4, r4, #1
	subs	r3, #112
	ands	r4, r1
	lsls	r1, r5, #1
	str	r3, [r6, #4]
	ldr	r0, [pc, #260]
	str	r1, [sp, #4]
	subs	r1, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #32]
	adds	r1, r0, r1
	lsrs	r0, r5, #31
	adds	r0, r5, r0
	str	r5, [sp, #0]
	asrs	r0, r0, #1
	subs	r2, r2, r0
	lsls	r4, r4, #2
	mov	r0, sl
	subs	r3, r3, r5
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #36]
	bl	Func_080072f4
	adds	r0, r7, #0
	movs	r1, #60
	ldr	r2, [pc, #224]
	bl	Func_080e38b8
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L15:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L16
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
.L14:
	mov	r3, r9
	cmp	r3, #8
	bne.n	.L17
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	Func_080b5088
	ldr	r2, [pc, #172]
	movs	r3, #4
	add	r2, fp
	str	r3, [r2, #0]
.L17:
	mov	r2, r9
	cmp	r2, #6
	bne.n	.L18
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	movs	r2, #1
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #10
	negs	r2, r2
	subs	r3, #1
	bl	Func_080d6888
.L18:
	mov	r2, r9
	cmp	r2, #14
	bne.n	.L19
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	movs	r2, #1
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #10
	negs	r2, r2
	subs	r3, #1
	bl	Func_080d6888
.L19:
	movs	r1, #16
	movs	r0, #16
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #96]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #216
	ldr	r2, [sp, #8]
	movs	r0, #1
	lsls	r3, r3, #4
	add	r9, r0
	adds	r2, r2, r3
	mov	r1, r9
	str	r2, [sp, #8]
	cmp	r1, #64
	beq.n	.L20
	b.n	.L21
.L20:
	ldr	r0, [pc, #64]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e50
	.4byte 0x02010000
	.4byte 0x00002710
	.4byte 0x3f3f3f3f
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x080ede48
	.4byte 0xfffffc00
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
