.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008581, 0x02008039, 0x02008045, 0x0200804d, 0x020084f1, 0x02008041, 0x0200870d
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88bc
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88ec
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #44]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000064
	ldr	r0, [pc, #32]
	b.n	.L_0200007a
.L_02000064:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_0200006e
	ldr	r0, [pc, #32]
	b.n	.L_0200007a
.L_0200006e:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02000078
	ldr	r0, [pc, #28]
	b.n	.L_0200007a
.L_02000078:
	ldr	r0, [pc, #28]
.L_0200007a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000040
	.4byte 0x02008964
	.4byte 0x00000041
	.4byte 0x02008ab4
	.4byte 0x00000042
	.4byte 0x02008b2c
	.2byte 0x894c
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200880c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #36]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020000e4
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_020000e4
	movs	r0, #10
	adds	r1, r5, #0
	bl 0x0200889c
	b.n	.L_02000108
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020000e4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_020000fa
	ldr	r0, [pc, #24]
	bl 0x02008844
	b.n	.L_02000100
.L_020000fa:
	ldr	r0, [pc, #20]
	bl 0x02008844
.L_02000100:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200885c
.L_02000108:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001a1e
	.2byte 0x1999
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #64]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200880c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #36]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200015c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_0200015c
	movs	r0, #11
	adds	r1, r5, #0
	bl 0x0200889c
	b.n	.L_02000180
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200015c:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_02000172
	ldr	r0, [pc, #24]
	bl 0x02008844
	b.n	.L_02000178
.L_02000172:
	ldr	r0, [pc, #20]
	bl 0x02008844
.L_02000178:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200885c
.L_02000180:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x00001a20
	.2byte 0x199b
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200880c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020001c8
	movs	r0, #12
	adds	r1, r6, #0
	bl 0x0200889c
	b.n	.L_02000226
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020001c8:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_020001e6
	ldr	r0, [pc, #80]
	bl 0x02008844
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200885c
	b.n	.L_02000226
.L_020001e6:
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl 0x02008844
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x0200884c
	bl 0x0200888c
	movs	r1, #0
	bl 0x02008804
	cmp	r0, #0
	bne.n	.L_02000212
	movs	r0, #10
	bl 0x020087ec
	adds	r0, r5, #1
	bl 0x02008844
	b.n	.L_0200021e
.L_02000212:
	movs	r0, #20
	bl 0x020087ec
	adds	r0, r5, #2
	bl 0x02008844
.L_0200021e:
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x0200885c
.L_02000226:
	pop	{r5, r6, pc}
	.4byte 0x00001a22
	.2byte 0x199d
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl 0x0200880c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #44]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000262
	movs	r0, #3
	adds	r1, r5, #0
	bl 0x020088ac
	b.n	.L_02000292
.L_02000262:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_02000284
	ldr	r0, [pc, #12]
	bl 0x02008844
	b.n	.L_0200028a
	.4byte 0xffffc000
	.4byte 0x02000240
	.2byte 0x1a24
	.2byte 0x0000
.L_02000284:
	ldr	r0, [pc, #12]
	bl 0x02008844
.L_0200028a:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200885c
.L_02000292:
	pop	{r5, pc}
	.2byte 0x19a1
	.2byte 0x0000
	push	{r5, r6, lr}
	ldr	r6, [pc, #60]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r6, r6, r3
	ldr	r0, [r6, #0]
	bl 0x0200880c
	ldrh	r5, [r0, #6]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r5, r5, r3
	ldr	r3, [pc, #32]
	ldr	r1, [r6, #0]
	ands	r5, r3
	lsls	r5, r5, #16
	movs	r0, #8
	movs	r2, #0
	bl 0x0200883c
	asrs	r5, r5, #16
	movs	r3, #192
	lsls	r5, r5, #16
	lsls	r3, r3, #24
	cmp	r5, r3
	bne.n	.L_020002dc
	movs	r0, #8
	bl 0x020088a4
	b.n	.L_02000300
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002dc:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_020002f2
	ldr	r0, [pc, #36]
	bl 0x02008844
	b.n	.L_020002f8
.L_020002f2:
	ldr	r0, [pc, #32]
	bl 0x02008844
.L_020002f8:
	movs	r0, #8
	movs	r1, #0
	bl 0x0200885c
.L_02000300:
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #0
	bl 0x02008864
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x00001a2c
	.2byte 0x19a9
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	bl 0x020087f4
	movs	r0, #0
	bl 0x02008884
	ldr	r7, [pc, #108]
	adds	r0, r7, #0
	bl 0x02008844
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200884c
	ldr	r3, [pc, #96]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r3, r2
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x02008804
	cmp	r0, #0
	bne.n	.L_02000384
	adds	r0, r7, #2
	bl 0x02008844
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200884c
	ldr	r0, [r6, #0]
	movs	r1, #0
	bl 0x02008804
	cmp	r0, #0
	bne.n	.L_02000368
	adds	r0, r5, #0
	b.n	.L_0200037c
.L_02000368:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #108]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r3, #1
	strh	r3, [r2, #0]
.L_0200037c:
	movs	r1, #0
	bl 0x0200885c
	b.n	.L_0200038c
.L_02000384:
	adds	r0, r5, #0
	movs	r1, #0
	bl 0x0200885c
.L_0200038c:
	bl 0x020087fc
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x00001985
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl 0x020087f4
	movs	r0, #0
	bl 0x02008884
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_020003be
	ldr	r0, [pc, #28]
	bl 0x02008844
	b.n	.L_020003c4
.L_020003be:
	ldr	r0, [pc, #24]
	bl 0x02008844
.L_020003c4:
	movs	r0, #17
	movs	r1, #0
	bl 0x0200885c
	bl 0x020087fc
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001a26
	.2byte 0x19a3
	.2byte 0x0000
	push	{lr}
	bl 0x020087f4
	movs	r0, #0
	bl 0x02008884
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_020003fe
	ldr	r0, [pc, #28]
	bl 0x02008844
	b.n	.L_02000404
.L_020003fe:
	ldr	r0, [pc, #24]
	bl 0x02008844
.L_02000404:
	movs	r0, #18
	movs	r1, #0
	bl 0x0200885c
	bl 0x020087fc
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001a27
	.2byte 0x19a4
	.2byte 0x0000
	push	{lr}
	bl 0x020087f4
	movs	r0, #0
	bl 0x02008884
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_0200043e
	ldr	r0, [pc, #28]
	bl 0x02008844
	b.n	.L_02000444
.L_0200043e:
	ldr	r0, [pc, #24]
	bl 0x02008844
.L_02000444:
	movs	r0, #17
	movs	r1, #0
	bl 0x0200885c
	bl 0x020087fc
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001a2a
	.2byte 0x19a7
	.2byte 0x0000
	push	{lr}
	bl 0x020087f4
	movs	r0, #0
	bl 0x02008884
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_0200047e
	ldr	r0, [pc, #28]
	bl 0x02008844
	b.n	.L_02000484
.L_0200047e:
	ldr	r0, [pc, #24]
	bl 0x02008844
.L_02000484:
	movs	r0, #18
	movs	r1, #0
	bl 0x0200885c
	bl 0x020087fc
	pop	{pc}
	.2byte 0x0000
	.4byte 0x00001a2b
	.2byte 0x19a8
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #76]
	adds	r0, r5, #0
	bl 0x02008844
	movs	r1, #0
	movs	r0, #11
	bl 0x0200884c
	bl 0x0200888c
	movs	r1, #0
	bl 0x02008804
	cmp	r0, #0
	bne.n	.L_020004ca
	movs	r0, #10
	bl 0x020087ec
	adds	r0, r5, #1
	bl 0x02008844
	b.n	.L_020004d6
.L_020004ca:
	movs	r0, #20
	bl 0x020087ec
	adds	r0, r5, #2
	bl 0x02008844
.L_020004d6:
	movs	r0, #11
	movs	r1, #0
	bl 0x0200885c
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #26
	bl 0x020087ac
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1a17
	.2byte 0x0000
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	ldr	r1, [pc, #88]
	ldr	r4, [pc, #92]
	cmp	r0, #0
	beq.n	.L_0200051c
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	cmp	r2, r4
	beq.n	.L_02000538
	ldr	r3, [pc, #76]
	cmp	r2, r3
	bne.n	.L_0200054a
	ldr	r0, [pc, #72]
	b.n	.L_02000556
.L_0200051c:
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	cmp	r2, r4
	bne.n	.L_02000540
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #99
	bne.n	.L_0200053c
.L_02000538:
	ldr	r0, [pc, #44]
	b.n	.L_02000556
.L_0200053c:
	ldr	r0, [pc, #44]
	b.n	.L_02000556
.L_02000540:
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_0200054a
	ldr	r0, [pc, #40]
	b.n	.L_02000556
.L_0200054a:
	ldr	r3, [pc, #40]
	cmp	r2, r3
	bne.n	.L_02000554
	ldr	r0, [pc, #36]
	b.n	.L_02000556
.L_02000554:
	ldr	r0, [pc, #36]
.L_02000556:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x00000040
	.4byte 0x00000041
	.4byte 0x02008fe8
	.4byte 0x02008e44
	.4byte 0x02008b80
	.4byte 0x02008d18
	.4byte 0x00000042
	.4byte 0x02008de4
	.2byte 0x8b5c
	.2byte 0x0200
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	ldr	r5, [pc, #360]
	adds	r2, #255
	str	r2, [r3, #0]
	adds	r2, #11
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200880c
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #0
	bl 0x0200887c
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #320]
	cmp	r2, r3
	beq.n	.L_020005c4
	b.n	.L_020006ce
.L_020005c4:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_020005e2
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #99
	bne.n	.L_02000600
.L_020005e2:
	movs	r0, #19
	movs	r1, #0
	movs	r2, #0
	bl 0x0200881c
	movs	r0, #20
	movs	r1, #0
	movs	r2, #0
	bl 0x0200881c
	movs	r0, #18
	movs	r1, #3
	bl 0x02008814
	b.n	.L_020006ac
.L_02000600:
	movs	r5, #128
	lsls	r5, r5, #6
	movs	r1, #153
	movs	r0, #17
	lsls	r1, r1, #17
	ldr	r2, [pc, #248]
	adds	r3, r5, #0
	bl 0x02008824
	movs	r3, #192
	movs	r2, #168
	lsls	r3, r3, #7
	lsls	r2, r2, #17
	ldr	r1, [pc, #236]
	movs	r0, #18
	bl 0x02008824
	movs	r0, #19
	bl 0x0200880c
	movs	r1, #0
	bl 0x020087c4
	movs	r0, #20
	bl 0x0200880c
	movs	r1, #0
	bl 0x020087c4
	movs	r0, #17
	bl 0x0200880c
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #17
	bl 0x0200880c
	str	r6, [r0, #12]
	movs	r0, #18
	bl 0x0200880c
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #18
	bl 0x0200880c
	str	r6, [r0, #12]
	movs	r0, #19
	bl 0x0200880c
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #19
	bl 0x0200880c
	str	r6, [r0, #12]
	movs	r0, #20
	bl 0x0200880c
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #20
	bl 0x0200880c
	movs	r1, #224
	lsls	r1, r1, #8
	str	r6, [r0, #12]
	movs	r0, #17
	bl 0x02008894
	movs	r0, #17
	bl 0x0200880c
	movs	r1, #0
	bl 0x020087c4
	adds	r1, r5, #0
	movs	r0, #18
	bl 0x02008894
	movs	r0, #18
	bl 0x0200880c
	movs	r1, #0
	bl 0x020087c4
.L_020006ac:
	ldr	r3, [pc, #76]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #99
	bne.n	.L_020006ce
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	bne.n	.L_020006ce
	bl 0x02008710
.L_020006ce:
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_020006e4
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x020087b4
.L_020006e4:
	movs	r0, #196
	lsls	r0, r0, #2
	bl 0x020087a4
	cmp	r0, #0
	beq.n	.L_020006f8
	movs	r0, #160
	lsls	r0, r0, #4
	bl 0x020087dc
.L_020006f8:
	movs	r0, #0
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x00000040
	.4byte 0x014f0000
	.2byte 0x0000
	.2byte 0x0159
	movs	r0, #0
	bx	lr
	push	{lr}
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #27
	bl 0x020087ac
	bl 0x020087f4
	movs	r0, #0
	bl 0x02008884
	movs	r0, #30
	bl 0x020087ec
	bl 0x020087cc
	movs	r0, #86
	bl 0x020088b4
	movs	r1, #0
	movs	r2, #0
	ldr	r0, [pc, #96]
	bl 0x020087e4
	movs	r0, #20
	bl 0x020087ec
	bl 0x020087d4
	bl 0x020087bc
	bl 0x0200886c
	bl 0x02008874
	ldr	r0, [pc, #72]
	bl 0x02008844
	movs	r2, #10
	movs	r0, #16
	movs	r1, #0
	bl 0x02008854
	movs	r1, #2
	movs	r0, #15
	bl 0x02008834
	movs	r0, #20
	bl 0x020087ec
	movs	r2, #10
	movs	r0, #15
	movs	r1, #0
	bl 0x02008854
	movs	r1, #3
	movs	r0, #15
	bl 0x0200882c
	movs	r0, #30
	bl 0x020087ec
	movs	r0, #15
	movs	r1, #0
	movs	r2, #10
	bl 0x02008854
	bl 0x020087fc
	pop	{pc}
	.4byte 0x000019f5
	.4byte 0x000019f6
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020121, 0x08020219, 0x08020241, 0x08020249, 0x080202a1, 0x08038211, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c80a1, 0x080c80f9, 0x080c8101, 0x080c8129, 0x080c8149, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c8199, 0x080c81a1, 0x080c81d1, 0x080c83a9, 0x080c83b9, 0x080c8481, 0x080c84e1, 0x080c8779, 0x080c87a9, 0x08108009, 0x08108011, 0x08108019, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000040
	.4byte 0x1010103b
	.4byte 0xffffffff
	.4byte 0x1030303b
	.4byte 0xffffffff
	.4byte 0x1040403b
	.4byte 0xffffffff
	.4byte 0x1050503b
	.4byte 0xffffffff
	.4byte 0x1060703e
	.4byte 0xffffffff
	.4byte 0x1070803e
	.4byte 0xffffffff
	.4byte 0x00000041
	.4byte 0x1030503e
	.4byte 0xffffffff
	.4byte 0x1040203b
	.4byte 0xffffffff
	.4byte 0x1050603e
	.4byte 0xffffffff
	.4byte 0x00000042
	.4byte 0x1020603b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c0
	.4byte 0x00000002
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00002000
	.4byte 0xffff00c2
	.4byte 0x00000001
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00014000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x00008000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0001e000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x014a0000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00016000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00026000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00b9
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x01960000
	.4byte 0x00012000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0000a000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x019c0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008319
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000198a
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000198b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000198c
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000198d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000198e
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001995
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001996
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001997
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001998
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200809d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000199a
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008115
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000199c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008231
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000019a2
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000019a5
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000019a6
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200839d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020083dd
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0200841d
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0200845d
	.4byte 0x00000173
	.4byte 0xffff00d1
	.4byte 0x0040304c
	.4byte 0x00000173
	.4byte 0xffff00d2
	.4byte 0x0040304d
	.4byte 0x00000173
	.4byte 0xffff00d4
	.4byte 0x0040304f
	.4byte 0x00000173
	.4byte 0xffff00d5
	.4byte 0x00403050
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000198f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001990
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001991
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001992
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001993
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001994
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200818d
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000019a0
	.4byte 0x00000173
	.4byte 0xffff00d3
	.4byte 0x0040304e
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008299
	.4byte 0x00008d15
	.4byte 0x091b0008
	.4byte 0x000019aa
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a2d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008299
	.4byte 0x00008d15
	.4byte 0x091b0009
	.4byte 0x000019aa
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a2d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a0b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a0c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a0d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a0e
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a0f
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a10
	.4byte 0x00000000
	.4byte 0x091a000b
	.4byte 0x0200849d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a1a
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a1b
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a1c
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a1d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200809d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a1f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008115
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001a21
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008231
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001a25
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a28
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001a29
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0200839d
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020083dd
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0200841d
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0200845d
	.4byte 0x00000173
	.4byte 0xffff00d1
	.4byte 0x0040304c
	.4byte 0x00000173
	.4byte 0xffff00d2
	.4byte 0x0040304d
	.4byte 0x00000173
	.4byte 0xffff00d4
	.4byte 0x0040304f
	.4byte 0x00000173
	.4byte 0xffff00d5
	.4byte 0x00403050
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a11
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a12
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a13
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a14
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a15
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a16
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200818d
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a23
	.4byte 0x00000173
	.4byte 0xffff00d3
	.4byte 0x0040304e
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
