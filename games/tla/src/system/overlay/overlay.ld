/* One overlay of The Lost Age, loaded at 0x2008000. Its objects are its
   rows of games/tla/objects.tsv in ROM order, which the build writes for the
   overlay it links into extern.ld, text.ld, bss.ld and discard.ld; bss.ld
   is the whole .bss section, under the header games/tla/overlays.tsv gives
   it. Its rows open with its entry table DIR/entry.s, and each names the
   section it places, so text.ld keeps their order. Where its globals are written in assembly, its .bss is DIR/bss.s,
   on the overlay's bss row. */
INCLUDE "extern.ld"
SECTIONS
{
    .text 0x2008000 :
    {
        INCLUDE "text.ld"
    }
    INCLUDE "bss.ld"
    /DISCARD/ :
    {
        INCLUDE "discard.ld"
        *(.ARM.attributes) *(.comment) *(.note*)
    }
}
