.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_0808ba1c, 0x0808ba1c
	.set sub_0808e4b4, 0x0808e4b4
	.set sub_0808f32c, 0x0808f32c
	.set sub_0808fe38, 0x0808fe38
	.set sub_08091200, 0x08091200
	.set sub_08091220, 0x08091220
	.set sub_08091254, 0x08091254
	.set sub_08096b28, 0x08096b28
	.set sub_08098294, 0x08098294
	.set sub_080f9010, 0x080f9010
	.global Overlay_080983a0
Overlay_080983a0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r5, [pc, #228]
	movs	r0, #6
	sub	sp, #4
	ldr	r6, [r5, #0]
	bl	sub_08098294
	movs	r0, #8
	bl	sub_0808fe38
	ldr	r3, [pc, #216]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r0, r0, r3
	mov	sl, r0
	ldr	r0, [r0, #0]
	ldr	r7, [r5, #16]
	bl	sub_0808ba1c
	ldr	r2, [pc, #200]
	ldr	r3, [r0, #8]
	adds	r5, r7, r2
	str	r3, [r5, #0]
	movs	r3, #166
	lsls	r3, r3, #3
	adds	r3, r3, r7
	ldr	r2, [r0, #12]
	mov	r8, r3
	ldr	r3, [r0, #16]
	mov	r0, r8
	subs	r3, r3, r2
	str	r3, [r0, #0]
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #0
	bl	sub_08091220
	movs	r1, #1
	ldr	r0, [pc, #168]
	bl	sub_08091200
	movs	r0, #1
	bl	sub_08091254
	movs	r0, #1
	bl	sub_080030f8
	ldr	r0, [pc, #152]
	mov	r2, sp
	movs	r1, #8
	bl	sub_0808e4b4
	cmp	r0, #0
	beq.n	.L_0809841c
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r2, [sp, #0]
	bl	sub_08096b28
.L_0809841c:
	movs	r0, #131
	bl	sub_080f9010
	ldr	r0, [pc, #128]
	movs	r1, #1
	adds	r3, r6, r0
	strh	r1, [r3, #0]
	ldr	r2, [r5, #0]
	cmp	r2, #0
	bge.n	.L_08098434
	ldr	r3, [pc, #116]
	adds	r2, r2, r3
.L_08098434:
	ldr	r0, [pc, #116]
	asrs	r2, r2, #16
	adds	r3, r6, r0
	strh	r2, [r3, #0]
	mov	r3, r8
	ldr	r2, [r3, #0]
	cmp	r2, #0
	bge.n	.L_08098448
	ldr	r0, [pc, #96]
	adds	r2, r2, r0
.L_08098448:
	ldr	r0, [pc, #100]
	asrs	r2, r2, #16
	adds	r3, r6, r0
	strh	r2, [r3, #0]
	ldr	r3, [pc, #96]
	adds	r2, r6, r3
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r0, #2
	strh	r3, [r2, #0]
	adds	r3, r6, r0
	strh	r1, [r3, #0]
	bl	sub_0808f32c
	ldr	r2, [pc, #80]
	movs	r5, #0
	adds	r6, r7, r2
.L_0809846a:
	movs	r0, #1
	bl	sub_080030f8
	strh	r5, [r6, #0]
	adds	r5, #1
	cmp	r5, #18
	ble.n	.L_0809846a
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #60]
	bl	sub_080041d8
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000052c
	.4byte 0x00010001
	.4byte 0x50000005
	.4byte 0x00000cb8
	.4byte 0x0000ffff
	.4byte 0x00000cbc
	.4byte 0x00000cbe
	.4byte 0x00000cba
	.4byte 0x0000052a
	.4byte 0x080982dd
