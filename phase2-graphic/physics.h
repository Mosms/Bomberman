#ifndef _PHYSICS_H_
#define _PHYSICS_H_
//此为最基础模拟物理，让人物移动受限的类，如果挂载有这个类，就可以让人物无法通过或移动
#include <component.h>

class Physics : public Component {
  public:
    Physics();

    bool CannotGo();
//这个函数其实没有什么用到的地方（），但是仍属聊胜于无
//判断不判断其实不重要，重要的是有没有可以判断的东西//没有Physics的object便相应地不能阻挡人物/炸弹的前进
};

#endif  // _PHYSICS_H_
