.syntax unified
	.thumb
	.global Menu_LoadSelectedResource
	.thumb_func
Menu_LoadSelectedResource:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #72]
	adds r0, r5, #0
	bl NodeChain_GetNodeAtCount
	adds r6, r0, #0
	ldrh r3, [r6, #10]
	cmp r3, #1
	beq .L_0803f6e6
	cmp r3, #6
	bne .L_0803f74c
.L_0803f6e6:
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlock
	movs r3, #195
	lsls r3, r3, #2
	adds r5, r5, r3
	ldrh r3, [r6, #8]
	adds r7, r0, #0
	ldr r0, .L_0803f754
	mov r8, r3
	bl Resource_GetTableEntry
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	adds r2, r7, r3
	ldrh r3, [r6, #8]
	adds r1, r7, #0
	lsls r3, r3, #1
	ldrh r3, [r3, r0]
	adds r0, r0, r3
	str r0, [r2]
	bl Resource_DecodeByteLzInRam
	ldrh r3, [r5, #10]
	cmp r3, #0
	bne .L_0803f726
	bl Resource_FindFreeEntry
	strh r0, [r5, #12]
.L_0803f726:
	movs r1, #128
	ldrh r0, [r5, #12]
	lsls r1, r1, #3
	adds r2, r7, #0
	bl VramBlock_LoadCached
	movs r3, #1
	strh r3, [r5, #10]
	mov r3, r8
	strh r3, [r5, #8]
	movs r3, #40
	strh r3, [r5, #34]
	strh r3, [r5, #36]
	movs r3, #240
	strh r0, [r5, #14]
	strh r3, [r5, #38]
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_0803f74c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803f754:
	.4byte 0x000001d7
