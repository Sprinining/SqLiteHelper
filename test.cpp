#include "DBHelper.h"
#include <vector>
#include "md5.h"

using namespace std;

int main()
{
    string sql_tmid_select = "SELECT * from tmid";
    string sql_tmid_update = "UPDATE tmid set time = 8.00 where id = 28080001; " \
        "SELECT * from tmid";
    string sql_tmid_delete = "DELETE from tmid where id=28080007; " \
        "SELECT * from tmid";

    string sql_user_select = "SELECT * from user";
    string sql_user_update = "UPDATE user set name = 'ÕÅ·É' where id = 1; " \
        "SELECT * from user";
    string sql_user_delete = "DELETE from user where name='Ã¤×Ð'; " \
        "SELECT * from user";

	DBHelper dbhelper;
	dbhelper.db_create();

	dbhelper.table_tmid_create();
	dbhelper.table_tmid_insert(28080001, "½ð¿ËË¹", "Å®", "¸ÇÂ×", 2.00, 50, 0.8, 100.00);
    dbhelper.table_tmid_insert(28080002, "µÂÀ³ÎÄ", "ÄÐ", "´¸Ê¯", 2.50, 60, 0.6, 100.00);
    dbhelper.table_tmid_insert(28080003, "°¢Ä¾Ä¾", "ÄÐ", "°²ÄÝ", 1.00, 65, 0.9, 100.00);
    dbhelper.table_tmid_insert(28080004, "ÑÇË÷", "ÄÐ", "Ê¯Í·ÈË", 3.50, 80, 0.7, 100.00);
    dbhelper.table_tmid_insert(28080007, "°¢", "ÄÐ", "°²ÄÝ", 1.00, 65, 0.9, 100.00);
	dbhelper.table_tmid_select(sql_tmid_select);
	//dbhelper.table_tmid_update(sql_tmid_update);
	//dbhelper.table_tmid_delete(sql_tmid_delete);
    //vector<Tmid> vtmid;
    //vtmid = dbhelper.sort_tmid(1,7);
    //for (int i = 0; i < vtmid.size(); i++) {
    //    cout << vtmid[i].id << '\t'
    //        << vtmid[i].name << '\t'
    //        << vtmid[i].sex << '\t'
    //        << vtmid[i].manager << '\t'
    //        << vtmid[i].time << '\t'
    //        << vtmid[i].difficulty << '\t'
    //        << vtmid[i].correctrate << '\t'
    //        << vtmid[i].score << endl;
    //}

    

    dbhelper.table_user_create();
    dbhelper.table_user_insert("°µÒá½£Ä§", "001", "666", 0);
    dbhelper.table_user_insert("ÔóÀ­Ë¹", "002", "777", 1);
    dbhelper.table_user_insert("É³»Ê", "003", "888", 2);
    dbhelper.table_user_insert("Äª¸ÊÄÈ", "004", "000", 2);
    dbhelper.table_user_insert("Ã¤×Ð", "005", "111", 2);
    dbhelper.table_user_select(sql_user_select);
    //dbhelper.table_tmid_update(sql_user_update);
    //dbhelper.table_tmid_delete(sql_user_delete);
    //vector<User> vuser;
    //vuser = dbhelper.get_user();
    //for (int i = 0; i < vuser.size(); i++) {
    //    cout << vuser[i].id << '\t'
    //        << vuser[i].name << '\t'
    //        << vuser[i].account << '\t'
    //        << vuser[i].password << '\t'
    //        << vuser[i].type << endl;
    //}

    //dbhelper.table_execute(sql_user_select);

    //dbhelper.table_user_login("001", "777");
    //dbhelper.table_user_updatepassowrd("001", "777");
}
