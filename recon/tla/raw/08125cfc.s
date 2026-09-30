.syntax unified
	.thumb
	.global Func_08125cfc
	.thumb_func
Func_08125cfc:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #168
	ldr r3, [r3]
	ldr r0, [r3]
	cmp r0, #79
	bhi .L_08125d62
	movs r2, #240
	movs r3, #7
	lsls r2, r2, #8
	ands r3, r0
	adds r2, #129
	adds r4, r3, r2
	adds r3, r0, #0
	cmp r0, #0
	bge .L_08125d20
	adds r3, r0, #7
.L_08125d20:
	asrs r3, r3, #3
	movs r2, #13
	ldr r5, .L_08125d64
	subs r2, r2, r3
	lsls r3, r2, #6
	movs r1, #0
	adds r2, r3, r5
.L_08125d2e:
	adds r1, #1
	strh r4, [r2]
	adds r2, #2
	cmp r1, #32
	bne .L_08125d2e
	movs r3, #128
	lsls r3, r3, #4
	orrs r4, r3
	adds r3, r0, #0
	cmp r3, #0
	bge .L_08125d46
	adds r3, #7
.L_08125d46:
	asrs r3, r3, #3
	adds r2, r3, #0
	adds r2, #13
	cmp r2, #20
	bhi .L_08125d62
	ldr r0, .L_08125d64
	lsls r3, r2, #6
	movs r1, #0
	adds r2, r3, r0
.L_08125d58:
	adds r1, #1
	strh r4, [r2]
	adds r2, #2
	cmp r1, #32
	bne .L_08125d58
.L_08125d62:
	pop {r5, pc}
.L_08125d64:
	.4byte 0x06006000
