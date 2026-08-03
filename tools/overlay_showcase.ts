#!/usr/bin/env bun
// Prove that a named showcase overlay is closed, descriptively named, and
// still reconstructs the reviewed decoded image byte for byte.

import { existsSync, readFileSync, readdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { assembleOverlay } from "./overlay_disasm.ts";
import { canonicalCSource } from "./full_c_progress.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));

interface Showcase {
  id: string;
  decoded_bytes: number;
  exact_c_owners: number;
  sha256: string;
  retained_assembly: string;
}

function checked(command: string[]): void {
  const result = Bun.spawnSync(command, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
  if (result.exitCode !== 0) {
    throw new Error(`${command.slice(1).join(" ")} failed:\n${result.stderr}${result.stdout}`);
  }
}

function main(id: string): void {
  if (!/^resource_[0-9a-f]+$/.test(id)) {
    throw new Error("usage: overlay_showcase.ts resource_NNN");
  }
  const manifest = JSON.parse(readFileSync(join(ROOT, "metrics/overlay-showcases.json"), "utf8")) as {
    overlays: Showcase[];
  };
  const showcase = manifest.overlays.find((row) => row.id === id);
  if (showcase === undefined) throw new Error(`${id} is not a registered showcase overlay`);

  const semantic = join(ROOT, "semantic/overlays");
  const semanticOwners = existsSync(semantic)
    ? readdirSync(semantic).filter((name) => name.startsWith(`${id}_c_`) && name.endsWith(".c"))
    : [];
  if (semanticOwners.length !== 0) {
    throw new Error(`${id} still has semantic owners: ${semanticOwners.join(", ")}`);
  }

  const code = join(ROOT, "assets/code");
  const exactOwners = readdirSync(code).filter((name) => name.startsWith(`${id}_c_`) && name.endsWith(".c"));
  if (exactOwners.length !== showcase.exact_c_owners) {
    throw new Error(`${id} exact owner count is ${exactOwners.length}, expected ${showcase.exact_c_owners}`);
  }
  for (const name of exactOwners) {
    const source = readFileSync(join(code, name), "utf8");
    if (!canonicalCSource(source)) {
      throw new Error(`${name} uses inline assembly or register-pinned fakematch and is not canonical Exact C`);
    }
    const header = id.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    if (!new RegExp(`^#include\\s+"${header}\\.h"`, "m").test(source)) {
      throw new Error(`${name} does not use the showcase overlay's named interface`);
    }
  }

  const image = assembleOverlay(join(code, `${id}_overlay.s`));
  const digest = new Bun.CryptoHasher("sha256").update(image).digest("hex");
  if (image.length !== showcase.decoded_bytes || digest !== showcase.sha256) {
    throw new Error(
      `${id} decoded image changed: bytes=${image.length}/${showcase.decoded_bytes} sha256=${digest}/${showcase.sha256}`,
    );
  }

  checked([process.execPath, "tools/overlay_published.ts", id]);
  checked([process.execPath, "tools/overlay_gaps.ts", id]);
  checked([process.execPath, "tools/overlay_certify.ts", id]);
  checked([process.execPath, "tools/overlay_driver.ts", id]);

  console.log(
    `showcase=complete overlay=${id} exact_c_owners=${exactOwners.length} ` +
      `decoded_bytes=${image.length} sha256=${digest} retained_assembly=${showcase.retained_assembly}`,
  );
}

if (import.meta.main) {
  const requested = Bun.argv[2] ?? "";
  if (requested === "--all") {
    const manifest = JSON.parse(readFileSync(join(ROOT, "metrics/overlay-showcases.json"), "utf8")) as {
      overlays: Showcase[];
    };
    for (const overlay of manifest.overlays) main(overlay.id);
  } else main(requested);
}
