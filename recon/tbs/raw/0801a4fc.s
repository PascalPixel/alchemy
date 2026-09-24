.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080048b0, 0x080048b0
	.set sub_0801a5a4, 0x0801a5a4
	.global UiGlyph_LoadEntryWithPalette
	.global Func_0801a4fc
	.thumb_func
UiGlyph_LoadEntryWithPalette:
Func_0801a4fc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #193
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #17
	mov	r8, r3
	adds	r7, r2, #0
	bl	sub_080048b0
	adds	r6, r0, #0
	ldr	r0, [pc, #116]
	bl	sub_08002f40
	adds	r3, r5, #0
	cmp	r5, #127
	bls.n	.L_0801a522
	subs	r3, #112
.L_0801a522:
	lsls	r3, r3, #1
	ldrh	r3, [r3, r0]
	ldr	r1, [pc, #104]
	adds	r5, r0, r3
	adds	r3, r5, #0
	adds	r2, r6, r1
	adds	r3, #32
	str	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r3, r6, r2
	subs	r1, #2
	movs	r2, #4
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_0801a5a4
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_0801a556
	bl	sub_08004080
	str	r0, [r7, #0]
.L_0801a556:
	movs	r3, #128
	lsls	r3, r3, #3
	movs	r1, #128
	adds	r2, r6, r3
	ldr	r0, [r7, #0]
	lsls	r1, r1, #2
	bl	sub_08003fa4
	mov	r1, r8
	str	r0, [r1, #0]
	movs	r0, #17
	bl	sub_08002dd8
	ldr	r1, [sp, #20]
	ldr	r2, [pc, #32]
	lsls	r1, r1, #5
	adds	r1, r1, r2
	ldr	r3, [pc, #28]
	adds	r0, r5, #0
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000000f0
	.4byte 0x00000604
	.4byte 0x05000200
	.4byte 0x040000d4
	.2byte 0x0010
	.2byte 0x8000
	bx	lr
	.2byte 0x0000
