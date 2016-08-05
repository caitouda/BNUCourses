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

enum QUERY_TYPE {FOR_DAY,FOR_WEEK,FOR_MONTH,FOR_QUARTER,FOR_YEAR};//��� �� �� �� ���� ��
enum MONTH {JAN=1,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC};//�·�
enum QUARTER {FIRST=1,SECOND,THIRD,FOURTH};//�Ă�����
int monthday[13]= {0,31,28,31,30,31,30,31,31,30,31,30,31}; //ӛ�ÿ���씵

class City
{
public:
    string city_Name;//�������Q
    string city_ID;//���о�̖
    string pollute_Type;//��Ⱦ���
    int pollute_Value;//��Ⱦֵ
    string polluteLevelRome;//�_�R�ȼ�
    string polluteLevel;//�ȼ�
    int year;
    int month;
    int day;
public:
    friend istream& operator >> (istream& is, City& city)//���dݔ��
    {
        is>>city.city_ID>>city.city_Name>>city.pollute_Type>>city.pollute_Value>>city.polluteLevelRome>>city.polluteLevel
        >>city.year>>city.month>>city.day;
        return is;
    }
    friend ostream& operator << (ostream& os,City& city)//���dݔ��
    {
        os<<city.city_ID<<city.city_Name<<city.pollute_Type<<city.pollute_Value<<city.polluteLevelRome<<city.polluteLevel
        <<city.year<<city.month<<city.day;
        return os;
    }
    City()//���캯��
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

    ~City() {} //��������
};

vector<City> dataTable;	//�n�е������Y�ϱ�
set<string> s_city;		//�����������ּ���

void showMenu()//��ʽ�_ʼ�����@ʾ
{
    cout<<"   �����x�� "<<endl;
    cout<<"1����ͨ��ԃ"<<endl;
    cout<<"2���yӋ��ԃ"<<endl;
    cout<<"3�������ԃ"<<endl;
    cout<<"4���˳�ϵ�y"<<endl;
}


bool fileRead()//�n�x�뺯��
{
    ifstream fin("data.txt");
    while(!fin.eof())
    {
        City city;
        fin>>city;
        dataTable.push_back(city);//�ӑB�������
        s_city.insert(city.city_Name);
    }

    fin.close();
    return true;
}


int GetWeek(int mouth,int day)//�@ȡ�ܔ�
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

QUARTER GetQuarter(int month)//�����·�ȡ����
{
    QUARTER TYPE;
    switch (month)
    {
    case JAN:
    case FEB:
    case MAR:
        TYPE=FIRST;//��һ����
        break;
    case APR:
    case MAY:
    case JUN:
        TYPE=SECOND;//�ڶ�����
        break;
    case JUL:
    case AUG:
    case SEP:
        TYPE=THIRD;//��������
        break;
    case OCT:
    case NOV:
    case DEC:
        TYPE=FOURTH;//���ļ���
        break;
    default:
        cout<<"error input��"<<endl;
    }

    return TYPE;
}

QUARTER GetQuarter2(int jidu)//�@ȡ����ֵ2
{
    QUARTER TYPE;
    switch (jidu)
    {
    case 1:
        TYPE=FIRST;//��һ����
        break;
    case 2:
        TYPE=SECOND;//�ڶ�����
        break;
    case 3:
        TYPE=THIRD;//��������
        break;
    case 4:
        TYPE=FOURTH;//���ļ���
        break;
    default:
        cout<<"error input��"<<endl;
    }

    return TYPE;
}

QUERY_TYPE getQueryType(int type)//�@���������
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
        cout<<"error input��"<<endl;

    }
    return ret;
}

string getPollute_Type(int type)//�@����Ⱦ���
{
    string ret;
    switch(type)
    {
    case 1:
        ret="��";
        break;
    case 2:
        ret="��";
        break;
    case 3:
        ret="�p΢��Ⱦ";
        break;
    case 4:
        ret="�p����Ⱦ";
        break;
    case 5:
        ret="�ж���Ⱦ";
        break;
    case 6:
        ret="�ж�����Ⱦ";
        break;
    case 7:
        ret="����Ⱦ";
        break;
    default:
        cout<<"error input��"<<endl;

    }
    return ret;
}

//��������ͨ��ԃ
void shownorm(vector<City> Table1, string City_Name)//��ͨ�@ʾ����
{

    cout<<endl;
    if (Table1.size()==0)
    {
        cout<<"�]�д�ӛ�"<<endl;
        return;
    }
    cout<<"         "<<City_Name<<"��"<<endl;
    for (size_t i=0; i!=Table1.size(); ++i )
    {
        cout<<"2006��  "<<Table1[i].month<<"��  "<<Table1[i].day<<"��"<<endl;
        cout<<"��Ҫ��Ⱦ�"<<setw(12)<<Table1[i].pollute_Type<<endl;
        cout<<"��Ⱦָ����"<<setw(14)<<Table1[i].pollute_Value<<endl;
        cout<<"��Ⱦ�）�e��"<<setw(12)<<Table1[i].polluteLevelRome<<endl;
        cout<<"�՚��r��"<<setw(14)<<Table1[i].polluteLevel<<endl<<endl;
    }
}

bool normInquire(vector<City> TABLE,string City_Name,QUERY_TYPE type=FOR_DAY)//��һ��Ҫ����ͨ��ԃ����
{
    vector<City> oneCityTable;//һ�����е�city�M��
    oneCityTable.clear();     //���

    vector<City> oneTable;//��@ȡ�ĳ��нM�ϣ�һ�ܣ�һ�£�һ���ȣ�������һ�� ��
    oneTable.clear();

    for (vector<City>::iterator iter=TABLE.begin(); iter!=TABLE.end(); ++iter)
    {
        if ((*iter).city_Name == City_Name)
        {
            oneCityTable.push_back(*iter);//�õ�onecitytable
        }
    }
    switch (type)
    {
    case FOR_DAY:
    {
        cout<<"ݔ�����ڣ��� �գ���"<<endl;//���ԃ
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
        cout<<"ݔ���ԃ�ڎ���(x <= 53)��"<<endl;//�ܲ�ԃ
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
        cout<<"ݔ���ԃ�·ݣ�"<<endl;//�²�ԃ
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
        cout<<"ݔ���ԃ����(x <= 4)��"<<endl;//���Ȳ�ԃ
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
    }//�Y��switch
    return true;
}

//���� �� �yӋ����
//�yӋ��ԃ1
void statisticsDisplay(map<string,int> result,string city_Name)//�yӋݔ��
{
    cout<<endl;
    cout<<"      "<<city_Name<<"��"<<endl;
    for (map<string,int>::iterator mapIter=result.begin(); mapIter!=result.end(); ++mapIter)
    {
        cout<<setw(10)<<(*mapIter).first<<setw(2)<<"��"<<setw(12)<<(*mapIter).second<<endl;
    }
}

bool statisticQuery(vector<City> table,const string city_Name,QUERY_TYPE type=FOR_WEEK)
{
    vector<City> oneCityTable;
    oneCityTable.clear();//���
    for (vector<City>::iterator iter=table.begin(); iter!=table.end(); ++iter)
    {
        if ((*iter).city_Name == city_Name)
        {
            oneCityTable.push_back(*iter);
        }
    }

    map<string,int> queryResult;
    queryResult.clear();
    queryResult["��"]=0;
    queryResult["��"]=0;
    queryResult["�p΢��Ⱦ"]=0;
    queryResult["�p����Ⱦ"]=0;
    queryResult["�ж���Ⱦ"]=0;
    queryResult["�ж�����Ⱦ"]=0;
    queryResult["����Ⱦ"]=0;
    switch (type)
    {
    case FOR_WEEK:
    {
        int week;
        cout<<"ݔ���ܾ�̖(<= 53)��"<<endl;
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
        cout<<"ݔ���¾�̖��"<<endl;
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
        cout<<"ݔ��ڎ׼��ȣ�"<<endl;
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
        cout<<"ݔ��ڎ��꣺"<<endl;
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

//�yӋ��ԃ2
void statisticsDisplay2(map<string,int> result,string PolluLevel)//�yӋ�ĵڶ��N��r�ĽyӋ�@ʾ����
{
    cout<<endl;
    cout<<"        Ʒ�|��"<<PolluLevel<<endl;
    for (map<string,int>::iterator mapIter=result.begin(); mapIter!=result.end(); ++mapIter)
    {
        cout<<setw(8)<<(*mapIter).first<<setw(2)<<"��"<<setw(12)<<(*mapIter).second<<endl;
    }
}

bool statisticQuery2(vector<City> table,string PolluLevel,QUERY_TYPE type=FOR_DAY,int T=0)//������Ⱦ��͵ĽyӋ����
{

    vector<City> onePolluteTable;
    onePolluteTable.clear();//���


    for (vector<City>::iterator iter=table.begin(); iter!=table.end(); ++iter)
    {
        if ((*iter).polluteLevel == PolluLevel)
        {
            onePolluteTable.push_back(*iter);//�@ȡһ�N��Ⱦ��͵ĳ���
        }
    }

    map<string,int> queryResult;
    queryResult.clear();
    for(set<string>::iterator ITER=s_city.begin(); ITER!=s_city.end(); ++ITER)
    {
        queryResult.insert(pair<string,int>((*ITER),0));

    }
    map<string,int> queryPlay;//��Ⱦ�씵���ĳһ��ֵ �ĕr�� Ҫshowplay�ĺ���
    queryPlay.clear();
    switch (type)
    {
    case FOR_WEEK:
    {
        int week;
        cout<<"ݔ���ܾ�̖(<= 53)��"<<endl;
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
        cout<<"ݔ���¾�̖��"<<endl;
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
        cout<<"ݔ��ڎ׼��ȣ�"<<endl;
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
        cout<<"ݔ��ڎ��꣺"<<endl;
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

//������Ҫ���������ԃ ����
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
    sort(tVector.begin(), tVector.end(), cmp);//�Լ����������㷨
}

void HeapAdjust(vector<pair<string ,int> > &Heap, int s, int m)
{

    for (int j = s * 2 + 1; j < m; j = j * 2 + 1)
    {
        if (j < m - 1 && Heap[j + 1].second> Heap[j].second)
            j++;
        if ( Heap[s].second > Heap[j].second)//���ɴ����
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

    sort(minK.begin(),minK.end(),cmp);//��minK������������K����Сֵ�ǁy���


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
    cout<<"            ���а�����"<<endl;
    for (size_t i=0; i<result.size(); i++)
    {
        cout<<"��"<<i+1<<"����"<<setw(16)<<result[i].first<<setw(12)<<result[i].second<<endl;
    }
    cout<<"***************************************"<<endl;
}

bool sortQuery(vector<City> table,int rankCount,QUERY_TYPE type=FOR_WEEK)//���򺯔�
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
        cout<<"ݔ���ܔ�(<=53)��";
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
        cout<<"ݔ���·ݣ�";
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
        cout<<"ݔ�뼾�ȣ�";
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
        cout<<"ݔ����ݣ�";
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
    cout<<"�x�딵��������������"<<endl;
    fileRead();//�x���ļ�
    cout<<"�x��ɹ���"<<endl;
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
                cout<<"ݔ��������Q��";
                cin>>city_Name;
                if (s_city.find(city_Name) ==s_city.end())//����ԓ����
                {
                    cout<<"�]��ԓ���У�Ո����ݔ�룡"<<endl;
                    break;
                }
                cout<<"Ո�x���ԃ��ͣ�"<<endl;
                cout<<"0����ԃһ��"<<endl;
                cout<<"1����ԃһ��"<<endl;
                cout<<"2����ԃһ��"<<endl;
                cout<<"3����ԃһ����"<<endl;
                cout<<"4����ԃһ��"<<endl;
                cin>>queryType;
                if (!normInquire(dataTable,city_Name,getQueryType(queryType)))
                {
                    cout<<"��ԃʧ����Ո����ݔ��"<<endl;
                }
                cout<<"�Ƿ��˳���Y/N��"<<endl;
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
                cout<<"ݔ���ԃ���ݣ�"<<endl;
                cout<<endl;
                cout<<"1���yӋһ��������Ⱦ��r"<<endl<<endl;
                cout<<"2��ݔ����Ⱦ��r��ԃ����"<<endl<<endl;
                cin>>flag;
                if (flag==1)
                {
                    cout<<"ݔ��������Q��";
                    cin>>city_Name;
                    if (s_city.find(city_Name) ==s_city.end())//����ԓ����
                    {
                        cout<<"�]��ԓ���У�Ո����ݔ�룡"<<endl;
                        break;
                    }
                    cout<<"Ո�x���ԃ��ͣ�"<<endl;
                    cout<<"1���yӋһ��"<<endl;
                    cout<<"2���yӋһ��"<<endl;
                    cout<<"3���yӋһ����"<<endl;
                    cout<<"4���yӋһ��"<<endl;
                    cin>>queryType;
                    if (!statisticQuery(dataTable,city_Name,getQueryType(queryType)))
                    {
                        cout<<"��ԃʧ����Ո����ݔ��"<<endl;
                    }
                }
                if (flag==2)
                {
                    cout<<"Ո�x���ԃ������"<<endl;
                    cout<<"1���ܲ�ԃ"<<endl;
                    cout<<"2���²�ԃ"<<endl;
                    cout<<"3�����Ȳ�ԃ"<<endl;
                    cout<<"4�����ԃ"<<endl;
                    cin>>queryType;
                    cout<<"Ո�x���ԃ�Ŀ՚�Ʒ�|��r��"<<endl;
                    cout<<"1����"<<endl;
                    cout<<"2����"<<endl;
                    cout<<"3���p΢��Ⱦ"<<endl;
                    cout<<"4���p����Ⱦ"<<endl;
                    cout<<"5���ж���Ⱦ"<<endl;
                    cout<<"6���ж�����Ⱦ"<<endl;
                    cout<<"7������Ⱦ"<<endl;
                    cin>>pollute_Type;
                    cout<<"Ո������Ⱦ��͵��씵��"<<endl<<endl;
                    cout<<"ע�⣺������ܲ�ԃ���씵 <= 7 "<<endl<<endl;
                    cout<<"      ������²�ԃ���씵 <= the day of month"<<endl<<endl;
                    cout<<"      ����Ǽ��Ȳ�ԃ���씵 <= the day of quarter"<<endl<<endl;
                    cout<<"      ��������ԃ���씵 <= 365 "<<endl<<endl;
                    cin>>time;
                    if (!statisticQuery2(dataTable,getPollute_Type(pollute_Type),getQueryType(queryType),time))
                    {
                        cout<<"��ԃʧ����Ո����ݔ��"<<endl;
                    }
                }
                cout<<"�Ƿ��˳���Y/N��"<<endl;
                cin>>ch;
                if(ch=='Y'||ch=='y') break;
            }
        }
        system("cls");
        break;
        case 3:
        {
            char ch;
            while (1)//�����ԃ
            {
                cout<<"ݔ�����а��L�ȣ�";
                cin>>rankLen;
                cout<<"Ո�x���ԃ��ͣ�"<<endl;
                cout<<"1��һ������"<<endl;
                cout<<"2��һ������"<<endl;
                cout<<"3��һ��������"<<endl;
                cout<<"4��һ������"<<endl;
                cin>>queryType;
                if (!sortQuery(dataTable,rankLen,getQueryType(queryType)))
                {
                    cout<<"��ԃʧ����Ո����ݔ��"<<endl;
                }
                cout<<"�Ƿ��˳���Y/N��"<<endl;
                cin>>ch;
                if(ch=='Y'||ch=='y') break;
            }

        }
        system("cls");//����
        break;
        case 4:
            cout<<"���x����ʹ�ã�"<<endl;
            exit(1);
            break;
        default:
            cout<<"ݔ���e�`��Ո����ݔ�룡"<<endl;
            system("cls");
            break;
        }
    }
    return 0;
}
