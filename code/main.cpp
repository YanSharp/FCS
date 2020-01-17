#include "head.h"

void similarity_check(vector<product> prd_list, int **result, vector<operation> oper_list){
    for(int i = 0; i < prd_list.size(); i++){
        for(int j = 0; j < i; j++){
            result[i][j] = 0;
        }
    }
    for(int i = 1; i < prd_list.size(); i++){
        for(int j = 0; j < i; j++){
            for(int x = 0; x < oper_list.size(); x++){
                bool f1 = false;
                bool f2 = false;
                for(int y = 0; y < prd_list[i].instruction.size(); y++){
                    if(oper_list[x] == prd_list[i].instruction[y]) f1 = true;
                }
                for(int y = 0; y < prd_list[j].instruction.size(); y++){
                    if(oper_list[x] == prd_list[j].instruction[y]) f2 = true;
                }
                if(f1 == f2) result[i][j]++;
            }
        }
    }
}

int main(){
    vector<operation> oper_list ;
    vector<product> prd_list;
    read_prd_list(prd_list, oper_list);
    printf("\n---------------------------------------------------\n");
    int **similarity_table = new int* [prd_list.size()];
    for (int i = 0; i < prd_list.size(); i++){
        similarity_table[i] = new int [i];
    }
    for(int i = 0; i < oper_list.size(); i++){
        printf("%c%d ", oper_list[i].type, oper_list[i].setting);
    }
    printf("\n");
    printf("---\n %d \n---\n",(int)prd_list.size());
    similarity_check(prd_list, similarity_table, oper_list);
    for(int i = 0; i < prd_list.size(); i++){
        for(int j = 0; j < i; j++){
            printf("%d ",similarity_table[i][j]);
        }
        printf(" - line %d\n", i);
    }
    printf("\n---------------grouping-------------\n");
    vector<group> groups = to_group(prd_list, similarity_table);
    print_groups(groups);

    printf("\n---------------sorting--------------\n");
    group_sort(groups);
    print_groups(groups);
    printf("\n--------------regrouping------------\n");
    re_group(groups);
    print_groups(groups);
    printf("\n------------making modules----------\n");
    vector<vector<operation>> modules;
    modeler(groups, modules);
    printf("\n------------sorting modules----------\n");
    vector<pair<int, vector<operation>>> s_modules = sorting_modules(modules, prd_list);
    return 0;
}