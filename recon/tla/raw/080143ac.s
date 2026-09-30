.syntax unified
	.thumb
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	push {lr}
	ldr r1, .L_080143dc
	movs r4, #255
	ldrh r3, [r1, #2]
	lsls r4, r4, #8
	adds r4, #255
	movs r0, #96
	movs r2, #0
	cmp r3, r4
	bne .L_080143c4
	movs r0, #0
	b .L_080143da
.L_080143c4:
	adds r2, #1
	adds r1, #4
	cmp r2, #95
	bgt .L_080143da
	movs r4, #255
	ldrh r3, [r1, #2]
	lsls r4, r4, #8
	adds r4, #255
	cmp r3, r4
	bne .L_080143c4
	adds r0, r2, #0
.L_080143da:
	pop {pc}
.L_080143dc:
	.4byte ResourceTableEntries
