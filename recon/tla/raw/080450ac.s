.syntax unified
	.thumb
	.global Func_080450ac
	.thumb_func
Func_080450ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	mov r10, r3
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	movs r0, #0
	cmp r5, #96
	beq .L_080450f2
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	bl RenderResource_LoadFrame
	ldr r3, [sp, #28]
	movs r1, #128
	str r3, [sp, #0]
	mov r2, r8
	mov r3, r10
	lsls r1, r1, #24
	adds r0, r5, #0
	bl RenderOutput_Create
	ldrb r3, [r0, #21]
	movs r2, #32
	orrs r3, r2
	strb r3, [r0, #21]
	movs r3, #251
	strb r3, [r0, #15]
.L_080450f2:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
