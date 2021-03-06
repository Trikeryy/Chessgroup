#include "gamestatus.h"
#include "ui_gamestatus.h"
#include "walls.h"
#include "multiplayerlogin.h"
#include "loginuser.h"
#include <QPushButton>
#include "validatemoves.h"
#include <QtGui>
#include <QTime>
#include <QTimer>
#include <QWidget>
#include <QtCore>
#include <QTableWidget>
#include <iostream>
#include <unistd.h>
//#include "Connector.hpp"
#include <QThread>
#include <string>
#include <QtSql>

validatemoves *valid = new validatemoves;
extern QString player1username, player2username;
void validate(walls *temp, int c, QWidget *baseWidget);
void disOrange();
using std::string;

//void secs(QWidget *baseWidget);

int count=0, turn=1, expw[60], max=0;


int wR,wC,bR,bC;
walls *click1;
walls *temp;
walls *tile[8][8] = { { NULL } };

void walls::mousePressEvent(QMouseEvent *event)
{
    QWidget *baseWidget=nullptr;
    validate(this,++count,baseWidget);
}


GameStatus::GameStatus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameStatus)
{
    ui->setupUi(this);
    ui->welcomemsg->setText("               WELCOME    ");

}

GameStatus::~GameStatus()
{
    delete ui;
}


class Border
{
public:
    Border();
    void outline(QWidget *baseWidget, int xPos, int yPos, int Pos)
    {
         QLabel *outLabel = new QLabel(baseWidget);

        if(!Pos)
            outLabel->setGeometry(xPos,yPos,552,20);        //Horizontal Borders

        else
            outLabel->setGeometry(xPos,yPos,20,512);        //Vertical Borders

        outLabel->setStyleSheet("QLabel { background-color :rgb(170, 170, 127); color : black; }");
    }

};

void buttongoback(QWidget *baseWidget)
{
    QPushButton *go = new QPushButton(baseWidget);
    go->setGeometry(0,670,100,30);
    go->setText("<- EXIT GAME");
    go->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(164, 164, 164, 255), stop:1 rgba(255, 255, 255, 255));");
    if (go->isCheckable())
    {
        qDebug() << "exit checked";
    }

}
void savebutton(QWidget *baseWidget)
{
    QPushButton *save = new QPushButton(baseWidget);
    save->setGeometry(850,670,100,30);
    save->setText("SAVE GAME");
    save->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(164, 164, 164, 255), stop:1 rgba(255, 255, 255, 255));");

}
void HowtoPlayInfo(QWidget *baseWidget)
{
    QPushButton *infoguide = new QPushButton(baseWidget);
    infoguide->setGeometry(10,30,80,40);
    infoguide->setText("How To Play");
    infoguide->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(164, 164, 164, 255), stop:1 rgba(255, 255, 255, 255));");

}

void gogoback()
{
    GameStatus *gaming;
    gaming = new GameStatus;
    gaming->show();
}


void chessBoard(QWidget *baseWidget, walls *tile[8][8])
{
    int i,j,k=0,hor,ver;
    Border *border[4]={ NULL };


    //borderDisplay
    {
    border[0]->outline(baseWidget,330,105,0);
    border[1]->outline(baseWidget,330,637,0);
    border[2]->outline(baseWidget,330,125,1);
    border[2]->outline(baseWidget,862,125,1);
    }

    //Create 64 tiles (allocating memories to the objects of Tile class)
    ver=125;
    for(i=0;i<8;i++)
    {
        hor=350;
        for(j=0;j<8;j++)
        {
            tile[i][j] = new walls(baseWidget);
            tile[i][j]->tileColor=(i+j)%2;
            tile[i][j]->piece=0;
            tile[i][j]->row=i;
            tile[i][j]->col=j;
            tile[i][j]->tileNum=k++;
            tile[i][j]->wallDisplay();
            tile[i][j]->setGeometry(hor,ver,64,64);
            hor+=64;


        }
        ver+=64;

    }



    //white pawns piece colour 0
    for(j=0;j<8;j++)
    {
        tile[1][j]->piece=1;
        tile[1][j]->pieceColor=0;
        tile[1][j]->display('P');
    }

    //black pawns piece colour 1
    for(j=0;j<8;j++)
    {
        tile[6][j]->piece=1;
        tile[6][j]->pieceColor=1;
        tile[6][j]->display('P');
    }

    //white and black remaining elements
    for(j=0;j<8;j++)
    {
        tile[0][j]->piece=1;
        tile[0][j]->pieceColor=0;
        tile[7][j]->piece=1;
        tile[7][j]->pieceColor=1;
    }

    {
    tile[0][0]->display('R');
    tile[0][1]->display('H');
    tile[0][2]->display('B');
    tile[0][3]->display('Q');
    tile[0][4]->display('K');
    tile[0][5]->display('B');
    tile[0][6]->display('H');
    tile[0][7]->display('R');

    }


    {
    tile[7][0]->display('R');
    tile[7][1]->display('H');
    tile[7][2]->display('B');
    tile[7][3]->display('Q');
    tile[7][4]->display('K');
    tile[7][5]->display('B');
    tile[7][6]->display('H');
    tile[7][7]->display('R');

    }
//    int WTaken =16;
//    int BTaken = 16;
//    if((click1->pieceColor==1) == (temp->pieceColor==0))
//    {
//        BTaken=BTaken-1;
//    }
//    qDebug() << "BLlack piece taken " <<BTaken;
//    if (tile[r][c]->pieceColor==click1->pieceColor==1)
//    {
//        WTaken=WTaken-1;
//    }
//    qDebug() <<"White piece taken" <<WTaken;




    wR=7;
    wC=4;

    bR=0;
    bC=4;

}


QSqlDatabase pdatabase;
   void connClose()
   {
       pdatabase.close();
       pdatabase.removeDatabase(QSqlDatabase::defaultConnection);
   }

   bool connOpen()
   {
       pdatabase=QSqlDatabase::addDatabase("QSQLITE");
       pdatabase.setDatabaseName("/Users/Diana/Documents/database/chessproject.db");

       if(!pdatabase.open()){
           qDebug()<<("Failed to open the database");
           return false;
       }
       else{
           qDebug()<<("Connected2...");
           return true;
       }
   }

void validate(walls *temp,int c,QWidget *baseWidget)
{

    int retValue,i,rowcount1;
    QString turnLabel,piecename;
    QString position,newposition;
    enum Columna
    {
        Turnlabel,Position,PieceName,NewPosition
    };


    qDebug() << "touch wall";





    disOrange();
    QTableView *tableview=new QTableView(baseWidget);

    QStandardItemModel *model = new QStandardItemModel(0,4,baseWidget); //increment Rows and 4 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("P.Colour")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("OldMove")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("P.Name")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("NewMove")));
    rowcount1=model->rowCount(QModelIndex());




    model->insertRow(rowcount1);
    tableview->setModel(model);
    tableview->setGeometry(950,170,400,520);

    if(c==1)
    {

        //each time it's users turn to move a piece.
        if(temp->piece && (temp->pieceColor==turn))
        {



            if (temp->pieceColor==1 && turn==1)
            {
                turnLabel="W";
                qDebug() << turnLabel;
            }
            if (temp->pieceColor==0 && turn==0)
            {
                turnLabel="B";
                qDebug() << turnLabel;
            }

            qDebug() << "grab piece";
            //moving a piece and the colour
            expw[max++]=temp->tileNum;

//position

            if(temp->tileNum==0)
            {
                position="A8";
            }

            else if(temp->tileNum==1)
            {
                position="B8";
            }

            else if(temp->tileNum==2)
            {
                position="C8";
            }

            else if(temp->tileNum==3)
            {
                position="D8";
            }

            else if(temp->tileNum==4)
            {
                position="E8";
            }

            else if(temp->tileNum==5)
            {
                position="F8";
            }

            else if(temp->tileNum==6)
            {
                position="G8";
            }

            else if(temp->tileNum==7)
            {
                position="H8";
            }

            else if(temp->tileNum==8)
            {
                position="A7";
            }

            else if(temp->tileNum==9)
            {
                position="B7";
            }

            else if(temp->tileNum==10)
            {
                position="C7";
            }

            else if(temp->tileNum==11)
            {
                position="D7";
            }

            else if(temp->tileNum==12)
            {
                position="E7";
            }
            else if(temp->tileNum==13)
            {
                position="F7";
            }

            else if(temp->tileNum==14)
            {
                position="G7";
            }

            else if(temp->tileNum==15)
            {
                position="H7";
            }

            else if(temp->tileNum==16)
            {
                position="A6";
            }

            else if(temp->tileNum==17)
            {
                position="B6";
            }

            else if(temp->tileNum==18)
            {
                position="C6";
            }

            else if(temp->tileNum==19)
            {
                position="D6";
            }

            else if(temp->tileNum==20)
            {
                position="E6";
            }

            else if(temp->tileNum==21)
            {
                position="F6";
            }

            else if(temp->tileNum==22)
            {
                position="G6";
            }

            else if(temp->tileNum==23)
            {
                position="H6";
            }

            else if(temp->tileNum==24)
            {
                position="A5";
            }

            else if(temp->tileNum==25)
            {
                position="B5";
            }

            else if(temp->tileNum==26)
            {
                position="C5";
            }

            else if(temp->tileNum==27)
            {
                position="D5";
            }

            else if(temp->tileNum==28)
            {
                position="E5";
            }

            else if(temp->tileNum==29)
            {
                position="F5";
            }

            else if(temp->tileNum==30)
            {
                position="G5";
            }

            else if(temp->tileNum==31)
            {
                position="H5";
            }

            else if(temp->tileNum==32)
            {
                position="A4";
            }

            else if(temp->tileNum==33)
            {
                position="B4";
            }

            else if(temp->tileNum==34)
            {
                position="C4";
            }

            else if(temp->tileNum==35)
            {
                position="D4";
            }

            else if(temp->tileNum==36)
            {
                position="E4";
            }

            else if(temp->tileNum==37)
            {
                position="F4";
            }

            else if(temp->tileNum==38)
            {
                position="G4";
            }

            else if(temp->tileNum==39)
            {
                position="H4";
            }

            else if(temp->tileNum==40)
            {
                position="A3";
            }

            else if(temp->tileNum==41)
            {
                position="B3";
            }

            else if(temp->tileNum==42)
            {
                position="C3";
            }

            else if(temp->tileNum==43)
            {
                position="D3";
            }

            else if(temp->tileNum==44)
            {
                position="E3";
            }

            else if(temp->tileNum==45)
            {
                position="F3";
            }

            else if(temp->tileNum==46)
            {
                position="G3";
            }

            else if(temp->tileNum==47)
            {
                position="H3";
            }

            else if(temp->tileNum==48)
            {
                position="A2";
            }

            else if(temp->tileNum==49)
            {
                position="B2";
            }

            else if(temp->tileNum==50)
            {
                position="C2";
            }


            else if(temp->tileNum==51)
            {
                position="D2";
            }

            else if(temp->tileNum==52)
            {
                position="E2";
            }

            else if(temp->tileNum==53)
            {
                position="F2";
            }

            else if(temp->tileNum==54)
            {
                position="G2";
            }

            else if(temp->tileNum==55)
            {
                position="H2";
            }

            else if(temp->tileNum==56)
            {
                position="A1";
            }

            else if(temp->tileNum==57)
            {
                position="B1";
            }

            else if(temp->tileNum==58)
            {
                position="C1";
            }

            else if(temp->tileNum==59)
            {
                position="D1";
            }

            else if(temp->tileNum==60)
            {
                position="E1";
            }

            else if(temp->tileNum==61)
            {
                position="F1";
            }

            else if(temp->tileNum==62)
            {

                position="G1";
            }

            else if(temp->tileNum==63)
            {
                position="H1";
            }



            qDebug()<< position;
            model->setItem(rowcount1,Turnlabel, new QStandardItem(turnLabel));
            model->setItem(rowcount1,Position, new QStandardItem(position));
            if(model->rowCount() == 0)
            {
                model->setRowCount(1);
            }




            //check if the specific piece move is valid.
            retValue=valid->chooser(temp);

            //if the move is legal retValue=1.
            if(retValue)
            {
               click1= new walls();
               temp->setStyleSheet("QLabel {background-color: green;}");
               click1=temp;

            }
            //if move is illegal.
            else
             {
                temp->setStyleSheet("QLabel {background-color: red;}");
                count=0;
             }


    }
        else
        {

            //doesn't allow user to move their piece if it isn't their turn
            count=0;

        }
    }

    else
    {

        if(temp->tileNum==click1->tileNum)
        {

            click1->wallDisplay();

            max=0;
            count=0;


        }
        for(i=0;i<max;i++)
        {

            if(temp->tileNum==expw[i] && turn==1)
            {

                click1->piece=0;

                temp->piece=1;

                temp->pieceColor=click1->pieceColor;

                temp->pieceName=click1->pieceName;


//                if(tile[wR][wC]->pieceColor!=temp->pieceColor)
//                {
//                    qDebug() <<"YES CAPTURED AN EMENMY OF MINE";
//                }



                qDebug() <<temp->pieceName;

                piecename=temp->pieceName;


                click1->display(click1->pieceName);
                temp->display(click1->pieceName);


                if(temp->tileNum==0)
                {
                    newposition="A8";
                }

                else if(temp->tileNum==1)
                {
                    newposition="B8";
                }

                else if(temp->tileNum==2)
                {
                    newposition="C8";
                }

                else if(temp->tileNum==3)
                {
                    newposition="D8";
                }

                else if(temp->tileNum==4)
                {
                    newposition="E8";
                }

                else if(temp->tileNum==5)
                {
                    newposition="F8";
                }

                else if(temp->tileNum==6)
                {
                    newposition="G8";
                }

                else if(temp->tileNum==7)
                {
                    newposition="H8";
                }

                else if(temp->tileNum==8)
                {
                    newposition="A7";
                }

                else if(temp->tileNum==9)
                {
                    newposition="B7";
                }

                else if(temp->tileNum==10)
                {
                    newposition="C7";
                }

                else if(temp->tileNum==11)
                {
                    newposition="D7";
                }

                else if(temp->tileNum==12)
                {
                    newposition="E7";
                }
                else if(temp->tileNum==13)
                {
                    newposition="F7";
                }

                else if(temp->tileNum==14)
                {
                    newposition="G7";
                }

                else if(temp->tileNum==15)
                {
                    newposition="H7";
                }

                else if(temp->tileNum==16)
                {
                    newposition="A6";
                }

                else if(temp->tileNum==17)
                {
                    newposition="B6";
                }

                else if(temp->tileNum==18)
                {
                    newposition="C6";
                }

                else if(temp->tileNum==19)
                {
                    newposition="D6";
                }

                else if(temp->tileNum==20)
                {
                    newposition="E6";
                }

                else if(temp->tileNum==21)
                {
                    newposition="F6";
                }

                else if(temp->tileNum==22)
                {
                    newposition="G6";
                }

                else if(temp->tileNum==23)
                {
                    newposition="H6";
                }

                else if(temp->tileNum==24)
                {
                    newposition="A5";
                }

                else if(temp->tileNum==25)
                {
                    newposition="B5";
                }

                else if(temp->tileNum==26)
                {
                    newposition="C5";
                }

                else if(temp->tileNum==27)
                {
                    newposition="D5";
                }

                else if(temp->tileNum==28)
                {
                    newposition="E5";
                }

                else if(temp->tileNum==29)
                {
                    newposition="F5";
                }

                else if(temp->tileNum==30)
                {
                    newposition="G5";
                }

                else if(temp->tileNum==31)
                {
                    newposition="H5";
                }

                else if(temp->tileNum==32)
                {
                    newposition="A4";
                }

                else if(temp->tileNum==33)
                {
                    newposition="B4";
                }

                else if(temp->tileNum==34)
                {
                    newposition="C4";
                }

                else if(temp->tileNum==35)
                {
                    newposition="D4";
                }

                else if(temp->tileNum==36)
                {
                    newposition="E4";
                }

                else if(temp->tileNum==37)
                {
                    newposition="F4";
                }

                else if(temp->tileNum==38)
                {
                    newposition="G4";
                }

                else if(temp->tileNum==39)
                {
                    newposition="H4";
                }

                else if(temp->tileNum==40)
                {
                    newposition="A3";
                }

                else if(temp->tileNum==41)
                {
                    newposition="B3";
                }

                else if(temp->tileNum==42)
                {
                    newposition="C3";
                }

                else if(temp->tileNum==43)
                {
                    newposition="D3";
                }

                else if(temp->tileNum==44)
                {
                    newposition="E3";
                }

                else if(temp->tileNum==45)
                {
                    newposition="F3";
                }

                else if(temp->tileNum==46)
                {
                    newposition="G3";
                }

                else if(temp->tileNum==47)
                {
                    newposition="H3";
                }

                else if(temp->tileNum==48)
                {
                    newposition="A2";
                }

                else if(temp->tileNum==49)
                {
                    newposition="B2";
                }

                else if(temp->tileNum==50)
                {
                    newposition="C2";
                }


                else if(temp->tileNum==51)
                {
                    newposition="D2";
                }

                else if(temp->tileNum==52)
                {
                    newposition="E2";
                }

                else if(temp->tileNum==53)
                {
                    newposition="F2";
                }

                else if(temp->tileNum==54)
                {
                    newposition="G2";
                }

                else if(temp->tileNum==55)
                {
                    newposition="H2";
                }

                else if(temp->tileNum==56)
                {
                    newposition="A1";
                }

                else if(temp->tileNum==57)
                {
                    newposition="B1";
                }

                else if(temp->tileNum==58)
                {
                    newposition="C1";
                }

                else if(temp->tileNum==59)
                {
                    newposition="D1";
                }

                else if(temp->tileNum==60)
                {
                    newposition="E1";
                }

                else if(temp->tileNum==61)
                {
                    newposition="F1";
                }

                else if(temp->tileNum==62)
                {

                    newposition="G1";
                }

                else if(temp->tileNum==63)
                {
                    newposition="H1";
                }
                qDebug() << newposition;


//                connOpen();
//                QString n="lol";
//                QString l="lo";
//                QSqlQuery qry;
//                qry.prepare("select * from savegame where position ='"+n+"', pieceName ='"+l+"'");
//                qry.prepare("INSERT INTO savegame(position, pieceName) VALUES ('"+n+"','"+l+"')");
//                qry.exec();
//                connClose();

//                connOpen();
//                QSqlQuery qry;
//                qry.prepare("select * from savegame where  position='"+newposition+"'");
//                qry.prepare("INSERT INTO userprimarydata (position,) VALUES ('"+newposition+"')");
//                qry.exec();
//                connClose();



                model->setItem(rowcount1,PieceName, new QStandardItem(piecename));
                model->setItem(rowcount1,NewPosition, new QStandardItem(newposition));
                if(model->rowCount() == 0)
                {
                    model->setRowCount(1);
                }



                click1->wallDisplay();
                temp->wallDisplay();
                //checks if move is illegal
                retValue=valid->check(click1);
                 //if illegal turn the square red.
                if(retValue)
                {
                    tile[wR][wC]->setStyleSheet("QLabel {background-color: red;}");


                }


                disOrange();

                max=0;
                turn=(turn+1)%2;

                //black's turn
//                player1status1->hide();
//                player2status2->show();


                count=0;


            }

            else if(temp->tileNum==expw[i] && turn==0)
            {

                count=1;

                click1->piece=0;
                temp->piece=1;


                temp->pieceColor=click1->pieceColor;
                temp->pieceName=click1->pieceName;


                piecename=temp->pieceName;
                qDebug() <<piecename;

//                if(tile[wR][wC]->pieceColor!=temp->pieceColor)
//                {
//                    qDebug() <<"YES CAPTURED AN EMENMY OF MINE";
//                }



                if(temp->tileNum==0)
                {
                    newposition="A8";
                }

                else if(temp->tileNum==1)
                {
                    newposition="B8";
                }

                else if(temp->tileNum==2)
                {
                    newposition="C8";
                }

                else if(temp->tileNum==3)
                {
                    newposition="D8";
                }

                else if(temp->tileNum==4)
                {
                    newposition="E8";
                }

                else if(temp->tileNum==5)
                {
                    newposition="F8";
                }

                else if(temp->tileNum==6)
                {
                    newposition="G8";
                }

                else if(temp->tileNum==7)
                {
                    newposition="H8";
                }

                else if(temp->tileNum==8)
                {
                    newposition="A7";
                }

                else if(temp->tileNum==9)
                {
                    newposition="B7";
                }

                else if(temp->tileNum==10)
                {
                    newposition="C7";
                }

                else if(temp->tileNum==11)
                {
                    newposition="D7";
                }

                else if(temp->tileNum==12)
                {
                    newposition="E7";
                }
                else if(temp->tileNum==13)
                {
                    newposition="F7";
                }

                else if(temp->tileNum==14)
                {
                    newposition="G7";
                }

                else if(temp->tileNum==15)
                {
                    newposition="H7";
                }

                else if(temp->tileNum==16)
                {
                    newposition="A6";
                }

                else if(temp->tileNum==17)
                {
                    newposition="B6";
                }

                else if(temp->tileNum==18)
                {
                    newposition="C6";
                }

                else if(temp->tileNum==19)
                {
                    newposition="D6";
                }

                else if(temp->tileNum==20)
                {
                    newposition="E6";
                }

                else if(temp->tileNum==21)
                {
                    newposition="F6";
                }

                else if(temp->tileNum==22)
                {
                    newposition="G6";
                }

                else if(temp->tileNum==23)
                {
                    newposition="H6";
                }

                else if(temp->tileNum==24)
                {
                    newposition="A5";
                }

                else if(temp->tileNum==25)
                {
                    newposition="B5";
                }

                else if(temp->tileNum==26)
                {
                    newposition="C5";
                }

                else if(temp->tileNum==27)
                {
                    newposition="D5";
                }

                else if(temp->tileNum==28)
                {
                    newposition="E5";
                }

                else if(temp->tileNum==29)
                {
                    newposition="F5";
                }

                else if(temp->tileNum==30)
                {
                    newposition="G5";
                }

                else if(temp->tileNum==31)
                {
                    newposition="H5";
                }

                else if(temp->tileNum==32)
                {
                    newposition="A4";
                }

                else if(temp->tileNum==33)
                {
                    newposition="B4";
                }

                else if(temp->tileNum==34)
                {
                    newposition="C4";
                }

                else if(temp->tileNum==35)
                {
                    newposition="D4";
                }

                else if(temp->tileNum==36)
                {
                    newposition="E4";
                }

                else if(temp->tileNum==37)
                {
                    newposition="F4";
                }

                else if(temp->tileNum==38)
                {
                    newposition="G4";
                }

                else if(temp->tileNum==39)
                {
                    newposition="H4";
                }

                else if(temp->tileNum==40)
                {
                    newposition="A3";
                }

                else if(temp->tileNum==41)
                {
                    newposition="B3";
                }

                else if(temp->tileNum==42)
                {
                    newposition="C3";
                }

                else if(temp->tileNum==43)
                {
                    newposition="D3";
                }

                else if(temp->tileNum==44)
                {
                    newposition="E3";
                }

                else if(temp->tileNum==45)
                {
                    newposition="F3";
                }

                else if(temp->tileNum==46)
                {
                    newposition="G3";
                }

                else if(temp->tileNum==47)
                {
                    newposition="H3";
                }

                else if(temp->tileNum==48)
                {
                    newposition="A2";
                }

                else if(temp->tileNum==49)
                {
                    newposition="B2";
                }

                else if(temp->tileNum==50)
                {
                    newposition="C2";
                }


                else if(temp->tileNum==51)
                {
                    newposition="D2";
                }

                else if(temp->tileNum==52)
                {
                    newposition="E2";
                }

                else if(temp->tileNum==53)
                {
                    newposition="F2";
                }

                else if(temp->tileNum==54)
                {
                    newposition="G2";
                }

                else if(temp->tileNum==55)
                {
                    newposition="H2";
                }

                else if(temp->tileNum==56)
                {
                    newposition="A1";
                }

                else if(temp->tileNum==57)
                {
                    newposition="B1";
                }

                else if(temp->tileNum==58)
                {
                    newposition="C1";
                }

                else if(temp->tileNum==59)
                {
                    newposition="D1";
                }

                else if(temp->tileNum==60)
                {
                    newposition="E1";
                }

                else if(temp->tileNum==61)
                {
                    newposition="F1";
                }

                else if(temp->tileNum==62)
                {

                    newposition="G1";
                }

                else if(temp->tileNum==63)
                {
                    newposition="H1";
                }

                qDebug() << newposition;
//                connOpen();
//                QSqlQuery qry;
//                qry.prepare("select * from savegame where  position='"+newposition+"'");
//                qry.prepare("INSERT INTO userprimarydata (position,) VALUES ('"+newposition+"')");
//                qry.exec();
//                connClose();


                model->setItem(rowcount1,PieceName, new QStandardItem(piecename));
                model->setItem(rowcount1,NewPosition, new QStandardItem(newposition));
                if(model->rowCount() == 0)
                {
                    model->setRowCount(1);
                }




                click1->display(click1->pieceName);
                temp->display(click1->pieceName);

                click1->wallDisplay();
                temp->wallDisplay();
                //checks if move is illegal
                retValue=valid->check(click1);
                 //if illegal turn the square red.
                if(retValue)
                {
                    tile[wR][wC]->setStyleSheet("QLabel {background-color: red;}");
                }

//                qDebug() <<tile[bR][bC]->piece;

                disOrange();


                max=0;

                turn=1;

                //whites turn

//                player2status2->hide();
//                player1status1->show();
                count=0;

            }

        }
        sleep(double(0.4));



    }
    tableview->show();

}



void accessories(QWidget *baseWidget)
{

    QLabel *player1 = new QLabel(baseWidget);
    player1->setGeometry(100,500,100,100);
    player1->setPixmap(QPixmap(":/eachpiece/profile.png"));
    QLabel *name1 = new QLabel(player1username, baseWidget);
    name1->setGeometry(125,610,80,20);


    QLabel *player2 = new QLabel(baseWidget);
    player2->setGeometry(100,100,100,100);
    player2->setPixmap(QPixmap(":/eachpiece/profile.png"));
    QLabel *name2 = new QLabel(player2username, baseWidget);
    name2->setGeometry(125,210,80,20);
//    QLabel *time2 = new QLabel("00:00:00", baseWidget);
//    time2->setGeometry(120,235,80,20);


    QLabel *player1status1 = new QLabel(player1username + " it's your turn.",baseWidget);
    player1status1->setGeometry(180,620,150,50);
    player1status1->setStyleSheet("font-weight: bold; color: red");

    QLabel *player2status2 = new QLabel(player2username + " it's your turn.",baseWidget);
    player2status2->setGeometry(190,220,150,50);
    player2status2->setStyleSheet("font-weight: bold; color: red");

}



//void secs(QWidget *baseWidget)
//{
//     int sec;

//     QLabel *timer1 = new QLabel(baseWidget);
//     timer1->setGeometry(110,635,80,20);


//     QLabel *timer2 = new QLabel(baseWidget);
//     timer2->setGeometry(110,235,70,20);

//     qDebug () << ("Timer Started");
//     for(sec=30;sec--;)
//     {
//         if (turn==1)
//         {
//             qDebug () << ("00 : 00 : "+ QString::number(sec));
//             timer1->setText("00 : 00 : "+ QString::number(sec));
//             sleep(1);

//             if (sec==0)
//             {
//                 qDebug () << ("Timer Done");
//                 qDebug () << ("00 : 00 : 00");
//                 timer1->setText("00 : 00 : 00");
//              }
//         }


//        if (turn==0)
//        {
//            qDebug () << ("00 : 00 : "+ QString::number(sec));
//            timer2->setText("00 : 00 : "+ QString::number(sec));
//            sleep(1);

//            if (sec==0)
//            {
//                qDebug () << ("Timer Done");
//                qDebug () << ("00 : 00 : 00");
//                timer2->setText("00 : 00 : 00");
//             }
//        }

//     }



//}

void disOrange()
{
    int i;
    for(i=0;i<=max;i++)
        //changes the colour from orange to the original colour of the walls.
        tile[expw[i]/8][expw[i]%8]->wallDisplay();



}




void GameStatus::on_newgame_clicked()
{

    if (player1username.contains("") && player2username.contains(""))
    {

        QWidget *myWidget=new QWidget();
        myWidget->setGeometry(0,0,1370,700);
        accessories(myWidget);
        chessBoard(myWidget,tile);
        buttongoback(myWidget);
        savebutton(myWidget);
        HowtoPlayInfo(myWidget);
//        connClose();
//        connOpen();
        myWidget->show();
        this->hide();
//        secs(myWidget);
//        sleep(1);
    }

}


void GameStatus::on_goback_clicked()
{
    this->hide();
    loginuser *login;
    login = new loginuser(this);
    login->show();
}
