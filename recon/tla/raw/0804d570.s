.syntax unified
	.thumb
	.global Menu_AnimateSelectionToEntry
	.thumb_func
Menu_AnimateSelectionToEntry:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r5, r0, #0
	bl Func_0803f800
	bl AffineEffect_InitializeWork
	movs r0, #1
	bl Menu_AppendResourceEntry
	movs r0, #15
	bl Menu_AppendResourceEntry
	movs r0, #2
	bl Menu_AppendResourceEntry
	movs r0, #7
	bl Menu_AppendResourceEntry
	subs r1, r6, #1
	adds r0, r5, #0
	bl Func_0804d28c
	adds r6, r0, #0
	bl Menu_EndResourceSelection
	bl Func_0803f810
	adds r0, r6, #0
	pop {r5, r6, pc}
