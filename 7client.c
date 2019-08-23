#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mysql/mysql.h>

#define SIZE 128

/*
客户端思路分析
1. 初始化

2. 连接到数据库

3. 循环接受用户的输入,执行对应的语句

4. 断开连接

*/

//显示欢迎信息
void show_board(void)
{
    //清屏
    system("clear");
    puts("Welcome to the MySQL monitor.  Commands end with ; or \\g.");
    puts("Your MySQL connection id is 54");
    puts("Server version: 5.7.23 MySQL Community Server (GPL)");
    puts("Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved.");
    puts("Oracle is a registered trademark of Oracle Corporation and/or its affiliates. Other names may be trademarks of their respective owners.");
    puts("Type 'help;' or '\\h' for help. Type '\\c' to clear the current input statement.");
}

//显示结果集
int show_result(MYSQL_RES *res)
{
    int  i = 0;
    long rows = 0;
    unsigned int cols = 0;

    MYSQL_ROW row = NULL;
    MYSQL_FIELD *fields = NULL;

    //获取结果集中行数
    rows = mysql_num_rows(res);
    printf("结果集中行数: %ld\n", rows);

    //获取结果集中列数
    cols = mysql_num_fields(res);
    printf("结果集中列数: %u\n", cols);

    //显示字段的信息
    fields = mysql_fetch_fields(res);

    printf("+----------+---------+-----------+\n|");
    //输出字段信息
    for (i = 0; i < cols; i++)
    {
        printf("%10s|", fields[i].name); 
    }
    printf("\n");
    printf("+----------+----------+----------+\n");

    //输出结果集
    while(1)
    {
        //获取一行数据
        row = mysql_fetch_row(res);    
        if (NULL == row)
        {
            break; 
        }

        printf("|");
        for (i = 0; i < cols; i++)
        {
            printf("%10s|", row[i]); 
        }
        printf("\n");
    }
    printf("+----------+----------+----------+\n");

    return 0;
}

int main(void)
{
    int ret = -1;

    char buf[SIZE];

    MYSQL *mysql = NULL; 
    MYSQL_RES *res = NULL;

    //1. 初始化
    mysql = mysql_init(NULL);
    if (NULL == mysql)
    {
        printf("初始化失败....\n"); 
        return 1;
    }
    printf("MYSQL初始化成功....\n");

    //2. 连接到MYSQL数据库
    mysql = mysql_real_connect(mysql, "192.168.12.29", "root", "123456", "scott", 0, NULL, 0); 
    if (NULL == mysql)
    {
        printf("mysql_real_connect failed....\n"); 
        return 1;
    }
    printf("连接MySQL数据库成功....\n");

    //显示欢迎信息
    show_board();

    //3. 循环接受用户的输入，执行对应的语句
    while(1)
    {
        //输出提示符 mysql>    
        write(STDIN_FILENO, "mysql> ", strlen("mysql> ")); 


        memset(buf, 0, SIZE);
    
        //获取用户输入的信息
        read(STDIN_FILENO, buf, SIZE); 

        //去掉换行符
        buf[strlen(buf) - 1] = '\0';

        if (buf[strlen(buf) - 1] == ';')
        {
            buf[strlen(buf) - 1]  = 0;
        }

        if (buf[0] == '\0')
        {
            continue; 
        }

        //退出
        if (strcmp(buf, "quit") == 0 || strcmp(buf, "exit") == 0)
        {
            printf("Bye\n");
            break; 
        }


        //执行SQL语句
        ret = mysql_query(mysql, buf);
        if (0 != ret)
        {
            printf("mysql_query %s\n", mysql_error(mysql)); 
            continue;
        }

        //4. 获取结果集
        res = mysql_store_result(mysql);
        if (NULL == res)
        {
            printf("Query OK, %ld row affected (0.44 sec)\n", mysql_affected_rows(mysql)); 
        }
        else
        {
            //5. 显示结果集
            show_result(res);
            //6. 释放结果集
            mysql_free_result(res);    
        }
    }


    //7. 断开连接
    mysql_close(mysql);

    return 0;
}
