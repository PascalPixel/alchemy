.syntax unified
	.thumb
	.global Func_080ed788
	.thumb_func
Func_080ed788:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080ed7f8
	bl Func_080d2260
	movs r0, #157
	lsls r0, r0, #8
	movs r1, #6
	adds r0, #137
	bl Func_080d46a4
	ldr r1, .L_080ed7fc
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080ed7d0
	adds r6, r1, #0
	adds r5, r2, #0
.L_080ed7c2:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6]
	ands r3, r5
	cmp r3, #0
	bne .L_080ed7c2
.L_080ed7d0:
	ldr r3, .L_080ed800
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080ed7ee
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #6
	bl Func_080d46a4
	b .L_080ed7f8
.L_080ed7ee:
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #6
	bl Func_080d46a4
.L_080ed7f8:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080ed7fc:
	.4byte gInput
.L_080ed800:
	.4byte gPartyState
