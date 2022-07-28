#pragma once

#include "game-object.h"

namespace Near{

// `GameObjectCollection::ofType`で使用される、
// 特定のクラスかそのサブクラス以外をすっ飛ばすイテレーターです。
template<class _Object, class _Iterator>
class GameObjectIteratorOfType{
public:
  GameObjectIteratorOfType(_Iterator it, _Iterator end) : it(it), end(end){
    advance();
  }
  GameObjectIteratorOfType& operator++(){
    it ++;
    advance();
    return *this;
  }
  bool operator!=(const GameObjectIteratorOfType& other){
    return it != other.it;
  }
  std::shared_ptr<_Object> operator*(){
    return std::static_pointer_cast<_Object>(*it);
  }
private:
  _Iterator it;
  _Iterator end;
  void advance(){
    while(it != end){
      if(std::dynamic_pointer_cast<_Object>(*it)) break;
      it ++;
    }
  }
};

// `GameObjectCollection::ofExactType`で使用される、
// 特定のクラス以外をすっ飛ばすイテレーターです。
template<class _Object, class _Iterator>
class GameObjectIteratorOfExactType{
public:
  GameObjectIteratorOfExactType(_Iterator it, _Iterator end) : it(it), end(end){
    advance();
  }
  GameObjectIteratorOfExactType& operator++(){
    it ++;
    advance();
    return *this;
  }
  bool operator!=(const GameObjectIteratorOfExactType& other){
    return it != other.it;
  }
  std::shared_ptr<_Object> operator*(){
    return std::static_pointer_cast<_Object>(*it);
  }
private:
  _Iterator it;
  _Iterator end;
  void advance(){
    while(it != end){
      if(typeid(_Object) == typeid(**it)) break;
      it ++;
    }
  }
};

template<class _Object, class _Iterator>
class GameObjectCollectionOfType{
public:
  GameObjectCollectionOfType(_Iterator begin, _Iterator end) : _begin(begin), _end(end){
  }
  GameObjectIteratorOfType<_Object, _Iterator> begin(){
    return GameObjectIteratorOfType<_Object, _Iterator>(_begin, _end);
  }
  GameObjectIteratorOfType<_Object, _Iterator> end(){
    return GameObjectIteratorOfType<_Object, _Iterator>(_end, _end);
  }
private:
  _Iterator _begin;
  _Iterator _end;
};

template<class _Object, class _Iterator>
class GameObjectCollectionOfExactType{
public:
  GameObjectCollectionOfExactType(_Iterator begin, _Iterator end) : _begin(begin), _end(end){
  }
  GameObjectIteratorOfExactType<_Object, _Iterator> begin(){
    return GameObjectIteratorOfExactType<_Object, _Iterator>(_begin, _end);
  }
  GameObjectIteratorOfExactType<_Object, _Iterator> end(){
    return GameObjectIteratorOfExactType<_Object, _Iterator>(_end, _end);
  }
private:
  _Iterator _begin;
  _Iterator _end;
};

template<class _Iterator>
class GameObjectCollection{
public:
  virtual _Iterator begin() = 0;
  virtual _Iterator end() = 0;
  // 同じクラスかサブクラスの最初のインスタンスを探します。
  // サブクラスのインスタンスを無視できる場合、より高速に動作する`firstOfExactType()`がおすすめです。
  // @return 見つかった最初のオブジェクト。なかったら`nullptr`
  template<class T>
  std::shared_ptr<T> firstOfType(){
    auto collection = ofType<T>();
    auto begin = collection.begin();
    auto end = collection.end();
    return begin != end ? *begin : nullptr;
  }
  // 同じクラスの最初のインスタンスを探します。
  // サブクラスのインスタンスを無視する分高速に動作します。
  // サブクラスも必要な場合は`firstOfType()`をどうぞ。
  // @return 見つかった最初のオブジェクト。なかったら`nullptr`
  template<class T>
  std::shared_ptr<T> firstOfExactType(){
    auto collection = ofExactType<T>();
    auto begin = collection.begin();
    auto end = collection.end();
	return begin != end ? *begin : nullptr;
  }
  // 同じクラスかサブクラスの全てのインスタンスを探します。
  // サブクラスのインスタンスを無視できる場合、より高速に動作する`ofExactType()`がおすすめです。
  // @return 見つかったオブジェクトでforeachを回せる物体
  // @example
  // for(auto enemy : objects.ofType<Enemy>()){ ... }
  // @see ofExactType()
  template<class T>
  GameObjectCollectionOfType<T, _Iterator> ofType(){
    return GameObjectCollectionOfType<T, _Iterator>(begin(), end());
  }
  // 同じクラスのインスタンスを探します。
  // サブクラスのインスタンスを無視する分高速に動作します。
  // サブクラスも必要な場合は`ofType()`をどうぞ。
  // @return 見つかったオブジェクトでforeachを回せる物体
  // @see ofType()
  template<class T>
  GameObjectCollectionOfExactType<T, _Iterator> ofExactType(){
    return GameObjectCollectionOfExactType<T, _Iterator>(begin(), end());
  }
};

}
