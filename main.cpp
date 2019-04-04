#include "IOCContainer.h"

#include <iostream>
#include <string>
#include <memory>

class A
{
    public:
        A(std::string stuff):m_stuff(stuff){}
        virtual ~A(){}
        virtual std::string GetStuff() const {return m_stuff;}
    private:
        std::string m_stuff;
};

class C : public A
{
    public:
        C(std::string stuff):A(stuff){}
};

class B
{
    public:
        B():m_default((IOCContainer::getInstance())->Resolve<C>()),
            m_base1(IOCContainer::getInstance()->Resolve<A>()),
            m_base2(IOCContainer::getInstance()->Resolve<A>(&typeid(A))),
            m_idTest1(IOCContainer::getInstance()->Resolve<A>("IdTest1")),
            m_idTest2(IOCContainer::getInstance()->Resolve<A>("IdTest2"))
    {
        std::cout << "-----------------------------------------------" << std::endl;
        std::cout << m_default->GetStuff() << std::endl;
        std::cout << m_base1->GetStuff() << std::endl;
        std::cout << m_base2->GetStuff() << std::endl;
        std::cout << m_idTest1->GetStuff() << std::endl;
        std::cout << m_idTest2->GetStuff() << std::endl;
    }
    private:
        std::shared_ptr<C> m_default;
        std::shared_ptr<A> m_base1;
        std::shared_ptr<A> m_base2;
        std::shared_ptr<A> m_idTest1;
        std::shared_ptr<A> m_idTest2;
};

IOCContainer* IOCContainer::uniqueIns = NULL;

int main()
{
    (IOCContainer::getInstance())->Register(std::make_shared<C>("DefaultTest"));
    IOCContainer::getInstance()->Register(&typeid(A),std::make_shared<C>("BaseTest"));
    IOCContainer::getInstance()->Register("IdTest1",std::make_shared<C>("IdTest1"));
    IOCContainer::getInstance()->Register("IdTest2",std::make_shared<C>("IdTest2"));

    B b;
}
