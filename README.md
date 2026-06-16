# Linear Regression from Scratch — C++

A full implementation of Linear Regression built from the ground up in C++, using no external ML libraries. Every operation — matrix multiplication, gradient descent, loss computation — is implemented manually using a custom `Matrix` class built on `std::vector`.

This project was   a learning exercise to bridge the gap between ML theory and low-level implementation, and to demonstrate that understanding what happens *inside* `model.fit()` is more valuable than calling it.

---

## What this project does

Given a dataset of X and Y values  the model:

1. Organizes raw data into matrices
2. Adds a bias column to enable the intercept term
3. Finds the best-fit line through the data using two methods:
   - **Normal Equation** — closed-form, exact answer in one shot
   - **Gradient Descent** — iterative optimization, watching loss fall in real time
4. Evaluates predictions using MSE, MAE, and R²

**Sample output:**
```
=== Linear Regression from Scratch (C++) ===
Training Weights with the normal Equation....
w0: 24.2548
w1: 0.157452
Predicted VS Actual: : 
Predicted: 150.217, Actual: 152
Predicted: 189.58, Actual: 183
Predicted: 213.197, Actual: 214
Predicted: 244.688, Actual: 248
Predicted: 276.178, Actual: 271
Predicted: 299.796, Actual: 305
Predicted: 339.159, Actual: 340
Predicted: 354.904, Actual: 358
Predicted: 386.395, Actual: 391
Predicted: 417.885, Actual: 410
=== Metrics for Normal Equation Model ===
MSE: 20.565605
MAE: 3.928662
R-squared: 0.997073
Epoch No.  0 loss: 88961.8
Epoch No.  100 loss: 48118.9
Epoch No.  200 loss: 26043
Epoch No.  300 loss: 14110.8
Epoch No.  400 loss: 7661.31
Epoch No.  500 loss: 4175.32
Epoch No.  600 loss: 2291.11
Epoch No.  700 loss: 1272.69
Epoch No.  800 loss: 722.218
Epoch No.  900 loss: 424.685
Epoch No.  1000 loss: 263.867
Epoch No.  1100 loss: 176.943
Epoch No.  1200 loss: 129.959
Epoch No.  1300 loss: 104.565
Epoch No.  1400 loss: 90.8387
Epoch No.  1500 loss: 83.4196
Epoch No.  1600 loss: 79.4096
Epoch No.  1700 loss: 77.2421
Epoch No.  1800 loss: 76.0706
Epoch No.  1900 loss: 75.4373
Epoch No.  2000 loss: 75.0951
Epoch No.  2100 loss: 74.9101
Epoch No.  2200 loss: 74.8101
Epoch No.  2300 loss: 74.756
Epoch No.  2400 loss: 74.7268
Epoch No.  2500 loss: 74.711
Epoch No.  2600 loss: 74.7025
Epoch No.  2700 loss: 74.6979
Epoch No.  2800 loss: 74.6954
Epoch No.  2900 loss: 74.694
Epoch No.  3000 loss: 74.6933
Epoch No.  3100 loss: 74.6929
Epoch No.  3200 loss: 74.6927
Epoch No.  3300 loss: 74.6926
Epoch No.  3400 loss: 74.6925
Epoch No.  3500 loss: 74.6925
Epoch No.  3600 loss: 74.6925
Epoch No.  3700 loss: 74.6924
Epoch No.  3800 loss: 74.6924
Epoch No.  3900 loss: 74.6924
Epoch No.  4000 loss: 74.6924
Epoch No.  4100 loss: 74.6924
Epoch No.  4200 loss: 74.6924
Epoch No.  4300 loss: 74.6924
Epoch No.  4400 loss: 74.6924
Epoch No.  4500 loss: 74.6924
Epoch No.  4600 loss: 74.6924
Epoch No.  4700 loss: 74.6924
Epoch No.  4800 loss: 74.6924
Epoch No.  4900 loss: 74.6924
Training Weights with Gradient Descent....
w0: 0.000103936
w1: 0.17064
Predicted VS Actual (GD): : 
Predicted: 136.512, Actual: 152
Predicted: 179.172, Actual: 183
Predicted: 204.768, Actual: 214
Predicted: 238.896, Actual: 248
Predicted: 273.024, Actual: 271
Predicted: 298.62, Actual: 305
Predicted: 341.279, Actual: 340
Predicted: 358.343, Actual: 358
Predicted: 392.471, Actual: 391
Predicted: 426.599, Actual: 410
=== Metrics for Gradient Descent Model ===
MSE (GD): 74.692422
MAE (GD): 6.575054
R-squared (GD): 0.989370
```

---

## Project structure

```
├── matrix.h        # Matrix class — all linear algebra operations
├── matrix.cpp      # (empty — all implementation lives in header)
├── dataset.h       # Dataset class — data loading, bias column, normalization
├── model.h         # LinearRegression class — predict, loss, fit_simple, fit_gd
├── metrics.h       # Free functions — MSE, MAE, R²
└── main.cpp        # Entry point — wires everything together
```

Each file has one job. The dependency chain is:

```
matrix.h  ←  dataset.h  ←  model.h  ←  main.cpp
                                ↑
                           metrics.h
```

---

## The math

### 1. Prediction

The core operation of linear regression. Given a feature matrix $X$ and weight vector $w$, predictions are:

$$\hat{y} = Xw$$

Where:
- $X$ is $[m \times (n+1)]$ — $m$ samples, $n$ features, plus a bias column of 1s
- $w$ is $[(n+1) \times 1]$ — one weight per feature, plus the bias weight $w_0$
- $\hat{y}$ is $[m \times 1]$ — one predicted value per sample

For simple regression (1 feature), this expands to:

$$\hat{y}_i = w_0 \cdot 1 + w_1 \cdot x_i = w_0 + w_1 x_i$$

$w_0$ is the intercept (base price), $w_1$ is the slope (price per sqft).

---

### 2. The Bias Column

Before any computation, a column of 1s is prepended to $X$:

$$X_{\text{raw}} = \begin{bmatrix} 800 \\ 1200 \\ 1500 \end{bmatrix} \quad \Rightarrow \quad X_{\text{bias}} = \begin{bmatrix} 1 & 800 \\ 1 & 1200 \\ 1 & 1500 \end{bmatrix}$$

This turns $\hat{y} = w_1 x$ (line through origin) into $\hat{y} = w_0 + w_1 x$ (line with intercept). Without it, the model is forced through the origin.

---

### 3. Loss Function — Mean Squared Error (MSE)

Measures how wrong the model is on average:

$$\mathcal{L} = \frac{1}{m} \sum_{i=1}^{m} (\hat{y}_i - y_i)^2$$

Differences are squared so negative and positive errors don't cancel out. The goal of training is to minimize this number.

---

### 4. Normal Equation — Closed Form Solution

The mathematically exact answer for linear regression weights, computed in one shot with no iteration:

$$w = (X^T X)^{-1} X^T y$$

Step by step:
1. $X^T$ — transpose of $X$, shape $[(n+1) \times m]$
2. $X^T X$ — shape $[(n+1) \times (n+1)]$, for simple regression this is $[2 \times 2]$
3. $(X^T X)^{-1}$ — matrix inverse, computed using the 2×2 formula below
4. $X^T y$ — shape $[(n+1) \times 1]$
5. Final multiply gives $w$, shape $[(n+1) \times 1]$

**2×2 Matrix Inverse:**

$$A = \begin{bmatrix} a & b \\ c & d \end{bmatrix} \quad \Rightarrow \quad A^{-1} = \frac{1}{ad - bc} \begin{bmatrix} d & -b \\ -c & a \end{bmatrix}$$

Where $\det(A) = ad - bc$. If $\det(A) = 0$, the matrix is singular and not invertible.

---

### 5. Gradient Descent

An iterative optimization method that adjusts weights step by step, moving in the direction that reduces loss fastest:

$$w := w - \frac{\alpha}{m} X^T (Xw - y)$$

Where $\alpha$ is the **learning rate** — how big each step is.

Each iteration:

| Step | Operation | Shape |
|------|-----------|-------|
| Forward pass | $\hat{y} = Xw$ | $[m \times 1]$ |
| Error | $e = \hat{y} - y$ | $[m \times 1]$ |
| Gradient | $g = X^T e$ | $[(n+1) \times 1]$ |
| Scale | $g = \frac{\alpha}{m} \cdot g$ | $[(n+1) \times 1]$ |
| Update | $w = w - g$ | $[(n+1) \times 1]$ |

The gradient $g$ tells you which direction each weight should move to reduce loss. Subtracting it moves you downhill on the loss surface.

If loss *increases* during training, the learning rate $\alpha$ is too large — divide it by 10.

---

### 6. Evaluation Metrics

**Mean Absolute Error (MAE)** — average error in original units:

$$\text{MAE} = \frac{1}{m} \sum_{i=1}^{m} |\hat{y}_i - y_i|$$

**R² (Coefficient of Determination)** — how much variance the model explains:

$$R^2 = 1 - \frac{\sum_{i=1}^{m}(y_i - \hat{y}_i)^2}{\sum_{i=1}^{m}(y_i - \bar{y})^2}$$

Where $\bar{y}$ is the mean of all actual values.
- $R^2 = 1.0$ → perfect fit
- $R^2 = 0.0$ → model is no better than predicting the mean every time
- $R^2 < 0$ → model is worse than that

---

### 7. Normalization (for multi-feature extension)

When features have very different scales, gradient descent steps are unbalanced. Normalization fixes this:

$$X_{ij} = \frac{X_{ij} - \mu_j}{\sigma_j}$$

Where $\mu_j$ is the mean and $\sigma_j$ is the standard deviation of feature column $j$:

$$\mu_j = \frac{1}{m} \sum_{i=1}^{m} X_{ij} \qquad \sigma_j = \sqrt{\frac{1}{m} \sum_{i=1}^{m} (X_{ij} - \mu_j)^2}$$

Applied per column, skipping the bias column.

---

## How to build and run

```bash
g++ -std=c++17 main.cpp -o linreg
./linreg
```

No external dependencies. Standard C++17.

---

## Key design decisions

**Why C++?** Forces explicit memory management and makes every matrix operation visible. You cannot hide behind abstractions.

**Why `vector<vector<double>>` instead of raw arrays?** Safe resizing, no manual `new`/`delete`, and the Matrix class can be assigned and returned by value cleanly.

**Why implement Normal Equation before Gradient Descent?** The normal equation gives the exact answer — it's a ground truth to verify that matrix operations (transpose, multiply, inverse) are all correct before adding the complexity of iteration.

**Why a separate `Dataset` class?** Separation of concerns. The model should not know or care how data is loaded or preprocessed. Swap `load_hardcoded()` for a CSV reader later without touching `model.h`.

---

## What I learned

- How matrix multiplication is the engine behind every ML prediction
- Why the bias column trick works and what happens without it
- The difference between a closed-form solution and an iterative one
- Why feature normalization is necessary for gradient descent to converge
- How to structure a C++ project across multiple header files with include guards
- How to debug linear algebra code by checking shapes at every step
