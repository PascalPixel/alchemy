.syntax unified
	.thumb
	.global Func_080ff1f4
	.thumb_func
Func_080ff1f4:
	push {r5, r6, r7, lr}
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #8
	mov lr, r3
	cmp r0, #0
	bne .L_080ff23c
	lsls r3, r1, #1
	adds r5, r3, #5
	ldrb r3, [r2]
	movs r7, #0
	movs r6, #5
	movs r4, #0
	movs r0, #0
	cmp r3, #0
	beq .L_080ff226
	cmp r1, #0
	bne .L_080ff224
	ldr r3, .L_080ff278
	ldrb r6, [r3]
	b .L_080ff24e
.L_080ff224:
	adds r4, #1
.L_080ff226:
	adds r0, #1
	cmp r0, #4
	bgt .L_080ff24e
	ldrb r3, [r2, r0]
	cmp r3, #0
	beq .L_080ff226
	cmp r1, r4
	bne .L_080ff224
	ldr r3, .L_080ff278
	ldrb r6, [r3, r0]
	b .L_080ff24e
.L_080ff23c:
	cmp r1, #3
	bgt .L_080ff248
	adds r5, r1, #0
	movs r7, #5
	movs r6, #13
	b .L_080ff24e
.L_080ff248:
	adds r5, r1, #4
	movs r7, #8
	movs r6, #20
.L_080ff24e:
	movs r1, #1
	mov r2, r12
	adds r3, r1, #0
	eors r3, r2
	negs r2, r3
	orrs r2, r3
	lsrs r2, r2, #31
	movs r3, #15
	subs r3, r3, r2
	mov r2, lr
	ldr r0, [r2, #40]
	str r1, [sp, #0]
	str r3, [sp, #4]
	adds r1, r7, #0
	adds r2, r5, #0
	adds r3, r6, #0
	bl Render_SetTilemapFlagRect
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ff278:
	.4byte CharacterMenu_CursorWidths
