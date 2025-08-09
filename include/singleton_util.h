#pragma once
#include <iostream>

/**
 * @brief 单例模式模板类
 * @tparam T 单例类类型
 * @details 该类提供了一个静态方法 `GetInstance` 用于获取单例实例。
 *          禁止拷贝构造、移动构造和赋值操作符，以确保单例的唯一性。
 *          使用时需要继承该类并实现具体的单例类。
 */
template <class T>
class Singleton {
   public:
    static T* GetInstance() {
        static T instance;
        return &instance;
    }

   protected:
    Singleton()          = default;
    virtual ~Singleton() = default;

   private:
    Singleton(Singleton&&)                 = delete;
    Singleton(const Singleton&)            = delete;
    Singleton& operator=(Singleton&&)      = delete;
    Singleton& operator=(const Singleton&) = delete;
    // T*         operator&()                 = delete;
};