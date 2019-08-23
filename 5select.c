#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mysql/mysql.h>

int main(void)
{
    int ret = -1;
    int  i = 0;
    long rows = 0;

    unsigned int cols = 0;

    MYSQL *mysql = NULL; 
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;

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

    //3. 执行SQL语句 select
    ret = mysql_query(mysql, "select * from dept");
    if (0 != ret)
    {
        printf("mysql_query %s\n", mysql_error(mysql)); 
        return 1;
    }
    else
    {
        //printf("Query OK, %ld row affected (0.44 sec)\n", mysql_affected_rows(mysql)); 
    }

    printf("执行SQL语句ok....\n");

    //4. 获取结果集
    res = mysql_store_result(mysql);
    if (NULL == res)
    {
        printf("mysql_store_result: %s\n", mysql_error(mysql)); 
        return 1;
    }

    //获取结果集中行数
    rows = mysql_num_rows(res);
    printf("结果集中行数: %ld\n", rows);

    //获取结果集中列数
    cols = mysql_num_fields(res);
    printf("结果集中列数: %u\n", cols);

    //输出结果集
    while(1)
    {
        //获取一行数据
        row = mysql_fetch_row(res);    
        if (NULL == row)
        {
            break; 
        }

        for (i = 0; i < cols; i++)
        {
            printf("%10s ", row[i]); 
        }
        printf("\n");
    }


    //释放结果集
    mysql_free_result(res);    

    //断开连接
    mysql_close(mysql);

    return 0;
}
