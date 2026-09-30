.syntax unified
	.thumb
	.global Func_08022bd8
	.thumb_func
Func_08022bd8:
	push {r5, r6, r7, lr}
	sub sp, #4
	cmp r0, #3
	bne .L_08022bf8
	movs r1, #224
	lsls r1, r1, #4
	movs r0, #16
	bl Runtime_AllocateBlock
	movs r1, #192
	adds r7, r0, #0
	lsls r1, r1, #3
	movs r0, #12
	bl Runtime_AllocateBlock
	b .L_08022c0e
.L_08022bf8:
	movs r1, #224
	lsls r1, r1, #4
	movs r0, #16
	bl Runtime_AllocateHeapBlock
	movs r1, #192
	adds r7, r0, #0
	lsls r1, r1, #3
	movs r0, #12
	bl Runtime_AllocateHeapBlock
.L_08022c0e:
	adds r6, r0, #0
	bl Func_08014c4c
	movs r3, #128
	mov r4, sp
	movs r5, #0
	lsls r3, r3, #19
	str r5, [r4]
	adds r3, #212
	adds r0, r4, #0
	adds r1, r7, #0
	ldr r2, .L_08022c64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	str r5, [r4]
	adds r0, r4, #0
	adds r1, r6, #0
	ldr r2, .L_08022c68
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_08022c6c
	movs r1, #128
	movs r0, #93
	bl VramBlock_LoadCached
	ldr r5, .L_08022c70
	movs r0, #84
	adds r1, r5, #0
	bl Runtime_AllocateHeapBlock
	movs r2, #132
	movs r3, #128
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r1, r0, #0
	adds r3, #212
	ldr r0, .L_08022c74
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	add sp, #4
	pop {r5, r6, r7, pc}
.L_08022c64:
	.4byte 0x85000380
.L_08022c68:
	.4byte 0x85000180
.L_08022c6c:
	.4byte Data_0802e89c
.L_08022c70:
	.4byte 0x00000080
.L_08022c74:
	.4byte Render_CopyStridedCode
