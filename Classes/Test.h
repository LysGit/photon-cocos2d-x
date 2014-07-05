//
//  Test.h
//  photonTest
//
//  Created by lys on 14-7-3.
//
//

#ifndef __photonTest__Test__
#define __photonTest__Test__

#define NF_SERVICE_STATUS_CHANGE         "Notification_service_status_change"

typedef enum : int {
    //连接状态
    NF_Photon_DisConnect,
    NF_Photon_Connecting,
    NF_Photon_Connected,
    NF_Photon_ConnectFailed,
    
    //登录状态
    NF_Photon_LoginSuccess,
    NF_Photon_LoginFailed,
    
} PhotonStatusCode;


class Test {
public:
    Test();
};

#endif /* defined(__photonTest__Test__) */
