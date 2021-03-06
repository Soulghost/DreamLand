//
//  DragonBaseModel.h
//  Legend
//
//  Created by soulghost on 26/12/2017.
//
//

#ifndef DragonBaseModel_H
#define DragonBaseModel_H

#include "cocos2d.h"
#include "ValueDisplayNode.h"
#include "DragonDisplayNode.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "SGPlayer.h"

USING_NS_CC;
using namespace std;

class SGValueBar;
class SGSkill;

typedef function<void ()> EventCallback;
typedef function<void (float)> FloatCallback;

typedef struct DragonActionAlias {
    string attackName;
    string idleName;
    string walkName;
    string deathName;
    string conjureName;

    DragonActionAlias() {
        attackName = "attack";
        idleName = "idle";
        walkName = "walk";
        deathName = "death";
        conjureName = "conjure";
    }
    
    DragonActionAlias(const string &_idleName, const string &_walkName, const string &_attackName) {
        idleName = _idleName;
        walkName = _walkName;
        attackName = _attackName;
    }
    
    DragonActionAlias(const string &_idleName, const string &_walkName, const string &_attackName, const string &_deathName) {
        idleName = _idleName;
        walkName = _walkName;
        attackName = _attackName;
        deathName = _deathName;
    }
    
    DragonActionAlias(const string &_idleName, const string &_walkName, const string &_attackName, const string &_deathName, const string &_conjureName) {
        idleName = _idleName;
        walkName = _walkName;
        attackName = _attackName;
        deathName = _deathName;
        conjureName = _conjureName;
    }
} DragonActionAlias;

typedef enum ModelPosition {
    ModelPositionLeft = 0,
    ModelPositionRight = 1
} ModelPosition;

typedef enum ModelState {
    ModelStateIdle = 0,
    ModelStateWalk = 1,
    ModelStateDeath = 2,
    ModelStateConjure = 3,
    ModelStateAttack = 4
} ModelState;

class DragonBaseModel : public Ref {
public:
    string dataJSONPath;
    string textureJSONPath;
    string armatureName;
    DragonActionAlias actionAlias;
    
    bool operator = (DragonBaseModel *other) const {
        return _modelPosition == other->getModelPosition() && _modelNum == other->getModleNum();
    }
    
#pragma mark - battle data
    SGPlayer *_player;
    ModelState _modelState;
    
    DragonBaseModel();
    ~DragonBaseModel();
    virtual bool init();
    CREATE_FUNC(DragonBaseModel);
    
    DragonDisplayNode* getDisplayNode();
    void startAnimating();
    void playAnimationNamed(string name, unsigned int times, EventCallback finishCallback = nullptr);
    void playSkill(SGSkill *skill, float afterDelay = -1, EventCallback finishCallback = nullptr);
    
    // data
    void bindWithPlayer(SGPlayer *player);
    
    // location
    void setPosition(float x, float y);
    void setPosition(const Vec2& pos);
    
    // calculate
    void markOriginPosition();
    void markOriginLeftScale();
    Vec2 getOriginPosition();
    Vec2 getAttackPosition();
    
#pragma mark - Actions
    // move
    FiniteTimeAction* moveToAction(DragonBaseModel *destModel);
    FiniteTimeAction* transportToAction(DragonBaseModel *destModel);
    FiniteTimeAction* moveBackAction();
    // attack
    float doAttack();
    FiniteTimeAction* attackAction(FloatCallback startCallback = nullptr);
    // conjure
    FiniteTimeAction* conjureAction(FloatCallback startCallback);
    // 受到攻击的后仰和伤害作用
    void backwardInDelays(float seconds, EventCallback callback);
    void sufferAttackWithValue(AttackValue value, float afterDelay);
    void sufferAttackWithValue(AttackValue value, float afterDelay, EventCallback callback);
    void protectTarget(DragonBaseModel *target, AttackValue value, float afterDelay);
    void protectedByTarget(DragonBaseModel *target, AttackValue value, float afterDelay);
    // 直接数值处理
    void underHealWithValue(AttackValue value, float afterDelay = -1);
    void underHurtWithValue(AttackValue value, float afterDelay = -1);
    void resurgeWithValue(AttackValue value, float afterDelay = -1);
    // buff附加
    void addBuffAfterDelay(SGBuff *buff, float afterDelay = -1);
    
#pragma mark - Action Descriptions
    float durationForAttack();
    float durationForConjure();
    
#pragma mark - Indicator
    void showSkillNamed(const string &skillName);
    void showHitAnimation(Animate *animate, float scale, float delay);
    
#pragma mark - Shortcuts
    void runAction(Action *action);
    
#pragma mark - getter & setter
    void setModelPosition(ModelPosition modelPosition);
    ModelPosition getModelPosition() { return _modelPosition; }
    
    void setModelNum(int modelNum) { _modelNum = modelNum; }
    int getModleNum() { return _modelNum; }
    
    void setModelLocation(ModelPosition position, int num);
    
#pragma mark - Battle Operation
    void renderPlayerData();
    void setState(ModelState modelState);
    
    dragonBones::DragonBonesData *_dragonBonesData;
    dragonBones::Armature *_armature;
    dragonBones::CCFactory _dragonFactory;
    dragonBones::CCArmatureDisplay *_armatureDisplay;
    DragonDisplayNode *_displayNode;
    
#pragma mark - UI
    SGValueBar *_hpBar;
    SGValueBar *_mpBar;
    Label *_nameLabel;
    float nameLabelOffsetY;
    
    Vec2 _originPosition;
    Vec2 _originLeftScale;
    Vec2 _skillPosition;
    float _modelHeight;
    float _attackBackwardRatio;
    float _skillConjureRatio;
    
    void initWithInfo(string dataJSONPath, string textureJSONPath, string armatureName);
    void initWithInfo(string dataJSONPath, string textureJSONPath, string armatureName, DragonActionAlias actionAlias);
    
private:
    // direction control
    ModelPosition _modelPosition;
    int _modelNum;
    
    void commonInit();
    void setupViews();
};

#endif
