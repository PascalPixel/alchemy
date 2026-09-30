.syntax unified
	.thumb
	.global Func_0810b04c
	.thumb_func
Func_0810b04c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #129
	lsls r2, r2, #3
	adds r2, #255
	adds r3, r3, r2
	movs r5, #0
	ldrsb r5, [r3, r5]
	adds r7, r1, #0
	adds r6, r0, #0
	adds r1, r5, #0
	adds r0, r7, #0
	bl Shop_ServicePrice
	mov r8, r0
	cmp r6, #0
	beq .L_0810b0ac
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedrawFar
	adds r0, r7, #0
	adds r1, r5, #0
	bl Shop_CanServe
	cmp r0, #0
	beq .L_0810b08e
	ldr r5, .L_0810b0b4
	b .L_0810b090
.L_0810b08e:
	ldr r5, .L_0810b0b8
.L_0810b090:
	adds r0, r5, #0
	bl Func_0810a960
	movs r1, #5
	adds r5, r0, #0
	mov r0, r8
	bl UiText_DrawQuantity
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawResourceFar
.L_0810b0ac:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810b0b4:
	.4byte 0x000012dd
.L_0810b0b8:
	.4byte 0x000012de
