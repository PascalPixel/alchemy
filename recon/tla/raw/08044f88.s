.syntax unified
	.thumb
	.global Func_08044f88
	.thumb_func
Func_08044f88:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	mov r9, r1
	adds r6, r2, #0
	mov r10, r3
	bl Resource_FindFreeEntry
	adds r7, r0, #0
	movs r0, #0
	cmp r7, #95
	bgt .L_08045008
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_08044f4c
	ldr r2, .L_08045000
	mov r3, r10
	mov r11, r2
	str r3, [sp, #0]
	mov r1, r11
	mov r2, r9
	adds r3, r6, #0
	adds r0, r7, #0
	bl RenderOutput_Create
	movs r5, #253
	mov r2, r10
	adds r3, r6, #0
	strb r5, [r0, #15]
	mov r8, r0
	adds r3, #32
	str r2, [sp, #0]
	mov r1, r11
	mov r2, r9
	adds r0, r7, #0
	bl RenderOutput_Create
	ldrh r1, [r0, #24]
	ldr r3, .L_08044ffc
	lsls r2, r1, #22
	lsrs r2, r2, #22
	adds r2, #8
	ands r2, r3
	ldr r3, .L_08045004
	strb r5, [r0, #15]
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #24]
	mov r0, r8
	b .L_08045008
	.2byte 0x0000
.L_08044ffc:
	.4byte 0x000003ff
.L_08045000:
	.4byte 0x80004000
.L_08045004:
	.4byte 0xfffffc00
.L_08045008:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
