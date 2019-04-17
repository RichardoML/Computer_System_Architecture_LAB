/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
{
    int row,col,i,j;
    int a0,a1,a2,a3,a4,a5,a6,a7;
    //32*32
    if(N==32&&M==32)
    {
        for(row=0;row<N;row=row+8)
        {
            for(col=0;col<M;col=col+8)//某一个块内部
            {   
                for(i=row;i<row+8;i++)
                {
                    if(row==col)//对角线上的块,一次取出A的一块的一行
                    {
                        a0 =A[i][col];
                        a1 =A[i][col+1];
                        a2 =A[i][col+2];
                        a3 =A[i][col+3];
                        a4 =A[i][col+4];
                        a5 =A[i][col+5];
                        a6 =A[i][col+6];
                        a7 =A[i][col+7];

                       
                        B[col][i] =a0;
                        B[col+1][i] =a1;
                        B[col+2][i] =a2;
                        B[col+3][i] =a3;
                        B[col+4][i] =a4;
                        B[col+5][i] =a5;
                        B[col+6][i] =a6;
                        B[col+7][i] =a7;
                        continue;

                     }
                    for(j=col;j<col+8;j++)
                    {
                        
                        B[j][i] = A[i][j];
                    }
                }
            }   
        }
    }
    //64*64
    if(N==64&&M==64)
    {
        for(row=0;row<N;row=row+8)
        {
            for(col=0;col<M;col=col+8)//对应每一个8*8的块,8*8k块的内部需j=要按照4*4来处理
                {
                /*
                A 左上角4*4 转置到 B的左上角
                */
                for(i=row;i<row+4;i++)
                {
                    for(j=col;j<col+4;j++)
                    {
                        B[j][i] = A[i][j];
                    }
                } 
                /*
                A的右上角4*4转置到 B的右上角4*4
                */
                for(i=row;i<row+4;i++)
                {
                    for(j=col+4;j<col+8;j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
                /*
                A的左下角转置到B的右上角，同时把B的右上角移动到左下角
                注意：移动不需要做转置操作
                */
                for(j=col;j<col+4;j++)
                {
                   a4 = B[j][row+4];
                   a5 = B[j][row+5];
                   a6 = B[j][row+6];
                   a7 = B[j][row+7];

                    B[j][row+4]=A[row+4][j];
                    B[j][row+5]=A[row+5][j];
                    B[j][row+6]=A[row+6][j];
                    B[j][row+7]=A[row+6][j];

                    B[j+4][row]=a4;
                    B[j+4][row+1]=a5;
                    B[j+4][row+2]=a6;
                    B[j+4][row+3]=a7;
                }
                /*
                A右下角的4*4转置到B的右下角
                */
                for(i=row+4;i<row+8;i++)
                {
                    for(j=col+4;j<col+8;j++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    }
    if(N==61&&M==67)
    {
        for(row=0;row<N;row=row+17)
        {
            for(col=0;col<M;col=col+17)
            {
                for(i=row;i<row+17;i++)
                {
                    for(j=col;j<col+17;j++)
                    {
                        B[j][i]=A[i][j];
                    }
                }
            }
        }
    }
}

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

