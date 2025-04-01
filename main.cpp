#include<iostream>
using namespace std;
#define maprow 3
#define mapline 4
#include <limits>
#include <cstring>
#include <iomanip>
struct Node{
    bool isunique;
    double l,r,u,d;
    double maxNode=0;
}map[maprow+1][mapline+1];;

void initMap(){
    for(int i=1;i<=maprow;i++){
        for(int j=1;j<=mapline;j++){
           map[i][j]={0,0,0,0,0};
        }
    }
    map[1][4]={1,0,0,0,0,1};
    map[2][4]={1,0,0,0,0,-1};
    map[2][2]={1,0,0,0,0,0};
}

void PrintMap(){
    cout<<"V:"<<endl;

    // 打印 V 部分（maxNode 矩阵）
    cout << "\n=== V (Max Node Matrix) ===" << endl;
    cout << "+";
    for (int j = 1; j <= mapline; j++) {
        cout << "--------+";
    }
    cout << endl;

    for (int i = 1; i <= maprow; i++) {
        cout << "|";
        for (int j = 1; j <= mapline; j++) {
            cout << " " << setw(6) << fixed << setprecision(4) << map[i][j].maxNode << " |";
        }
        cout << endl;
        cout << "+";
        for (int j = 1; j <= mapline; j++) {
            cout << "--------+";
        }
        cout << endl;
    }

    cout<<"Q:"<<endl;
    for(int i=1;i<=maprow;i++){
        for(int j=1;j<=mapline;j++){
            if(map[i][j].isunique) continue;
            cout<<"Point["<<i<<","<<j<<"]:"<<endl;
            cout<<"左:"<<map[i][j].l<<" ";
            cout<<"右:"<<map[i][j].r<<" ";
            cout<<"上:"<<map[i][j].u<<" ";
            cout<<"下:"<<map[i][j].d<<" ";
            cout<<endl;
        }
    }
}
bool NotOutMap(int x,int y){
    return (x>0&&x<=maprow)&&(y>0&&y<=mapline);
}
double UpdateNode(int x,int y,int i,int j){
    if(NotOutMap(x,y)) return map[x][y].maxNode;
    else return map[i][j].maxNode;
}
int dx[4]={0,0,1,-1},dy[4]={1,-1,0,0};
void doMDP(double gamma,double RightDo,double FalseDo){
    Node tmpmap[maprow+1][mapline+1];
    memcpy(tmpmap,map,sizeof(map));//注意保存之前的状态，更新会改变！

    for(int i=1;i<=maprow;i++){
        for(int j=1;j<=mapline;j++){
            if(map[i][j].isunique) continue;
            for(int op=0;op<4;op++){
                double value;
                int x=i+dx[op],y=j+dy[op];
                double truevalue,falsevalue1,falsevalue2;
                truevalue= RightDo*UpdateNode(x,y,i,j);
                //错误情况

                int dil=-1,dir=1;
                if(dx[op]==0){
                    falsevalue1= FalseDo*UpdateNode(i+dil,j,i,j);
                    falsevalue2= FalseDo*UpdateNode(i+dir,j,i,j);
                }else{
                    falsevalue1= FalseDo*UpdateNode(i,j+dil,i,j);
                    falsevalue2= FalseDo*UpdateNode(i,j+dir,i,j);
                }
                value=(truevalue+falsevalue1+falsevalue2)*gamma;
                Node&node=tmpmap[i][j];
                switch (op) {
                    case 0:node.u=value;break;
                    case 1:node.d=value;break;
                    case 2:node.l=value;break;
                    case 3:node.r=value;break;
                }
                tmpmap[i][j].maxNode=max(max(node.u,node.d),max(node.l,node.r));
            }
        }
    }
    memcpy(map,tmpmap,sizeof(map));
}
int main(){
    initMap();
    PrintMap();
    //gamma
    double gamma=1,RightDo=0.8,FalseDo=0.1;//在给定方向正确走的前提，没有正确走即使走两侧
    int T=5;//迭代次数
    for(int i=1;i<=T;i++){
        cout<<"--------------------第"<<i<<"次迭代--------------------------"<<endl;
        doMDP(gamma,RightDo,FalseDo);
        PrintMap();
    }
    return 0;
}