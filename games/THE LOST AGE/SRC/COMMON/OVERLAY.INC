/* Fixed overlay entry/import veneer; the calling convention permits r4 clobber.
   A Lost Age code overlay opens with seven of these entry veneers. */
.macro overlay_veneer target
    .if ((\target & 1) == 0) && ((\target & 3) != 0)
    .error "ARM entry point target must be word aligned"
    .endif
    ldr r4, [pc, #0]
    bx r4
    .4byte \target
.endm
