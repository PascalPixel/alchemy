.syntax unified
	.thumb
	.global Func_08101c40
	.thumb_func
Func_08101c40:
	push {r5, lr}
	adds r5, r0, #0
	adds r0, r2, #0
	negs r2, r3
	orrs r2, r3
	lsrs r4, r2, #31
	movs r3, #15
	subs r4, r3, r4
	sub sp, #8
	adds r3, r1, #0
	cmp r1, #0
	bge .L_08101c5a
	adds r3, r1, #3
.L_08101c5a:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r1, r3
	lsls r1, r3, #3
	subs r1, r1, r3
	movs r3, #1
	adds r2, r0, #2
	str r3, [sp, #0]
	adds r1, #1
	adds r0, r5, #0
	movs r3, #6
	str r4, [sp, #4]
	bl Func_08101c18
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
