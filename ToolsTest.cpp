// #include<iostream>
// #include<fstream>

// #include"KoppJson.h"
// #include"KoppTools.h"

// using namespace std;
// void test1(){
//     fstream file("aaa.txt");
//     string dd;
//     getline(file , dd , '\0');

//     KoppJson json;
//     string input = "[false,false,[true,true],false,[[true]]]";
//     json.setJsonStr(dd);
//     int ret = json.BuildJson();
//     string ret_str;
//     json.ToLevelStr( ret_str);


//     //cout<<ret_str<<endl;
//     json.get("tasks" , 0 , "args",1);
//     string res;
//     json.JP.backword();
//     json.JP.get_ptr()->make("\"error\"");
//     json.JP.get_ptr()->make("null");
//     cout<<json.JP.get_ptr()->del(2)<<"SS"<<endl;
//     json.JP.get_ptr()->chg(2,"\"Hello world\"");
//     json.JP.get_ptr()->chg("dd","\"Hell world\"");
//     json.JP.get_ptr()->del(6);
//     // json.JP.get_ptr()->del(6);

//     json.JP.backword();
//     json.JP.get_ptr()->del("tyvpe");
//     json.JP.get_ptr()->make("\"Name\" :  \"Tom\"");
//     json.JP.get_ptr()->chg("options" , "null");
//     json.JP.get_ptr()->chg(4 , "null");
//     cout<<'\n'<<json.JP.get_ptr()->tostr(res,1);
//     cout<<'\n'<<res<<'\n';
//     // KoppValue* ptr;
//     // cout<<json.this_val->get(ptr , 0 , "args",1)<<endl;
//     // string res;
//     // cout<<"\n\n"<<ptr->tostr(res,1);
//     // cout<<'\n'<<res<<'\n';

// }


// int main(){
//     // KoppTools t;
//     // t.open("aaa.txt");
//     // t.p->get("\"tasks\"" , 0 , "args" , 2);
//     // //cout<<t.p->JP.get_ptr()->get_str(1);
//     // t.dir();
//     //cout<<t.p->get_path();
//     // string tw = "dir " ;
//     // std::string::iterator tee = tw.begin();
//     // cout<<parse_op(tee);
//     cout<<"Hello world";
//     test1();
//     //t.dir();
// }