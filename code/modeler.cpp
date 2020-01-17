#include "head.h"

void create_module(vector<vector<int>>& graph_of_modules, vector<vector<operation>>& modules, vector<int> to_add){
    sort(to_add.begin(),to_add.end());
    while(to_add.size() > 1){
        for(int j = 0; j < modules[to_add.back()].size(); j++){
            modules[to_add.front()].push_back(modules[to_add.back()][j]);
        }
        modules.erase(modules.begin() + to_add.back());
        for(int j = 0; j < graph_of_modules.size(); j++){
            if(to_add.front() == j) graph_of_modules[to_add.front()][j] = 0;
            else{
                graph_of_modules[to_add.front()][j] |= graph_of_modules[to_add.back()][j];
                graph_of_modules[j][to_add.front()] |= graph_of_modules[j][to_add.back()];
            }
            graph_of_modules[j].erase(graph_of_modules[j].begin() + to_add.back());
        }
        graph_of_modules.erase(graph_of_modules.begin() + to_add.back());
        to_add.pop_back();
    }
}

bool find_way(vector<vector<int>> matrix, vector<int>& path, int goal, vector<vector<operation>> modules){
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 1; j < path.size(); j++){
            if(i == path[j]) return false;
        }
        //printf("Node %d ", i);
        if(matrix[path.back()][i] != 0){
            path.push_back(i);
            /*printf("path: ");
            for(int j = 0; j < path.size(); j++) printf("%d ", path[j]);
            printf("\n");*/
            int lenth = 0;
            for(int j = 0; j < path.size(); j++){
                lenth += modules[path[j]].size();
            }
            if(path.front() == goal && path.back() == goal) return true;
            if(lenth > DEPT){/*printf("\nDEPTexeption\n");*/ path.pop_back(); continue;}
            if(path.back() == goal) return true;
            if(find_way(matrix, path, goal, modules)) return true;
            path.pop_back();
        }
    }
    return false;
}

bool find_jump(vector<vector<int>> matrix, vector<int>& path, vector<vector<operation>> modules){
    //printf("looking for jump\n");
    int inp_cnt = 0;
    for(int i = 0; i < matrix.size(); i++){
        inp_cnt += matrix[path.front()][i];
    }
    if(inp_cnt < 2) return false;
    for(int i = 0; i < matrix.size(); i++){
        if(matrix[path.front()][i]){
            matrix[path.front()][i] = 0;
            if(find_way(matrix, path, i, modules)){
                return true;
            }
            while(path.size() != 1) path.pop_back();
            matrix[path.back()][i] = 1;
        }
    }
    return false;
}
void group_modeler(group group_p, vector<vector<operation>>& modules){
    for(int i = 0; i < group_p.operations.size(); i++){
        modules[i].push_back(group_p.operations[i].element);
    }
    vector<vector<int>>graph_of_modules;
    for (int i = 0; i < group_p.operations.size(); i++){
        graph_of_modules.assign(group_p.operations.size(), vector<int>(group_p.operations.size()));
    }
    for(int i = 0; i < group_p.products.size(); i++){
        int prev_oper = 0;
        int cur_oper = 0;
        for(int k = 0; k < group_p.operations.size(); k++){
            if(group_p.products[i].instruction[0] == group_p.operations[k].element){ prev_oper = k;    break;}
        }
        for(int j = 1; j < group_p.products[i].instruction.size(); j++){
            for(int k = 0; k < group_p.operations.size(); k++){
                if(group_p.products[i].instruction[j] == group_p.operations[k].element){
                    cur_oper = k;
                    graph_of_modules[cur_oper][prev_oper] = 1;
                    prev_oper = cur_oper;
                    break;
                }
            }
        }
    }
    printf("\n Graph matrix\n");
    for(int i = 0; i < graph_of_modules.size(); i++){
        for(int j = 0; j < graph_of_modules[i].size(); j++){
            printf("%d ",graph_of_modules[i][j]);
        }
        printf("\n");
    }
    vector<int> path;
    for(int i = 0; i < modules.size(); i++){    //looking for loops
        path.push_back(i);
        while(modules[i].size() + 1 < DEPT && find_way(graph_of_modules, path, i, modules)){
            path.pop_back();
            printf("final path: \n");
            for(int j = 0; j < path.size(); j++) printf("%d ", path[j]);
            create_module(graph_of_modules, modules, path);
            printf("module created\n");
            printf("\n Graph matrix\n");
            for(int i = 0; i < graph_of_modules.size(); i++){
                for(int j = 0; j < graph_of_modules[i].size(); j++){
                    printf("%d ",graph_of_modules[i][j]);
                }
                printf("\n");
            }
            path.clear();
            path.push_back(i);
        }
        //printf("next node\n");
        path.clear();
    }
    //printf("looking for jumps\n");
    for(int i = 0; i < modules.size() - 1; i++){    //looking for jumps
        path.push_back(i);
        while(modules[i].size() + 1 < DEPT && find_jump(graph_of_modules, path, modules)){
            printf("\nfinal path: ");
            for(int j = 0; j < path.size(); j++) printf("%d ", path[j]);
            create_module(graph_of_modules, modules, path);
            printf("module created\n");
            printf("\n Graph matrix\n");
            for(int i = 0; i < graph_of_modules.size(); i++){
                for(int j = 0; j < graph_of_modules[i].size(); j++){
                    printf("%d ",graph_of_modules[i][j]);
                }
                printf("\n");
            }
            path.clear();
            path.push_back(i);
        }
        //printf("next node\n");
        path.clear();
    }

    for(int x = 0; x < modules.size(); x++){
        printf("\nmodule%d: ", x);
        for(int y = 0; y < modules[x].size(); y++){
            printf("%c%d ", modules[x][y].type, modules[x][y].setting);
        }
    }
    printf("\n");
}

void modeler(vector<group> groups_p, vector<vector<operation>>& modules){
    for(int i = 0; i < groups_p.size(); i++){
        printf("group %d: ", i);
        vector<vector<operation>> temp(groups_p[i].operations.size());
        group_modeler(groups_p[i], temp);
        modules.insert(modules.end(), temp.begin(), temp.end());
    }
    sort(modules.begin(), modules.end(),
        [](vector<operation> a, vector<operation> b) {
            return (a.size() < b.size());
        }
    );
    /*for(int x = 0; x < modules.size(); x++){
        printf("\nmodule%d: ", x);
        for(int y = 0; y < modules[x].size(); y++){
            printf("%c%d ", modules[x][y].type, modules[x][y].setting);
        }
    }
    printf("\n");*/
    printf("\n-----------remaking modules----------\n");
    for(int i = 0; i < modules.size(); i++){
        if(modules[i].size() == 1){
            for(int x = i + 1; x < modules.size(); x++){
                for(int y = 0; y < modules[x].size(); y++){
                    if(modules[i].front() == modules[x][y]){
                        //printf("%c%d in module%d found in module%d\n", modules[i][0].type, modules[i][0].setting, i, x);
                        modules.erase(modules.begin() + i);
                        i--;
                        x = modules.size();
                        break;
                    }
                }
            }
        }
        else {
            for(int j = 0; j < modules[i].size(); j++){
                for(int x = i + 1; x < modules.size(); x++){
                    for(int y = 0; y < modules[x].size(); y++){
                        if(modules[i][j] == modules[x][y]){
                            printf("%c%d in module%d erased found in module%d\n", modules[x][y].type, modules[x][y].setting, x, i);
                            modules[x].erase(modules[x].begin() + y);
                            x = modules.size();
                            break;
                        }
                    }
                }
            }
            sort(modules.begin(), modules.end(),
            [](vector<operation> a, vector<operation> b) {
                    return (a.size() < b.size());
                });
        }
    }
    for(int x = 0; x < modules.size(); x++){
        printf("\nmodule%d: ", x);
        for(int y = 0; y < modules[x].size(); y++){
            printf("%c%d ", modules[x][y].type, modules[x][y].setting);
        }
    }
    printf("\n");
}

vector<pair<int, vector<operation>>> sorting_modules(vector<vector<operation>>& modules, vector<product> prd_list){
    vector<vector<int>> graph_of_modules;
    for (int i = 0; i < modules.size(); i++){
        graph_of_modules.assign(modules.size() + 1, vector<int>(modules.size() + 1));
        for(int j = 0; j < graph_of_modules[i].size(); j++){
            graph_of_modules[i][j] = 0;
        }
    }
    for(int i = 0; i < prd_list.size(); i++){
        int a;
        int b;
        for(int x = 0; x < modules.size(); x++){
            for(int y = 0; y < modules[x].size(); y++){
                if(prd_list[i].instruction.front() == modules[x][y]){ a = x; x = modules.size(); break;}
            }
        }
        for(int x = 0; x < modules.size(); x++){
            for(int y = 0; y < modules[x].size(); y++){
                if(prd_list[i].instruction.back() == modules[x][y]){ b = x; x = modules.size(); break;}
            }
        }
        graph_of_modules.back()[a] += 1;
        graph_of_modules[b].back() += 1;
        for(int j = 0; j < prd_list[i].instruction.size() - 1; j++){
            operation cur_oper = prd_list[i].instruction[j];
            operation next_oper = prd_list[i].instruction[j + 1];
            for(int x = 0; x < modules.size(); x++){
                for(int y = 0; y < modules[x].size(); y++){
                    if(cur_oper == modules[x][y]){ a = x; x = modules.size(); break;}
                }
            }
            for(int x = 0; x < modules.size(); x++){
                for(int y = 0; y < modules[x].size(); y++){
                    if(next_oper == modules[x][y]){ b = x; x = modules.size(); break;}
                }
            }
            if(a != b) graph_of_modules[a][b] += 1;
        }
    }
    printf("\n Graph matrix\n");
    for(int i = 0; i < graph_of_modules.size(); i++){
        for(int j = 0; j < graph_of_modules[i].size(); j++){
            printf("%d ",graph_of_modules[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    vector<pair<int, vector<operation>>> sorted_modules;
    for(int i = 0; i < modules.size(); i++){
        int delta = 0;
        for(int j = 0; j < modules[i].size(); j++){
            delta += graph_of_modules[i][j]>0?1:0;
            delta -= graph_of_modules[j][i]>0?1:0;
        }
        sorted_modules.push_back(pair<int, vector<operation>>(delta, modules[i]));
        //printf("%d : %d\n", i, delta);
    }
    for(int i = 0; i < sorted_modules.size(); i++){
        sorted_modules[i].first *= 100;
        int delta = 0;
        for(int j = 0; j < sorted_modules.size(); j++){
            delta += graph_of_modules[i][j];
            delta -= graph_of_modules[j][i];
        }
        delta -= graph_of_modules[i].back();
        delta += graph_of_modules.back()[i];
        sorted_modules[i].first += delta;
        printf("%d : %d\n", i, sorted_modules[i].first);
    }
    sort(sorted_modules.begin(), sorted_modules.end(), 
        [modules, graph_of_modules](pair<int, vector<operation>> a, pair<int, vector<operation>> b) {
            if(a.first == b.first){
                int x = find(modules.begin(), modules.end(), a.second) - modules.begin();
                int y = find(modules.begin(), modules.end(), b.second) - modules.begin();
                if(x < y && graph_of_modules[x][y]) return false;
                else return true;
            }
            return (a.first > b.first);
        });
    for (vector<pair<int, vector<operation>>>::iterator it = sorted_modules.begin(); it != sorted_modules.end(); it++){
        printf("module%d : ", (int)distance(sorted_modules.begin(), it));
        for(int i = 0; i < it->second.size(); i++){
            printf("%c%d ",it->second[i].type, it->second[i].setting);
        }
        printf("\n");
    }
    return sorted_modules;
}