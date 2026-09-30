.syntax unified
	.thumb
	.global Func_080ff27c
	.thumb_func
Func_080ff27c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	mov r9, r1
	mov r10, r2
	movs r7, #0
	movs r6, #0
.L_080ff290:
	mov r2, r10
	ldrb r3, [r2, r6]
	cmp r3, #0
	beq .L_080ff2c0
	cmp r9, r7
	bne .L_080ff2be
	ldr r3, .L_080ff2e0
	lsls r5, r6, #1
	adds r5, r5, r3
	movs r3, #1
	adds r0, r5, #0
	mov r1, r8
	movs r2, #0
	negs r3, r3
	adds r5, #1
	bl UiText_DrawResourceFar
	adds r0, r5, #0
	mov r1, r8
	movs r2, #0
	movs r3, #15
	bl UiText_DrawResourceFar
.L_080ff2be:
	adds r7, #1
.L_080ff2c0:
	adds r6, #1
	cmp r6, #4
	ble .L_080ff290
	cmp r7, #0
	bne .L_080ff2d6
	ldr r0, .L_080ff2e4
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawResourceFar
.L_080ff2d6:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080ff2e0:
	.4byte 0x0000110d
.L_080ff2e4:
	.4byte 0x0000110b
