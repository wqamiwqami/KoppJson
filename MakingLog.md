8.5 - 8.8  
构造了基本的结构,  
实现了由字符串解析到结构,再从结构输出字符串  
实现了层次化输出  
完成了构造函数和复制构造函数  
创建了解析和字符串化的错误码(100 - 200)  
  
  
  
8.9  
实现了查找操作(get)  
实现了创建操作(make)  
实现了删除操作(del)  
增添了增删查的错误码(300 - 500)  
创建了KoppJsonPtr类,用于探测当前所处的位置  
为KoppJsonPtr构造了一个反向链表,便于位置回退  
正式启用了这个项目日志  



8.10  
完成了setLevel相关函数  
增添了修改(chg)错误码(600)  
完成了KoppValue的chg函数以及copy和clear函数  
创建了KoppTools.h文件,完成了KoppTools的主要函数定义  
完成了KoppTools的open和json函数,实现打开功能  
实现了dir函数,实现查看功能,实现了查看当前目录功能  
制定一套字符串规范   
1500行左右,添加注释,规范代码,今天结束任务  



8.11 
本周最后一天  
今天主要在学如何构建一个cpp项目  
学了一下cmake,使用mingw编译器+cmake成功编译运行了项目  
初步实现解析器(解析操作名称,分包)  
整理了一下项目目录  
  
KoppJson 1.2
├─include
│  ├─KoppJson.h      和json处理相关的定义  
│  ├─KoppTools.h     json处理工具,为命令行操作做准备  
│  └─KoppTemplate    函数模板实现  
├─src
│  ├─KoppBuild.cpp   从字符串构建json相关  
│  ├─KoppJsonPtr.cpp 指向处理位置的指针  
│  ├─KoppTools.cpp   json处理工具的实现
│  └─KoppValue.cpp   json数值操作相关(增删改查)
│
├─test      过去的test文件  
├─build     cmake文件  
├─main.cpp  主函数  
└─CMakeLists.txt  
  
再用git做一个项目管理吧,试一试  
尝试在其他机器也能获取文件进行项目管理  




TODO:
1. ~~实现change类,用于改变键值~~  
2. ~~初步构造一个Tool工具类~~  
3. 根据字符串解析规则,写一个解析器(针对外部对象)  
4. 实现Tools类的cd方法,以及cd和tree方法  
5. 在实现cd方法后,构造shell类  
6. 实现shell中命令行以cd方法和dir方法操作json  
7. 在Tools类实现增删改查,并映射到shell中  
8. 实现根据错误码输出错误信息,需要大量文本  


Thinks:  
最终命令行操作可以实现:  
  
~ >                       //初始  
~ > open("C:/test.json")  //打开文件  
~ > json([[ true] , true ]) //创建一个json对象  

//一些展示操作
test >
test > dir                //展示下属一级的对象  
test > tree               //以树形展示所有下属对象  
test > str                //以字符串展示所有下属对象  

//改变当前所处的位置  
test > cd name  
test/name > cd 0  
test/name/0 > cd ..  
test/name > cd 0 my  
test/name/0/my > cd ...  
test >  

//创建一个json对象  
test > mk "Hello":99  
test > mk "Hello"  

//删除  
test > del "name"  
test > del 0  

//改变  
test > ch 0 99  
test > ch "name" null  
 
字符串规范  
原则:针对内部,一律不需要最外层引号;针对外部,可以选择是否要引号.  

内部:  
除了有一个make函数(针对obj)会整个分析字符串 , 如 "key 1" : 12   
其余函数所需要的输入都是无外层引号的(get , del , make , chg)  

外部:
<!-- 可以选择是否要引号,用空格作为分割符  
考虑中,有两种情况需要引号 ,   
一种是值内带空格,这时需要引号来划定范围,如 Tom Jack  -> "Tom Jack"  
另一种是关键字 '.' (仅在cd中), 如果值内全是'.',  
默认为cd的关键字,其含义为当前位置(n个表示回退n-1次) -->  

1.2版本  
思考之后还是决定,外部对象(字符串对象)一律需要冒号,否则失效,  
分隔以非冒号内的空格都对象为准  

