.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.global Overlay_08024dec
Overlay_08024dec:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #4]
	bl	sub_08016ce4
	adds	r3, r5, #0
	adds	r3, #87
	strb	r0, [r3, #0]
	movs	r0, #1
	ldrh	r3, [r5, #4]
	adds	r3, #2
	strh	r3, [r5, #4]
	pop	{r5, pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r6, [r3, #4]
	adds	r0, r6, #0
	bl	sub_08016ce4
	adds	r3, r5, #0
	adds	r3, #87
	strb	r0, [r3, #0]
	adds	r0, r6, #0
	bl	sub_08016cfc
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #2
	strh	r3, [r5, #4]
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r6, [r3, #4]
	adds	r0, r6, #0
	bl	sub_08016ce4
	adds	r3, r5, #0
	adds	r3, #87
	strb	r0, [r3, #0]
	adds	r0, r6, #0
	bl	sub_08016d18
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #2
	strh	r3, [r5, #4]
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r2, #4
	ldrsh	r3, [r6, r2]
	ldr	r2, [r6, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r5, [r3, #4]
	adds	r0, r5, #0
	bl	sub_08016ce4
	adds	r3, r6, #0
	adds	r3, #87
	strb	r0, [r3, #0]
	movs	r3, #128
	lsls	r0, r0, #24
	lsls	r3, r3, #17
	cmp	r0, r3
	bne.n	.L_08024e96
	adds	r0, r5, #0
	bl	sub_08016d18
	b.n	.L_08024e9c
.L_08024e96:
	adds	r0, r5, #0
	bl	sub_08016cfc
.L_08024e9c:
	ldrh	r3, [r6, #4]
	movs	r0, #1
	adds	r3, #2
	strh	r3, [r6, #4]
	pop	{r5, r6, pc}
