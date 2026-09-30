.syntax unified
	.thumb
	.global Func_080cc54c
	.thumb_func
Func_080cc54c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #0
	sub sp, #12
	mov r8, r0
	bl Func_080cb8e8
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	adds r7, r0, #0
	mov r10, r1
	ldr r6, [r3, #32]
	cmp r7, #0
	beq .L_080cc65e
	ldr r3, [r7, #8]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r0, #128
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	lsls r0, r0, #13
	str r3, [r5, #8]
	adds r2, r5, #0
	ldrh r1, [r7, #6]
	bl Func_0801489c
	movs r3, #197
	lsls r3, r3, #1
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080cc5c4
	ldr r3, [r5]
	ldr r1, [r5, #8]
	cmp r3, #0
	bge .L_080cc5a4
	ldr r2, .L_080cc66c
	adds r3, r3, r2
.L_080cc5a4:
	asrs r2, r3, #21
	movs r0, #31
	adds r3, r1, #0
	ands r2, r0
	cmp r3, #0
	bge .L_080cc5b4
	ldr r1, .L_080cc66c
	adds r3, r3, r1
.L_080cc5b4:
	asrs r3, r3, #21
	ands r3, r0
	lsls r3, r3, #5
	adds r3, r2, r3
	ldr r2, .L_080cc670
	lsls r3, r3, #2
	adds r1, r3, r2
	b .L_080cc5ee
.L_080cc5c4:
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r6, r0
	ldr r1, [r3]
	ldr r3, [r5]
	ldr r2, [r5, #8]
	cmp r3, #0
	bge .L_080cc5d8
	ldr r0, .L_080cc674
	adds r3, r3, r0
.L_080cc5d8:
	asrs r0, r3, #20
	adds r3, r2, #0
	cmp r3, #0
	bge .L_080cc5e4
	ldr r2, .L_080cc674
	adds r3, r3, r2
.L_080cc5e4:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r0, r3
	lsls r3, r3, #2
	adds r1, r1, r3
.L_080cc5ee:
	ldrb r6, [r1, #2]
	adds r3, r6, #0
	subs r3, #242
	cmp r3, #5
	bhi .L_080cc61a
	adds r3, r7, #0
	adds r3, #34
	ldr r2, [r5, #8]
	ldrb r0, [r3]
	ldr r1, [r5]
	bl Func_080201c0
	adds r2, r0, #0
	ldr r0, [r7, #12]
	cmp r2, r0
	blt .L_080cc628
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r0, r1
	cmp r2, r3
	bgt .L_080cc628
	b .L_080cc626
.L_080cc61a:
	movs r0, #3
	adds r1, r6, #0
	bl Func_080ccd78
	cmp r0, #0
	beq .L_080cc628
.L_080cc626:
	mov r8, r6
.L_080cc628:
	mov r2, r8
	cmp r2, #0
	bne .L_080cc65e
	movs r3, #197
	lsls r3, r3, #1
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080cc65e
	ldr r3, .L_080cc678
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #118
	adds r3, r3, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080cc65e
	adds r0, r5, #0
	bl Func_08020358
	cmp r0, #3
	bne .L_080cc65e
	movs r2, #99
	mov r8, r2
.L_080cc65e:
	mov r0, r8
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cc66c:
	.4byte 0x001fffff
.L_080cc670:
	.4byte gMapBlocks
.L_080cc674:
	.4byte 0x000fffff
.L_080cc678:
	.4byte gPartyState
