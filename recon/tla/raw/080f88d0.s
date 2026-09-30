.syntax unified
	.thumb
	.global Func_080f88d0
	.thumb_func
Func_080f88d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	sub sp, #8
	movs r6, #0
	movs r3, #5
	adds r5, r7, #0
	str r6, [r7, #16]
	adds r5, #16
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	movs r3, #13
	bl UiWindow_UpdateOrCreate
	ldr r5, [r5]
	movs r3, #3
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r3, #12
	movs r1, #0
	movs r2, #3
	bl UiWindow_DrawDividerLineFar
	movs r1, #8
	negs r1, r1
	movs r2, #11
	adds r0, r5, #0
	bl UiIcon_CreateWithResourceVariant
	movs r2, #13
	mov r8, r2
	mov r3, r8
	strb r3, [r0, #5]
	movs r3, #255
	strb r3, [r7, #28]
	movs r3, #254
	str r0, [r7, #20]
	strb r3, [r0, #15]
	ldr r2, [r7, #24]
	subs r3, #255
	strb r6, [r7, #29]
	strb r3, [r2, #15]
	bl Resource_FindFreeEntry
	adds r6, r0, #0
	cmp r6, #95
	bgt .L_080f8960
	ldr r2, .L_080f899c
	movs r1, #128
	bl VramBlock_LoadResourceFar
	movs r3, #4
	negs r3, r3
	movs r1, #128
	str r3, [sp, #0]
	adds r2, r5, #0
	movs r3, #84
	lsls r1, r1, #23
	adds r0, r6, #0
	bl RenderOutput_CreateFar
	mov r2, r8
	strb r2, [r0, #5]
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	str r0, [r3]
.L_080f8960:
	bl Resource_FindFreeEntry
	adds r6, r0, #0
	cmp r6, #95
	bgt .L_080f8990
	ldr r2, .L_080f89a0
	movs r1, #128
	bl VramBlock_LoadResourceFar
	movs r3, #8
	movs r1, #128
	str r3, [sp, #0]
	adds r2, r5, #0
	movs r3, #84
	lsls r1, r1, #23
	adds r0, r6, #0
	bl RenderOutput_CreateFar
	movs r2, #190
	mov r3, r8
	lsls r2, r2, #1
	strb r3, [r0, #5]
	adds r3, r7, r2
	str r0, [r3]
.L_080f8990:
	adds r0, r5, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080f899c:
	.4byte 0x000001fe
.L_080f89a0:
	.4byte 0x000001ff
