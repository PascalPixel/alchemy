.syntax unified
	.thumb
	.global Map_WriteLayerCellTile
	.thumb_func
Map_WriteLayerCellTile:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r3, #0
	ldr r3, .L_080109cc
	mov r10, r1
	mov r8, r2
	movs r1, #136
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r3, r2, r1
	ldr r6, [r3]
	mov r1, r10
	movs r3, #1
	ands r1, r3
	mov r10, r1
	lsls r0, r0, #1
	mov r1, r8
	ands r1, r3
	mov r9, r0
	mov r8, r1
	mov r3, r9
	add r3, r8
	lsls r3, r3, #1
	add r3, r10
	lsls r3, r3, #1
	adds r2, r2, r3
	ldr r1, [sp, #28]
	movs r3, #206
	lsls r3, r3, #2
	adds r2, r2, r3
	cmp r1, #0
	bne .L_08010932
	ldrh r3, [r2]
	movs r0, #0
	cmp r5, r3
	beq .L_080109be
.L_08010932:
	movs r1, #128
	strh r5, [r2]
	lsls r1, r1, #3
	movs r0, #14
	bl Runtime_AllocateHeapBlock
	lsls r3, r5, #2
	adds r7, r0, #0
	ldr r0, [r3, r6]
	adds r1, r7, #0
	adds r0, r6, r0
	bl Resource_DecodeByteLz
	mov r3, r9
	add r3, r8
	lsls r3, r3, #5
	add r3, r10
	ldr r2, .L_080109d0
	lsls r3, r3, #6
	adds r4, r7, #0
	adds r5, r3, r2
	movs r6, #0
.L_0801095e:
	ldr r3, .L_080109d4
	adds r0, r4, #0
	adds r1, r5, #0
	ldr r2, .L_080109d8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r6, #1
	adds r4, #64
	adds r5, #128
	cmp r6, #15
	bls .L_0801095e
	ldr r3, [sp, #28]
	cmp r3, #0
	beq .L_080109b6
	mov r3, r9
	add r3, r8
	lsls r3, r3, #6
	add r3, r10
	ldr r2, .L_080109dc
	lsls r3, r3, #5
	ldr r5, .L_080109e0
	adds r1, r3, r2
	adds r4, r7, #0
	movs r6, #0
.L_0801098e:
	movs r0, #0
.L_08010990:
	ldrh r3, [r4]
	lsls r3, r3, #2
	ldrh r2, [r3, r5]
	strh r2, [r1]
	ldr r2, .L_080109e4
	adds r3, r3, r2
	ldrh r3, [r3]
	adds r2, r1, #0
	adds r2, #64
	adds r0, #1
	strh r3, [r2]
	adds r1, #2
	adds r4, #4
	cmp r0, #15
	bls .L_08010990
	adds r6, #1
	adds r1, #96
	cmp r6, #15
	bls .L_0801098e
.L_080109b6:
	movs r0, #14
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
.L_080109be:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080109cc:
	.4byte gMapWork
.L_080109d0:
	.4byte gMapBlocks
.L_080109d4:
	.4byte 0x040000d4
.L_080109d8:
	.4byte 0x84000010
.L_080109dc:
	.4byte 0x06004000
.L_080109e0:
	.4byte gMapCellBuffer
.L_080109e4:
	.4byte Data_02010002
