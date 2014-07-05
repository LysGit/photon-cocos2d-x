//
//  Test.cpp
//  photonTest
//
//  Created by lys on 14-7-3.
//
//

#include "Test.h"
#include "LitePeer.h"

#include <thread>
#include <iostream>
#include <mutex>

#include "cocos2d.h"

using namespace ExitGames::Photon;
using namespace ExitGames::Common;
using namespace ExitGames::Photon::StatusCode;
using namespace std;

/*
class TestConnection : public PhotonListener {
public:
    
    TestConnection(){}
    
    void updateService(){
        while ( true ) {
            m_pLitePeer->service();
            sleep( 1 );
        }
    }
    
    LitePeer * m_pLitePeer;
    
    void Start(){
        m_pLitePeer = new LitePeer(*this);
        m_pLitePeer->connect( "192.168.4.108:5055", (unsigned char*)"MyServer");
        
        std::thread t1(&TestConnection::updateService,this);
//        t1.join();
        t1.detach();
        
    }

    
    void debugReturn(DebugLevel::DebugLevel debugLevel, const JString& string)
    {
        printf("\n\n debugReturn");
    }
    
    void onOperationResponse(const OperationResponse& operationResponse)
    {
        printf("\n\n onOperationResponse");
    };
    void onStatusChanged(int statusCode)
    {
        switch(statusCode){
            case CONNECT:
                printf("\n Connect Success!");
                break;
            case DISCONNECT:
                printf("\n Disconnect!");
                break;
        }
    };
    void onEvent(const EventData& eventData)
    {
        printf("\n\n onEvent");
    };
};
*/

typedef enum : Byte {
    DisConnect,
    Connecting_,
    Connecting,
    Connected_,
    Connected,
    ConnectFailed_,
    ConnectFailed,
    LoginSuccess,
    LoginFailed
}ClientState;

typedef enum : Byte {
    //login
    kLogin = 249,
    kLoginSuccess = 248,
    kLoginFailed = 247,
    
    //room
    kCreate = 250,
    kJoin = 255,
    kLeave = 254,
    kRaiseEvent = 253,
    kSetProperties = 252,
    kGetProperties = 251
} OpCodeEnum;

typedef enum : Byte {
    RoomId = 251,
    UserName = 252,
    PassWord = 253
} OpKeyEnum;


class TestConnection : public PhotonListener {
public:
    TestConnection();
    ~TestConnection(){}

    void Start();
    void updateService();
    void dispatcher();
    
    //委托事件
    void debugReturn(DebugLevel::DebugLevel debugLevel, const JString& string);
    void onOperationResponse(const OperationResponse& operationResponse);
    void onStatusChanged(int statusCode);
    void onEvent(const EventData& eventData);
    
private:
    PhotonPeer * m_pPhotonPeer;
    ClientState m_state;

    string m_address;
    string username;
    string password;
};

TestConnection::TestConnection(){
    
    m_pPhotonPeer = new PhotonPeer(*this);
    m_state = DisConnect;
    m_address = "192.168.4.108:5055";
    username = "";
    password = "";
}

void TestConnection::Start(){

    std::thread t2(&TestConnection::dispatcher,this);
    t2.detach();
    
}
void TestConnection::updateService(){
    while ( true ) {
        m_pPhotonPeer->service();
        sleep( 1 );
    }
}

void TestConnection::dispatcher(){
    
    while ( true ) {
        
        switch ( m_state ) {
            case DisConnect:
            {
                //推送通知修正界面 -- 点击连接
                cocos2d::CCNotificationCenter::getInstance()->postNotification( NF_SERVICE_STATUS_CHANGE, (cocos2d::Ref *)NF_Photon_DisConnect );
                
                //修改连接状态
                m_state = Connecting_;
                
                //链接
                m_pPhotonPeer->connect( m_address.c_str(), (unsigned char*)"MyServer");
                
                //开启线程链接监测
                std::thread t1(&TestConnection::updateService,this);
                t1.detach();
            }
                break;
                
                case Connecting_:
            {
                //推送通知修正界面 -- 连接提示界面
                cocos2d::CCNotificationCenter::getInstance()->postNotification( NF_SERVICE_STATUS_CHANGE, (cocos2d::Ref *)NF_Photon_Connecting );
                
                //修改连接状态
                m_state = Connecting;
            }
                break;
                
                case Connected_:
            {
                //推送通知修正界面 -- 连接成功界面
                cocos2d::CCNotificationCenter::getInstance()->postNotification( NF_SERVICE_STATUS_CHANGE, (cocos2d::Ref *)NF_Photon_Connected );
                
                //修改连接状态
                m_state = Connected;
            }
                break;
            case ConnectFailed_:
            {
                //推送通知修正界面 -- 连接失败界面
                cocos2d::CCNotificationCenter::getInstance()->postNotification( NF_SERVICE_STATUS_CHANGE, (cocos2d::Ref *)NF_Photon_ConnectFailed );
                
                //修改连接状态
                m_state = ConnectFailed;
            }
                break;
            default:
                break;
        }
        
    }
}

#pragma mark - IPhotonPeerListener implementation
void TestConnection::debugReturn(DebugLevel::DebugLevel debugLevel, const JString& string)
{
    printf("\n\ndebugReturn\n");
}

void TestConnection::onOperationResponse(const OperationResponse& operationResponse)
{
    printf("\n\nonOperationResponse\n");
};
void TestConnection::onStatusChanged(int statusCode)
{
    switch(statusCode){
        case CONNECT:
        {
            printf("\nConnect Success!\n");
            
            //修改连接状态
            m_state = Connected_;
        }
            break;
        case DISCONNECT:
        {
            printf("\nDisconnect!\n");
            
            //修改连接状态
            m_state = ConnectFailed_;
        }
            break;
    }
};
void TestConnection::onEvent(const EventData& eventData)
{
    printf("\n\nonEvent\n");
};

Test::Test(){
    
    TestConnection *t = new TestConnection( );
    t->Start();
}
