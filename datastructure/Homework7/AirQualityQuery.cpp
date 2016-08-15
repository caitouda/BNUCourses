#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <set>
#include <map>
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <tchar.h>

using namespace std;

enum QUERY_TYPE {FOR_DAY,FOR_WEEK,FOR_MONTH,FOR_QUARTER,FOR_YEAR};//類型 天 周 月 季度 年
enum MONTH {JAN=1,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC};//月份
enum QUARTER {FIRST=1,SECOND,THIRD,FOURTH};//四個季度
int monthday[13]= {0,31,28,31,30,31,30,31,31,30,31,30,31}; //記錄每月天數

class City
{
public:
    string city_Name;//城市名稱
    string city_ID;//城市編號
    string pollute_Type;//污染類型
    int pollute_Value;//污染值
    string polluteLevelRome;//羅馬等級
    string polluteLevel;//等級
    int year;
    int month;
    int day;
public:
    friend istream& operator >> (istream& is, City& city)//重載輸入
    {
        is>>city.city_ID>>city.city_Name>>city.pollute_Type>>city.pollute_Value>>city.polluteLevelRome>>city.polluteLevel
        >>city.year>>city.month>>city.day;
        return is;
    }
    friend ostream& operator << (ostream& os,City& city)//重載輸出
    {
        os<<city.city_ID<<city.city_Name<<city.pollute_Type<<city.pollute_Value<<city.polluteLevelRome<<city.polluteLevel
        <<city.year<<city.month<<city.day;
        return os;
    }
    City()//構造函數
    {
        city_Name="";
        city_ID="";
        pollute_Type="";
        pollute_Value=0;
        polluteLevelRome="";
        polluteLevel="";
        year=2006;
        month=JAN;
        day=1;
    }

    ~City() {} //析構函數
};

vector<City> dataTable;	//檔中的所有資料表
set<string> s_city;		//各個城市名字集合

void showMenu()//程式開始介面顯示
{
    cout<<"   功能選擇： "<<endl;
    cout<<"1、普通查詢"<<endl;
    cout<<"2、統計查詢"<<endl;
    cout<<"3、排序查詢"<<endl;
    cout<<"4、退出系統"<<endl;
}


bool fileRead()//檔讀入函數
{
    ifstream fin("data.txt");
    while(!fin.eof())
    {
        City city;
        fin>>city;
        dataTable.push_back(city);//動態存入陣列
        s_city.insert(city.city_Name);
    }

    fin.close();
    return true;
}


int GetWeek(int mouth,int day)//獲取周數
{
    int time=0;

    for (int i=0; i<mouth; i++)
    {
        time+=monthday[i];
    }
    time+=day;
    if (time%7==0)return time/7;
    else
        return time/7+1;

}

QUARTER GetQuarter(int month)//根據月份取季度
{
    QUARTER TYPE;
    switch (month)
    {
    case JAN:
    case FEB:
    case MAR:
        TYPE=FIRST;//第一季度
        break;
    case APR:
    case MAY:
    case JUN:
        TYPE=SECOND;//第二季度
        break;
    case JUL:
    case AUG:
    case SEP:
        TYPE=THIRD;//第三季度
        break;
    case OCT:
    case NOV:
    case DEC:
        TYPE=FOURTH;//第四季度
        break;
    default:
        cout<<"error input！"<<endl;
    }

    return TYPE;
}

QUARTER GetQuarter2(int jidu)//獲取季度值2
{
    QUARTER TYPE;
    switch (jidu)
    {
    case 1:
        TYPE=FIRST;//第一季度
        break;
    case 2:
        TYPE=SECOND;//第二季度
        break;
    case 3:
        TYPE=THIRD;//第三季度
        break;
    case 4:
        TYPE=FOURTH;//第四季度
        break;
    default:
        cout<<"error input！"<<endl;
    }

    return TYPE;
}

QUERY_TYPE getQueryType(int type)//獲得排序類型
{
    QUERY_TYPE ret;
    switch(type)
    {
    case 0:
        ret=FOR_DAY;
        break;
    case 1:
        ret=FOR_WEEK;
        break;
    case 2:
        ret=FOR_MONTH;
        break;
    case 3:
        ret=FOR_QUARTER;
        break;
    case 4:
        ret=FOR_YEAR;
        break;
    default:
        cout<<"error input！"<<endl;

    }
    return ret;
}

string getPollute_Type(int type)//獲得污染類型
{
    string ret;
    switch(type)
    {
    case 1:
        ret="優";
        break;
    case 2:
        ret="良";
        break;
    case 3:
        ret="輕微污染";
        break;
    case 4:
        ret="輕度污染";
        break;
    case 5:
        ret="中度污染";
        break;
    case 6:
        ret="中度重污染";
        break;
    case 7:
        ret="重污染";
        break;
    default:
        cout<<"error input！"<<endl;

    }
    return ret;
}

//以下是普通查詢
void shownorm(vector<City> Table1, string City_Name)//普通顯示函數
{

    cout<<endl;
    if (Table1.size()==0)
    {
        cout<<"沒有此記錄"<<endl;
        return;
    }
    cout<<"         "<<City_Name<<"："<<endl;
    for (size_t i=0; i!=Table1.size(); ++i )
    {
        cout<<"2006年  "<<Table1[i].month<<"月  "<<Table1[i].day<<"日"<<endl;
        cout<<"首要污染物："<<setw(12)<<Table1[i].pollute_Type<<endl;
        cout<<"污染指數："<<setw(14)<<Table1[i].pollute_Value<<endl;
        cout<<"污染物級別："<<setw(12)<<Table1[i].polluteLevelRome<<endl;
        cout<<"空氣狀況："<<setw(14)<<Table1[i].polluteLevel<<endl<<endl;
    }
}

bool normInquire(vector<City> TABLE,string City_Name,QUERY_TYPE type=FOR_DAY)//第一個要求普通查詢函數
{
    vector<City> oneCityTable;//一個城市的city組合
    oneCityTable.clear();     //清空

    vector<City> oneTable;//想獲取的城市組合（一周，一月，一季度，或者是一年 ）
    oneTable.clear();

    for (vector<City>::iterator iter=TABLE.begin(); iter!=TABLE.end(); ++iter)
    {
        if ((*iter).city_Name == City_Name)
        {
            oneCityTable.push_back(*iter);//得到onecitytable
        }
    }
    switch (type)
    {
    case FOR_DAY:
    {
        cout<<"輸入日期（月 日）："<<endl;//天查詢
        int mo,da;
        cin>>mo>>da;
        for (vector<City>::iterator One_iter=oneCityTable.begin(); One_iter!=oneCityTable.end(); ++One_iter)
        {
            if (mo == (*One_iter).month && da == (*One_iter).day )
            {
                oneTable.push_back(*One_iter);
            }
        }
        shownorm(oneTable,City_Name);
        break;
    }
    case FOR_WEEK:
    {
        cout<<"輸入查詢第幾周(x <= 53)："<<endl;//周查詢
        int week;
        cin>>week;
        for (vector<City>::iterator One_iter=oneCityTable.begin(); One_iter!=oneCityTable.end(); ++One_iter)
        {
            if (week == GetWeek((*One_iter).month,(*One_iter).day) )
            {
                oneTable.push_back(*One_iter);
            }
        }
        shownorm(oneTable,City_Name);
        break;
    }
    case FOR_MONTH:
    {
        cout<<"輸入查詢月份："<<endl;//月查詢
        int month;
        cin>>month;
        for (vector<City>::iterator One_iter=oneCityTable.begin(); One_iter!=oneCityTable.end(); ++One_iter)
        {
            if (month == (*One_iter).month )
            {
                oneTable.push_back(*One_iter);
            }
        }
        shownorm(oneTable,City_Name);
        break;
    }
    case FOR_QUARTER:
    {
        int jidu;
        QUARTER type;
        cout<<"輸入查詢季度(x <= 4)："<<endl;//季度查詢
        cin>>jidu;
        type=GetQuarter2(jidu);

        for (vector<City>::iterator One_iter=oneCityTable.begin(); One_iter!=oneCityTable.end(); ++One_iter)
        {
            if (type == GetQuarter((*One_iter).month ))
            {
                oneTable.push_back(*One_iter);
            }
        }
        shownorm(oneTable,City_Name);
        break;
    }
    case FOR_YEAR:
    {
        shownorm(oneCityTable,City_Name);
        break;
    }
    default:
    {
        cout<<"error input!"<<endl;
        return false;
    }
    }//結束switch
    return true;
}

//以下 是 統計函數
//統計查詢1
void statisticsDisplay(map<string,int> result,string city_Name)//統計輸出
{
    cout<<endl;
    cout<<"      "<<city_Name<<"："<<endl;
    for (map<string,int>::iterator mapIter=result.begin(); mapIter!=result.end(); ++mapIter)
    {
        cout<<setw(10)<<(*mapIter).first<<setw(2)<<"："<<setw(12)<<(*mapIter).second<<endl;
    }
}

bool statisticQuery(vector<City> table,const string city_Name,QUERY_TYPE type=FOR_WEEK)
{
    vector<City> oneCityTable;
    oneCityTable.clear();//清空
    for (vector<City>::iterator iter=table.begin(); iter!=table.end(); ++iter)
    {
        if ((*iter).city_Name == city_Name)
        {
            oneCityTable.push_back(*iter);
        }
    }

    map<string,int> queryResult;
    queryResult.clear();
    queryResult["優"]=0;
    queryResult["良"]=0;
    queryResult["輕微污染"]=0;
    queryResult["輕度污染"]=0;
    queryResult["中度污染"]=0;
    queryResult["中度重污染"]=0;
    queryResult["重污染"]=0;
    switch (type)
    {
    case FOR_WEEK:
    {
        int week;
        cout<<"輸入周編號(<= 53)："<<endl;
        cin>>week;

        for (size_t i=0; i!=oneCityTable.size(); ++i)
        {
            if (week == GetWeek(oneCityTable[i].month,oneCityTable[i].day))
            {
                queryResult[oneCityTable[i].polluteLevel]++;
            }
        }
        statisticsDisplay(queryResult,city_Name);
    }
    break;
    case FOR_MONTH:
    {
        int month;
        cout<<"輸入月編號："<<endl;
        cin>>month;

        for (size_t i=0; i!=oneCityTable.size(); ++i)
        {
            if (month == oneCityTable[i].month)
            {
                queryResult[oneCityTable[i].polluteLevel]++;
            }
        }
        statisticsDisplay(queryResult,city_Name);
    }
    break;
    case FOR_QUARTER:
    {
        int quarter;
        cout<<"輸入第幾季度："<<endl;
        cin>>quarter;

        for (size_t i=0; i!=oneCityTable.size(); ++i)
        {
            if (quarter == GetQuarter(oneCityTable[i].month))
            {
                queryResult[oneCityTable[i].polluteLevel]++;
            }
        }

        statisticsDisplay(queryResult,city_Name);
    }
    break;
    case FOR_YEAR:
    {
        int year;
        cout<<"輸入第幾年："<<endl;
        cin>>year;

        for (size_t i=0; i!=oneCityTable.size(); ++i)
        {
            if (year == oneCityTable[i].year)
            {
                queryResult[oneCityTable[i].polluteLevel]++;
            }
        }

        statisticsDisplay(queryResult,city_Name);
    }
    break;
    default:
        cout<<"error input!"<<endl;
        return false;

    }
    return true;
}

//統計查詢2
void statisticsDisplay2(map<string,int> result,string PolluLevel)//統計的第二種情況的統計顯示函數
{
    cout<<endl;
    cout<<"        品質為"<<PolluLevel<<endl;
    for (map<string,int>::iterator mapIter=result.begin(); mapIter!=result.end(); ++mapIter)
    {
        cout<<setw(8)<<(*mapIter).first<<setw(2)<<"："<<setw(12)<<(*mapIter).second<<endl;
    }
}

bool statisticQuery2(vector<City> table,string PolluLevel,QUERY_TYPE type=FOR_DAY,int T=0)//根據污染類型的統計函數
{

    vector<City> onePolluteTable;
    onePolluteTable.clear();//清空


    for (vector<City>::iterator iter=table.begin(); iter!=table.end(); ++iter)
    {
        if ((*iter).polluteLevel == PolluLevel)
        {
            onePolluteTable.push_back(*iter);//獲取一種污染類型的城市
        }
    }

    map<string,int> queryResult;
    queryResult.clear();
    for(set<string>::iterator ITER=s_city.begin(); ITER!=s_city.end(); ++ITER)
    {
        queryResult.insert(pair<string,int>((*ITER),0));

    }
    map<string,int> queryPlay;//污染天數大於某一個值 的時候 要showplay的函數
    queryPlay.clear();
    switch (type)
    {
    case FOR_WEEK:
    {
        int week;
        cout<<"輸入周編號(<= 53)："<<endl;
        cin>>week;

        cout<<onePolluteTable.size()<<endl;
        for (size_t i=0; i!=onePolluteTable.size(); ++i)
        {
            if (week == GetWeek(onePolluteTable[i].month,onePolluteTable[i].day))
            {
                queryResult[onePolluteTable[i].city_Name]++;
            }
        }
        for(set<string>::iterator iter=s_city.begin(); iter!=s_city.end(); ++iter)
        {
            if (queryResult[(*iter)] >= T)
            {
                queryPlay.insert(make_pair((*iter),queryResult[(*iter)]));
            }
        }
        statisticsDisplay2(queryPlay,PolluLevel);
    }
    break;

    case FOR_MONTH:
    {
        int month;
        cout<<"輸入月編號："<<endl;
        cin>>month;

        for (size_t i=0; i!=onePolluteTable.size(); ++i)
        {
            if (month == onePolluteTable[i].month)
            {
                queryResult[onePolluteTable[i].city_Name]++;
            }
        }
        for(set<string>::iterator iter=s_city.begin(); iter!=s_city.end(); ++iter)
        {
            if (queryResult[(*iter)] >= T)
            {
                queryPlay.insert(make_pair((*iter),queryResult[(*iter)]));
            }
        }

        statisticsDisplay2(queryPlay,PolluLevel);
    }
    break;
    case FOR_QUARTER:
    {
        int quarter;
        cout<<"輸入第幾季度："<<endl;
        cin>>quarter;

        for (size_t i=0; i!=onePolluteTable.size(); ++i)
        {
            if (quarter == GetQuarter(onePolluteTable[i].month))
            {
                queryResult[onePolluteTable[i].city_Name]++;
            }
        }

        for(set<string>::iterator iter=s_city.begin(); iter!=s_city.end(); ++iter)
        {
            if (queryResult[(*iter)] >= T)
            {
                queryPlay.insert(make_pair((*iter),queryResult[(*iter)]));
            }
        }

        statisticsDisplay2(queryPlay,PolluLevel);
    }
    break;
    case FOR_YEAR:
    {
        int year;
        cout<<"輸入第幾年："<<endl;
        cin>>year;

        for (size_t i=0; i!=onePolluteTable.size(); ++i)
        {
            if (year == onePolluteTable[i].year)
            {
                queryResult[onePolluteTable[i].city_Name]++;
            }
        }

        for(set<string>::iterator iter=s_city.begin(); iter!=s_city.end(); ++iter)
        {
            if (queryResult[(*iter)] >= T)
            {
                queryPlay.insert(make_pair((*iter),queryResult[(*iter)]));
            }
        }
        statisticsDisplay2(queryPlay,PolluLevel);
    }
    break;
    default:
        cout<<"error input!"<<endl;
        return false;

    }
    return true;
}

//第三個要求是排序查詢 函數
int cmp(const pair<std::string, int>& x, const pair<string, int>& y)
{
    return x.second < y.second;
}

void sortMapByValue(map<string, int>& tMap, vector<pair<string, int> >& tVector)
{
    for (map<string, int>::iterator curr = tMap.begin(); curr != tMap.end(); curr++)
    {
        tVector.push_back(make_pair(curr->first, curr->second));
    }
    sort(tVector.begin(), tVector.end(), cmp);//自己寫排序演算法
}

void HeapAdjust(vector<pair<string ,int> > &Heap, int s, int m)
{

    for (int j = s * 2 + 1; j < m; j = j * 2 + 1)
    {
        if (j < m - 1 && Heap[j + 1].second> Heap[j].second)
            j++;
        if ( Heap[s].second > Heap[j].second)//生成大根堆
            break;
        swap(Heap[s],Heap[j]);
        s = j;
    }
}

vector<pair<string ,int> > HeapSortMinK(vector<pair<string ,int> > &Heap,int k)
{

    vector<pair<string, int> > minK;
    minK.clear();

    for (int i=0; i<k; i++)
    {
        minK.push_back(Heap[i]);
    }

    for (int q=k/2 ; q>0 ; q--)
    {
        HeapAdjust(minK,q-1,k);
    }

    for (size_t j=k; j<Heap.size(); j++)
    {
        if (minK[0].second>Heap[j].second)
        {
            minK[0]=Heap[j];
            HeapAdjust(minK,0,k);
        }
    }

    sort(minK.begin(),minK.end(),cmp);//將minK排序，因為求出的K個最小值是亂序的


    return minK;
}

vector<pair<string ,int> > SortByValue(map<string, int>& tMap, vector<pair<string, int> >& tVector,int k)
{
    for (map<string, int>::iterator curr = tMap.begin(); curr != tMap.end(); curr++)
    {
        tVector.push_back(make_pair(curr->first, curr->second));
    }

    return HeapSortMinK(tVector,k);


}

void sortDisplay(vector<pair<string,int> > result,int rankCount)
{
    cout<<endl;
    cout<<"            排行榜如下"<<endl;
    for (size_t i=0; i<result.size(); i++)
    {
        cout<<"第"<<i+1<<"名："<<setw(16)<<result[i].first<<setw(12)<<result[i].second<<endl;
    }
    cout<<"***************************************"<<endl;
}

bool sortQuery(vector<City> table,int rankCount,QUERY_TYPE type=FOR_WEEK)//排序函數
{
    vector<pair<string,int> > tVector;
    map<string,int> queryResult;
    queryResult.clear();
    tVector.clear();
    switch (type)
    {
    case FOR_WEEK:
    {
        int week;
        cout<<"輸入周數(<=53)：";
        cin>>week;
        for (size_t i=0; i!=table.size(); ++i)
        {
            if (week == GetWeek(table[i].month,table[i].day))
            {

                map<string,int>::iterator it=queryResult.find(table[i].city_Name);
                if (it != queryResult.end())
                {
                    queryResult[table[i].city_Name]+=table[i].pollute_Value;
                }
                else
                {
                    queryResult.insert(map<string ,int>::value_type(table[i].city_Name,table[i].pollute_Value));
                }
            }

        }
        vector<pair<string, int> > vec=SortByValue(queryResult,tVector,rankCount);
        sortDisplay(vec,rankCount);
        break;
    }
    case FOR_MONTH:
    {

        int month;
        cout<<"輸入月份：";
        cin>>month;
        for (size_t i=0; i!=table.size(); ++i)
        {
            if (month == table[i].month)
            {

                map<string,int>::iterator it=queryResult.find(table[i].city_Name);
                if (it != queryResult.end())
                {
                    queryResult[table[i].city_Name]+=table[i].pollute_Value;
                }
                else
                {
                    queryResult.insert(map<string ,int>::value_type(table[i].city_Name,table[i].pollute_Value));
                }
            }

        }
        vector<pair<string, int> > vec=SortByValue(queryResult,tVector,rankCount);
        sortDisplay(vec,rankCount);
        break;
    }
    case FOR_QUARTER:
    {
        int quarter;
        cout<<"輸入季度：";
        cin>>quarter;
        for (size_t i=0; i!=table.size(); ++i)
        {
            if (quarter == GetQuarter(table[i].month))
            {

                map<string,int>::iterator it=queryResult.find(table[i].city_Name);
                if (it != queryResult.end())
                {
                    queryResult[table[i].city_Name]+=table[i].pollute_Value;
                }
                else
                {
                    queryResult.insert(map<string ,int>::value_type(table[i].city_Name,table[i].pollute_Value));
                }
            }

        }
        vector<pair<string, int> > vec=SortByValue(queryResult,tVector,rankCount);

        sortDisplay(vec,rankCount);

        break;
    }
    case FOR_YEAR:
    {

        int  year;
        cout<<"輸入年份：";
        cin>>year;
        for (size_t i=0; i!=table.size(); ++i)
        {
            if (year == table[i].year)
            {

                map<string,int>::iterator it=queryResult.find(table[i].city_Name);
                if (it != queryResult.end())
                {
                    queryResult[table[i].city_Name]+=table[i].pollute_Value;
                }
                else
                {
                    queryResult.insert(map<string ,int>::value_type(table[i].city_Name,table[i].pollute_Value));
                }
            }

        }
        vector<pair<string, int> > vec=SortByValue(queryResult,tVector,rankCount);
        sortDisplay(vec,rankCount);
        break;
    }
    default:
        cout<<"error input!"<<endl;
        return false;
    }
    return true;
}

int main()
{
    int selectItem;
    int queryType;
    int pollute_Type;
    int time;
    int rankLen;
    string city_Name;
    cout<<"讀入數據。。。。。。"<<endl;
    fileRead();//讀入文件
    cout<<"讀入成功！"<<endl;
    system("cls");
    while(1)
    {
        showMenu();
        cin>>selectItem;
        switch (selectItem)
        {
        case 1:
        {
            char ch;
            while (1)
            {
                cout<<"輸入城市名稱：";
                cin>>city_Name;
                if (s_city.find(city_Name) ==s_city.end())//尋找該城市
                {
                    cout<<"沒有該城市，請重新輸入！"<<endl;
                    break;
                }
                cout<<"請選擇查詢類型："<<endl;
                cout<<"0、查詢一天"<<endl;
                cout<<"1、查詢一周"<<endl;
                cout<<"2、查詢一月"<<endl;
                cout<<"3、查詢一季度"<<endl;
                cout<<"4、查詢一年"<<endl;
                cin>>queryType;
                if (!normInquire(dataTable,city_Name,getQueryType(queryType)))
                {
                    cout<<"查詢失敗，請重新輸入"<<endl;
                }
                cout<<"是否退出（Y/N）"<<endl;
                cin>>ch;
                if(ch=='Y'||ch=='y') break;
            }
        }
        system("cls");
        break;
        case 2:
        {
            char ch;
            while (1)
            {
                int flag;
                cout<<"輸入查詢內容："<<endl;
                cout<<endl;
                cout<<"1、統計一個城市污染狀況"<<endl<<endl;
                cout<<"2、輸入污染狀況查詢城市"<<endl<<endl;
                cin>>flag;
                if (flag==1)
                {
                    cout<<"輸入城市名稱：";
                    cin>>city_Name;
                    if (s_city.find(city_Name) ==s_city.end())//尋找該城市
                    {
                        cout<<"沒有該城市，請重新輸入！"<<endl;
                        break;
                    }
                    cout<<"請選擇查詢類型："<<endl;
                    cout<<"1、統計一周"<<endl;
                    cout<<"2、統計一月"<<endl;
                    cout<<"3、統計一季度"<<endl;
                    cout<<"4、統計一年"<<endl;
                    cin>>queryType;
                    if (!statisticQuery(dataTable,city_Name,getQueryType(queryType)))
                    {
                        cout<<"查詢失敗，請重新輸入"<<endl;
                    }
                }
                if (flag==2)
                {
                    cout<<"請選擇查詢範圍："<<endl;
                    cout<<"1、周查詢"<<endl;
                    cout<<"2、月查詢"<<endl;
                    cout<<"3、季度查詢"<<endl;
                    cout<<"4、年查詢"<<endl;
                    cin>>queryType;
                    cout<<"請選擇查詢的空氣品質狀況："<<endl;
                    cout<<"1、優"<<endl;
                    cout<<"2、良"<<endl;
                    cout<<"3、輕微污染"<<endl;
                    cout<<"4、輕度污染"<<endl;
                    cout<<"5、中度污染"<<endl;
                    cout<<"6、中度重污染"<<endl;
                    cout<<"7、重污染"<<endl;
                    cin>>pollute_Type;
                    cout<<"請插入污染類型的天數："<<endl<<endl;
                    cout<<"注意：如果是周查詢，天數 <= 7 "<<endl<<endl;
                    cout<<"      如果是月查詢，天數 <= the day of month"<<endl<<endl;
                    cout<<"      如果是季度查詢，天數 <= the day of quarter"<<endl<<endl;
                    cout<<"      如果是年查詢，天數 <= 365 "<<endl<<endl;
                    cin>>time;
                    if (!statisticQuery2(dataTable,getPollute_Type(pollute_Type),getQueryType(queryType),time))
                    {
                        cout<<"查詢失敗，請重新輸入"<<endl;
                    }
                }
                cout<<"是否退出（Y/N）"<<endl;
                cin>>ch;
                if(ch=='Y'||ch=='y') break;
            }
        }
        system("cls");
        break;
        case 3:
        {
            char ch;
            while (1)//排序查詢
            {
                cout<<"輸入排行榜長度：";
                cin>>rankLen;
                cout<<"請選擇查詢類型："<<endl;
                cout<<"1、一周排行"<<endl;
                cout<<"2、一月排行"<<endl;
                cout<<"3、一季度排行"<<endl;
                cout<<"4、一年排行"<<endl;
                cin>>queryType;
                if (!sortQuery(dataTable,rankLen,getQueryType(queryType)))
                {
                    cout<<"查詢失敗，請重新輸入"<<endl;
                }
                cout<<"是否退出（Y/N）"<<endl;
                cin>>ch;
                if(ch=='Y'||ch=='y') break;
            }

        }
        system("cls");//清屏
        break;
        case 4:
            cout<<"感謝您的使用！"<<endl;
            exit(1);
            break;
        default:
            cout<<"輸入錯誤！請重新輸入！"<<endl;
            system("cls");
            break;
        }
    }
    return 0;
}
