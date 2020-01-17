#include "head.h"

void print_groups(vector<group> groups){
    for(int i = 0; i < groups.size(); i++){
        printf("\ngroup %d: ", i);
        for(int j = 0; j < groups[i].products.size(); j++){
            printf("prd %d ",groups[i].products[j].id);
        }
        printf("\noperations: ");
        for(int j = 0; j < groups[i].operations.size(); j++){
            printf("%c%d ",groups[i].operations[j].element.type ,groups[i].operations[j].element.setting);
            //printf("hp = %d\n", groups[i].operations[j].hp);
        }
    }
    printf("\n");
};

vector<int> check_line(int prd_count, int x, int y, int group_id, int **similarity_table, vector<int>& flag, int max){
    for(int i = 0; i < x; i++){
        if(similarity_table[x][i] == max && flag[i] == 0){
            flag[i] = group_id;
            check_line(prd_count, x, i, group_id, similarity_table, flag, max);
            similarity_table[x][i] = -1;
        }
    }
    for(int j = x; j < prd_count; j++){
        if(similarity_table[j][x] == max && flag[j] == 0){
            flag[j] = group_id;
            check_line(prd_count, x, j, group_id, similarity_table, flag, max);
            similarity_table[j][x] = -1;
        }
    }
    for(int i = 0; i <  y; i++){
        if(similarity_table[y][i] == max && flag[i] == 0){
            flag[i] = group_id;
            check_line(prd_count, x, i, group_id, similarity_table, flag, max);
            similarity_table[y][i] = -1;
        }
    }
    for(int j = y; j < prd_count; j++){
        if(similarity_table[j][y] == max && flag[j] == 0){
            flag[j] = group_id;
            check_line(prd_count,j, y, group_id, similarity_table, flag, max);
            similarity_table[j][y] = -1;
        }
    }
    return flag;
}

int is_finished(vector<int> flags){
    int undef = 0;
    for(int i = 0; i < flags.size(); i++) if(flags[i] == 0) undef++;
    return undef;
}

vector<group> to_group(vector<product> prd_list, int **similarity_table){
    vector<group> groups;
    vector<int> flag;
    for(int i = 0; i < prd_list.size(); i++) flag.push_back(0);
    int max = 0;
    int x;
    int y;
    int group_id = 1;
    while (is_finished(flag) != 0)
    {
        if(is_finished(flag) == 1){
            for(int i = 0; i < flag.size(); i++){
                if(flag[i] == 0) flag[i] = group_id;
            }
        }
        for (int i = 0; i < prd_list.size(); i++){
            for(int j = 0; j < i; j++){
                if(similarity_table[i][j] > max){
                    max = similarity_table[i][j];
                    x = i;
                    y = j;
                }
            }
        }
        if(flag[x] == 0 && flag[y] == 0){
            flag[x] = group_id;
            flag[y] = group_id;
            flag = check_line(prd_list.size(),x,y,group_id,similarity_table,flag,max);
            group_id++;
        }
        similarity_table[x][y] = -1;
        max = 0;
    }
    //for(int i = 0; i < flag.size(); i++) printf("%d ", flag[i]);
    for(int i = 0; i < flag.size(); i++){
        if(flag[i] > groups.size()) {for(int j = groups.size(); j < flag[i]; j++) groups.push_back(group());}
        //printf("adding product %d to group %d\n", i+1, flag[i]-1);
        groups[flag[i] - 1].products.push_back(prd_list[i]);
        for(int j = 0; j < groups[flag[i] - 1].products.back().instruction.size(); j++){
            bool is = false;
            for(int k = 0; k < groups[flag[i] - 1].operations.size(); k++){
                if(groups[flag[i] - 1].products.back().instruction[j] == groups[flag[i] - 1].operations[k].element) is = true;
            }
            //printf("command %c%d",groups[flag[i] - 1].products.back().instruction[j].type,groups[flag[i] - 1].products.back().instruction[j].setting);
            if(!is) {
                //printf(" is added\n");
                groups[flag[i] - 1].operations.push_back(uniq_operation(groups[flag[i] - 1].products.back().instruction[j]));
            }
            else{
                for(int k = 0; k < groups[flag[i] - 1].operations.size(); k++){
                    if(groups[flag[i] - 1].operations[k].element == groups[flag[i] - 1].products.back().instruction[j]){
                        groups[flag[i] - 1].operations[k].hp++;    
                        //printf(" isn't added hp = %d\n", groups[flag[i] - 1].operations[k].hp);
                        break;
                    }
                }
            }
        }
        //printf("\n");
    }
    return groups;
}

int check_pr_cupability(vector<group>& groups, int i){
    int score = 0;
    for(int j = i + 1; j < groups.size(); j++){
        for(int y = 0; y < groups[j].products.size(); y++){
            //printf("\ncheking product %d to group %d:",groups[j].products[y].id,i);
            int simul_check = 0;
            int prev_step = 0;
            int stat = 0;
            for(int x = 0; x < groups[j].products[y].instruction.size(); x++){
                for(int k = 0; k < groups[i].operations.size(); k++){
                    stat++;
                    if(groups[i].operations[k].element == groups[j].products[y].instruction[x]) simul_check ++;
                }
            }
            //printf(" %d & %d \n",(int)groups[j].products[y].instruction.size(), simul_check);
            if(simul_check == groups[j].products[y].instruction.size()){
                groups[i].products.push_back(groups[j].products[y]);
                    
                for(int a = 0; a < groups[i].products.back().instruction.size(); a++){
                    bool is = false;
                    int is_pos = 0;
                    for(int k = 0; k < groups[i].operations.size(); k++){
                        if(groups[i].products.back().instruction[a] == groups[i].operations[k].element){is = true;   is_pos = k;}
                    }
                    //printf("command %c%d",groups[i].products.back().instruction[a].type,groups[i].products.back().instruction[a].setting);
                    if(!is) {
                        groups[i].operations.push_back(uniq_operation(groups[i].products.back().instruction[a]));
                        //printf(" is added   hp = %d\n", groups[i].operations.back().hp);
                    }
                    else{
                        groups[i].operations[is_pos].hp++;
                        //printf(" isn't added hp = %d\n", groups[i].operations[is_pos].hp);
                    }
                    for(int k = 0; k < groups[j].operations.size(); k++){
                        if(groups[i].products.back().instruction[a] == groups[j].operations[k].element){
                            groups[j].operations[k].hp--;
                            //printf("operaton %c%d hp decreased to %d\n", groups[j].operations[k].element.type, groups[j].operations[k].element.setting, groups[j].operations[k].hp);
                            if(!groups[j].operations[k].hp){
                                //printf("operaton %c%d is erased\n",groups[j].operations[k].element.type,groups[j].operations[k].element.setting);
                                groups[j].operations.erase(groups[j].operations.begin() + k);
                            }
                        }
                    }
                }
                groups[j].products.erase(groups[j].products.begin() + y);
                y--;
                //printf(" moved\n");
                score ++;
            }
            //else printf(" not moved\n");
        }
    }
    for(int i = 0; i < groups.size(); i++)
        if(!groups[i].products.size())
            groups.erase(groups.begin() + i);
    return score;
}

bool sorted(vector<int> flags){
    bool flag = true;
    for(int i = 0; i < flags.size(); i++) if(flags[i] != -1) flag = false;
    return flag;
}

void group_sort(vector<group>& groups){
    sort(groups.begin(),groups.end(), greater<group>());
    print_groups(groups);
    for(int i = 0; i < groups.size() - 1; i++){
        if(groups[i] == groups[i+1]){
            int j = i + 1;
            while((j + 1 < groups.size()) && groups[j] == groups[j + 1]) j++;
            vector<int> flags;
            //printf("Defining lines %d & %d\n",i , j);
            for(int a = 0; a <= j - i; a++){
                vector<group> sub_groups(groups.size() - i + 1);
                copy(groups.begin() + i, groups.end(), sub_groups.begin());
                swap(sub_groups[0], sub_groups[a]);
                //printf("\nline %d\n", a);
                flags.push_back(check_pr_cupability(sub_groups, 0));     
                //printf("score = %d\n", flags.back());
            }
            int k = 0;
            while(1){
                group temp_gr;
                int max = -1;
                int max_pos = 0;
                for(int a = 0; a < flags.size(); a++){
                    if(flags[a] > max){
                        max = flags[a];
                        max_pos = a;
                    } 
                }
                flags[max_pos] = -1;
                if(k != max_pos){
                    swap(groups[i + k], groups[i + max_pos]);
                    swap(flags[k], flags[max_pos]);
                }
                k++;
                if(sorted(flags)) break;;
            }
            i = j;
        }
    }
}

void re_group(vector<group>& groups){
    for(int i = 0; i < groups.size(); i++){
        check_pr_cupability(groups, i);
    }
}
