#include "pch.h"
#include <NearLib/utils.h>

#include <string>

namespace Near{

void throwResult(const char* msg, HRESULT res){
  LPWSTR errorText = nullptr;
  DWORD formatRes = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, res, 0, reinterpret_cast<LPWSTR>(&errorText), 0, nullptr);
  std::string exMsg = msg;
  exMsg.append(" (HRESULT ").append(std::to_string(res));
  if(SUCCEEDED(formatRes)){
    exMsg.append(" ").append(narrow(errorText)).append(")");
  }else{
    exMsg.append(" Unknown error)");
  }
  if(errorText){
    LocalFree(errorText);
  }
  throw std::exception(exMsg.c_str());
}

std::string narrow(std::wstring_view str){
  int u8Len = WideCharToMultiByte(CP_UTF8, 0, str.data(), str.size(), nullptr, 0, NULL, false);
  std::string u8;
  u8.resize(u8Len);
  WideCharToMultiByte(CP_UTF8, 0, str.data(), str.size(), u8.data(), u8Len, NULL, false);
  return u8;
}

std::wstring widen(std::string_view str){
  int wideLen = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), nullptr, 0);
  std::wstring wide;
  wide.resize(wideLen);
  MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), wide.data(), wideLen);
  return wide;
}

}
