#include<string>
#include<vector>
#include<iostream>

#ifndef KOPPJSON_H__
#define KOPPJSON_H__

//json值的类型
typedef enum {
    KOPP_NULL   , // null,没有值
    KOPP_FALSE  , // false
    KOPP_TRUE   , // true
    KOPP_NUMBER , // 这里使用的是浮点数(double)
    KOPP_STRING , // 字符串
    KOPP_ARRAY  , // 由元素构成,类似于 [ [1 , 3 , [2,5]] , [1] , false]
    KOPP_OBJECT   // 由键值对构成 , {"name":"tom" , "years":[1,2,3]}
} kopp_type;

//错误码
enum {
    KOPP_NO_ERROR                           = 0,   //默认无错误
    KOPP_UNKNOWN_ERROR                      = 999, //未知错误

    KOPP_PARSE_OK                           = 100, //解析成功
    KOPP_PARSE_INVALID_VALUE                = 101, //解析无效值, 如nul ,falss, 1e-1000
    KOPP_PARSE_NUMBER_TOO_BIG               = 102, //数字超出解析范围
    KOPP_PARSE_MISS_QUOTATION_MARK          = 103, //字符串遗漏引号，例如 "wd
    KOPP_PARSE_INVALID_STRING_ESCAPE        = 104, //无效的字符转义
    KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET = 105, //数组遗漏','或者']','}'
    KOPP_PARSE_MISS_KEY                     = 106, //对象遗漏key(key形式不规范)
    KOPP_PARSE_MISS_COLON                   = 107, //对象遗漏冒号

    KOPP_TO_STR_OK                          = 200, //转换字符串成功

    KOPP_GET_OK                             = 300, //获取值成功
    KOPP_GET_ARRAY_NEED_INDEX               = 301, //类型为array,需要数字下标
    KOPP_GET_ARRAY_OUT_INDEX                = 302, //下标超过了取值范围
    KOPP_GET_OBJECT_NO_THIS_KEY             = 303, //object没有找到相应的key
    KOPP_GET_NEED_ARRAY_OR_OBJECT           = 304, //需要列表或者对象(类型不是列表或对象)

    KOPP_MAKE_OK                            = 400, //创建成功
    KOPP_MAKE_PARSE_ERROR                   = 401, //解析错误,详细可见解析错误码
    KOPP_MAKE_NOT_OBJECT_OR_ARRAY           = 402, //当前位置不是arr或者obj
    KOPP_MAKE_OBJECT_MISS_KEY               = 403, //目标对象是obj,key格式不正确(遗漏引号)
    KOPP_MAKE_OBJECT_MISS_COLON             = 404, //目标对象是obj,遗漏冒号
    KOPP_MAKE_OBJECT_KEY_ALREADY_EXIST      = 405, //对应的key已经存在

    KOPP_DEL_OK                             = 500, //删除成功
    KOPP_DEL_NEED_ARRAY_OR_OBJECT           = 501, //删除需要是obj或者arr对象
    KOPP_DEL_GET_ERROR                      = 502, //没有找到相应的值,无法删除

    KOPP_CHG_OK                             = 600, //修改成功
    KOPP_CHG_GET_ERROR                      = 601, //没有找到相应的值,无法删除
    KOPP_CHG_NOT_ARRAY_OR_OBJECT            = 602, //处理位置不是arr或者obj
    KOPP_CHG_PARSE_ERROR                    = 603, //解析修改后的值失败
};

class KoppJson;
class KoppValue;
class KoppPair;
class KoppObject;
class KoppArray;
class KoppJsonPtr;

//json值
class KoppValue{
public:
    KoppValue();
    ~KoppValue();
    KoppValue(bool&        val);
    KoppValue(double&      val);
    KoppValue(std::string& val);
    KoppValue(KoppArray    arr);
    KoppValue(KoppObject   obj);
    KoppValue(KoppValue&   val);
    
    bool clear   ( );//清除原有信息
    void copy    ( KoppValue& val );//复制另一个
    void setLevel( int level );//setLevel应该递归调用,让下属所有元素都set
    int  get_size( );//获取尺寸,只适用于arr和obj
    int  tostr   ( std::string& res , bool hasLevel = false ); //生成字符串到目标位置

    std::string get_str(bool hasLevel = false);//生成字符串并输出

    //对obj和arr通用的生成
    //创建新的成员,只能用于obj对象,error可以返回解析错误码
    //创建新成员,只能用于arr
    int make    ( std::string in                        , int& error );
    int make    ( std::string in                                     );
    int makePair( std::string key   , std::string value , int& error );
    int makePair( std::string key   , std::string value              );
    int makeVal ( std::string value                     , int& error );
    int makeVal ( std::string value                                  );

    //删除指定成员
    int del( int& error , std::string key );
    int del( int& error , int         key );
    int del(              std::string key );
    int del(              int         key );

    //修改指定成员
    int chg( int& error , std::string key , std::string new_val );
    int chg( int& error , int         key , std::string new_val );
    int chg(              std::string key , std::string new_val );
    int chg(              int         key , std::string new_val );


    //获取成员,int用于arr,而str用于obj.(形势允许可以相互转换)
                                int get( KoppValue*  &ptr , std::string key                  );
                                int get( KoppValue*  &ptr , const char* key                  );
                                int get( KoppValue*  &ptr , int         key                  );
                                int get( KoppJsonPtr &JP  , std::string key                  );
                                int get( KoppJsonPtr &JP  , int         key                  );
                                int get( KoppJsonPtr &JP  , const char* key                  );
    template<typename ... Args> int get( KoppValue*  &ptr , int         first , Args... args );
    template<typename ... Args> int get( KoppValue*  &ptr , const char* first , Args... args );
    template<typename ... Args> int get( KoppValue*  &ptr , std::string first , Args... args );
    template<typename ... Args> int get( KoppJsonPtr &JP  , int         first , Args... args );
    template<typename ... Args> int get( KoppJsonPtr &JP  , const char* first , Args... args );
    template<typename ... Args> int get( KoppJsonPtr &JP  , std::string first , Args... args );

    //检查obj成员是否存在
    bool check( std::string key );
    bool check( int         i   );

    void*     vp;    //value_ptr,值的指针
    kopp_type type;  //值类型
    int       level; //深度,上层每有一个object或者array,深度+1
};


//json,值和名称组成的数对,object的组成
class KoppPair{
public:
    KoppPair(KoppValue* key,KoppValue* value):key(key),value(value)
        {key_str = (std::string*)key->vp;};
    KoppPair(KoppPair& pair);
    ~KoppPair(){delete key;delete value;};

    void setLevel(int level);

    KoppValue* getKey(){return key;  };
    KoppValue* getVal(){return value;};

    std::string* key_str; //指向key的string的指针
    KoppValue*   key;
    KoppValue*   value;
};


//json对象,由多个数对(KoppPair)构成
class KoppObject{
public:
    KoppObject(){};
    KoppObject(KoppObject& obj);
    ~KoppObject(){for(KoppPair* pair:pair_list){delete pair;};};

    void push_back(KoppPair* pair){pair_list.push_back(pair);};
    int  tostr    (std::string& res , bool hasLevel = false);
    void setLevel (int level);

    std::vector<KoppPair*> pair_list;
    int level;
};


//json数组,由多个值(KoppValue)构成
class KoppArray{
public:
    KoppArray(){};
    KoppArray(KoppArray& arr);
    ~KoppArray(){for(KoppValue* val:val_list){delete val;};};
    
    void push_back(KoppValue* val){val_list.push_back(val);};
    int  tostr    (std::string& res , bool hasLevel = false);
    void setLevel (int level);

    std::vector<KoppValue*> val_list;
    int level;
};


//探测指针,用于指向现在正在处理的JsonValue,并且指明源方向
class KoppJsonPtr{
    //构造了一个反向链表,便于回溯
    class node{
    public:
        node(KoppValue* ptr):last(nullptr),ptr(ptr){};
        node(node* last,KoppValue* ptr):last(last),ptr(ptr){};
        ~node(){};

        void set_name(std::string& name){this->name = name;};
        
        node*       last;
        std::string name;
        KoppValue*  ptr;
    };

public:
    KoppJsonPtr(){root = new node(nullptr);end = root;};
    KoppJsonPtr(KoppValue* VAL){set_root(VAL);};
    ~KoppJsonPtr(){to_root();delete root;};
    
    //向前,创建新的节点
    void forword(KoppValue* VAL                    ){end = new node(end , VAL);};
    void forword(KoppValue* VAL , std::string& name){forword(VAL);end->set_name(name);};

    void        set_root(KoppValue* VAL){root->ptr = VAL;};//设置根节点的级别
    bool        backword();//返回上一级(并释放空间)
    void        to_root (){ while(backword());}//返回根节点
    bool        back    (KoppValue* res , int n = 1);//回溯n次,将回溯结果赋给相应指针(不会释放空间)
    std::string get_path();//以test/name/0/my形式显示
    KoppValue*  get_ptr (){return end->ptr;};//获取当前位置的指针

    //根节点和尾节点
    node* end ;
    node* root;
};


//json主类
class KoppJson{
public:
    KoppJson(){JP.set_root(&this->VAL);};
    ~KoppJson(){};

    void        setJsonStr(std::string str){this->str = (str);};//设置JsonStr
    std::string getJsonStr(){return str;};

    /*返回值均为错误码*/

    //创建和输出
    int BuildJson (int rootLevel = 0);//创建Json值对象,即JsonStr->KoppValue val
    int ToJsonStr ();//KoppValue val -> JsonStr;
    int ToLevelStr(std::string& res);//创建有层次的string输出
    int build     (KoppValue &val);//供给BuildJson()调用
    int tostr     (std::string& res , bool hasLevel = false);//供给ToJsonStr()和ToLevelStr(std::string& res)调用

    //取值
    template<typename ... Args>
    int get( Args...     args ) { return this->VAL.get( this->JP , args... ); };
    int get( std::string key  ) { return this->VAL.get( this->JP , key     ); };
    int get( int         key  ) { return this->VAL.get( this->JP , key     ); };

    //供给build(KoppValue &val)调用,void* &res是KoppValue的vp对象
    int KoppParseArray (std::string::iterator& ch ,  void* &res , int& level);
    int KoppParseObject(std::string::iterator& ch ,  void* &res , int& level);

    //JP相关,调用当前位置
    KoppValue*  ptr()     {return JP.get_ptr(); };//获取位置指针
    std::string get_path(){return JP.get_path();};//获取路径(字符串形式)

    //成员变量
    KoppValue             VAL ;      //根节点
    std::string           str ;      //全局str
    std::string::iterator ch ;       //解析位置迭代器
    KoppJsonPtr           JP ;       //当前正在处理的val对象,探测指针
    std::string           this_str ; //当前正在处理的val对象的str值
};


//把函数模板导进来
#include"KoppTemplate.hpp"

#endif