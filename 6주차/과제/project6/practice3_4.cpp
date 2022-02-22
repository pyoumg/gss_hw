#include "my_solver.h"

#define index_2d(row, col, dim) row*dim+col

void practice3_4() {
    int i, ia, j, n, l[32];
    float a[32*32],a2[32*32], b[32],b2[32], x[32], s[32];
    float error = 0.0f;

    float temp = 0.0f,temp2=0.0f;
    /********************************/

    FILE* fp_r = fopen("linear_system_3-4.txt", "r");
    FILE* fp_w = fopen("solution_3-4.txt", "w");
    if (fp_r == NULL) {
        printf("%s file open error...\n", "linear_system_3-4.txt");
        return;
    }
    if (fp_w == NULL) {
        printf("%s file open error...\n", "solution_3-4.txt");
        return;
    }


    fscanf(fp_r,"%d", &n);
    ia = n;
    if (n > 32){
        fclose(fp_r);
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(fp_r, "%f", &a[index_2d(j, i, n)]);
            a2[index_2d(j, i, n)] = a[index_2d(j, i, n)];
        }
    }
    
    for (int i = 0; i < n; i++) {
        fscanf(fp_r, "%f", &b[i]);
        b2[i] = b[i];
    }


    gespp_(&n, a, &ia, l, s);

    solve_(&n, a, &ia, l, b, x);

    fprintf(fp_w, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(fp_w, "%.6f\n", x[i]);
    }

    for (int i = 0; i < n; i++) {
        temp = 0.0f;
        for (int j = 0; j < n; j++) {
            
            temp += a2[index_2d(j, i, n)] * x[j];
        }
        temp -= b2[i];
        error += temp * temp;
        temp2 += b2[i] * b2[i];
    }
    error = sqrt(error / temp2);
    fprintf(fp_w, "%.6f\n", error);

  
    /********************************/
    fclose(fp_r);
    fclose(fp_w);
}
