//
//  GameScene.hpp
//  Bullet
//
//  Created by zason on 7/1/16.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "Player.hpp"
#include "EnemyPlane.hpp"

typedef enum{

    E_SINGLE,
    E_ONLINE
    
}eGameType;

class GameScene : public cocos2d::Layer
{
public:
    
    eGameType gameType;
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void update(float delta);
    
    CREATE_FUNC(GameScene);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    void startStep(float dt);
    void startGame();
    
    static GameScene* pInstance;
    static void onUpdatePosition(float x, float y);
    
    static void setGameType(eGameType type){ pInstance -> gameType = type; };
    
private:
    
    int countDown;
    bool isGameOver;

    Player *playerA; // 玩家A飞机
    Player *playerB; // 玩家B飞机
    
    cocos2d::Sprite *bullet;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::Menu *overMenu;
    
    int bulletSpeed;
    
    GameScene();
    
    void initData();
    void loadPlayer();
    void loadBackground();
    void loadBullet();
    void loadEnemyPlane(float delta);
    void updateBullet();
    void updateEnemyPlane(float delta);
    void resetBullet();
    void collisionDetect();
    void gameOver();
    void playPlaneBlowupAnimation();
    void clear();
    void restart(cocos2d::Ref *pSender);
    
    cocos2d::Vec2 limitPosition(cocos2d::Vec2 newPos);
    cocos2d::Vector<EnemyPlane *> enemyVector;
    
};

#endif /* GameScene_hpp */
