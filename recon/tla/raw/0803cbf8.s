.syntax unified
	.thumb
	.global Func_0803cbf8
	.thumb_func
Func_0803cbf8:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r3, [r5]
	ldrh r3, [r3, #18]
	cmp r3, #4
	bne .L_0803cc9e
	ldrh r2, [r5, #20]
	adds r3, r2, #0
	cmp r3, #10
	bne .L_0803cc16
	movs r1, #192
	lsls r1, r1, #2
	bl Func_0803cba8
	b .L_0803cc54
.L_0803cc16:
	cmp r3, #9
	bne .L_0803cc26
	movs r1, #128
	lsls r1, r1, #2
	adds r0, r5, #0
	bl Func_0803cba8
	b .L_0803cc54
.L_0803cc26:
	cmp r3, #8
	bne .L_0803cc36
	movs r1, #128
	lsls r1, r1, #1
	adds r0, r5, #0
	bl Func_0803cba8
	b .L_0803cc54
.L_0803cc36:
	cmp r3, #6
	bne .L_0803cc46
	movs r1, #128
	lsls r1, r1, #2
	adds r0, r5, #0
	bl Func_0803cba8
	b .L_0803cc54
.L_0803cc46:
	cmp r3, #4
	bne .L_0803cc56
	movs r1, #192
	lsls r1, r1, #2
	adds r0, r5, #0
	bl Func_0803cba8
.L_0803cc54:
	ldrh r2, [r5, #20]
.L_0803cc56:
	adds r3, r2, #0
	cmp r3, #2
	bne .L_0803cc70
	ldr r3, [r5]
	movs r1, #12
	ldrsh r0, [r3, r1]
	movs r2, #14
	ldrsh r1, [r3, r2]
	ldrh r2, [r3, #8]
	ldrh r3, [r3, #10]
	bl Func_0803a084
	ldrh r2, [r5, #20]
.L_0803cc70:
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r5, #20]
	lsls r3, r3, #16
	lsrs r6, r3, #16
	cmp r6, #0
	bne .L_0803cc9e
	ldr r0, .L_0803cca0
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_0803cca4
	adds r2, #64
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r5]
	strh r6, [r3, #18]
.L_0803cc9e:
	pop {r5, r6, pc}
.L_0803cca0:
	.4byte 0x000001d5
.L_0803cca4:
	.4byte 0x06000100
