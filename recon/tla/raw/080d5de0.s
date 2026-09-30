.syntax unified
	.thumb
	.global ObjectEffect_PrepareContextEffect
	.thumb_func
ObjectEffect_PrepareContextEffect:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_080d5e48
	mov r8, r0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl ObjectTable_Get
	adds r5, r0, #0
	ldr r6, [r5, #80]
	movs r1, #12
	adds r1, #255
	adds r0, r6, #0
	bl ResourceMetadata_RegisterFar
	movs r1, #0
	strb r1, [r6, #26]
	movs r3, #15
	strb r3, [r0, #5]
	ldr r2, .L_080d5e4c
	ldr r3, [r5, #8]
	movs r0, #128
	ands r3, r2
	lsls r0, r0, #12
	adds r3, r3, r0
	str r3, [r5, #8]
	ldr r3, [r5, #16]
	str r1, [r5, #36]
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #24
	str r1, [r5, #44]
	adds r0, r5, #0
	str r3, [r5, #56]
	str r3, [r5, #64]
	mov r1, r8
	bl Object_SetMode
	movs r0, #18
	bl WaitFrames
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080d5e48:
	.4byte gPartyState
.L_080d5e4c:
	.4byte 0xfff00000
