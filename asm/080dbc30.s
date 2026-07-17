@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_080dbc30
	.thumb_func
Func_080dbc30:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #48]
	mov	fp, r1
	ldr	r1, [r6, #0]
	sub	sp, #168
	str	r1, [sp, #36]
	subs	r3, r6, #4
	ldr	r3, [r3, #0]
	ldr	r5, [pc, #36]
	mov	r9, r3
	add	r5, r9
	str	r0, [r5, #0]
	movs	r0, #0
	ldr	r7, [r6, #4]
	bl	Func_080cd594
	ldr	r2, [pc, #24]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	mov	r2, fp
	cmp	r2, #7
	bne.n	.L0
	b.n	.L1
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000052
.L1:
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #24]
	movs	r0, #47
	str	r3, [sp, #40]
	movs	r1, #7
	movs	r3, #7
	movs	r2, #7
	str	r5, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r6, #28]
	mov	r0, sp
	adds	r0, #40
	str	r0, [sp, #20]
	str	r3, [r0, #4]
	b.n	.L2
.L0:
	ldr	r3, [r5, #0]
	mov	r1, sp
	ldr	r0, [r3, #4]
	adds	r1, #40
	str	r1, [sp, #20]
	bl	Func_080cef64
.L2:
	movs	r2, #1
	ldr	r0, [pc, #164]
	mov	r1, r9
	movs	r3, #0
	bl	Func_080e0524
	mov	r2, fp
	cmp	r2, #5
	bne.n	.L3
	ldr	r1, [pc, #152]
	ldr	r0, [pc, #152]
	b.n	.L4
.L3:
	mov	r3, fp
	cmp	r3, #7
	bne.n	.L5
	ldr	r1, [pc, #140]
	ldr	r0, [pc, #144]
.L4:
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	b.n	.L6
.L5:
	ldr	r1, [pc, #124]
	ldr	r0, [pc, #132]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	ldr	r0, [pc, #124]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	mov	r0, fp
	cmp	r0, #6
	bne.n	.L7
	movs	r0, #160
	movs	r6, #0
	lsls	r0, r0, #19
.L9:
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L8
	adds	r3, r6, #3
.L8:
	asrs	r3, r3, #2
	lsls	r2, r3, #10
	lsls	r1, r3, #5
	orrs	r2, r1
	orrs	r2, r3
	adds	r6, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r6, #64
	bne.n	.L9
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	b.n	.L6
.L7:
	mov	r1, fp
	cmp	r1, #4
	bhi.n	.L10
	ldr	r2, [pc, #68]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	pop	{r2, r4, r6, pc}
	lsrs	r5, r1, #32
	pop	{r3, r4, r6, pc}
	lsrs	r5, r1, #32
	pop	{r2, r7, pc}
	lsrs	r5, r1, #32
	pop	{r3, r7, pc}
	lsrs	r5, r1, #32
	pop	{r2, r3, r7, pc}
	lsrs	r5, r1, #32
	ldr	r0, [pc, #24]
	b.n	.L11
	ldr	r0, [pc, #36]
	b.n	.L11
	.4byte 0x00000000
	.4byte 0x000000ce
	.4byte 0x00000c56
	.4byte 0x0000005a
	.4byte 0x00000054
	.4byte 0x0000007d
	.4byte 0x00000073
	.4byte 0x04000050
	.4byte 0x080dbd40
	.4byte 0x000000b9
	ldr	r0, [pc, #172]
	b.n	.L11
	ldr	r0, [pc, #172]
	b.n	.L11
.L10:
	ldr	r0, [pc, #172]
.L11:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #168]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
.L6:
	mov	r2, fp
	cmp	r2, #7
	bne.n	.L12
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #144]
	movs	r3, #50
	b.n	.L13
.L12:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #128]
	movs	r3, #75
.L13:
	add	r2, r9
	str	r3, [r2, #0]
	movs	r1, #144
	ldr	r0, [pc, #124]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r3, [pc, #120]
	movs	r2, #1
	movs	r6, #0
	negs	r2, r2
	add	r3, r9
.L14:
	adds	r6, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r6, #64
	bne.n	.L14
	ldr	r5, [pc, #104]
	add	r5, r9
	ldr	r3, [r5, #0]
	add	r7, sp, #156
	ldr	r0, [r3, #8]
	adds	r1, r7, #0
	bl	Func_080e3958
	mov	r3, fp
	cmp	r3, #3
	bne.n	.L15
	ldr	r3, [r7, #4]
	subs	r3, #16
	str	r3, [r7, #4]
.L15:
	mov	r0, fp
	cmp	r0, #4
	bne.n	.L16
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L17
	ldr	r3, [r7, #0]
	adds	r3, #28
	b.n	.L18
.L17:
	ldr	r3, [r7, #0]
	subs	r3, #28
.L18:
	str	r3, [r7, #0]
.L16:
	mov	r1, fp
	cmp	r1, #7
	bne.n	.L19
	ldr	r3, [pc, #44]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L20
	ldr	r3, [r7, #0]
	adds	r3, #16
	b.n	.L21
	.4byte 0x0000006e
	.4byte 0x000000a1
	.4byte 0x0000008d
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007098
	.4byte 0x00007828
.L20:
	ldr	r3, [r7, #0]
	subs	r3, #16
.L21:
	str	r3, [r7, #0]
.L19:
	mov	r2, fp
	cmp	r2, #5
	bne.n	.L22
	ldr	r0, [r7, #0]
	movs	r1, #3
	bl	Func_080022ec
	ldr	r2, [pc, #28]
	ldr	r3, [pc, #24]
	str	r0, [r7, #0]
	strh	r3, [r2, #0]
.L22:
	ldr	r2, [pc, #24]
	add	r2, r9
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L23
	add	r5, sp, #60
	movs	r4, #36
	b.n	.L24
	.4byte 0x00000055
	.4byte 0x04000020
	.4byte 0x00007828
.L24:
	ldr	r3, [r2, #0]
	ldrsh	r0, [r3, r4]
	adds	r1, r5, #0
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	Func_080e396c
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r6, #1
	adds	r5, #12
	adds	r4, #2
	cmp	r6, r3
	bne.n	.L24
.L23:
	ldr	r3, [pc, #804]
	movs	r0, #225
	lsls	r0, r0, #7
	movs	r2, #0
	add	r3, r9
	add	r0, r9
	str	r2, [sp, #32]
	str	r3, [sp, #28]
	str	r7, [sp, #24]
	str	r0, [sp, #16]
.L49:
	ldr	r1, [sp, #28]
	ldr	r3, [r1, #0]
	ldr	r0, [sp, #32]
	ldr	r1, [r3, #20]
	bl	Func_080022fc
	ldr	r2, [sp, #32]
	mov	r8, r0
	cmp	r2, #4
	bne.n	.L25
	movs	r0, #136
	bl	Func_080f9010
.L25:
	mov	r3, fp
	cmp	r3, #6
	beq.n	.L26
	ldr	r0, [sp, #32]
	cmp	r0, #24
	bne.n	.L27
	movs	r0, #134
	bl	Func_080b50e8
	b.n	.L27
.L26:
	ldr	r1, [sp, #32]
	cmp	r1, #60
	bne.n	.L27
	movs	r0, #134
	bl	Func_080b50e8
.L27:
	mov	r2, fp
	cmp	r2, #5
	bne.n	.L28
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L29
	ldr	r0, [sp, #32]
	movs	r1, #3
	bl	Func_080022ec
	movs	r1, #3
	bl	Func_080022fc
	lsls	r1, r0, #3
	adds	r1, r1, r0
	ldr	r2, [pc, #696]
	ldr	r3, [sp, #24]
	lsls	r1, r1, #9
	add	r1, r9
	movs	r0, #72
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	str	r0, [sp, #0]
	movs	r0, #62
	str	r0, [sp, #4]
	subs	r2, #2
	b.n	.L30
.L29:
	ldr	r0, [sp, #32]
	movs	r1, #3
	bl	Func_080022ec
	movs	r1, #3
	bl	Func_080022fc
	lsls	r1, r0, #3
	adds	r1, r1, r0
	ldr	r3, [sp, #24]
	ldr	r0, [pc, #652]
	lsls	r1, r1, #9
	add	r1, r9
	ldr	r2, [r3, #0]
	adds	r1, r1, r0
	ldr	r3, [r3, #4]
	movs	r0, #72
	str	r0, [sp, #0]
	movs	r0, #62
	str	r0, [sp, #4]
	subs	r2, #70
.L30:
	subs	r3, #32
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	bl	Func_080072f4
	b.n	.L31
.L28:
	bl	Func_08004458
	mov	r1, r8
	lsls	r5, r1, #1
	add	r5, r8
	add	r6, sp, #60
	lsls	r5, r5, #2
	ldr	r3, [r6, r5]
	movs	r2, #31
	ands	r2, r0
	adds	r3, r3, r2
	add	r7, sp, #48
	subs	r3, #16
	str	r3, [r7, #0]
	bl	Func_08004458
	adds	r5, #4
	ldr	r3, [r6, r5]
	movs	r2, #63
	ands	r2, r0
	adds	r3, r3, r2
	subs	r3, #16
	str	r3, [r7, #4]
	ldr	r2, [sp, #32]
	cmp	r2, #47
	bgt.n	.L31
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #0]
	ldr	r1, [sp, #16]
	lsls	r3, r3, #15
	str	r3, [r1, #0]
	ldr	r3, [r0, #4]
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	ldr	r3, [r7, #0]
	ldr	r2, [r0, #0]
	subs	r3, r3, r2
	lsls	r3, r3, #11
	str	r3, [r1, #12]
	ldr	r3, [r7, #4]
	ldr	r2, [r0, #4]
	subs	r3, r3, r2
	lsls	r3, r3, #11
	str	r3, [r1, #16]
	movs	r3, #0
	str	r3, [r1, #24]
.L31:
	movs	r5, #225
	movs	r2, #32
	lsls	r5, r5, #7
	movs	r6, #0
	mov	r8, r2
	movs	r7, #64
	add	r5, r9
.L38:
	ldr	r1, [r5, #24]
	cmp	r1, #0
	blt.n	.L32
	mov	r3, fp
	cmp	r3, #7
	bne.n	.L33
	cmp	r1, #5
	ble.n	.L34
	ldr	r0, [sp, #28]
	ldr	r3, [r0, #0]
	ldr	r1, [r3, #4]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	mov	r0, r8
	str	r0, [sp, #0]
	str	r7, [sp, #4]
	ldr	r0, [sp, #20]
	lsls	r1, r1, #2
	ldr	r4, [r1, r0]
	ldr	r1, [pc, #468]
	subs	r2, #16
	subs	r3, #32
	ldr	r0, [sp, #36]
	add	r1, r9
	bl	Func_080072f4
	b.n	.L34
.L33:
	mov	r2, fp
	cmp	r2, #4
	bne.n	.L35
	cmp	r1, #5
	ble.n	.L34
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L36
	adds	r3, r1, #3
.L36:
	asrs	r1, r3, #2
	lsls	r1, r1, #11
	ldr	r3, [pc, #432]
	add	r1, r9
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	adds	r1, r1, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	mov	r0, r8
	str	r0, [sp, #0]
	subs	r2, #16
	subs	r3, #32
	str	r7, [sp, #4]
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	bl	Func_080072f4
	b.n	.L34
.L35:
	mov	r2, fp
	cmp	r2, #5
	beq.n	.L34
	cmp	r1, #1
	ble.n	.L34
	cmp	r1, #0
	bge.n	.L37
	adds	r1, #3
.L37:
	asrs	r1, r1, #2
	ldr	r3, [pc, #380]
	lsls	r1, r1, #11
	add	r1, r9
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	adds	r1, r1, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	mov	r0, r8
	str	r0, [sp, #0]
	subs	r2, #16
	subs	r3, #32
	str	r7, [sp, #4]
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	bl	Func_080072f4
.L34:
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #24
	bne.n	.L32
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L32:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #64
	bne.n	.L38
	mov	r1, fp
	cmp	r1, #5
	bne.n	.L39
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L40
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #288]
	movs	r3, #7
	ldr	r5, [pc, #276]
	ands	r0, r3
	add	r1, r9
	mov	r8, r0
	add	r5, r9
	movs	r4, #36
	movs	r7, #2
	mov	sl, r1
.L42:
	ldr	r2, [sp, #32]
	cmp	r2, r7
	blt.n	.L41
	cmp	r8, r6
	bne.n	.L41
	movs	r3, #8
	mov	r0, sl
	str	r3, [r0, #0]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r4]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r6, #0
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r4, [sp, #8]
.L41:
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r6, #1
	adds	r4, #2
	adds	r7, #4
	cmp	r6, r3
	bne.n	.L42
	b.n	.L40
.L39:
	ldr	r1, [pc, #208]
	mov	r2, r9
	ldr	r3, [r2, r1]
	ldr	r3, [r3, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L40
	ldr	r3, [sp, #32]
	movs	r7, #7
	movs	r5, #36
	ands	r7, r3
.L46:
	lsls	r3, r6, #2
	ldr	r0, [sp, #32]
	adds	r3, #16
	cmp	r0, r3
	blt.n	.L43
	cmp	r7, r6
	bne.n	.L43
	ldr	r2, [pc, #180]
	movs	r3, #8
	add	r2, r9
	str	r3, [r2, #0]
	mov	r2, fp
	cmp	r2, #6
	bne.n	.L44
	mov	r0, r9
	ldr	r3, [r0, r1]
	ldrsh	r0, [r3, r5]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #14
	movs	r2, #5
	adds	r3, r6, #0
	bl	Func_080d6888
	b.n	.L45
.L44:
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldrsh	r0, [r3, r5]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r6, #0
	bl	Func_080d6888
.L45:
	ldr	r2, [sp, #28]
	ldr	r3, [r2, #0]
	ldrsh	r0, [r3, r5]
	movs	r1, #4
	bl	Func_080b5088
.L43:
	ldr	r1, [pc, #104]
	mov	r2, r9
	ldr	r3, [r2, r1]
	ldr	r3, [r3, #20]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r3
	bne.n	.L46
.L40:
	movs	r0, #4
	movs	r1, #4
	bl	Func_080e155c
	mov	r3, fp
	cmp	r3, #6
	beq.n	.L47
	bl	Func_080cd52c
.L47:
	ldr	r2, [pc, #80]
	movs	r3, #1
	add	r2, r9
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #32]
	adds	r0, #28
	adds	r1, #1
	str	r0, [sp, #16]
	str	r1, [sp, #32]
	cmp	r1, #64
	beq.n	.L48
	b.n	.L49
.L48:
	ldr	r0, [pc, #52]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #168
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007828
	.4byte 0x00000c56
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261
