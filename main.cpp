#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <queue>
#define N 20000000
#define H 4000010
#define mod 4000007
using namespace std;
int dx[]= {0,1,0,-1};
int dy[]= {1,0,-1,0};

int da[4][3]=
{
    {0,1,1},
    {1,1,0},
    {0,-1,-1},
    {-1,-1,0}
};
int db[4][3]=
{
    {1,1,0},
    {0,-1,-1},
    {-1,-1,0},
    {0,1,1}
};
int da1[4][3]=
{
    {0,-1,-1},
    {1,1,0},
    {0,1,1},
    {-1,-1,0}
};
int db1[4][3]=
{
    {1,1,0},
    {0,1,1},
    {-1,-1,0},
    {0,-1,-1}
};
int border[9][9];
int bn[20];
/*
0 空白     *
1 砖       #
2 屎       O
3 蜗牛     @
4 箱子     D
5 屎加箱子 B
6 屎加蜗牛 Q
*/
int cg(char c)
{
    switch(c)
    {
    case '*':
        return 0;
    case '#':
        return 1;
    case 'O':
        return 2;
    case '@':
        return 3;
    case 'D':
        return 4;
    case 'B':
        return 5;
    case 'Q':
        return 6;
    }
}
struct board
{
    int b[5],s;
    board() {}
    bool operator==(board t)
    {
        for(int i=0; i<=4; i++)
            if(t.b[i]!=b[i])return false;
        return true;
    }
    bool encode(int bd[7][7])
    {
        int num,x,y;
        bool flag=true;
        memset(b,0,sizeof(b));
        for(int i=0; i<=6; i++)
            for(int j=0; j<=6; j++)
            {
                if(bd[i][j]==4)flag=false;
                num=i*7+j;
                x=num/10;
                y=num%10;
                b[x]|=(bd[i][j]<<(y*3));
                if(bd[i][j]==3 || bd[i][j]==6)
                {
                    s=i*7+j;
                }
            }
        return flag;
    }

    bool decode(int bd[7][7])
    {
        int x,y;
        bool flag=true;
        for(int i=0; i<=4; i++)
        {
            for(int j=0; j<=9; j++)
            {
                x=(i*10+j)/7;
                y=(i*10+j)%7;
                bd[x][y]=(b[i]>>(j*3))%8;
                if(bd[x][y]==4)flag=false;
            }
        }
        return flag;
    }
};
int tim,sum;
struct hashmap
{
    board d[N];
    int head[H],next[N],cnt;

    void init()
    {
        memset(head,-1,sizeof(head));
        cnt=0;
    }
    int hashcode(board t)
    {
        int h=0;
        for(int i=0; i<=4; i++)
        {
            h=(h>>2)^(h<<2)^t.b[i];
        }
        h%=mod;
        if(h<0)h+=mod;
        return h;
    }
    int find(board t)
    {
        int h=hashcode(t);

        for(int i=head[h]; i!=-1; i=next[i])
        {
            if(d[i]==t)return i;
        }
        return -1;
    }
    int push(board t)
    {
        int h=hashcode(t);
        //int num=0;
        tim++;
        for(int i=head[h]; i!=-1; i=next[i])
        {
            sum++;
            if(d[i]==t)return -1;
            //  num++;
        }
        // cout<<num<<endl;
        sum++;
        d[cnt]=t;
        next[cnt]=head[h];
        head[h]=cnt++;
        return cnt-1;
    }
} hm;
int que[N],pr[N];

bool check(int x,int y,int d,int bd[7][7])
{
    if(x>=7 || x<=-1 || y>=7 || y<=-1)return false;
    int tx,ty,a,b;
    if(bd[x][y]==4 || bd[x][y]==5)//如果是个箱子
    {
        tx=x+dx[d];
        ty=y+dy[d];
        if(tx>=7 || tx<=-1 || ty>=7 || ty<=-1)return false;
        if(bd[tx][ty]!=0 && bd[tx][ty]!=2)return false;//再下一个位置一定得是空的

        if(border[tx+1][ty+1]==1)return false;

        int B=bd[tx][ty]==2,A=1;//四个箱子不能在一起，除非有四个屎
        for(int i=0; i<3; i++)
        {
            a=tx+da[d][i];
            b=ty+db[d][i];
            if(a>=7 || a<=-1 || b>=7 || b<=-1)A++;
            else if(bd[a][b]==4 || bd[a][b]==1)A++;
            else if(bd[a][b]==5)A++,B++;
        }
        if(A==4 && B!=4)return false;

        B=bd[tx][ty]==2;
        A=1;//四个箱子不能在一起，除非有四个屎
        for(int i=0; i<3; i++)
        {
            a=tx+da1[d][i];
            b=ty+db1[d][i];
            if(a>=7 || a<=-1 || b>=7 || b<=-1)A++;
            else if(bd[a][b]==4 || bd[a][b]==1)A++;
            else if(bd[a][b]==5)A++,B++;
        }
        if(A==4 && B!=4)return false;
    }
    if(bd[x][y]==1)return false;
    return true;
}
void change(int bd[7][7],int b[7][7],int sx,int sy,int x,int y,int d)
{
    memcpy(b,bd,sizeof(int)*49);
    int tx,ty;
    if(bd[x][y]==4 || bd[x][y]==5)//如果是个箱子
    {
        tx=x+dx[d];
        ty=y+dy[d];
        if(bd[tx][ty]==0)
            b[tx][ty]=4;
        else if(bd[tx][ty]==2)
            b[tx][ty]=5;
    }

    if(bd[x][y]==4)
        b[x][y]=3;
    else if(bd[x][y]==5)
        b[x][y]=6;
    else if(bd[x][y]==0)
        b[x][y]=3;
    else if(bd[x][y]==2)
        b[x][y]=6;
    if(bd[sx][sy]==3)
        b[sx][sy]=0;
    else if(bd[sx][sy]==6)
        b[sx][sy]=2;
}
int search(board t)
{
    queue<int>q;
    hm.init();
    int front=0,rear=0,tmp,bd[7][7],b[7][7],x,y,tx,ty;
    board tt;
    que[rear++]=hm.push(t);
    pr[front]=-1;
    while(front<rear)
    {
        tmp=que[front++];
        t=hm.d[tmp];
        if(t.decode(bd))return front-1;

        x=t.s/7;
        y=t.s%7;
        //cout<<x<<" "<<y<<endl;
        for(int i=0; i<=3; i++)
        {
            tx=x+dx[i];
            ty=y+dy[i];
            if(check(tx,ty,i,bd))
            {
                //if(tmp==2)cout<<'a'<<i<<endl;
                change(bd,b,x,y,tx,ty,i);
                if(tt.encode(b))
                {
                    hm.push(tt);
                    pr[rear]=front-1;
                    que[rear++]=hm.cnt-1;

                    return rear-1;
                }
                //if(tmp==0)cout<<'a'<<tt.s/7<<" "<<tt.s%7<<endl;
//
//                for(int i=0;i<=6;i++,cout<<endl)
//                    for(int j=0;j<=6;j++)
//                        cout<<b[i][j];

                if(hm.push(tt)!=-1)
                {
                    pr[rear]=front-1;
                    que[rear++]=hm.cnt-1;

                    if(rear==N)cout<<"ddd"<<endl;
                    if(rear%1000000==0)cout<<rear<<endl;
                }
            }
        }
    }
    return -1;
}

void output(int f)
{
    if(pr[f]==-1)return ;
    output(pr[f]);
    int a,b,c,d;
    a=hm.d[f].s/7;
    b=hm.d[f].s%7;
    c=hm.d[pr[f]].s/7;
    d=hm.d[pr[f]].s%7;
    if(a==c)
    {
        if(b>d)cout<<"R";
        else cout<<"L";
    }
    else
    {
        if(a>c)cout<<"D";
        else cout<<"U";
    }
}

void initialize(char bd[9][9])
{
    memset(border,0,sizeof(border));
    int x,y;
    for(int i=1; i<=7; i++)
        for(int j=1; j<=7; j++)
            if(bd[i][j]=='*' || bd[i][j]=='@')
            {
                for(int k=0; k<=3; k++)
                {
                    x=i+dx[k];
                    y=j+dy[k];
                    if(bd[x][y]=='#')
                    {
                        x=i+dx[(k+1)%4];
                        y=j+dy[(k+1)%4];
                        if(bd[x][y]=='#')border[i][j]=1;
                    }
                }
            }
//    for(int i=0; i<=8; i++,cout<<endl)
//        for(int j=0; j<=8; j++)
//            cout<<border[i][j]<<" ";
}
int main()
{
    freopen("in.txt","r",stdin);

    int num;
    scanf("%d",&num);
    while(num--)
    {
        char str[9][9];
        int bd[7][7];
        for(int i=0; i<=8; i++)
        {
            scanf("%s",str[i]);
        }
        for(int i=1; i<=7; i++)
            for(int j=1; j<=7; j++)
            {
                bd[i-1][j-1]=cg(str[i][j]);
            }
        initialize(str);
        board t;
        t.encode(bd);

        tim=sum=0;
        int tmp=search(t);
        //cout<<tmp<<endl;
        output(tmp);
        cout<<endl<<endl;
        cout<<sum*1.0/tim;
    }

    return 0;
}
