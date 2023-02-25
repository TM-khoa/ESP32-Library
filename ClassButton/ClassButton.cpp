 /**
 * @file ClassButton.cpp
 * @author Spirit Boi (minhkhoa9a31314@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "stdint.h"

typedef void (*BtnCallback)(void *arg);

class ClassButton
{
private:
    /* data */
    uint8_t delay_ms;
    bool HoldPush;
public:
    ClassButton(/* args */);
    ~ClassButton();
};

ClassButton::ClassButton(/* args */)
{

}

ClassButton::~ClassButton()
{
}
