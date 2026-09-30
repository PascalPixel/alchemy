.syntax unified
	.thumb
	.global Script_SetOrCompareComparisonResult
	.thumb_func
Script_SetOrCompareComparisonResult:
	push {lr}
	adds r4, r2, #0
	cmp r1, #0
	bne .L_08025fac
	adds r3, r0, #0
	adds r3, #87
	strb r4, [r3]
	b .L_08025fce
.L_08025fac:
	cmp r1, #1
	bne .L_08025fbc
	adds r2, r0, #0
	adds r2, #87
	ldrb r3, [r2]
	adds r3, r3, r4
	strb r3, [r2]
	b .L_08025fce
.L_08025fbc:
	adds r0, #87
	ldrb r2, [r0]
	lsls r3, r4, #24
	lsrs r3, r3, #24
	movs r1, #0
	cmp r2, r3
	bne .L_08025fcc
	movs r1, #1
.L_08025fcc:
	strb r1, [r0]
.L_08025fce:
	pop {pc}
