
#include "head_server.h"


int mp = 1;   //返回信息表头标志
int my_fd = 999;

void * user(void * arg)
{
    int * p =(int *)arg;
    int client_fd = *p,tmp,cc_log = 999;
    char buf[500],man[20],account[20],account_password[20];
    char my_name[20]={};
    datap_t data_ball;
    datap_t kong_ball;
    oneself_t oneself_inf;
    while(1)
    {
	data_ball = kong_ball;
	bzero(man,sizeof(man));
	bzero(buf,sizeof(buf));
	tmp = recv(client_fd,buf,sizeof(buf),0);

	if(tmp == -1){
	    perror(" user recv error!\n");
	    exit(1);
	}else if(tmp == 0){
	    off_fun(account);                          //离线函数
	    break;
	}
	
	
	//把buf解包
	data_unpack(buf,&data_ball);

	
	
	//提取出操作指令赋值到man
	strcpy(man,data_ball.cmd);
	
	
	if(strncmp(man,"enroll",6) == 0){       //注册
	    set_up_user(&client_fd,data_ball.account_name,data_ball.account_password);
	}
	if(strncmp(man,"login",5) == 0){         //登录
	    strcpy(account,data_ball.account);//账号
	    //strcpy(my_account,account);
	    strcpy(account_password,data_ball.account_password);//密码
	    cc_log = login_fun(&client_fd,account,account_password,my_name);   //返回登录成功或失败
	    if(cc_log == 0){
		log_fd(client_fd,account);         //登录成功更改文件描述符函数
		my_fd = client_fd;
		oneself_inf.my_fd = client_fd;
		strcpy(oneself_inf.my_account,data_ball.account);
		strcpy(oneself_inf.my_name,my_name);
		view_friend( oneself_inf.my_account);       //更正好友状态
		ret_inf(client_fd,account);       //返回个人信息的函数
		lixian_news(&oneself_inf);          //返回离线期间的消息
		lixian_qunnews(&oneself_inf);           //返回离线期间的群消息
	    }
	}
	if(strncmp(man,"quit",4) == 0){         //退出  更改文件描述符到999
	    if(cc_log != 999){
		off_fun(account);                          //离线函数
		my_fd = 999;
	    }
	    break;
	}
	if(strncmp(man,"addfriend",9) == 0){         //添加好友

	    two_account_t two_inf1 ={};
	    strcpy(two_inf1.i_account,oneself_inf.my_account);
	    strcpy(two_inf1.he_account,data_ball.recv_account);
	    strcpy(two_inf1.data,data_ball.send_message);          //
	    strcpy(two_inf1.time,data_ball.time);
	    strcpy(two_inf1.my_name,oneself_inf.my_name);
	    two_inf1.i_fd = oneself_inf.my_fd;
	    add_friend(&two_inf1);         //添加好友函数

	}
	if(strncmp(man,"viewfriend",10) == 0){
	    view_friend( oneself_inf.my_account);       //查看好友
	    ret_inf(oneself_inf.my_fd,oneself_inf.my_account);       //返回个人信息的函数
	}
	if(strncmp(man,"chatfriend",10) == 0){                  //与好友发消息
	    two_account_t two_inf ={};
	    strcpy(two_inf.i_account,oneself_inf.my_account);
	    strcpy(two_inf.he_account,data_ball.recv_account);
	    strcpy(two_inf.data,data_ball.send_message);          //
	    strcpy(two_inf.time,data_ball.time);
	    two_inf.i_fd = oneself_inf.my_fd;
	    int cc = goto_data4(&two_inf);	     ///判断是否是自己的好友
	    if(cc == 2){
		goto_data(&two_inf);    }                //发送函数
	}
	if(strncmp(man,"viewfnew",8) == 0){                //查找聊天记录
	    two_account_t two_inf1 ={};
	    strcpy(two_inf1.i_account,oneself_inf.my_account);
	    strcpy(two_inf1.he_account,data_ball.send_message);
	    two_inf1.i_fd = oneself_inf.my_fd;
	    int cd = goto_data4(&two_inf1);	     ///判断是否是自己的好友
	    if(cd == 2){
		viewfnew(&two_inf1);             }                //查找聊天记录函数
	}
	if(strncmp(man,"cregchat",8) == 0)           //创建群聊
	{
	    strcpy(oneself_inf.myqun_name,data_ball.send_message);
	    cregchat(&oneself_inf);	           //返回群号
	}
	if(strncmp(man,"addgmem",7) == 0){          //拉好友进群
	    strcpy(oneself_inf.qun_num,data_ball.recv_account);
	    strcpy(oneself_inf.he_acc,data_ball.send_message);
	    addgmem(&oneself_inf);
	}
	if(strncmp(man,"gchat",5) == 0){                     // 开始群聊
	    oneself_t gchat ={};
	    gchat.my_fd = client_fd;
	    strcpy(gchat.my_account,oneself_inf.my_account);
	    strcpy(gchat.my_name,oneself_inf.my_name);
	    strcpy(gchat.qun_num,data_ball.recv_account);
	    strcpy(gchat.data,data_ball.send_message);
	    strcpy(gchat.time,data_ball.time);
	    gchat0(&gchat);           //群聊函数

	}
	if(strncmp(man,"viewgmem",8) == 0)                            //查看群成员
	{
	    oneself_t viewgmen={};
	    viewgmen.my_fd = client_fd;
	    strcpy(viewgmen.qun_num,data_ball.send_message);
	    strcpy(viewgmen.my_account,oneself_inf.my_account);
	    viewgmen0(&viewgmen);            //查看群成员的函数    
	}
	if(strncmp(man,"setbanned",9) == 0)       //设置群禁言
	{
	    oneself_t setbanned={};
	    setbanned.my_fd = client_fd;
	    strcpy(setbanned.qun_num,data_ball.send_message);
	    strcpy(setbanned.my_account,oneself_inf.my_account);
	    strcpy(setbanned.my_name,oneself_inf.my_name);
	    setbanned0(&setbanned);           //设置禁言的函数
	}
	if(strncmp(man,"unsetbanned",11) == 0)       //解除群禁言
	{
	    oneself_t unsetbanned={};
	    unsetbanned.my_fd = client_fd;
	    strcpy(unsetbanned.qun_num,data_ball.send_message);
	    strcpy(unsetbanned.my_account,oneself_inf.my_account);
	    strcpy(unsetbanned.my_name,oneself_inf.my_name);
	    unsetbanned0(&unsetbanned);           //解除禁言的函数
	}
	if(strncmp(man,"addgadmin",9) == 0){    //设置群管理员
	    oneself_t addgadmin={};
	    addgadmin.my_fd = client_fd;
	    strcpy(addgadmin.man,man);
	    strcpy(addgadmin.qun_num,data_ball.recv_account);
	    strcpy(addgadmin.my_account,oneself_inf.my_account);
	    strcpy(addgadmin.my_name,oneself_inf.my_name);
	    strcpy(addgadmin.he_acc,data_ball.send_message);
	    addgadmin0(&addgadmin);           //设置群管理员
	}
	if(strncmp(man,"delgadmin",9) == 0){    //取消群管理员
	    oneself_t delgadmin={};
	    delgadmin.my_fd = client_fd;
	    strcpy(delgadmin.man,man);
	    strcpy(delgadmin.qun_num,data_ball.recv_account);
	    strcpy(delgadmin.my_account,oneself_inf.my_account);
	    strcpy(delgadmin.my_name,oneself_inf.my_name);
	    strcpy(delgadmin.he_acc,data_ball.send_message);
	    addgadmin0(&delgadmin);           //取消群管理员
	}
	if(strncmp(man,"delfriend",9) == 0){          //删除好友，双向
	    two_account_t delfriend = {};
	    delfriend.i_fd = client_fd;
	    strcpy(delfriend.he_account,data_ball.send_message);
	    strcpy(delfriend.i_account,oneself_inf.my_account);
	    delfriend0(&delfriend);            //删好友的函数
	}
	if(strncmp(man,"delgmem",7) == 0){            //踢群里的人
	    oneself_t delgmem ={};
	    delgmem.my_fd = client_fd;
	    strcpy(delgmem.qun_num,data_ball.recv_account);	    
	    strcpy(delgmem.he_acc,data_ball.send_message);	    
	    strcpy(delgmem.my_account,oneself_inf.my_account);
	    strcpy(delgmem.my_name,oneself_inf.my_name);
	    delgmem0(&delgmem);              //踢群里的人的函数	    
	}

    }
    close(client_fd);
}





void set_up_user(int * fd,char * name,char * password)       //注册qq
{
    char account_l[7];
    int ret,tm;
    char sql[200];
    int ddd = 0;

    while(1)
    {
	bzero(account_l,7);
	bzero(sql,sizeof(sql));
	generate_account(account_l);      ////获得随机账号
	sprintf(sql,"select * from qq_data where account=\"%s\";",account_l);
	ret = sqlite3_exec(mydb,sql,kong_fun1,(void *)&ddd,NULL);     //验证随机账号与账号库是否重复
	if(ret != SQLITE_OK){
	    perror("set_up_user select error!\n");
	    exit(1);
	}
	if(ddd == 0)
	{             ///不重复
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"insert into qq_data values (999,\"%s\",\"%s\",\"%s\");",account_l,name,password);      ///添加文件描述符、账号、昵称、密码到账号库
	    tm = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	    if(tm != SQLITE_OK ){
		perror("insert into error!\n");
		exit(1);
	    }
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"create table if not exists friend_%s(zhuang_tai char[20],number int,friend_acc char[20],friend_name char[20]);",account_l); //创建个人好友列表
	    tm =sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	    if(tm != SQLITE_OK){
		perror("account_l create table error!\n");
		exit(1);
	    }
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"insert into friend_%s values (NULL,999,NULL,NULL);",account_l);      ///添加好友信息判断位到好友库
	    tm = sqlite3_exec(mydb,sql,NULL,NULL,NULL);/////////
	    if(tm != SQLITE_OK ){
		perror("friend into 999 error!\n");
		exit(1);
	    }
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"create table if not exists qun_%s(qun_account[7],qun_name char[20])",account_l); //创建个人群列表
	    tm =sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	    if(tm != SQLITE_OK){
		perror("set_up_user qun  create table error!\n");
		exit(1);
	    }
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"create table if not exists inf_%s(name char[20],account char[20],age char[20],sex char[20]);",account_l); //创建个人信息的列表--姓名，账号，年龄，性别，个性签名
	    tm =sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	    if(tm != SQLITE_OK){
		perror("account_l create table error!\n");
		exit(1);
	    }
	    //strcpy(my_name,name);
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"insert into inf_%s values (\"%s\",\"%s\",NULL,NULL);",account_l,name,account_l);      ///添加个人信息到个人库
	    tm = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	    if(tm != SQLITE_OK ){
		perror("insert into error!\n");
		exit(1);
	    }
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"create table if not exists qunnew_%s(z_t int,time char[20],from_t char[50],data char[100]);",account_l); //创建个人离线时的群消息缓存表
	    tm =sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	    if(tm != SQLITE_OK){
		perror("account_l create table error!\n");
		exit(1);
	    }
	    break;
	}else{
	    // md = 0;
	    continue;
	}
    }
    //把注册到的账号打包
    datap_t send_data = {};
    strcpy(send_data.recv_message,account_l);
    strcpy(send_data.send_account,"server");
    const char * dabao_data = data_package(&send_data);
    ret = send(*fd,dabao_data,strlen(dabao_data),0);
    if(ret == -1){
	perror("send client account_l error!\n");
	exit(1);
    }
}




int kong_fun1(void * arg,int col,char ** str,char **name)
{
    int * ddd =(int *)arg;
    *ddd = 1;
    return 0;
}



extern void generate_account(char * p)      //生成账号
{
    int i;
    srand((unsigned)time(NULL));
    for(i =0;i<6;i++)
	p[i] = ((rand()%9+1) + 48);
    p[6] = '\0';
    // return p;
}

const char * data_package(datap_t* datap1)   //打包
{
    //第1步 创建一个小容器{}
    struct json_object *str1 = json_object_new_object();

    //第2步 将数值转换为json对象
    struct json_object *value1 = json_object_new_string(datap1->cmd);
    struct json_object *value2 = json_object_new_string(datap1->account_name);
    struct json_object *value3 = json_object_new_string(datap1->account_password);
    struct json_object *value4 = json_object_new_string(datap1->recv_account);
    struct json_object *value5 = json_object_new_string(datap1->send_message);
    struct json_object *value6 = json_object_new_string(datap1->recv_message);
    struct json_object *value7 = json_object_new_string(datap1->account);
    struct json_object *value8 = json_object_new_string(datap1->send_account);
    struct json_object *value9 = json_object_new_string(datap1->time);


    //第3步 将json对象根据key值添加到容器中
    json_object_object_add(str1,"命令",value1);
    json_object_object_add(str1,"账户名",value2);
    json_object_object_add(str1,"账户密码",value3);
    json_object_object_add(str1,"接收方",value4);
    json_object_object_add(str1,"发送信息",value5);
    json_object_object_add(str1,"接收信息",value6);
    json_object_object_add(str1,"账号",value7);
    json_object_object_add(str1,"发送方",value8);
    json_object_object_add(str1,"时间",value9);

    //第4步 将小容器转换成字符流
    const char *temp = json_object_to_json_string(str1);

    return temp;
}


void data_unpack(const char * arg,datap_t* datap1)      //解包
{
    //第一步 将字符流转换成容器
    struct json_object *str1 = json_tokener_parse(arg);

    //第2步 根据小容器中的key值获取数值对象
    struct json_object *value1 = json_object_object_get(str1,"命令");
    struct json_object *value2 = json_object_object_get(str1,"账户名");
    struct json_object *value3 = json_object_object_get(str1,"账户密码");
    struct json_object *value4 = json_object_object_get(str1,"接收方");
    struct json_object *value5 = json_object_object_get(str1,"发送信息");
    struct json_object *value6 = json_object_object_get(str1,"接收信息");
    struct json_object *value7 = json_object_object_get(str1,"账号");
    struct json_object *value8 = json_object_object_get(str1,"发送方");
    struct json_object *value9 = json_object_object_get(str1,"时间");

    //第3步 将数值对象转换为对应的数值
    const char *buf1= json_object_get_string(value1);
    const char *buf2= json_object_get_string(value2);
    const char *buf3= json_object_get_string(value3);
    const char *buf4= json_object_get_string(value4);
    const char *buf5= json_object_get_string(value5);
    const char *buf6= json_object_get_string(value6);
    const char *buf7= json_object_get_string(value7);
    const char *buf8= json_object_get_string(value8);
    const char *buf9= json_object_get_string(value9);

    strcpy(datap1->cmd,buf1);
    strcpy(datap1->account_name,buf2);
    strcpy(datap1->account_password,buf3);
    strcpy(datap1->recv_account,buf4);
    strcpy(datap1->send_message,buf5);
    strcpy(datap1->recv_message,buf6);
    strcpy(datap1->account,buf7);
    strcpy(datap1->send_account,buf8);
    strcpy(datap1->time,buf9);
}


int login_fun(int * fd,char * account,char * password,char * my_name)  //登录
{
    char sql[100];
    int tem=9,ret;
    datap_t data;
    fd_password_t fd_password_p = {};
    fd_password_p.fd = *fd;
    strcpy(fd_password_p.password,password);
    fd_password_p.ddd = 0;

    //strcpy(fd_password_p.my_name,my_name);


    sprintf(sql,"select * from qq_data where account =\"%s\";",account);/////00000
    tem = sqlite3_exec(mydb,sql,login_find_acc,(void *)&fd_password_p,NULL);
    if(tem != SQLITE_OK){
	perror("login_fun select * from account error!\n");
	exit(1);
    }
    strcpy(my_name,fd_password_p.my_name);
    if( fd_password_p.ddd == 0){
	strcpy(data.recv_message,"账号不正确！");
	strcpy(data.send_account,"server");
	const char * dabao_data = data_package(&data);
	ret = send(*fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("login_fun send client 账号不正确 error!\n");
	    exit(1);
	}
	//	md = 0;
	return 1;
    }
    return 0;
}

int login_find_acc(void * arg,int col,char ** str,char **name)   //登录查找函数
{
    //md++;
    datap_t data ={};
    int ret;
    fd_password_t * fd_password_p =(fd_password_t *)arg;
    fd_password_p->ddd = 1;
    strcpy(fd_password_p->my_name,str[2]);
    int n =strlen(fd_password_p->password);
    if(strncmp(str[3],fd_password_p->password,n) == 0){       ////登录成功
	//定义结构体，获得fd,回数据
	strcpy(data.recv_message,"登录成功");
	strcpy(data.send_account,"server");
	const char * dabao_data = data_package(&data);
	ret = send(fd_password_p->fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("login_find_acc send client 登录成功 error!\n");
	    exit(1);
	}
    }else{       ////登录失败
	strcpy(data.recv_message,"密码错误!");
	strcpy(data.send_account,"server");
	const char * dabao_data = data_package(&data);
	ret = send(fd_password_p->fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("login_find_acc send client 密码错误 error!\n");

	    exit(1);
	} 
    }    
    return 0;
}

void off_fun(char * account)   ///离线函数
{
    char sql[100];
    int tmp;
    sprintf(sql,"update qq_data set fd=999 where account=\"%s\";",account);
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if(tmp != SQLITE_OK ){
	perror("off_fun error!\n");
	exit(1);
    }
}

void log_fd(int fd,char * account)     //登录改fd
{
    char sql[100];
    int tmp;
    sprintf(sql,"update qq_data set fd=%d where account=\"%s\";",fd,account);
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if(tmp != SQLITE_OK ){
	perror("log_fd error!\n");
	exit(1);
    }
}
void ret_inf(int fd,char * account)   //登录后返回信息函数
{
    //返回个人信息
    //返回好友信息
    //返回群聊信息
    char sql[200];
    int tem,ret;
    datap_t send_data1 = {};
    datap_t send_data2 = {};
    mp = 1;
    dd_t ddd;
    ddd.fd =fd;
    ddd.ddd = 0;
    //int ddd = fd;
    usleep(10000);
    sprintf(sql,"select * from inf_%s where account !=\"NULL\";",account);
    tem = sqlite3_exec(mydb,sql,find_i_inf,(void *)&fd,NULL);    //返回个人信息
    if(tem != SQLITE_OK ){
	perror("ret_inf find_i_inf error!\n");
	exit(1);
    }
    usleep(10000);
    mp = 1;
    // md = 0;
    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from friend_%s where number<999;",account);
    tem = sqlite3_exec(mydb,sql,friend_i_inf,(void *)&ddd,NULL);   //返回好友信息
    if(tem != SQLITE_OK ){
	perror("ret_inf find_i_inf error!\n");
	exit(1);
    }
    if(ddd.ddd != 999){
	//暂无好友信息
	strcpy(send_data1.recv_message,"暂未添加好友");
	strcpy(send_data1.send_account,"server");
	const char * dabao_data = data_package(&send_data1);
	ret = send(fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("find_i_inf send2 client error!\n");
	    exit(1);
	}
    }
    usleep(10000);
    mp = 1;
    // md = 0;
    //ddd = fd;
    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qun_%s where qun_account != \"NULL\";",account);
    tem = sqlite3_exec(mydb,sql,qun_i_inf,(void *)&ddd,NULL);   //返回群信息
    if(tem != SQLITE_OK ){
	perror("qun_inf find_i_inf error!\n");
	exit(1);
    }
    if(ddd.ddd != 998){
	//暂无好友信息
	strcpy(send_data2.recv_message,"暂未创建或加入群聊");
	strcpy(send_data2.send_account,"server");
	const char * dabao_data = data_package(&send_data2);
	ret = send(fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("find_i_inf send2 client error!\n");
	    exit(1);
	}
    }
}

int find_i_inf(void * arg,int col,char ** str,char ** name)   //返回个人信息函数
{
    //strcpy(my_name,str[0]);
    int * fd = (int *)arg;
    char buf[200];
    int ret;
    while(mp == 1){
	bzero(buf,sizeof(buf));
	sprintf(buf,"%-20s%-20s%-20s%-20s",name[0],name[1],name[2],name[3]);
	datap_t send_data = {};
	strcpy(send_data.recv_message,buf);
	strcpy(send_data.send_account,"server");
	const char * dabao_data = data_package(&send_data);
	ret = send(*fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("find_i_inf send1 client  error!\n");
	    exit(1);
	}
	mp = 0;
    }
    usleep(1000);
    bzero(buf,sizeof(buf));
    sprintf(buf,"%-20s%-20s%-20s%-20s",str[0],str[1],str[2],str[3]);
    datap_t send_data1 = {};
    strcpy(send_data1.recv_message,buf);
    strcpy(send_data1.send_account,"server");
    const char * dabao_data1 = data_package(&send_data1);
    ret = send(*fd,dabao_data1,strlen(dabao_data1),0);
    if(ret == -1){
	perror("find_i_inf send2 client error!\n");
	exit(1);
    }
    return 0;
}

int friend_i_inf(void * arg,int col,char ** str,char ** name)   //返回好友信息函数
{
    dd_t * ddd = (dd_t * )arg;
    ddd->ddd = 999;
    int fd = ddd->fd;
    char buf[150];
    int ret;
    while(mp == 1){
	sprintf(buf,"%-20s%-20s%-20s",name[0],name[2],name[3]);
	datap_t send_data = {};
	strcpy(send_data.recv_message,buf);
	strcpy(send_data.send_account,"server");
	const char * dabao_data = data_package(&send_data);
	ret = send(fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("friend_i_inf send1 client  error!\n");
	    exit(1);
	}
	mp = 0;
    }
    usleep(10000);
    bzero(buf,150);
    sprintf(buf,"%-20s%-20s%-20s",str[0],str[2],str[3]);
    datap_t send_data1 = {};
    strcpy(send_data1.recv_message,buf);
    strcpy(send_data1.send_account,"server");
    const char * dabao_data = data_package(&send_data1);
    ret = send(fd,dabao_data,strlen(dabao_data),0);
    if(ret == -1){
	perror("friend_i_inf send2 client error!\n");
	exit(1);
    }
    return 0;
}

int qun_i_inf(void * arg,int col,char ** str,char ** name)   //返回群信息函数
{
    dd_t * ddd = (dd_t * )arg;
    ddd->ddd = 998;
    int fd = ddd->fd;
    //int * fd = (int *)arg;
    int ret;
    char buf[150];
    while(mp == 1){
	sprintf(buf,"%-20s%-20s",name[0],name[1]);
	datap_t send_data = {};
	strcpy(send_data.recv_message,buf);
	strcpy(send_data.send_account,"server");
	const char * dabao_data = data_package(&send_data);
	ret = send(fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("qun_i_inf send1 client  error!\n");
	    exit(1);
	}
	mp = 0;
    }
    usleep(10000);
    bzero(buf,150);
    sprintf(buf,"%-20s%-20s",str[0],str[1]);
    datap_t send_data1 = {};
    strcpy(send_data1.recv_message,buf);
    strcpy(send_data1.send_account,"server");
    const char * dabao_data = data_package(&send_data1);
    ret = send(fd,dabao_data,strlen(dabao_data),0);
    if(ret == -1){
	perror("qun_i_inf send2 client error!\n");
	exit(1);
    }
    return 0;
}


void add_friend(two_account_t * two_inf)         //添加好友函数
{
    char sql[100];
    int tmp;
    if(strncmp(two_inf->i_account,two_inf->he_account,6) == 0){
	//添加自己
	strcpy(sql,"自己不能成为自己的好友奥");
	server_go(two_inf->i_fd,sql,"server");
    }else{
	int ddd = 0;
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from friend_%s where friend_acc=\"%s\";",two_inf->i_account,two_inf->he_account);
	tmp = sqlite3_exec(mydb,sql,kong_fun1,(void *)&ddd,NULL);
	if( tmp != SQLITE_OK){
	    perror("add_friend select1 error!\n");
	    exit(1);
	}
	if(ddd == 0){                     //该账号还不是自己的好友，可以添加
	    //md = 0;
	    bzero(sql,sizeof(sql));
	    two_inf->ddd = 0;
	    sprintf(sql,"select * from qq_data where account=\"%s\";",two_inf->he_account);
	    tmp = sqlite3_exec(mydb,sql,add_find_fri,(void*)two_inf,NULL);
	    if( tmp != SQLITE_OK){
		perror("add_friend selectllll2 error!\n");/////////
		exit(1);
	    }
	    if(two_inf->ddd == 0){                     //你添加的好友不存在
		bzero(sql,sizeof(sql));
		strcpy(sql,"你添加的好友不存在");
		datap_t data;
		strcpy(data.recv_message,sql);
		strcpy(data.send_account,"server");
		const char * dabao = data_package(&data);     //打包数据
		tmp = send(two_inf->i_fd,dabao,strlen(dabao),0);
		if(tmp == -1){
		    perror("add_friend 好友不存在发送失败\n");
		    exit(1);
		}
	    }
	}else{                       //该账号已经是你的好友
	    bzero(sql,sizeof(sql));
	    strcpy(sql,"该账号已经是你的好友了");
	    server_go(two_inf->i_fd,sql,"server");
	}
    }
}

int add_find_fri(void * arg,int col,char ** str,char ** name)   //添加好友查找
{
    two_account_t * account_t =(two_account_t *)arg;
    account_t->ddd = 1;
    int he_fd = atoi(str[0]);
    int i_fd = account_t->i_fd;
    char sql[200];
    int tm;
    bzero(sql,sizeof(sql));
    if(atoi(account_t->i_account) > atoi(account_t->he_account)){
	sprintf(sql,"create table f%s%s(z_t int,data char[100],from_t char[7],goto_t char[7],time char[20]);",account_t->i_account,account_t->he_account);
    }else{
	sprintf(sql,"create table f%s%s(z_t int,data char[100],from_t char[7],goto_t char[7],time char[20]);",account_t->he_account,account_t->i_account);
    }
    char * mm;
    tm = sqlite3_exec(mydb,sql,NULL,NULL,&mm);
    if(tm != SQLITE_OK ){
	perror("add_friend_fir end  error!\n");
	exit(1);
    }
    //把好友加到自己的好友列表
    sprintf(sql,"insert into friend_%s values (\"NULL\",1,\"%s\",\"%s\");",account_t->i_account,str[1],str[2]);      ///添加好友信息到好友库
    tm = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if(tm != SQLITE_OK ){
	perror("friend into 999 error!\n");
	exit(1);
    }else{
	bzero(sql,sizeof(sql));
	sprintf(sql,"acc:%s-name:%s->已经添加成功",str[1],str[2]);
	server_go(i_fd,sql,"server");
    }
    //把自己加到对方的好友列表
    bzero(sql,sizeof(sql));
    sprintf(sql,"insert into friend_%s values (\"NULL\",1,\"%s\",\"%s\");",str[1],account_t->i_account,account_t->my_name);      ///添加好友信息到好友库
    tm = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if(tm != SQLITE_OK ){
	perror("friend into 999 error!\n");
	exit(1);
    }else if(he_fd == 999 ){
	char dd[50];
	if(atoi(account_t->i_account) > atoi(str[1])){
	    sprintf(dd,"f%s%s",account_t->i_account,str[1]);
	}else{
	    sprintf(dd,"f%s%s",str[1],account_t->i_account);
	}
	char data[50];
	sprintf(data,"acc:%s-name:%s->将你添加为好友",account_t->i_account,account_t->my_name);
	bzero(sql,sizeof(sql));
	sprintf(sql,"insert into %s values(1,\"%s\",\"server\",\"%s\",\"\");",dd,data,str[1]);
	tm = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	if( tm != SQLITE_OK ){
	    perror("goto_data insert6 error!\n");
	    exit(1);
	}                  //存记录，标志为1


    }else{
	bzero(sql,sizeof(sql));
	sprintf(sql,"acc:%s-name:%s->将你添加为好友",account_t->i_account,account_t->my_name);
	server_go(he_fd,sql,"server");////////////////
	bzero(sql,sizeof(sql));
	strcpy(sql,"hello! Nice to meet you! ");
	server_go(he_fd,sql,account_t->i_account);
    }


    return 0;
}

void server_go(int fd,char * data,char * from)          //服务器发送信息的函数
{
    int ret;
    datap_t data1 = {};
    strcpy(data1.recv_message,data);     //赋值内容
    strcpy(data1.send_account,from);      //赋值来自谁
    time_t ti_me;
    time(&ti_me);
    struct tm * p =localtime(&ti_me);
    char tim[30];
    sprintf(tim,"time:<%02d:%02d>",p->tm_hour,p->tm_min);
    strcpy(data1.time,tim);       //赋值时间
    const char * dabao_data = data_package(&data1);
    ret = send(fd,dabao_data,strlen(dabao_data),0);
    if(ret == -1){
	perror("server_go send error!\n");
	exit(1);
    }
}

extern void view_friend(char * account)     //查看好友
{
    char sql[200];
    int tmp;
    sprintf(sql,"select * from friend_%s where friend_acc != \"NULL\";",account);
    tmp = sqlite3_exec(mydb,sql,zhuang_t,(void *)account,NULL);
    if(tmp != SQLITE_OK){
	perror("view_friend select error!\n");
	exit(1);
    }

}

int zhuang_t(void * arg,int col,char ** str,char **name)   //好友状态修正
{
    two_account_t two_account;
    strcpy(two_account.i_account,(char * )arg);
    strcpy(two_account.he_account,str[2]);
    char sql[200];
    int tmp;
    sprintf(sql,"select * from qq_data where account = \"%s\";",two_account.he_account);
    tmp = sqlite3_exec(mydb,sql,zhuang_t1,(void *)&two_account,NULL);
    if(tmp != SQLITE_OK){
	perror("zhuang_t select error!\n");
	exit(1);
    }
    return 0;
}
int zhuang_t1(void * arg,int col,char ** str,char **name)   //好友状态修正1
{
    two_account_t * two_account =(two_account_t *)arg;
    int fd = atoi(str[0]);
    char sql[200];
    int tmp;
    if( fd != 999){
	sprintf(sql,"update friend_%s set zhuang_tai=\"在线\" where friend_acc = \"%s\";",two_account->i_account,two_account->he_account);
    }else{
	sprintf(sql,"update friend_%s set zhuang_tai=\"离线\" where friend_acc = \"%s\";",two_account->i_account,two_account->he_account);
    }
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if(tmp != SQLITE_OK){
	perror("zhuang_t1 update error!\n");
	exit(1);
    }
    return 0;
}

int goto_data4(two_account_t * two_inf)     //判断是否是自己的好友
{
    char sql[200];
    int tmp;
    int ddd = 0;
    sprintf(sql,"select * from friend_%s where friend_acc =\"%s\" ",two_inf->i_account,two_inf->he_account);
    //  mg = 0;
    tmp = sqlite3_exec(mydb,sql,kong_fun1,(void * )&ddd,NULL);
    if(tmp != SQLITE_OK){
	perror("goto_data4 error!\n");
	exit(1);
    }
    if(ddd == 0){
	//没有找到该好友
	bzero(sql,sizeof(sql));
	strcpy(sql,"该账号不是你好友");
	server_go(two_inf->i_fd,sql,"server");
	return 3;
    }else{
	return 2;        //找到该好友
    }
}

void goto_data(two_account_t * two_inf)      //与好友私聊的函数
{
    char sql[500];
    int tmp;
    int tmq;
    char dd[50];
    int he_fd = 999;
    if(atoi(two_inf->i_account) > atoi(two_inf->he_account)){
	sprintf(dd,"f%s%s",two_inf->i_account,two_inf->he_account);
    }else{
	sprintf(dd,"f%s%s",two_inf->he_account,two_inf->i_account);
    }
    //md = 0;
    sprintf(sql,"select * from qq_data where fd != 999 and account = \"%s\";",two_inf->he_account);
    tmq = sqlite3_exec(mydb,sql,cat_fd,(void *)&he_fd,NULL);
    if( tmq != SQLITE_OK ){
	perror("goto_data insert2 error!\n");
	exit(1);
    }
    if(he_fd != 999){            //该好友在线
	datap_t data_go = {};
	strcpy(data_go.recv_message,two_inf->data);
	strcpy(data_go.time,two_inf->time);
	strcpy(data_go.send_account,two_inf->i_account);
	const char * dabao = data_package(&data_go);
	tmq = send(he_fd,dabao,strlen(dabao),0);
	if( tmp == -1){
	    perror("goto_data3 send error!\n");
	    exit(1);
	}      //在线时信息发送成功
	bzero(sql,sizeof(sql));
	sprintf(sql,"insert into %s values(0,\"%s\",\"%s\",\"%s\",\"%s\");",dd,two_inf->data,two_inf->i_account,two_inf->he_account,two_inf->time);
	tmq = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	if( tmq != SQLITE_OK ){
	    perror("goto_data insert3 error!\n");
	    exit(1);
	}                  //存记录，标志为0


    }else{                  //该好友不在线
	bzero(sql,sizeof(sql));
	sprintf(sql,"insert into %s values(1,\"%s\",\"%s\",\"%s\",\"%s\");",dd,two_inf->data,two_inf->i_account,two_inf->he_account,two_inf->time);
	tmq = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
	if( tmq != SQLITE_OK ){
	    perror("goto_data insert4 error!\n");
	    exit(1);
	}                  //存记录，标志为1
    }
}

int cat_fd(void * arg,int col,char ** str,char **name)   //获取在线好友fd
{
    //md++;
    int * he_fd = (int *)arg;
    *he_fd = atoi(str[0]);
    return 0;
}
void lixian_news(oneself_t * oneself_inf)          //离线期间的消息获取
{
    char sql[200];
    int tmp;
    //获取我的好友信息
    sprintf(sql,"select * from friend_%s where friend_acc != \"NULL\";",oneself_inf->my_account);
    tmp = sqlite3_exec(mydb,sql,lixian_news1,oneself_inf,NULL);
    if(tmp != SQLITE_OK){
	perror("lixian_news select 1 error!\n");
	exit(1);
    }

}

int lixian_news1(void * arg,int col,char ** str,char **name)   //离线期间的消息获取1
{
    oneself_t * oneself_inf = (oneself_t *)arg;
    int tmp;
    char sql[200];
    char dd[20];      //与该好友的消息记录表名
    if(atoi(oneself_inf->my_account) > atoi(str[2])){
	sprintf(dd,"f%s%s",oneself_inf->my_account,str[2]);
    }else{
	sprintf(dd,"f%s%s",str[2],oneself_inf->my_account);
    }

    sprintf(sql,"select * from %s where z_t = 1 and goto_t = \"%s\";",dd,oneself_inf->my_account);
    tmp = sqlite3_exec(mydb,sql,lixian_news2,oneself_inf,NULL);    //回调发送
    if(tmp != SQLITE_OK){
	perror("lixian_news66 select 1 error!\n");
	exit(1);
    }
    bzero(sql,sizeof(sql));
    sprintf(sql,"update %s set z_t = 0 where z_t = 1 and goto_t = \"%s\";",dd,oneself_inf->my_account);
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if( tmp != SQLITE_OK ){
	perror("lixian_news1 update error!\n");
	exit(1);
    }
    return 0;
}

int lixian_news2(void * arg,int col,char ** str,char **name)   //离线期间的消息获取2
{
    oneself_t * oneself_inf = (oneself_t *)arg;
    int tmp;
    char sql[200];
    datap_t data_go = {};
    strcpy(data_go.recv_message,str[1]);
    strcpy(data_go.time,str[4]);
    strcpy(data_go.send_account,str[2]);
    const char * dabao = data_package(&data_go);
    usleep(10000);
    tmp = send(oneself_inf->my_fd,dabao,strlen(dabao),0);
    if( tmp == -1){
	perror("lixian_news2 send error!\n");
	exit(1);
    }
    return 0;
}
void lixian_qunnews(oneself_t * gchat)    //离线期间的群消息获取
{
    char sql[200];
    int tmp;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qunnew_%s where z_t = 1;",gchat->my_account);
    tmp = sqlite3_exec(mydb,sql,lixian_qunnews1,(void *)gchat,NULL);
    if( tmp != SQLITE_OK ){
	perror("lixian_news1 update error!\n");
	exit(1);
    }
    //更改缓存消息状态为0
    bzero(sql,sizeof(sql));
    sprintf(sql,"update qunnew_%s set z_t = 0 where z_t = 1;",gchat->my_account);
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if( tmp != SQLITE_OK ){
	perror("lixian_news1 update error!\n");
	exit(1);
    }
}

int lixian_qunnews1(void * arg,int col,char ** str,char **name)   //离线期间的群消息获取1
{
    int tmp;
    oneself_t * gchat = (oneself_t *)arg;
    datap_t data_go = {};
    strcpy(data_go.recv_message,str[3]);
    strcpy(data_go.time,str[1]);
    strcpy(data_go.send_account,str[2]);
    const char * dabao = data_package(&data_go);
    usleep(10000);
    tmp = send(gchat->my_fd,dabao,strlen(dabao),0);
    if( tmp == -1){
	perror("lixian_news2 send error!\n");
	exit(1);
    }
    return 0;
}

void viewfnew(two_account_t * two_inf)       //查看聊天记录
{
    char sql[200];
    int tmp;
    char dd[20];
    if((atoi(two_inf->i_account)) > (atoi(two_inf->he_account))){
	sprintf(dd,"f%s%s",two_inf->i_account,two_inf->he_account);
    }else{
	sprintf(dd,"f%s%s",two_inf->he_account,two_inf->i_account);
    }

    mp = 0;
    sprintf(sql,"select * from %s where z_t != 1;",dd);
    tmp = sqlite3_exec(mydb,sql,viewfnew_sql,(void *)two_inf,NULL);
    if(tmp != SQLITE_OK){
	perror("viewfnew_sql error!\n");
	exit(1);
    }
}

int viewfnew_sql(void * arg,int col,char ** str,char **name)   //查看聊天记录1
{
    two_account_t * two_inf = (two_account_t *)arg;
    int tmp;
    datap_t data_go = {};
    char sql[200];
    while( mp == 0){                 //打表头
	bzero(sql,sizeof(sql));
	sprintf(sql,"%-10s%-10s%-10s%-50s",name[4],name[2],name[3],name[1]);
	strcpy(data_go.recv_message,sql);
	strcpy(data_go.send_account,"server");
	const char * dabao = data_package(&data_go);
	tmp = send(two_inf->i_fd,dabao,strlen(dabao),0);
	if( tmp == -1){
	    perror("goto_data3 send error!\n");
	    exit(1);
	}
	mp++;
    }
    usleep(10000);
    bzero(sql,sizeof(sql));
    sprintf(sql,"%-10s%-10s%-10s%-50s",str[4],str[2],str[3],str[1]);
    strcpy(data_go.recv_message,sql);
    strcpy(data_go.send_account,"server");
    const char * dabao = data_package(&data_go);
    tmp = send(two_inf->i_fd,dabao,strlen(dabao),0);
    if( tmp == -1){
	perror("goto_data3 send error!\n");
	exit(1);
    }
    return 0;
}

void cregchat(oneself_t * oneself_inf)     //创建群聊返回群号
{
    char qun_acc[7];
    int tmp;
    generate_account(qun_acc);
    char sql[200];         
    sprintf(sql,"insert into qun_%s values(\"%s\",\"%s\");",oneself_inf->my_account,qun_acc,oneself_inf->myqun_name);
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //将群信息添加至我的群列表中
    if(tmp != SQLITE_OK){
	perror("lixian_news1 select 1 error!\n");
	exit(1);
    }

    bzero(sql,sizeof(sql));
    sprintf(sql,"create table qunliao%s(cy_name char[20],cy_acc char[20],cy_power int);",qun_acc);
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //创建该群的数据库表
    if(tmp != SQLITE_OK){
	perror("lixian_news1 select 1 error!\n");
	exit(1);
    }

    bzero(sql,sizeof(sql));
    sprintf(sql,"insert into qunliao%s values(\"%s\",\"%s\",1);",qun_acc,oneself_inf->my_name,oneself_inf->my_account);
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //将自己加进该表
    if(tmp != SQLITE_OK){
	perror("lixian_news1 select 888 error!\n");
	exit(1);
    }

    char data[50];
    sprintf(data,"您的群聊已成功创建，群聊号是%s",qun_acc);
    server_go(oneself_inf->my_fd,data,"server");
}


void addgmem(oneself_t * oneself_inf)      //拉群成员的函数
{

    //判断是不是你好友
    char sql[200];
    int tmp;
    char he_name[20]={"NULL"};       //获得被邀请人的名字
    char qun_name[20] ={"NULL"};       //获得群名字
    char data[100];
    int he_fd;          //获取他的fd
    sprintf(sql,"select * from friend_%s where friend_acc = \"%s\";",oneself_inf->my_account,oneself_inf->he_acc);
    tmp = sqlite3_exec(mydb,sql,addgmem1,he_name,NULL);
    if(tmp != SQLITE_OK){
	perror("addgmem select 1 error!\n");
	exit(1);
    }
    if(strncmp(he_name,"NULL",4) == 0)
    {               //他不是你的好友，不能添加进群
	bzero(data,sizeof(data));
	sprintf(data,"%s还不是你的好友，不能邀请ta进群奥",oneself_inf->he_acc);
	server_go(oneself_inf->my_fd,data,"server");
    }else
    {                               //他是你的好友
	//判断自己有没有这个群  并获得群名字
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from qun_%s where qun_account = \"%s\";",oneself_inf->my_account,oneself_inf->qun_num);
	tmp = sqlite3_exec(mydb,sql,addgmem2,qun_name,NULL);
	if(tmp != SQLITE_OK){
	    perror("addgmem select 2 error!\n");
	    exit(1);
	}
	if(strncmp(qun_name,"NULL",4) == 0)              //自己没有这个群，不能拉人进这个群
	{
	    bzero(data,sizeof(data));
	    sprintf(data,"您还没有这个群:%s",oneself_inf->qun_num);
	    server_go(oneself_inf->my_fd,data,"server");
	}else
	{
	    //将他加进群表里边，权限是3，返回信息给自己添加成功
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"insert into qunliao%s values(\"%s\",\"%s\",3);",oneself_inf->qun_num,he_name,oneself_inf->he_acc);
	    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    
	    if(tmp != SQLITE_OK){
		perror("addgmem  select 3 error!\n");
		exit(1);
	    }
	    bzero(data,sizeof(data));
	    sprintf(data,"name:%s,account:%s,已经添加至群:%s中",he_name,oneself_inf->he_acc,qun_name);
	    server_go(oneself_inf->my_fd,data,"server");
	    //将这个群添加到他的群列表里，通知他被添加群了
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"insert into qun_%s values(\"%s\",\"%s\");",oneself_inf->he_acc,oneself_inf->qun_num,qun_name);
	    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //将群信息添加至ta的群列表中
	    if(tmp != SQLITE_OK){
		perror(" addgmem insert 1 error!\n");
		exit(1);
	    }
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"select * from qq_data where account = \"%s\";",oneself_inf->he_acc);
	    tmp = sqlite3_exec(mydb,sql,addgmem3,&he_fd,NULL);
	    if( tmp != SQLITE_OK){
		perror(" addgmem select 4 error!\n");
		exit(1);
	    }
	    bzero(data,sizeof(data));
	    sprintf(data,"name:%s,account:%s,邀请你加入群:%s中，快去聊天叭",oneself_inf->my_name,oneself_inf->my_account,qun_name);
	    if( he_fd != 999){            //他在线，直接发送
		server_go(he_fd,data,"server");
	    }else
	    {              //他不在线
		char dd[50];
		if(atoi(oneself_inf->my_account) > atoi(oneself_inf->he_acc)){
		    sprintf(dd,"f%s%s",oneself_inf->my_account,oneself_inf->he_acc);
		}else{
		    sprintf(dd,"f%s%s",oneself_inf->he_acc,oneself_inf->my_account);
		}
		bzero(sql,sizeof(sql));
		sprintf(sql,"insert into %s values(1,\"%s\",\"server\",\"%s\",\"\");",dd,data,oneself_inf->he_acc);
		tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
		if( tmp != SQLITE_OK ){
		    perror("goto_data insert5 error!\n");
		    exit(1);
		}                  //存记录，标志为1
	    }
	}
    }
}
int addgmem1(void * arg,int col,char ** str,char **name)   //拉群成员的函数1
{
    char * he_name = (char *)arg;
    strcpy(he_name,str[3]);
    return 0;
}

int addgmem2(void * arg,int col,char ** str,char **name)   //拉群成员的函数2
{
    char * qun_name = (char *)arg;
    strcpy(qun_name,str[1]);
    return 0;
}

int addgmem3(void * arg,int col,char ** str,char **name)   //拉群成员的函数2
{
    int * he_fd = (int *)arg;
    * he_fd = atoi(str[0]);
    return 0;
}
int addgmem4(void * arg,int col,char ** str,char **name)   //拉群成员的函数4
{
    int * he_fd = (int *)arg;
    * he_fd = atoi(str[2]);
    return 0;
}

void gchat0(oneself_t * gchat)    //群聊函数
{
    char sql[200];
    char data[50];
    int tmp;
    int ddd = 0;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qun_%s where qun_account = \"%s\";",gchat->my_account,gchat->qun_num);
    tmp = sqlite3_exec(mydb,sql,kong_fun1,(void *)&ddd,NULL);    //查看有无这个群
    if(tmp != SQLITE_OK){
	perror(" addgmem insert 1 error!\n");
	exit(1);
    }
    if( ddd == 0){     //您没有这个群，不能群聊
	bzero(data,sizeof(data));
	sprintf(data,"您没有这个群聊:%s",gchat->qun_num);
	server_go(gchat->my_fd,data,"server");	
    }else{        //权限->禁言   //您有这个群聊
	bzero(sql,sizeof(sql));
	int qx;
	sprintf(sql,"select * from qunliao%s where cy_acc = \"%s\" ;",gchat->qun_num,gchat->my_account);
	tmp = sqlite3_exec(mydb,sql,addgmem4,(void *)&qx,NULL);    //判断是否禁言
	if(tmp != SQLITE_OK){
	    perror(" addgmem insert 3 error!\n");
	    exit(1);
	}
	if(qx > 3){
	    bzero(data,sizeof(data));
	    sprintf(data,"群聊:%s禁言中！您不是管理层",gchat->qun_num);
	    server_go(gchat->my_fd,data,"server");	

	}else{

	    bzero(sql,sizeof(sql));
	    sprintf(sql,"select * from qunliao%s where cy_acc != \"NULL\" and cy_acc != \"%s\";",gchat->qun_num,gchat->my_account);
	    tmp = sqlite3_exec(mydb,sql,gchat1,(void *)gchat,NULL);    //查找群成员的函数
	    if(tmp != SQLITE_OK){
		perror(" addgmem insert 2 error!\n");
		exit(1);
	    }
	}
    }
}

int gchat1(void * arg,int col,char ** str,char **name)   //查找群成员的函数
{
    char sql[500];
    int fd;
    int tmp;
    oneself_t * gchat =(oneself_t *)arg;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qq_data where account = \"%s\";",str[1]);
    tmp = sqlite3_exec(mydb,sql,addgmem3,(void *)&fd,NULL);    //找到该群成员，返回他的fd
    if(tmp != SQLITE_OK){
	perror(" gchat1 select 1 error!\n");
	exit(1);
    }
    char from[50];
    sprintf(from,"new from qun:%s->he_account:%s",gchat->qun_num,gchat->my_account);
    if(fd != 999){      //该群成员在线
	server_go(fd,gchat->data,from);	
    }else{              //该群成员不在线
	bzero(sql,sizeof(sql));
	sprintf(sql,"insert into qunnew_%s values(1,\"%s\",\"%s\",\"%s\");",str[1],gchat->time,from,gchat->data);
	tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //将该条消息缓存
	if(tmp != SQLITE_OK){
	    perror(" gchat1 insert 1 error!\n");
	    exit(1);
	}
    }
    return 0;
}

void viewgmen0(oneself_t * viewgmen)         //查看群成员的函数
{
    char sql[200];
    int tmp;
    mp = 1;
    char data[50];
    int ddd = 0;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qun_%s where qun_account = \"%s\";",viewgmen->my_account,viewgmen->qun_num);
    tmp = sqlite3_exec(mydb,sql,kong_fun1,(void *)&ddd,NULL);    //查看有无这个群
    if(tmp != SQLITE_OK){
	perror(" viewgmen0 select 0 error!\n");
	exit(1);
    }
    if( ddd == 0){     //您没有这个群，不能群聊
	bzero(data,sizeof(data));
	sprintf(data,"您没有这个群聊:%s",viewgmen->qun_num);
	server_go(viewgmen->my_fd,data,"server");
    }else{
	bzero(sql,sizeof(sql));	
	sprintf(sql,"select * from qunliao%s where cy_name != \"NULL\";",viewgmen->qun_num);
	tmp = sqlite3_exec(mydb,sql,viewgmen1,(void * )viewgmen,NULL);    //找到群成员
	if(tmp != SQLITE_OK){
	    perror(" viewgmen0 select  error!\n");
	    exit(1);
	}
    }
}

int viewgmen1(void * arg,int col,char ** str,char ** name)   //查看群成员的函数1
{
    oneself_t * viewgmen = (oneself_t *)arg;
    char buf[150];
    int ret;

    while(mp == 1){
	sprintf(buf,"%-20s%-20s%-20s",name[0],name[1],name[2]);
	datap_t send_data = {};
	strcpy(send_data.recv_message,buf);
	strcpy(send_data.send_account,"server");
	const char * dabao_data = data_package(&send_data);
	ret = send(viewgmen->my_fd,dabao_data,strlen(dabao_data),0);
	if(ret == -1){
	    perror("viewgmen1 send 1 error!\n");
	    exit(1);
	}
	mp = 0;
    }
    usleep(5000);
    bzero(buf,150);
    char bq[20];
    if( atoi(str[2]) == 1){
	strcpy(bq,"群主");
    }else if(atoi(str[2]) == 2){
	strcpy(bq,"管理员");
    }else{
	strcpy(bq,"成员");
    }
    sprintf(buf,"%-20s%-20s%-20s",str[0],str[1],bq);
    datap_t send_data1 = {};
    strcpy(send_data1.recv_message,buf);
    strcpy(send_data1.send_account,"server");
    const char * dabao_data = data_package(&send_data1);
    ret = send(viewgmen->my_fd,dabao_data,strlen(dabao_data),0);
    if(ret == -1){
	perror("viewgmen1 send 2 error!\n");
	exit(1);
    }
    return 0;
}

void setbanned0(oneself_t * setbanned)      //设置禁言0
{
    char sql[200];
    int tmp;
    char data[100];
    int ddd = 0;
    int qx=0;
    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qun_%s where qun_account = \"%s\";",setbanned->my_account,setbanned->qun_num);
    tmp = sqlite3_exec(mydb,sql,kong_fun1,(void *)&ddd,NULL);    //查看有无这个群
    if(tmp != SQLITE_OK){
	perror(" setbanned0 select 0 error!\n");
	exit(1);
    }
    if( ddd == 0){     //您没有这个群，不能设置禁言
	bzero(data,sizeof(data));
	sprintf(data,"您没有这个群聊:%s",setbanned->qun_num);
	server_go(setbanned->my_fd,data,"server");
    }else{
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from qunliao%s where cy_acc = \"%s\" ;",setbanned->qun_num,setbanned->my_account);
	tmp = sqlite3_exec(mydb,sql,addgmem4,(void *)&qx,NULL);    //判断是否是管理层
	if(tmp != SQLITE_OK){
	    perror(" setbanned0 select 1 error!\n");
	    exit(1);
	}
	if(qx > 2){
	    bzero(data,sizeof(data));
	    sprintf(data,"该群聊:%s中，您不是管理层，不能设置禁言",setbanned->qun_num);
	    server_go(setbanned->my_fd,data,"server");
	}else{              //是管理层，可以设置禁言	
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"update qunliao%s set cy_power=4 where cy_power = 3;",setbanned->qun_num);
	    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //设置禁言
	    if(tmp != SQLITE_OK){
		perror(" setbanned0 update 1 error!\n");
		exit(1);
	    }
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"select * from qunliao%s where cy_acc != \"NULL\" and cy_acc != \"%s\";",setbanned->qun_num,setbanned->my_account);
	    tmp = sqlite3_exec(mydb,sql,setbanned1,(void *)setbanned,NULL);    //通知成员已经禁言
	    if(tmp != SQLITE_OK){
		perror("setbanned0 select 3 error!\n");
		exit(1);
	    }
	}
    }
}

extern int setbanned1(void * arg,int col,char ** str,char ** name)   //设置禁言1
{
    char sql[500];
    int fd;
    int tmp;
    oneself_t * gchat =(oneself_t *)arg;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qq_data where account = \"%s\";",str[1]);
    tmp = sqlite3_exec(mydb,sql,addgmem3,(void *)&fd,NULL);    //找到该群成员，返回他的fd
    if(tmp != SQLITE_OK){
	perror(" gchat1 select 1 error!\n");
	exit(1);
    }
    char from[50];
    sprintf(from,"acc:%s将群:%s设置了禁言",gchat->my_account,gchat->qun_num);
    if(fd != 999){      //该群成员在线
	server_go(fd,from,"server");	
    }else{              //该群成员不在线
	bzero(sql,sizeof(sql));
	sprintf(sql,"insert into qunnew_%s values(1,\"\",\"server\",\"%s\");",str[1],from);
	tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //将该条消息缓存
	if(tmp != SQLITE_OK){
	    perror(" gchat1 insert 1 error!\n");
	    exit(1);
	}
    }
    return 0;
}

void unsetbanned0(oneself_t * unsetbanned)      //解除禁言0
{
    char sql[200];
    int tmp;
    char data[100];
    int ddd = 0;
    int qx=0;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qun_%s where qun_account = \"%s\";",unsetbanned->my_account,unsetbanned->qun_num);
    tmp = sqlite3_exec(mydb,sql,kong_fun1,(void *)&ddd,NULL);    //查看有无这个群
    if(tmp != SQLITE_OK){
	perror(" unsetbanned0 select 0 error!\n");
	exit(1);
    }
    if( ddd == 0){     //您没有这个群，不能设置禁言
	bzero(data,sizeof(data));
	sprintf(data,"您没有这个群聊:%s",unsetbanned->qun_num);
	server_go(unsetbanned->my_fd,data,"server");
    }else{
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from qunliao%s where cy_acc = \"%s\" ;",unsetbanned->qun_num,unsetbanned->my_account);
	tmp = sqlite3_exec(mydb,sql,addgmem4,(void *)&qx,NULL);    //判断是否是管理层
	if(tmp != SQLITE_OK){
	    perror(" unsetbanned0 select 1 error!\n");
	    exit(1);
	}
	if(qx > 2){
	    bzero(data,sizeof(data));
	    sprintf(data,"该群聊:%s中，您不是管理层，不能解除禁言",unsetbanned->qun_num);
	    server_go(unsetbanned->my_fd,data,"server");
	    return;
	}else{              //是管理层，可以设置禁言	
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"update qunliao%s set cy_power=3 where cy_power = 4;",unsetbanned->qun_num);
	    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //解除禁言
	    if(tmp != SQLITE_OK){
		perror(" unsetbanned0 update 1 error!\n");
		exit(1);
	    }
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"select * from qunliao%s where cy_acc != \"NULL\" and cy_acc != \"%s\";",unsetbanned->qun_num,unsetbanned->my_account);
	    tmp = sqlite3_exec(mydb,sql,unsetbanned1,(void *)unsetbanned,NULL);    //通知成员已经禁言
	    if(tmp != SQLITE_OK){
		perror("unsetbanned0 select 3 error!\n");
		exit(1);
	    }
	}
    }
}

extern int unsetbanned1(void * arg,int col,char ** str,char ** name)   //设置禁言1
{
    char sql[500];
    int fd;
    int tmp;
    oneself_t * gchat =(oneself_t *)arg;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qq_data where account = \"%s\";",str[1]);
    tmp = sqlite3_exec(mydb,sql,addgmem3,(void *)&fd,NULL);    //找到该群成员，返回他的fd
    if(tmp != SQLITE_OK){
	perror(" gchat1 select 1 error!\n");
	exit(1);
    }
    char from[50];
    sprintf(from,"acc:%s将群:%s解除了禁言",gchat->my_account,gchat->qun_num);
    if(fd != 999){      //该群成员在线
	server_go(fd,from,"server");	
    }else{              //该群成员不在线
	bzero(sql,sizeof(sql));
	sprintf(sql,"insert into qunnew_%s values(1,\"\",\"server\",\"%s\");",str[1],from);
	tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //将该条消息缓存
	if(tmp != SQLITE_OK){
	    perror(" gchat1 insert 1 error!\n");
	    exit(1);
	}
    }
    return 0;
}
void addgadmin0(oneself_t *addgadmin)       //设置群管理员
{
    char sql[200],data[100];
    int tmp,ddd = 0,qx = 0;
    if(strcmp(addgadmin->my_account,addgadmin->he_acc) == 0){
	bzero(data,sizeof(data));
	strcpy(data,"不能操作自己");
	server_go(addgadmin->my_fd,data,"server");
    }else{
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from qun_%s where qun_account = \"%s\";",addgadmin->my_account,addgadmin->qun_num);
	tmp = sqlite3_exec(mydb,sql,kong_fun1,(void *)&ddd,NULL);    //查看有无这个群
	if(tmp != SQLITE_OK){
	    perror(" unsetbanned0 select 0 error!\n");
	    exit(1);
	}
	if( ddd == 0){     //您没有这个群，不能设置管理员
	    bzero(data,sizeof(data));
	    strcpy(data,"该群不存在");
	    server_go(addgadmin->my_fd,data,"server");
	}else{
	    //判断是不是群主
	    bzero(sql,sizeof(sql));
	    sprintf(sql,"select * from qunliao%s where cy_acc = \"%s\" ;",addgadmin->qun_num,addgadmin->my_account);
	    tmp = sqlite3_exec(mydb,sql,addgmem4,(void *)&qx,NULL);    //判断是否是群主
	    if(tmp != SQLITE_OK){
		perror(" unsetbanned0 select 1 error!\n");
		exit(1);
	    }
	    if(qx != 1){     //不是群主
		bzero(data,sizeof(data));
		sprintf(data,"该群聊:%s中，您不是群主，不能设置管理员",addgadmin->qun_num);
		server_go(addgadmin->my_fd,data,"server");
	    }else{              //是群主
		bzero(sql,sizeof(sql));
		if(strcmp(addgadmin->man,"addgadmin") == 0){
		    sprintf(sql,"update qunliao%s set cy_power = 2 where cy_acc = \"%s\";",addgadmin->qun_num,addgadmin->he_acc);
		}else{
		    sprintf(sql,"update qunliao%s set cy_power = 3 where cy_acc = \"%s\";",addgadmin->qun_num,addgadmin->he_acc);
		}
		tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //设置管理员
		if(tmp != SQLITE_OK){
		    perror(" unsetbanned0 select 1 error!\n");
		    exit(1);
		}
	    }
	}
    }
}


void delfriend0(two_account_t * delfriend)     //删除好友的函数0
{
    char sql[200],data[100],he_name[30] = {"NULL"};
    int tmp;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from friend_%s where friend_acc = \"%s\";",delfriend->i_account,delfriend->he_account);
    tmp = sqlite3_exec(mydb,sql,addgmem1,he_name,NULL);
    if(tmp != SQLITE_OK){
	perror("delfriend0 select 1 error!\n");
	exit(1);
    }
    if(strncmp(he_name,"NULL",4) == 0)
    {               //他不是你的好友
	bzero(data,sizeof(data));
	sprintf(data,"%s不是你的好友",delfriend->he_account);
	server_go(delfriend->i_fd,data,"server");
    }else{          //是你的好友
	bzero(sql,sizeof(sql));
	sprintf(sql,"delete from friend_%s where friend_acc=\"%s\";",delfriend->i_account,delfriend->he_account);
	tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //删我方
	if(tmp != SQLITE_OK){
	    perror(" delete i error!\n");
	    exit(1);
	}

	bzero(sql,sizeof(sql));
	sprintf(sql,"delete from friend_%s where friend_acc=\"%s\";",delfriend->he_account,delfriend->i_account);
	tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //删对方
	if(tmp != SQLITE_OK){
	    perror(" delete i error!\n");
	    exit(1);
	}

	bzero(data,sizeof(data));
	sprintf(data,"acc:%s,name:%s,删除成功！江湖路远，有缘再见！",delfriend->he_account,he_name);
	server_go(delfriend->i_fd,data,"server");
    }
}

void delgmem0(oneself_t * delgmem)  //踢群成员的函数0
{
    char sql[200],data[100];
    int tmp,ddd = 0,qx = 0,he_qx = 0,he_fd = 999;

    bzero(sql,sizeof(sql));
    sprintf(sql,"select * from qun_%s where qun_account = \"%s\";",delgmem->my_account,delgmem->qun_num);
    tmp = sqlite3_exec(mydb,sql,kong_fun1,(void *)&ddd,NULL);    //查看有无这个群
    if(tmp != SQLITE_OK){
	perror(" delgmem0 select 0 error!\n");
	exit(1);
    }
    if( ddd == 0){     //您没有这个群
	bzero(data,sizeof(data));
	sprintf(data,"群聊:%s不存在，请检查群聊号是否正确",delgmem->qun_num);
	server_go(delgmem->my_fd,data,"server");
    }else{         //有群
	bzero(sql,sizeof(sql));
	sprintf(sql,"select * from qunliao%s where cy_acc = \"%s\" ;",delgmem->qun_num,delgmem->my_account);
	tmp = sqlite3_exec(mydb,sql,addgmem4,(void *)&qx,NULL);    //判断是否是管理层
	if(tmp != SQLITE_OK){
	    perror(" delgmem0 select 1 error!\n");
	    exit(1);
	}
	if(qx > 2){
	    bzero(data,sizeof(data));
	    sprintf(data,"该群聊:%s中，您的权限不足",delgmem->qun_num);
	    server_go(delgmem->my_fd,data,"server");
	    return;
	}else{              //有权限	
	    bzero(sql,sizeof(sql));        //判断有无这个群成员，以及他的权限
	    sprintf(sql,"select * from qunliao%s where cy_acc = \"%s\" ;",delgmem->qun_num,delgmem->he_acc);
	    tmp = sqlite3_exec(mydb,sql,addgmem4,(void *)&he_qx,NULL);    //判断是否是管理层
	    if(tmp != SQLITE_OK){
		perror(" delgmem0 select 1 error!\n");
		exit(1);
	    }
	    if(he_qx == 0){    //不存在这个人
		bzero(data,sizeof(data));
		sprintf(data,"该群聊:%s中，没有群成员:%s，请仔细核对",delgmem->qun_num,delgmem->he_acc);
		server_go(delgmem->my_fd,data,"server");
		return;
	    }else if(he_qx <= qx){         //他的权限比你高或者相同
		bzero(data,sizeof(data));
		if(he_qx < qx)
		    strcpy(data,"您不能踢出群主!!");
		else{
		    strcpy(data,"您不能踢出管理员!!");
		}
		server_go(delgmem->my_fd,data,"server");
		return;
	    }else{                 //可ti
		bzero(sql,sizeof(sql));
		sprintf(sql,"delete from qunliao%s where cy_acc=\"%s\";",delgmem->qun_num,delgmem->he_acc);
		tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //删除群里的人
		if(tmp != SQLITE_OK){
		    perror(" delgmem0 delete 0 error!\n");
		    exit(1);
		}
		bzero(sql,sizeof(sql));
		sprintf(sql,"delete from qun_%s where qun_account=\"%s\";",delgmem->he_acc,delgmem->qun_num);
		tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //把该群从对方群列表里清除
		if(tmp != SQLITE_OK){
		    perror(" delgmem0 delete 0 error!\n");
		    exit(1);
		}
		bzero(data,sizeof(data));
		sprintf(data,"该群成员：%s已被您踢出，好自为之！",delgmem->he_acc);
		server_go(delgmem->my_fd,data,"server");

		bzero(sql,sizeof(sql));
		sprintf(sql,"select * from qq_data where account = \"%s\";",delgmem->he_acc);
		tmp = sqlite3_exec(mydb,sql,addgmem3,(void *)&he_fd,NULL);    //找到该群成员，返回他的fd
		if(tmp != SQLITE_OK){
		    perror(" gchat1 select 1 error!\n");
		    exit(1);
		}
		bzero(data,sizeof(data));
		if(qx == 1){
		    sprintf(data,"您已被群主：%s移除该群%s,珍重!",delgmem->my_account,delgmem->qun_num);
		}else{
		    sprintf(data,"您已被管理员：%s移除该群%s,珍重!",delgmem->my_account,delgmem->qun_num);
		}
		if(he_fd != 999){      //该群成员在线
		    server_go(he_fd,data,"server");	
		}else{              //该群成员不在线
		    bzero(sql,sizeof(sql));
		    sprintf(sql,"insert into qunnew_%s values(1,\"\",\"server\",\"%s\");",delgmem->he_acc,data);
		    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);    //将该条消息缓存
		    if(tmp != SQLITE_OK){
			perror(" gchat1 insert 1 error!\n");
			exit(1);
		    }
		}
	    }
	}
    }
}
