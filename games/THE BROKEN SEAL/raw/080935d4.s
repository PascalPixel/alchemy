.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08004278, 0x08004278
	.set sub_080048f4, 0x080048f4
	.global Overlay_080935d4
Overlay_080935d4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #168]
	ldr	r1, [pc, #172]
	movs	r0, #27
	ldr	r7, [r3, #0]
	bl	sub_080048f4
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r0, r0, r1
	ldr	r3, [r0, #0]
	adds	r3, #91
	ldrb	r3, [r3, #0]
	mov	sl, r3
	cmp	r3, #0
	bne.n	.L_0809367c
	movs	r2, #214
	lsls	r2, r2, #2
	adds	r2, r2, r7
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_0809367c
	movs	r1, #212
	movs	r2, #213
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	adds	r6, r7, r1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldr	r3, [r6, #0]
	subs	r2, r2, r3
	ldr	r3, [pc, #112]
	adds	r5, r7, r3
	ldrh	r3, [r5, #0]
	adds	r3, #1
	strh	r3, [r5, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r0, r3, #0
	muls	r0, r2
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_080022ec
	adds	r2, r0, #0
	movs	r0, #210
	ldr	r1, [r6, #0]
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldr	r4, [pc, #80]
	ldr	r0, [r3, #0]
	adds	r1, r1, r2
	mov	ip, pc
	bx	r4
	movs	r1, #211
	lsls	r1, r1, #2
	adds	r3, r7, r1
	str	r0, [r3, #0]
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #56]
	adds	r3, #1
	str	r3, [r2, #0]
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	mov	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	bne.n	.L_0809367c
	mov	r2, sl
	mov	r3, r8
	strh	r2, [r3, #0]
	ldr	r0, [pc, #36]
	bl	sub_08004278
.L_0809367c:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e70
	.4byte 0x00000ccc
	.4byte 0x0000035a
	.4byte 0x03000118
