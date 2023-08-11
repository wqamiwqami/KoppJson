#include<iostream>
#include<fstream>
#include<iomanip>
#include<algorithm>

#include"KoppJson.h"
#include"KoppTools.h"

std::vector<std::string>kopp_op_strs = {"noop" , "open" , "json" , 
    "cd" , "dir" , "tree" , "str" , "type" , "mk" , "chg" , "del"};


// 就是cout,后一个为真则换行
void show(const char* str , bool endl = false){
    std::cout<<str;
    if(endl) std::cout<<std::endl;
}

// 输出类型,由kopp_type转换成字符串
std::string type_str(kopp_type& type){
    switch (type){
        case KOPP_NULL   : return std::string("null"  );
        case KOPP_FALSE  : return std::string("bool"  );
        case KOPP_TRUE   : return std::string("bool"  );
        case KOPP_NUMBER : return std::string("number");
        case KOPP_ARRAY  : return std::string("array" );
        case KOPP_OBJECT : return std::string("object");
        case KOPP_STRING : return std::string("string");
        default          : return std::string(""      );
    }
}

// 显示下属对象,如果为obj则为 <n pair> , n是pair的数量
// 默认会对过长字符串裁切 , cut = false则不会裁切
std::string show_dir_val(KoppValue*vp , bool cut = true){
    switch (vp->type){
        case KOPP_NULL   : return std::string("null"  );
        case KOPP_FALSE  : return std::string("false" );
        case KOPP_TRUE   : return std::string("true"  );
        default          : return std::string(""      );
        case KOPP_NUMBER : return      std::to_string(*(double*)vp->vp);
        case KOPP_ARRAY  : return "<"+ std::to_string(vp->get_size()  ) + " val >";
        case KOPP_OBJECT : return "<"+ std::to_string(vp->get_size()  ) + " pair>";
        
        case KOPP_STRING : {
            //裁切操作
            if((!cut) || (*(std::string*)(vp->vp)).size() <= 20) 
                return (*(std::string*)(vp->vp));
            return (*(std::string*)(vp->vp)).substr(0,18)+"...";
        }
    }
}

// 设置窗口使用utf8编码
void setUTF_8(){
#ifdef _WIN32
    //http://m.blog.csdn.net/article/details?id=52789570
    //控制台显示乱码纠正
   // system("chcp 65001"); //设置字符集（使用SetConsoleCP(65001)设置无效，原因未知）
    SetConsoleOutputCP (65001);
    // CONSOLE_FONT_INFOEX info = { 0 }; // 以下设置字体来支持中文显示。
    // info.cbSize = sizeof(info);
    // info.dwFontSize.Y = 16; // leave X as zero
    // info.FontWeight = FW_NORMAL;
    // wcscpy(info.FaceName, L"Consolas");
    // SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
#endif
}


KoppTools::KoppTools(){
    setUTF_8();
    this->p           = new KoppJson() ;
    this->jsonDefined =     false ;
    this->error       =     KOPP_NO_ERROR ;
    this->current_op  =     KOPP_NO_OP ;
}

// 打开一个文件
void KoppTools::open(std::string file_path){
    if(this->jsonDefined){
        show("已经打开了一个文件,请先保存当前文件",1);
        return;
    }

    std::fstream file(file_path);
    if(!file.is_open()){
        show("打开文件失败,请检查文件路径是否正确",1);
    }

    //获取文件字符
    std::string str;
    std::getline(file , str , '\0');
    file.close();

    //构建json对象
    this->p->setJsonStr(str);
    this->error = this->p->BuildJson(0);

    if(error == KOPP_PARSE_OK){
        this->jsonDefined = true;
        show("打开文件成功!",1);
    }else{
        show("打开文件失败,解析失败" , 1);
        this->show_error();
    }
}

// 使用文本来创建json
void KoppTools::json(std::string json_str){
    if(this->jsonDefined){
        show("已经打开了一个文件,请先保存当前文件",1);
        return;
    }

    this->p->setJsonStr(json_str);
    this->error = this->p->BuildJson(0);

    if(error == KOPP_PARSE_OK){
        this->jsonDefined = true;
        show("解析成功!",1);
    }else{
        show("json文本解析失败" , 1);
        this->show_error();
    }
}

// 展示当前位置下所有元素
void KoppTools::dir(){
    //不为arr和obj只展示当前的
    if(p->ptr()->type != KOPP_ARRAY && p->ptr()->type != KOPP_OBJECT)
        std::cout<<std::endl<<show_dir_val(this->p->ptr())<<std::endl;

    //输出格式化
    std::cout<<std::setiosflags (std::ios::fixed)
             <<std::setprecision(6              )
             <<std::setiosflags (std::ios::left );

    // arr输出,注意要用p->ptr()->vp , 而不是p->ptr().
    if(p->ptr()->type == KOPP_ARRAY){
        int index = 0;
        std::cout<<"\n\033[32mArray dir:\nindex     type      value\033[0m"
                 <<std::endl;

        for(KoppValue* val:((KoppArray*)p->ptr()->vp)->val_list){

            std::cout<<std::setw(10)<<            (index++  )
                     <<std::setw(10)<<type_str    (val->type)
                     <<std::setw(25)<<show_dir_val(val      )
                     <<std::endl;
        }
    }

    // obj输出
    if(p->ptr()->type == KOPP_OBJECT){
        std::cout<<"\n\033[32mObject dir:\nkey            type      value\033[0m"
                 <<std::endl;

        for(KoppPair* pair:((KoppObject*)(p->ptr()->vp))->pair_list){

            // 裁剪过长
            std::string key = (*(std::string*)(pair->getKey()->vp));
            if(key.size() >= 12) 
                key = key.substr(0,10)+"...";
                
            std::cout<<std::setw(15)<<            (key                 )
                     <<std::setw(10)<<type_str    (pair->getVal()->type)
                     <<std::setw(25)<<show_dir_val(pair->getVal()      )
                     <<std::endl;
        }
    }

}

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
//         if(*ch == str[0]+32 && *(ch+1) == str[1]+32){
//             return parse_op_literal(kopp_op(i) , ch , capitalizeString(str).c_str());
//         }
//         return KOPP_NO_OP;
        
//     }
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


// std::vector<std::string> KoppTools::spliter(std::string input){
//     bool hasQuoting = false; //引号
//     std::string::iterator ch = input.begin();
//     parse_whitespace(ch);

//     while(ch != input.end()){

//     }
// }