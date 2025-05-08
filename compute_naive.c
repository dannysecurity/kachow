#include "compute.h"

// Computes the convolution of two matrices
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  // TODO: convolve matrix a and matrix b, and store the resulting matrix in
  // output_matrix
  if (a_matrix == NULL) {
      return -1;
  } else if (b_matrix == NULL) {
      return -1;
  } else if (output_matrix == NULL){
      return -1;
  } else {
      uint32_t output_col = a_matrix->cols - b_matrix->cols + 1;
      uint32_t output_row = a_matrix->rows - b_matrix->rows + 1;
      uint32_t output_dim = output_col * output_row;

      *output_matrix = (matrix_t*)calloc(1, sizeof(matrix_t));
      (*output_matrix)->rows = output_row;
      (*output_matrix)->cols = output_col;
      (*output_matrix)->data = (uint32_t*)calloc(output_dim, sizeof(uint32_t));

      for (uint32_t i = 0; i < output_row; i++){
          for (uint32_t j = 0; j < output_col; j++){
              uint32_t sum = 0;
              for (uint32_t r = 0; r < b_matrix->rows; r++){
                  for (uint32_t c = 0; c < b_matrix->cols; c++){
                      sum += a_matrix->data[(i+r) * a_matrix->cols + (j + c)] 
                          * b_matrix->data[(b_matrix->rows - 1 - r) * b_matrix->cols
                          + (b_matrix->cols - 1 - c)];
                  }
              }
              (*output_matrix)->data[i * output_col + j] = sum;
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
