#include <iostream>
using namespace std;

int n;
int s[100][100];//ӛ�ÿһ�N��r�µĸ��Y�c
double a[100],b[100],m[100][100],w[100][100];//ӛ�������ֵ,a����~�Y�c����,b���~�Y�c����
double p[100];//��ȡ���򼯵��Y��Ԫ��

void BinaryTree()//������ь���ĄӑBҎ�����㷨
{
    int i,j,r;
    for(i = 0; i <= n; i++)
    {
        w[i+1][i] = a[i];
        m[i+1][i] = 0;
    }
    for(r = 0; r < n; r++)
    {
        for(i = 1; i <= n - r; i++)
        {
            j = i + r;
            w[i][j] = w[i][j-1] + a[j] + b[j];
            m[i][j] = m[i+1][j];
            s[i][j] = i;

            for(int k = i + 1; k <= j; k++)//����i��j�ĸ�k,���� min{m[i][k-1] + m[k+1][j]}
            {
                double t = m[i][k-1] + m[k+1][j];//�����c��k
                if(t < m[i][j])
                {
                    m[i][j] = t;
                    s[i][j] = k;
                }
            }

            m[i][j] += w[i][j];
        }
    }
}

void Show(int left, int right)//ݔ��������ь�������P�YӍ,�fޒ����ݔ����
{
    int k = s[left][right];
    cout<<p[k]<<endl;
    if(left <= k-1 && k - 1 < right)
    {
        cout<<"with "<<k<<" as the root node of the left subtree:";
        Show(left,k-1);
    }
    if(k+1 <= right && k+1 > left)
    {
        cout<<"with "<<k<<" as the root node of the right subtree:";
        Show(k+1,right);
    }
}

int main()
{
    int i;
    cout<<"Please input the number of elements:";
    cin>>n;
    cout<<"Please id of elements:"<<endl;
    for(i = 1; i <= n; i++) cin>>p[i];
    cout<<"The probability of leaf nodes"<<endl;
    for(i = 1; i <= n; i++) cin>>b[i];//ݔ���~���c
    cout<<"The probability of non-leaf nodes"<<endl;
    for(i = 0; i <= n; i++) cin>>a[i];//ݔ����~���c
    BinaryTree();
    cout<<endl<<"The average length of the optimal binary search tree is:"<<m[1][n]<<endl<<endl;
    cout<<"the first one is the root node, followed by its left subtree nodes and right subtree nodes"<<endl<<endl;
    Show(1,n);
    cout<<endl<<endl;
    return 0;
}
