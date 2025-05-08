# kachow

This project implements and optimizes 2D matrix convolutions using SIMD, OpenMP, and MPI. The goal is to build a performant system for grayscale video processing using matrix operations.

---

## 🗂️ Repository Structure

| File                          | Description |
|-------------------------------|-------------|
| `compute.h`                   | Header file containing shared matrix and convolution function declarations. |
| `compute_naive.c`             | Naive implementation of 2D convolution. Acts as the baseline for performance comparison. |
| `compute_optimized.c`         | Optimized implementation using SIMD (AVX2) and OpenMP to accelerate performance. |
| `compute_optimized_mpi.c`     | MPI-compatible compute implementation for distributed task-based convolution. |
| `coordinator.h`               | Header file for coordinator logic and task execution function declarations. |
| `coordinator_naive.c`         | Basic coordinator that sequentially executes convolution tasks. |
| `coordinator_mpi.c`           | Parallel coordinator using Open MPI to dispatch tasks to worker processes. |

---

## 🧠 Concepts Covered

- **2D Convolutions**: Sliding-window matrix operations for computing blur and sharpen filters.
- **SIMD (AVX2)**: Vectorized integer operations using 256-bit registers for per-pixel acceleration.
- **OpenMP**: Thread-based parallelization for row-wise matrix computations.
- **Open MPI**: Process-based distributed computing model to scale across multiple cores/nodes.
- **Matrix I/O**: Custom binary format with helper routines provided in `io.o` for reading and writing matrices.


