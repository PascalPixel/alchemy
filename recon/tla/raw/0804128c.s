.syntax unified
	.thumb
	.global Func_0804128c
	.thumb_func
Func_0804128c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	sub sp, #4
	mov r10, r3
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #0
	movs r3, #19
	movs r1, #0
	movs r2, #30
	bl UiWindow_Create
	movs r3, #7
	adds r6, r0, #0
	movs r5, #2
	mov r8, r3
.L_080412b8:
	adds r2, r5, #0
	movs r3, #29
	adds r0, r6, #0
	movs r1, #0
	str r5, [sp, #0]
	bl UiWindow_DrawDividerLine
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r3, r8
	adds r5, #2
	cmp r3, #0
	bge .L_080412b8
	ldr r5, .L_0804134c
	movs r3, #0
	movs r7, #0
	mov r8, r3
.L_080412dc:
	movs r3, #0
	ldrsh r0, [r5, r3]
	adds r1, r6, #0
	adds r3, r7, #0
	movs r2, #8
	bl Func_08041268
	movs r3, #18
	ldrsh r0, [r5, r3]
	adds r1, r6, #0
	adds r3, r7, #0
	movs r2, #64
	bl Func_08041268
	movs r3, #36
	ldrsh r0, [r5, r3]
	adds r1, r6, #0
	adds r3, r7, #0
	movs r2, #120
	bl Func_08041268
	movs r3, #54
	ldrsh r0, [r5, r3]
	adds r1, r6, #0
	adds r3, r7, #0
	movs r2, #176
	bl Func_08041268
	movs r3, #1
	add r8, r3
	mov r3, r8
	adds r7, #16
	adds r5, #2
	cmp r3, #8
	ble .L_080412dc
	bl Func_08044460
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	adds r2, r6, #0
	str r3, [sp, #0]
	bl RenderOutput_Create
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #164
	add r3, r10
	str r0, [r3]
	add sp, #4
	adds r0, r6, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804134c:
	.4byte Data_080aa170
