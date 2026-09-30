.syntax unified
	.thumb
	.global Func_0804bba8
	.thumb_func
Func_0804bba8:
	push {r5, lr}
	mov r5, r9
	push {r5}
	sub sp, #8
	mov r5, sp
	mov r3, r9
	str r3, [sp, #4]
	adds r0, r5, #0
	movs r3, #255
	strh r3, [r5]
	bl Func_080461c8
	adds r0, r5, #0
	movs r1, #1
	bl BattlePres_SetActorModesFar
	add sp, #8
	pop {r3}
	mov r9, r3
	pop {r5, pc}
