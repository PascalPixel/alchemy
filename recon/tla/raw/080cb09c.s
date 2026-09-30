.syntax unified
	.thumb
	.global Func_080cb09c
	.thumb_func
Func_080cb09c:
	push {r5, r6, r7, lr}
	movs r5, #192
	lsls r5, r5, #18
	sub sp, #12
	ldr r6, [r5, #108]
	bl Func_080cb8e8
	adds r2, r0, #0
	ldr r7, [r5, #32]
	movs r0, #0
	cmp r2, #0
	beq .L_080cb132
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	lsls r0, r0, #13
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	ldrh r1, [r2, #6]
	adds r2, r5, #0
	bl Func_0801489c
	movs r0, #197
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080cb108
	ldr r2, [r5]
	ldr r3, [r5, #8]
	cmp r2, #0
	bge .L_080cb0ea
	ldr r1, .L_080cb138
	adds r2, r2, r1
.L_080cb0ea:
	asrs r2, r2, #21
	movs r1, #31
	ands r2, r1
	cmp r3, #0
	bge .L_080cb0f8
	ldr r0, .L_080cb138
	adds r3, r3, r0
.L_080cb0f8:
	asrs r3, r3, #21
	ands r3, r1
	lsls r3, r3, #5
	adds r3, r2, r3
	ldr r2, .L_080cb13c
	lsls r3, r3, #2
	adds r1, r3, r2
	b .L_080cb130
.L_080cb108:
	movs r0, #156
	ldr r2, [r5]
	lsls r0, r0, #1
	adds r3, r7, r0
	ldr r1, [r3]
	ldr r3, [r5, #8]
	cmp r2, #0
	bge .L_080cb11c
	ldr r0, .L_080cb140
	adds r2, r2, r0
.L_080cb11c:
	asrs r2, r2, #20
	cmp r3, #0
	bge .L_080cb126
	ldr r0, .L_080cb140
	adds r3, r3, r0
.L_080cb126:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r2, r3
	lsls r3, r3, #2
	adds r1, r1, r3
.L_080cb130:
	ldrb r0, [r1, #2]
.L_080cb132:
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cb138:
	.4byte 0x001fffff
.L_080cb13c:
	.4byte gMapBlocks
.L_080cb140:
	.4byte 0x000fffff
