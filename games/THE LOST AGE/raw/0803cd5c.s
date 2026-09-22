.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0803939c, 0x0803939c
	.set sub_081c0048, 0x081c0048
	.global Overlay_0803cd5c
Overlay_0803cd5c:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #137
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	adds	r6, r0, #0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0803cd80
	bl	sub_081c0048
	cmp	r0, #0
	bne.n	.L_0803cd80
	movs	r5, #1
.L_0803cd80:
	ldr	r1, [pc, #44]
	movs	r2, #129
	ldr	r3, [r1, #0]
	lsls	r2, r2, #2
	adds	r2, #255
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803cd92
	movs	r5, #1
.L_0803cd92:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803cd9e
	movs	r5, #1
.L_0803cd9e:
	cmp	r5, #0
	beq.n	.L_0803cdaa
	movs	r3, #0
	strh	r3, [r6, #20]
	movs	r0, #1
	b.n	.L_0803cdac
.L_0803cdaa:
	movs	r0, #0
.L_0803cdac:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #137
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	adds	r5, r0, #0
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_0803cdd8
	bl	sub_081c0048
	cmp	r0, #0
	bne.n	.L_0803cdd8
	movs	r7, #1
.L_0803cdd8:
	ldr	r1, [pc, #60]
	ldrb	r3, [r6, #4]
	ldr	r2, [r1, #4]
	cmp	r3, #0
	beq.n	.L_0803cde4
	ldr	r2, [r1, #28]
.L_0803cde4:
	movs	r3, #129
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803cdf2
	movs	r7, #1
.L_0803cdf2:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803ce06
	ldrh	r3, [r5, #20]
	cmp	r3, #1
	bne.n	.L_0803ce08
	movs	r7, #1
	b.n	.L_0803ce08
.L_0803ce06:
	strh	r2, [r5, #20]
.L_0803ce08:
	cmp	r7, #0
	beq.n	.L_0803ce14
	movs	r3, #0
	strh	r3, [r5, #20]
	movs	r0, #1
	b.n	.L_0803ce16
.L_0803ce14:
	movs	r0, #0
.L_0803ce16:
	pop	{r5, r6, r7, pc}
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #215
	lsls	r2, r2, #3
	adds	r5, r3, r2
	movs	r6, #0
.L_0803ce2c:
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_0803ce52
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_0803ce52
	ldrh	r2, [r0, #22]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0803ce52
	ldrh	r3, [r0, #20]
	cmp	r3, #0
	beq.n	.L_0803ce52
	movs	r1, #2
	ands	r1, r2
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	bl	sub_0803939c
.L_0803ce52:
	adds	r6, #1
	adds	r5, #40
	cmp	r6, #3
	bne.n	.L_0803ce2c
	movs	r0, #10
	bl	sub_08013560
	pop	{r5, r6, pc}
