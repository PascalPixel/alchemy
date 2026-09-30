.syntax unified
	.thumb
	.global UiText_LoadRemappedGlyph
	.thumb_func
UiText_LoadRemappedGlyph:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r1, #0
	movs r1, #193
	lsls r1, r1, #3
	adds r7, r0, #0
	movs r0, #68
	mov r8, r2
	bl Runtime_AllocateHeapBlock
	adds r6, r0, #0
	ldr r0, .L_080456e8
	bl Resource_GetTableEntry
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	lsls r5, r5, #1
	adds r2, r6, r3
	ldrh r3, [r5, r0]
	adds r1, r6, #0
	adds r0, r0, r3
	str r0, [r2]
	bl Func_0801591c
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocate
	movs r2, #0
	mov lr, r0
	mov r5, lr
	mov r12, r2
.L_08045672:
	ldrb r4, [r6]
	adds r6, #1
	ldrb r2, [r7, r4]
	adds r3, r2, #0
	cmp r3, #255
	bne .L_080456ae
	movs r3, #128
	lsls r3, r3, #1
	adds r0, r7, r3
	ldr r3, [r0]
	strb r3, [r7, r4]
	ldr r1, [r0]
	cmp r1, #63
	bgt .L_080456ac
	movs r3, #160
	lsls r2, r1, #1
	lsls r3, r3, #19
	adds r3, r3, r2
	ldr r2, .L_080456ec
	mov r10, r3
	lsls r3, r4, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	mov r2, r10
	strh r3, [r2]
	adds r3, r1, #1
	str r3, [r0]
	ldrb r2, [r7, r4]
	b .L_080456ae
.L_080456ac:
	adds r2, r3, #0
.L_080456ae:
	strb r2, [r5]
	movs r3, #1
	movs r2, #128
	add r12, r3
	lsls r2, r2, #3
	adds r5, #1
	cmp r12, r2
	blt .L_08045672
	mov r3, r8
	ldr r2, .L_080456f0
	lsls r1, r3, #6
	movs r3, #128
	lsls r3, r3, #19
	adds r1, r1, r2
	adds r3, #212
	mov r0, lr
	ldr r2, .L_080456f4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, lr
	bl Sys_Free
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080456e8:
	.4byte 0x000001d7
.L_080456ec:
	.4byte 0x05000200
.L_080456f0:
	.4byte 0x06004000
.L_080456f4:
	.4byte 0x84000100
