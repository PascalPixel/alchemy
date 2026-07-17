@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d3c80
	.thumb_func
Func_080d3c80:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #88]
	ldmia	r3!, {r1}
	ldr	r5, [pc, #88]
	mov	r9, r1
	ldr	r3, [r3, #0]
	sub	sp, #40
	add	r5, r9
	str	r3, [sp, #28]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #72]
	strh	r3, [r2, #0]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #32
	ldr	r0, [r3, #4]
	adds	r1, r2, #0
	str	r2, [sp, #20]
	bl	Func_080cef64
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #36]
	movs	r3, #50
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #28]
	lsls	r1, r1, #3
	b.n	.L0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000cf
	.4byte 0x00007784
	.4byte 0x080cd261
.L0:
	bl	Func_080041d8
	ldr	r4, [pc, #536]
	str	r4, [sp, #12]
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	lsls	r3, r3, #1
	ldrb	r3, [r4, r3]
	cmp	r3, #0
	beq.n	.L1
	movs	r5, #31
	mov	fp, r5
	ldr	r7, [pc, #516]
	movs	r5, #225
	lsls	r5, r5, #7
	movs	r0, #0
	add	r5, r9
	mov	r8, r0
	add	r7, r9
.L4:
	bl	Func_08004458
	ldr	r3, [pc, #504]
	str	r3, [r5, #4]
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L2
	bl	Func_08004458
	mov	r1, fp
	ands	r0, r1
	adds	r0, #80
	lsls	r6, r0, #16
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	b.n	.L3
.L2:
	bl	Func_08004458
	mov	r2, fp
	ands	r0, r2
	adds	r0, #8
	lsls	r6, r0, #16
	bl	Func_08004458
	movs	r3, #63
	ands	r3, r0
	negs	r3, r3
.L3:
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #3
	adds	r3, r3, r2
	lsls	r3, r3, #1
	subs	r3, r6, r3
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #16]
	str	r3, [r5, #8]
	mov	r3, r8
	str	r3, [r5, #24]
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #408]
	lsls	r3, r3, #1
	movs	r0, #1
	ldrb	r3, [r1, r3]
	movs	r4, #8
	add	sl, r0
	adds	r5, #28
	add	r8, r4
	cmp	sl, r3
	bne.n	.L4
.L1:
	movs	r2, #0
	str	r2, [sp, #24]
	ldr	r2, [pc, #388]
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #24]
	ldr	r5, [sp, #12]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L5
	b.n	.L6
.L5:
	ldr	r0, [pc, #368]
	add	r0, r9
	str	r0, [sp, #16]
.L24:
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	bne.n	.L7
	ldr	r2, [sp, #24]
	cmp	r2, #103
	bgt.n	.L7
	ldr	r3, [pc, #352]
	ldr	r1, [r3, #0]
	ldr	r3, [sp, #24]
	movs	r2, #192
	cmp	r3, #95
	ble.n	.L8
	ldr	r4, [sp, #24]
	lsls	r3, r4, #1
	adds	r3, r3, r4
	movs	r2, #156
	lsls	r3, r3, #3
	lsls	r2, r2, #4
	subs	r2, r2, r3
.L8:
	ldr	r5, [sp, #16]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L9
	ldrh	r3, [r1, #54]
	ldr	r0, [pc, #304]
	subs	r3, r3, r2
	strh	r3, [r1, #54]
	str	r0, [sp, #12]
	b.n	.L7
.L9:
	ldrh	r3, [r1, #54]
	adds	r3, r3, r2
	strh	r3, [r1, #54]
	ldr	r1, [pc, #288]
	str	r1, [sp, #12]
.L7:
	ldr	r5, [pc, #288]
	add	r5, r9
	ldr	r2, [r5, #0]
	ldr	r3, [r2, #24]
	ldr	r4, [sp, #12]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r4, r3]
	ldr	r0, [sp, #24]
	subs	r3, #80
	cmp	r0, r3
	bne.n	.L10
	movs	r0, #134
	bl	Func_080b50e8
	ldr	r2, [r5, #0]
.L10:
	ldr	r3, [r2, #24]
	ldr	r1, [sp, #12]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r1, r3]
	ldr	r4, [sp, #24]
	subs	r3, #8
	cmp	r4, r3
	bne.n	.L11
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #240]
	ldr	r3, [pc, #240]
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [r5, #0]
.L11:
	ldr	r3, [r2, #24]
	ldr	r5, [sp, #12]
	lsls	r2, r3, #1
	adds	r3, r2, #1
	ldrb	r3, [r5, r3]
	ldr	r0, [sp, #24]
	subs	r3, #8
	cmp	r0, r3
	ble.n	.L12
	b.n	.L13
.L12:
	ldrb	r3, [r5, r2]
	movs	r1, #0
	mov	fp, r1
	cmp	r3, #0
	bne.n	.L14
	b.n	.L13
.L14:
	movs	r2, #225
	lsls	r2, r2, #7
	add	r2, r9
	mov	r8, r2
.L23:
	mov	r4, r8
	ldr	r3, [r4, #8]
	cmp	r3, #1
	bne.n	.L15
	mov	r0, fp
	lsls	r2, r0, #4
	lsls	r3, r0, #7
	subs	r3, r3, r2
	ldr	r1, [pc, #180]
	movs	r5, #0
	lsls	r3, r3, #2
	mov	sl, r5
	adds	r7, r3, r1
.L18:
	movs	r1, #5
	mov	r0, sl
	bl	Func_080022fc
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #96
	ldr	r0, [r7, #24]
	bl	Func_080022ec
	movs	r1, #3
	bl	Func_080022fc
	movs	r2, #4
	mov	r3, sl
	adds	r6, r7, #0
	adds	r4, r5, r0
	mov	ip, r2
	cmp	r3, #2
	ble.n	.L16
	movs	r5, #0
	mov	ip, r5
.L16:
	ldr	r2, [pc, #132]
	lsls	r3, r4, #2
	ldr	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	ldr	r3, [pc, #124]
	ldrb	r5, [r3, r4]
	movs	r0, #128
	lsls	r0, r0, #4
	lsrs	r3, r5, #1
	add	r1, r9
	adds	r1, r1, r0
	subs	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	ldr	r0, [pc, #108]
	ldrb	r4, [r0, r4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r5, [sp, #0]
	ldr	r0, [sp, #20]
	str	r4, [sp, #4]
	mov	r5, ip
	ldr	r4, [r5, r0]
	ldr	r0, [sp, #28]
	bl	Func_080072f4
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r0, r6, #0
	movs	r1, #64
	bl	Func_080e3908
	ldr	r3, [r6, #24]
	ldr	r2, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	cmp	r2, #1
	ble.n	.L17
	ldr	r1, [sp, #24]
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L17
	subs	r3, r2, #1
	str	r3, [r6, #8]
.L17:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L18
	b.n	.L19
	.4byte 0x080ee1f5
	.4byte 0x00007828
	.4byte 0xffc00000
	.4byte 0x03001e80
	.4byte 0x00007784
	.4byte 0x06060606
	.4byte 0x02010000
	.4byte 0x080ee214
	.4byte 0x080ee1fb
	.4byte 0x080ee207
.L15:
	mov	r4, r8
	ldr	r3, [r4, #24]
	ldr	r5, [sp, #24]
	cmp	r5, r3
	blt.n	.L19
	movs	r1, #6
	ldrsh	r3, [r4, r1]
	movs	r1, #2
	ldrsh	r2, [r4, r1]
	movs	r1, #32
	str	r1, [sp, #0]
	movs	r5, #1
	movs	r1, #64
	mov	r6, fp
	str	r1, [sp, #4]
	ands	r6, r5
	ldr	r1, [sp, #20]
	lsls	r0, r6, #2
	ldr	r4, [r0, r1]
	subs	r2, #16
	ldr	r0, [sp, #28]
	mov	r1, r9
	bl	Func_080072f4
	movs	r2, #128
	lsls	r2, r2, #9
	mov	r0, r8
	movs	r1, #64
	bl	Func_080e3908
	mov	r2, r8
	ldr	r3, [r2, #4]
	movs	r2, #224
	lsls	r2, r2, #14
	cmp	r3, r2
	ble.n	.L19
	mov	r3, r8
	str	r5, [r3, #8]
	mov	r5, fp
	str	r2, [r3, #4]
	lsls	r2, r5, #4
	lsls	r3, r5, #7
	subs	r3, r3, r2
	ldr	r0, [pc, #304]
	movs	r4, #0
	lsls	r3, r3, #2
	ldr	r7, [pc, #300]
	mov	sl, r4
	movs	r1, #127
	adds	r5, r3, r0
.L21:
	ldrb	r3, [r7, #0]
	mov	r4, r8
	ldr	r2, [r4, #0]
	subs	r3, #40
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldrb	r3, [r7, #1]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	str	r1, [sp, #8]
	bl	Func_08004458
	ldr	r1, [sp, #8]
	ands	r0, r1
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	Func_08004458
	ldr	r1, [sp, #8]
	ands	r0, r1
	negs	r0, r0
	lsls	r3, r0, #11
	str	r3, [r5, #16]
	cmp	r6, #0
	beq.n	.L20
	ldr	r3, [r5, #12]
	lsls	r3, r3, #1
	str	r3, [r5, #12]
	lsls	r3, r0, #12
	str	r3, [r5, #16]
.L20:
	movs	r0, #1
	movs	r3, #32
	add	sl, r0
	str	r3, [r5, #8]
	mov	r2, sl
	movs	r3, #0
	str	r3, [r5, #24]
	adds	r7, #2
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L21
	ldr	r2, [pc, #212]
	movs	r3, #8
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #144
	bl	Func_080f9010
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [pc, #196]
	mov	r4, r9
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L19
	ldr	r5, [pc, #184]
	movs	r6, #36
	add	r5, r9
.L22:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, sl
	movs	r1, #7
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	sl, r2
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L22
.L19:
	ldr	r5, [sp, #16]
	movs	r3, #28
	add	r8, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	ldr	r0, [pc, #140]
	lsls	r3, r3, #1
	movs	r4, #1
	ldrb	r3, [r0, r3]
	add	fp, r4
	cmp	fp, r3
	beq.n	.L13
	b.n	.L23
.L13:
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r1, [r3, #24]
	lsls	r0, r1, #1
	lsls	r1, r1, #2
	adds	r0, #4
	adds	r1, #8
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #108]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [sp, #24]
	ldr	r3, [pc, #88]
	adds	r2, #1
	str	r2, [sp, #24]
	str	r3, [sp, #12]
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #0]
	ldr	r3, [r3, #24]
	ldr	r5, [pc, #72]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r5, r3]
	ldr	r0, [sp, #24]
	ldr	r2, [pc, #60]
	cmp	r0, r3
	beq.n	.L6
	b.n	.L24
.L6:
	ldr	r0, [pc, #64]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x080ee1d3
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x080ee1f5
	.4byte 0x00007824
	.4byte 0x080cd261
