.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.global Overlay_0801cc50
Overlay_0801cc50:
.L_0801cc50:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r2, #0
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	ldr	r4, [pc, #96]
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r7, r0, #16
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	adds	r1, r6, #0
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r6, r0, #16
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	adds	r1, r3, #0
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r0, r0, #16
	cmp	r7, #0
	bge.n	.L_0801cc8c
	movs	r7, #0
.L_0801cc8c:
	cmp	r7, #31
	ble.n	.L_0801cc92
	movs	r7, #31
.L_0801cc92:
	cmp	r6, #0
	bge.n	.L_0801cc98
	movs	r6, #0
.L_0801cc98:
	cmp	r6, #31
	ble.n	.L_0801cc9e
	movs	r6, #31
.L_0801cc9e:
	cmp	r0, #0
	bge.n	.L_0801cca4
	movs	r0, #0
.L_0801cca4:
	cmp	r0, #31
	ble.n	.L_0801ccaa
	movs	r0, #31
.L_0801ccaa:
	lsls	r3, r6, #5
	lsls	r0, r0, #10
	adds	r0, r0, r3
	adds	r0, r7, r0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0x0118
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r1, #0
	adds	r0, #12
	movs	r1, #24
	sub	sp, #8
	bl	sub_080022fc
	adds	r5, r0, #0
	lsls	r5, r5, #18
	asrs	r5, r5, #16
	ldr	r2, [pc, #268]
	adds	r0, r5, #0
	movs	r1, #96
	mov	r8, r2
	bl	sub_080022fc
	lsls	r0, r0, #16
	mov	r2, r8
	asrs	r0, r0, #16
	subs	r6, #7
	ldrb	r3, [r2, r0]
	lsls	r6, r6, #16
	asrs	r6, r6, #16
	adds	r3, r3, r6
	adds	r0, r5, #0
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	movs	r1, #96
	adds	r0, #32
	mov	sl, r3
	bl	sub_080022fc
	mov	r2, r8
	ldrb	r3, [r2, r0]
	adds	r5, #64
	adds	r3, r3, r6
	lsls	r3, r3, #16
	adds	r0, r5, #0
	movs	r1, #96
	asrs	r7, r3, #16
	bl	sub_080022fc
	mov	r2, r8
	ldrb	r3, [r2, r0]
	adds	r3, r3, r6
	lsls	r3, r3, #16
	mov	r2, sl
	asrs	r3, r3, #16
	cmp	r2, #0
	bge.n	.L_0801cd30
	movs	r2, #0
	mov	sl, r2
.L_0801cd30:
	mov	r2, sl
	cmp	r2, #31
	ble.n	.L_0801cd3a
	movs	r2, #31
	mov	sl, r2
.L_0801cd3a:
	cmp	r7, #0
	bge.n	.L_0801cd40
	movs	r7, #0
.L_0801cd40:
	cmp	r7, #31
	ble.n	.L_0801cd46
	movs	r7, #31
.L_0801cd46:
	cmp	r3, #0
	bge.n	.L_0801cd4c
	movs	r3, #0
.L_0801cd4c:
	cmp	r3, #31
	ble.n	.L_0801cd52
	movs	r3, #31
.L_0801cd52:
	mov	r5, sp
	strh	r3, [r5, #4]
	ldr	r3, [pc, #152]
	mov	r2, sl
	strh	r2, [r5, #0]
	mov	sl, r3
	ldr	r2, [pc, #148]
	mov	r1, sl
	strh	r7, [r5, #2]
	ldr	r3, [pc, #144]
	adds	r0, r5, #0
	mov	r9, r2
	bl	.L_0801cc50
	ldr	r6, [pc, #140]
	ldr	r3, [pc, #140]
	adds	r2, r6, #0
	strh	r0, [r3, #0]
	ldr	r1, [pc, #140]
	mov	r3, sl
	adds	r0, r5, #0
	bl	.L_0801cc50
	ldr	r3, [pc, #132]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #132]
	mov	r8, r3
	adds	r1, r6, #0
	mov	r2, r8
	mov	r3, r9
	adds	r0, r5, #0
	bl	.L_0801cc50
	ldr	r3, [pc, #120]
	ldr	r1, [pc, #124]
	strh	r0, [r3, #0]
	ldr	r2, [pc, #124]
	mov	r3, r8
	adds	r0, r5, #0
	bl	.L_0801cc50
	ldr	r3, [pc, #116]
	ldr	r1, [pc, #120]
	strh	r0, [r3, #0]
	ldr	r2, [pc, #120]
	ldr	r3, [pc, #120]
	adds	r0, r5, #0
	bl	.L_0801cc50
	ldr	r3, [pc, #116]
	mov	r2, sl
	strh	r0, [r3, #0]
	ldr	r1, [pc, #116]
	ldr	r3, [pc, #116]
	adds	r0, r5, #0
	bl	.L_0801cc50
	ldr	r3, [pc, #112]
	movs	r2, #128
	strh	r0, [r3, #0]
	ldr	r1, [pc, #112]
	lsls	r2, r2, #9
	ldr	r3, [pc, #112]
	adds	r0, r5, #0
	bl	.L_0801cc50
	ldr	r3, [pc, #108]
	add	sp, #8
	strh	r0, [r3, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x08036750
	.4byte 0x0000eeee
	.4byte 0x0000cccc
	.4byte 0x00011110
	.4byte 0x0000bbbb
	.4byte 0x050001e8
	.4byte 0x0000d555
	.4byte 0x050001ea
	.4byte 0x0000aaaa
	.4byte 0x050001ec
	.4byte 0x0000a221
	.4byte 0x00009999
	.4byte 0x050001ee
	.4byte 0x00010888
	.4byte 0x0000dddd
	.4byte 0x00013333
	.4byte 0x050001f0
	.4byte 0x00012221
	.4byte 0x00015555
	.4byte 0x050001f2
	.4byte 0x00013bbb
	.4byte 0x00017777
	.4byte 0x050001f4
