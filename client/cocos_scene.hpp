#ifndef __COCOS_SCENE_HPP__
# define __COCOS_SCENE_HPP__

# include "master.hpp"

# include <cocos2d.h>

class CocosScene : public cocos2d::CCScene
{
public:
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // game loop
    void tick(cocos2d::ccTime dt);

    // stop all subsystems and exit game
    void end_scene();

    SCENE_NODE_FUNC(CocosScene);

private:

    Master _master;
};

#endif // __COCOS_SCENE_HPP__
