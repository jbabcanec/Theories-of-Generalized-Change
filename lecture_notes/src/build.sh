#!/bin/bash
# Build script for MATH241 Fall 2026 Worksheets
# Generates both filled and blank versions of each worksheet
#
# Usage: bash build.sh [worksheet_number]
#   No args: build all worksheets
#   With arg: build only that worksheet (e.g., bash build.sh 01)

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

build_worksheet() {
    local texfile="$1"
    local basename="${texfile%.tex}"

    echo "=== Building: $basename ==="

    # --- FILLED VERSION ---
    # Uncomment the \worksheetfilledtrue line
    sed 's/^%\\worksheetfilledtrue/\\worksheetfilledtrue/' "$texfile" > "${basename}_filled_tmp.tex"
    pdflatex -interaction=nonstopmode -output-directory=filled "${basename}_filled_tmp.tex" > /dev/null 2>&1
    pdflatex -interaction=nonstopmode -output-directory=filled "${basename}_filled_tmp.tex" > /dev/null 2>&1
    # Rename output
    if [ -f "filled/${basename}_filled_tmp.pdf" ]; then
        mv "filled/${basename}_filled_tmp.pdf" "filled/${basename}.pdf"
        echo "  [OK] filled/${basename}.pdf"
    else
        echo "  [FAIL] filled version"
    fi
    rm -f "${basename}_filled_tmp.tex"
    rm -f filled/${basename}_filled_tmp.{aux,log,out,toc}

    # --- BLANK VERSION ---
    # The default has \worksheetfilledtrue commented out, so just compile as-is
    pdflatex -interaction=nonstopmode -output-directory=blank "$texfile" > /dev/null 2>&1
    pdflatex -interaction=nonstopmode -output-directory=blank "$texfile" > /dev/null 2>&1
    if [ -f "blank/${basename}.pdf" ]; then
        echo "  [OK] blank/${basename}.pdf"
    else
        echo "  [FAIL] blank version"
    fi
    rm -f blank/${basename}.{aux,log,out,toc}
}

if [ -n "$1" ]; then
    # Build specific worksheet
    for f in ${1}*.tex; do
        [ -f "$f" ] && build_worksheet "$f"
    done
else
    # Build all worksheets
    for f in [0-9][0-9]_*.tex; do
        build_worksheet "$f"
    done
fi

echo ""
echo "Done! Check filled/ and blank/ directories."
