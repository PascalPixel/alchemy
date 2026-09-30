.syntax unified
	.thumb
	.global Map_RenderAnimatedTileFrame
	.thumb_func
Map_RenderAnimatedTileFrame:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldrb r3, [r0, #16]
	ldr r2, .L_08022fdc
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	ldr r2, .L_08022fe0
	adds r5, r3, r2
	ldrb r2, [r0, #20]
	ldrb r3, [r0, #21]
	muls r3, r2
	cmp r3, #0
	bge .L_08022f84
	adds r3, #63
.L_08022f84:
	asrs r6, r3, #6
	movs r4, #0
	cmp r4, r6
	bcs .L_08022fd4
	ldr r3, .L_08022fe4
	movs r0, #255
	lsls r0, r0, #8
	movs r2, #63
	mov r8, r3
	mov lr, r0
	mov r12, r2
	movs r7, #62
.L_08022f9c:
	adds r3, r1, #0
	subs r3, #64
	cmp r3, #63
	bhi .L_08022fca
	lsls r3, r4, #4
	mov r0, r12
	adds r3, r1, r3
	ands r3, r0
	mov r0, r8
	ldrb r2, [r0, r3]
	adds r3, r2, #0
	ands r3, r7
	adds r0, r5, r3
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08022fc2
	ldrb r3, [r0]
	b .L_08022fc8
.L_08022fc2:
	ldrh r2, [r0]
	mov r3, lr
	ands r3, r2
.L_08022fc8:
	strh r3, [r0]
.L_08022fca:
	adds r4, #1
	adds r5, #64
	adds r1, #1
	cmp r4, r6
	bcc .L_08022f9c
.L_08022fd4:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08022fdc:
	.4byte ResourceTableEntries
.L_08022fe0:
	.4byte 0x06010000
.L_08022fe4:
	.4byte Map_TileDissolveOrder
