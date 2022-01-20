#ifndef _HEAD_SERVER_H_
#define _HEAD_SERVER_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sqlite3.h>
#include <time.h>
#include<json/json.h>


typedef struct data_package{
    char cmd[50];
    char account[50];
    char account_name[50];
    char account_password[50];
    char recv_account[50];
    char send_message[50];
    char recv_message[200];///客户端接收的信息
    char send_account[50];///谁发给客户端的
    char time[50];
}datap_t;   //打包解包的结构体

typedef struct fd_password{
    int fd;
    char password[50];
    char my_name[50];
    int ddd;
}fd_password_t;

typedef struct two_account{
    char i_account[20];
    char he_account[20];
    int i_fd;
    char data[100];
    char time[20];
    char my_name[20];
    int ddd;
}two_account_t;

typedef struct oneself{
    int my_fd;
    char my_account[7];
    char my_name[20];
    char myqun_name[20];
    char qun_num[7];
    char he_acc[7];
    char data[100];
    char time[20];
    char man[50];
}oneself_t;///////////////存自己信息方便传参

typedef struct dd{
    int fd;
    int ddd;
}dd_t;

sqlite3 * mydb; //数据库


extern void * user(void * arg);

extern void set_up_user(int * fd,char * name,char * password);//注册qq

extern void generate_account(char * p);//生成账号

extern int kong_fun(void * arg,int col,char ** str,char **name);    //创建账号时查询该账号是否存在于数据库中

extern int kong_fun1(void * arg,int col,char ** str,char **name);    //创建账号时查询该账号是否存在于数据库中

//extern int login_find_acc(void * arg,int col,char ** str,char **name);  //验证密码

extern int login_find_acc(void * arg,int col,char ** str,char **name);   //登录查找函数

extern int find_i_inf(void * arg,int col,char ** str,char ** name);   //返回个人信息函数

extern int friend_i_inf(void * arg,int col,char ** str,char ** name);   //返回好友信息函数

extern int qun_i_inf(void * arg,int col,char ** str,char ** name);   //返回群信息函数

extern int add_find_fri(void * arg,int col,char ** str,char ** name);   //添加好友查找

extern const char * data_package(datap_t* datap1);//打包函数

extern void data_unpack(const char * arg,datap_t* datap1);//解包函数

extern int login_fun(int * fd,char * account,char * password,char * my_name);      //登录函数

extern void off_fun(char * account);   ///离线函数

extern void log_fd(int fd,char * account);  //登录改fd

extern void ret_inf(int fd,char * account);  //登录后返回信息函数
 
extern void add_friend(two_account_t * two_inf);         //添加好友函数

extern void server_go(int fd,char * data,char * from);         //服务器发送信息的函数

extern void view_friend(char * account);//查看好友

extern int zhuang_t(void * arg,int col,char ** str,char **name);   //好友状态修正

extern int zhuang_t1(void * arg,int col,char ** str,char **name);   //好友状态修正1

extern void goto_data(two_account_t * two_inf);      //与好友私聊的函数

extern int goto_data4(two_account_t * two_inf);     //判断是否是自己的好友

extern int goto_data2(void * arg,int col,char ** str,char **name);   //与好友私聊的函数2

extern void * goto_data1(void * arg);  //与好友私聊的线程函数1

extern int goto_data3(void * arg,int col,char ** str,char **name);   //与好友私聊的函数3

extern void viewfnew(two_account_t * two_inf);  //查看聊天记录

extern int viewfnew_sql(void * arg,int col,char ** str,char **name);   //查看聊天记录1

extern int cat_fd(void * arg,int col,char ** str,char **name);   //获取在线好友fd

extern void lixian_news(oneself_t * oneself_inf);    //离线期间的消息获取

extern int lixian_news1(void * arg,int col,char ** str,char **name);   //离线期间的消息获取1

extern int lixian_news2(void * arg,int col,char ** str,char **name);   //离线期间的消息获取2

extern void cregchat(oneself_t * oneself_inf);  //创建群聊返回群号

extern void addgmem(oneself_t * oneself_inf);  //拉群成员的函数

extern int addgmem1(void * arg,int col,char ** str,char **name);   //拉群成员的函数1

extern int addgmem2(void * arg,int col,char ** str,char **name);   //拉群成员的函数2

extern int addgmem3(void * arg,int col,char ** str,char **name);   //拉群成员的函数3

extern int addgmem4(void * arg,int col,char ** str,char **name);   //拉群成员的函数4
    
extern void gchat0(oneself_t * gchat);    //群聊函数

extern int gchat1(void * arg,int col,char ** str,char **name);   //查找群成员的函数

extern void lixian_qunnews(oneself_t * gchat);    //离线期间的群消息获取

extern int lixian_qunnews1(void * arg,int col,char ** str,char **name);   //离线期间的群消息获取1

extern void viewgmen0(oneself_t * viewgmen);      //查看群成员的函数

extern int viewgmen1(void * arg,int col,char ** str,char ** name);   //查看群成员的函数1

extern void setbanned0(oneself_t * setbanned);      //设置禁言0

extern int setbanned1(void * arg,int col,char ** str,char ** name);   //设置禁言1

extern void unsetbanned0(oneself_t * unsetbanned);      //解除禁言0

extern int unsetbanned1(void * arg,int col,char ** str,char ** name);   //解除禁言1

extern void addgadmin0(oneself_t *addgadmin);//设置群管理员

extern void delfriend0(two_account_t * delfriend);     //删除好友的函数0

extern void delgmem0(oneself_t * delgmem); //踢群成员的函数0

#endif
