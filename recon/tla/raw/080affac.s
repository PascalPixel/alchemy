.syntax unified
	.thumb
	.global Func_080affac
	.thumb_func
Func_080affac:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r2, #0
	cmp r5, #7
	ble .L_080affe8
	bl Func_080ad3a8
	adds r0, #42
	ldrb r1, [r0]
	cmp r1, #47
	bls .L_080affc4
	movs r1, #0
.L_080affc4:
	lsls r3, r1, #1
	ldr r2, .L_080b0024
	adds r3, r3, r1
	lsls r3, r3, #3
	adds r3, r3, r2
	movs r4, #0
	adds r0, r6, #0
	adds r1, r3, #4
.L_080affd4:
	ldrb r2, [r1]
	adds r4, #1
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r1, #1
	stmia r0!, {r3}
	cmp r4, #3
	ble .L_080affd4
	b .L_080b0020
.L_080affe8:
	adds r0, r6, #0
	adds r1, #36
	movs r4, #3
.L_080affee:
	ldrb r2, [r1]
	subs r4, #1
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r1, #1
	stmia r0!, {r3}
	cmp r4, #0
	bge .L_080affee
	cmp r5, #7
	bgt .L_080b0020
	adds r0, r5, #0
	bl Owner_GetRecordStride180
	adds r1, r6, #0
	adds r0, #146
	movs r4, #3
.L_080b0010:
	ldrb r2, [r0]
	ldr r3, [r1]
	subs r4, #1
	adds r3, r3, r2
	adds r0, #1
	stmia r1!, {r3}
	cmp r4, #0
	bge .L_080b0010
.L_080b0020:
	movs r0, #0
	pop {r5, r6, pc}
.L_080b0024:
	.4byte Data_080c6684
