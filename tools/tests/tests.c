/* Small, invented assets exercise editing and failure paths without a ROM. */
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE
#include "../ags-graphics/graphics.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

static char *tools;

static void check(bool condition, const char *message) {
  if (!condition)
    fail("test: %s", message);
}

static void equal(const Buffer *a, const Buffer *b, const char *message) {
  check(a->len == b->len && (!a->len || !memcmp(a->data, b->data, a->len)),
        message);
}

static void save(const char *path, const void *data, size_t size) {
  Buffer b = {(uint8_t *)data, size, size};
  writeFile(path, &b);
}

static void textFile(const char *path, const char *text) {
  save(path, text, strlen(text));
}

static void runProgram(const char *path, bool success, char *const args[]) {
  pid_t child = fork();
  check(child >= 0, "fork");
  if (!child) {
    if (!freopen("command.log", "w", stdout) ||
        !freopen("command.log", "a", stderr))
      _exit(127);
    execvp(path, args);
    _exit(127);
  }
  int status;
  while (waitpid(child, &status, 0) < 0)
    check(errno == EINTR, "waitpid");
  /* A rejected input must produce an ordinary error, never a crash. */
  if (!WIFEXITED(status) ||
      (success ? WEXITSTATUS(status) != 0 : WEXITSTATUS(status) != 1)) {
    Buffer log = readFile("command.log");
    fprintf(stderr, "%s", log.data);
    fail("%s returned unexpected status %d", path, status);
  }
}

static void run(const char *name, bool success, char *const args[]) {
  char *path = formatString("%s/ags-%s", tools, name);
  runProgram(path, success, args);
  free(path);
}

#define RUN(name, success, ...)                                                \
  run(name, success, (char *const[]){(char *)name, __VA_ARGS__, NULL})

static Buffer decodePalette(const Buffer *packed) {
  Buffer out = {0};
  size_t at = 0;
  for (;;) {
    check(at < packed->len, "palette flags are present");
    unsigned flags = packed->data[at++];
    for (unsigned bit = 0; bit < 8; bit++) {
      check(at < packed->len, "palette token is present");
      if (!(flags & (0x80 >> bit))) {
        byte(&out, packed->data[at++]);
        continue;
      }
      check(at + 2 <= packed->len, "palette copy is complete");
      unsigned first = packed->data[at++], second = packed->data[at++];
      size_t distance = (first >> 4) * 256 + second;
      if (!distance)
        return out;
      size_t length = (first & 15) + 1;
      if (length == 1) {
        check(at < packed->len, "long palette copy is complete");
        length = packed->data[at++] + 17;
      }
      check(distance <= out.len, "palette copy refers to earlier output");
      for (size_t i = 0; i < length; i++)
        byte(&out, out.data[out.len - distance]);
    }
  }
}

static void compression(void) {
  for (size_t length = 0; length <= 10000;
       length = length ? length * 3 + 1 : 1) {
    Buffer input = {0};
    uint32_t seed = 0x12345678;
    for (size_t i = 0; i < length; i++) {
      seed = seed * 1664525 + 1013904223;
      byte(&input, i % 32 < 16 ? seed >> 24 : i % 7);
    }
    Buffer packed = compressLz(&input, &input, "plz", packer);
    Buffer decoded = decodePalette(&packed);
    equal(&input, &decoded,
          "palette compression round trip, including overlapping copies");
    free(input.data);
    free(packed.data);
    free(decoded.data);
  }
  uint8_t shipped[600], neutral[600];
  for (size_t i = 0; i < sizeof(shipped); i++) {
    shipped[i] = i % 2;
    neutral[i] = i % 2 ? 2 : 0;
  }
  Buffer a = {shipped, sizeof(shipped), 0}, b = {neutral, sizeof(neutral), 0};
  Buffer packed = compressLz(&a, &b, "plz", packer);
  Buffer decoded = decodePalette(&packed);
  equal(&a, &decoded, "post-packing literal edits survive copies");
  free(packed.data);
  free(decoded.data);

  uint8_t zeros[] = {0, 0, 7, 0, 8};
  uint8_t expected[] = {0xe1, 7, 0xe0, 8, 0};
  a = (Buffer){zeros, sizeof(zeros), 0};
  b = (Buffer){expected, sizeof(expected), 0};
  packed = compressZeros(&a);
  equal(&packed, &b, "zero-skip lengths and terminator");
  free(packed.data);
  Buffer empty = {0};
  writeChanged("empty.ld", &empty);
  writeChanged("empty.ld", &empty);
  check(exists("empty.ld"), "empty linker fragments can be saved repeatedly");
}

static void graphics(void) {
  Image image = {.width = 16, .height = 16, .depth = 4};
  for (unsigned i = 0; i < 16; i++) {
    byte(&image.palette, i * 8);
    byte(&image.palette, 0);
    byte(&image.palette, 0);
  }
  for (unsigned y = 0; y < 16; y++)
    for (unsigned x = 0; x < 16; x++)
      byte(&image.pixels, (x / 8 + y / 8 * 2 + 1));
  Buffer png = writeImage(&image);
  writeFile("tiles.png", &png);
  RUN("graphics", true, "tiles.png", "tiles.4bpp");
  Buffer tiles = readFile("tiles.4bpp");
  check(tiles.len == 128, "four 4bpp tiles");
  for (size_t i = 0; i < tiles.len; i++)
    check(tiles.data[i] == (i / 32 + 1) * 17,
          "tile order and packed pixel pairs");
  RUN("graphics", true, "tiles.png", "redrawn.png", "--from", "4bpp", "--to",
      "4bpp16x16");
  RUN("graphics", true, "redrawn.png", "redrawn.4bpp16x16");
  Buffer redrawn = readFile("redrawn.4bpp16x16");
  equal(&tiles, &redrawn, "redrawing preserves the tile stream");
  textFile("map.tsv", "cell:u8\tgrid:2x3\tat:0,0\tfill:ff\n01 02\n\n03 04\n");
  RUN("graphics", true, "map.tsv", "map.plane");
  Buffer plane = readFile("map.plane");
  uint8_t expected[] = {1, 2, 255, 255, 3, 4};
  Buffer reference = {expected, sizeof(expected), 0};
  equal(&plane, &reference, "blank map rows retain their position");
  RUN("graphics", false, "tiles.png", "tiles.4bpp", "-mwidth", "0");
  Buffer preserved = readFile("tiles.4bpp");
  equal(&tiles, &preserved, "invalid edits leave the previous output intact");
  free(preserved.data);
  free(plane.data);
  free(redrawn.data);
  free(tiles.data);
  free(png.data);
  freeImage(&image);
}

static void audio(void) {
  uint8_t wav[48] = "RIFF";
  write32(wav + 4, sizeof(wav) - 8);
  memcpy(wav + 8, "WAVEfmt ", 8);
  write32(wav + 16, 16);
  write16(wav + 20, 1);
  write16(wav + 22, 1);
  write32(wav + 24, 8000);
  write32(wav + 28, 8000);
  write16(wav + 32, 1);
  write16(wav + 34, 8);
  memcpy(wav + 36, "data", 4);
  write32(wav + 40, 4);
  wav[44] = 0;
  wav[45] = 128;
  wav[46] = 255;
  wav[47] = 64;
  save("tone.wav", wav, sizeof(wav));
  RUN("audio", true, "tone.wav", "tone.bin");
  Buffer sample = readFile("tone.bin");
  check(sample.len == 20 && read32(sample.data + 4) == 8192000 &&
            read32(sample.data + 12) == 3,
        "PCM rate and sample count");
  check(sample.data[16] == 128 && sample.data[17] == 0 &&
            sample.data[18] == 127 && sample.data[19] == 192,
        "unsigned WAV becomes signed PCM");
  save("tone.wav", wav, sizeof(wav) - 1);
  RUN("audio", false, "tone.wav", "tone.bin");
  Buffer preserved = readFile("tone.bin");
  equal(&sample, &preserved, "truncated WAV preserves the previous output");
  free(sample.data);
  free(preserved.data);
}

static void be32(Buffer *b, uint32_t n) {
  byte(b, n >> 24);
  byte(b, n >> 16);
  byte(b, n >> 8);
  byte(b, n);
}

static void midiFile(unsigned key) {
  const char *skeleton = "smsh-sequence 1\nstream Track\nalign 4 0\n"
                         "header Song block_count=0 priority=0 reverb=0 "
                         "tone_bank=Voices tracks=Track\n";
  Buffer conductor = {0}, midi = {0};
  byte(&conductor, 0);
  byte(&conductor, 255);
  byte(&conductor, 1);
  size_t n = strlen(skeleton);
  check(n < 16384, "short MIDI fixture");
  if (n >= 128)
    byte(&conductor, (n >> 7) | 128);
  byte(&conductor, n & 127);
  append(&conductor, skeleton, n);
  uint8_t end[] = {0, 255, 47, 0};
  append(&conductor, end, sizeof(end));
  uint8_t header[] = {'M', 'T', 'h', 'd', 0, 0, 0, 6, 0, 1, 0, 2, 0, 24};
  append(&midi, header, sizeof(header));
  append(&midi, "MTrk", 4);
  be32(&midi, conductor.len);
  append(&midi, conductor.data, conductor.len);
  uint8_t track[] = {0, 0x90, key, 100, 24,  0x80, key, 0,   0,  255,
                     6, 4,    'f', 'i', 'n', 'e',  0,   255, 47, 0};
  append(&midi, "MTrk", 4);
  be32(&midi, sizeof(track));
  append(&midi, track, sizeof(track));
  writeFile("song.mid", &midi);
  free(conductor.data);
  free(midi.data);
}

static void music(void) {
  midiFile(60);
  RUN("music", true, "song.mid", "song.s");
  Buffer first = readFile("song.s");
  check(strstr((char *)first.data, "0x3c") &&
            strstr((char *)first.data, "Voices"),
        "MIDI note and tone-bank relocation");
  midiFile(61);
  RUN("music", true, "song.mid", "song.s");
  Buffer second = readFile("song.s");
  check(strstr((char *)second.data, "0x3d") != NULL,
        "edited MIDI note reaches the assembly");
  textFile("song.mid", "MThd");
  RUN("music", false, "song.mid", "song.s");
  Buffer preserved = readFile("song.s");
  equal(&second, &preserved, "invalid MIDI preserves the previous output");
  free(first.data);
  free(second.data);
  free(preserved.data);
}

static void catalog(void) {
  const char *header =
      "msgid \"\"\nmsgstr \"\"\n"
      "\"X-Alchemy-Format: 1\\n\"\n\"X-Alchemy-Target: tbs-en\\n\"\n"
      "\"X-Alchemy-Symbol-Count: 256\\n\"\n\n";
  char *po = formatString(
      "%smsgctxt \"MsgGreeting\"\nmsgid \"0\"\nmsgstr \"Hello!\"\n", header);
  textFile("text.po", po);
  free(po);
  RUN("text", true, "text.po", "text.inc");
  RUN("text", true, "text.po", "text.h", "--header", "--as-value");
  Buffer ids = readFile("text.h");
  check(strstr((char *)ids.data, "#define MsgGreeting ((s32)MsgGreeting)") !=
            NULL,
        "message names remain linker values");
  const char *outputs[] = {"text.inc", "text.h"};
  struct stat before[2];
  struct timespec fixedTime[2] = {{1, 0}, {1, 0}};
  for (size_t i = 0; i < 2; i++) {
    check(!utimensat(AT_FDCWD, outputs[i], fixedTime, 0),
          "set generated text output timestamps");
    check(!stat(outputs[i], before + i), "stat generated text output");
  }
  RUN("text", true, "text.po", "text.inc");
  RUN("text", true, "text.po", "text.h", "--header", "--as-value");
  for (size_t i = 0; i < 2; i++) {
    struct stat after;
    check(!stat(outputs[i], &after), "stat unchanged generated text output");
    check(before[i].st_dev == after.st_dev &&
              before[i].st_ino == after.st_ino &&
              before[i].st_mtime == after.st_mtime,
          "identical text archives and headers retain their inode and mtime");
  }
  Buffer first = readFile("text.inc");
  po = formatString(
      "%smsgctxt \"MsgGreeting\"\nmsgid \"0\"\nmsgstr \"Goodbye!\"\n", header);
  textFile("text.po", po);
  free(po);
  RUN("text", true, "text.po", "text.inc");
  Buffer second = readFile("text.inc");
  check(first.len != second.len || memcmp(first.data, second.data, first.len),
        "edited message changes its archive");
  textFile("text.po", "msgid \"0\"\nmsgstr \"broken\"\n");
  RUN("text", false, "text.po", "text.inc");
  Buffer preserved = readFile("text.inc");
  equal(&second, &preserved, "invalid catalog preserves the previous output");
  free(ids.data);
  free(first.data);
  free(second.data);
  free(preserved.data);
  /* PO line wrapping must preserve every native symbol, including
   * embedded ending controls and control arguments that equal an opcode. */
  po = formatString(
      "%smsgctxt \"MsgGreeting\"\nmsgid \"0\"\n"
      "msgstr "
      "\"{text_color:5}Hello{line_break}there{text_color:1}"
      "{actor_name:2}{end_wait}{text_color:3}red{text_color:9}blue"
      "{text_color:16}white{end_wait}\"\n\n"
      "msgctxt \"MsgDeparture\"\nmsgid \"1\"\n"
      "msgstr \"Now{page_break}go!{end_now}\"\n\n"
      "msgctxt \"MsgLiteral\"\nmsgid \"2\"\n"
      "msgstr \"Literal{byte:5c}n\"\n\n"
      "#, alchemy-null\nmsgctxt \"message\"\nmsgid \"3\"\nmsgstr \"\"\n",
      header);
  textFile("text.po", po);
  free(po);
  RUN("text", true, "text.po", "text.inc");
  Buffer markup = readFile("text.inc");
  po = formatString(
      "%smsgctxt \"MsgGreeting\"\nmsgid \"0\"\n"
      "msgstr \"\"\n\"{text_color:yellow}Hello\\n\"\n"
      "\"there{text_color:1}{actor_name:2}{end_wait}{text_color:red}red"
      "{text_color:blue}blue{text_color:white}white{end_wait}\"\n\n"
      "msgctxt \"MsgDeparture\"\nmsgid \"1\"\n"
      "msgstr \"Now{page_break}go!{end_now}\"\n\n"
      "msgctxt \"MsgLiteral\"\nmsgid \"2\"\n"
      "msgstr \"Literal\\\\n\"\n\n"
      "#, alchemy-null\nmsgctxt \"message\"\nmsgid \"3\"\nmsgstr \"\"\n",
      header);
  textFile("text.po", po);
  free(po);
  RUN("text", true, "text.po", "text.inc");
  Buffer multiline = readFile("text.inc");
  equal(&markup, &multiline,
        "PO newlines and four color names preserve numeric colors, controls "
        "and literal \\n");

  const char *invalidFlags[] = {
      "alchemy-end-wait",
      "alchemy-end-now",
      "alchemy-end-wait, alchemy-end-now",
      "alchemy-null, alchemy-end-wait",
      "alchemy-null, alchemy-end-now",
  };
  for (size_t i = 0; i < sizeof(invalidFlags) / sizeof(*invalidFlags); i++) {
    po = formatString("%s#, %s\nmsgctxt \"message\"\nmsgid \"0\"\n"
                      "msgstr \"\"\n",
                      header, invalidFlags[i]);
    textFile("text.po", po);
    free(po);
    RUN("text", false, "text.po", "text.inc");
    Buffer unchanged = readFile("text.inc");
    equal(&multiline, &unchanged,
          "obsolete ending flags preserve the previous archive");
    free(unchanged.data);
  }
  const char *invalidColors[] = {"{text_color:magenta}", "{actor_name:yellow}"};
  for (size_t i = 0; i < sizeof(invalidColors) / sizeof(*invalidColors); i++) {
    po = formatString("%smsgctxt \"message\"\nmsgid \"0\"\nmsgstr \"%s\"\n",
                      header, invalidColors[i]);
    textFile("text.po", po);
    free(po);
    RUN("text", false, "text.po", "text.inc");
    Buffer unchanged = readFile("text.inc");
    equal(&multiline, &unchanged,
          "unknown colors and color names on other commands preserve output");
    free(unchanged.data);
  }
  free(markup.data);
  free(multiline.data);
}

static void buildFiles(void) {
  textFile("a.c", "#include \"a.h\"\n");
  textFile("a.h", "#include \"b.h\"\n");
  textFile("b.h", "#include \"a.h\"\n#include \"text/messages.inc\"\n");
  RUN("deps", true, "deps.mk", "out", "out/obj", "-I.", "-Gtext/", "--", "a.c");
  Buffer deps = readFile("deps.mk");
  check(strstr((char *)deps.data,
               "out/obj/a.o: a.h b.h out/text/messages.inc") != NULL,
        "include cycles terminate and generated dependencies remain");
  free(deps.data);
  /* Make must rescan a changed source or header before deciding which
   * objects need rebuilding, without looping on generated includes. */
  textFile("scan.c", "#include \"scan.h\"\n");
  textFile("scan.h", "#include \"out/generated.h\"\n");
  check(!mkdir("out", 0777), "create generated include directory");
  textFile("out/generated.h", "");
  char *makefile = formatString(
      ".DEFAULT_GOAL := all\ninclude scan.mk\n"
      "scan.mk: %s/ags-deps\n"
      "\t%s/ags-deps $@ out out/obj -I. -- scan.c\n"
      "all: out/obj/scan.o\n"
      "out/obj/scan.o: scan.c\n\tmkdir -p out/obj\n\ttouch $@\n",
      tools, tools);
  textFile("scan.make", makefile);
  free(makefile);
  char *const makeArgs[] = {"make", "-f", "scan.make", NULL};
  char *const queryArgs[] = {"make", "-q", "-f", "scan.make", NULL};
  runProgram("make", true, makeArgs);
  runProgram("make", true, queryArgs);
  /* Force an older scan time, avoiding sleeps and filesystem clock races. */
  const struct timespec oldTimes[2] = {{1, 0}, {1, 0}};
  check(!utimensat(AT_FDCWD, "scan.mk", oldTimes, 0), "age dependency graph");
  textFile("scan.h", "#include \"out/generated.h\"\n#include \"added.h\"\n");
  textFile("added.h", "");
  runProgram("make", true, makeArgs);
  deps = readFile("scan.mk");
  check(strstr((char *)deps.data, "out/obj/scan.o: added.h out/generated.h scan.h"),
        "header edits add transitive object dependencies");
  const char *self = strstr((char *)deps.data, "scan.mk:");
  check(self && strstr(self, " scan.c") && strstr(self, " scan.h") &&
            strstr(self, " added.h") &&
            !strstr(self, " out/generated.h"),
        "scanner depends on sources and headers, not generated includes");
  free(deps.data);
  runProgram("make", true, queryArgs);
  check(!utimensat(AT_FDCWD, "scan.mk", oldTimes, 0), "age source scan");
  textFile("scan.c", "#include \"scan.h\"\n#include \"source_added.h\"\n");
  textFile("source_added.h", "");
  runProgram("make", true, makeArgs);
  deps = readFile("scan.mk");
  check(strstr((char *)deps.data,
               "out/obj/scan.o: added.h out/generated.h scan.h source_added.h"),
        "source edits add new object dependencies");
  free(deps.data);
  runProgram("make", true, queryArgs);
  textFile("objects.tsv",
           "image\tplace\tobject\tsections\teditions\n"
           "main\trom_start\tgames/a\t\ten\nmain\trom_start\tgames/b\t\tja\n"
           "main\trom_start\tgames/c\n");
  textFile("overlays.tsv", "overlay\tbss\n");
  RUN("objects", true, "objects.tsv", "overlays.tsv", "en", "out");
  Buffer objects = readFile("out/objects.mk"),
         linker = readFile("out/ld/rom_start.ld");
  check(strstr((char *)objects.data, "games/a.o") &&
            !strstr((char *)objects.data, "games/b.o") &&
            strstr((char *)objects.data, "games/c.o"),
        "edition filters the object list");
  const char *a = strstr((char *)linker.data, "games/a.o"),
             *c = strstr((char *)linker.data, "games/c.o");
  check(a && c && a < c, "linker retains source object order");
  free(objects.data);
  free(linker.data);
}

static void header(void) {
  uint8_t data[257] = {0};
  data[256] = 42;
  save("sample.gba", data, sizeof(data));
  RUN("header", true, "sample.gba", "-tEXAMPLE", "-cTEST", "-p", "--silent");
  Buffer rom = readFile("sample.gba");
  check(rom.len == 512 && rom.data[256] == 42 && rom.data[257] == 255,
        "header padding preserves payload");
  unsigned sum = 0x19;
  for (size_t i = 0xa0; i <= 0xbd; i++)
    sum += rom.data[i];
  check(!(sum & 255) && !memcmp(rom.data + 0xa0, "EXAMPLE", 7),
        "header fields and complement");
  RUN("header", false, "sample.gba", "-");
  free(rom.data);
}

static void overlays(void) {
  uint8_t data[] = {0, 0xf0, 0, 0xf8, 0xff, 0xf7, 0xff, 0xff};
  save("overlay.bin", data, sizeof(data));
  RUN("overlays", true, "overlay.bin", "overlay.lz");
  uint8_t filtered[] = {0, 0xf0, 1, 0xf8, 0, 0xf0, 2, 0xf8};
  Buffer input = {filtered, sizeof(filtered), 0};
  Buffer expected = compressLz(&input, &input, "tagged", packer),
         actual = readFile("overlay.lz");
  equal(&expected, &actual,
        "overlay filter handles forward calls and wrapped negative "
        "displacements");
  free(expected.data);
  free(actual.data);
}

static void removeTree(const char *path) {
  DIR *dir = opendir(path);
  if (!dir) {
    check(!unlink(path), "remove test file");
    return;
  }
  struct dirent *entry;
  while ((entry = readdir(dir))) {
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
      continue;
    char *child = formatString("%s/%s", path, entry->d_name);
    removeTree(child);
    free(child);
  }
  closedir(dir);
  check(!rmdir(path), "remove test directory");
}

int main(int argc, char **argv) {
  check(argc == 2, "test runner needs the tools directory");
  tools = realpath(argv[1], NULL);
  check(tools != NULL, "find tools directory");
  char scratch[] = "/tmp/alchemy-tools-XXXXXX";
  check(mkdtemp(scratch) != NULL && !chdir(scratch), "create test directory");
  const char *names[] = {"graphics", "audio",   "music",  "text",
                         "deps",     "objects", "header", "overlays"};
  for (size_t i = 0; i < sizeof(names) / sizeof(*names); i++)
    RUN(names[i], true, "--help");
  compression();
  graphics();
  audio();
  music();
  catalog();
  buildFiles();
  header();
  overlays();
  check(!chdir("/"), "leave test directory");
  removeTree(scratch);
  free(tools);
  puts("C tools: all tests passed");
  return 0;
}
