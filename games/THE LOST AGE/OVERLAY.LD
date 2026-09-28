/* A code overlay: its whole listing, linked where the resource loader places it. */
OUTPUT_ARCH(arm)
SECTIONS
{
    .text 0x2008000 : { *(.text .text.* .rodata .rodata.*) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
