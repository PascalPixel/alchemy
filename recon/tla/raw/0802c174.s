.syntax unified
	.thumb
	.global Func_0802c174
	.thumb_func
Func_0802c174:
	push {r5, lr}
	lsrs r3, r0, #31
	adds r3, r0, r3
	movs r2, #31
	asrs r3, r3, #1
	ands r3, r2
	ldr r2, .L_0802c1e8
	lsls r3, r3, #7
	adds r4, r3, r2
	ldr r5, .L_0802c1ec
	movs r3, #62
	ands r3, r0
	lsls r3, r3, #6
	adds r1, r3, r5
	movs r0, #0
.L_0802c192:
	ldrh r2, [r4]
	ldr r5, .L_0802c1f0
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
	adds r1, #2
	adds r4, #4
	cmp r0, #31
	bls .L_0802c192
	movs r2, #252
	movs r3, #248
	lsls r2, r2, #4
	lsls r3, r3, #4
	adds r1, r1, r2
	adds r4, r4, r3
	movs r0, #0
.L_0802c1c2:
	ldrh r2, [r4]
	ldr r5, .L_0802c1f0
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
	adds r1, #2
	adds r4, #4
	cmp r0, #31
	bls .L_0802c1c2
	pop {r5, pc}
	.2byte 0x0000
.L_0802c1e8:
	.4byte gMapBlocks
.L_0802c1ec:
	.4byte 0x06004000
.L_0802c1f0:
	.4byte gMapCellBuffer
