#include <omp.h>
#include <x86intrin.h>

#include "compute.h"

// Computes the convolution of two matrices
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  // TODO: convolve matrix a and matrix b, and store the resulting matrix in
  // output_matrix

  if (a_matrix == NULL) {
    return -1;
  } else if (b_matrix == NULL) {
    return -1;
  } else if (output_matrix == NULL) {
    return -1;
  } else {
      uint32_t output_col = a_matrix->cols - b_matrix->cols + 1;
      uint32_t output_row = a_matrix->rows - b_matrix->rows + 1;
      uint32_t output_dim = output_col * output_row;

      *output_matrix = (matrix_t *)calloc(1, sizeof(matrix_t));
      (*output_matrix)->rows = output_row;
      (*output_matrix)->cols = output_col;
      (*output_matrix)->data = (uint32_t *)calloc(output_dim, sizeof(uint32_t));

      *output_matrix = (matrix_t *) calloc(1, sizeof(matrix_t));

      (*output_matrix)->rows = output_row;
      (*output_matrix)->cols = output_col;
      (*output_matrix)->data = calloc(output_col * output_row, sizeof(int32_t));

      __m256i const mask = _mm256_setr_epi32(7,6,5,4,3,2,1,0);

      #pragma omp parallel for collapse(2) 
      for (uint32_t rowss = 0; rowss < output_row; rowss++) {
          for (uint32_t colss = 0; colss < output_col; colss++) {     
          int res = 0;
          #pragma for reduction(+ : res)
          for (uint32_t r = 0; r < b_matrix->rows; r++) {
              int32_t partial = 0;

              uint32_t col = b_matrix->cols;
              int32_t *aa = a_matrix->data + ((a_matrix->cols * rowss) + colss + (r * a_matrix->cols));
              int32_t *bb = b_matrix->data + ((b_matrix->rows - r - 1) * b_matrix->cols);

              if (col >= 8){
                  __m256i part = _mm256_setzero_si256();

                  for (int i = 0; i < col / 8 * 8; i+=8){

                    
                      __m256i t = _mm256_mullo_epi32(_mm256_loadu_si256((__m256i *)(aa + i)), 
                              _mm256_permutevar8x32_epi32(_mm256_loadu_si256((__m256i *) (bb + col - i - 8)), mask));

                      part = _mm256_add_epi32(part, t);
                  }
                
                  int32_t temp[8];
                  _mm256_storeu_si256((__m256i *) temp, part);
                  for (int z = 0; z < 8; z++){
                
                      partial += temp[z]; 
                  }

              }
              for (int i = col / 8 * 8; i < col; i++) {
                  partial += aa[i] * bb[col - 1 -i];
              }
              res += partial;
          }

          (*output_matrix)->data[(rowss * output_col) + colss] = res;
          } 
      }

    return 0;
    }
}


















// Executes a task
int execute_task(task_t *task) {
  matrix_t *a_matrix, *b_matrix, *output_matrix;

  char *a_matrix_path = get_a_matrix_path(task);
  if (read_matrix(a_matrix_path, &a_matrix)) {
    printf("Error reading matrix from %s\n", a_matrix_path);
    return -1;
  }
  free(a_matrix_path);

  char *b_matrix_path = get_b_matrix_path(task);
  if (read_matrix(b_matrix_path, &b_matrix)) {
    printf("Error reading matrix from %s\n", b_matrix_path);
    return -1;
  }
  free(b_matrix_path);

  if (convolve(a_matrix, b_matrix, &output_matrix)) {
    printf("convolve returned a non-zero integer\n");
    return -1;
  }

  char *output_matrix_path = get_output_matrix_path(task);
  if (write_matrix(output_matrix_path, output_matrix)) {
    printf("Error writing matrix to %s\n", output_matrix_path);
    return -1;
  }
  free(output_matrix_path);

  free(a_matrix->data);
  free(b_matrix->data);
  free(output_matrix->data);
  free(a_matrix);
  free(b_matrix);
  free(output_matrix);
  return 0;
}
