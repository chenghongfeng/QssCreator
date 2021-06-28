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

    Singleton(const Singleton &) = delete;
    void operator=(const Singleton &) = delete;

protected:
    //
    Singleton() {  }
    ~Singleton() {  }

private:

};

// 单体模板---基于QObject类派生，用这个
template<typename T>
class  CxSingleton_Object
{
public:
    static T* singleton(QObject *parent)
    {
        if (instance() == 0)
        {
            instance() = new T(parent);
        }
        return instance();
    }
    static T*& instance()
    {
        static T *t = 0;
        return t;
    }

    static void close_singleton()
    {
        if (instance() != 0)
        {
            instance()->deleteLater();
            instance() = 0;
        }
    }

    // 设置实例
    void setImpl(T*impl){
        instance() =  impl;
    }

protected:
    //
    CxSingleton_Object() { ; }
    ~CxSingleton_Object() { ; }

private:

};


// 单体模板---基于QWidget类派生，用这个
template<typename T>
class  CxSingleton_Widget
{
public:
    static T* singleton(QWidget *parent)
    {
        if (instance() == 0)
        {
            instance() = new T(parent);
        }
        return instance();
    }
    static T*& instance()
    {
        static T *t = 0;
        return t;
    }

    static void close_singleton()
    {
        if (instance() != 0)
        {
            instance()->deleteLater();
            instance() = 0;
        }
    }

    // 设置实例
    void setImpl(T*impl){
        instance() =  impl;
    }

protected:
    //
    CxSingleton_Widget() { ; }
    ~CxSingleton_Widget() { ; }

private:

};
#endif // SINGLETON_H
