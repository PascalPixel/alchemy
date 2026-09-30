.syntax unified
	.thumb
	.global Func_0802c1f4
	.thumb_func
Func_0802c1f4:
	push {r5, lr}
	lsrs r3, r0, #31
	adds r3, r0, r3
	movs r2, #31
	asrs r3, r3, #1
	ands r3, r2
	ldr r2, .L_0802c234
	lsls r3, r3, #2
	ldr r5, .L_0802c238
	adds r4, r3, r2
	movs r3, #62
	ands r3, r0
	adds r1, r3, r5
	movs r0, #0
.L_0802c210:
	ldrh r2, [r4]
	ldr r5, .L_0802c23c
	lsls r2, r2, #2
	adds r3, r2, r5
	ldrh r3, [r3]
	adds r5, #2
	strh r3, [r1]
	adds r3, r2, r5
	ldrh r3, [r3]
	adds r2, r1, #0
	adds r2, #64
	adds r0, #1
	strh r3, [r2]
	adds r1, #128
	adds r4, #128
	cmp r0, #63
	bls .L_0802c210
	pop {r5, pc}
.L_0802c234:
	.4byte gMapBlocks
.L_0802c238:
	.4byte 0x06004000
.L_0802c23c:
	.4byte gMapCellBuffer
