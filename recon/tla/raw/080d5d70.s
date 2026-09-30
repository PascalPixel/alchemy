.syntax unified
	.thumb
	.global Func_080d5d70
	.thumb_func
Func_080d5d70:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r1, #0
	bl ObjectTable_Get
	movs r3, #1
	adds r5, r0, #0
	eors r3, r6
	ldr r0, [r5, #80]
	negs r2, r3
	orrs r2, r3
	movs r1, #12
	lsrs r2, r2, #31
	adds r1, #255
	movs r6, #26
	subs r6, r6, r2
	mov r8, r0
	bl ResourceMetadata_RegisterFar
	movs r1, #0
	mov r2, r8
	strb r1, [r2, #26]
	movs r3, #15
	strb r3, [r0, #5]
	ldr r2, .L_080d5ddc
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
	adds r1, r6, #0
	bl Object_SetMode
	movs r0, #18
	bl WaitFrames
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_080d5ddc:
	.4byte 0xfff00000
