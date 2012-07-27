#ifndef  _TEXT_INPUT_HELPER_HPP_
# define  _TEXT_INPUT_HELPER_HPP_

# include <cocos2d.h>
# include <CCTextFieldTTF.h>
# include <CCSprite.h>

struct TextInput
{
    cocos2d::CCTextFieldTTF *input;
    cocos2d::CCSprite *bg;

    const char * get_string(void)
    {
        return input->getString();
    }

    void create_input(cocos2d::CCLayer *parent, const cocos2d::CCPoint &position, const cocos2d::CCSize &size, cocos2d::CCTextAlignment alignment, const char *placeholder, const char *fontName, float fontSize)
    {
        _place_holder = placeholder;

        unsigned char color[] = {100, 100, 100};
        _texture.initWithData(color, cocos2d::kCCTexture2DPixelFormat_RGB888, 1, 1, size);
        
        bg = cocos2d::CCSprite::spriteWithTexture(&_texture);
        bg->setPosition(position);
        parent->addChild(bg, 1);

        input = cocos2d::CCTextFieldTTF::textFieldWithPlaceHolder(_place_holder, size, alignment, fontName, fontSize);
        input->setPosition(position);
        input->setColorSpaceHolder(cocos2d::ccc3(150, 150, 150));
        input->setColor(cocos2d::ccc3(250, 250, 250));
        parent->addChild(input, 2);
    }

    void on_click(const cocos2d::CCPoint &touch_point)
    {
        cocos2d::CCRect rect = getRect(bg);
        if (cocos2d::CCRect::CCRectContainsPoint(rect, touch_point))
        {
            input->setPlaceHolder("");
            input->attachWithIME();
        }
        else
        {
            input->setPlaceHolder(_place_holder);
            input->detachWithIME();
        }
    }

private:

    static cocos2d::CCRect getRect(cocos2d::CCNode * pNode)
    {
        cocos2d::CCRect rc;
        rc.origin = pNode->getPosition();
        rc.size = pNode->getContentSize();
        rc.origin.x -= rc.size.width / 2;
        rc.origin.y -= rc.size.height / 2;
        return rc;
    }

    cocos2d::CCTexture2D _texture;
    const char *_place_holder;
};

#endif //_TEXT_INPUT_HELPER_HPP_
