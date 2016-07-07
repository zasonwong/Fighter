//
//  PreStartScene.hpp
//  Bullet
//
//  Created by zason on 7/5/16.
//
//

#ifndef PreStartScene_hpp
#define PreStartScene_hpp

#include <stdio.h>
#include "cocos2d.h"

class PreStartScene : public cocos2d::Layer
{
public:
    
    PreStartScene();
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PreStartScene);
    
    static PreStartScene* pInstance;
    
    static void onUpdateCompeteName(std::string name);
    static void onStartGame();
    
private:
    
    void gameReadyCallback(Ref* pSender);
    void menuCloseCallback(Ref* pSender);
    
    void startGame(float dt);
    void startStep(float dt);
    
    cocos2d::Sprite* playerBSprite;
    cocos2d::Label* playerBLabel;
    cocos2d::Label* waitingLabel;
    
};

#endif /* PreStartScene_hpp */
