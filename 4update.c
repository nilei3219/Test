#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mysql/mysql.h>

int main(void)
{
    int ret = -1;

    MYSQL *mysql = NULL; 

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

    //3. 执行SQL语句 更新语句 update
    ret = mysql_query(mysql, "update dept set dname = '1Name', loc = '1Loc' where deptno = 1");
    if (0 != ret)
    {
        printf("mysql_query %s\n", mysql_error(mysql)); 
        return 1;
    }
    else
    {
        printf("Query OK, %ld row affected (0.44 sec)\n", mysql_affected_rows(mysql)); 
    }

    printf("执行SQL语句ok....\n");
    
    //4. 断开连接
    mysql_close(mysql);

    return 0;
}
