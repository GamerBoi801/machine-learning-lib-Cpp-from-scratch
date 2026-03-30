Perfect learning mindset. Let me lay out the full project structure, math, and function specs — no code completions, just enough to guide your thinking.Here's the full project breakdown — math, English, and function specs for each piece. No code completions, just everything you need to think and implement.---

## `Matrix` class — what to extend

Your existing class becomes the linear algebra engine for everything else. Replace `int**` with `vector<vector<double>>` (doubles matter for weights and gradients).

**`transpose()`**
Math: if A is m×n, then Aᵀ is n×m where `Aᵀ[j][i] = A[i][j]`.
In English: flip rows and columns. The first row becomes the first column. This is needed for the normal equation and for computing gradients — you'll call it constantly.

**`multiply(Matrix& B)`**
Math: C[i][j] = Σₖ A[i][k] × B[k][j], for k from 0 to A.cols.
In English: this is your existing `matmul`, just moved inside the class as a method that returns a new Matrix. Check dimensions first and throw or print a clear error.

**`add(Matrix& B)`**
Math: C[i][j] = A[i][j] + B[i][j], element-wise. Shapes must match.
In English: used to add bias terms and update weights during gradient descent.

**`scale(double alpha)`**
Math: C[i][j] = alpha × A[i][j], scalar multiplication.
In English: you'll use this to multiply a gradient matrix by the learning rate before subtracting from weights.

**`subtract(Matrix& B)`**
Math: C[i][j] = A[i][j] - B[i][j].
In English: computing the error vector `(y_hat - y)` and the weight update step.

---

## `Dataset` class

**What it holds:** a Matrix `X` (features) and a Matrix `y` (targets). Both as column vectors or matrices.

**`load_hardcoded()`** — for Phase 1, skip file I/O. Just hard-code 10–20 (x, y) pairs representing a line with some noise, e.g. house size vs price. Populate `X` and `y` directly.

**`add_bias_column()`**
Math: prepend a column of 1s to X, making it an [m × (n+1)] matrix.
In English: this turns `y = wx` into `y = w₀ + w₁x`. The column of 1s means the bias weight `w₀` gets multiplied by 1 — so it just gets added. Every linear regression needs this. Without it your line is forced through the origin.

**`normalize()`** (do this later, for multi-feature)
Math: for each feature column j, `X[i][j] = (X[i][j] - mean_j) / std_j`.
In English: if feature 1 is "size in sqft" (values ~1000–3000) and feature 2 is "num bedrooms" (values 1–5), gradient descent will move wildly on the big feature and barely at all on the small one. Normalization puts every feature on the same scale.

---

## `LinearRegression` class

**What it holds:** a Matrix `weights` — shape [(n+1) × 1] where n is the number of features. The +1 is for the bias weight.

**`predict(Matrix& X)`**
Math: ŷ = X · w, matrix multiplication of [m × (n+1)] by [(n+1) × 1], giving [m × 1].
In English: for each sample, multiply each feature by its corresponding weight and sum them up — that's the predicted value. This is literally your matmul.

**`compute_loss(Matrix& y_hat, Matrix& y)`** — MSE
Math: L = (1/m) × Σᵢ (ŷᵢ − yᵢ)².
In English: subtract predictions from true values, square each difference (so negatives don't cancel positives), average them. Gives you one number telling you how wrong the model is on average. The goal of training is to shrink this number.

**`fit_simple()` — Phase 1, normal equation (no iteration)**
Math: w = (XᵀX)⁻¹ Xᵀy.
In English: this is the closed-form solution — the mathematically exact answer for linear regression. No loops, no learning rate. You need matrix inverse for this, which is non-trivial to implement. For 1 feature + bias, you can do it by hand (2×2 inverse has a simple formula: swap diagonal, negate off-diagonal, divide by determinant). Implement this first to verify your matmul is correct before touching gradient descent.

**`fit_gd()` — Phase 2, gradient descent**
Math: w := w − (α/m) × Xᵀ(Xw − y), repeated for some number of epochs.
In English, step by step:
1. Compute predictions: `y_hat = X · w`
2. Compute errors: `error = y_hat - y` (shape [m×1])
3. Compute gradient: `grad = Xᵀ · error` (shape [(n+1)×1]) — this tells you which direction each weight should move
4. Scale it: `grad = grad.scale(alpha / m)`
5. Update: `w = w - grad`
6. Repeat for `epochs` iterations, printing loss every 100 epochs so you can watch it fall

---

## `Metrics` — free functions (not a class)

These just take matrices and return a double. Put declarations in `metrics.h`, definitions in `metrics.cpp`.

**`mse(Matrix& y_hat, Matrix& y)`** — same as compute_loss, extracted so you can call it from anywhere.

**`mae(Matrix& y_hat, Matrix& y)`**
Math: (1/m) × Σᵢ |ŷᵢ − yᵢ|
In English: average absolute error — more interpretable than MSE because it's in the original units (not squared).

**`r_squared(Matrix& y_hat, Matrix& y)`**
Math: R² = 1 − (SS_res / SS_tot), where SS_res = Σ(yᵢ − ŷᵢ)² and SS_tot = Σ(yᵢ − ȳ)²
In English: how much of the variance in y does your model explain? 1.0 = perfect, 0.0 = no better than predicting the mean every time, negative = your model is somehow worse than that.

---

## Suggested order of attack

Start with just `Matrix` and `main.cpp`. Get multiply, transpose, scale, subtract working and tested with manual values you verify by hand. Then write `Dataset` with hardcoded data. Then `LinearRegression::predict` and `compute_loss` — at this point you can verify the output makes sense even before training. Then normal equation. Then gradient descent. Metrics last.

Every function has one job and takes/returns a Matrix — that discipline will make debugging much easier.