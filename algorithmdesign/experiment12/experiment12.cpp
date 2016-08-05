#include<stdio.h>
#include<cstring>
using namespace std;

struct beibao
{
    double w,p;
    int id;
};
struct beibao bao[10000];
double c,cw,cp,bestp;
int n;
int a[10000],b[10000];

double bound(int i)
{
    //Ӌ���Ͻ�
    double cleft=c-cw;//ʣ�N����
    double bound=cp;
    //����Ʒ��λ�����rֵ�f�p����b����Ʒ
    while(i<n&&bao[i].w<=cleft)
    {
        cleft-=bao[i].w;
        bound+=bao[i].p;
        i++;
    }
    //�b�M����
    if(i<n)
    {
        bound+=bao[i].p*cleft/bao[i].w;
    }
    return bound;
}

void backtrack(int i)
{
    if(i==n) //���_�~�Y�c
    {
        if(cp>bestp)
        {
            bestp=cp;
            for(int k=0; k<n; k++)
            {
                b[k]=a[k];
            }
            memset(a,0,sizeof(a));
        }
        return;
    }
    //�����Ә�
    if(cw+bao[i].w<=c)
    {
        //�M�����Ә�
        cw+=bao[i].w;
        cp+=bao[i].p;
        a[i]=1;
        backtrack(i+1);
        a[i]=0;
        cw-=bao[i].w;
        cp-=bao[i].p;
    }
    if(bound(i+1)>bestp)
        backtrack(i+1);//�M�����Ә�
}

int main()
{
    int i,j,te;
    double temp;
    printf("Please input the number of the goods:");
    scanf("%d",&n);
    printf("Please input the whole volumn of the bag:");
    scanf("%lf",&c);
    printf("Please input the volumn and value of the goods in turn by one line:\n");
    for(i=0; i<n; i++)
    {
        scanf("%lf%lf",&bao[i].w,&bao[i].p);
        bao[i].id=i+1;
    }
    for(i=0; i<n-1; i++)
    {
        for(j=i+1; j<n; j++)
        {
            if(bao[i].p/bao[i].w<bao[j].p/bao[j].w)
            {
                temp=bao[i].w;
                bao[i].w=bao[j].w;
                bao[j].w=temp;
                temp=bao[i].p;
                bao[i].p=bao[j].p;
                bao[j].p=temp;
                te=bao[i].id;
                bao[i].id=bao[j].id;
                bao[j].id=te;
            }
        }
    }
    backtrack(0);
    printf("The order of the goods is:\n");
    for(i=0; i<n; i++)
        if(b[i])printf("%d ",bao[i].id);
    printf("\n");
    printf("The largest value of the bag is:%.0f\n",bestp);
    return 0;
}

