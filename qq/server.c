
#include "head_server.h"


int main(int argc, char *argv[])
{
    int server_fd,client_fd;
    int tmp;

    if(argc != 3){
	printf("usage:<%s><addr><port>\n",argv[0]);
	exit(1);
    }

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd == -1){
	perror("socket error!\n");
	exit(1);
    }

    int on=1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    struct sockaddr_in server_inf;
    server_inf.sin_family = AF_INET;
    server_inf.sin_port = htons(atoi(argv[2]));
    server_inf.sin_addr.s_addr = inet_addr(argv[1]);
    tmp = bind(server_fd,(struct sockaddr *)&server_inf,sizeof(server_inf));
    if(tmp == -1){
	perror("bind server_inf error!\n");
	exit(1);
    }

    tmp = listen(server_fd,10);
    if(tmp == -1){
	perror("listen error!\n");
	exit(1);
    }
    //数据库打开
    tmp = sqlite3_open("./qq.db",&mydb);
    if(tmp != SQLITE_OK){
	perror("sqlite3_open error!\n");
	exit(1);
    }
    //创建所有人的qq账号数据表
    char sql[100] ="create table if not exists qq_data(fd int,account char[20],name char[20],password chre[20]);";            //文件描述符、qq账号、qq昵称、qq密码
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if(tmp != SQLITE_OK){
	perror("create table qq_data error!\n");
	exit(1);
    }
    bzero(sql,sizeof(sql));
    strcpy(sql,"update qq_data set fd=999 where account !=\"NULL\";");
    tmp = sqlite3_exec(mydb,sql,NULL,NULL,NULL);
    if(tmp != SQLITE_OK){
	perror("update table qq_data  999  error!\n");
	exit(1);
    }
    
    struct sockaddr_in client_inf;
    struct sockaddr_in kong;
    pthread_t pid;
    int len;
    len = sizeof(client_inf);
    while(1){
	client_inf = kong;
	client_fd = accept(server_fd,(struct sockaddr *)&client_inf,&len);
	if(client_fd == -1){
	    perror("accept error!\n");
	    exit(1);
	}
	tmp = pthread_create(&pid,NULL,user,(void *)&client_fd);
	if( tmp != 0){
	    perror("pthread_create error!\n");
	    exit(1);
	}
	pthread_detach(pid);
    }
    return 0;
}

