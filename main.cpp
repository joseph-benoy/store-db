#include<iostream>
#include<windows.h>
#include<cstdlib>
#include<string>
#include<mysql.h>
#include<stdlib.h>
#include<cstring>

#define clr system("cls")
#define head cout<<"------ ABC STORES ------\n\n"
#define pause system("pause")
#define server "localhost"
#define user "test"
#define password "3057"
#define db "shop"

MYSQL_RES *res_set;
MYSQL_ROW row;
MYSQL_FIELD *field;

using namespace std;

class shop
{
    int id,stock;
    double price;
    char name[30];
public:
    void read_item()
    {
        cout<<"Enter the Item ID : ";
        cin>>id;
        cout<<"Enter the Item name : ";
        cin>>name;
        cout<<"Enter the Item price : ";
        cin>>price;
        cout<<"Enter the Item stock : ";
        cin>>stock;
    }
    friend void insert_db(shop,MYSQL *con);
    friend void update_db(shop,MYSQL *con,char id[10]);
    friend void delete_db(shop,MYSQL *con,char id[10]);
};

void insert_db(shop s,MYSQL *con)
{
    string query("insert into item values(");
    string item_id;
    string item_name;
    string item_price;
    string item_stock;
    item_id = to_string(s.id);
    item_name = s.name;
    item_price = to_string(s.price);
    item_stock = to_string(s.stock);
    query+=item_id+",'"+item_name+"',"+item_price+","+item_stock+")";
    mysql_query(con,query.c_str());
}

void update_db(shop s,MYSQL *con,char id[10])
{
    clr;
    head;
    cout<<"Update Item  : "<<id<<endl;
    s.read_item();
    string st("update item set item_id = ");
    string item_id;
    string item_name;
    string item_price;
    string item_stock;
    item_id = to_string(s.id);
    item_name = s.name;
    item_price = to_string(s.price);
    item_stock = to_string(s.stock);
    st+=item_id+","+"item_name = '"+item_name+"',"+"price = "+item_price+","+"stock = "+item_stock+" where item_id = "+id;
    mysql_query(con,st.c_str());
}

void delete_db(shop s,MYSQL *con,char id[10])
{
    string st("delete from item where item_id = ");
    st+=id;
    mysql_query(con,st.c_str());
    pause;
}

int search_db(MYSQL *con,char id[10])
{
    string s("select * from item where item_id = ");
    s+=id;
    mysql_query(con,s.c_str());
    res_set = mysql_store_result(con);
    int num_row = mysql_num_rows(res_set);
    if(num_row==0)
    {
        return 0;
    }
    else
    {
        mysql_free_result(res_set);
        return 1;
    }
}

int main()
{
    MYSQL *connect;
    int opt;
    shop item;
    connect = mysql_init(NULL);
    clr;
    head;
    if(!connect)
    {
        cout<<"Couldn't connect to Database\n";
        exit(1);
    }
    connect = mysql_real_connect(connect,server,user,password,db,0,NULL,0);
    if(!connect)
    {
        cout<<"Database connection error!\n";
        exit(1);
    }
    else
    {
        cout<<"Successfully connect to database\n";
        pause;
    }
    for(;;)
    {
        clr;
        head;
        cout<<"1. Add item\n";
        cout<<"2. Display all items\n";
        cout<<"3. Search for a item\n";
        cout<<"4. Update an item\n";
        cout<<"5. Delete an item\n";
        cout<<"6. Exit\n";
        cout<<"Enter an option : ";
        cin>>opt;
        if(opt==1)
        {
            clr;
            head;
            item.read_item();
            insert_db(item,connect);
            pause;
        }
        else if(opt==2)
        {
            clr;
            head;
            cout<<"- ALL ITEMS -\n\n";
            mysql_query(connect,"select * from item");
            res_set = mysql_store_result(connect);
            int num_field = mysql_num_fields(res_set);
            field = mysql_fetch_fields(res_set);
            for(int i=0;i<num_field;i++)
            {
                cout<<field[i].name<<"  |  ";
            }
            cout<<"\n\n";
            while((row=mysql_fetch_row(res_set)))
            {
                for(int i=0;i<num_field;i++)
                {
                    cout<<row[i]<<"\t|\t";
                }
                cout<<endl;
            }
            cout<<endl;
            mysql_free_result(res_set);
            pause;
        }
        else if(opt==3)
        {
            clr;
            head;
            char id[10];
            cout<<"Enter the Item ID : ";
            cin>>id;
            string s("select * from item where item_id = ");
            s+=id;
            mysql_query(connect,s.c_str());
            res_set = mysql_store_result(connect);
            int num_row = mysql_num_rows(res_set);
            if(num_row==0)
            {
                cout<<"\nItem no found!\n\n";
            }
            else
            {
                int num_field = mysql_num_fields(res_set);
                field = mysql_fetch_fields(res_set);
                cout<<"\n\n";
                for(int i=0;i<num_field;i++)
                {
                    cout<<field[i].name<<"  |  ";
                }
                cout<<"\n\n";
                while((row = mysql_fetch_row(res_set)))
                {
                    for(int i=0;i<num_field;i++)
                    {
                        cout<<row[i]<<"\t|\t";
                    }
                    cout<<endl<<endl;
                }
                mysql_free_result(res_set);
            }
            pause;
        }
        else if(opt==4)
        {
            clr;
            head;
            char id[10];
            cout<<"Enter the Item ID : ";
            cin>>id;
            if(search_db(connect,id)==0)
            {
                cout<<"\nItem not found!\n\n";
            }
            else
            {
                update_db(item,connect,id);
            }
            pause;
        }
        else if(opt==5)
        {
            clr;
            head;
            char id[10];
            cout<<"Enter the Item ID : ";
            cin>>id;
            if(search_db(connect,id)==0)
            {
                cout<<"\nItem not found!\n\n";
            }
            else
            {
                delete_db(item,connect,id);
            }
        }
        else if(opt==6)
        {
            exit(1);
        }
        else
        {
            cout<<"Invalid option!\n";
        }
    }
    return 0;
}
