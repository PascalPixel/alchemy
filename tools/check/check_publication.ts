#!/usr/bin/env bun

const ZERO_OID = /^0+$/;
const BLOCKED_EXTENSIONS = new Set([
  "a",
  "bin",
  "bps",
  "bsdiff",
  "d",
  "diff",
  "dis",
  "dll",
  "dmp",
  "dump",
  "dylib",
  "elf",
  "exe",
  "gba",
  "gz",
  "ips",
  "lst",
  "log",
  "map",
  "o",
  "patch",
  "raw",
  "rom",
  "sav",
  "so",
  "sym",
  "tar",
  "tgz",
  "ups",
  "xdelta",
  "xdelta3",
  "zip",
  "7z",
]);
const BLOCKED_DIRECTORIES = new Set([
  ".cache",
  "alchemy-gcc",
  "analysis",
  "build",
  "builds",
  "cmatch",
  "comparisons",
  "compiler-output",
  "diffs",
  "disassembly",
  "dist",
  "dump",
  "dumps",
  "m2c",
  "objdump",
  "out",
  "private",
  "report",
  "reports",
  "rom",
  "roms",
  "toolchain",
  "toolchains",
  "work",
]);
const PRIVATE_REPORT =
  /(?:^|[._-])(?:analysis|comparison|diff|dump|report)(?:[._-]|$)/i;
const REPORT_EXTENSIONS = new Set([
  "csv",
  "json",
  "jsonl",
  "log",
  "tsv",
  "txt",
]);

type GitResult = { stdout: Buffer; stderr: Buffer; exitCode: number };

function git(args: string[], input?: string): GitResult {
  return Bun.spawnSync(["git", ...args], {
    stdin: input === undefined ? undefined : Buffer.from(input),
    stdout: "pipe",
    stderr: "pipe",
  });
}

function output(result: GitResult, label: string): Buffer {
  if (result.exitCode !== 0)
    throw new Error(result.stderr.toString().trim() || `${label} failed`);
  return result.stdout;
}

function nulList(value: Uint8Array): string[] {
  return Buffer.from(value)
    .toString()
    .split("\0")
    .filter((item) => item.length > 0);
}

function lines(value: Uint8Array): string[] {
  return Buffer.from(value)
    .toString()
    .split(/\r?\n/)
    .map((item) => item.trim())
    .filter((item) => item.length > 0);
}

function extension(path: string): string {
  const leaf = path.slice(path.lastIndexOf("/") + 1);
  const dot = leaf.lastIndexOf(".");
  return dot < 0 ? "" : leaf.slice(dot + 1).toLowerCase();
}

function canonicalBinarySource(path: string): boolean {
  const normalized = path.replaceAll("\\", "/").toLowerCase();
  return (
    normalized.startsWith("assets/maps/") &&
    ["metatiles.bin", "metatile_attributes.bin"].includes(
      normalized.slice(normalized.lastIndexOf("/") + 1),
    )
  );
}

export function publicationPathReason(path: string): string | undefined {
  const normalized = path.replaceAll("\\", "/");
  const components = normalized.split("/").filter((item) => item.length > 0);
  const leaf = components.at(-1)?.toLowerCase() ?? "";
  const directories = components.slice(0, -1).map((item) => item.toLowerCase());
  const suffix = extension(normalized);
  if (normalized.startsWith("/") || components.includes(".."))
    return "invalid repository path";
  if (
    directories.some(
      (item) => BLOCKED_DIRECTORIES.has(item) || item.startsWith(".cmatch"),
    )
  ) {
    return "private or generated directory";
  }
  if (leaf === "baserom" || leaf.startsWith("baserom."))
    return "private ROM name";
  if (leaf.includes(".gba.") || leaf.includes(".rom."))
    return "private ROM name";
  if (BLOCKED_EXTENSIONS.has(suffix) && !canonicalBinarySource(normalized))
    return "private or generated file type";
  if (REPORT_EXTENSIONS.has(suffix) && PRIVATE_REPORT.test(leaf))
    return "private analysis report";
  return undefined;
}

function gbaImage(data: Uint8Array): boolean {
  if (
    data.length < 0xc0 ||
    data.length % 0x8000 !== 0 ||
    data.length > 0x04000000
  )
    return false;
  if (data[0xb2] !== 0x96 || data[0xb3] !== 0 || data[0xb4] !== 0) return false;
  for (let index = 0xb5; index <= 0xbb; index++)
    if (data[index] !== 0) return false;
  let sum = 0;
  for (let index = 0xa0; index <= 0xbc; index++)
    sum = (sum + data[index]) & 0xff;
  return data[0xbd] === ((-sum - 0x19) & 0xff);
}

export function publicationContentReason(data: Uint8Array): string | undefined {
  if (gbaImage(data)) return "GBA ROM image";
  if (
    data.length >= 4 &&
    data[0] === 0x7f &&
    data[1] === 0x45 &&
    data[2] === 0x4c &&
    data[3] === 0x46
  ) {
    return "ELF build product";
  }
  if (
    data.length >= 8 &&
    Buffer.from(data.subarray(0, 8)).toString("ascii") === "!<arch>\n"
  ) {
    return "archive or object library";
  }
  if (data.length >= 2 && data[0] === 0x4d && data[1] === 0x5a)
    return "native executable";
  if (data.length >= 4) {
    const magic = Buffer.from(data.subarray(0, 4)).readUInt32BE();
    if (
      [
        0xfeedface, 0xcefaedfe, 0xfeedfacf, 0xcffaedfe, 0xcafebabe, 0xbebafeca,
        0xcafebabf, 0xbfbafeca, 0x0061736d,
      ].includes(magic)
    ) {
      return "native executable";
    }
  }
  return undefined;
}

// Only the opening and closing markers are matched. A bare `=======` is a valid
// Markdown heading underline, so it is deliberately not matched.
const CONFLICT_MARKER = /^(?:<{7}|>{7}) /m;
const MARKER_EXTENSIONS = [
  "md",
  "ts",
  "js",
  "json",
  "sh",
  "c",
  "h",
  "s",
  "asm",
  "tsv",
  "txt",
];

export function conflictMarkerReason(
  path: string,
  data: Uint8Array,
): string | undefined {
  if (!MARKER_EXTENSIONS.includes(extension(path))) return undefined;
  const text = Buffer.from(data).toString();
  if (!CONFLICT_MARKER.test(text)) return undefined;
  const line =
    text.split("\n").findIndex((entry) => /^(?:<{7}|>{7}) /.test(entry)) + 1;
  return `unresolved conflict marker at line ${line}; resolve the merge before committing`;
}

export function publicationEntryReason(
  path: string,
  data: Uint8Array,
): string | undefined {
  const pathReason = publicationPathReason(path);
  if (pathReason !== undefined) return pathReason;
  if (
    ["asm", "s"].includes(extension(path)) &&
    /^\s*\.incbin\b/im.test(Buffer.from(data).toString())
  ) {
    return "committed incbin payload";
  }
  return publicationContentReason(data);
}

function stagedPaths(): string[] {
  const paths = nulList(
    output(
      git(["diff", "--cached", "--name-only", "--diff-filter=ACMRT", "-z"]),
      "staged path scan",
    ),
  );
  // Exclude submodules which are tracked as commit objects, not blobs.
  return paths.filter((path) => {
    const stage = output(
      git(["ls-files", "--stage", path]),
      "ls-files",
    ).toString();
    return stage && !stage.startsWith("160000");
  });
}

function stagedBlob(path: string): Buffer {
  return output(git(["show", `:${path}`]), `staged blob ${path}`);
}

function changedPaths(commit: string): string[] {
  const paths = nulList(
    output(
      git([
        "diff-tree",
        "--root",
        "--no-commit-id",
        "--name-only",
        "--diff-filter=ACMRT",
        "-r",
        "-z",
        commit,
      ]),
      `commit path scan ${commit}`,
    ),
  );
  return paths.filter((path) => {
    const lsTree = output(git(["ls-tree", commit, path]), "ls-tree").toString();
    return lsTree && !lsTree.includes(" commit ");
  });
}

function commitBlob(commit: string, path: string): Buffer {
  return output(
    git(["show", `${commit}:${path}`]),
    `commit blob ${commit}:${path}`,
  );
}

function reject(
  entries: Array<{ scope: string; path: string; data: () => Buffer }>,
): void {
  const failures: string[] = [];
  for (const entry of entries) {
    const pathReason = publicationPathReason(entry.path);
    if (pathReason !== undefined) {
      failures.push(`${entry.scope} ${entry.path}: ${pathReason}`);
      continue;
    }
    const contentReason = publicationEntryReason(entry.path, entry.data());
    if (contentReason !== undefined)
      failures.push(`${entry.scope} ${entry.path}: ${contentReason}`);
  }
  if (failures.length > 0)
    throw new Error(`publication gate rejected:\n${failures.join("\n")}`);
}

function checkStaged(): void {
  const entries = stagedPaths().map((path) => ({
    scope: "staged",
    path,
    data: () => stagedBlob(path),
  }));
  reject(entries);
  // Conflict markers are current-tree hygiene, so scan staged content only.
  // The publication scan over immutable outgoing history remains separate.
  const failures: string[] = [];
  for (const entry of entries) {
    const reason = conflictMarkerReason(entry.path, entry.data());
    if (reason !== undefined)
      failures.push(`${entry.scope} ${entry.path}: ${reason}`);
  }
  if (failures.length > 0)
    throw new Error(`publication gate rejected:\n${failures.join("\n")}`);
}

function revisions(local: string, remote: string): string[] {
  const args = ["rev-list", local];
  if (!ZERO_OID.test(remote)) args.push(`^${remote}`);
  return lines(output(git(args), `outgoing revision scan ${local}`));
}

// A run of space-separated hex byte pairs: someone pasted bytes.
const BYTE_DUMP = /(?:\b[0-9a-fA-F]{2}\b[ \t]+){7}\b[0-9a-fA-F]{2}\b/;

/**
 * Reject a commit MESSAGE that carries ROM bytes.
 *
 * Everything else in this file scans file blobs. Commit messages were never
 * scanned at all, so the gate passing said nothing about them -- and messages
 * are published to the remote exactly like file contents are.
 *
 * This deliberately checks one unambiguous thing: a run of space-separated hex
 * byte pairs, which is a pasted byte dump and never ordinary prose. It does
 * NOT try to detect quoted disassembly. A message legitimately discusses
 * instructions ("the bl displacement", "the prologue pushes r7"), and any
 * pattern loose enough to catch a pasted listing also blocks that, which would
 * push authors to work around the gate rather than with it. Where to draw that
 * line stays an author's judgement; this catches the case that is not a
 * judgement call.
 */
export function commitMessageReason(message: string): string | undefined {
  return BYTE_DUMP.test(message) ? "commit message contains a raw byte dump" : undefined;
}

function commitMessage(commit: string): string {
  return output(git(["log", "-1", "--format=%B", commit]), `commit message ${commit}`).toString();
}

async function checkPush(): Promise<void> {
  const updates = lines(Buffer.from(await Bun.stdin.text()));
  const commits = new Set<string>();
  for (const update of updates) {
    const fields = update.split(/\s+/);
    if (fields.length !== 4) throw new Error("invalid pre-push update");
    const [, local, , remote] = fields;
    if (ZERO_OID.test(local)) continue;
    for (const commit of revisions(local, remote)) commits.add(commit);
  }
  const messageFailures: string[] = [];
  for (const commit of commits) {
    const reason = commitMessageReason(commitMessage(commit));
    if (reason !== undefined) messageFailures.push(`${commit.slice(0, 12)}: ${reason}`);
  }
  if (messageFailures.length > 0) {
    for (const failure of messageFailures) console.error(failure);
    throw new Error(`refusing to publish ${messageFailures.length} commit message(s)`);
  }
  const entries: Array<{ scope: string; path: string; data: () => Buffer }> =
    [];
  for (const commit of commits) {
    for (const path of changedPaths(commit)) {
      entries.push({
        scope: commit.slice(0, 12),
        path,
        data: () => commitBlob(commit, path),
      });
    }
  }
  reject(entries);
}

function mustRejectPath(path: string): void {
  if (publicationPathReason(path) === undefined)
    throw new Error(`private path accepted: ${path}`);
}

function mustAcceptPath(path: string): void {
  const reason = publicationPathReason(path);
  if (reason !== undefined)
    throw new Error(`source path rejected: ${path}: ${reason}`);
}

function selfTest(): void {
  for (const path of [
    "gs1-en.gba",
    "gs1-ja.gba",
    "gs1-de.gba",
    "gs1-es.gba",
    "gs1-fr.gba",
    "gs1-it.gba",
    "gs2-en.gba",
    "gs2-ja.gba",
    "gs2-de.gba",
    "gs2-es.gba",
    "gs2-fr.gba",
    "gs2-it.gba",
    "roms/private/gs1-en.gba",
    "out/diff.json",
    "work/rom.raw",
    "alchemy-gcc/bin/compiler",
    "analysis/regions.json",
    "reports/comparison.json",
    "dump.bin",
    "private-diff.json",
    "game.elf",
    "gs1-en.gba.lz",
    "regional.patch",
    "engine.bsdiff",
    ".cmatch-fresh/result.s",
    "comparisons/shared-runs.json",
    "compiler-output/function.s",
  ])
    mustRejectPath(path);
  for (const path of [
    "src/main.c",
    "asm/080000c0.s",
    "assets/graphics/title.png",
    "assets/audio/theme.mid",
    "assets/audio/wave.wav",
    "assets/data/layout.json",
    "tools/metrics/compare_roms.ts",
    "tools/make/build_full.ts",
    "assets/data/resource_2_build_stamp.txt",
    "assets/maps/town/metatiles.bin",
    "assets/maps/town/metatile_attributes.bin",
    "rom.sha1",
  ])
    mustAcceptPath(path);
  const rom = Buffer.alloc(0x8000);
  rom[0xb2] = 0x96;
  let sum = 0;
  for (let index = 0xa0; index <= 0xbc; index++)
    sum = (sum + rom[index]) & 0xff;
  rom[0xbd] = (-sum - 0x19) & 0xff;
  if (publicationContentReason(rom) !== "GBA ROM image")
    throw new Error("disguised GBA ROM was accepted");
  if (
    publicationContentReason(Buffer.from([0x7f, 0x45, 0x4c, 0x46])) !==
    "ELF build product"
  ) {
    throw new Error("disguised ELF was accepted");
  }
  if (
    publicationContentReason(Buffer.from("!<arch>\n")) !==
    "archive or object library"
  ) {
    throw new Error("disguised archive was accepted");
  }
  if (publicationContentReason(Buffer.from("canonical source")) !== undefined) {
    throw new Error("ordinary source content was rejected");
  }
  if (
    publicationEntryReason(
      "asm/08000000.s",
      Buffer.from('.incbin "rom.gba"\n'),
    ) !== "committed incbin payload"
  ) {
    throw new Error("committed incbin payload was accepted");
  }

  const bytes = (text: string): Uint8Array => new TextEncoder().encode(text);
  if (
    conflictMarkerReason("HANDOVER.md", bytes("a\n<<<<<<< HEAD\nb\n")) ===
    undefined
  ) {
    throw new Error("an opening conflict marker was accepted");
  }
  if (
    conflictMarkerReason("HANDOVER.md", bytes("a\n>>>>>>> topic\n")) ===
    undefined
  ) {
    throw new Error("a closing conflict marker was accepted");
  }
  // The shared entry reason must not flag markers because it also scans
  // immutable outgoing history.
  if (
    publicationEntryReason("HANDOVER.md", bytes("x\n<<<<<<< HEAD\n")) !==
    undefined
  ) {
    throw new Error("the history-facing gate flagged a conflict marker");
  }
  // A bare ======= is a Markdown heading underline, not a conflict.
  if (
    conflictMarkerReason("HANDOVER.md", bytes("Title\n=======\n\nbody\n")) !==
    undefined
  ) {
    throw new Error(
      "a Markdown heading underline was rejected as a conflict marker",
    );
  }
  // <<<<<<< without the trailing space is ordinary prose or a diff sample.
  if (
    conflictMarkerReason(
      "HANDOVER.md",
      bytes("see <<<<<<<HEAD in the output\n"),
    ) !== undefined
  ) {
    throw new Error("a marker-like string without the separator was rejected");
  }
  if (
    conflictMarkerReason("assets/readme/x.png", bytes("<<<<<<< HEAD\n")) !==
    undefined
  ) {
    throw new Error("a binary extension was scanned for conflict markers");
  }

  // A pasted byte dump in a commit message is the case this catches.
  if (
    commitMessageReason("fixed the header\n\n00 11 22 33 44 55 66 77\n") ===
    undefined
  ) {
    throw new Error("a byte dump in a commit message was accepted");
  }
  // Everything below is prose a real commit message contains, and blocking any
  // of it would push authors around the gate instead of through it.
  for (const accepted of [
    "Close 12 owners the sweep left open\n",
    // A full SHA is 40 hex characters and appears in ordinary messages.
    "reverts 3d36cfb0aa11bb22cc33dd44ee55ff6677889900\n",
    // Addresses and spans are how these commits describe their own work.
    "resource_39b:e6c span 0x02000e6c..0x02000e78 is not audited\n",
    // Discussing instructions must stay allowed; only pasted BYTES are not.
    "the prologue pushes r7 where the reference does not\n",
    // Two hex pairs in a row are a byte pair, not a dump.
    "the low halfword ff 00 stayed wrong\n",
  ]) {
    if (commitMessageReason(accepted) !== undefined) {
      throw new Error(`a legitimate commit message was rejected: ${accepted.trim()}`);
    }
  }
}

async function main(args: string[]): Promise<void> {
  if (args.length !== 1)
    throw new Error(
      "usage: check_publication.ts {--staged|--pre-push|--self-test}",
    );
  if (args[0] === "--staged") checkStaged();
  else if (args[0] === "--pre-push") await checkPush();
  else if (args[0] === "--self-test") {
    selfTest();
    console.log("self-test=ok");
  } else
    throw new Error(
      "usage: check_publication.ts {--staged|--pre-push|--self-test}",
    );
}

if (import.meta.main) await main(Bun.argv.slice(2));
