#include "ReConfig.hpp"

/* 判斷爲空 */
bool str_empty(const char * string)
{
    return NULL == string || '\0' == * string;
}

/**
 * @brief cnf_add_option 添加config文件配置
 * 
 * @param cnf 打开的ini文件句柄
 * @param section 段名
 * @param key 键
 * @param value 值
 * @return true 
 * @return false 
 */
bool cnf_add_option(Config * cnf, const char * section, const char * key, const char * value)
{
    /* 判断段名，键值对是否为空 */
    if(NULL == cnf || str_empty(section) || str_empty(key) || str_empty(value))
    {
        return false;
    }
    /* config数据获取 */
    Data * p = cnf->data;
    while(NULL != p && 0 != strcmp(p->section, section))
    {
        /* 先将指针放置在下一个节点 */
        p = p->next;
    }
    if(NULL == p)
    {
        /* 分配空间存储段名 */
        Data * ps = (Data *)malloc(sizeof(Data));
        if(NULL == ps)
        {
            return false;
        }
        ps->section = (char *)malloc(sizeof(char *) * (strlen(section) + 1));
        /* 将段数据拷贝至对应的空间中 */
        strcpy(ps->section, section);
        ps->option = NULL;
        /* 链表形式储存数据 */
        ps->next   = cnf->data;
        /* 对当前节点赋值 */
        cnf->data  = p = ps;
    }
    Option * q = p->option;
    /* 遍历比较键 */
    while(NULL != q && 0 != strcmp(q->key, key))
    {
        q = q->next;
    }
    if(NULL == q)
    {
        q = (Option *)malloc(sizeof(char) * (strlen(key) + 1));
        if(q == NULL)
        {
            return false;
        }
        q->key = (char *)malloc(sizeof(char) * strlen(key) + 1);
        /* 获取键名 */
        strcpy(q->key, key);
        q->next = p->option;
        p->option = q;
        q->value = (char *)malloc(sizeof(char) * (strlen(value) + 1));
    }
    else if(strlen(q->value) < strlen(value))
    {
        q->value = (char *)realloc(q->value, sizeof(char) * (strlen(value) + 1));
    }
    strcpy(q->value, value);
    return true;
}
/* 判断空 */
char * trim_string(char * string, int mode)
{
    char * left = string;
    if((mode & TRIM_LEFT) != 0)
    {
        for(; * left != '\0'; left++)
        {
            if(0 == isspace(*left))
                break;
        }
    }
    return left;
}

Config * cnf_read_config(const char * filename, char comment, char separator)
{
    Config * cnf = (Config *)malloc(sizeof(Config));
    if(NULL == cnf)
    {
        return NULL;
    }
    cnf->comment    = comment;
    cnf->separator  = separator;
    cnf->data       = NULL;

    if(str_empty(filename))
    {
        PRINT_ERRMSG("filename empty");
        return cnf;
    }
    FILE * fp = fopen(filename, "r");
    if(NULL == fp)
    {
        PRINT_ERRMSG("fopen");
        return NULL;
    }

    char *s, *e, *pLine, sLine[MAX_VALUE];
    char section[MAX_VALUE] = {'\0'}, key[MAX_VALUE], value[MAX_VALUE];
    while(NULL != fgets(sLine, MAX_VALUE, fp))
    {
        pLine = trim_string(sLine, TRIM_BETWEEN);
        if(*pLine == '\0' || *pLine == comment)
        {
            continue;
        }
        s = strchr(pLine, comment);
        if(s != NULL)
        {
            *s = '\0';
        }
        s = strchr(pLine, '[');
        if(s != NULL)
        {
            e = strchr(++s, ']');
            if(e != NULL)
            {
                *e = '\0';
                strcpy(section, s);
            }
        }
        else
        {
            s = strchr(pLine, separator);
            if(s != NULL && * section != '\0')
            {
                *s = '\0';
                strcpy(key, trim_string(pLine, TRIM_RIHGT));
                strcpy(value, trim_string(s + 1, TRIM_LEFT));
                cnf_add_option(cnf, section, key, value);
            }
        }/* end if */
    }/* end while */
    fclose(fp);
    return cnf;
}

/* 分离字符串 */
/************************************
1.以逗号作为分割符
2.将数字单个提取，保存在数组中
3.对数组中的数字进行atof转换
4.进行到下一个数字
5.循环处理到字符串末尾
************************************/
void cnf_transfer_value(char * str, std::vector<float> arr)
{
    int cnt = 0;//用于遍历到字符串结尾
    int num = 0;//用于计算从一个逗号到下一个逗号之间的浮点数有多少个字节
    for(; *(str + cnt) != '\0'; cnt++)
    {
        num++;
        if(*(str + cnt) == ',')//判断浮点型数字的分隔符
        {
            char * value_temp = (char *)calloc(1, num);//创建临时变量保存数据
            memcpy(value_temp, str + (cnt + 1 - num), num); //
            arr.push_back(atof(value_temp));//将字符型数据转换成浮点型数据
            free(value_temp);
            num = 0;//分配空间从零开始计数
        }
    }
}

bool cnf_get_value(Config * cnf, const char * section, const char * key)
{
    Data * p = cnf->data;
    while(NULL != p && 0 != strcmp(p->section, section))
    {
        p = p->next;
    }
    if(NULL == p)
    {
        PRINT_ERRMSG("section");
        return false;
    }
    Option *q = p->option;
    while(NULL != q)
    {
        if(strcmp(q->key, key))
            break;
        q = q->next;
    }
    if(NULL == q)
    {
        PRINT_ERRMSG("key");
        return false;
    }
    cnf->re_string  = q->value;
    cnf->re_int     = atoi(cnf->re_string);
    cnf->re_bool    = 0 == strcmp("true", cnf->re_string);
    cnf->re_double  = atof(cnf->re_string);
    cnf_transfer_value(cnf->re_string, cnf->re_array);

    return true;
}

Data * cnf_Is_section(Config * cnf, const char * section)
{
    Data * p = cnf->data;
    while(NULL != p && 0 != strcmp(p->section, section))
    {
        p = p->next;
    }
    if(NULL == p)
        return NULL;
    
    return p;
}

Option * cnf_Is_option(Config * cnf, const char * section, const char * key)
{
    Data * p = cnf_Is_section(cnf, section);
    if(NULL == p)
        return NULL;
    Option * q = p->option;
    while( NULL != q && 0 != strcmp(q->key, key))
    {
        q = q->next;
    }
    if(NULL == q)
        return NULL;
    return q;
}

bool cnf_write_file(Config * cnf, const char * filename, const char * header)
{
    FILE * fp = fopen(filename, "w");
    if(NULL == fp)
    {
        PRINT_ERRMSG("fopen");
        return false;
    }
    if( !str_empty(header) )
    {
        fprintf(fp, "%c %s\n\n", cnf->comment, header);
    }

    Option * q;
    Data * p = cnf->data;
    while(NULL != p)
    {
        fprintf(fp, "[%s\n]", p->section);
        q = p->option;
        while(NULL != q)
        {
            fprintf(fp, "%s %c %s\n", q->key, cnf->separator, q->value);
            q = q->next;
        }
        p = p->next;
    }

    fclose(fp);
    return true;
}

bool cnf_remove_option(Config * cnf, const char * section, const char * key)
{
    Data * ps = cnf_Is_section(cnf, section);
    if(NULL == ps)
        return false;
    Option *p, *q;
    q = p = ps->option;
    while(NULL != p && 0 != strcmp(p->key, key))
    {
        if(p != q)
        {
            q = p->next;
        }
        p = p->next;
    }
    if(NULL == p)
        return false;
    if(p == q)
        ps->option = p->next;
    else 
        q->next = p->next;
    
    free(p->key);
    free(p->value);
    free(p);
    q = p = NULL;
    return true;
}

bool cnf_remove_section(Config * cnf, const char * section)
{
    if(str_empty(section))
        return false;
    Data * p, * q;
    q = p = cnf->data;
    while(NULL != p && 0 != strcmp(p->section, section))
    {
        if(p != q)
            q = p->next;
        p = q->next;
    }
    if(NULL == p)
        return false;
    if(p == q)
        cnf->data = p->next;
    else
        q->next = p->next;
    Option *ot, *o = p->option;
    while(NULL != o)
    {
        free(o->key);
        free(o->value);
        ot = o;
        o  = o->next;
        free(ot);
    }
    free(p);
    q = p = NULL;

    return true;
}

void destory_config(Config ** cnf)
{
    if(NULL == *cnf)
        return;
    if(NULL != (*cnf)->data)
    {
        Data * pt, *p = (*cnf)->data;
        Option *qt, * q;
        while(NULL != p)
        {
            q = p->option;
            while(NULL != q)
            {

                free(q->key);
                free(q->value);

                qt = q;
                q  = q->next;
                free(qt);
            }
            free(p->section);

            pt = p;
            p  = p->next;
            free(pt);
        }
    }
    free(*cnf);
    *cnf = NULL;
}

void print_config(Config * cnf)
{
    Data * p = cnf->data;
    while(NULL != p)
    {
        printf("[%s]\n", p->section);
        Option * q = p->option;
        while(NULL != q)
        {
            printf("%s%c%s", q->key, cnf->separator, q->value);
            q = q->next;
        }
        p = p->next;
    }
}

int main(int argc, const char * argv[])
{
    Config * cnf = cnf_read_config(argv[1], '#', '=');
    if(NULL ==  cnf)
    {
        PRINT_ERRMSG("cnf read config");
        return -1;
    }
    // if(! cnf_write_file(cnf, "saveconfig.ini", "") )
    // {
    //     PRINT_ERRMSG("cnf write file");
    //     return -1;
    // }
    printf("-------------After Read File------------\n");
    // print_config(cnf);
    
    cnf_get_value(cnf, "ARRARY", "operation_recv_elem");

    return 0;
}
