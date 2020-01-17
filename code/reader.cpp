#include "head.h"

void read_prd_list(vector<product>& prd_list, vector<operation>& opr_list){
    FILE *file = NULL;
    while(file == NULL){
        printf("Please, select filename: ");
        string filename;
        char* temp = new char[20];
        scanf("%s", temp);
        filename = temp;
        delete temp;
        filename += ".txt";
        file = fopen(filename.c_str() , "r");
        if(file == NULL){
            printf("Wrong filread_prd_listename %s, try again\n",filename.c_str());
        }
        else{
            char temp;
            operation cur_oper;
            product cur_product;
            cur_product.id = 1;
            while (fscanf (file, "%c%d%c", &(cur_oper.type), &(cur_oper.setting), &temp) != EOF) {
                printf("%c%d ", cur_oper.type, cur_oper.setting);
                cur_product.instruction.push_back(cur_oper);
                bool is = false;
                for(int i = 0; i < opr_list.size(); i++){
                    if(cur_oper == opr_list[i]) is = true;
                }
                if(is == false){
                    opr_list.push_back(cur_oper);
                }
                if(temp == ';'){
                    printf("\n");
                    fscanf(file,"%*c");
                    prd_list.push_back(cur_product);
                    //printf("product added\n");
                    cur_product.instruction.clear();
                    cur_product.id++;
                }
                if(temp == '.'){
                    prd_list.push_back(cur_product);
                    //printf("\nproduct added\n");
                }
            }
        }
    }
}