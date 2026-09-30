.syntax unified
	.thumb
	.global Func_080fd52c
	.thumb_func
Func_080fd52c:
	push {lr}
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	lsls r0, r0, #10
	ands r3, r1
	orrs r0, r3
	cmp r2, #0
	bne .L_080fd546
	ldr r3, .L_080fd558
	movs r2, #144
	lsls r2, r2, #2
	b .L_080fd54e
.L_080fd546:
	ldr r3, .L_080fd558
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #66
.L_080fd54e:
	adds r3, r3, r2
	strh r0, [r3]
	movs r0, #1
	pop {pc}
	.2byte 0x0000
.L_080fd558:
	.4byte gPartyState
