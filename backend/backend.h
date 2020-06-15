#ifndef BACKEND_H
#define BACKEND_H

#include "aithread.h"
#include "netthread.h"
#include "cards.h"
#include <qvector.h>
#include <qlist.h>
#include <playerthread.h>
#include <ctime>


/*
    ����ࣺ������ǰ�˽��л�ȡ״̬��Ϣ�ͽ��в�����
    ÿ��������Ҷ���һ���࣬��backend��������ǰ�˺ͺ��Ψһ�Ľӿھ�������ࡣ
    ��ǰ��ÿ����������ʾ����֮����ú���

    getCurrentStatue();

    ��ȡ��ǰ״̬��ע�⣬���û���κβ������ܸı�״̬�Ļ����ú���Ҳ��ı�״̬�����������ƣ���������ǰ��ң�
    ��Ҫÿ�ε��ù�getCurrentStatue()���������β鿴������Ϣ�Ƿ��и���

    ״̬ flag��
        -2    Error
        -1      ������ʱ��ʼ������Ϸ׼��״̬���ȴ���������;
        0       ������Ϸ����ʼ���ƣ�
        1       ��ǰ��ҿɽ��в�����ѡ�����ƻ���ƣ�
        2       ��ǰ�����ѡ�����ƣ��������ƣ�
        3       ��ǰ��������ƺ����ѡ����ƣ������ܴ��ڣ�
        4       ��ǰ�����ѡ����ƣ��������ƺ��ƶѣ��������������ɫ�ƣ���Ҫ��ǰ��ѡ����ɫ����һ���ֽ���ǰ���жϣ�
        ��2 - 4����غϽ�����
        ������һ����ң�����غϿ�ʼǰ����ʾ��
        5       ��һ��������˽�UNO���������ɣ�
        6       �����غ�
        7       ��Ҫ����n�Ų������غ�
        8       ��Ϸʤ��
        9       ��Ϸʧ��


        ���ֹ���
            ���Լ�Ϊ��һ��������ң�
            Ŀǰ��֧����������+N�ƣ�
            ����ѡ�����ƣ�����Ҫû���Ƶ�ʱ���ٽ��У�ѡ��������֮����������ϵ����ܹ����ƣ���������ѡ����ƣ�
 */

enum flags { Error = -2, Init = -1, Start, Opera, Draw, DrawOpera, Put, ForgetUNO, Skip, DrawN, Win, Lost  };

class Backend : public QObject{
    Q_OBJECT
        QThread workerThreads[7];
private:

    bool usingAI;

    QVector<playerThread*> Players;
    QVector<Card*> cards;

    QList<Card*> queueTop;

    int playerCount;
    int netPlayerCount;
    int AIPlayerCount;

    flags currentStatue;
    int currentTurnId;
    int direction;

    bool unoflag;
    bool drawNflag;
    bool skipFlag;

    Card::COLOR currentColor;
    int nextDrawNum;

    // ��������
    Card* randomCardFromStack();                    // ���ƶ������ץһ��
    void getNextPlayer();                           // �ı�currentTurnId
    void setAllFlagsFalse();                        // ��λ���е�flag
    int getTopNum();

    void callAIforOp();

public:
    Backend(int playerNum, QString myName, bool isAI = false);         // single player
    Backend(int maxPlayerNum,int AInum, QString myName);       // net players
    ~Backend();

    // for the front to get current statue
    flags getCurrentStatue();     // return statue flag
    int getCurrnetTurnID();
    bool getPlayerCards(int id, QVector<Card*>& mycards);
    bool getPlayerCards(QVector<Card*>& cards);        // �����ҵ����ƣ�����ʾ��
    bool getPlayerValidCards(int id, QVector<int>& cardIDs);
    bool getPlayerValidCards(QVector<int>& cardIDs);     // �����ҿ��Գ��Ƶ����
    bool getTopFiveCards(QVector<Card*> &cards);        // �����ƶѶ��ϵ�������
    int getPlayerCardCount(int playerID);               
    QString getPlayerName(int playerID);            

    int countPoints(int playerID);                      // �����ʱ��������

public slots:
    // ǰ�˵���
    void startGame();
    void sayUNO();
    void playCard(int cardID,Card::COLOR color);
    void drawCard();


signals:
    // ��AI��
    void StartMove(int id);
    //��ǰ��
    void MoveEnds();
};

class StatueERROR {
public:
    StatueERROR() {
        this->errorMessage = "";
    }
    StatueERROR(QString errorMessage) {
        this->errorMessage = errorMessage;
        this->type = "statue";
   }
   QString errorMessage;
   QString type;
};

class StackNoCardError:public StatueERROR {
public:
    StackNoCardError() {
        this->type = "StackNoCardError";
        this->errorMessage = "StackNoCard";
    }
    StackNoCardError(QString errorMessage){
        this->type = "StackNoCardError";
        this->errorMessage = errorMessage;
    }
};

#endif // BACKEND_H
