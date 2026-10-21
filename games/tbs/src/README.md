# The Broken Seal's source

The Broken Seal's code is laid out the way the cartridge itself divides the
program. The main part of it comes in banks, and each big bank has a folder
here. The code for each town and dungeon, and for the world map, isn't in
that main part at all: it's an overlay, loaded when its scene starts, and the
overlays have folders of their own. A few more folders gather one kind of
thing, such as the pictures or the sound, wherever it ends up.

If you know the game, the easiest way in is usually through something you
remember: a Psynergy's animation, the people of a town or a shop counter.

## Why does each bank get a folder?

The main image is the program the processor runs from the cartridge, and
`main.ld` lays it out in banks. Every bank after the first starts at a fixed
address of its own, and whatever room the bank before it doesn't fill is
left empty. Each bank also has a table of 8-byte jumps, one for every
function used from outside it, and code in another bank reaches those
functions only through their jumps, usually by the function's name with
`Far` added: the main loop, in the `object` bank, starts a battle with
`Battle_RunEncounterFar`. The first bank, the runtime's, is the exception.
Every other bank calls its functions directly, and its table is mostly the
overlays' way in. Inside a bank, functions simply call each other.
[System](system/README.md#why-the-program-is-cut-into-banks) explains what
the tables buy.

So each bank is a piece of the program the original kept apart from the
rest, and the code here is grouped the same way. The nine big banks each have
a folder, in the order the cartridge holds them: `runtime`, `field`,
`window`, `game`, `object`, `menu`, `shop`, `battle` and `effect`. Five small
banks come after them, and their code sits in the folder it belongs with: the
credits and the title intro in `graphics`, Lucky Dice in `field`, the slot
machine in `object` and the sound cue layer in `sound`. The other folders
don't follow a bank. `graphics`, `map`, `sound` and `system` each gather one
kind of thing wherever the link puts it, and `scene` and `debug` hold
overlays. The cartridge keeps no names, so every folder name is ours, chosen
for what the folder holds.

Within a folder, every file the build compiles or assembles becomes one
object. [`objects.tsv`](../objects.tsv) lists every object in link order,
with the image it belongs to (`main`, or an overlay's resource number) and
where in that image it goes, such as `rom_main_07`, the battle's bank.
[`main.ld`](../main.ld) gives each bank its starting address in each edition.

The biggest objects are too long to read as one file, so they're written in
parts. `battle/battle.c` includes its private header, `battle_internal.h`,
and then each file under `battle/` in ROM order, and only `battle.c` itself
is compiled. Camelot's own files aren't known, so the parts and their names
are ours.

## What does each folder cover?

Walking around takes four of them. [Field](field/README.md) is the engine on
screen whenever the party is out walking, whether that's a town, a dungeon or
the world map: it moves the people and things, turns the direction pad into
steps, and loads, scrolls and draws the maps.
[Field objects](object/README.md) runs one field scene on that engine, from
setting it up to the exit the party leaves by, with random encounters,
Psynergy used on the field and the commands scene scripts run, and the
game's main loop is there too. [Scenes](scene/README.md) holds the places
themselves, one overlay each: who stands where, what happens when the party
talks to someone or searches a spot, and the cutscenes. [Maps](map/README.md)
has no code at all. It's the maps under every town, dungeon and house, and
the world map's ground, written as tables.

Two folders share the battles. [Battles](battle/README.md) plays a fight out
from start to finish: who acts in what order, what the enemies choose, what
each action does to the units it reaches, and the coins and experience the
party comes away with. [Battle effects](effect/README.md) draws what plays
over the battle while Psynergy, a Djinni, a summon, an item or an enemy's
attack goes off, one function for each effect.

Three hold the screens the player reads and chooses from.
[Windows](window/README.md) draws the framed boxes nearly every line of text
appears in, decodes the game's compressed messages and types them out, and
holds a good many of the screens built from windows, the battle menus, the
save file menu and name entry among them. [Menus](menu/README.md) has the
field menu's Psynergy, Djinn, Item and Status screens, along with the title
and file select screens and the Battle Arena's lobby, which are overlays.
[Shops](shop/README.md) runs every counter where the party spends or earns
coins: buying and selling, repairs, a sanctum's healing, the inn, the artifact
counter and the Game Tickets a shop gives away.

[Game rules](game/README.md) answers what the rest of the game asks about the
party: who's in it and what they carry, their levels and classes, the Djinn
each member holds, and the flags that remember what the party has done. Its
`.tsv` tables describe every item, ability, Djinni, enemy, summon and class.

Five more sit underneath all of that. [Runtime](runtime/README.md) powers the
game up and then runs every frame through `WaitFrames`, along with the
buttons, memory, fades, sprites, saving and the link cable.
[System](system/README.md) decides where everything sits in memory and how
banks and overlays reach each other, and holds the resource directory the
game finds its pictures, tables and overlays through, and the flash save
library. [Graphics](graphics/README.md) holds most of the pictures, from
monsters and backdrops to fonts and tile sets, with the title intro, the
credits and the decoders that unpack pictures for everyone else.
[Sound](sound/README.md) turns the numbers the game plays into music,
fanfares and sound effects on top of Nintendo's MusicPlayer2000 driver; the
songs and samples themselves are in [`../sound`](../sound). And
[Debug rooms](debug/README.md) visits the three test rooms left in the
finished game, each put together like any other scene.

## How does a place's code get loaded?

When the party walks into a place, its code is unpacked from the cartridge
into one 32 KB area of work RAM, `gOverlayArea`, and runs from there until
the party moves on. That code is the place's overlay.

The whole game runs in one loop that never returns, `Game_ResetForNewGame`
in [object/object/scene.c](object/object/scene.c), and each pass of it runs
one scene, or a battle or a minigame. A battle or a minigame needs no
overlay, because its code is in the main image. For every other scene the
loop looks up its row of the scene table, `gField_SceneTable` in
[field/scenes.s](field/scenes.s), and `Scene_LoadOverlay` unpacks the overlay
that row names with `Resource_LoadCode`. Then `Field_RunScene` runs the scene
until the party takes an exit, and the loop goes round again.

The table has 201 scenes and they share 96 overlays, since several scenes can
use one. Ninety of the overlays are in [Scenes](scene/README.md), three in
[Menus](menu/README.md) and three in [Debug rooms](debug/README.md). Each
overlay is built around three files: its entry table, `entry/<place>.s`, six
jumps the engine calls to start the scene and to ask for its entrances,
exits, placements, events and regions; its own code and tables,
`<place>.c`; and its import table, `import/<place>.s`, the jumps it takes
into the main image.

Only one overlay is loaded at a time, so one overlay can't call another, and
code that several places need is repeated in every overlay that uses it: the
headers in `scene/common/` hold such code, and each scene that includes one
compiles its own copy.
[System](system/README.md#how-an-overlay-reaches-the-rest-of-the-game) has
the details of the jumps and of how an overlay is packed.

## Where are the headers?

They're in [`../include`](../include). Its top level and the folders under
it (`battle`, `field`, `game`, `graphics`, `lib`, `menu`, `sound` and
`system`) hold what the whole game shares: its types and structures, the game
flags in `flags.h`, the songs and sound effects in `songs.h`, and in
`event.h` the commands scenes are written in. The `Makefile` puts each of
those folders on the include path, so code includes their headers by name
alone.

[`../include/unit`](../include/unit) mirrors this folder. Every C file the
build compiles here has a header at the same path under `unit` that declares
what the file defines, for the code elsewhere that uses it:
`unit/battle/battle.h` goes with `battle/battle.c`, and `unit/scene/` has one
for each place. [`../include/farcall`](../include/farcall) has a header for
each bank, with the `...Far` names its table gives. An object written
in parts also keeps a private header beside it, such as
`battle/battle_internal.h`, for what only its parts share.

## What's written in C, and what in assembly?

Nearly all of it is C: in tbs-en's link maps, about 95% of the bytes of code
come from C files. The game's own C is compiled by alchemy-gcc, a GCC from
July 2000, into Thumb, the compact instruction set most of the game runs in.
Three files written for ARM, the processor's other instruction set,
`window/glyph_arm.c`, `field/update.c` and `field/vertices.c`, are compiled
by GCC 2.95.2, and most of the sound driver and the flash save library by
pret's `old_agbcc`. `make modern` builds the same C with a current GCC as
well. [Matching](MATCHING.md) explains how a build is judged against the
cartridge, and why some of the C is written the way it is.

Code stays assembly where its own instructions show that none of those
compilers wrote it, and each such file gives its proof at the top, in its
`@ C2:` lines. The tells are things like a branch on the carry a shift leaves
behind (`runtime/trig.s`), a jump through an address loaded straight into the
program counter (`graphics/tile/decode.s`), a routine that switches from
Thumb to ARM partway through (`sound/mixer.s`), registers saved without the
return address (`system/resource/decode_byte_lz.s`), or a change of the
processor's mode (`system/entry.s`). A few of the sound driver's routines are
assembly for another reason: they're MusicPlayer2000's own library assembly,
which pret's decompilations keep as assembly too. All of it comes to about
16 KB, mostly decoders that unpack pictures and data, the sound mixing, and
the small fast routines copied into the console's internal RAM. A few tiny
handwritten helpers sit inside the C instead, each marked `CAMELOT_ASM` with
its proof.

Most of the remaining code is jumps: the 8-byte entries of the far-call
tables in `system/far_call/` (`.S` files, which go through the C preprocessor
first) and of every overlay's entry and import tables, all written out whole
by the `overlay_veneer` macro. The compiler's own helper routines, such as
the floating-point arithmetic two scenes use, come from libgcc, which the
build compiles from alchemy-gcc's sources.

Most `.s` files hold no code at all. They place data: a picture built from
the PNG beside it, a table from its `.tsv`, or a map, each under a name the
code uses.

## Where's a good place to start?

To read the game the way it runs, start at power-on. `System_Initialize`, in
[runtime/runtime/system.c](runtime/runtime/system.c), sets the console up and
hands over to `Game_ResetForNewGame`, the loop the whole game lives in, which
is short enough to read in one go. [Runtime](runtime/README.md) follows the
same path in prose.

To find something you remember from playing, search for its English name.
Each effect's comment lists the abilities that play it, so a search of
`effect/` for Quake lands on the function that draws it, in
[effect/parts/quake.c](effect/parts/quake.c). Each place's code is the scene
named for it. The game's text is in [`../text`](../text), a PO catalog for
each language, and in `en.po` many lines carry the name the code knows them
by: "(No saved data)" is `MsgSaveNoData`, which leads to the save file menu
in [window/battle_command/save.c](window/battle_command/save.c).

Every big object opens with a guide comment that says what it holds, most of
them with where each family of functions starts, and every part's first lines
say what's in it. So the tops of a folder's files read like its table of
contents. And before changing anything, [Matching](MATCHING.md) says how a
build is judged and what the markers in the comments mean.
