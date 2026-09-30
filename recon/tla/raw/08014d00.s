.syntax unified
	.thumb
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	push {r5, lr}
	movs r4, #192
	adds r5, r0, #0
	lsls r4, r4, #18
	ldr r0, [r4, r5]
	cmp r0, #0
	bne .L_08014d38
	adds r3, r1, #3
	ldr r0, [r4]
	lsrs r3, r3, #2
	lsls r1, r3, #2
	movs r3, #129
	adds r2, r0, r1
	lsls r3, r3, #18
	cmp r2, r3
	bcc .L_08014d34
	ldr r0, [r4, #4]
	ldr r3, .L_08014d3c
	adds r1, r0, r1
	cmp r1, r3
	bls .L_08014d2e
	movs r0, #0
	b .L_08014d38
.L_08014d2e:
	str r1, [r4, #4]
	str r0, [r4, r5]
	b .L_08014d38
.L_08014d34:
	str r2, [r4]
	str r0, [r4, r5]
.L_08014d38:
	pop {r5, pc}
	.2byte 0x0000
.L_08014d3c:
	.4byte Data_03006fbf
