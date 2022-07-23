#pragma once

#include <Windows.h>
#include <string_view>

namespace Near{

void throwResult(const char* msg, HRESULT res);

template<class T>
void safeRelease(T*& thing){
  if(thing){
    thing->Release();
    thing = nullptr;
  }
}

template<class T>
void safeDestroy(T*& thing){
  if(thing){
    thing->Destroy();
    thing = nullptr;
  }
}

template<class T>
void safeUninitDelete(T*& thing){
  if(thing){
    thing->uninit();
    delete thing;
    thing = nullptr;
  }
}

template<class T>
void safeDelete(T*& thing){
  if(thing){
    delete thing;
    thing = nullptr;
  }
}

template<class T>
void safeDeleteArray(T*& thing){
  if(thing){
    delete[] thing;
    thing = nullptr;
  }
}

// 0なら0、負なら-1、正なら1を返します。
constexpr int sign(float v){
  return v > 0 ? 1 : v < 0 ? -1 : 0;
}

template<typename T>
T parseInt(std::string_view str, int base = 10){
  T val;
  auto res = std::from_chars(str.data(), str.data() + str.size(), val, base);
  if(res.ec == std::errc::invalid_argument) throw std::invalid_argument("Not a number: " + std::string(str));
  if(res.ec == std::errc::result_out_of_range) throw std::out_of_range("Number out of range: " + std::string(str));
  return val;
}

template<typename T>
T parseFloat(std::string_view str){
  T val;
  auto res = std::from_chars(str.data(), str.data() + str.size(), val);
  if(res.ec == std::errc::invalid_argument) throw std::invalid_argument("Not a number: " + std::string(str));
  if(res.ec == std::errc::result_out_of_range) throw std::out_of_range("Number out of range: " + std::string(str));
  return val;
}

// ワイド文字列をUTF-8文字列に変換します。
// Windows系APIの戻り値変換にでも使ってね。
std::string narrow(std::wstring_view str);

// UTF-8文字列をワイド文字列に変換します。
// Windows系APIを呼ぶときに使ってね。
std::wstring widen(std::string_view str);

}
