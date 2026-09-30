.syntax unified
	.thumb
	.global Func_080cb144
	.thumb_func
Func_080cb144:
	push {r5, r6, lr}
	movs r5, #192
	lsls r5, r5, #18
	sub sp, #12
	ldr r6, [r5, #108]
	bl Func_080cb8e8
	ldr r4, [r5, #32]
	cmp r0, #0
	bne .L_080cb15c
	movs r0, #0
	b .L_080cb1cc
.L_080cb15c:
	ldr r1, [r0, #8]
	mov r3, sp
	str r1, [r3]
	ldr r2, [r0, #12]
	str r2, [r3, #4]
	movs r2, #197
	ldr r0, [r0, #16]
	lsls r2, r2, #1
	str r0, [r3, #8]
	adds r3, r6, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080cb1a4
	adds r3, r1, #0
	cmp r1, #0
	bge .L_080cb184
	ldr r4, .L_080cb1d0
	adds r3, r1, r4
.L_080cb184:
	asrs r2, r3, #21
	movs r1, #31
	ands r2, r1
	adds r3, r0, #0
	cmp r0, #0
	bge .L_080cb194
	ldr r4, .L_080cb1d0
	adds r3, r0, r4
.L_080cb194:
	asrs r3, r3, #21
	ands r3, r1
	lsls r3, r3, #5
	ldr r1, .L_080cb1d4
	adds r3, r2, r3
	lsls r3, r3, #2
	adds r2, r3, r1
	b .L_080cb1ca
.L_080cb1a4:
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r4, r2
	ldr r2, [r3]
	adds r3, r1, #0
	cmp r3, #0
	bge .L_080cb1b6
	ldr r4, .L_080cb1d8
	adds r3, r3, r4
.L_080cb1b6:
	asrs r1, r3, #20
	cmp r0, #0
	bge .L_080cb1c0
	ldr r3, .L_080cb1d8
	adds r0, r0, r3
.L_080cb1c0:
	asrs r3, r0, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
.L_080cb1ca:
	ldrb r0, [r2, #2]
.L_080cb1cc:
	add sp, #12
	pop {r5, r6, pc}
.L_080cb1d0:
	.4byte 0x001fffff
.L_080cb1d4:
	.4byte gMapBlocks
.L_080cb1d8:
	.4byte 0x000fffff
