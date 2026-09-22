.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08077008, 0x08077008
	.set sub_08077148, 0x08077148
	.set sub_08077168, 0x08077168
	.set sub_08091858, 0x08091858
	.global Overlay_08091890
Overlay_08091890:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	bl	sub_08077168
	bl	sub_08091858
	adds	r0, r5, #0
	bl	sub_08077008
	adds	r6, r0, #0
	ldrh	r1, [r6, #52]
	ldrh	r3, [r6, #54]
	strh	r1, [r6, #56]
	strh	r3, [r6, #58]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080918ca
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080918ca
	adds	r3, r0, #0
.L_080918ca:
	strh	r3, [r6, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080918de
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_080918de
	movs	r3, #1
	strh	r3, [r6, #20]
.L_080918de:
	movs	r3, #58
	ldrsh	r0, [r6, r3]
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	lsls	r0, r0, #14
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080918fc
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080918fc
	adds	r3, r0, #0
.L_080918fc:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_08091910
	movs	r2, #58
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_08091910
	movs	r3, #1
	strh	r3, [r6, #22]
.L_08091910:
	ldr	r3, [pc, #188]
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r8, r3
	bl	sub_08077148
	cmp	r8, r0
	bge.n	.L_08091948
	ldr	r3, [pc, #176]
	movs	r2, #252
	lsls	r2, r2, #1
	adds	r7, r3, r2
	adds	r5, r0, #0
.L_0809192c:
	ldrb	r0, [r7, #0]
	bl	sub_08077008
	adds	r6, r0, #0
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	adds	r7, #1
	cmp	r3, #0
	beq.n	.L_08091942
	movs	r3, #1
	add	r8, r3
.L_08091942:
	subs	r5, #1
	cmp	r5, #0
	bne.n	.L_0809192c
.L_08091948:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080919c4
	ldr	r3, [pc, #132]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_08077008
	movs	r5, #1
	adds	r6, r0, #0
	strh	r5, [r6, #56]
	lsls	r5, r5, #14
	movs	r3, #52
	ldrsh	r1, [r6, r3]
	adds	r0, r5, #0
	bl	sub_080022ec
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r0, r2
	bgt.n	.L_0809197e
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L_0809197e
	adds	r5, r0, #0
.L_0809197e:
	lsls	r3, r5, #16
	strh	r5, [r6, #20]
	cmp	r3, #0
	bne.n	.L_08091992
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_08091992
	movs	r3, #1
	strh	r3, [r6, #20]
.L_08091992:
	movs	r3, #58
	ldrsh	r0, [r6, r3]
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	lsls	r0, r0, #14
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080919b0
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080919b0
	adds	r3, r0, #0
.L_080919b0:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080919c4
	movs	r2, #58
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_080919c4
	movs	r3, #1
	strh	r3, [r6, #22]
.L_080919c4:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000131
	.4byte 0x02000240
