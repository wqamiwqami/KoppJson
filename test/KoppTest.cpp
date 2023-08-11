#include<iostream>
#include<fstream>

#include"KoppJson.h"

#include"KoppTools.h"

using namespace std;

static int test_count = 0;
static int test_pass = 0;
extern bool showMemInfo;


//测试函数,line是调用的函数,用__LINE__即可, expect_num指期待的错误码 ,MemInfo指是否打印内存调用
void Expect(int line , string input , int expect_num ,string output="" , bool isNum = 0 , bool MemInfo = 0){
    showMemInfo=MemInfo;
    if(MemInfo) { std::cout<<"\nMemInfo:\n";};
    test_count+=2;
    KoppJson json;

    json.setJsonStr(input);

    int ret = json.BuildJson();

    if(ret == expect_num){test_pass++;}
    else{std::cout<<"line: "<<line<<"  expect: "<<expect_num<<" actual: "<<ret<<endl;return;};
    
    if(output == ""){test_count--;return;};

    json.ToJsonStr();
    string ret_str = json.getJsonStr();

    if(!isNum){
        if(ret_str == output){test_pass++;}
        else{std::cout<<"line: "<<line<<"  expect: "<<output<<" actual: "<<ret_str<<endl;};
    }else{
        if(stod(ret_str) == stod(output)){test_pass++;}
        else{std::cout<<"line: "<<line<<"  expect: "<<output<<" actual: "<<ret_str<<endl;};
    }
}

void ExpectGet(int line,string input , int expect_num,string output=""){
    test_count+=2;
    KoppJson json;
    json.setJsonStr(input);
    int ret = json.BuildJson();
}

// int main(){

//     std::cout<<"TEST START 1.0\n"<<endl;

//     Expect(__LINE__, "null" , KOPP_PARSE_OK , "null");
//     Expect(__LINE__, "true" , KOPP_PARSE_OK , "true");
//     Expect(__LINE__, "false " , KOPP_PARSE_OK , "false");

//     Expect(__LINE__, "falsee" , KOPP_PARSE_INVALID_VALUE);
//     Expect(__LINE__, "nu" , KOPP_PARSE_INVALID_VALUE);

//     Expect(__LINE__, "1121" , KOPP_PARSE_OK , "1121" , true);
//     Expect(__LINE__, "588" , KOPP_PARSE_OK , "588" , true);
//     Expect(__LINE__, "-13.6e3" , KOPP_PARSE_OK , "-13.6e3" , true);
//     Expect(__LINE__, "-13.6e30000" , KOPP_PARSE_NUMBER_TOO_BIG , "", true);
//     Expect(__LINE__, "0100" , KOPP_PARSE_INVALID_VALUE , "" , true);

//     Expect(__LINE__, "\"ab\"" , KOPP_PARSE_OK , "\"ab\"");
//     Expect(__LINE__, "\"a\n\rb\"" , KOPP_PARSE_OK , "\"a\n\rb\"");
//     Expect(__LINE__, "\"ab\\ad" , KOPP_PARSE_INVALID_STRING_ESCAPE);
//     Expect(__LINE__, "\"ab\\\\" , KOPP_PARSE_MISS_QUOTATION_MARK);

//     Expect(__LINE__, "  falsee" , KOPP_PARSE_INVALID_VALUE);
//     Expect(__LINE__, "  -13.6e3" , KOPP_PARSE_OK , "-13.6e3" , true);
//     Expect(__LINE__, "\n\n\r\"ab\\\\" , KOPP_PARSE_MISS_QUOTATION_MARK);

//     Expect(__LINE__, "[1]" , KOPP_PARSE_OK , "[1.000000]" );
//     Expect(__LINE__, "[ 1, 3,  4 ]" , KOPP_PARSE_OK , "[1.000000,3.000000,4.000000]" );
//     Expect(__LINE__, "[\"ad\",\"as\"]" , KOPP_PARSE_OK , "[\"ad\",\"as\"]" );
//     Expect(__LINE__, "[false,false,[true,true],false,[[true]]]" , KOPP_PARSE_OK , "[false,false,[true,true],false,[[true]]]");
//     Expect(__LINE__, "[false,false,[true,true],false,[[true]]" , KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET);
//     Expect(__LINE__, "[false,false,[true,true],false,[[true]],]" , KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET);
//     Expect(__LINE__, "[false,false," , KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,"");
//     Expect(__LINE__, "[false,false,]" , KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,"");

//     Expect(__LINE__, "{\"a\" : false}" , KOPP_PARSE_OK , "{\"a\":false}" );
//     Expect(__LINE__, "{\"a\" : false}" , KOPP_PARSE_OK , "{\"a\":false}" );

//     string input("{\"sites\": [{ \"name\":\"cnjc\" , \"url\":\"www.runoob.com\" }, { \"name\":\"google\" , \"url\":\"www.google.com\" }, { \"name\":\"wb\" , \"url\":\"www.weibo.com\" }]}");
//     Expect(__LINE__, input , KOPP_PARSE_OK , "{\"sites\":[{\"name\":\"cnjc\",\"url\":\"www.runoob.com\"},{\"name\":\"google\",\"url\":\"www.google.com\"},{\"name\":\"wb\",\"url\":\"www.weibo.com\"}]}" );

//     Expect(__LINE__, "{\"a\" : false" , KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET);
//     Expect(__LINE__, "{\"a\" : false , }" , KOPP_PARSE_MISS_COMMA_OR_SQUARE_BRACKET);
//     Expect(__LINE__, "{\"a\" : fal" , KOPP_PARSE_INVALID_VALUE);
//     Expect(__LINE__, "{\"a\" }" , KOPP_PARSE_MISS_COLON);
//     Expect(__LINE__, "{false : false }" , KOPP_PARSE_MISS_KEY);




//     std::cout<<"\nTEST END"<<endl;
    
//     std::cout<<endl<<"res:"<<test_pass<<'/'<<test_count;

// }