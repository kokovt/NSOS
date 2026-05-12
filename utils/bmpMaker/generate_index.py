#!/usr/bin/env python3
"""
Optional maintainer helper: wipes and rebuilds bmpMaker/index.html.

Reads src/kernel/c/fonts/basicFont.c and rewrites the huge GLYPH_COMMENT +
INITIAL_BYTES arrays inside index.html so the web editor matches the repo
after you edit that C file.

You do NOT need this script just to edit fonts in the browser — only run
    python3 bmpMaker/generate_index.py
when basicFont.c’s comments or default bytes change and you want the HTML
embedding updated too.
"""
import json
import re

path_c = "/home/koko/dev/NSOS/src/kernel/c/fonts/basicFont.c"
path_out = "/home/koko/dev/NSOS/bmpMaker/index.html"

text = open(path_c, encoding="utf-8").read()
pat = re.compile(
    r"\{\s*((?:0x[0-9A-Fa-f]{2},\s*)+0x[0-9A-Fa-f]{2})\s*\},\s*//\s*(.+?)\s*$",
    re.MULTILINE,
)
rows = pat.findall(text)
assert len(rows) == 224, len(rows)

comments = [r[1].strip() for r in rows]
hex_rows = []
for h, _ in rows:
    nums = [int(x, 16) for x in re.findall(r"0x[0-9A-Fa-f]{2}", h)]
    hex_rows.append(nums)

js_comments = json.dumps(comments, ensure_ascii=False)
js_bytes = json.dumps(hex_rows)

# Use \n in generated JavaScript as two-char sequence backslash + n
NL = "\\n"

PART1 = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>NSOS font editor (224 glyphs)</title>
  <style>
    :root {
      --cell: 28px;
      --ink: #1a1a2e;
      --paper: #f4f4f8;
      --accent: #4361ee;
      --border: #c8cad4;
    }
    * { box-sizing: border-box; }
    body {
      font-family: system-ui, sans-serif;
      margin: 0;
      padding: 1.25rem;
      background: #e8e9ef;
      color: #222;
      line-height: 1.45;
    }
    h1 { font-size: 1.25rem; margin: 0 0 0.5rem; }
    .sub { font-size: 0.875rem; color: #555; margin-bottom: 1rem; }
    .layout {
      display: flex;
      flex-wrap: wrap;
      gap: 1.5rem;
      align-items: flex-start;
    }
    .panel {
      background: #fff;
      border-radius: 10px;
      padding: 1rem;
      box-shadow: 0 1px 3px rgba(0,0,0,0.08);
    }
    .toolbar {
      display: flex;
      flex-wrap: wrap;
      gap: 0.5rem;
      margin-bottom: 0.75rem;
      align-items: center;
    }
    .toolbar label { font-size: 0.85rem; display: flex; align-items: center; gap: 0.35rem; }
    .toolbar input[type="number"] { width: 3.25rem; }
    .toolbar select { min-width: 14rem; max-width: 100%; font-size: 0.85rem; }
    button.tool {
      padding: 0.45rem 0.85rem;
      font-size: 0.9rem;
      border: 1px solid var(--border);
      border-radius: 6px;
      background: #fff;
      cursor: pointer;
    }
    button.tool:hover { background: #f0f1f5; }
    button.tool:focus-visible {
      outline: 2px solid var(--accent);
      outline-offset: 2px;
    }
    #grid {
      display: grid;
      gap: 0;
      user-select: none;
      touch-action: none;
    }
    #grid button.cell {
      width: var(--cell);
      height: var(--cell);
      padding: 0;
      border: 2px solid var(--border);
      border-radius: 0;
      background: var(--paper);
      cursor: pointer;
    }
    #grid button.cell.on {
      background: var(--ink);
      border-color: var(--ink);
    }
    #grid button.cell:focus-visible {
      outline: 3px solid var(--accent);
      outline-offset: 2px;
      z-index: 1;
    }
    .outputs h2 {
      font-size: 0.95rem;
      margin: 0 0 0.35rem;
    }
    .out-block {
      position: relative;
      margin-bottom: 1rem;
    }
    .out-block:last-child { margin-bottom: 0; }
    .out-head {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 0.5rem;
      flex-wrap: wrap;
    }
    pre.out {
      margin: 0.35rem 0 0;
      padding: 0.65rem 0.75rem;
      background: #1e1e2e;
      color: #e8e8f0;
      border-radius: 6px;
      font-size: 0.75rem;
      overflow-x: auto;
      white-space: pre;
      font-family: ui-monospace, "Cascadia Code", monospace;
      max-height: 22rem;
    }
    pre.out.tall { max-height: 36rem; }
    button.copy {
      font-size: 0.75rem;
      padding: 0.25rem 0.5rem;
      border-radius: 4px;
      border: 1px solid var(--border);
      background: #fff;
      cursor: pointer;
    }
    button.copy:hover { background: #f0f1f5; }
    .hint { font-size: 0.8rem; color: #666; margin-top: 0.75rem; }
  </style>
</head>
<body>
  <h1>NSOS bitmap font editor</h1>
  <p class="sub">224 glyphs (same order as <code>basicFont.c</code>). <strong>FONT_BPL = ⌈width / 8⌉</strong> (extra bytes per row if width &gt; 8, e.g. 10-wide → <code>BPL=2</code>, 10×10 → <code>BPG=20</code>). Max 32×32 here. Packing matches kernel <code>drawChar</code> per 8-pixel chunk: <strong>bit 0 = left</strong>; enable “Legacy MSB-left” for old one-byte bmpMaker rows. Dimensions apply on spinners or Apply.</p>

  <div class="layout">
    <div class="panel">
      <div class="toolbar">
        <label title="Columns per glyph (1–32); BPL = ceil(cols / 8)">Cols <input type="number" id="fontW" min="1" max="32" step="1" value="8" /></label>
        <label title="Rows per glyph">Rows <input type="number" id="fontH" min="1" max="32" step="1" value="8" /></label>
        <button type="button" class="tool" id="applyDimBtn">Apply size</button>
        <label title="Match original bmpMaker / historical glyph bytes (MSB = left)">
          <input type="checkbox" id="legacyMsb" checked /> Legacy MSB-left
        </label>
      </div>
      <div class="toolbar">
        <label>Glyph
          <select id="glyphSelect" aria-label="Current glyph index"></select>
        </label>
      </div>
      <div class="toolbar">
        <button type="button" class="tool" id="clearBtn">Clear glyph</button>
        <button type="button" class="tool" id="invertBtn">Invert glyph</button>
        <button type="button" class="tool" id="clearAllBtn">Clear all glyphs</button>
      </div>
      <div id="grid" role="grid" aria-label="pixel grid"></div>
      <p class="hint" id="bitHint"></p>
    </div>

    <div class="panel outputs" style="flex: 1; min-width: min(100%, 320px); max-width: 640px;">
      <div class="out-block">
        <div class="out-head">
          <h2>Binary (current glyph)</h2>
          <button type="button" class="copy" data-copy="binary">Copy</button>
        </div>
        <pre class="out" id="outBinary" aria-live="polite"></pre>
      </div>
      <div class="out-block">
        <div class="out-head">
          <h2>Hex (current glyph)</h2>
          <button type="button" class="copy" data-copy="hex">Copy</button>
        </div>
        <pre class="out" id="outHex" aria-live="polite"></pre>
      </div>
      <div class="out-block">
        <div class="out-head">
          <h2>C initializer (current glyph)</h2>
          <button type="button" class="copy" data-copy="c">Copy</button>
        </div>
        <pre class="out" id="outC" aria-live="polite"></pre>
      </div>
      <div class="out-block">
        <div class="out-head">
          <h2>basic_font.h (example)</h2>
          <button type="button" class="copy" data-copy="header">Copy</button>
        </div>
        <pre class="out" id="outHeader" aria-live="polite"></pre>
      </div>
      <div class="out-block">
        <div class="out-head">
          <h2>basicFont.c (full array)</h2>
          <button type="button" class="copy" data-copy="fullc">Copy</button>
        </div>
        <pre class="out tall" id="outFullC" aria-live="polite"></pre>
      </div>
    </div>
  </div>

  <textarea id="clipFallback" aria-hidden="true" style="position:fixed;left:-9999px;top:0;opacity:0;height:0"></textarea>

  <script>
"""

# Inline script (placeholders substituted below)
_SCRIPT = r'''
(function () {
  const NUM_GLYPHS = 224;
  const GLYPH_COMMENT = __COMMENTS__;
  const INITIAL_BYTES = __BYTES__;

  function displayNameFromComment(comment) {
    var m = /\(([^)]*)\)/.exec(comment);
    return m ? m[1].trim() : "NULLCHAR";
  }

  function emptyGrid(h, w) {
    return Array.from({ length: h }, function () { return Array(w).fill(false); });
  }

  function resizeGrid(oldG, oldH, oldW, newH, newW) {
    var g = emptyGrid(newH, newW);
    var copyH = Math.min(oldH, newH);
    var copyW = Math.min(oldW, newW);
    var r, c;
    for (r = 0; r < copyH; r++) {
      for (c = 0; c < copyW; c++) {
        g[r][c] = oldG[r][c];
      }
    }
    return g;
  }

  function bytesPerLine(w) {
    return Math.max(1, Math.ceil(w / 8));
  }

  function unpackRowMulti(chunk, w, legacyMsb) {
    var row = Array(w).fill(false);
    var c;
    for (c = 0; c < w; c++) {
      var bi = c >> 3;
      var k = c & 7;
      var bit = legacyMsb ? (7 - k) : k;
      if (bi < chunk.length) {
        row[c] = ((chunk[bi] >> bit) & 1) !== 0;
      }
    }
    return row;
  }

  function packRowMulti(row, w, legacyMsb) {
    var bpl = bytesPerLine(w);
    var chunk = [];
    var bi;
    for (bi = 0; bi < bpl; bi++) chunk.push(0);
    var c;
    for (c = 0; c < w; c++) {
      if (!row[c]) continue;
      var byteIdx = c >> 3;
      var k = c & 7;
      var bit = legacyMsb ? (7 - k) : k;
      chunk[byteIdx] |= (1 << bit) & 0xff;
    }
    return chunk;
  }

  function glyphFromBytes(bytes, w, h, legacyMsb) {
    var bpl = bytesPerLine(w);
    var g = emptyGrid(h, w);
    var r;
    for (r = 0; r < h; r++) {
      var slice = [];
      var k;
      for (k = 0; k < bpl; k++) {
        var idx = r * bpl + k;
        slice.push(idx < bytes.length ? bytes[idx] : 0);
      }
      var bits = unpackRowMulti(slice, w, legacyMsb);
      var c;
      for (c = 0; c < w; c++) g[r][c] = bits[c];
    }
    return g;
  }

  function bytesFromGlyph(grid, h, w, legacyMsb) {
    var flat = [];
    var r;
    for (r = 0; r < h; r++) {
      var chunk = packRowMulti(grid[r], w, legacyMsb);
      flat = flat.concat(chunk);
    }
    return flat;
  }

  var fontWidth = 8;
  var fontHeight = 8;
  var legacyMsb = true;
  var glyphs = [];

  function initGlyphsFromEmbedded() {
    var i;
    for (i = 0; i < NUM_GLYPHS; i++) {
      glyphs.push(glyphFromBytes(INITIAL_BYTES[i], fontWidth, fontHeight, true));
    }
  }

  initGlyphsFromEmbedded();

  var currentIndex = 0;
  var cells = glyphs[0];

  var gridEl = document.getElementById("grid");
  var glyphSelect = document.getElementById("glyphSelect");
  var legacyEl = document.getElementById("legacyMsb");
  var fontWEl = document.getElementById("fontW");
  var fontHEl = document.getElementById("fontH");
  var bitHint = document.getElementById("bitHint");

  function readBoundedInt(inputEl, minV, maxV, fallback) {
    var v = parseInt(String(inputEl.value).trim(), 10);
    if (!isFinite(v)) return fallback;
    return Math.max(minV, Math.min(maxV, v));
  }

  function normalizeDimInputsApply() {
    var nw = readBoundedInt(fontWEl, 1, 32, fontWidth);
    var nh = readBoundedInt(fontHEl, 1, 32, fontHeight);
    fontWEl.value = String(nw);
    fontHEl.value = String(nh);
    applyDimensionChange(nw, nh);
  }

  var outBinary = document.getElementById("outBinary");
  var outHex = document.getElementById("outHex");
  var outC = document.getElementById("outC");
  var outHeader = document.getElementById("outHeader");
  var outFullC = document.getElementById("outFullC");
  var clipFallback = document.getElementById("clipFallback");

  var buttons = [];

  function formatHexByte(b) {
    return "0x" + b.toString(16).toUpperCase().padStart(2, "0");
  }

  function formatBinaryByte(b) {
    var s = "0b";
    var bit;
    for (bit = 7; bit >= 0; bit--) {
      s += (b >> bit) & 1 ? "1" : "0";
    }
    return s;
  }

  function refreshOutputs() {
    var h = fontHeight;
    var w = fontWidth;
    var bpl = bytesPerLine(w);
    var flat = bytesFromGlyph(cells, h, w, legacyMsb);
    var nl = String.fromCharCode(10);

    var binLines = [];
    var hexLines = [];
    var cLines = [];
    var r;
    var k;
    for (r = 0; r < h; r++) {
      var rowComma = r < h - 1 ? "," : "";
      var binChunks = [];
      var cchunks = [];
      for (k = 0; k < bpl; k++) {
        var bv = flat[r * bpl + k];
        var bcm = k < bpl - 1 ? ", " : "";
        binChunks.push(formatBinaryByte(bv) + bcm + " ");
        cchunks.push(formatHexByte(bv));
      }
      binLines.push(
        "// row " + r + ": " +
        binChunks.join("").trim() +
        rowComma +
        " (" + bpl + " byte" + (bpl !== 1 ? "s" : "") + "/row × " +
        (legacyMsb ? "MSB" : "LSB") + "-left per chunk)",
      );
      hexLines.push(cchunks.join(", ") + rowComma + " // row " + r);
      cLines.push(cchunks.join(", ") + rowComma);
    }

    outBinary.textContent = "[" + nl + binLines.join(nl) + nl + "]";
    outHex.textContent = "[" + nl + hexLines.join(nl) + nl + "]";
    outC.textContent = "{" + nl + cLines.join(nl) + nl + "}";

    outHeader.textContent = [
      "#ifndef _BASIC_FONT_H",
      "#define _BASIC_FONT_H",
      "",
      "enum {",
      "    FONT_WIDTH     = " + w + ",",
      "    FONT_HEIGHT    = " + h + ",",
      "    FONT_BPL       = " + bpl + ",  // Bytes per line (⌈width / 8⌉)",
      "    FONT_BPG       = FONT_HEIGHT * FONT_BPL,  // Bytes per glyph",
      "    FONT_NUMGLYPHS = " + NUM_GLYPHS,
      "};",
      "",
      "extern unsigned char font[FONT_NUMGLYPHS][FONT_BPG];",
      "",
      "#endif"
    ].join(nl);

    var cBody = [];
    var i;
    for (i = 0; i < NUM_GLYPHS; i++) {
      var bytes = bytesFromGlyph(glyphs[i], h, w, legacyMsb);
      var parts = bytes.map(formatHexByte);
      cBody.push("    { " + parts.join(", ") + "},   // " + GLYPH_COMMENT[i]);
    }
    outFullC.textContent =
      nl + nl + "#include <fonts/basic_font.h>" + nl + nl +
      "unsigned char font[FONT_NUMGLYPHS][FONT_BPG] = {" + nl +
      cBody.join(nl) +
      nl + "};";

    bitHint.textContent = legacyMsb
      ? "MSB-left per 8-pixel chunk (legacy bmpMaker). First column in each chunk → 0b10000000."
      : "LSB-left per 8-pixel chunk (kernel drawChar). Column 0 of each chunk → 0b00000001.";
  }

  function syncButton(r, c) {
    var row = buttons[r];
    if (!row) return;
    var btn = row[c];
    if (!btn) return;
    btn.classList.toggle("on", cells[r][c]);
    btn.setAttribute("aria-pressed", cells[r][c] ? "true" : "false");
  }

  function setCell(r, c, on) {
    if (cells[r][c] === on) return;
    cells[r][c] = on;
    syncButton(r, c);
    refreshOutputs();
  }

  function toggleCell(r, c) {
    setCell(r, c, !cells[r][c]);
  }

  function rebuildGridDom() {
    gridEl.innerHTML = "";
    buttons = [];
    var h = fontHeight;
    var w = fontWidth;
    gridEl.style.gridTemplateColumns = "repeat(" + w + ", var(--cell))";
    gridEl.style.gridTemplateRows = "repeat(" + h + ", var(--cell))";
    gridEl.style.width = "calc(" + w + " * var(--cell))";
    var r, c;
    for (r = 0; r < h; r++) {
      buttons[r] = [];
      for (c = 0; c < w; c++) {
        var btn = document.createElement("button");
        btn.type = "button";
        btn.className = "cell";
        btn.setAttribute("role", "gridcell");
        btn.setAttribute("aria-rowindex", String(r + 1));
        btn.setAttribute("aria-colindex", String(c + 1));
        btn.setAttribute("aria-pressed", cells[r][c] ? "true" : "false");
        if (cells[r][c]) btn.classList.add("on");
        btn.dataset.r = String(r);
        btn.dataset.c = String(c);
        gridEl.appendChild(btn);
        buttons[r][c] = btn;
      }
    }
  }

  function parseRC(el) {
    var r = parseInt(el.dataset.r, 10);
    var c = parseInt(el.dataset.c, 10);
    if (r >= 0 && r < fontHeight && c >= 0 && c < fontWidth) return { r: r, c: c };
    return null;
  }

  function fillGlyphSelect() {
    glyphSelect.innerHTML = "";
    var i;
    for (i = 0; i < NUM_GLYPHS; i++) {
      var opt = document.createElement("option");
      opt.value = String(i);
      var name = displayNameFromComment(GLYPH_COMMENT[i]);
      var u = GLYPH_COMMENT[i].split(" ")[0] || "";
      opt.textContent = String(i).padStart(3, "0") + " — " + name + " — " + u;
      glyphSelect.appendChild(opt);
    }
    glyphSelect.value = String(currentIndex);
  }

  function applyDimensionChange(newW, newH) {
    newW = Math.max(1, Math.min(32, isFinite(newW) ? newW : fontWidth));
    newH = Math.max(1, Math.min(32, isFinite(newH) ? newH : fontHeight));
    if (newW === fontWidth && newH === fontHeight) {
      rebuildGridDom();
      refreshOutputs();
      return;
    }
    var i;
    for (i = 0; i < NUM_GLYPHS; i++) {
      glyphs[i] = resizeGrid(glyphs[i], fontHeight, fontWidth, newH, newW);
    }
    fontWidth = newW;
    fontHeight = newH;
    cells = glyphs[currentIndex];
    fontWEl.value = String(fontWidth);
    fontHEl.value = String(fontHeight);
    rebuildGridDom();
    refreshOutputs();
  }

  function setCurrentIndex(next) {
    next = Math.max(0, Math.min(NUM_GLYPHS - 1, next));
    currentIndex = next;
    cells = glyphs[currentIndex];
    glyphSelect.value = String(currentIndex);
    rebuildGridDom();
    refreshOutputs();
  }

  fillGlyphSelect();
  rebuildGridDom();

  var painting = false;
  var paintValue = false;

  function paintAtClient(x, y) {
    var el = document.elementFromPoint(x, y);
    if (!(el instanceof HTMLElement) || !el.classList.contains("cell")) return;
    var rc = parseRC(el);
    if (!rc) return;
    setCell(rc.r, rc.c, paintValue);
  }

  gridEl.addEventListener("pointerdown", function (e) {
    var t = e.target;
    if (!(t instanceof HTMLElement) || !t.classList.contains("cell")) return;
    if (e.button !== 0 && e.pointerType === "mouse") return;
    e.preventDefault();
    gridEl.setPointerCapture(e.pointerId);
    var rc = parseRC(t);
    if (!rc) return;
    painting = true;
    paintValue = !cells[rc.r][rc.c];
    setCell(rc.r, rc.c, paintValue);
  });

  gridEl.addEventListener("pointermove", function (e) {
    if (!painting) return;
    paintAtClient(e.clientX, e.clientY);
  });

  function endPaint() {
    painting = false;
  }

  gridEl.addEventListener("pointerup", endPaint);
  gridEl.addEventListener("pointercancel", endPaint);
  gridEl.addEventListener("lostpointercapture", endPaint);

  gridEl.addEventListener("keydown", function (e) {
    var t = e.target;
    if (!(t instanceof HTMLElement) || !t.classList.contains("cell")) return;
    if (e.key !== " " && e.key !== "Enter") return;
    e.preventDefault();
    var rc = parseRC(t);
    if (!rc) return;
    toggleCell(rc.r, rc.c);
  });

  glyphSelect.addEventListener("change", function () {
    setCurrentIndex(parseInt(glyphSelect.value, 10));
  });

  legacyEl.addEventListener("change", function () {
    legacyMsb = legacyEl.checked;
    refreshOutputs();
  });

  document.getElementById("applyDimBtn").addEventListener("click", normalizeDimInputsApply);
  fontWEl.addEventListener("change", normalizeDimInputsApply);
  fontHEl.addEventListener("change", normalizeDimInputsApply);
  fontWEl.addEventListener("keydown", function (e) {
    if (e.key === "Enter") { e.preventDefault(); normalizeDimInputsApply(); }
  });
  fontHEl.addEventListener("keydown", function (e) {
    if (e.key === "Enter") { e.preventDefault(); normalizeDimInputsApply(); }
  });

  document.getElementById("clearBtn").addEventListener("click", function () {
    var r, c;
    for (r = 0; r < fontHeight; r++) {
      for (c = 0; c < fontWidth; c++) {
        cells[r][c] = false;
        syncButton(r, c);
      }
    }
    refreshOutputs();
  });

  document.getElementById("invertBtn").addEventListener("click", function () {
    var r, c;
    for (r = 0; r < fontHeight; r++) {
      for (c = 0; c < fontWidth; c++) {
        cells[r][c] = !cells[r][c];
        syncButton(r, c);
      }
    }
    refreshOutputs();
  });

  document.getElementById("clearAllBtn").addEventListener("click", function () {
    if (!confirm("Clear all " + NUM_GLYPHS + " glyphs?")) return;
    var gi;
    for (gi = 0; gi < NUM_GLYPHS; gi++) {
      glyphs[gi] = emptyGrid(fontHeight, fontWidth);
    }
    cells = glyphs[currentIndex];
    rebuildGridDom();
    refreshOutputs();
  });

  function copyText(text) {
    if (navigator.clipboard && navigator.clipboard.writeText) {
      return navigator.clipboard.writeText(text).then(function () { return true; }).catch(function () { return fallbackCopy(text); });
    }
    return Promise.resolve(fallbackCopy(text));
  }

  function fallbackCopy(text) {
    clipFallback.value = text;
    clipFallback.select();
    try {
      document.execCommand("copy");
      return true;
    } catch (_) {
      return false;
    }
  }

  document.querySelectorAll("button.copy").forEach(function (btn) {
    btn.addEventListener("click", function () {
      var kind = btn.getAttribute("data-copy");
      var tx = "";
      if (kind === "binary") tx = outBinary.textContent;
      else if (kind === "hex") tx = outHex.textContent;
      else if (kind === "c") tx = outC.textContent;
      else if (kind === "header") tx = outHeader.textContent;
      else if (kind === "fullc") tx = outFullC.textContent;
      copyText(tx);
    });
  });

  refreshOutputs();
})();
'''

script_core = (
    _SCRIPT.replace("__COMMENTS__", js_comments)
    .replace("__BYTES__", js_bytes)
)

OUT = PART1 + script_core + "\n  </script>\n</body>\n</html>\n"
open(path_out, "w", encoding="utf-8").write(OUT)
print("Wrote", path_out, "bytes", len(OUT))
