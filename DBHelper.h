#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "sqlite3.h"
#include "md5.h"

using namespace std;

struct Tmid {
    int id;
    string name;
    string sex;
    string manager;
    double time;
    double difficulty;
    double correctrate;
    double score;
};
bool cmp_id(Tmid a, Tmid b) { return a.id < b.id;}
bool cmp_name(Tmid a, Tmid b) { return a.name < b.name; }
bool cmp_sex(Tmid a, Tmid b) { return a.sex < b.sex; }
bool cmp_manager(Tmid a, Tmid b) { return a.manager < b.manager; }
bool cmp_time(Tmid a, Tmid b) { return a.time < b.time; }
bool cmp_difficulty(Tmid a, Tmid b) { return a.difficulty < b.difficulty;}
bool cmp_correctrate(Tmid a, Tmid b) { return a.correctrate < b.correctrate; }
bool cmp_score(Tmid a, Tmid b) { return a.score < b.score; }


struct User {
    int id;
    string name;
    string account;
    string password;
    int type;
};

struct User_count {
    int num;
};

//����ӡ
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
//��½ʱ�����û�����
static int callback2(void* data, int argc, char** argv, char** azColName) {
	struct User* user = (User*)data;
	printf("%s = %s\n", azColName[0], argv[0] ? argv[0] : "NULL");
	user->id = argv[0]!= 0 ? atoi(argv[0]) : 0;//���ز���
	user->name = argv[1];
	user->account = argv[2];
	user->password = argv[3];
	user->type = atoi(argv[4]);
    printf("\n");
    return 0;
}
//��ѯ�˺��Ƿ��Ѵ���
static int callback3(void* data, int argc, char** argv, char** azColName) {
    User_count *user_count = (User_count*)data;
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        user_count->num = argv[i] != 0 ? atoi(argv[i]) : 0;;
    }
    printf("\n");
    return 0;
}
//����tmid������ѯ������
static int callback4(void* data, int argc, char** argv, char** azColName) {
    vector<Tmid>* vtemp = (vector<Tmid>*)data;
    struct Tmid temp;
    temp.id = argv[0] != 0 ? atoi(argv[0]) : 0;
    temp.name = argv[1];
    temp.sex = argv[2];
    temp.manager = argv[3];
    temp.time = argv[4] != 0 ? atof(argv[4]) : 0;
    temp.difficulty = argv[5] != 0 ? atof(argv[5]) : 0;
    temp.correctrate = argv[6] != 0 ? atof(argv[6]) : 0;
    temp.score = argv[7] != 0 ? atof(argv[7]) : 0;
    vtemp->push_back(temp);
    printf("\n");
    return 0;
}
//����user������ѯ������
static int callback5(void* data, int argc, char** argv, char** azColName) {
    vector<User>* vtemp = (vector<User>*)data;
    struct User temp;
    temp.id = argv[0] != 0 ? atoi(argv[0]) : 0;
    temp.name = argv[1];
    temp.account = argv[2];
    temp.password = argv[3];
    temp.type = argv[4] != 0 ? atoi(argv[4]) : 0;
    vtemp->push_back(temp);
    printf("\n");
    return 0;
}


class DBHelper {
private:
    string db_name = "Topographic_map_index_data.db";
    string table_tmid_name = "tmid";
    string table_user_name = "user";

public:
    void db_create();
    void table_execute(string sqlstring);
    void PrintMD5(const string& str, MD5& md5);

    void table_tmid_create();
    void table_tmid_insert(int id, string name, string sex, string manager,
        double time, double difficulty, double correctrate, double score);
    void table_tmid_select(string sql_tmid_select);
    void table_tmid_update(string sql_tmid_update);
    void table_tmid_delete(string sql_tmid_delete);
    vector<Tmid> get_tmid();
    vector<Tmid> sort_tmid(int order, int col);//order:0������1�ݼ�  col:��1��8����ڼ����ֶ�

    void table_user_create();
    void table_user_insert(string name, string account, string password, int type);
    void table_user_select(string sql_user_select);
    void table_user_update(string sql_user_update);
    void table_user_delete(string sql_user_delete);
    vector<User> get_user();
    int table_user_count(string account);

    int table_user_login(string account, string password);//0:�˺Ų����� 1��������� 2���˺�����ƥ��
    void table_user_updatepassowrd(string account, string newpassword);

};

void DBHelper::db_create() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open((char*)db_name.data(), &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);
}


void DBHelper::PrintMD5(const string& str, MD5& md5) {
    cout << "MD5(" << str << ") = " << md5.toString() << endl;
}


void DBHelper::table_tmid_create() {
    sqlite3* db;
    char* zErrMsg = 0;
    int  rc;
    char* sql;

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    /**
    * id:           ͼ����ţ�8λΨһ������Ϊ������28080001��
    * name:         Ա������
    * sex:          �Ա�
    * manager:      ҵ�񲿾���
    * time:         ����ʱ������λΪСʱ��
    * difficulty:   ����ͼ�Ѷȣ��򵥣�һ�㣬��ͨ�Ѷȣ����ѣ����ѣ���ֵ�ֱ�Ϊ50,60,65��70,80��
    * correctrate:  ��ȷ��
    * score:        �÷�(�÷� = ����ͼ�Ѷ� * ��ȷ�� / ����ʱ�������ֶ�ֻ�ܲ鿴�������ڽ������޸�)
    */
        string str = "CREATE TABLE " + table_tmid_name + "("  \
        "id INT PRIMARY KEY     NOT NULL," \
        "name           TEXT    NOT NULL," \
        "sex            TEXT    NOT NULL," \
        "manager        TEXT    NOT NULL," \
        "time           REAL    NOT NULL," \
        "difficulty      REAL    ," \
        "correctrate    REAL    ," \
        "score          REAL    );";
        sql = (char*)str.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
}


void DBHelper::table_tmid_insert(int id, string name, string sex, string manager,
    double time, double difficulty, double correctrate, double score) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    ostringstream sql_tmid_insert;
    sql_tmid_insert << "INSERT INTO tmid (id,name,sex,manager,time,difficulty,correctrate,score) "  \
        "VALUES (" << id << ", '" << name << "', '" << sex << "', '" << manager << "', " 
        << time << ", " << difficulty << ", " << correctrate << ", " << score << " );";
    string str = sql_tmid_insert.str();
    sql = (char*)str.data();
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Records created successfully\n");
    }
    sqlite3_close(db);
}


void DBHelper::table_tmid_select(string sql_tmid_select) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = (char*)sql_tmid_select.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}


void DBHelper::table_tmid_update(string sql_tmid_update) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    sql = (char*)sql_tmid_update.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}


void DBHelper::table_tmid_delete(string sql_tmid_delete) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    sql = (char*)sql_tmid_delete.data();
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}


vector<Tmid> DBHelper::get_tmid() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    ostringstream ss;
    ss << "select * from tmid";
    string str = ss.str();
    sql = (char*)str.data();

    /* Execute SQL statement */
    vector<Tmid> vtmid;
    rc = sqlite3_exec(db, sql, callback4, (void*)&vtmid, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);

    return vtmid;
}


vector<Tmid> DBHelper::sort_tmid(int order, int col) {
    vector<Tmid> vtmid;
    vtmid = get_tmid();
    switch (col)
    {
    case 1:sort(vtmid.begin(), vtmid.end(), cmp_id);
        if(order) reverse(vtmid.begin(), vtmid.end());
        break;
    case 2:sort(vtmid.begin(), vtmid.end(), cmp_name);
        if (order) reverse(vtmid.begin(), vtmid.end());
        break;
    case 3:sort(vtmid.begin(), vtmid.end(), cmp_sex);
        if (order) reverse(vtmid.begin(), vtmid.end());
        break;
    case 4:sort(vtmid.begin(), vtmid.end(), cmp_manager);
        if (order) reverse(vtmid.begin(), vtmid.end());
        break;
    case 5:sort(vtmid.begin(), vtmid.end(), cmp_time);
        if (order) reverse(vtmid.begin(), vtmid.end());
        break;
    case 6:sort(vtmid.begin(), vtmid.end(), cmp_difficulty);
        if (order) reverse(vtmid.begin(), vtmid.end());
        break;
    case 7:sort(vtmid.begin(), vtmid.end(), cmp_correctrate);
        if (order) reverse(vtmid.begin(), vtmid.end());
        break;
    case 8:sort(vtmid.begin(), vtmid.end(), cmp_score);
        if (order) reverse(vtmid.begin(), vtmid.end());
        break;
    default:
        break;
    }
    return vtmid;
}


void DBHelper::table_user_create() {
    sqlite3* db;
    char* zErrMsg = 0;
    int  rc;
    char* sql;

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    /**
    * id:           id����
    * name:         ����
    * account:      ��¼��
    * password:     ����
    * type:         �û�����(int��� 0:ϵͳ����Ա,1:ҵ�񲿾���,2:��ҵ��Ա)
    */
    string str = "CREATE TABLE " + table_user_name + "("  \
        "id INTEGER PRIMARY KEY NOT NULL," \
        "name           TEXT    NOT NULL," \
        "account        TEXT    NOT NULL," \
        "password       TEXT    NOT NULL," \
        "type           INT     NOT NULL);";
    sql = (char*)str.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
}


void DBHelper::table_user_insert(string name, string account, string password, int type) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    //�Ȳ�ѯ�˺��Ƿ��Ѿ�����
    int user_num = table_user_count(account);

    if (user_num!=0) {//�˺��Ѵ����޷�����
        cout << "�˺��Ѵ����޷�����!!!" << endl;
    }
    else {//����
        MD5 md5;
        md5.update(password);//����ת��ΪMD5ֵ
        ostringstream sql_user_insert;
        sql_user_insert << "INSERT INTO user (name,account,password,type) "  \
            "VALUES ('" << name << "', '" << account << "', '" << md5.toString() << "', " << type << "); ";
        string str = sql_user_insert.str();
        sql = (char*)str.data();

        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else {
            fprintf(stdout, "Records created successfully\n");
        }
    }
    sqlite3_close(db);
}


void DBHelper::table_user_select(string sql_user_select) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = (char*)sql_user_select.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}

//�˴���ֹ�����룬��������ר�ŵ�table_user_updatepassword()
void DBHelper::table_user_update(string sql_user_update) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    sql = (char*)sql_user_update.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}


void DBHelper::table_user_delete(string sql_user_delete) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    sql = (char*)sql_user_delete.data();
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}


vector<User> DBHelper::get_user() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    ostringstream ss;
    ss << "select * from user";
    string str = ss.str();
    sql = (char*)str.data();

    /* Execute SQL statement */
    vector<User> vuser;
    rc = sqlite3_exec(db, sql, callback5, (void*)&vuser, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);

    return vuser;
}


void DBHelper::table_execute(string sqlstring) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    sql = (char*)sqlstring.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}


int DBHelper::table_user_count(string account) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    ostringstream countstr;
    countstr << "select count(*) from user where account ='" << account << "'";
    string str = countstr.str();
    sql = (char*)str.data();

    /* Execute SQL statement */
    User_count user_count;
    user_count.num = 0;
    rc = sqlite3_exec(db, sql, callback3, (void*)&user_count, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
    return user_count.num;
}


int DBHelper::table_user_login(string account, string password) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    //�ж��˺������Ƿ�ƥ��
    struct User temp;//���ڽ��ղ�ѯ�Ľ��
    temp.id = 0;
    temp.account = "";
    temp.password = "";
    temp.name = "";
    temp.type = 0;

    ostringstream sql_user_identifyaccount;
    sql_user_identifyaccount << "select * from user where account ='" << account << "'";
    string str = sql_user_identifyaccount.str();
    sql = (char*)str.data();

    rc = sqlite3_exec(db, sql, callback2, (void*)&temp, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);

    MD5 md5;
    md5.update(password);//����ת��ΪMD5ֵ
    if (table_user_count(account) == 0) {
        cout << "�˺Ų�����";
    }
    else if (temp.password == md5.toString()) {
        cout << "��½�ɹ�";
    }
    else {
        cout << temp.password << endl;
        cout << md5.toString() << endl;
        cout << "�������";
    }
}


void DBHelper::table_user_updatepassowrd(string account, string newpassword) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open((char*)db_name.data(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    MD5 md5;
    md5.update(newpassword);//תMD5
    ostringstream ss;
    ss << "UPDATE user set password = '" << md5.toString() << "' where account = '" << account << "';";
    cout << "����ĳ�" << md5.toString() << "��" << newpassword << endl;
    string str = ss.str();
    sql = (char*)str.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}
