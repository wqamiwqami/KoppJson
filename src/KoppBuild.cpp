#include "KoppJson.h"
#include <iostream>
#include <string>  

/*从字符串创建(解析)json对象 和 从json对象生成字符串 相关*/



//是否打印内存信息
// extern bool showMemInfo;
bool showMemInfo = 0;

#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')


/*和解析KoppJson相关的函数,JsonStr->KoppValue(单一)*/
int KoppParseLiteral(kopp_type& type , std::string::iterator& ch , const char* literal ,  void* &res);
int KoppParseString(std::string::iterator& ch ,  void* &res);
void KoppParseWhitespace(std::string::iterator& ch);
// int KoppJson::KoppParseArray(std::string::iterator& ch ,  void* &res , int& level);
// int KoppJson::KoppParseObject(std::string::iterator& ch ,  void* &res, int& level);
// int KoppJson::build(KoppValue &val);
// int KoppJson::BuildJson();


/*和生成字符串相关函数,包括生成有层次的json字符串*/
// int KoppJson::tostr(std::string& res , bool hasLevel);
// int KoppJson::ToJsonStr();
// int KoppJson::ToLevelStr(std::string& res);
std::string strJsonStr(std::string src);//为字符串添加转义符和引号
// int KoppValue::tostr(std::string& res , bool hasLevel);
// int KoppArray::tostr(std::string &res , bool hasLevel);
// int KoppObject::tostr(std::string &res , bool hasLevel);


/*和内存管理相关 , creatInfo可以当show默认为真时,可以在new val时打印相关信息*/
void creatInfo(kopp_type type , void* in , bool show = showMemInfo);
// KoppValue::~KoppValue();



void creatInfo(kopp_type type , void* in , bool show){
    if(show) std::cout<<"new "<<type<<" "<<in<<'\n';
}

int KoppParseLiteral(kopp_type& type , std::string::iterator& ch , const char* literal ,  void* &res){
    size_t i = 0;
    while(literal[i] != '\0'){
        if(*ch != literal[i]){
            return KOPP_PARSE_INVALID_VALUE;
        }
        i++;
        ch++;
    }
    if(type == KOPP_NULL) {res = nullptr;}
    else{res = new bool(type == KOPP_TRUE);creatInfo(type,res);};

    return KOPP_PARSE_OK;
}

int KoppParseNumber(std::string::iterator& ch ,  void* &res){
    std::string::iterator p = ch;
    if (*p == '-') p++;

    if (*p == '0'){ 
        p++; 
        if (*p != '.') return KOPP_PARSE_INVALID_VALUE;
    }else {
        if (!ISDIGIT1TO9(*p)) return KOPP_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }

    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p)) return KOPP_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return KOPP_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    std::string num = "";

    while(ch != p){num+=*ch;ch++;};
    num+=*ch;
    
    double resdb;
    try{
        resdb = stod(num, NULL);
    }catch( std::exception &e){
        return KOPP_PARSE_NUMBER_TOO_BIG;
    }

    res = new double(resdb);
    creatInfo(KOPP_NUMBER , res);
    return KOPP_PARSE_OK;
}

int KoppParseString(std::string::iterator& ch ,  void* &res){
    std::string res_str = "";

    while(1){
        ch++;
        switch (*ch){
            case '\0':return KOPP_PARSE_MISS_QUOTATION_MARK;

            case '\\':{
                ch++;
                switch (*ch) {
                    case '\"':  res_str += '\"'; break;
                    case '\\':  res_str += '\\'; break;
                    case '/' :  res_str += '/' ; break;
                    case 'b' :  res_str += '\b'; break;
                    case 'f' :  res_str += '\f'; break;
                    case 'n' :  res_str += '\n'; break;
                    case 'r' :  res_str += '\r'; break;
                    case 't' :  res_str += '\t'; break;
                    default: return KOPP_PARSE_INVALID_STRING_ESCAPE;
                }
            }
            break;

            case '\"': {
                res = new std::string(res_str);
                creatInfo(KOPP_STRING , res);
                ch++;
                return KOPP_PARSE_OK;
            }

            default: {res_str += *ch; }
        }

    }
}

void KoppParseWhitespace(std::string::iterator& ch){
    //  ,\t,\n,\r,
    while(*ch == ' ' || *ch == '\t' || *ch=='\n' || *ch=='\r'){
        ch++;
    };
}

int KoppJson::KoppParseArray(std::string::iterator& ch ,  void* &res , int& level){
    KoppArray *arr = new KoppArray;
    arr->level = level+1;
    creatInfo(KOPP_ARRAY , arr);
    ch++;
    int ret = KOPP_PARSE_OK;;
    //KoppParseWhitespace(ch);
    while(1){
        KoppValue* val = new KoppValue();
        val->level = level+1;
        ret = this->build(*val);
        if(ret != KOPP_PARSE_OK) goto EXIT;
        arr->push_back(val);

        KoppParseWhitespace(ch);
        if(*ch != ',') break;
        ch++;
        KoppParseWhitespace(ch);

        if(*ch == ']' || ch == this->str.end()){
            ret =  KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
            goto EXIT;
        };
    }

    if(*ch != ']'){
        ret =  KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
        goto EXIT;
    }

EXIT:
    res = (void*) arr;
    ch++;
    
    return ret;
}

int KoppJson::KoppParseObject(std::string::iterator& ch ,  void* &res, int& level){
    KoppObject *obj = new KoppObject();
    obj->level = level+1;
    creatInfo(KOPP_OBJECT , obj);
    ch++;
    int ret = KOPP_PARSE_OK;

    while(1){
        KoppValue* key = new KoppValue();
        key->level = level+1;
        KoppParseWhitespace(ch);
        if(*ch != '"'){ 
            ret = KOPP_PARSE_MISS_KEY;
            goto EXIT;
        }
        
        ret = KoppParseString(ch,key->vp);
        if(ret != KOPP_PARSE_OK) goto EXIT;
        key->type = KOPP_STRING;

        KoppParseWhitespace(ch);
        if(*ch != ':'){ 
            ret = KOPP_PARSE_MISS_COLON;
            goto EXIT;
        }
        ch++;

        KoppValue* val = new KoppValue();
        val->level = level+1;
        ret = this->build(*val);
        if(ret != KOPP_PARSE_OK) { delete key; goto EXIT; }

        KoppPair* pair = new KoppPair(key,val);
        obj->push_back(pair);

        KoppParseWhitespace(ch);
        if(*ch != ',') break;
        ch++;KoppParseWhitespace(ch);
        if(*ch == '}' || ch == this->str.end()){
            ret = KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
            goto EXIT;
        };
    }

    if(*ch != '}'){
        ret =  KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
        goto EXIT;
    }
    

EXIT:
    res = (void*) obj;
    ch++;
    return ret;
}

int KoppJson::build(KoppValue &val){
    KoppParseWhitespace(ch);
    switch (*ch){
        case 'f' : { val.type = KOPP_FALSE;  return KoppParseLiteral (val.type , ch , "false" , val.vp            ); }
        case 't' : { val.type = KOPP_TRUE;   return KoppParseLiteral (val.type , ch , "true"  , val.vp            ); }
        case 'n' : { val.type = KOPP_NULL;   return KoppParseLiteral (val.type , ch , "null"  , val.vp            ); }
        case '"' : { val.type = KOPP_STRING; return KoppParseString  (           ch ,           val.vp            ); }
        case '[' : { val.type = KOPP_ARRAY;  return KoppParseArray   (           ch ,           val.vp , val.level); }
        case '{' : { val.type = KOPP_OBJECT; return KoppParseObject  (           ch ,           val.vp , val.level); }
        default  : { val.type = KOPP_NUMBER; return KoppParseNumber  (           ch ,           val.vp            ); }
    }
}

int KoppJson::BuildJson(int rootLevel){
    ch = str.begin();
    VAL.level = rootLevel;//=0
    int ret = build(VAL);
    KoppParseWhitespace(ch);

    if(ch != str.end() && ret == KOPP_PARSE_OK) 
        return KOPP_PARSE_INVALID_VALUE;

    return ret;
};

int KoppJson::tostr(std::string& res , bool hasLevel){
    return this->VAL.tostr(res,hasLevel);
}

int KoppJson::ToJsonStr(){
    std::string res;
    int num = tostr(res);
    if(num == KOPP_TO_STR_OK) {this->str = res;}
    return num;
}

int KoppJson::ToLevelStr(std::string& res){
    int num = tostr(res , true);
    return num;
}

std::string strJsonStr(std::string src){
    std::string res(1,'\"');
    for(char ch:src){
        res+=ch;
        if(ch == '\\') res+=ch;
    }
    res+='\"';
    return res;
}

int KoppValue::tostr(std::string& res , bool hasLevel){

    int ret = KOPP_TO_STR_OK;
    switch (type){
        case KOPP_NULL   : {res = res + "null" ;                            return ret; };
        case KOPP_FALSE  : {res = res + "false";                            return ret; };
        case KOPP_TRUE   : {res = res + "true" ;                            return ret; };
        case KOPP_NUMBER : {res = res + std::to_string( *(double*)vp);      return ret; };
        case KOPP_STRING : {res = res + strJsonStr(*(std::string*)vp);      return ret; };
        case KOPP_ARRAY  : {ret = ((KoppArray*)vp) ->tostr(res , hasLevel); return ret; };
        case KOPP_OBJECT : {ret = ((KoppObject*)vp)->tostr(res , hasLevel); return ret; };
    }
    return KOPP_UNKNOWN_ERROR;//不可能返回这个

}

int KoppArray::tostr(std::string &res , bool hasLevel){
    int ret;
    res += "[";
    
    for(KoppValue* val:this->val_list){
        if(hasLevel) res+='\n'+std::string( 4*(this->level) , ' ');
        ret = val->tostr(res , hasLevel);
        if(ret != KOPP_TO_STR_OK) return ret;
        res+=',';
    }

    if(!hasLevel) res[res.size()-1] = ']';
    else{
        res[res.size()-1] = '\n';
        res+=std::string( 4*(this->level-1) , ' ') + "]";
    }
    return KOPP_TO_STR_OK;
}

int KoppObject::tostr(std::string &res , bool hasLevel){
    int ret;
    res+="{";


    for(KoppPair* pair:this->pair_list){
        if(hasLevel) res+='\n'+std::string( 4*(this->level) , ' ');
        ret = pair->getKey()->tostr(res , hasLevel);
        if(ret != KOPP_TO_STR_OK) return ret;
        res+=':';
        if(hasLevel) res+=' ';

        ret = pair->getVal()->tostr(res , hasLevel);
        if(ret != KOPP_TO_STR_OK) return ret;
        res+=',';
    }
    
    if(!hasLevel) res[res.size()-1] = '}';
    else{
        res[res.size()-1] = '\n';
        res+=std::string( 4*(this->level-1) , ' ') + "}";
    }
    
    return KOPP_TO_STR_OK;
}

std::string KoppValue::get_str(bool hasLevel){
    std::string res;
    this->tostr(res , hasLevel);
    return res;
}

KoppValue::~KoppValue(){
    switch(this->type){
        case KOPP_NULL   :                                 break;
        case KOPP_FALSE  : delete (bool*)        this->vp; break;
        case KOPP_TRUE   : delete (bool*)        this->vp; break;
        case KOPP_NUMBER : delete (double*)      this->vp; break;
        case KOPP_STRING : delete (std::string*) this->vp; break;
        case KOPP_ARRAY  : delete (KoppArray*)   this->vp; break;
        case KOPP_OBJECT : delete (KoppObject*)  this->vp; break;
    }

    if(showMemInfo) std::cout<<"delete "<<this->type<<" val"<<this->vp<<std::endl;
    this->type = KOPP_NULL;
    this->vp = nullptr;
}

