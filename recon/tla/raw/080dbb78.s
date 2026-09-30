.syntax unified
	.thumb
	.global Func_080dbb78
	.thumb_func
Func_080dbb78:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r4, r2, #0
	ldr r2, [r3, #108]
	movs r6, #197
	lsls r6, r6, #1
	ldr r5, [r3, #32]
	adds r3, r2, r6
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080dbbbe
	adds r3, r0, #0
	cmp r0, #0
	bge .L_080dbb9e
	ldr r2, .L_080dbbf4
	adds r3, r0, r2
.L_080dbb9e:
	asrs r2, r3, #21
	movs r0, #31
	ands r2, r0
	adds r3, r1, #0
	cmp r1, #0
	bge .L_080dbbae
	ldr r4, .L_080dbbf4
	adds r3, r1, r4
.L_080dbbae:
	asrs r3, r3, #21
	ands r3, r0
	lsls r3, r3, #5
	ldr r6, .L_080dbbf8
	adds r3, r2, r3
	lsls r3, r3, #2
	adds r2, r3, r6
	b .L_080dbbf0
.L_080dbbbe:
	cmp r4, #2
	bgt .L_080dbbd2
	lsls r3, r4, #3
	subs r3, r3, r4
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r5, r3]
	b .L_080dbbd4
.L_080dbbd2:
	ldr r2, .L_080dbbfc
.L_080dbbd4:
	cmp r0, #0
	bge .L_080dbbdc
	ldr r3, .L_080dbc00
	adds r0, r0, r3
.L_080dbbdc:
	asrs r0, r0, #20
	cmp r1, #0
	bge .L_080dbbe6
	ldr r4, .L_080dbc00
	adds r1, r1, r4
.L_080dbbe6:
	asrs r3, r1, #20
	lsls r3, r3, #7
	adds r3, r0, r3
	lsls r3, r3, #2
	adds r2, r2, r3
.L_080dbbf0:
	adds r0, r2, #0
	pop {r5, r6, pc}
.L_080dbbf4:
	.4byte 0x001fffff
.L_080dbbf8:
	.4byte gMapBlocks
.L_080dbbfc:
	.4byte gMapCellBuffer
.L_080dbc00:
	.4byte 0x000fffff
