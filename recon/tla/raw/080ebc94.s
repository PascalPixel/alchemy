.syntax unified
	.thumb
	.global BattleFx_DrawScaledObject
	.thumb_func
BattleFx_DrawScaledObject:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r3, #0
	mov r8, r3
	adds r3, r6, #0
	adds r3, #71
	ldrb r2, [r3]
	movs r3, #4
	ands r3, r2
	sub sp, #24
	ldr r7, [r6]
	cmp r3, #0
	beq .L_080ebcbc
	ldr r2, [r6, #8]
	movs r3, #254
	lsls r3, r3, #17
	subs r3, r3, r2
	mov r8, r3
.L_080ebcbc:
	ldr r5, .L_080ebd14
	ldr r1, [r7, #12]
	ldr r0, [r6, #40]
	mov lr, r5
	.2byte 0xf800
	str r0, [sp, #0]
	ldr r1, [r7, #12]
	ldr r0, [r6, #44]
	mov lr, r5
	.2byte 0xf800
	ldr r2, [r6, #4]
	add r1, sp, #8
	mov r5, r8
	mov r4, sp
	str r0, [r4, #4]
	str r2, [r1]
	str r5, [r1, #4]
	ldr r0, [r6, #8]
	adds r3, r0, r5
	str r3, [r1, #8]
	movs r3, #0
	str r3, [r1, #12]
	ldr r3, .L_080ebd18
	ldr r5, .L_080ebd1c
	adds r2, r2, r3
	cmp r2, r5
	bhi .L_080ebd0a
	ldr r3, .L_080ebd20
	cmp r0, r3
	ble .L_080ebd0a
	movs r5, #224
	lsls r5, r5, #16
	cmp r0, r5
	bge .L_080ebd0a
	adds r0, r7, #0
	adds r2, r4, #0
	movs r3, #0
	bl Render_ApplyProjectedPlacementFar
.L_080ebd0a:
	add sp, #24
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ebd14:
	.4byte IwramMulQ16
.L_080ebd18:
	.4byte 0x001fffff
.L_080ebd1c:
	.4byte 0x012ffffe
.L_080ebd20:
	.4byte 0xffe00000
