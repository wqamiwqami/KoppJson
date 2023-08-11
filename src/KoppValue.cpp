#include "KoppJson.h"
#include <iostream>
#include <string>  

/*和json对象的一些数值操作相关*/

//复制构造
KoppValue::KoppValue(KoppValue& val){
    this->type = val.type;
    this->level = val.level;

    switch (val.type){
        case KOPP_NULL   : this->vp =                (nullptr              ); break;
        case KOPP_FALSE  : this->vp = new bool       (false                ); break;
        case KOPP_TRUE   : this->vp = new bool       (true                 ); break;
        case KOPP_NUMBER : this->vp = new double     (*(double*)     val.vp); break;
        case KOPP_STRING : this->vp = new std::string(*(std::string*)val.vp); break;
        case KOPP_ARRAY  : this->vp = new KoppArray  (*(KoppArray*)  val.vp); break;
        case KOPP_OBJECT : this->vp = new KoppObject (*(KoppObject*) val.vp); break;
    }
}

KoppArray::KoppArray(KoppArray& arr){
    this->level = arr.level;

    for(KoppValue* val:arr.val_list){
        KoppValue* new_val = new KoppValue(*val);
        this->push_back(new_val);
    }
}

KoppPair::KoppPair(KoppPair& pair){
    this->key   = new KoppValue(*pair.getKey());
    this->value = new KoppValue(*pair.getVal());
}

KoppObject::KoppObject(KoppObject& obj){
    this->level = obj.level;

    for(KoppPair* pair:obj.pair_list){
        KoppPair* new_pair = new KoppPair(*pair);
        this->push_back(new_pair);
    }
}


//KoppValue的构造函数
KoppValue::KoppValue()                 : type(KOPP_NULL               ), level(0) {vp = nullptr;              };
KoppValue::KoppValue(bool& val)        : type(val?KOPP_TRUE:KOPP_FALSE), level(0) {vp = new bool       (val); };
KoppValue::KoppValue(double& val)      : type(KOPP_NUMBER             ), level(0) {vp = new double     (val); };
KoppValue::KoppValue(std::string& val) : type(KOPP_STRING             ), level(0) {vp = new std::string(val); };
KoppValue::KoppValue(KoppArray  arr)   : type(KOPP_ARRAY              ), level(0) {vp = new KoppArray  (arr); };
KoppValue::KoppValue(KoppObject  obj)  : type(KOPP_OBJECT             ), level(0) {vp = new KoppObject (obj); };


//清空已有信息,成为null类型
bool KoppValue::clear(){
    if(this->type == KOPP_NULL)
        return false;
    
    switch (this->type)
    {
        case KOPP_FALSE  : delete (bool*)       this->vp ; break;
        case KOPP_TRUE   : delete (bool*)       this->vp ; break;
        case KOPP_NUMBER : delete (double*)     this->vp ; break;
        case KOPP_STRING : delete (std::string*)this->vp ; break;
        case KOPP_ARRAY  : delete (KoppArray*)  this->vp ; break;
        case KOPP_OBJECT : delete (KoppObject*) this->vp ; break;
        default : return false;
    }

    this->type = KOPP_NULL;
    level = 0;
    return true;
}

//复制另一个的,注意会先进行清除操作
void KoppValue::copy(KoppValue& val){
    this->clear();

    this->type = val.type;
    this->level = val.level;

    switch (val.type){
        case KOPP_NULL   : this->vp =                (nullptr              ); break;
        case KOPP_FALSE  : this->vp = new bool       (false                ); break;
        case KOPP_TRUE   : this->vp = new bool       (true                 ); break;
        case KOPP_NUMBER : this->vp = new double     (*(double*)     val.vp); break;
        case KOPP_STRING : this->vp = new std::string(*(std::string*)val.vp); break;
        case KOPP_ARRAY  : this->vp = new KoppArray  (*(KoppArray*)  val.vp); break;
        case KOPP_OBJECT : this->vp = new KoppObject (*(KoppObject*) val.vp); break;
    }
}

//获取长度,只对arr和obj有效
int KoppValue::get_size(){
    if(this->type == KOPP_ARRAY) 
        return ((KoppArray*)this->vp)->val_list.size();
    if(this->type == KOPP_OBJECT)
        return ((KoppObject*)this->vp)->pair_list.size();
    return 0;
};

//setLevel的递归调用
void KoppValue::setLevel(int level){
    this->level = level;//非arr和obj直接设置就行
    //调用,然后递归调用
    switch(this->type){
        case KOPP_ARRAY :{
            ((KoppArray*)this->vp)->setLevel(level);
            return;
        }

        case KOPP_OBJECT :{
            ((KoppObject*)this->vp)->setLevel(level);
            return;
        }
    }
}

void KoppPair::setLevel(int level){
    key->setLevel(level);
    value->setLevel(level);
};

void KoppObject::setLevel(int level){
    this->level = level;
    for(KoppPair* pair : pair_list){
        pair->setLevel(level+1);
    };
};

void KoppArray::setLevel(int level){
    this->level = level;
    for(KoppValue* val:val_list){
        val->setLevel(level+1);
    };
};


//获取指定位置的值,使用str获取,适用于obj,(默认转换可以转换为int , 也可以用于arr)
int KoppValue::get(KoppValue* &val,std::string key){
    switch(this->type){
        case KOPP_ARRAY:{
            // 判断是否是数字
            for(char ch:key){
                if((ch) < '0' || (ch) > '9'){
                    return KOPP_GET_ARRAY_NEED_INDEX;
                }
            }
            return get(val,std::stoi(key));
        }

        case KOPP_OBJECT:{
            for(KoppPair* pair: ((KoppObject*)this->vp)->pair_list){
                if(key == *pair->key_str){
                    val = pair->getVal();
                    return KOPP_GET_OK;
                }
            }
            return KOPP_GET_OBJECT_NO_THIS_KEY;
        }

        default :{
            return KOPP_GET_NEED_ARRAY_OR_OBJECT;
        }
    }
}

//获取指定位置的值,使用index获取,适用于arr,(默认转换也可以用于obj)
int KoppValue::get(KoppValue* &val , int key){
    if(this->type != KOPP_ARRAY && this->type != KOPP_OBJECT) 
        return KOPP_GET_NEED_ARRAY_OR_OBJECT;
    if(this->type == KOPP_OBJECT) 
        return get(val,std::to_string(key) );

    if(key >= ((KoppArray*)this->vp)->val_list.size() || key<0)
        return KOPP_GET_ARRAY_OUT_INDEX;
    
    val = ((KoppArray*)this->vp)->val_list.at(key);
    return KOPP_GET_OK;
}

int KoppValue::get(KoppValue*  &ptr , const char* key ){
    return get(ptr , std::string(key));
};

// 下面的函数是get方法针对KoppJsonPtr的特化(稍微改变了一些地方)
// 注意更新JP的操作
int KoppValue::get(KoppJsonPtr& JP , std::string key){
    KoppValue* res;
    int ret = this->get(res , key);
    if(ret != KOPP_GET_OK) return ret;//查找失效

    JP.forword(res , key);//更新JP , 后一个变量是名称,便于后续查看名称
    return ret;
}

int KoppValue::get(KoppJsonPtr& JP , int key){
    KoppValue* res;
    int ret = this->get(res , key);
    if(ret != KOPP_GET_OK) return ret;
    std::string str = std::to_string(key);

    JP.forword(res , str);//更新JP
    return ret;
}

int KoppValue::get(KoppJsonPtr &JP  , const char* key){
    return get(JP,std::string(key));
};


//检查obj是否有相应的key
bool KoppValue::check(std::string key){
    if(this->type != KOPP_OBJECT)
        return false;

    for(KoppPair* pair: ((KoppObject*)this->vp)->pair_list){
        if(key == *pair->key_str){
            return true;
        }
    }
    return false;
}

//检查arr是否有相应的下标
bool KoppValue::check(int key){
    if(this->type != KOPP_ARRAY)
        return false;

    if(key < ((KoppArray*)this->vp)->val_list.size() && key >= 0 )
        return true;
    return false;
}



//增添元素操作
//针对obj增添键值对
int KoppValue::makePair(std::string key , std::string value , int& error){
    if(this->type != KOPP_OBJECT) return KOPP_MAKE_NOT_OBJECT_OR_ARRAY;

    //key相关
    if(key.front()!='\"' && key.back() != '\"') return KOPP_MAKE_OBJECT_MISS_KEY;
    key = key.substr(1,key.size()-2);
    if(this->check(key)) return KOPP_MAKE_OBJECT_KEY_ALREADY_EXIST;

    KoppValue* key_p = new KoppValue(key);
    key_p->setLevel(this->level);

    //val相关
    KoppJson json;
    json.setJsonStr(value);
    error = json.BuildJson(this->level);

    if(error != KOPP_PARSE_OK){
        delete key_p;
        return KOPP_MAKE_PARSE_ERROR;
    }
    KoppValue* val_p = new KoppValue(json.VAL);

    //增添
    KoppPair* pair = new KoppPair(key_p , val_p);
    ((KoppObject*)this->vp)->push_back(pair);

    return KOPP_MAKE_OK;
}

int KoppValue::makePair(std::string key , std::string value){
    int error;
    return makePair(key , value,error);
}

//针对arr增添值
int KoppValue::makeVal(std::string value, int& error){
    if(this->type != KOPP_ARRAY) return KOPP_MAKE_NOT_OBJECT_OR_ARRAY;

    KoppJson json;
    json.setJsonStr(value);
    error = json.BuildJson(this->level);

    if(error != KOPP_PARSE_OK){
        return KOPP_MAKE_PARSE_ERROR;
    }

    KoppValue* val_p = new KoppValue(json.VAL);
    ((KoppArray*)this->vp)->push_back(val_p);

    return KOPP_MAKE_OK;
}

int KoppValue::makeVal(std::string value){
    int error;
    return makeVal(value,error);
}

//合二为一
int KoppValue::make(std::string in , int& error){
    if(this->type != KOPP_ARRAY && this->type != KOPP_OBJECT) 
        return KOPP_MAKE_NOT_OBJECT_OR_ARRAY;
    if(this->type == KOPP_ARRAY)
        return this->makeVal(in , error);
    
    //针对obj会进行分割操作
    if(in.front() != '\"') return KOPP_MAKE_OBJECT_MISS_KEY;
    for(size_t i = 1 ; i<in.size() ; i++){
        if(in[i] == '\"' && in[i-1] != '\\'){
            std::string key = in.substr(0 , i+1);

            while (in[++i] == ' ');
            if(in[i] != ':') return KOPP_MAKE_OBJECT_MISS_COLON;

            std::string val = in.substr(i+1);
            return makePair(key , val,error);
        }
    }
    return KOPP_MAKE_OBJECT_MISS_KEY;
}

int KoppValue::make(std::string in){
    int error;
    return make(in,error);
}


//删除操作
int KoppValue::del(int& error , std::string key){
    if(this->type != KOPP_ARRAY && this->type != KOPP_OBJECT)
        return KOPP_DEL_NEED_ARRAY_OR_OBJECT;
    KoppValue* VP;
    error = this->get(VP , key);

    if(error != KOPP_GET_OK) 
        return KOPP_DEL_GET_ERROR;
    
    if(this->type == KOPP_ARRAY){
        std::vector<KoppValue *>* list = &((KoppArray*)this->vp)->val_list;
        list->erase(list->begin()+stoi(key));
        return KOPP_DEL_OK;
    }

    //this->type == KOPP_OBJECT的情况下
    std::vector<KoppPair *>* list = &((KoppObject*)this->vp)->pair_list;
    for(size_t i = 0 ; i<list->size() ; i++){
        KoppPair* pair = list->at(i);
        if(key == *pair->key_str){
            list->erase(list->begin()+i);
            return KOPP_DEL_OK;
        }
    }
    
    return KOPP_UNKNOWN_ERROR;//能找到却删不掉..暂时没这种可能
}

int KoppValue::del(int& error , int key){
    return del(error , std::to_string(key));
}

int KoppValue::del(std::string key){
    int error;
    return del(error , key);
}

int KoppValue::del(int key){
    int error;
    return del(error , key);
}


//修改操作
int KoppValue::chg( int& error , std::string key , std::string new_val){
    if(this->type != KOPP_ARRAY && this->type != KOPP_OBJECT)
        return KOPP_CHG_NOT_ARRAY_OR_OBJECT;

    //寻找位置
    KoppValue* VP;
    error = this->get(VP , key);
    if(error != KOPP_GET_OK) 
        return KOPP_CHG_GET_ERROR;

    //解析目标值
    KoppJson json;
    json.setJsonStr(new_val);
    error = json.BuildJson(this->level);
    if(error != KOPP_PARSE_OK) 
        return KOPP_CHG_PARSE_ERROR;

    // 复制会自动调用clear函数,清除原有的对象
    // arr和obj都可以直接修改VP所指向的值来实现修改
    VP->copy(json.VAL);
    return KOPP_DEL_OK;
}

int KoppValue::chg( int& error , int key , std::string new_val){
    return chg( error , std::to_string(key) , new_val);
}

int KoppValue::chg(std::string key , std::string new_val){
    int error;
    return chg(error, key , new_val);
}

int KoppValue::chg(int key , std::string new_val){
    int error;
    return chg(error, key , new_val);
}

