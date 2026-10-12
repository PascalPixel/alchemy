/* One overlay of The Broken Seal, loaded at 0x2008000. Its objects are its
   rows of games/tbs/objects.tsv, which the build writes for the overlay it
   links into extern.ld, text.ld, bss.ld and discard.ld; bss.ld is the whole
   .bss section, under the header games/tbs/overlays.tsv gives it. Its unit
   row names its C file DIR/NAME.c and is three objects of text.ld: its
   entry table, the C file and its import table. The entry table is
   DIR/entry/NAME.s where that file exists and DIR/entry.s otherwise, and
   the import table is DIR/import/NAME.s or DIR/import.s the same way. */
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
