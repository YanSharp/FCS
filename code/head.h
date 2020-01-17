#include <stdio.h>
#include <vector>
#include <map>
#include <locale.h>
#include <string>
#include <algorithm>
#include <stdarg.h>

using namespace std;

#define DEPT 5

struct operation
{
    char type;
    int setting;

    friend bool operator == (operation c1, operation c2){
        return ((c1.type == c2.type) && (c1.setting == c2.setting));
    }
    friend bool operator != (operation c1, operation c2){
        return ((c1.type != c2.type) && (c1.setting != c2.setting));
    }
};

struct product
{
    int id;
    vector<operation> instruction;
};

struct uniq_operation{
    operation element;
    int hp = 1;

    uniq_operation(operation operation_p){
        element = operation_p;
        hp = 1;
    }

    uniq_operation(const uniq_operation &u_op){
        element = u_op.element;
        hp = u_op.hp;
    }
};

struct group
{
    vector<product> products;
    vector<uniq_operation> operations;

    group(){}
    group(product prd){products.push_back(prd);}

    friend bool operator > (group c1, group c2){
        return (c1.operations.size() > c2.operations.size());
    }
    friend bool operator < (group c1, group c2){
        return (c1.operations.size() < c2.operations.size());
    }
    friend bool operator == (group c1, group c2){
        return (c1.operations.size() == c2.operations.size());
    }
    friend bool operator != (group c1, group c2){
        return (c1.operations.size() != c2.operations.size());
    }
};

void read_prd_list(vector<product>& prd_list, vector<operation>& opr_list);

vector<group> to_group(vector<product> prd_list, int **similarity_table);
void group_sort(vector<group>& groups);
void re_group(vector<group>& groups);
void print_groups(vector<group> groups);

void modeler(vector<group> groups_p, vector<vector<operation>>& modules);
vector<pair<int, vector<operation>>> sorting_modules(vector<vector<operation>>& modules, vector<product> prd_list);