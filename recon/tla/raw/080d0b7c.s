.syntax unified
	.thumb
	.global DisplayTransition_FillTilemapAndSolidTile
	.thumb_func
DisplayTransition_FillTilemapAndSolidTile:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #124]
	ldr r3, .L_080d0be0
	sub sp, #4
	adds r4, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_080d0be4
	ldr r2, .L_080d0be8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #1
	negs r2, r2
	cmp r4, r2
	beq .L_080d0bdc
	movs r1, #0
	movs r3, #7
.L_080d0ba8:
	lsls r1, r1, #4
	subs r3, #1
	orrs r1, r4
	cmp r3, #0
	bge .L_080d0ba8
	movs r3, #161
	lsls r3, r3, #3
	adds r2, r5, r3
	movs r3, #7
.L_080d0bba:
	subs r3, #1
	stmia r2!, {r1}
	cmp r3, #0
	bge .L_080d0bba
	movs r2, #161
	lsls r2, r2, #3
	adds r0, r5, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #192
	lsls r2, r2, #24
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_080d0bdc:
	add sp, #4
	pop {r5, pc}
.L_080d0be0:
	.4byte 0xf000f000
.L_080d0be4:
	.4byte 0x06002000
.L_080d0be8:
	.4byte 0x85000140
