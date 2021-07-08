#ifndef SINGLETON_H
#define SINGLETON_H
#include <QWidget>

// 单体模板
template<typename T>
class Singleton
{
public:

    static T*& getInstance()
    {
        static T *instance = new T();
        return instance;
    }
    static void closeInstance(){
        if (getInstance() != nullptr)
        {
            delete getInstance();
            getInstance() = nullptr;
        }
    }

    Singleton(const Singleton &) = delete;
    void operator=(const Singleton &) = delete;

protected:
    //
    Singleton() {  }
    ~Singleton() {  }

private:

};


#endif // SINGLETON_H
