#include<fstream>
#include<vector>
#include<iostream>
#include<string>
#include <algorithm>


#include"KoppJson.h"

#ifdef _WIN32
    #include <Windows.h>
#endif

#ifndef KOPPTOOLS_H__
#define KOPPTOOLS_H__

typedef enum {
    KOPP_NO_OP = 0,   // 无操作
    KOPP_OPEN_OP = 1, // 打开文件操作
    KOPP_JSON_OP = 2, // 使用json字符串输入构建
    KOPP_CD_OP = 3,   // 指定位置
    KOPP_DIR_OP = 4,  // 展示
    KOPP_TREE_OP = 5, // 树形展示
    KOPP_STR_OP = 6,  // 字符展示
    KOPP_TYPE_OP = 7, // 获取类型
    KOPP_MK_OP = 8,   // make,   制造
    KOPP_CHG_OP = 9,  // change, 改变
    KOPP_DEL_OP = 10,  // delete, 删除

} kopp_op;//操作
 
extern std::vector<std::string>kopp_op_strs ;
// {"noop" , "open" , "json" , 
//    "cd" , "dir" , "tree" , "str" , "type" , "mk" , "chg" , "del"};

CONST INT OP_NUM = 10;


class KoppTools{
public:
    KoppTools();
    //~KoppTools();
    
    void open(std::string file_path); // 打开文件
    void json(std::string json_str ); // 使用字符串转换为json对象
    //void save();

    //void cd  (std::string path);
    void dir (); // 展示当前位置下所有元素
    // void tree();
    // void str ();
    // void type();
    

    // void mk     (std::string input);
    // void chg    (std::string input);
    // void del    (std::string input);
    
    // 展示错误码
    // TODO : 除了错误码之外还会打印错误信息
    void show_error(){std::cout<<"错误码: "<<error<<std::endl;};

// private:
    KoppJson* p ;           //主要指针,指向正在处理的json文件
    bool      jsonDefined ; //是否已经打开一个文件
    int       error ;       //是一个错误码
    kopp_op   current_op ;  //当前操作

    std::vector<std::string> spliter(std::string input);//分割字符串,然后交给相应的程序执行


};

// bool parse_whitespace(std::string::iterator& ch){
//     //  ,\t,\n,\r,
//     bool white = false;
//     while(*ch == ' ' || *ch == '\t' || *ch=='\n' || *ch=='\r'){
//         ch++;
//         white = true;
//     };
//     return white;
// }

// kopp_op parse_op_literal(kopp_op op, std::string::iterator& ch , const char* literal){
//     std::cout<<*ch;
//     size_t i = 0;
//     while(literal[i] != '\0'){
//         if(*ch != literal[i]){
//             return KOPP_NO_OP;
//         }
//         i++;
//         ch++;
//     }
//     if(!parse_whitespace(ch)) 
//         return KOPP_NO_OP;
//     return op;
// }


// //小写变大写
// std::string capitalizeString(std::string s)
// {
//     std::transform(s.begin(), s.end(), s.begin(),
//                    [](unsigned char c){ return std::toupper(c); });
//     return s;
// }

// kopp_op parse_op(std::string::iterator& ch){
//     for(size_t i = 0 ; i < OP_NUM ; i++){
//         std::string str = kopp_op_strs[i];
//         if (*ch == str[0] && *(ch+1) == str[1]){
//             return parse_op_literal(kopp_op(i) , ch , str.c_str());
//         }
//         if(*ch == str[0]-32 && *(ch+1) == str[1]-32){
//             return parse_op_literal(kopp_op(i) , ch , capitalizeString(str).c_str());
//         }

        
//     }
//     return KOPP_NO_OP;
//     // switch (*ch)
//     // {
//     // case 'o': return parse_op_literal(KOPP_OPEN_OP , ch , "open");
//     // case 'O': return parse_op_literal(KOPP_OPEN_OP , ch , "OPEN");
//     // case 'j': return parse_op_literal(KOPP_OPEN_OP , ch , "json");
//     // case 'J': return parse_op_literal(KOPP_OPEN_OP , ch , "Json");
//     // case 'c': { if(*(ch+1) = 'd')return parse_op_literal(KOPP_OPEN_OP , ch , "cd");
//     //             if(*(ch+1) = 'h')return parse_op_literal(KOPP_OPEN_OP , ch , "chg");                            }
//     // case 'C': { if(*(ch+1) = 'D')return parse_op_literal(KOPP_OPEN_OP , ch , "CD");
//     //             if(*(ch+1) = 'H')return parse_op_literal(KOPP_OPEN_OP , ch , "CHG");   
//     // case 'd': { if(*(ch+1) = 'i')return parse_op_literal(KOPP_OPEN_OP , ch , "dir");
//     //             if(*(ch+1) = 'e')return parse_op_literal(KOPP_OPEN_OP , ch , "del");                            }
//     // case 'D': { if(*(ch+1) = 'I')return parse_op_literal(KOPP_OPEN_OP , ch , "DIR");
//     //             if(*(ch+1) = 'E')return parse_op_literal(KOPP_OPEN_OP , ch , "DEL");  
//     // case 't': { if(*(ch+1) = 'r')return parse_op_literal(KOPP_OPEN_OP , ch , "tree");
//     //             if(*(ch+1) = 'y')return parse_op_literal(KOPP_OPEN_OP , ch , "type");                            }
//     // case 'T': { if(*(ch+1) = 'R')return parse_op_literal(KOPP_OPEN_OP , ch , "TREE");
//     //             if(*(ch+1) = 'Y')return parse_op_literal(KOPP_OPEN_OP , ch , "TYPE");    
    
//     // default:
//     //     break;
//     // }
// }





#endif