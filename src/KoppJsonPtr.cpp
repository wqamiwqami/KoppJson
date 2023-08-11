#include "KoppJson.h"
#include <iostream>
#include <string>

bool KoppJsonPtr::backword(){
    if(end == root) return false;
    node* last = end->last;
    delete end;
    end = last;
    return true;
}

//回溯n次,将回溯结果赋给相应指针
bool KoppJsonPtr::back(KoppValue* res , int n){
    node* ptr = end; 
    for(size_t i = 0 ; i<n ; i++){
        if(ptr == nullptr) return false;
        ptr = ptr->last;
    }
    res = ptr->ptr;
    return true;
}


//以test/name/0/my形式显示
std::string KoppJsonPtr::get_path(){
    node* ptr = end;
    std::string res = end->name;
    while(ptr != root){
        ptr = ptr->last;
        res = ptr->name + "\033[34m/\033[0m" + res;
    }
    return ("~" + res + ">");
}