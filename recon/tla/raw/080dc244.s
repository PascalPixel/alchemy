.syntax unified
	.thumb
	.global Field_BeginPaletteTransition
	.thumb_func
Field_BeginPaletteTransition:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #34
	adds r6, r5, r3
	adds r7, r0, #0
	movs r1, #2
	adds r0, r6, #0
	bl Func_080d170c
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #164
	adds r5, r5, r3
	movs r3, #0
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_080dc27c
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #1
	movs r1, #1
	bl Func_080d170c
	b .L_080dc284
.L_080dc27c:
	adds r0, r6, #0
	movs r1, #1
	bl Func_080d170c
.L_080dc284:
	movs r0, #132
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	adds r0, r7, #0
	bl Func_080d17ac
	pop {r5, r6, r7, pc}
