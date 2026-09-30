.syntax unified
	.thumb
	.global Runtime_AllocateHeapBlock
	.thumb_func
Runtime_AllocateHeapBlock:
	push {r5, lr}
	movs r4, #192
	adds r5, r0, #0
	lsls r4, r4, #18
	ldr r0, [r4, r5]
	cmp r0, #0
	bne .L_08014cf8
	adds r3, r1, #3
	lsrs r3, r3, #2
	ldr r0, [r4, #4]
	lsls r1, r3, #2
	ldr r3, .L_08014cfc
	adds r2, r0, r1
	cmp r2, r3
	bls .L_08014cf4
	ldr r0, [r4]
	movs r3, #129
	adds r1, r0, r1
	lsls r3, r3, #18
	cmp r1, r3
	bcc .L_08014cee
	movs r0, #0
	b .L_08014cf8
.L_08014cee:
	str r1, [r4]
	str r0, [r4, r5]
	b .L_08014cf8
.L_08014cf4:
	str r2, [r4, #4]
	str r0, [r4, r5]
.L_08014cf8:
	pop {r5, pc}
	.2byte 0x0000
.L_08014cfc:
	.4byte Data_03006fbf
