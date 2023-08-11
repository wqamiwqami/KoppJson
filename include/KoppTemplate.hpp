#include"KoppJson.h"

//函数模板必须写在头文件里,没得办法
//改了种方法,让.h导入了这个文件

//使用递归的方式获取相应的值 , 首值是string
//思路就是用一根ptr指针,获取目标位置,再在目标位置寻找新的目标位置,把指针挪过去
template<typename ... Args>
int KoppValue::get(KoppValue* &ptr , std::string first , Args... args){
    int ret = get(ptr , first);

    if(ret != KOPP_GET_OK) 
        return ret;
    return ptr->get(ptr , args...);
}

//使用递归的方式获取相应的值 , 首值是int
template<typename ... Args>
int KoppValue::get(KoppValue* &ptr,int first , Args... args){
    int ret = get(ptr , first);

    if(ret != KOPP_GET_OK) 
        return ret;
    return ptr->get(ptr , args...);
}

template<typename ... Args>
int KoppValue::get(KoppValue* &ptr , const char* first , Args... args){
    return this->get(ptr , std::string(first) , args...);
}


//递归调用,更新JP
template<typename ... Args>
int KoppValue::get(KoppJsonPtr& JP , int first , Args... args){
    int ret = get(JP , first);

    if(ret != KOPP_GET_OK) 
        return ret;
    return JP.get_ptr()->get(JP , args...);
}

template<typename ... Args>
int KoppValue::get(KoppJsonPtr& JP , std::string first , Args... args){
    int ret = get(JP , first);

    if(ret != KOPP_GET_OK) 
        return ret;
    return JP.get_ptr()->get(JP , args...);
}

template<typename ... Args>
int KoppValue::get(KoppJsonPtr& JP , const char* first , Args... args){
    return this->get(JP , std::string(first) , args...);
}
