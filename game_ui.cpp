#include "game_ui.h"
#include "ui_game_ui.h"
#include"chooselevel_ui.h"
#include<iostream>
#include<QPixmap>
#include<QGraphicsScene>
#include<QList>
using namespace std;

Game_UI::Game_UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Game_UI)
{
    ui->setupUi(this);
}

Game_UI::~Game_UI()
{
    delete ui;
}

void Game_UI::setLevel(int level){
    this->level=level;
}

int Game_UI::getLevel(){
    return this->level;
}

void Game_UI::on_returnButton_clicked()
{
    ChooseLevel_UI *chooseLevel_ui=new ChooseLevel_UI;
    chooseLevel_ui->show();

    //此处可能呀清空游戏数据

    delete this;
}

void Game_UI::on_beginButton_clicked()
{
    gameMap=map.creatMap(rowSize,columnSize,level,numOfPic);
    //////////////////////////////
    for(int i=1;i<rowSize-1;i++){
        for(int j=1;j<columnSize-1;j++){
            gameButtonMap[i][j]->setText(QString::number(gameMap[i][j]));
            gameButtonMap[i][j]->show();
        }

        cout<<endl;
    }
    ///////////////////////////////
}

bool Game_UI::allCleared()
{
    for(int i=1;i<rowSize-1;i++){
        for(int j=1;j<columnSize-1;j++){
            if(gameMap[i][j] != 0)
                return false;
        }
        }
    return true;
}

void Game_UI::createGameMap(){
    MyButton *button=new MyButton;
    gameButtonMap=(MyButton***) malloc(rowSize*sizeof(MyButton**));
    for(int i=0;i<rowSize;i++){
        gameButtonMap[i]=(MyButton**)malloc(columnSize*sizeof(MyButton*));
        for(int j=0;j<columnSize;j++){
            gameButtonMap[i][j]=button;
        }
    }

    gameMap=map.creatMap(rowSize,columnSize,level,numOfPic);

    for(int i=1;i<rowSize-1;i++){
        for(int j=1;j<columnSize-1;j++){
            /////////////////
            cout<<gameMap[i][j]<<"\t";
            ///////////////////////
            MyButton *myButton=new MyButton;
            myButton->setParent(this);
            myButton->setCoordinate(i,j);
            myButton->setGeometry(start_x+j*edgeOfButton,start_y+i*edgeOfButton,edgeOfButton,edgeOfButton);
            myButton->show();
            //连接信号槽，传递button的数组下标
            connect(myButton, SIGNAL(clicked(int,int)),this, SLOT(on_myButton_clicked(int,int)));
            myButton->setText(QString::number(gameMap[i][j]));
            gameButtonMap[i][j]=myButton;
        }
        /////////////////
        cout<<endl;
        //////////////////
    }

}

void Game_UI::on_myButton_clicked(int row,int column){
    /////////////////
    cout<<row<<" "<<column<<endl;
    //////////////////////////////
    voiceplayer=new VoicePlayer;
    //voiceplayer->Play_Voice(1);//播放按钮音效

    if(count == 1){
        if(gameMap[row][column] == gameMap[vertex1.first][vertex1.second])
        {
            QList<Vertex> list;
            if(map.canLink_2(gameMap,row,column,vertex1.first,vertex1.second,list)!=-1)//判断能否连接消除
            {
                voiceplayer->Play_Voice(2);//播放消除音效
                gameMap[row][column]=0;
                gameMap[vertex1.first][vertex1.second]=0;
                gameButtonMap[row][column]->hide();
                gameButtonMap[vertex1.first][vertex1.second]->hide();
                count--;

//                Vertex v1,v2;
//                v2=list.takeFirst();
//                v1=list.takeLast();
                cout<<"length"<<list.length()<<endl;
//                for(int i=1;i<=list.length();i++)
//                {
//                    cout<<i
//                    //cout<<i<<"hhhhhhhhhh"<<list.takeAt(i).first<<" "<<list.takeAt(i).second<<endl;
//                }
                cout<<"hhhhhhhhhh1:"<<list.takeLast().first<<" "<<list.takeLast().second<<endl;
                cout<<"hhhhhhhhhh2:"<<list.takeFirst().first<<" "<<list.takeFirst().second<<endl;
               // cout<<"hhhhhhhhhhhh"<<v1.first<<" "<<v1.second<<" "<<v2.first<<" "<<v2.second<<endl;

                //判断是否全部消除(游戏通关)
                if(allCleared())
                {
                    cout<<"全部消除！！！"<<endl;
                }
            }
            else
            {
                voiceplayer->Play_Voice(1);//播放按钮音效
                vertex1.first = row;
                vertex1.second = column;
            }
        }
        else
        {
            voiceplayer->Play_Voice(1);//播放按钮音效
            vertex1.first = row;
            vertex1.second = column;
        }
    }
    else if(count ==0){
        voiceplayer->Play_Voice(1);//播放按钮音效
        vertex1.first = row;
        vertex1.second = column;
        count++;
    }


}
