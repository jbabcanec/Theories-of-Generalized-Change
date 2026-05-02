# Theories of Generalized Change: A Calculus III Textbook

A complete Calculus III textbook, lecture-note workbook, and solutions manual written by **Joseph Babcanec** for **MATH 241** at **Benedict College** (Columbia, SC).

The book is the source of truth for everything in the course: the chapter content, the in-class worksheets that mirror it, the homework problem sets, and the syllabus pacing. The same materials are used across both Spring and Fall sections.

## Repository Contents

```
.
├── main.tex, intro.tex, preface.tex, appendix.tex, refs.tex   # Textbook glue
├── chapters/                          # Seven textbook chapters + per-chapter code/figures
├── solutions_manual/                  # Per-chapter worked solutions + main_solutions.pdf
├── lecture_notes/                     # 23 in-class worksheets (src/, blank/, filled/)
│   ├── src/                           # LaTeX sources + worksheet_style.sty + pics/
│   ├── blank/                         # Student handouts (problems only)
│   ├── filled/                        # Instructor versions (with worked answers)
│   ├── MATH241_Lecture_Notes_Blank.pdf
│   └── MATH241_Lecture_Notes_Filled.pdf
├── pics/                              # Cover art and shared images
├── Theories of Generalized Change.pdf # Compiled textbook (~16 MB, 700+ pp)
└── README.md, LICENSE
```

The working folders `audit/`, `problems_review/`, and `standards/` are intentionally excluded from the repository (they hold internal review/grading artifacts).

## Textbook Structure

Seven chapters, building from vector geometry to vector calculus, with two reference chapters at the end:

| Ch. | Title                                | Status in MATH 241  |
|-----|--------------------------------------|---------------------|
| 1   | Vectors and 3D Geometry              | **Taught**          |
| 2   | Vector Functions and Parametric Curves | **Taught**        |
| 3   | Partial Derivatives                  | **Taught**          |
| 4   | Multiple Integration                 | **Taught**          |
| 5   | Vector Calculus                      | **Taught**          |
| 6   | Differential Equations               | Reference only      |
| 7   | Complex Analysis                     | Reference only      |

Chapters 6 and 7 are included for students who want to continue past MATH 241 (PDEs, complex variables) but are not part of the course itself.

## Lecture Notes — 23 Worksheets

The `lecture_notes/` directory contains the in-class worksheet for every lecture, in two versions: a **blank** copy that students fill in during class, and a **filled** copy with all answers worked out for the instructor. Each worksheet is a single-source LaTeX document that toggles between the two versions via `\worksheetfilledtrue`.

| #  | Topic                                              | Textbook §  |
|----|----------------------------------------------------|-------------|
| 01 | Calculus Review and 3D Coordinates                 | 1.1–1.2     |
| 02 | Vectors                                            | 1.3         |
| 03 | Lines and Planes                                   | 1.4         |
| 04 | Vector-Valued Functions                            | 2.1         |
| 05 | Motion and Arc Length                              | 2.2         |
| 06 | Functions of Several Variables                     | 3.1         |
| 07 | Limits and Continuity                              | 3.2         |
| 08 | Partial Derivatives                                | 3.3         |
| 09 | Chain Rule                                         | 3.4         |
| 10 | Directional Derivatives and Gradient               | 3.5         |
| 11 | Tangent Planes and Linear Approximations           | 3.6         |
| 12 | Extrema and Critical Points                        | 3.7         |
| 13 | Double Integrals over Rectangular Regions          | 4.1         |
| 14 | Double Integrals over General Regions              | 4.2         |
| 15 | Applications of Multiple Integrals                 | 4.3         |
| 16 | Triple Integrals                                   | 4.4         |
| 17 | Change of Variables                                | 4.5         |
| 18 | Vector Fields                                      | 5.1         |
| 19 | Line Integrals                                     | 5.2         |
| 20 | Green's Theorem                                    | 5.3         |
| 21 | Curl and Divergence                                | 5.4         |
| 22 | Surface Integrals                                  | 5.5         |
| 23 | Stokes' Theorem and the Divergence Theorem         | 5.6         |

### Why these 23 lectures?

A single semester at Benedict College gives roughly 26 class meetings after subtracting exams, holidays, and a review day. The course has been pared from "everything in the textbook" to "everything that pays for itself in the time available." Three textbook topics are intentionally omitted from the lecture sequence:

- **Quadric surfaces.** Useful for visualization, but students are not assessed on classifying them and the figures appear naturally in later chapters when they are needed (e.g. ellipsoids in change-of-variables, paraboloids in surface integrals). The textbook still treats them in Chapter 1 for reference.
- **The TNB frame and curvature formulas.** A beautiful topic, but it does not scaffold any later material in MATH 241 and competes for time with the chain rule and gradient. Chapter 2 still contains the full treatment for interested readers.
- **Lagrange multipliers.** Relegated to reading. Chapter 3 covers Lagrange multipliers and the Karush–Kuhn–Tucker conditions in depth (with a worked SVM example), but in-class time is better spent on the unconstrained extrema and gradient-descent tools that the engineering and data-science majors will actually use afterward.

The lectures that *are* taught are numbered sequentially **01–23**, matching the worksheet filenames, the worksheet headers, and the syllabus calendar.

## How to Compile

The textbook and the lecture notes use a small custom style file each (`main.tex` includes its own preamble; `lecture_notes/src/worksheet_style.sty` handles the worksheets). MiKTeX or TeX Live with the standard math packages (`amsmath`, `amssymb`, `mathtools`, `physics`, `tcolorbox`, `tikz`, `pgfplots`, `esint`, `booktabs`, `enumitem`, `hyperref`) is enough.

```bash
# Full textbook
pdflatex main.tex && pdflatex main.tex   # second pass for cross-refs/index

# Solutions manual
cd solutions_manual && pdflatex main_solutions.tex

# Individual worksheet (both blank and filled versions)
cd lecture_notes/src && bash build.sh           # all worksheets
cd lecture_notes/src && bash build.sh 07        # just lecture 07

# Combine the per-lecture PDFs into the bound versions
pdfunite lecture_notes/filled/*.pdf lecture_notes/MATH241_Lecture_Notes_Filled.pdf
pdfunite lecture_notes/blank/*.pdf  lecture_notes/MATH241_Lecture_Notes_Blank.pdf
```

The build artifacts (`*.aux`, `*.log`, `*.synctex.gz`, `main.pdf`, etc.) are gitignored; the repository tracks only the published `Theories of Generalized Change.pdf`.

## Programming Examples

Each chapter ships with parallel implementations of its computational examples in **C, C++, Python, and Ruby**, under `chapters/chpN_code/`. The same algorithm is implemented in all four languages so students can compare paradigms and use the language they already know.

```
chapters/
├── chp1_code/   # Vectors and 3D geometry
├── chp2_code/   # Vector functions, curvature
├── chp3_code/   # Partial derivatives, gradient descent
├── chp4_code/   # Multiple integration
├── chp5_code/   # Vector fields, line/surface integrals
├── chp6_code/   # Differential equations
└── chp7_code/   # Complex analysis
```

A handful of figure assets live next to the code in each chapter's `chpN_pics/` directory.

## Notation

- Vectors are bold: $\mathbf{v}$, $\mathbf{F}$ (or angle-bracket notation $\langle a,b,c\rangle$).
- Unit vectors carry a hat: $\hat{\mathbf{i}}$, $\hat{\mathbf{j}}$, $\hat{\mathbf{k}}$.
- Partial derivatives: $\partial f/\partial x$ or $f_x$; gradient: $\nabla f$.
- Dot and cross products: $\mathbf{a}\cdot\mathbf{b}$, $\mathbf{a}\times\mathbf{b}$.
- Complex numbers: $z = x + iy$.

## Author

**Joseph Babcanec** — Department of Mathematics and Computer Science, **Benedict College**, Columbia, SC. Comments, corrections, and pull requests are welcome.

## License

See `LICENSE`. The textbook and accompanying materials are intended for classroom use; please cite the work if you adopt it elsewhere.
