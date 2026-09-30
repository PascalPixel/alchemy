.syntax unified
	.thumb
	.global Ability_LoadGlyph
	.thumb_func
Ability_LoadGlyph:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	ldr r5, [sp, #24]
	adds r6, r1, #0
	mov r8, r2
	mov r10, r3
	bl BattleAction_Get
	adds r1, r6, #0
	ldrh r0, [r0, #4]
	mov r2, r8
	mov r3, r10
	str r5, [sp, #0]
	bl Func_0803d9bc
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
