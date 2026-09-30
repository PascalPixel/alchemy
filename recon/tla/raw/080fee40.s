.syntax unified
	.thumb
	.global Func_080fee40
	.thumb_func
Func_080fee40:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	adds r7, r0, #0
	ldr r5, [r6, #40]
	sub sp, #8
	movs r0, #0
	cmp r5, #0
	bne .L_080fee70
	movs r3, #15
	adds r5, r6, #0
	adds r5, #40
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	adds r0, r5, #0
	movs r1, #0
	movs r2, #5
	movs r3, #30
	bl UiWindow_UpdateOrCreate
	ldr r5, [r5]
.L_080fee70:
	cmp r0, #0
	beq .L_080feeb2
	movs r3, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #0
	adds r3, r5, #0
	adds r0, r7, #0
	movs r1, #0
	bl Func_080380d8
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r6, r2
	str r0, [r3]
	adds r2, #172
	movs r3, #240
	strb r3, [r0, #15]
	adds r3, r6, r2
	ldrh r3, [r3]
	cmp r3, #3
	bne .L_080feea4
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080fa3d4
.L_080feea4:
	movs r2, #128
	lsls r2, r2, #1
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_080ff370
	b .L_080feebc
.L_080feeb2:
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #0
	bl Func_080ff370
.L_080feebc:
	add sp, #8
	pop {r5, r6, r7, pc}
