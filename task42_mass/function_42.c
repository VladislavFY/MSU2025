#include "prototypes.h"

int function_of_program(double *array, int len){
    int cur_len=1;
    int max_len=1;
       for(int i=1; i<len;i++){
           if(array[i]-array[i-1]>=-eps){
               cur_len++;
               if(cur_len>max_len) max_len=cur_len;
        }
        else if(array[i]-array[i-1]<-eps) cur_len=1;
    }
    return max_len;
}
