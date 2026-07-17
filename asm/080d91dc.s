@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080d91dc
	.thumb_func
Func_080d91dc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	str	r1, [sp, #72]
	ldr	r3, [pc, #88]
	adds	r6, r0, #0
	ldmia	r3!, {r0}
	str	r0, [sp, #68]
	ldr	r1, [pc, #84]
	ldr	r3, [r3, #0]
	adds	r5, r0, r1
	str	r3, [sp, #64]
	movs	r0, #0
	str	r6, [r5, #0]
	bl	Func_080cd594
	ldr	r5, [r5, #0]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L0
	ldr	r2, [sp, #72]
	cmp	r2, #3
	bne.n	.L1
	ldr	r2, [r5, #4]
	eors	r2, r3
	add	r3, sp, #80
	str	r3, [sp, #0]
	add	r3, sp, #76
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #0
	b.n	.L2
.L1:
	ldr	r4, [sp, #72]
	cmp	r4, #2
	beq.n	.L3
	cmp	r4, #4
	bne.n	.L4
.L3:
	ldr	r2, [r5, #4]
	eors	r2, r3
	add	r3, sp, #80
	str	r3, [sp, #0]
	add	r3, sp, #76
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #3
.L2:
	movs	r3, #1
	bl	Func_080de2f8
	b.n	.L5
	.4byte 0x03001eec
	.4byte 0x00007828
.L4:
	ldr	r2, [r5, #4]
	eors	r2, r3
	add	r3, sp, #80
	str	r3, [sp, #0]
	add	r3, sp, #76
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r1, #2
	movs	r3, #1
	bl	Func_080de2f8
.L5:
	ldr	r0, [sp, #80]
	movs	r1, #5
	lsls	r0, r0, #2
	bl	Func_080022ec
	str	r0, [sp, #80]
.L0:
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #68]
	ldr	r0, [pc, #56]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r2, [pc, #48]
	ldr	r5, [sp, #68]
	movs	r3, #1
	adds	r1, r5, r2
	ldr	r0, [pc, #44]
	movs	r2, #1
	bl	Func_080e0524
	ldr	r3, [sp, #72]
	cmp	r3, #3
	beq.n	.L6
	cmp	r3, #5
	bne.n	.L7
.L6:
	ldr	r4, [sp, #68]
	ldr	r5, [pc, #28]
	ldr	r0, [pc, #32]
	adds	r1, r4, r5
	movs	r2, #1
	b.n	.L8
	movs	r0, r0
	.4byte 0x000000cc
	.4byte 0x04000020
	.4byte 0x00000076
	.4byte 0x0000060e
	.4byte 0x000000b7
	.4byte 0x00002b8e
	.4byte 0x000000b0
.L8:
	movs	r3, #1
	bl	Func_080e0524
	ldr	r0, [sp, #72]
	cmp	r0, #3
	bne.n	.L9
	ldr	r0, [pc, #888]
	b.n	.L10
.L9:
	ldr	r0, [pc, #888]
.L10:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #884]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
	b.n	.L11
.L7:
	ldr	r1, [sp, #72]
	cmp	r1, #4
	bne.n	.L12
	ldr	r2, [sp, #68]
	ldr	r3, [pc, #868]
	ldr	r0, [pc, #868]
	adds	r1, r2, r3
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	b.n	.L11
.L12:
	ldr	r4, [sp, #72]
	cmp	r4, #0
	bne.n	.L13
	ldr	r2, [pc, #844]
	ldr	r5, [sp, #68]
	ldr	r0, [pc, #848]
	adds	r1, r5, r2
	movs	r3, #0
	movs	r2, #1
	bl	Func_080e0524
	b.n	.L14
.L13:
	ldr	r3, [sp, #68]
	ldr	r4, [pc, #824]
	ldr	r0, [pc, #836]
	adds	r1, r3, r4
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
.L14:
	ldr	r5, [sp, #72]
	cmp	r5, #0
	bne.n	.L15
	ldr	r0, [pc, #796]
	b.n	.L16
.L15:
	ldr	r0, [sp, #72]
	cmp	r0, #2
	beq.n	.L17
	cmp	r0, #4
	bne.n	.L18
.L17:
	ldr	r0, [pc, #808]
	b.n	.L16
.L18:
	ldr	r1, [sp, #72]
	cmp	r1, #1
	bne.n	.L19
	ldr	r0, [pc, #772]
	b.n	.L16
.L19:
	ldr	r0, [pc, #796]
.L16:
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #764]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f0
.L11:
	ldr	r2, [sp, #72]
	cmp	r2, #3
	bne.n	.L20
	ldr	r3, [sp, #68]
	ldr	r4, [pc, #772]
	ldr	r0, [pc, #736]
	adds	r1, r3, r4
	b.n	.L21
.L20:
	ldr	r5, [sp, #72]
	cmp	r5, #2
	beq.n	.L22
	cmp	r5, #4
	bne.n	.L23
.L22:
	ldr	r2, [sp, #68]
	ldr	r3, [pc, #752]
	ldr	r0, [pc, #744]
	adds	r1, r2, r3
.L21:
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	b.n	.L24
.L23:
	ldr	r4, [sp, #68]
	ldr	r5, [pc, #736]
	ldr	r0, [pc, #700]
	adds	r1, r4, r5
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
.L24:
	movs	r0, #0
	ldr	r5, [pc, #724]
	mov	sl, r0
.L25:
	bl	Func_08004458
	movs	r1, #200
	bl	Func_08002304
	subs	r0, #100
	lsls	r0, r0, #14
	str	r0, [r5, #0]
	bl	Func_08004458
	movs	r1, #200
	bl	Func_08002304
	subs	r0, #100
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	Func_08004458
	movs	r1, #200
	bl	Func_08002304
	movs	r1, #1
	subs	r0, #100
	movs	r2, #128
	lsls	r0, r0, #14
	movs	r3, #0
	add	sl, r1
	lsls	r2, r2, #2
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L25
	ldr	r4, [sp, #68]
	ldr	r5, [pc, #656]
	adds	r3, r4, r5
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #20]
	cmp	r3, #1
	bne.n	.L26
	add	r5, sp, #120
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r5, #0
	bl	Func_080e396c
	ldr	r0, [r5, #0]
	negs	r0, r0
	lsls	r0, r0, #2
	movs	r1, #5
	bl	Func_080022ec
	adds	r0, #64
	str	r0, [sp, #48]
	b.n	.L27
.L26:
	ldr	r3, [r2, #4]
	movs	r2, #64
	negs	r2, r2
	str	r2, [sp, #48]
	cmp	r3, #1
	beq.n	.L27
	movs	r3, #0
	str	r3, [sp, #48]
.L27:
	ldr	r4, [sp, #48]
	ldr	r2, [pc, #600]
	lsls	r3, r4, #8
	str	r3, [r2, #0]
	ldr	r5, [sp, #68]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #592]
	adds	r2, r5, r0
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r5, r1
	movs	r3, #50
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #580]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r2, [pc, #560]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r4, #7
	str	r4, [sp, #44]
	cmp	r3, #1
	beq.n	.L28
	movs	r5, #3
	str	r5, [sp, #44]
.L28:
	movs	r0, #142
	bl	Func_080f9010
	ldr	r2, [pc, #532]
	ldr	r1, [sp, #68]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r4, #108
	movs	r0, #0
	lsls	r3, r3, #3
	negs	r4, r4
	mov	fp, r0
	cmp	r3, r4
	bne.n	.L29
	b.n	.L30
.L29:
	ldr	r5, [pc, #524]
	str	r5, [sp, #36]
.L67:
	ldr	r0, [sp, #36]
	ldr	r0, [r0, #0]
	mov	r1, fp
	str	r0, [sp, #40]
	cmp	r1, #80
	bne.n	.L31
	movs	r0, #0
	bl	Func_080b50e8
.L31:
	ldr	r2, [sp, #68]
	ldr	r4, [pc, #484]
	adds	r3, r2, r4
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #28]
	cmp	r3, #1
	bne.n	.L32
	mov	r0, fp
	lsls	r5, r0, #11
	adds	r0, r5, #0
	bl	Func_08002322
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r2, [sp, #80]
	lsls	r3, r3, #2
	ldr	r1, [sp, #48]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	adds	r3, r3, r1
	subs	r3, #20
	adds	r0, r5, #0
	mov	sl, r3
	bl	Func_0800231c
	ldr	r3, [sp, #76]
	ldr	r2, [sp, #44]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	movs	r5, #4
	eors	r5, r2
	adds	r0, r0, r3
	movs	r3, #2
	str	r3, [sp, #0]
	adds	r7, r0, #0
	adds	r3, r5, #0
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	bl	Func_080ed408
	ldr	r3, [sp, #36]
	adds	r3, #136
	ldr	r3, [r3, #0]
	str	r3, [sp, #56]
	mov	r8, r3
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #47
	adds	r3, r5, #0
	movs	r1, #7
	movs	r2, #7
	bl	Func_080ed408
	ldr	r3, [pc, #396]
	ldr	r4, [r3, #0]
	mov	r5, fp
	subs	r7, #24
	str	r4, [sp, #60]
	cmp	r5, #32
	ble.n	.L33
	lsls	r3, r5, #1
	subs	r3, r7, r3
	adds	r7, r3, #0
	adds	r7, #64
.L33:
	ldr	r0, [sp, #68]
	ldr	r1, [pc, #344]
	movs	r5, #40
	adds	r6, r0, r1
	mov	r2, sl
	str	r4, [sp, #8]
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #64]
	adds	r1, r6, #0
	adds	r3, r7, #0
	bl	Func_08007304
	mov	r2, fp
	ldr	r4, [sp, #8]
	cmp	r2, #3
	bgt.n	.L34
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #64]
	adds	r1, r6, #0
	mov	r2, sl
	adds	r3, r7, #0
	bl	Func_080072f4
.L34:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
.L32:
	movs	r3, #0
	str	r3, [sp, #52]
	ldr	r2, [pc, #288]
	ldr	r4, [sp, #68]
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L35
	b.n	.L36
.L35:
	ldr	r5, [sp, #40]
	mov	r0, sp
	adds	r5, #12
	mov	r1, sp
	mov	r3, sp
	str	r5, [sp, #32]
	adds	r0, #84
	adds	r1, #96
	adds	r3, #108
	movs	r4, #0
	movs	r5, #36
	str	r0, [sp, #28]
	str	r1, [sp, #24]
	str	r3, [sp, #20]
	str	r4, [sp, #16]
	str	r5, [sp, #12]
	mov	r9, fp
.L66:
	ldr	r0, [sp, #68]
	adds	r6, r0, r2
	ldr	r1, [sp, #12]
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r3, [r6, #0]
	ldr	r4, [sp, #12]
	ldr	r5, [r0, #0]
	ldrsh	r0, [r3, r4]
	bl	Func_080b5070
	movs	r1, #3
	lsls	r0, r0, #1
	bl	Func_080022ec
	ldr	r3, [sp, #16]
	adds	r3, #80
	adds	r7, r0, #0
	cmp	fp, r3
	bne.n	.L37
	movs	r0, #212
	bl	Func_080f9010
.L37:
	bl	Func_080049ac
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #32]
	bl	Func_080051d8
	ldr	r3, [r5, #8]
	ldr	r2, [sp, #28]
	str	r3, [r2, #0]
	str	r7, [r2, #4]
	ldr	r3, [r5, #16]
	str	r3, [r2, #8]
	ldr	r0, [sp, #28]
	bl	Func_08004cb4
	ldr	r3, [sp, #16]
	adds	r3, #48
	cmp	fp, r3
	bne.n	.L38
	ldr	r3, [r6, #0]
	ldr	r4, [sp, #12]
	movs	r2, #1
	ldrsh	r0, [r3, r4]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	ldr	r3, [sp, #52]
	bl	Func_080d6888
.L38:
	ldr	r0, [sp, #16]
	cmp	fp, r0
	bgt.n	.L39
	b.n	.L40
.L39:
	movs	r1, #2
	str	r1, [sp, #0]
	ldr	r3, [sp, #44]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	bl	Func_080ed408
	ldr	r2, [pc, #132]
	ldr	r2, [r2, #0]
	movs	r3, #3
	str	r2, [sp, #56]
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	ldr	r3, [sp, #44]
	bl	Func_080ed408
	ldr	r4, [pc, #104]
	ldr	r5, [sp, #72]
	ldr	r4, [r4, #0]
	str	r4, [sp, #60]
	cmp	r5, #0
	bne.n	.L41
	mov	r1, fp
	negs	r0, r1
	lsls	r0, r0, #10
	bl	Func_08004bd4
	b.n	.L42
.L41:
	ldr	r2, [sp, #72]
	cmp	r2, #1
	beq.n	.L42
	cmp	r2, #2
	bne.n	.L43
	mov	r3, fp
	lsls	r0, r3, #10
	bl	Func_08004c1c
	b.n	.L42
	.4byte 0x00000093
	.4byte 0x0000008d
	.4byte 0x03001388
	.4byte 0x00002b8e
	.4byte 0x000000a5
	.4byte 0x0000009c
	.4byte 0x0000009b
	.4byte 0x0000008f
	.4byte 0x000000bb
	.4byte 0x000065c0
	.4byte 0x02010000
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x03001f0c
	.4byte 0x03001f08
.L43:
	mov	r4, fp
	lsls	r5, r4, #10
	adds	r0, r5, #0
	bl	Func_08004c1c
	adds	r0, r5, #0
	bl	Func_08004c6c
.L42:
	ldr	r0, [sp, #52]
	lsls	r2, r0, #6
	lsls	r3, r0, #9
	subs	r3, r3, r2
	ldr	r1, [sp, #24]
	ldr	r2, [pc, #756]
	movs	r5, #0
	lsls	r3, r3, #2
	mov	sl, r5
	mov	r8, r1
	adds	r6, r3, r2
.L48:
	ldr	r3, [sp, #16]
	add	r3, sl
	cmp	fp, r3
	ble.n	.L44
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	asrs	r3, r3, #8
	adds	r4, r3, #0
	muls	r4, r3
	adds	r0, r0, r2
	adds	r3, r4, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #708]
	bl	Func_080072f0
	asrs	r7, r0, #9
	cmp	r7, #0
	beq.n	.L44
	mov	r5, r8
	mov	r1, r8
	adds	r0, r6, #0
	bl	Func_080e3944
	ldr	r0, [r5, #0]
	movs	r1, #5
	lsls	r0, r0, #2
	bl	Func_080022ec
	ldr	r1, [sp, #48]
	ldr	r2, [r5, #8]
	ldr	r3, [pc, #676]
	adds	r0, r0, r1
	str	r0, [r5, #0]
	cmp	r2, r3
	bgt.n	.L45
	ldr	r4, [sp, #24]
	adds	r3, #1
	str	r3, [r4, #8]
	adds	r2, r3, #0
.L45:
	ldr	r3, [pc, #664]
	cmp	r2, r3
	ble.n	.L46
	ldr	r5, [sp, #24]
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L46:
	ldr	r0, [pc, #656]
	adds	r3, r2, r0
	cmp	r3, #0
	bge.n	.L47
	adds	r3, r2, #0
	subs	r3, #251
.L47:
	asrs	r3, r3, #6
	movs	r0, #6
	subs	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r2, [pc, #640]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #24]
	adds	r1, r2, r1
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #64]
	ldr	r4, [sp, #60]
	bl	Func_080072f4
	ldr	r5, [r6, #0]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	Func_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #8]
.L44:
	movs	r5, #1
	add	sl, r5
	mov	r0, sl
	adds	r6, #28
	cmp	r0, #32
	bne.n	.L48
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
.L40:
	ldr	r1, [sp, #20]
	movs	r3, #0
	str	r3, [r1, #0]
	str	r3, [r1, #4]
	str	r3, [r1, #8]
	add	r2, sp, #96
	adds	r1, r2, #0
	ldr	r0, [sp, #20]
	bl	Func_080e3944
	ldr	r0, [sp, #96]
	movs	r1, #5
	lsls	r0, r0, #2
	bl	Func_080022ec
	ldr	r3, [sp, #48]
	adds	r0, r0, r3
	ldr	r3, [sp, #16]
	adds	r3, #52
	str	r0, [sp, #96]
	cmp	fp, r3
	blt.n	.L49
	ldr	r3, [sp, #16]
	adds	r3, #76
	cmp	fp, r3
	bge.n	.L49
	mov	r3, r9
	subs	r3, #52
	cmp	r3, #0
	bge.n	.L50
	adds	r3, #3
.L50:
	asrs	r0, r3, #2
	movs	r1, #6
	bl	Func_080022fc
	movs	r4, #2
	adds	r5, r0, #0
	ldr	r3, [sp, #44]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r4, [sp, #0]
	bl	Func_080ed408
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #3
	ldr	r2, [sp, #68]
	adds	r1, r1, r5
	ldr	r0, [pc, #448]
	ldr	r5, [sp, #24]
	ldr	r3, [pc, #448]
	lsls	r1, r1, #6
	adds	r1, r2, r1
	ldr	r4, [r0, #0]
	ldr	r2, [r5, #0]
	adds	r1, r1, r3
	ldr	r3, [r5, #4]
	movs	r0, #40
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	str	r4, [sp, #56]
	subs	r2, #20
	subs	r3, #20
	ldr	r0, [sp, #64]
	bl	Func_080072f4
	movs	r0, #46
	bl	Func_08002dd8
.L49:
	ldr	r0, [sp, #72]
	cmp	r0, #0
	bne.n	.L51
	ldr	r3, [sp, #16]
	adds	r3, #80
	cmp	fp, r3
	bge.n	.L52
	b.n	.L53
.L52:
	ldr	r3, [sp, #16]
	adds	r3, #108
	cmp	fp, r3
	blt.n	.L54
	b.n	.L53
.L54:
	mov	r3, r9
	subs	r3, #80
	cmp	r3, #0
	bge.n	.L55
	adds	r3, #3
.L55:
	asrs	r0, r3, #2
	movs	r1, #7
	bl	Func_080022fc
	movs	r1, #2
	adds	r5, r0, #0
	str	r1, [sp, #0]
	ldr	r3, [sp, #44]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	bl	Func_080ed408
	lsls	r1, r5, #4
	ldr	r2, [pc, #340]
	ldr	r3, [sp, #68]
	subs	r1, r1, r5
	lsls	r1, r1, #6
	ldr	r4, [r2, #0]
	adds	r1, r3, r1
	ldr	r2, [sp, #96]
	ldr	r3, [sp, #100]
	movs	r0, #24
	ldr	r5, [pc, #332]
	str	r0, [sp, #0]
	movs	r0, #40
	str	r0, [sp, #4]
	str	r4, [sp, #56]
	adds	r1, r1, r5
	subs	r2, #12
	subs	r3, #20
	ldr	r0, [sp, #64]
	bl	Func_080072f4
	b.n	.L56
.L51:
	ldr	r0, [sp, #72]
	cmp	r0, #3
	beq.n	.L57
	cmp	r0, #5
	bne.n	.L58
.L57:
	ldr	r3, [sp, #16]
	adds	r3, #80
	cmp	fp, r3
	bge.n	.L59
	b.n	.L53
.L59:
	ldr	r3, [sp, #16]
	adds	r3, #104
	cmp	fp, r3
	blt.n	.L60
	b.n	.L53
.L60:
	mov	r3, r9
	subs	r3, #80
	cmp	r3, #0
	bge.n	.L61
	adds	r3, #3
.L61:
	asrs	r0, r3, #2
	movs	r1, #6
	bl	Func_080022fc
	movs	r1, #2
	str	r1, [sp, #0]
	ldr	r3, [sp, #44]
	adds	r5, r0, #0
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	bl	Func_080ed408
	ldr	r2, [pc, #232]
	ldr	r3, [sp, #68]
	ldr	r0, [pc, #236]
	lsls	r5, r5, #11
	ldr	r4, [r2, #0]
	adds	r5, r3, r5
	ldr	r2, [sp, #96]
	ldr	r3, [sp, #100]
	movs	r1, #32
	adds	r5, r5, r0
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	str	r4, [sp, #56]
	subs	r2, #16
	subs	r3, #32
	ldr	r0, [sp, #64]
	adds	r1, r5, #0
	bl	Func_080072f4
	b.n	.L56
.L58:
	ldr	r1, [sp, #72]
	cmp	r1, #4
	bne.n	.L62
	ldr	r3, [sp, #16]
	adds	r3, #80
	cmp	fp, r3
	bge.n	.L63
	b.n	.L53
.L63:
	ldr	r3, [sp, #16]
	adds	r3, #104
	cmp	fp, r3
	blt.n	.L64
	b.n	.L53
.L64:
	mov	r0, r9
	subs	r0, #80
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	movs	r1, #6
	asrs	r0, r0, #1
	bl	Func_080022fc
	movs	r2, #2
	str	r2, [sp, #0]
	ldr	r3, [sp, #44]
	adds	r5, r0, #0
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	bl	Func_080ed408
	ldr	r3, [pc, #132]
	movs	r4, #2
	ldr	r3, [r3, #0]
	str	r4, [sp, #0]
	ldr	r4, [sp, #44]
	str	r3, [sp, #56]
	mov	r8, r3
	movs	r3, #8
	orrs	r3, r4
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	bl	Func_080ed408
	ldr	r1, [sp, #68]
	ldr	r0, [pc, #112]
	ldr	r2, [pc, #108]
	lsls	r5, r5, #11
	adds	r5, r1, r5
	ldr	r0, [r0, #0]
	ldr	r3, [sp, #100]
	adds	r5, r5, r2
	ldr	r2, [sp, #96]
	movs	r4, #64
	movs	r6, #32
	str	r0, [sp, #60]
	str	r4, [sp, #0]
	str	r4, [sp, #8]
	str	r6, [sp, #4]
	mov	sl, r0
	subs	r2, #32
	ldr	r0, [sp, #64]
	subs	r3, #24
	adds	r1, r5, #0
	bl	Func_08007304
	ldr	r2, [sp, #96]
	ldr	r3, [sp, #100]
	ldr	r4, [sp, #8]
	subs	r2, #32
	adds	r3, #8
	str	r4, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #64]
	adds	r1, r5, #0
	bl	Func_0800730c
	movs	r0, #47
	bl	Func_08002dd8
.L56:
	movs	r0, #46
	bl	Func_08002dd8
	b.n	.L53
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x030001d8
	.4byte 0x00000139
	.4byte 0x0000027a
	.4byte 0xfffffec6
	.4byte 0x080ede5c
	.4byte 0x03001f08
	.4byte 0x0000060e
	.4byte 0x00002b8e
	.4byte 0x03001f0c
.L62:
	ldr	r3, [sp, #16]
	adds	r3, #80
	cmp	fp, r3
	blt.n	.L53
	ldr	r3, [sp, #16]
	adds	r3, #104
	cmp	fp, r3
	bge.n	.L53
	mov	r3, r9
	subs	r3, #80
	cmp	r3, #0
	bge.n	.L65
	adds	r3, #3
.L65:
	asrs	r0, r3, #2
	movs	r1, #6
	bl	Func_080022fc
	movs	r3, #3
	adds	r5, r0, #0
	str	r3, [sp, #0]
	movs	r1, #7
	ldr	r3, [sp, #44]
	movs	r2, #7
	movs	r0, #46
	bl	Func_080ed408
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #3
	ldr	r2, [sp, #68]
	adds	r1, r1, r5
	ldr	r0, [pc, #148]
	ldr	r3, [pc, #148]
	lsls	r1, r1, #6
	adds	r1, r2, r1
	ldr	r4, [r0, #0]
	adds	r1, r1, r3
	ldr	r2, [sp, #96]
	ldr	r3, [sp, #100]
	movs	r0, #40
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	str	r4, [sp, #56]
	subs	r2, #20
	subs	r3, #20
	ldr	r0, [sp, #64]
	bl	Func_080072f4
	movs	r0, #46
	bl	Func_08002dd8
.L53:
	ldr	r5, [sp, #16]
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #52]
	movs	r4, #8
	negs	r4, r4
	adds	r5, #8
	adds	r0, #2
	adds	r1, #1
	str	r5, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #52]
	add	r9, r4
	ldr	r2, [pc, #92]
	ldr	r4, [sp, #68]
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L36
	b.n	.L66
.L36:
	bl	Func_080cd52c
	ldr	r0, [pc, #76]
	ldr	r5, [sp, #68]
	movs	r3, #1
	adds	r2, r5, r0
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #56]
	adds	r3, r5, r2
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r1, #1
	lsls	r3, r3, #3
	add	fp, r1
	adds	r3, #108
	cmp	fp, r3
	beq.n	.L30
	b.n	.L67
.L30:
	ldr	r0, [pc, #44]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f08
	.4byte 0x00002b8e
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261
