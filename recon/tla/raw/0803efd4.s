.syntax unified
	.thumb
	.global Func_0803efd4
	.thumb_func
Func_0803efd4:
	push {lr}
	movs r3, #210
	lsls r3, r3, #2
	adds r0, r0, r3
	ldr r0, [r0]
	sub sp, #12
	movs r2, #0
	cmp r0, #0
	beq .L_0803efee
.L_0803efe6:
	ldr r0, [r0, #4]
	adds r2, #1
	cmp r0, #0
	bne .L_0803efe6
.L_0803efee:
	ldr r3, .L_0803f000
	mov r0, sp
	lsls r2, r2, #1
	strh r3, [r0, r2]
	movs r1, #0
	bl BattlePres_SetActorModesFar
	add sp, #12
	pop {pc}
.L_0803f000:
	.4byte 0x000000ff
